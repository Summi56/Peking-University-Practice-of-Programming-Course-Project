#pragma once
#include "tiles/Tile.h"
#include <QVector>
#include <random>

/**
 * @brief 命运格 - 随机奖励或惩罚
 *
 * 对应文档中的"命运"格：
 * - 随机触发奖励或惩罚事件
 * - 事件内容与C++编程概念相关
 */
class DestinyTile : public Tile {
    Q_OBJECT
public:
    explicit DestinyTile(int id, const QString& name, QObject* parent = nullptr);

    void onPlayerEnter(Player* player, Game* game) override;

    struct DestinyEvent {
        QString description;
        int moneyChange;    // 资金变化（正为奖励，负为惩罚）
        bool skipNextTurn;  // 是否跳过下回合
        QString explanation; // C++知识解释
    };

    QColor getColor() const override;
    QString getDisplayText() const override;

private:
    QVector<DestinyEvent> m_events;
    std::mt19937 m_rng;

    void initEvents();
};
