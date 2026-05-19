#pragma once
#include <QString>
#include <QVector>

// 前置声明
class Player;
class Game;

/**
 * @brief 道具基类 - 所有道具的抽象基类
 *
 * 道具系统展示：
 * - 封装：每个道具有自己的属性和效果
 * - 多态：use函数是虚函数，不同道具有不同效果
 */
class Item : public QObject {
    Q_OBJECT
public:
    enum ItemType {
        LEAVE_SLIP,     // 假条 - 免除一次上机课
        VIRTUAL_CARD,   // 虚函数卡 - 在继承格阻断基类调用
        LUCK_CARD,      // 手气卡 - 重新掷骰子
        DELETE_CARD     // delete卡 - 删除一个对象（地产）
    };

    explicit Item(ItemType type, const QString& name,
                  const QString& description, int price,
                  QObject* parent = nullptr);
    virtual ~Item() = default;

    // 使用道具 - 纯虚函数
    virtual bool use(Player* user, Game* game) = 0;

    // 获取信息
    ItemType getType() const { return m_type; }
    QString getName() const { return m_name; }
    QString getDescription() const { return m_description; }
    int getPrice() const { return m_price; }

protected:
    ItemType m_type;
    QString m_name;
    QString m_description;
    int m_price;
};

// 具体道具类

/**
 * @brief 假条 - 免除一次上机课
 */
class LeaveSlip : public Item {
public:
    LeaveSlip();
    bool use(Player* user, Game* game) override;
};

/**
 * @brief 虚函数卡 - 阻断继承链
 */
class VirtualFunctionCard : public Item {
public:
    VirtualFunctionCard();
    bool use(Player* user, Game* game) override;
};

/**
 * @brief 手气卡 - 重新掷骰子
 */
class LuckCard : public Item {
public:
    LuckCard();
    bool use(Player* user, Game* game) override;
};

/**
 * @brief delete卡 - 删除一个地产
 */
class DeleteCard : public Item {
public:
    DeleteCard();
    bool use(Player* user, Game* game) override;
};
