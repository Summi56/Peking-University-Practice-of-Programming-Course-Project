#include "players/HumanPlayer.h"
#include "core/Game.h"
#include <QColor>

HumanPlayer::HumanPlayer(const QString& name, QObject* parent)
    : Player(name, parent) {
}

void HumanPlayer::makeDecision(Game* game) {
    // 人类玩家的决策通过UI交互完成
    // 这里不需要实现，因为UI会调用Game的相应方法
    Q_UNUSED(game)
}

QColor HumanPlayer::getPlayerColor() const {
    return QColor(50, 150, 250); // 蓝色
}
