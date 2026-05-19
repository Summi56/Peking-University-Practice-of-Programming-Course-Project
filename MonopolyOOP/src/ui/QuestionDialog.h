#pragma once
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include <QButtonGroup>

/**
 * @brief OOP问题答题对话框
 *
 * 显示题目和选项，等待玩家回答
 */
class QuestionDialog : public QDialog {
    Q_OBJECT
public:
    explicit QuestionDialog(QWidget* parent = nullptr);

    void setQuestion(const QString& question,
                     const QVector<QString>& options,
                     const QString& category,
                     int difficulty);

signals:
    void answerSubmitted(int selectedIndex);

private slots:
    void onAnswerClicked(int index);
    void onTimeout();

private:
    void setupUI();
    void startTimer();

    QLabel* m_lblCategory;
    QLabel* m_lblDifficulty;
    QLabel* m_lblQuestion;
    QVector<QPushButton*> m_btnOptions;
    QButtonGroup* m_btnGroup;
    QLabel* m_lblTimer;
    int m_timeLeft = 30; // 30秒答题时间
};
