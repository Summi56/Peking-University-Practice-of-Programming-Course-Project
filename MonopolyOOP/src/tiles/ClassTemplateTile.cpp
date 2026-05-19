#include "tiles/ClassTemplateTile.h"
#include "players/Player.h"
#include "core/Game.h"
#include <QRandomGenerator>

ClassTemplateTile::ClassTemplateTile(int id, const QString& name,
                                     const QString& description,
                                     int basePrice, QObject* parent)
    : Tile(id, name, description, CLASS_TEMPLATE, parent),
      m_basePrice(basePrice), m_instantiated(false),
      m_currentRent(0), m_owner(nullptr) {

    // 定义实例化选项 - 类比 vector<T> 的不同实例化
    m_options.append({"vector<int>", basePrice, 30, QColor(255, 180, 100)});
    m_options.append({"vector<double>", basePrice + 50, 40, QColor(255, 150, 80)});
    m_options.append({"vector<string>", basePrice + 100, 50, QColor(255, 120, 60)});
    m_options.append({"vector<Shape*>", basePrice + 200, 70, QColor(255, 100, 50)});
}

void ClassTemplateTile::onPlayerEnter(Player* player, Game* game) {
    Q_UNUSED(game)

    if (!m_instantiated) {
        emit tileEventTriggered(
            QString("=== 类模板实例化 ===\n%1 尚未实例化\n选择类型来实例化这个模板！")
            .arg(m_name));

        if (m_owner == nullptr) {
            // 显示选项给玩家选择
            QString options;
            for (int i = 0; i < m_options.size(); ++i) {
                options += QString("%1. %2 (费用:%3, 收租:%4)\n")
                           .arg(i + 1).arg(m_options[i].typeName)
                           .arg(m_options[i].cost).arg(m_options[i].rent);
            }
            emit tileEventTriggered(options);
        }
    } else {
        // 已经实例化，像普通地产一样处理
        if (m_owner == nullptr) {
            emit tileEventTriggered(QString("%1<%2> 可以购买，价格: %3 比特")
                                    .arg(m_name).arg(m_instanceType).arg(m_basePrice));
        } else if (m_owner == player) {
            emit tileEventTriggered(QString("欢迎来到你的 %1<%2>")
                                    .arg(m_name).arg(m_instanceType));
        } else {
            int rent = m_currentRent * (1 + (m_owner->getPropertyCount() / 10));
            if (player->deductFunds(rent)) {
                m_owner->addFunds(rent);
                emit tileEventTriggered(
                    QString("%1<%2> 属于 %3，你支付了 %4 比特租金！")
                    .arg(m_name).arg(m_instanceType)
                    .arg(m_owner->getName()).arg(rent));
            }
        }
    }
}

void ClassTemplateTile::instantiate(Player* player, int optionIndex) {
    if (optionIndex < 0 || optionIndex >= m_options.size()) return;

    const InstantiationOption& opt = m_options[optionIndex];
    if (player->deductFunds(opt.cost)) {
        m_instantiated = true;
        m_instanceType = opt.typeName;
        m_currentRent = opt.rent;
        m_owner = player;
        emit tileEventTriggered(
            QString("成功实例化 %1<%2>！花费 %3 比特")
            .arg(m_name).arg(m_instanceType).arg(opt.cost));
    }
}

void ClassTemplateTile::reset() {
    m_instantiated = false;
    m_instanceType.clear();
    m_currentRent = 0;
    m_owner = nullptr;
}

QColor ClassTemplateTile::getColor() const {
    if (m_instantiated) {
        if (m_owner) return QColor(255, 140, 80);
        return QColor(255, 180, 120);
    }
    return QColor(220, 200, 180); // 未实例化：灰褐色
}

QString ClassTemplateTile::getDisplayText() const {
    if (m_instantiated) {
        return QString("%1\n<%2>\n租:%3")
               .arg(m_name).arg(m_instanceType).arg(m_currentRent);
    }
    return QString("%1\n[未实例化]\n%2比特起").arg(m_name).arg(m_basePrice);
}
