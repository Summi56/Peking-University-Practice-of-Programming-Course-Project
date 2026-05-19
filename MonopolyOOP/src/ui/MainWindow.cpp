#include "ui/MainWindow.h"
#include "ui/GameBoardWidget.h"
#include "ui/PlayerInfoWidget.h"
#include "ui/DiceWidget.h"
#include "ui/QuestionDialog.h"
#include "core/Game.h"
#include "core/GameBoard.h"
#include "tiles/Tile.h"
#include "players/Player.h"
#include "players/AIPlayer.h"
#include <QMessageBox>
#include <QApplication>
#include <QScreen>
#include <QComboBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QGridLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollBar>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    setWindowTitle("C++ OOP 大富翁 - 面向对象编程学习游戏");
    resize(1200, 800);

    // 居中显示
    QRect screen = QApplication::primaryScreen()->geometry();
    move((screen.width() - width()) / 2, (screen.height() - height()) / 2);
}

MainWindow::~MainWindow() {
    if (m_game) {
        delete m_game;
    }
}

void MainWindow::setupUI() {
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    setupMenuScreen();
    setupGameScreen();
    setupGameOverScreen();
}

void MainWindow::setupMenuScreen() {
    m_menuScreen = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(m_menuScreen);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    // 标题
    QLabel* title = new QLabel("C++ OOP 大富翁", m_menuScreen);
    QFont titleFont = title->font();
    titleFont.setPointSize(36);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #2196F3; margin-bottom: 20px;");
    layout->addWidget(title);

    QLabel* subtitle = new QLabel("在游戏中学习C++面向对象编程！", m_menuScreen);
    QFont subFont = subtitle->font();
    subFont.setPointSize(14);
    subtitle->setFont(subFont);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: #666; margin-bottom: 30px;");
    layout->addWidget(subtitle);

    // 设置区域
    QGroupBox* settingsBox = new QGroupBox("游戏设置", m_menuScreen);
    QGridLayout* settingsLayout = new QGridLayout(settingsBox);

    settingsLayout->addWidget(new QLabel("玩家名称:"), 0, 0);
    m_editPlayerName = new QLineEdit("玩家", settingsBox);
    settingsLayout->addWidget(m_editPlayerName, 0, 1);

    settingsLayout->addWidget(new QLabel("游戏模式:"), 1, 0);
    m_comboMode = new QComboBox(settingsBox);
    m_comboMode->addItem("单人 vs 电脑", Game::SINGLE_PLAYER);
    m_comboMode->addItem("双人对战", Game::TWO_PLAYER);
    m_comboMode->addItem("练习模式", Game::PRACTICE_MODE);
    settingsLayout->addWidget(m_comboMode, 1, 1);

    settingsLayout->addWidget(new QLabel("难度:"), 2, 0);
    m_comboDifficulty = new QComboBox(settingsBox);
    m_comboDifficulty->addItem("简单", Game::EASY);
    m_comboDifficulty->addItem("普通", Game::NORMAL);
    m_comboDifficulty->addItem("困难", Game::HARD);
    settingsLayout->addWidget(m_comboDifficulty, 2, 1);

    layout->addWidget(settingsBox);
    settingsBox->setMaximumWidth(400);

    // 按钮
    m_btnStart = new QPushButton("开始游戏", m_menuScreen);
    m_btnStart->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; "
        "font-size: 18px; padding: 12px 40px; border-radius: 8px; }"
        "QPushButton:hover { background-color: #45a049; }"
    );
    connect(m_btnStart, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    layout->addWidget(m_btnStart, 0, Qt::AlignCenter);

    m_btnHowToPlay = new QPushButton("游戏说明", m_menuScreen);
    m_btnHowToPlay->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; "
        "font-size: 14px; padding: 8px 30px; border-radius: 6px; }"
    );
    connect(m_btnHowToPlay, &QPushButton::clicked, this, &MainWindow::onHowToPlayClicked);
    layout->addWidget(m_btnHowToPlay, 0, Qt::AlignCenter);

    m_btnAbout = new QPushButton("关于", m_menuScreen);
    m_btnAbout->setStyleSheet(
        "QPushButton { background-color: #757575; color: white; "
        "font-size: 14px; padding: 8px 30px; border-radius: 6px; }"
    );
    connect(m_btnAbout, &QPushButton::clicked, this, &MainWindow::onAboutClicked);
    layout->addWidget(m_btnAbout, 0, Qt::AlignCenter);

    m_btnQuit = new QPushButton("退出", m_menuScreen);
    m_btnQuit->setStyleSheet(
        "QPushButton { background-color: #f44336; color: white; "
        "font-size: 14px; padding: 8px 30px; border-radius: 6px; }"
    );
    connect(m_btnQuit, &QPushButton::clicked, this, &MainWindow::onQuitClicked);
    layout->addWidget(m_btnQuit, 0, Qt::AlignCenter);

    layout->addStretch();
    m_stackedWidget->addWidget(m_menuScreen);
}

void MainWindow::setupGameScreen() {
    m_gameScreen = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(m_gameScreen);

    // 左侧：游戏地图
    m_boardWidget = new GameBoardWidget(m_gameScreen);
    m_boardWidget->setMinimumSize(600, 600);
    mainLayout->addWidget(m_boardWidget, 2);

    // 右侧：信息面板
    QVBoxLayout* rightLayout = new QVBoxLayout();

    // 回合信息
    QGroupBox* turnBox = new QGroupBox("游戏信息", m_gameScreen);
    QVBoxLayout* turnLayout = new QVBoxLayout(turnBox);
    m_lblCurrentTurn = new QLabel("当前回合: -", turnBox);
    m_lblRoundInfo = new QLabel("第 1 / 50 回合", turnBox);
    QFont infoFont = m_lblCurrentTurn->font();
    infoFont.setPointSize(12);
    infoFont.setBold(true);
    m_lblCurrentTurn->setFont(infoFont);
    m_lblRoundInfo->setFont(infoFont);
    turnLayout->addWidget(m_lblCurrentTurn);
    turnLayout->addWidget(m_lblRoundInfo);
    rightLayout->addWidget(turnBox);

    // 骰子控件
    QGroupBox* diceBox = new QGroupBox("骰子", m_gameScreen);
    QVBoxLayout* diceLayout = new QVBoxLayout(diceBox);
    m_diceWidget = new DiceWidget(diceBox);
    diceLayout->addWidget(m_diceWidget);
    m_btnRollDice = new QPushButton("掷骰子", diceBox);
    m_btnRollDice->setStyleSheet(
        "QPushButton { background-color: #FF9800; color: white; "
        "font-size: 16px; padding: 10px; border-radius: 6px; }"
    );
    connect(m_btnRollDice, &QPushButton::clicked, this, &MainWindow::onRollDiceClicked);
    diceLayout->addWidget(m_btnRollDice);
    rightLayout->addWidget(diceBox);

    // 玩家信息
    m_playerInfoWidget = new PlayerInfoWidget(m_gameScreen);
    rightLayout->addWidget(m_playerInfoWidget);

    // 决策按钮区域
    QGroupBox* decisionBox = new QGroupBox("决策", m_gameScreen);
    QHBoxLayout* decisionLayout = new QHBoxLayout(decisionBox);

    m_btnBuyYes = new QPushButton("购买", decisionBox);
    m_btnBuyNo = new QPushButton("不买", decisionBox);
    m_btnUpgradeYes = new QPushButton("升级", decisionBox);
    m_btnUpgradeNo = new QPushButton("不升级", decisionBox);

    decisionLayout->addWidget(m_btnBuyYes);
    decisionLayout->addWidget(m_btnBuyNo);
    decisionLayout->addWidget(m_btnUpgradeYes);
    decisionLayout->addWidget(m_btnUpgradeNo);

    connect(m_btnBuyYes, &QPushButton::clicked, this, &MainWindow::onBuyYesClicked);
    connect(m_btnBuyNo, &QPushButton::clicked, this, &MainWindow::onBuyNoClicked);
    connect(m_btnUpgradeYes, &QPushButton::clicked, this, &MainWindow::onUpgradeYesClicked);
    connect(m_btnUpgradeNo, &QPushButton::clicked, this, &MainWindow::onUpgradeNoClicked);

    m_btnBuyYes->setVisible(false);
    m_btnBuyNo->setVisible(false);
    m_btnUpgradeYes->setVisible(false);
    m_btnUpgradeNo->setVisible(false);

    rightLayout->addWidget(decisionBox);

    // 道具按钮
    m_btnUseItem = new QPushButton("使用道具", m_gameScreen);
    connect(m_btnUseItem, &QPushButton::clicked, this, &MainWindow::onUseItemClicked);
    rightLayout->addWidget(m_btnUseItem);

    // 游戏日志
    QGroupBox* logBox = new QGroupBox("游戏日志", m_gameScreen);
    QVBoxLayout* logLayout = new QVBoxLayout(logBox);
    m_gameLog = new QTextEdit(logBox);
    m_gameLog->setReadOnly(true);
    m_gameLog->setMaximumHeight(200);
    logLayout->addWidget(m_gameLog);
    rightLayout->addWidget(logBox);

    // 返回菜单
    m_btnBackToMenu = new QPushButton("返回主菜单", m_gameScreen);
    connect(m_btnBackToMenu, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);
    rightLayout->addWidget(m_btnBackToMenu);

    mainLayout->addLayout(rightLayout, 1);
    m_stackedWidget->addWidget(m_gameScreen);
}

void MainWindow::setupGameOverScreen() {
    m_gameOverScreen = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(m_gameOverScreen);
    layout->setAlignment(Qt::AlignCenter);

    QLabel* title = new QLabel("游戏结束", m_gameOverScreen);
    QFont titleFont = title->font();
    titleFont.setPointSize(32);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #f44336;");
    layout->addWidget(title);

    m_lblWinner = new QLabel("", m_gameOverScreen);
    QFont winnerFont = m_lblWinner->font();
    winnerFont.setPointSize(18);
    m_lblWinner->setFont(winnerFont);
    m_lblWinner->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_lblWinner);

    m_btnPlayAgain = new QPushButton("再来一局", m_gameOverScreen);
    m_btnPlayAgain->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; "
        "font-size: 16px; padding: 10px 30px; border-radius: 6px; }"
    );
    connect(m_btnPlayAgain, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    layout->addWidget(m_btnPlayAgain, 0, Qt::AlignCenter);

    m_btnMenuFromOver = new QPushButton("返回主菜单", m_gameOverScreen);
    connect(m_btnMenuFromOver, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);
    layout->addWidget(m_btnMenuFromOver, 0, Qt::AlignCenter);

    layout->addStretch();
    m_stackedWidget->addWidget(m_gameOverScreen);
}

void MainWindow::connectGameSignals() {
    if (!m_game) return;

    connect(m_game, &Game::gameStarted, this, &MainWindow::onGameStarted);
    connect(m_game, &Game::gameEnded, this, &MainWindow::onGameEnded);
    connect(m_game, &Game::turnChanged, this, &MainWindow::onTurnChanged);
    connect(m_game, &Game::diceResult, this, &MainWindow::onDiceResult);
    connect(m_game, &Game::playerMoved, this, &MainWindow::onPlayerMoved);
    connect(m_game, &Game::tileTriggered, this, &MainWindow::onTileTriggered);
    connect(m_game, &Game::buyDecisionRequired, this, &MainWindow::onBuyDecisionRequired);
    connect(m_game, &Game::upgradeDecisionRequired, this, &MainWindow::onUpgradeDecisionRequired);
    connect(m_game, &Game::messageLogged, this, &MainWindow::onMessageLogged);
    connect(m_game, &Game::stateChanged, this, &MainWindow::onStateChanged);
    connect(m_game, &Game::questionAsked, this, &MainWindow::onQuestionAsked);
}

void MainWindow::showScreen(int index) {
    m_stackedWidget->setCurrentIndex(index);
}

void MainWindow::updateDecisionButtons(bool showBuy, bool showUpgrade) {
    m_btnBuyYes->setVisible(showBuy);
    m_btnBuyNo->setVisible(showBuy);
    m_btnUpgradeYes->setVisible(showUpgrade);
    m_btnUpgradeNo->setVisible(showUpgrade);

    m_btnBuyYes->setEnabled(showBuy);
    m_btnBuyNo->setEnabled(showBuy);
    m_btnUpgradeYes->setEnabled(showUpgrade);
    m_btnUpgradeNo->setEnabled(showUpgrade);
}

void MainWindow::logMessage(const QString& msg) {
    m_gameLog->append(msg);
    // 滚动到底部
    m_gameLog->verticalScrollBar()->setValue(
        m_gameLog->verticalScrollBar()->maximum()
    );
}

// ========== Slots ==========

void MainWindow::onStartGameClicked() {
    // 清理旧游戏
    if (m_game) {
        delete m_game;
    }

    // 创建新游戏
    m_game = new Game(this);
    connectGameSignals();

    m_boardWidget->setGame(m_game);
    m_playerInfoWidget->setGame(m_game);

    Game::GameMode mode = static_cast<Game::GameMode>(
        m_comboMode->currentData().toInt()
    );
    Game::GameDifficulty diff = static_cast<Game::GameDifficulty>(
        m_comboDifficulty->currentData().toInt()
    );

    m_game->initialize(mode, diff);
    m_game->startGame();

    showScreen(1); // 切换到游戏界面
}

void MainWindow::onHowToPlayClicked() {
    QMessageBox::information(this, "游戏说明",
        "<h2>C++ OOP 大富翁 - 游戏说明</h2>"
        "<p>这是一个将C++面向对象编程知识融入大富翁玩法的教育游戏。</p>"
        "<h3>基本玩法：</h3>"
        "<ul>"
        "<li>轮流掷骰子在地图上移动</li>"
        "<li>经过起点获得200比特</li>"
        "<li>购买地产并升级来收取租金</li>"
        "<li>让对手破产或回合结束时资产最高者获胜</li>"
        "</ul>"
        "<h3>特殊格子：</h3>"
        "<ul>"
        "<li><b>继承与派生格</b>：进入时同时触发基类格子的效果（可用手气卡阻断）</li>"
        "<li><b>类模板格</b>：可选择不同'类型'来实例化地产</li>"
        "<li><b>函数模板格</b>：类似赌场，根据选择类型决定胜率</li>"
        "<li><b>机会格</b>：回答C++ OOP问题，答对获得道具</li>"
        "<li><b>命运格</b>：随机触发与编程相关的奖励或惩罚</li>"
        "<li><b>上机课格</b>：暂停一回合（可用假条免除）</li>"
        "<li><b>麦叔的铺子</b>：购买和出售道具</li>"
        "</ul>"
        "<h3>道具说明：</h3>"
        "<ul>"
        "<li><b>假条</b>：免除一次上机课</li>"
        "<li><b>虚函数卡</b>：阻断继承格子的基类调用</li>"
        "<li><b>手气卡</b>：重新掷骰子</li>"
        "<li><b>delete卡</b>：删除对手的一个地产</li>"
        "</ul>"
    );
}

void MainWindow::onAboutClicked() {
    QMessageBox::information(this, "关于",
        "<h2>C++ OOP 大富翁 v1.0</h2>"
        "<p>一个融合C++面向对象编程知识的教育大富翁游戏。</p>"
        "<p>本项目旨在通过游戏化的方式帮助学习C++ OOP的核心概念：</p>"
        "<ul>"
        "<li>类与对象</li>"
        "<li>继承与派生</li>"
        "<li>多态与虚函数</li>"
        "<li>运算符重载</li>"
        "<li>模板编程</li>"
        "<li>高级特性</li>"
        "</ul>"
        "<p>使用 Qt/C++ 开发。</p>"
    );
}

void MainWindow::onBackToMenuClicked() {
    if (m_game) {
        delete m_game;
        m_game = nullptr;
    }
    showScreen(0);
}

void MainWindow::onQuitClicked() {
    QApplication::quit();
}

void MainWindow::onRollDiceClicked() {
    if (m_game) {
        m_btnRollDice->setEnabled(false);
        m_diceWidget->startRolling();
        m_game->rollDice();
    }
}

void MainWindow::onBuyYesClicked() {
    if (m_game) {
        m_game->buyCurrentProperty(true);
        updateDecisionButtons(false, false);
    }
}

void MainWindow::onBuyNoClicked() {
    if (m_game) {
        m_game->buyCurrentProperty(false);
        updateDecisionButtons(false, false);
    }
}

void MainWindow::onUpgradeYesClicked() {
    if (m_game) {
        m_game->upgradeCurrentProperty(true);
        updateDecisionButtons(false, false);
    }
}

void MainWindow::onUpgradeNoClicked() {
    if (m_game) {
        m_game->upgradeCurrentProperty(false);
        updateDecisionButtons(false, false);
    }
}

void MainWindow::onUseItemClicked() {
    // 弹出道具选择对话框
    // 简化：使用第一个可用的道具
    if (m_game) {
        Player* player = m_game->getCurrentPlayer();
        if (player) {
            QMap<int, int> items = player->getItems();
            if (!items.isEmpty()) {
                int itemId = items.firstKey();
                m_game->useItem(itemId);
            } else {
                logMessage("没有可用的道具！");
            }
        }
    }
}

void MainWindow::onNextTurnClicked() {
    m_btnRollDice->setEnabled(true);
}

void MainWindow::onGameStarted() {
    logMessage("=== 游戏开始 ===");
}

void MainWindow::onGameEnded(Player* winner) {
    if (winner) {
        m_lblWinner->setText(QString("获胜者：%1！\n总资产：%2 比特")
                             .arg(winner->getName())
                             .arg(winner->getTotalAssets()));
    } else {
        m_lblWinner->setText("平局！");
    }
    showScreen(2);
}

void MainWindow::onTurnChanged(Player* player, int round) {
    if (!player) return;
    m_lblCurrentTurn->setText(QString("当前回合: %1").arg(player->getName()));
    m_lblRoundInfo->setText(QString("第 %1 / %2 回合")
                            .arg(round).arg(m_game ? m_game->getMaxRounds() : 50));

    // 高亮当前玩家
    m_playerInfoWidget->updateDisplay();

    // 人类玩家才能掷骰子
    m_btnRollDice->setEnabled(!player->isAI());
}

void MainWindow::onDiceResult(int value, bool isDouble) {
    m_diceWidget->showResult(value);
    if (isDouble) {
        logMessage(QString("掷出了 %1 点（连掷！）").arg(value));
    } else {
        logMessage(QString("掷出了 %1 点").arg(value));
    }
}

void MainWindow::onPlayerMoved(Player* player, int oldPos, int newPos) {
    m_boardWidget->updatePlayerPosition(player, newPos);
    m_boardWidget->update();
    logMessage(QString("%1 从 %2 移动到 %3")
               .arg(player->getName()).arg(oldPos).arg(newPos));
}

void MainWindow::onTileTriggered(Tile* tile, Player* player) {
    logMessage(QString("%1 进入了 [%2]").arg(player->getName()).arg(tile->getName()));
}

void MainWindow::onBuyDecisionRequired(Tile* tile, int price) {
    Q_UNUSED(tile)
    logMessage(QString("是否购买 %1，价格 %2 比特？").arg(tile->getName()).arg(price));
    updateDecisionButtons(true, false);
}

void MainWindow::onUpgradeDecisionRequired(Tile* tile, int price) {
    Q_UNUSED(tile)
    logMessage(QString("是否升级 %1，价格 %2 比特？").arg(tile->getName()).arg(price));
    updateDecisionButtons(false, true);
}

void MainWindow::onMessageLogged(QString message) {
    logMessage(message);
}

void MainWindow::onStateChanged(Game::GameState state) {
    switch (state) {
    case Game::ROLLING:
        m_btnRollDice->setEnabled(true);
        break;
    case Game::GAME_OVER:
        m_btnRollDice->setEnabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::onQuestionAsked(QString question, QVector<QString> options) {
    QuestionDialog dialog(this);
    dialog.setQuestion(question, options, "OOP知识", 1);

    if (dialog.exec() == QDialog::Accepted) {
        // 处理答案 - 实际上在Game中处理
    }
}
