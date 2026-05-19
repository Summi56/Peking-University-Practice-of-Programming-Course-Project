#include "tiles/PenaltyTile.h"
#include "players/Player.h"
#include "core/Game.h"

PenaltyTile::PenaltyTile(int id, const QString& name,
                         const QString& description,
                         int penalty, QObject* parent)
    : Tile(id, name, description, PENALTY, parent), m_penalty(penalty) {
}

void PenaltyTile::onPlayerEnter(Player* player, Game* game) {
    Q_UNUSED(game)

    emit tileEventTriggered(
        QString("=== %1 ===\n%2\n扣除 %3 比特！")
        .arg(m_name).arg(m_description).arg(m_penalty));

    if (player->deductFunds(m_penalty)) {
        emit tileEventTriggered(
            QString("%1 支付了 %2 比特\n[笑谈] V我50，告诉你C++的秘诀...")
            .arg(player->getName()).arg(m_penalty));
    } else {
        emit tileEventTriggered(
            QString("%1 资金不足！\n疯狂星期四也不能白吃啊...")
            .arg(player->getName()));
    }
}

QColor PenaltyTile::getColor() const {
    return QColor(150, 50, 50); // 深红色
}

QString PenaltyTile::getDisplayText() const {
    return QString("%1\n-%2比特").arg(m_name).arg(m_penalty);
}
