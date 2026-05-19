#pragma once
#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>

/**
 * @brief 骰子显示控件
 *
 * 用动画效果显示掷骰结果
 */
class DiceWidget : public QWidget {
    Q_OBJECT
public:
    explicit DiceWidget(QWidget* parent = nullptr);

    void startRolling();
    void showResult(int value);

private:
    void setupUI();
    QString getDiceText(int value) const;

    QLabel* m_lblDice;
    QPropertyAnimation* m_animation = nullptr;
    int m_currentValue = 1;
};
