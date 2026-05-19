#pragma once
#include <QObject>
#include <QVector>
#include <QMap>
#include <memory>
#include <QMap>

class Tile;
class Player;
class PropertyTile;

/**
 * @brief 游戏地图类 - 管理所有格子和玩家位置
 *
 * 封装了地图的布局、格子间的关联（如继承关系）、
 * 以及玩家移动的规则。
 */
class GameBoard : public QObject {
    Q_OBJECT
public:
    explicit GameBoard(QObject *parent = nullptr);
    ~GameBoard();

    // 初始化默认地图
    void initializeDefaultBoard();

    // 获取指定位置的格子
    Tile* getTile(int position) const;

    // 获取格子总数
    int tileCount() const { return m_tiles.size(); }

    // 添加格子到地图
    void addTile(Tile* tile);

    // 移动玩家，返回新位置
    int movePlayer(Player* player, int steps);

    // 获取玩家当前所在格子
    Tile* getPlayerTile(Player* player) const;

    // 获取指定格子的所有地产（用于显示继承链等）
    QVector<PropertyTile*> getPropertyChain(int position) const;

    // 重置地图状态
    void reset();

    // 获取所有格子
    const QVector<Tile*>& getAllTiles() const { return m_tiles; }

signals:
    void playerMoved(Player* player, int oldPos, int newPos);
    void boardReset();

private:
    QVector<Tile*> m_tiles;
    QMap<Player*, int> m_playerPositions;
};
