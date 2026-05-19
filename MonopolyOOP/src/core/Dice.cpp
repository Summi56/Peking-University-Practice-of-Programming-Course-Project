#include "core/Dice.h"
#include <QTime>

Dice::Dice(QObject *parent) : QObject(parent) {
    // 使用硬件随机数设备种子初始化Mersenne Twister引擎
    std::random_device rd;
    m_rng.seed(rd() ^ QTime::currentTime().msec());
    m_dist = std::uniform_int_distribution<int>(1, 6);
}

int Dice::roll() {
    m_previousRoll = m_lastRoll;
    m_lastRoll = m_dist(m_rng);
    m_isDouble = (m_lastRoll == m_previousRoll) && (m_lastRoll != 0);

    emit diceRolled(m_lastRoll);
    if (m_isDouble) {
        emit doubleRolled(m_lastRoll);
    }
    return m_lastRoll;
}

QVector<int> Dice::rollMultiple(int count) {
    QVector<int> results;
    for (int i = 0; i < count; ++i) {
        results.append(roll());
    }
    return results;
}
