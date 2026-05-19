#pragma once
#include "tiles/Tile.h"
#include <QVector>

/**
 * @brief 机会格 - 触发C++ OOP知识问答
 *
 * 对应文档中的"机会与命运"格：
 * - 触发OOP相关问题答题
 * - 答对获得道具卡（假条、虚函数卡、手气卡、delete卡）
 * - 答错可能触发惩罚
 */
class ChanceTile : public Tile {
    Q_OBJECT
public:
    explicit ChanceTile(int id, const QString& name, QObject* parent = nullptr);

    void onPlayerEnter(Player* player, Game* game) override;

    // 处理答题结果
    void handleAnswer(Player* player, bool correct);

    QColor getColor() const override;
    QString getDisplayText() const override;

signals:
    void questionReady(QString question, QVector<QString> options, int correctIndex);
    void answerResult(bool correct, QString explanation);

private:
    // 奖励表
    QVector<QString> m_rewards;
};
