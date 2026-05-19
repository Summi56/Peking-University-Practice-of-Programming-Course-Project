#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QTableWidget>
#include <QVector>

// 前置声明
class Game;
class Player;

/**
 * @brief 玩家信息面板
 *
 * 显示所有玩家的资金、地产、道具等信息
 */
class PlayerInfoWidget : public QWidget {
    Q_OBJECT
public:
    explicit PlayerInfoWidget(QWidget* parent = nullptr);

    void setGame(Game* game);
    void updateDisplay();

private slots:
    void onFundsChanged(Player* player, int newAmount);
    void onPropertyChanged(Player* player);

private:
    void setupUI();
    void updatePlayerInfo(int index, Player* player);

    Game* m_game = nullptr;

    struct PlayerDisplay {
        QLabel* lblName;
        QLabel* lblFunds;
        QLabel* lblProperties;
        QLabel* lblItems;
        QProgressBar* barAssets;
    };

    QVector<PlayerDisplay> m_displays;
    QLabel* m_lblTitle;
};
