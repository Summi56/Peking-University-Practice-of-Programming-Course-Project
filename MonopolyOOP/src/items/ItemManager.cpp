#include "items/ItemManager.h"
#include "players/Player.h"
#include "core/Game.h"

ItemManager::ItemManager(QObject* parent) : QObject(parent) {
    // 注册道具工厂
    m_itemFactories[0] = []() { return static_cast<Item*>(new LeaveSlip()); };
    m_itemFactories[1] = []() { return static_cast<Item*>(new VirtualFunctionCard()); };
    m_itemFactories[2] = []() { return static_cast<Item*>(new LuckCard()); };
    m_itemFactories[3] = []() { return static_cast<Item*>(new DeleteCard()); };
}

Item* ItemManager::createItem(int itemId) {
    if (m_itemFactories.contains(itemId)) {
        return m_itemFactories[itemId]();
    }
    return nullptr;
}

bool ItemManager::useItem(Player* player, int itemId, Game* game) {
    if (!player->hasItem(itemId)) {
        emit messageLogged("你没有这个道具！");
        return false;
    }

    Item* item = createItem(itemId);
    if (!item) {
        emit messageLogged("无效的道具！");
        return false;
    }

    bool success = item->use(player, game);
    if (success) {
        player->removeItem(itemId);
        emit itemUsed(player, item->getName(), true);
        emit messageLogged(QString("使用了 %1").arg(item->getName()));
    } else {
        emit itemUsed(player, item->getName(), false);
        emit messageLogged(QString("使用 %1 失败！").arg(item->getName()));
    }

    delete item;
    return success;
}

QString ItemManager::getItemName(int itemId) const {
    switch (itemId) {
    case 0: return "假条";
    case 1: return "虚函数卡";
    case 2: return "手气卡";
    case 3: return "delete卡";
    default: return "未知道具";
    }
}

QString ItemManager::getItemDescription(int itemId) const {
    switch (itemId) {
    case 0: return "免除一次上机课";
    case 1: return "在继承格阻断基类效果";
    case 2: return "重新掷骰子";
    case 3: return "删除一个对手的地产";
    default: return "";
    }
}

int ItemManager::getItemPrice(int itemId) const {
    switch (itemId) {
    case 0: return 100;
    case 1: return 150;
    case 2: return 80;
    case 3: return 200;
    default: return 0;
    }
}
