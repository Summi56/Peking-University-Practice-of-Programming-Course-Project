#pragma once
#include "tiles/Tile.h"
#include <QColor>

// 前置声明
class Player;

/**
 * @brief 地产格子类 - 可以购买、升级、收租的格子
 *
 * 对应文档中的"普通格子"：定义了一个类，可以创建对象（建房）
 * 其他玩家经过会被执行对象的内置函数（收租）
 * 经过自己的对象时可升级（添加成员函数，如修改收租金额）
 */
class PropertyTile : public Tile {
    Q_OBJECT
public:
    PropertyTile(int id, const QString& name, const QString& description,
                 int price, int baseRent, QObject* parent = nullptr);

    // 虚函数实现 - 玩家进入时的处理
    void onPlayerEnter(Player* player, Game* game) override;

    // 虚函数实现 - 玩家经过时的处理
    void onPlayerPass(Player* player, Game* game) override;

    // 获取属性
    int getPrice() const { return m_price; }
    int getRent() const { return m_rent; }
    int getUpgradeCost() const { return m_upgradeCost; }
    int getUpgradeLevel() const { return m_upgradeLevel; }

    // 所有者管理
    Player* getOwner() const { return m_owner; }
    void setOwner(Player* owner) { m_owner = owner; }
    bool hasOwner() const { return m_owner != nullptr; }

    // 升级地产
    virtual void upgrade();

    // 计算当前租金
    virtual int calculateRent() const { return m_rent; }

    // 获取抵押价值
    int getMortgageValue() const { return m_price / 2; }

    // 重置
    void reset() override;

    // UI颜色
    QColor getColor() const override;

    // 获取总资产价值（购买价格 + 升级投入）
    int getTotalValue() const { return m_price + m_upgradeLevel * m_upgradeCost; }

    QString getDisplayText() const override;

protected:
    int m_price;        // 购买价格
    int m_rent;         // 基础租金
    int m_upgradeCost;  // 升级费用
    int m_upgradeLevel; // 升级等级
    Player* m_owner;    // 所有者（封装：所有权信息隐藏在类内部）
};
