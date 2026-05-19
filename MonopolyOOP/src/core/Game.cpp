#include "core/Game.h"
#include "core/GameBoard.h"
#include "core/Dice.h"
#include "players/Player.h"
#include "players/HumanPlayer.h"
#include "players/AIPlayer.h"
#include "tiles/Tile.h"
#include "tiles/PropertyTile.h"
#include "tiles/InheritanceTile.h"
#include "tiles/ChanceTile.h"
#include "items/ItemManager.h"
#include "questions/QuestionBank.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

Game::Game(QObject *parent) : QObject(parent) {
    m_board = new GameBoard(this);
    m_dice = new Dice(this);
    m_questionBank = new QuestionBank(this);
    m_itemManager = new ItemManager(this);
    m_aiTimer = new QTimer(this);

    connect(m_aiTimer, &QTimer::timeout, this, &Game::handleAITurn);
}

Game::~Game() {
    for (Player* p : m_players) {
        delete p;
    }
    m_players.clear();
}

void Game::initialize(GameMode mode, GameDifficulty diff) {
    m_mode = mode;
    m_difficulty = diff;
    m_currentPlayerIndex = 0;
    m_currentRound = 1;
    m_state = SETUP;

    // 清理旧玩家
    for (Player* p : m_players) {
        delete p;
    }
    m_players.clear();

    // 初始化地图
    m_board->initializeDefaultBoard();

    // 设置玩家
    setupPlayers();

    emit messageLogged("游戏初始化完成！");
    emit messageLogged(QString("模式: %1, 难度: %2")
                       .arg(mode == SINGLE_PLAYER ? "单人" : mode == TWO_PLAYER ? "双人" : "练习")
                       .arg(diff == EASY ? "简单" : diff == NORMAL ? "普通" : "困难"));
}

void Game::setupPlayers() {
    int playerMoney = 1500;
    int aiMoney = 1500;

    switch (m_difficulty) {
    case EASY:
        playerMoney = 2000;
        aiMoney = 1000;
        break;
    case NORMAL:
        playerMoney = 1500;
        aiMoney = 1500;
        break;
    case HARD:
        playerMoney = 1000;
        aiMoney = 2000;
        break;
    }

    // 创建人类玩家
    HumanPlayer* human = new HumanPlayer("玩家", this);
    human->addFunds(playerMoney);
    m_players.append(human);

    if (m_mode == TWO_PLAYER) {
        HumanPlayer* human2 = new HumanPlayer("玩家2", this);
        human2->addFunds(playerMoney);
        m_players.append(human2);
    } else {
        // AI玩家
        AIPlayer* ai = new AIPlayer("电脑", this);
        ai->addFunds(aiMoney);
        m_players.append(ai);
    }

    emit messageLogged(QString("玩家初始资金: %1 比特").arg(playerMoney));
}

void Game::startGame() {
    if (m_players.isEmpty()) {
        setupPlayers();
    }
    m_state = ROLLING;
    emit gameStarted();
    emit turnChanged(getCurrentPlayer(), m_currentRound);
    emit messageLogged("=== 游戏开始 ===");

    // 如果是AI先手，自动处理
    if (dynamic_cast<AIPlayer*>(getCurrentPlayer())) {
        m_aiTimer->start(1000);
    }
}

void Game::nextTurn() {
    // 检查当前玩家是否破产
    if (getCurrentPlayer()->isBankrupt()) {
        removeBankruptPlayer(getCurrentPlayer());
        if (m_players.size() <= 1) {
            checkWinCondition();
            return;
        }
    }

    // 切换到下一个玩家
    m_currentPlayerIndex = (m_currentPlayerIndex + 1) % m_players.size();

    // 检查是否需要进入下一回合
    if (m_currentPlayerIndex == 0) {
        m_currentRound++;
        if (m_currentRound > m_maxRounds) {
            checkWinCondition();
            return;
        }
    }

    Player* current = getCurrentPlayer();

    // 检查是否被跳过
    if (current->isSkipped()) {
        emit messageLogged(QString("%1 被跳过一回合").arg(current->getName()));
        current->setSkipped(false);
        // 递归处理下一个
        QTimer::singleShot(500, this, &Game::nextTurn);
        return;
    }

    m_state = ROLLING;
    emit turnChanged(current, m_currentRound);
    emit messageLogged(QString("\n=== 第 %1 回合 - %2 的回合 ===")
                       .arg(m_currentRound).arg(current->getName()));

    // AI自动掷骰
    if (dynamic_cast<AIPlayer*>(current)) {
        m_aiTimer->start(800);
    }
}

void Game::rollDice() {
    if (m_state != ROLLING) return;

    int roll = m_dice->roll();
    bool isDouble = m_dice->isDouble();

    emit diceResult(roll, isDouble);
    emit messageLogged(QString("%1 掷出了 %2 点")
                       .arg(getCurrentPlayer()->getName()).arg(roll));

    m_state = MOVING;
    emit stateChanged(m_state);

    // 移动玩家
    Player* current = getCurrentPlayer();
    int oldPos = current->getPosition();
    int newPos = m_board->movePlayer(current, roll);
    current->setPosition(newPos);

    emit playerMoved(current, oldPos, newPos);

    // 处理格子事件
    QTimer::singleShot(300, this, &Game::processTileEvent);
}

void Game::processTileEvent() {
    Player* current = getCurrentPlayer();
    Tile* tile = m_board->getPlayerTile(current);

    m_state = TILE_EVENT;
    emit stateChanged(m_state);
    emit tileTriggered(tile, current);

    // 调用格子的虚函数 - 展示多态
    tile->onPlayerEnter(current, this);

    // 检查是否需要决策
    PropertyTile* prop = dynamic_cast<PropertyTile*>(tile);
    if (prop && !prop->hasOwner() && prop->getPrice() > 0) {
        if (current->getFunds() >= prop->getPrice()) {
            m_state = BUY_DECISION;
            emit stateChanged(m_state);
            emit buyDecisionRequired(tile, prop->getPrice());
            return;
        }
    }

    if (prop && prop->getOwner() == current && current->getFunds() >= prop->getUpgradeCost()) {
        m_state = UPGRADE_DECISION;
        emit stateChanged(m_state);
        emit upgradeDecisionRequired(tile, prop->getUpgradeCost());
        return;
    }

    // 回合结束
    QTimer::singleShot(500, this, [this]() {
        m_state = TURN_END;
        emit stateChanged(m_state);
        nextTurn();
    });
}

void Game::buyCurrentProperty(bool buy) {
    if (m_state != BUY_DECISION) return;

    Player* current = getCurrentPlayer();
    Tile* tile = m_board->getPlayerTile(current);
    PropertyTile* prop = dynamic_cast<PropertyTile*>(tile);

    if (prop && buy) {
        if (current->deductFunds(prop->getPrice())) {
            prop->setOwner(current);
            current->addProperty(prop);
            emit messageLogged(QString("%1 购买了 %2，花费 %3 比特")
                               .arg(current->getName())
                               .arg(prop->getName())
                               .arg(prop->getPrice()));
        }
    } else if (prop) {
        emit messageLogged(QString("%1 选择不购买 %2")
                           .arg(current->getName())
                           .arg(prop->getName()));
    }

    // 继续检查升级
    if (prop && prop->getOwner() == current && current->getFunds() >= prop->getUpgradeCost()) {
        m_state = UPGRADE_DECISION;
        emit stateChanged(m_state);
        emit upgradeDecisionRequired(tile, prop->getUpgradeCost());
        return;
    }

    m_state = TURN_END;
    emit stateChanged(m_state);
    nextTurn();
}

void Game::upgradeCurrentProperty(bool upgrade) {
    if (m_state != UPGRADE_DECISION) return;

    Player* current = getCurrentPlayer();
    Tile* tile = m_board->getPlayerTile(current);
    PropertyTile* prop = dynamic_cast<PropertyTile*>(tile);

    if (prop && upgrade) {
        if (current->deductFunds(prop->getUpgradeCost())) {
            prop->upgrade();
            emit messageLogged(QString("%1 升级了 %2，花费 %3 比特，收租提升至 %4")
                               .arg(current->getName())
                               .arg(prop->getName())
                               .arg(prop->getUpgradeCost())
                               .arg(prop->getRent()));
        }
    }

    m_state = TURN_END;
    emit stateChanged(m_state);
    nextTurn();
}

void Game::useItem(int itemId) {
    Player* current = getCurrentPlayer();
    if (m_itemManager->useItem(current, itemId, this)) {
        emit itemUsed(current, itemId, true);
    } else {
        emit itemUsed(current, itemId, false);
    }
}

void Game::answerQuestion(int answerIndex) {
    // 由QuestionDialog调用
}

void Game::handleAITurn() {
    m_aiTimer->stop();
    AIPlayer* ai = dynamic_cast<AIPlayer*>(getCurrentPlayer());
    if (ai && m_state == ROLLING) {
        rollDice();
    }
}

bool Game::needsBuyDecision() const {
    return m_state == BUY_DECISION;
}

bool Game::needsUpgradeDecision() const {
    return m_state == UPGRADE_DECISION;
}

Player* Game::getCurrentPlayer() const {
    if (m_players.isEmpty()) return nullptr;
    return m_players[m_currentPlayerIndex];
}

Player* Game::getPlayer(int index) const {
    if (index >= 0 && index < m_players.size()) {
        return m_players[index];
    }
    return nullptr;
}

void Game::checkWinCondition() {
    // 资金归零判负
    QVector<Player*> alivePlayers;
    for (Player* p : m_players) {
        if (!p->isBankrupt()) {
            alivePlayers.append(p);
        }
    }

    if (alivePlayers.size() == 1) {
        m_state = GAME_OVER;
        emit stateChanged(m_state);
        emit gameEnded(alivePlayers.first());
        emit messageLogged(QString("=== 游戏结束！%1 获胜！===").arg(alivePlayers.first()->getName()));
        return;
    }

    if (alivePlayers.isEmpty() || m_currentRound > m_maxRounds) {
        m_state = GAME_OVER;
        emit stateChanged(m_state);
        // 比较总资产
        Player* winner = nullptr;
        int maxAsset = -1;
        for (Player* p : m_players) {
            int asset = p->getFunds() + p->getTotalPropertyValue();
            if (asset > maxAsset) {
                maxAsset = asset;
                winner = p;
            }
        }
        emit gameEnded(winner);
        emit messageLogged(QString("=== 游戏结束！%1 以总资产 %2 获胜！===")
                           .arg(winner ? winner->getName() : "无")
                           .arg(maxAsset));
        return;
    }
}

void Game::removeBankruptPlayer(Player* player) {
    emit messageLogged(QString("%1 破产了！").arg(player->getName()));
    // 释放所有地产
    for (PropertyTile* prop : player->getProperties()) {
        prop->setOwner(nullptr);
        prop->reset();
    }
    player->clearProperties();
    m_players.removeAll(player);
}

void Game::endGame() {
    m_state = GAME_OVER;
    emit stateChanged(m_state);
}

void Game::pauseGame() {
    // 暂停逻辑
}

void Game::resumeGame() {
    // 恢复逻辑
}
