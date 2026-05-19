#pragma once
#include <QObject>
#include <QVector>
#include <QTimer>
#include <QMap>
#include <QVector>
#include <memory>

// 前置声明
class Tile;
#include <memory>

class GameBoard;
class Dice;
class Player;
class HumanPlayer;
class AIPlayer;
class QuestionBank;
class ItemManager;

/**
 * @brief 游戏主控制类 - 负责管理游戏流程
 *
 * 采用单例模式（可选）管理整个游戏的状态，
 * 控制回合流程、胜负判定、游戏模式等。
 */
class Game : public QObject {
    Q_OBJECT
public:
    enum GameMode {
        SINGLE_PLAYER,   // 单人vs电脑
        TWO_PLAYER,      // 双人对战
        PRACTICE_MODE    // 练习模式（无惩罚）
    };

    enum GameDifficulty {
        EASY,    // 玩家初始资金多，电脑初始资金少
        NORMAL,  // 公平开局
        HARD     // 玩家初始资金少，电脑初始资金多
    };

    enum GameState {
        SETUP,      // 设置阶段
        ROLLING,    // 等待掷骰
        MOVING,     // 移动中
        TILE_EVENT, // 处理格子事件
        BUY_DECISION,   // 购买决策
        UPGRADE_DECISION, // 升级决策
        USE_ITEM,   // 使用道具
        TURN_END,   // 回合结束
        GAME_OVER   // 游戏结束
    };

    explicit Game(QObject *parent = nullptr);
    ~Game();

    // 初始化游戏
    void initialize(GameMode mode, GameDifficulty diff);

    // 游戏控制
    void startGame();
    void pauseGame();
    void resumeGame();
    void endGame();

    // 回合控制
    void nextTurn();
    void rollDice();

    // 玩家操作接口
    void buyCurrentProperty(bool buy);
    void upgradeCurrentProperty(bool upgrade);
    void useItem(int itemId);
    void answerQuestion(int answerIndex);

    // Getters
    GameBoard* getBoard() const { return m_board; }
    Dice* getDice() const { return m_dice; }
    Player* getCurrentPlayer() const;
    Player* getPlayer(int index) const;
    int getPlayerCount() const { return m_players.size(); }
    GameState getState() const { return m_state; }
    GameMode getMode() const { return m_mode; }
    int getCurrentRound() const { return m_currentRound; }
    int getMaxRounds() const { return m_maxRounds; }
    QVector<Player*> getAllPlayers() const { return m_players; }
    QuestionBank* getQuestionBank() const { return m_questionBank; }
    ItemManager* getItemManager() const { return m_itemManager; }

    // 检查当前格子是否需要决策
    bool needsBuyDecision() const;
    bool needsUpgradeDecision() const;

signals:
    void gameStarted();
    void gameEnded(Player* winner);
    void turnChanged(Player* player, int round);
    void diceResult(int value, bool isDouble);
    void playerMoved(Player* player, int oldPos, int newPos);
    void tileTriggered(Tile* tile, Player* player);
    void buyDecisionRequired(Tile* tile, int price);
    void upgradeDecisionRequired(Tile* tile, int price);
    void questionAsked(QString question, QVector<QString> options);
    void questionResult(bool correct, QString explanation);
    void itemUsed(Player* player, int itemId, bool success);
    void stateChanged(GameState newState);
    void messageLogged(QString message);

public slots:
    void handleAITurn();

private:
    void setupPlayers();
    void checkWinCondition();
    void processTileEvent();
    void removeBankruptPlayer(Player* player);

    GameBoard* m_board = nullptr;
    Dice* m_dice = nullptr;
    QuestionBank* m_questionBank = nullptr;
    ItemManager* m_itemManager = nullptr;
    QVector<Player*> m_players;
    int m_currentPlayerIndex = 0;
    int m_currentRound = 0;
    int m_maxRounds = 50;  // 默认50回合
    GameState m_state = SETUP;
    GameMode m_mode = SINGLE_PLAYER;
    GameDifficulty m_difficulty = NORMAL;
    QTimer* m_aiTimer = nullptr;
};
