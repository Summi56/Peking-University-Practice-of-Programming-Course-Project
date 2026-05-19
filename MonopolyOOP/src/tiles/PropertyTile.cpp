#include "tiles/PropertyTile.h"
#include "players/Player.h"
#include "core/Game.h"
#include <QDebug>

PropertyTile::PropertyTile(int id, const QString& name, const QString& description,
                           int price, int baseRent, QObject* parent)
    : Tile(id, name, description, PROPERTY, parent),
      m_price(price), m_rent(baseRent), m_upgradeCost(price / 2),
      m_upgradeLevel(0), m_owner(nullptr) {
}

void PropertyTile::onPlayerEnter(Player* player, Game* game) {
    Q_UNUSED(game)

    if (m_owner == nullptr) {
        // 无人拥有的地产，等待玩家决策
        emit tileEventTriggered(QString("%1 可以购买，价格: %2 比特")
                                .arg(m_name).arg(m_price));
    } else if (m_owner == player) {
        // 自己的地产，可以升级
        if (m_upgradeLevel < 3) {
            emit tileEventTriggered(QString("欢迎来到你的 %1 （等级 %2），升级费用: %3")
                                    .arg(m_name).arg(m_upgradeLevel).arg(m_upgradeCost));
        } else {
            emit tileEventTriggered(QString("%1 已满级（等级 3）！").arg(m_name));
        }
    } else {
        // 需要支付租金
        int rent = calculateRent();
        if (player->deductFunds(rent)) {
            m_owner->addFunds(rent);
            emit tileEventTriggered(QString("%1 属于 %2，你支付了 %3 比特租金！")
                                    .arg(m_name).arg(m_owner->getName()).arg(rent));
        } else {
            // 资金不足，破产处理
            emit tileEventTriggered(QString("%1 资金不足支付 %2 比特租金！")
                                    .arg(player->getName()).arg(rent));
        }
    }
}

void PropertyTile::onPlayerPass(Player* player, Game* game) {
    Q_UNUSED(game)
    // 经过自己的地产可以小幅升级
    if (m_owner == player && m_upgradeLevel < 3) {
        // 可选择是否升级，在Game中处理
    }
}

void PropertyTile::upgrade() {
    if (m_upgradeLevel < 3) {
        m_upgradeLevel++;
        m_rent = static_cast<int>(m_rent * 1.5);
        m_upgradeCost = static_cast<int>(m_upgradeCost * 1.5);
    }
}

void PropertyTile::reset() {
    m_owner = nullptr;
    // 恢复初始租金
    m_upgradeLevel = 0;
    m_upgradeCost = m_price / 2;
}

QColor PropertyTile::getColor() const {
    if (m_owner) {
        return QColor(100, 200, 100); // 有主：绿色
    }
    return QColor(150, 220, 150); // 无主：浅绿
}

QString PropertyTile::getDisplayText() const {
    if (m_owner) {
        return QString("%1\nLv.%2\n租:%3").arg(m_name).arg(m_upgradeLevel).arg(m_rent);
    }
    return QString("%1\n%2比特").arg(m_name).arg(m_price);
}
