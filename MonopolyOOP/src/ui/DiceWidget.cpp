#include "ui/DiceWidget.h"
#include <QVBoxLayout>
#include <QTimer>
#include <QRandomGenerator>
#include <QLabel>
#include <QPropertyAnimation>

DiceWidget::DiceWidget(QWidget* parent) : QWidget(parent) {
    setupUI();

    // 创建动画
    m_animation = new QPropertyAnimation(this, "");
    m_animation->setDuration(1000);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
}

void DiceWidget::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    m_lblDice = new QLabel("🎲 1", this);
    QFont font = m_lblDice->font();
    font.setPointSize(36);
    font.setBold(true);
    m_lblDice->setFont(font);
    m_lblDice->setAlignment(Qt::AlignCenter);
    m_lblDice->setStyleSheet(
        "QLabel { background-color: white; border: 3px solid #333; "
        "border-radius: 20px; padding: 20px; min-width: 100px; min-height: 100px; }"
    );
    layout->addWidget(m_lblDice);
}

void DiceWidget::startRolling() {
    // 骰子滚动动画效果
    QTimer* timer = new QTimer(this);
    int count = 0;

    connect(timer, &QTimer::timeout, this, [this, timer, &count]() {
        int value = QRandomGenerator::global()->bounded(6) + 1;
        m_lblDice->setText(QString("🎲 %1").arg(value));
        count++;
        if (count >= 10) {
            timer->stop();
            timer->deleteLater();
        }
    });

    timer->start(100);
}

void DiceWidget::showResult(int value) {
    QString diceArt;
    switch (value) {
    case 1: diceArt = "⚀"; break;
    case 2: diceArt = "⚁"; break;
    case 3: diceArt = "⚂"; break;
    case 4: diceArt = "⚃"; break;
    case 5: diceArt = "⚄"; break;
    case 6: diceArt = "⚅"; break;
    }
    m_lblDice->setText(QString("%1 %2").arg(diceArt).arg(value));
    m_currentValue = value;
}

QString DiceWidget::getDiceText(int value) const {
    return QString::number(value);
}
