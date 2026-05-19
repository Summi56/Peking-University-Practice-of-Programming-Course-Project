#include "tiles/LabClassTile.h"
#include "players/Player.h"
#include "core/Game.h"

LabClassTile::LabClassTile(int id, const QString& name,
                           const QString& description,
                           QObject* parent)
    : Tile(id, name, description, LAB_CLASS, parent) {
}

void LabClassTile::onPlayerEnter(Player* player, Game* game) {
    Q_UNUSED(game)

    emit tileEventTriggered(
        QString("=== 上机课 ===\n欢迎来到 %1\n"
                "老师：'请大家完成今天的编程实验...'\n"
                "%2 需要专心写代码，下回合暂停！")
        .arg(m_name).arg(player->getName()));

    player->setSkipped(true);

    emit tileEventTriggered(
        "[小贴士] 上机课是巩固C++知识的好机会！\n"
        "使用'假条'道具可以免除一次上机课~");
}

QColor LabClassTile::getColor() const {
    return QColor(150, 100, 200); // 深紫色
}

QString LabClassTile::getDisplayText() const {
    return QString("%1\n[暂停1回合]").arg(m_name);
}
