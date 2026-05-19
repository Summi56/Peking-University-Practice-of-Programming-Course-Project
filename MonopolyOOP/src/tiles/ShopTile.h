#pragma once
#include "tiles/Tile.h"
#include <QMap>

/**
 * @brief 商店格 - 购买和出售道具
 *
 * 对应文档中的"麦叔的铺子"格：
 * - 可以购买各种道具卡
 * - 也可以出售不需要的道具
 */
class ShopTile : public Tile {
    Q_OBJECT
public:
    explicit ShopTile(int id, const QString& name,
                      const QString& description,
                      QObject* parent = nullptr);

    void onPlayerEnter(Player* player, Game* game) override;

    struct ShopItem {
        int itemId;
        QString name;
        QString description;
        int buyPrice;
        int sellPrice;
        int oopKnowledgeReq; // 需要的OOP知识等级（可选机制）
    };

    QVector<ShopItem> getAvailableItems() const { return m_items; }

    QColor getColor() const override;
    QString getDisplayText() const override;

signals:
    void shopOpened(QVector<ShopItem> items);

private:
    QVector<ShopItem> m_items;
};
