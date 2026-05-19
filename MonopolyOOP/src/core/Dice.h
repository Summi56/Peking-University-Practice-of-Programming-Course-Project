#pragma once
#include <QObject>
#include <random>

/**
 * @brief 骰子类 - 负责生成随机骰子点数
 *
 * 使用C++11随机数引擎，避免rand()的确定性问题
 * 同时支持普通骰子和"手气卡"重新掷骰功能
 */
class Dice : public QObject {
    Q_OBJECT
public:
    explicit Dice(QObject *parent = nullptr);

    // 掷骰子，返回1-6的随机数
    int roll();

    // 批量掷骰（用于函数模板格等特殊机制）
    QVector<int> rollMultiple(int count);

    // 获取最后一次掷出的点数
    int lastRoll() const { return m_lastRoll; }

    // 获取最后两次掷骰是否相同（用于判断连掷）
    bool isDouble() const { return m_isDouble; }

signals:
    void diceRolled(int value);
    void doubleRolled(int value);

private:
    std::mt19937 m_rng;
    std::uniform_int_distribution<int> m_dist;
    int m_lastRoll = 0;
    int m_previousRoll = 0;
    bool m_isDouble = false;
};
