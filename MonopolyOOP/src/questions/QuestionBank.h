#pragma once
#include <QObject>
#include <QVector>
#include <QRandomGenerator>
#include "questions/Question.h"

/**
 * @brief OOP题库管理器 - 管理所有C++ OOP相关的题目
 *
 * 封装了题目的加载、随机抽取、分类等功能
 */
class QuestionBank : public QObject {
    Q_OBJECT
public:
    explicit QuestionBank(QObject *parent = nullptr);

    // 获取随机题目
    Question getRandomQuestion();

    // 按分类获取题目
    Question getRandomQuestionByCategory(const QString& category);

    // 获取指定难度的题目
    Question getRandomQuestionByDifficulty(int difficulty);

    // 获取所有分类
    QVector<QString> getCategories() const;

    // 获取题库大小
    int getQuestionCount() const { return m_questions.size(); }

signals:
    void questionLoaded(Question question);

private:
    QVector<Question> m_questions;

    // 初始化内置题目
    void initQuestions();

    // 添加题目
    void addQuestion(const QString& text, const QVector<QString>& options,
                     int correct, const QString& explanation,
                     const QString& category, int difficulty);
};
