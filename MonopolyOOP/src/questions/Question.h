#pragma once
#include <QString>
#include <QVector>

/**
 * @brief OOP知识题目结构
 *
 * 封装了一道题目的所有信息
 */
struct Question {
    QString questionText;       // 题目内容
    QVector<QString> options;   // 选项（4个）
    int correctIndex;           // 正确答案索引
    QString explanation;        // 解释说明
    QString category;           // 分类（类/继承/多态/模板等）
    int difficulty;             // 难度 1-3

    Question() : correctIndex(0), difficulty(1) {}

    Question(const QString& text, const QVector<QString>& opts,
             int correct, const QString& expl, const QString& cat, int diff)
        : questionText(text), options(opts), correctIndex(correct),
          explanation(expl), category(cat), difficulty(diff) {
    }
};
