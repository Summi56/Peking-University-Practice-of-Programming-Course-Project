#pragma once
#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QColor>

// 前置声明
class PropertyTile;
class Game;

/**
 * @brief 玩家抽象基类 - 所有角色的基类
 *
 * 演示面向对象设计：
 * - 封装：资金、位置等属性私有化
 * - 继承：HumanPlayer和AIPlayer继承此类
 * - 多态：纯虚函数 makeDecision 让不同角色有不同行为
 */
class Player : public QObject {
    Q_OBJECT
public:
    explicit Player(const QString& name, QObject* parent = nullptr);
    virtual ~Player() = default;

    // ========== 纯虚函数 - 多态接口 ==========
    // 决策函数 - 人类玩家通过UI输入，AI通过算法决定
    virtual void makeDecision(Game* game) = 0;

    // 是否是AI
    virtual bool isAI() const = 0;

    // ========== 资金管理 ==========
    int getFunds() const { return m_funds; }
    void addFunds(int amount);
    bool deductFunds(int amount);

    // ========== 位置管理 ==========
    int getPosition() const { return m_position; }
    void setPosition(int pos) { m_position = pos; }

    // ========== 状态管理 ==========
    bool isBankrupt() const { return m_bankrupt; }
    void setBankrupt(bool b) { m_bankrupt = b; }

    bool isSkipped() const { return m_skipped; }
    void setSkipped(bool s) { m_skipped = s; }

    // ========== 地产管理 ==========
    const QVector<PropertyTile*>& getProperties() const { return m_properties; }
    void addProperty(PropertyTile* property);
    void removeProperty(PropertyTile* property);
    void clearProperties();
    int getPropertyCount() const { return m_properties.size(); }
    int getTotalPropertyValue() const;

    // ========== 道具管理 ==========
    void addItem(int itemId);
    void removeItem(int itemId);
    bool hasItem(int itemId) const;
    QMap<int, int> getItems() const { return m_items; }
    int getItemCount(int itemId) const;

    // ========== 获取信息 ==========
    QString getName() const { return m_name; }
    int getTotalAssets() const { return m_funds + getTotalPropertyValue(); }

    // 获取玩家颜色（用于UI显示）
    virtual QColor getPlayerColor() const = 0;

signals:
    void fundsChanged(int newAmount);
    void propertyAcquired(PropertyTile* property);
    void propertyLost(PropertyTile* property);
    void itemAdded(int itemId);
    void itemUsed(int itemId);

protected:
    QString m_name;
    int m_funds;
    int m_position;
    bool m_bankrupt;
    bool m_skipped;
    QVector<PropertyTile*> m_properties;
    QMap<int, int> m_items; // itemId -> count
};
