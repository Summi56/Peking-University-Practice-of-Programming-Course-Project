#pragma once
#include <QWidget>
#include <QVector>
#include <QMap>
#include <QPainter>
#include <QPaintEvent>

// 前置声明
class Game;
class Tile;
class Player;

/**
 * @brief 游戏地图绘制控件
 *
 * 负责绘制环形地图、格子、玩家棋子
 */
class GameBoardWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameBoardWidget(QWidget* parent = nullptr);

    void setGame(Game* game);

    // 更新玩家位置显示
    void updatePlayerPosition(Player* player, int position);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void drawBoard(QPainter& painter);
    void drawTiles(QPainter& painter);
    void drawPlayers(QPainter& painter);
    void drawTile(QPainter& painter, int index, const QRect& rect);
    QRect getTileRect(int index) const;
    QPoint getTileCenter(int index) const;

    Game* m_game = nullptr;
    QVector<QPoint> m_tileCenters;
    QVector<QRect> m_tileRects;
    int m_tileSize = 80;
    int m_gap = 4;
};
