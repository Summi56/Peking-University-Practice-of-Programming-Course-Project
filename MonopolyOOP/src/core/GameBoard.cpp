#include "core/GameBoard.h"
#include "tiles/Tile.h"
#include "tiles/PropertyTile.h"
#include "tiles/InheritanceTile.h"
#include "players/Player.h"
#include <QDebug>

GameBoard::GameBoard(QObject *parent) : QObject(parent) {}

GameBoard::~GameBoard() {
    for (Tile* tile : m_tiles) {
        delete tile;
    }
    m_tiles.clear();
}

void GameBoard::initializeDefaultBoard() {
    // 清理旧地图
    for (Tile* tile : m_tiles) {
        delete tile;
    }
    m_tiles.clear();
    m_playerPositions.clear();

    // 创建默认地图 - 20个格子的环形地图
    // 使用new创建，GameBoard负责管理生命周期

    // 0: 起点
    addTile(new PropertyTile(0, "起点/基类", "所有格子的基类，经过获得200比特", 0, 0, this));

    // 1: 普通地产 - int类
    addTile(new PropertyTile(1, "int类", "基础数据类型，占用4字节", 100, 20, this));

    // 2: 机会格
    addTile(new ChanceTile(2, "机会/运算符重载", this));

    // 3: 地产 - double类
    addTile(new PropertyTile(3, "double类", "浮点数据类型，精度更高", 120, 25, this));

    // 4: 上机课格
    addTile(new LabClassTile(4, "上机课", "C++实验课，暂停一回合", this));

    // 5: 继承格 - 派生类
    PropertyTile* baseProp = new PropertyTile(5, "Shape形状类", "所有形状的基类", 150, 30, this);
    addTile(baseProp);

    // 6: 函数模板格（赌场）
    addTile(new FunctionTemplateTile(6, "函数模板/sort", "泛型算法，试试手气", this));

    // 7: 命运格
    addTile(new DestinyTile(7, "命运/内存泄漏", this));

    // 8: 继承格 - Rectangle继承Shape
    InheritanceTile* rectTile = new InheritanceTile(8, "Rectangle矩形类", "继承自Shape类", 180, 35, 5, this);
    addTile(rectTile);

    // 9: 麦叔的铺子
    addTile(new ShopTile(9, "麦叔的铺子", "购买和出售道具", this));

    // 10: 扣款格 - 疯狂星期四
    addTile(new PenaltyTile(10, "疯狂星期四", "V我50！扣除50比特", 50, this));

    // 11: 类模板格
    addTile(new ClassTemplateTile(11, "类模板/vector", "可实例化为各种类型", 200, this));

    // 12: 地产 - string类
    addTile(new PropertyTile(12, "string类", "字符串类，STL容器", 160, 32, this));

    // 13: 机会格
    addTile(new ChanceTile(13, "机会/虚函数", this));

    // 14: 继承格 - Circle继承Shape
    InheritanceTile* circleTile = new InheritanceTile(14, "Circle圆形类", "继承自Shape类", 180, 35, 5, this);
    addTile(circleTile);

    // 15: 银行格
    addTile(new PropertyTile(15, "银行/构造函数", "管理你的比特币", 0, 0, this));

    // 16: 地产 - vector<int>
    addTile(new PropertyTile(16, "vector<int>", "int类型的动态数组", 200, 40, this));

    // 17: 上机课格
    addTile(new LabClassTile(17, "Debug调试课", "找Bug中，暂停一回合", this));

    // 18: 函数模板格
    addTile(new FunctionTemplateTile(18, "函数模板/find", "泛型查找算法", this));

    // 19: 继承格 - Triangle继承Shape
    InheritanceTile* triTile = new InheritanceTile(19, "Triangle三角类", "继承自Shape类", 180, 35, 5, this);
    addTile(triTile);
}

void GameBoard::addTile(Tile* tile) {
    m_tiles.append(tile);
}

Tile* GameBoard::getTile(int position) const {
    if (position >= 0 && position < m_tiles.size()) {
        return m_tiles[position];
    }
    // 环形地图
    int wrapped = ((position % m_tiles.size()) + m_tiles.size()) % m_tiles.size();
    return m_tiles[wrapped];
}

int GameBoard::movePlayer(Player* player, int steps) {
    int oldPos = m_playerPositions.value(player, 0);
    int newPos = (oldPos + steps) % m_tiles.size();
    m_playerPositions[player] = newPos;

    // 检查是否经过起点
    if (newPos < oldPos && steps > 0) {
        // 经过起点，奖励200比特
        player->addFunds(200);
    }

    emit playerMoved(player, oldPos, newPos);
    return newPos;
}

Tile* GameBoard::getPlayerTile(Player* player) const {
    int pos = m_playerPositions.value(player, 0);
    return getTile(pos);
}

QVector<PropertyTile*> GameBoard::getPropertyChain(int position) const {
    QVector<PropertyTile*> chain;
    Tile* tile = getTile(position);
    PropertyTile* prop = dynamic_cast<PropertyTile*>(tile);
    if (prop) {
        chain.append(prop);
        // 检查是否是继承格，添加基类
        InheritanceTile* inh = dynamic_cast<InheritanceTile*>(tile);
        if (inh) {
            Tile* base = getTile(inh->getBaseTileId());
            PropertyTile* baseProp = dynamic_cast<PropertyTile*>(base);
            if (baseProp) {
                chain.prepend(baseProp); // 基类在前
            }
        }
    }
    return chain;
}

void GameBoard::reset() {
    m_playerPositions.clear();
    for (Tile* tile : m_tiles) {
        tile->reset();
    }
    emit boardReset();
}
