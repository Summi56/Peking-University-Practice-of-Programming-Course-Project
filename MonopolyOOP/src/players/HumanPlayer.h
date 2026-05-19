#pragma once
#include "players/Player.h"

/**
 * @brief 人类玩家类
 *
 * 通过Qt信号与UI交互，等待用户输入做决策
 */
class HumanPlayer : public Player {
    Q_OBJECT
public:
    explicit HumanPlayer(const QString& name, QObject* parent = nullptr);

    // 人类玩家不做自动决策，等待UI输入
    void makeDecision(Game* game) override;

    bool isAI() const override { return false; }

    QColor getPlayerColor() const override;
};
