#include "tiles/Tile.h"
#include <QColor>

Tile::Tile(int id, const QString& name, const QString& description,
           TileType type, QObject* parent)
    : QObject(parent), m_id(id), m_name(name),
      m_description(description), m_type(type) {
}

void Tile::onPlayerPass(Player* player, Game* game) {
    // 默认经过不触发任何事件
    Q_UNUSED(player)
    Q_UNUSED(game)
}

QColor Tile::getColor() const {
    return QColor(200, 200, 200); // 默认灰色
}

void Tile::reset() {
    // 基类无状态需要重置
}

QString Tile::getDisplayText() const {
    return m_name;
}
