#include "players/AIPlayer.h"
#include "core/Game.h"
#include "tiles/PropertyTile.h"
#include <QRandomGenerator>
#include <QTime>
#include <QColor>

AIPlayer::AIPlayer(const QString& name, QObject* parent)
    : Player(name, parent) {
    std::random_device rd;
    m_rng.seed(rd() ^ QTime::currentTime().msec());
}

void AIPlayer::makeDecision(Game* game) {
    Q_UNUSED(game)
    // AI决策逻辑在Game中处理
    // 这里定义AI的基本策略倾向
}

bool AIPlayer::shouldBuyProperty(int price, int funds) {
    // AI：只要资金充足就购买（70%概率）
    if (funds > price * 2) {
        return QRandomGenerator::global()->bounded(100) < 70;
    }
    return false;
}

bool AIPlayer::shouldUpgradeProperty(int cost, int funds) {
    // AI：保留足够资金的情况下升级（40%概率）
    if (funds > cost * 3) {
        return QRandomGenerator::global()->bounded(100) < 40;
    }
    return false;
}

bool AIPlayer::shouldUseItem(int itemId) {
    // AI：很少主动使用道具（20%概率）
    Q_UNUSED(itemId)
    return QRandomGenerator::global()->bounded(100) < 20;
}

QColor AIPlayer::getPlayerColor() const {
    return QColor(250, 80, 80); // 红色
}
