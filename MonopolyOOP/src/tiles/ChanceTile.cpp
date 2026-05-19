#include "tiles/ChanceTile.h"
#include "players/Player.h"
#include "players/AIPlayer.h"
#include "core/Game.h"
#include "questions/QuestionBank.h"
#include "items/ItemManager.h"
#include <QRandomGenerator>

ChanceTile::ChanceTile(int id, const QString& name, QObject* parent)
    : Tile(id, name, "回答C++ OOP问题获得道具！", CHANCE, parent) {
}

void ChanceTile::onPlayerEnter(Player* player, Game* game) {
    emit tileEventTriggered(QString("=== 机会：OOP知识挑战 ===\n"
                                    "回答一个C++面向对象的问题来获得道具！"));

    // 从题库中获取随机问题
    auto question = game->getQuestionBank()->getRandomQuestion();

    // 发送信号给UI显示问题
    QVector<QString> options = question.options;
    emit questionReady(question.questionText, options, question.correctIndex);

    // 注意：回答结果通过Game::answerQuestion处理
    // 这里我们需要等待玩家回答...

    // 对于AI，自动回答
    AIPlayer* ai = dynamic_cast<AIPlayer*>(player);
    if (ai) {
        // AI有50%概率答对（模拟知识水平）
        bool correct = QRandomGenerator::global()->bounded(100) < 50;
        handleAnswer(player, correct);
        if (correct) {
            emit answerResult(true, "AI回答正确！获得道具！");
        } else {
            emit answerResult(false, "AI回答错误！");
        }
    }
}

void ChanceTile::handleAnswer(Player* player, bool correct) {
    if (correct) {
        // 答对获得随机道具
        QVector<int> possibleItems = {0, 1, 2, 3}; // 假条、虚函数卡、手气卡、delete
        int itemId = possibleItems[QRandomGenerator::global()->bounded(possibleItems.size())];

        QString itemName;
        switch (itemId) {
        case 0: itemName = "假条"; break;
        case 1: itemName = "虚函数卡"; break;
        case 2: itemName = "手气卡"; break;
        case 3: itemName = "delete卡"; break;
        }

        player->addItem(itemId);
        emit tileEventTriggered(QString("回答正确！获得道具：%1").arg(itemName));
    } else {
        // 答错惩罚 - 扣除少量资金
        int penalty = 30;
        player->deductFunds(penalty);
        emit tileEventTriggered(
            QString("回答错误！扣除 %1 比特作为惩罚...\n"
                    "提示：多学习C++面向对象知识哦！").arg(penalty));
    }
}

QColor ChanceTile::getColor() const {
    return QColor(255, 220, 100); // 黄色
}

QString ChanceTile::getDisplayText() const {
    return QString("%1\n[答题得道具]").arg(m_name);
}
