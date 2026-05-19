#pragma once
#include "tiles/Tile.h"

/**
 * @brief 函数模板格 - 类似原版赌场，展示函数模板机制
 *
 * 对应文档中的"函数模板"格：
 * - 类似赌场机制
 * - 玩家可以下注，根据随机结果获得不同奖励
 * - 类比函数模板 sort<T>() 可以对不同类型排序
 */
class FunctionTemplateTile : public Tile {
    Q_OBJECT
public:
    explicit FunctionTemplateTile(int id, const QString& name,
                                  const QString& description,
                                  QObject* parent = nullptr);

    void onPlayerEnter(Player* player, Game* game) override;

    // 赌博选项 - 类比函数模板的不同实例化
    struct GambleOption {
        QString name;        // 选项名称
        int minBet;          // 最小下注
        int maxBet;          // 最大下注
        QString description;
    };

    QVector<GambleOption> getOptions() const { return m_options; }

    // 执行赌博
    QString executeGamble(Player* player, int optionIndex, int bet);

    QColor getColor() const override;
    QString getDisplayText() const override;

private:
    QVector<GambleOption> m_options;
    std::mt19937 m_rng;
};
