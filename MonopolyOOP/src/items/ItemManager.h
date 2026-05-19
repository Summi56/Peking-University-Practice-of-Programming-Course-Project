#pragma once
#include <QObject>
#include <QMap>
#include <memory>
#include "items/Item.h"

// 前置声明
class Player;
class Game;

/**
 * @brief 道具管理器 - 管理所有道具的创建和使用
 *
 * 使用工厂模式创建道具
 */
class ItemManager : public QObject {
    Q_OBJECT
public:
    explicit ItemManager(QObject* parent = nullptr);

    // 创建道具
    Item* createItem(int itemId);

    // 使用道具
    bool useItem(Player* player, int itemId, Game* game);

    // 获取道具信息
    QString getItemName(int itemId) const;
    QString getItemDescription(int itemId) const;
    int getItemPrice(int itemId) const;

signals:
    void itemUsed(Player* player, QString itemName, bool success);
    void messageLogged(QString message);

private:
    QMap<int, std::function<Item*()>> m_itemFactories;
};
