#include "tiles/FunctionTemplateTile.h"
#include "players/Player.h"
#include "players/AIPlayer.h"
#include "core/Game.h"
#include <QRandomGenerator>
#include <QTime>

FunctionTemplateTile::FunctionTemplateTile(int id, const QString& name,
                                           const QString& description,
                                           QObject* parent)
    : Tile(id, name, description, FUNCTION_TEMPLATE, parent) {

    // 初始化随机数引擎
    std::random_device rd;
    m_rng.seed(rd() ^ QTime::currentTime().msec());

    // 定义赌博选项 - 类比不同函数模板实例化的结果
    m_options.append({"sort<int>", 10, 100,
                      "对int数组排序，简单高效"});
    m_options.append({"sort<double>", 20, 200,
                      "对double数组排序，精度更高"});
    m_options.append({"sort<custom_class>", 50, 500,
                      "对自定义类排序，风险高收益大"});
}

void FunctionTemplateTile::onPlayerEnter(Player* player, Game* game) {
    Q_UNUSED(game)

    emit tileEventTriggered(
        QString("=== 函数模板特化 ===\n欢迎来到 %1\n"
                "函数模板在不同类型上的表现不同，来试试运气！")
        .arg(m_name));

    // AI自动选择
    AIPlayer* ai = dynamic_cast<AIPlayer*>(player);
    if (ai) {
        int choice = QRandomGenerator::global()->bounded(m_options.size());
        int bet = m_options[choice].minBet;
        QString result = executeGamble(player, choice, bet);
        emit tileEventTriggered(result);
    }
}

QString FunctionTemplateTile::executeGamble(Player* player, int optionIndex, int bet) {
    if (optionIndex < 0 || optionIndex >= m_options.size()) {
        return "无效的选择！";
    }

    const GambleOption& opt = m_options[optionIndex];
    if (bet < opt.minBet || bet > opt.maxBet) {
        return QString("下注金额必须在 %1 到 %2 之间！").arg(opt.minBet).arg(opt.maxBet);
    }

    if (player->getFunds() < bet) {
        return "资金不足！";
    }

    player->deductFunds(bet);

    // 随机结果 - 不同类型的特化有不同的胜率
    std::uniform_int_distribution<int> dist(1, 100);
    int roll = dist(m_rng);

    int winThreshold;
    switch (optionIndex) {
    case 0: winThreshold = 60; break; // sort<int> - 60%胜率
    case 1: winThreshold = 50; break; // sort<double> - 50%胜率
    case 2: winThreshold = 35; break; // sort<custom_class> - 35%胜率
    default: winThreshold = 50;
    }

    if (roll <= winThreshold) {
        // 赢了 - 收益与风险成正比
        int multiplier = 100 / winThreshold;
        int winnings = bet * multiplier;
        player->addFunds(winnings);
        return QString("模板实例化成功！%1 运行完美！\n"
                       "投入 %2 比特，获得 %3 比特！")
               .arg(opt.name).arg(bet).arg(winnings);
    } else {
        // 输了
        return QString("模板实例化失败！%1 编译错误！\n"
                       "损失了 %2 比特...")
               .arg(opt.name).arg(bet);
    }
}

QColor FunctionTemplateTile::getColor() const {
    return QColor(200, 100, 200); // 紫色
}

QString FunctionTemplateTile::getDisplayText() const {
    return QString("%1\n[模板特化]").arg(m_name);
}
