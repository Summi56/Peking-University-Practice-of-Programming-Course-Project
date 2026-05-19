#include "tiles/InheritanceTile.h"
#include "players/Player.h"
#include "core/Game.h"
#include "core/GameBoard.h"
#include "items/ItemManager.h"

InheritanceTile::InheritanceTile(int id, const QString& name, const QString& description,
                                 int price, int baseRent, int baseTileId, QObject* parent)
    : PropertyTile(id, name, description, price, baseRent, parent),
      m_baseTileId(baseTileId), m_virtualBlocked(false), m_inheritanceBonus(20) {
    m_type = INHERITANCE;
}

void InheritanceTile::onPlayerEnter(Player* player, Game* game) {
    // 先触发基类格子的效果（继承链调用）
    // 这模拟了C++中派生类对象调用基类函数的行为

    emit tileEventTriggered(QString("=== 继承机制触发 ===\n%1 继承自基类格子").arg(m_name));

    // 检查是否使用了虚函数卡
    if (m_virtualBlocked) {
        emit tileEventTriggered("虚函数卡生效！阻断继承链调用！");
        m_virtualBlocked = false; // 使用后重置
    } else {
        // 触发基类效果 - 这是继承的核心演示
        emit tileEventTriggered("调用基类构造函数...触发基类格子效果！");
        // 获取基类格子并触发其效果
        Tile* baseTile = game->getBoard()->getTile(m_baseTileId);
        if (baseTile) {
            PropertyTile* baseProp = dynamic_cast<PropertyTile*>(baseTile);
            if (baseProp && baseProp->getOwner() && baseProp->getOwner() != player) {
                int baseRent = baseProp->calculateRent();
                emit tileEventTriggered(QString("基类格子 %1 收租 %2 比特！")
                                        .arg(baseProp->getName()).arg(baseRent));
                if (player->deductFunds(baseRent)) {
                    baseProp->getOwner()->addFunds(baseRent);
                }
            }
        }
    }

    // 然后触发派生类自己的效果（调用基类的onPlayerEnter）
    PropertyTile::onPlayerEnter(player, game);
}

void InheritanceTile::upgrade() {
    PropertyTile::upgrade();
    // 继承格升级时，增强继承加成
    m_inheritanceBonus += 10;
    emit tileEventTriggered(QString("继承加成提升至 %1%").arg(m_inheritanceBonus));
}

int InheritanceTile::calculateRent() const {
    // 继承格租金 = 基础租金 + 继承加成
    int baseRent = PropertyTile::calculateRent();
    return baseRent + (baseRent * m_inheritanceBonus / 100);
}

QColor InheritanceTile::getColor() const {
    if (m_owner) {
        return QColor(100, 150, 255); // 有主：蓝色
    }
    return QColor(150, 190, 255); // 无主：浅蓝
}

QString InheritanceTile::getDisplayText() const {
    return QString("%1\n(继承)\nLv.%2\n租:%3")
           .arg(m_name).arg(m_upgradeLevel).arg(calculateRent());
}
