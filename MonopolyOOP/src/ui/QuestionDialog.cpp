#include "ui/QuestionDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>

QuestionDialog::QuestionDialog(QWidget* parent) : QDialog(parent) {
    setupUI();
    setWindowTitle("OOP知识挑战");
    setMinimumSize(500, 350);
}

void QuestionDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 分类和难度
    QHBoxLayout* infoLayout = new QHBoxLayout();
    m_lblCategory = new QLabel("分类: -", this);
    m_lblDifficulty = new QLabel("难度: -", this);
    infoLayout->addWidget(m_lblCategory);
    infoLayout->addStretch();
    infoLayout->addWidget(m_lblDifficulty);
    mainLayout->addLayout(infoLayout);

    // 题目
    m_lblQuestion = new QLabel("题目内容", this);
    QFont qFont = m_lblQuestion->font();
    qFont.setPointSize(12);
    qFont.setBold(true);
    m_lblQuestion->setFont(qFont);
    m_lblQuestion->setWordWrap(true);
    m_lblQuestion->setStyleSheet("QLabel { background-color: #f5f5f5; padding: 15px; "
                                 "border-radius: 8px; border: 1px solid #ddd; }");
    mainLayout->addWidget(m_lblQuestion);

    // 选项按钮组
    m_btnGroup = new QButtonGroup(this);
    for (int i = 0; i < 4; ++i) {
        QPushButton* btn = new QPushButton(QString("选项 %1").arg(i + 1), this);
        btn->setStyleSheet(
            "QPushButton { background-color: #E3F2FD; padding: 10px; "
            "font-size: 12px; text-align: left; border-radius: 6px; }"
            "QPushButton:hover { background-color: #BBDEFB; }"
        );
        btn->setMinimumHeight(40);
        m_btnOptions.append(btn);
        m_btnGroup->addButton(btn, i);
        mainLayout->addWidget(btn);
    }

    connect(m_btnGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &QuestionDialog::onAnswerClicked);

    // 计时器
    m_lblTimer = new QLabel("剩余时间: 30秒", this);
    m_lblTimer->setAlignment(Qt::AlignCenter);
    QFont tFont = m_lblTimer->font();
    tFont.setBold(true);
    m_lblTimer->setFont(tFont);
    mainLayout->addWidget(m_lblTimer);
}

void QuestionDialog::setQuestion(const QString& question,
                                 const QVector<QString>& options,
                                 const QString& category,
                                 int difficulty) {
    m_lblCategory->setText(QString("分类: %1").arg(category));
    m_lblDifficulty->setText(QString("难度: %1").arg(QString("★").repeated(difficulty)));
    m_lblQuestion->setText(question);

    for (int i = 0; i < 4 && i < options.size(); ++i) {
        m_btnOptions[i]->setText(QString("%1. %2")
                                 .arg(QChar('A' + i)).arg(options[i]));
        m_btnOptions[i]->setEnabled(true);
    }

    m_timeLeft = 30;
    m_lblTimer->setText("剩余时间: 30秒");
    startTimer();
}

void QuestionDialog::startTimer() {
    QTimer* timer = new QTimer(this);
    timer->setInterval(1000);

    connect(timer, &QTimer::timeout, this, [this, timer]() {
        m_timeLeft--;
        m_lblTimer->setText(QString("剩余时间: %1秒").arg(m_timeLeft));

        if (m_timeLeft <= 10) {
            m_lblTimer->setStyleSheet("color: red;");
        }

        if (m_timeLeft <= 0) {
            timer->stop();
            timer->deleteLater();
            onTimeout();
        }
    });

    timer->start();
}

void QuestionDialog::onAnswerClicked(int index) {
    emit answerSubmitted(index);

    // 视觉反馈
    for (int i = 0; i < m_btnOptions.size(); ++i) {
        m_btnOptions[i]->setEnabled(false);
    }

    // 延迟关闭
    QTimer::singleShot(1500, this, &QDialog::accept);
}

void QuestionDialog::onTimeout() {
    emit answerSubmitted(-1); // -1 表示超时
    reject();
}
