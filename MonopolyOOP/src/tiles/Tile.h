#pragma once
#include <QObject>
#include <QString>
#include <QColor>
#include <QVector>
#include <QMap>

// 前置声明
class Player;
class Game;

/**
 * @brief 格子基类 - 所有格子类型的抽象基类
 *
 * 演示C++面向对象的核心概念：
 * - 封装：将格子的属性和行为封装在类中
 * - 继承：所有具体格子类型继承自此基类
 * - 多态：onPlayerEnter是纯虚函数，每个子类有不同实现
 */
class Tile : public QObject {
    Q_OBJECT
public:
    // 格子类型枚举
    enum TileType {
        PROPERTY,       // 普通地产
        INHERITANCE,    // 继承与派生格
        CLASS_TEMPLATE, // 类模板格
        FUNCTION_TEMPLATE, // 函数模板格
        CHANCE,         // 机会格
        DESTINY,        // 命运格
        LAB_CLASS,      // 上机课格
        SHOP,           // 商店格
        PENALTY,        // 扣款格
        START           // 起点
    };

    explicit Tile(int id, const QString& name, const QString& description,
                  TileType type, QObject* parent = nullptr);
    virtual ~Tile() = default;

    // 纯虚函数 - 玩家进入格子时触发的事件
    // 这是多态的核心：不同类型的格子有不同的行为
    virtual void onPlayerEnter(Player* player, Game* game) = 0;

    // 虚函数 - 玩家经过格子时触发（不是必须停留）
    virtual void onPlayerPass(Player* player, Game* game);

    // 获取格子信息
    int getId() const { return m_id; }
    QString getName() const { return m_name; }
    QString getDescription() const { return m_description; }
    TileType getType() const { return m_type; }

    // 获取格子颜色（用于UI显示）
    virtual QColor getColor() const;

    // 重置格子状态
    virtual void reset();

    // 获取显示字符串
    virtual QString getDisplayText() const;

signals:
    void tileEventTriggered(QString message);

protected:
    int m_id;
    QString m_name;
    QString m_description;
    TileType m_type;
};
