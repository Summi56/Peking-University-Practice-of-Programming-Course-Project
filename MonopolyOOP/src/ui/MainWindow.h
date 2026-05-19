#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QStackedWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QGridLayout>

// 前置声明
class Game;
class GameBoardWidget;
class PlayerInfoWidget;
class DiceWidget;

/**
 * @brief 主窗口类 - 游戏的主界面
 *
 * 管理游戏的所有UI界面，包括：
 * - 开始菜单
 * - 游戏主界面（地图 + 玩家信息 + 控制台）
 * - 游戏结束画面
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    // 菜单按钮
    void onStartGameClicked();
    void onHowToPlayClicked();
    void onAboutClicked();
    void onBackToMenuClicked();
    void onQuitClicked();

    // 游戏控制
    void onRollDiceClicked();
    void onBuyYesClicked();
    void onBuyNoClicked();
    void onUpgradeYesClicked();
    void onUpgradeNoClicked();
    void onUseItemClicked();
    void onNextTurnClicked();

    // 游戏事件响应
    void onGameStarted();
    void onGameEnded(Player* winner);
    void onTurnChanged(Player* player, int round);
    void onDiceResult(int value, bool isDouble);
    void onPlayerMoved(Player* player, int oldPos, int newPos);
    void onTileTriggered(Tile* tile, Player* player);
    void onBuyDecisionRequired(Tile* tile, int price);
    void onUpgradeDecisionRequired(Tile* tile, int price);
    void onMessageLogged(QString message);
    void onStateChanged(Game::GameState state);
    void onQuestionAsked(QString question, QVector<QString> options);

private:
    void setupUI();
    void setupMenuScreen();
    void setupGameScreen();
    void setupGameOverScreen();
    void connectGameSignals();
    void showScreen(int index);
    void updateDecisionButtons(bool showBuy, bool showUpgrade);
    void logMessage(const QString& msg);

    // 核心游戏对象
    Game* m_game = nullptr;

    // 界面切换
    QStackedWidget* m_stackedWidget;

    // 菜单界面
    QWidget* m_menuScreen;
    QPushButton* m_btnStart;
    QPushButton* m_btnHowToPlay;
    QPushButton* m_btnAbout;
    QPushButton* m_btnQuit;
    QComboBox* m_comboMode;
    QComboBox* m_comboDifficulty;
    QLineEdit* m_editPlayerName;

    // 游戏界面
    QWidget* m_gameScreen;
    GameBoardWidget* m_boardWidget;
    PlayerInfoWidget* m_playerInfoWidget;
    DiceWidget* m_diceWidget;
    QTextEdit* m_gameLog;
    QLabel* m_lblCurrentTurn;
    QLabel* m_lblRoundInfo;

    // 游戏控制按钮
    QPushButton* m_btnRollDice;
    QPushButton* m_btnBuyYes;
    QPushButton* m_btnBuyNo;
    QPushButton* m_btnUpgradeYes;
    QPushButton* m_btnUpgradeNo;
    QPushButton* m_btnUseItem;
    QPushButton* m_btnNextTurn;
    QPushButton* m_btnBackToMenu;

    // 游戏结束界面
    QWidget* m_gameOverScreen;
    QLabel* m_lblWinner;
    QPushButton* m_btnPlayAgain;
    QPushButton* m_btnMenuFromOver;
};
