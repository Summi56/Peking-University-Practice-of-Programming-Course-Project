#pragma once
#include "tiles/Tile.h"

/**
 * @brief 扣款格 - 停留在特定格子扣除资金
 *
 * 对应文档中的"扣款"类格子：
 * - 如"疯狂星期四"格扣除50个比特
 * - 类比编程中的各种"坑"
 */
class PenaltyTile : public Tile {
    Q_OBJECT
public:
    explicit PenaltyTile(int id, const QString& name,
                         const QString& description,
                         int penalty, QObject* parent = nullptr);

    void onPlayerEnter(Player* player, Game* game) override;

    int getPenalty() const { return m_penalty; }

    QColor getColor() const override;
    QString getDisplayText() const override;

private:
    int m_penalty;
};
