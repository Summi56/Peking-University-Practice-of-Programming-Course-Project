#pragma once
#include "players/Player.h"
#include <random>

/**
 * @brief AI玩家类 - 电脑控制的角色
 *
 * 按照固定策略或简单规则行动：
 * - 有资金就购买地产
 * - 有一定概率升级地产
 * - 随机使用道具
 */
class AIPlayer : public Player {
    Q_OBJECT
public:
    explicit AIPlayer(const QString& name, QObject* parent = nullptr);

    // AI自动决策
    void makeDecision(Game* game) override;

    bool isAI() const override { return true; }

    QColor getPlayerColor() const override;

private:
    std::mt19937 m_rng;

    // AI决策辅助函数
    bool shouldBuyProperty(int price, int funds);
    bool shouldUpgradeProperty(int cost, int funds);
    bool shouldUseItem(int itemId);
};
