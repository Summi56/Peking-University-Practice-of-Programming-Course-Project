#include "tiles/DestinyTile.h"
#include "players/Player.h"
#include "core/Game.h"
#include <QRandomGenerator>
#include <QTime>

DestinyTile::DestinyTile(int id, const QString& name, QObject* parent)
    : Tile(id, name, "", DESTINY, parent) {
    std::random_device rd;
    m_rng.seed(rd() ^ QTime::currentTime().msec());
    initEvents();
}

void DestinyTile::initEvents() {
    m_events.append({
        {"内存泄漏：忘记delete指针，损失100比特修复",
         -100, false,
         "C++中new分配的内存必须用delete释放，否则造成内存泄漏"},

        {"野指针：使用了未初始化的指针，损失80比特",
         -80, false,
         "指针声明后应初始化为nullptr，避免野指针"},

        {"段错误：数组越界访问，损失60比特",
         -60, false,
         "C++不检查数组越界，访问时要确保索引合法"},

        {"编译成功：代码一次通过，奖励100比特",
         100, false,
         "良好的编码习惯可以减少编译错误"},

        {"重构成功：使用了设计模式，奖励150比特",
         150, false,
         "设计模式是解决常见问题的最佳实践"},

        {"Stack Overflow：递归没有终止条件，跳过一回合",
         -50, true,
         "递归函数必须有终止条件，否则会栈溢出"},

        {"多态妙用：正确使用虚函数，奖励80比特",
         80, false,
         "虚函数是实现运行时多态的关键机制"},

        {"RAII：资源获取即初始化，奖励70比特",
         70, false,
         "RAII是C++资源管理的核心思想"},
    });
}

void DestinyTile::onPlayerEnter(Player* player, Game* game) {
    Q_UNUSED(game)

    std::uniform_int_distribution<int> dist(0, m_events.size() - 1);
    int index = dist(m_rng);
    const DestinyEvent& event = m_events[index];

    emit tileEventTriggered(QString("=== 命运：%1 ===\n%2")
                            .arg(m_name).arg(event.description));

    if (event.moneyChange > 0) {
        player->addFunds(event.moneyChange);
        emit tileEventTriggered(
            QString("奖励 %1 比特！\n[知识] %2")
            .arg(event.moneyChange).arg(event.explanation));
    } else {
        player->deductFunds(qAbs(event.moneyChange));
        emit tileEventTriggered(
            QString("损失 %1 比特！\n[教训] %2")
            .arg(qAbs(event.moneyChange)).arg(event.explanation));
    }

    if (event.skipNextTurn) {
        player->setSkipped(true);
        emit tileEventTriggered("下回合将被跳过！");
    }
}

QColor DestinyTile::getColor() const {
    return QColor(255, 100, 100); // 红色
}

QString DestinyTile::getDisplayText() const {
    return QString("%1\n[命运]").arg(m_name);
}
