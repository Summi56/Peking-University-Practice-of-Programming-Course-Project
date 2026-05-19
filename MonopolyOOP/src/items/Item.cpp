#include "items/Item.h"
#include "players/Player.h"
#include "core/Game.h"
#include "core/Dice.h"
#include <QDebug>

Item::Item(ItemType type, const QString& name,
           const QString& description, int price,
           QObject* parent)
    : QObject(parent),
      m_type(type), m_name(name), m_description(description), m_price(price) {
}

// ========== 假条 ==========
LeaveSlip::LeaveSlip() : Item(LEAVE_SLIP, "假条",
                              "免除一次上机课（跳过惩罚）", 100, nullptr) {
}

bool LeaveSlip::use(Player* user, Game* game) {
    Q_UNUSED(game)
    // 假条效果是预防性的，在将要被跳过时自动使用
    // 也可以主动使用来抵消当前的跳过状态
    if (user->isSkipped()) {
        user->setSkipped(false);
        return true;
    }
    // 没有跳过状态也可以使用，获得一个"免跳"buff（简化实现）
    return true;
}

// ========== 虚函数卡 ==========
VirtualFunctionCard::VirtualFunctionCard()
    : Item(VIRTUAL_CARD,
           "虚函数卡",
           "在继承格阻断基类效果（虚函数机制）", 150, nullptr) {
}

bool VirtualFunctionCard::use(Player* user, Game* game) {
    Q_UNUSED(user)
    Q_UNUSED(game)
    // 虚函数卡的效果在InheritanceTile::onPlayerEnter中检查
    // 玩家标记一个状态，下次经过继承格时自动生效
    return true;
}

// ========== 手气卡 ==========
LuckCard::LuckCard() : Item(LUCK_CARD,
                            "手气卡",
                            "重新掷骰子", 80, nullptr) {
}

bool LuckCard::use(Player* user, Game* game) {
    // 重新掷骰子并移动
    int roll = game->getDice()->roll();
    int oldPos = user->getPosition();
    int newPos = game->getBoard()->movePlayer(user, roll);
    user->setPosition(newPos);
    return true;
}

// ========== delete卡 ==========
DeleteCard::DeleteCard() : Item(DELETE_CARD,
                                "delete卡",
                                "删除一个对手的对象（地产）", 200, nullptr) {
}

bool DeleteCard::use(Player* user, Game* game) {
    // 选择一个对手的地产删除
    // 简化：删除第一个对手的最近地产
    for (Player* p : game->getAllPlayers()) {
        if (p != user && !p->getProperties().isEmpty()) {
            PropertyTile* target = p->getProperties().first();
            p->removeProperty(target);
            target->setOwner(nullptr);
            target->reset();
            return true;
        }
    }
    return false; // 没有可删除的地产
}
