#include "tiles/ShopTile.h"
#include "players/Player.h"
#include "players/AIPlayer.h"
#include "core/Game.h"
#include <QRandomGenerator>

ShopTile::ShopTile(int id, const QString& name,
                   const QString& description,
                   QObject* parent)
    : Tile(id, name, description, SHOP, parent) {
    // 初始化商店商品
    m_items.append({0, "假条", "免除一次上机课", 100, 50, 0});
    m_items.append({1, "虚函数卡", "在继承格不触发基类效果", 150, 75, 0});
    m_items.append({2, "手气卡", "重新掷骰子", 80, 40, 0});
    m_items.append({3, "delete卡", "删除一个对象（地产）", 200, 100, 0});
}

void ShopTile::onPlayerEnter(Player* player, Game* game) {
    Q_UNUSED(game)

    emit tileEventTriggered(
        QString("=== 麦叔的铺子 ===\n欢迎光临 %1！\n"
                "这里出售各种C++编程道具：\n")
        .arg(m_name));

    for (const auto& item : m_items) {
        emit tileEventTriggered(
            QString("%1. %2 - %3 比特 (%4)")
            .arg(item.itemId + 1).arg(item.name)
            .arg(item.buyPrice).arg(item.description));
    }

    // AI自动购买逻辑
    AIPlayer* ai = dynamic_cast<AIPlayer*>(player);
    if (ai) {
        // AI随机决定是否购买
        for (const auto& item : m_items) {
            if (player->getFunds() > item.buyPrice * 2 &&
                QRandomGenerator::global()->bounded(100) < 30) {
                if (player->deductFunds(item.buyPrice)) {
                    player->addItem(item.itemId);
                    emit tileEventTriggered(
                        QString("AI购买了 %1").arg(item.name));
                }
                break; // 每回合最多买一个
            }
        }
    }

    emit shopOpened(m_items);
}

QColor ShopTile::getColor() const {
    return QColor(255, 150, 50); // 橙色
}

QString ShopTile::getDisplayText() const {
    return QString("%1\n[商店]").arg(m_name);
}
