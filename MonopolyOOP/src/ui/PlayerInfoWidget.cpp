#include "ui/PlayerInfoWidget.h"
#include "core/Game.h"
#include "players/Player.h"
#include "tiles/PropertyTile.h"
#include "items/ItemManager.h"
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

PlayerInfoWidget::PlayerInfoWidget(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void PlayerInfoWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    m_lblTitle = new QLabel("玩家信息", this);
    QFont titleFont = m_lblTitle->font();
    titleFont.setBold(true);
    titleFont.setPointSize(12);
    m_lblTitle->setFont(titleFont);
    m_lblTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_lblTitle);

    // 为2个玩家创建显示区域
    for (int i = 0; i < 2; ++i) {
        QGroupBox* box = new QGroupBox(this);
        QGridLayout* grid = new QGridLayout(box);

        PlayerDisplay display;
        display.lblName = new QLabel("玩家 -", box);
        display.lblFunds = new QLabel("资金: 0 比特", box);
        display.lblProperties = new QLabel("地产: 0", box);
        display.lblItems = new QLabel("道具: 无", box);
        display.barAssets = new QProgressBar(box);
        display.barAssets->setRange(0, 5000);
        display.barAssets->setValue(0);
        display.barAssets->setFormat("总资产: %v");

        QFont nameFont = display.lblName->font();
        nameFont.setBold(true);
        display.lblName->setFont(nameFont);

        grid->addWidget(display.lblName, 0, 0, 1, 2);
        grid->addWidget(display.lblFunds, 1, 0);
        grid->addWidget(display.lblProperties, 1, 1);
        grid->addWidget(display.lblItems, 2, 0, 1, 2);
        grid->addWidget(display.barAssets, 3, 0, 1, 2);

        mainLayout->addWidget(box);
        m_displays.append(display);
    }

    mainLayout->addStretch();
}

void PlayerInfoWidget::setGame(Game* game) {
    m_game = game;

    // 连接信号
    if (game) {
        connect(game, &Game::turnChanged, this, [this]() {
            updateDisplay();
        });
    }

    updateDisplay();
}

void PlayerInfoWidget::updateDisplay() {
    if (!m_game) return;

    for (int i = 0; i < m_game->getPlayerCount() && i < m_displays.size(); ++i) {
        Player* player = m_game->getPlayer(i);
        if (!player) continue;

        PlayerDisplay& display = m_displays[i];

        // 名称（高亮当前玩家）
        QString nameText = player->getName();
        if (m_game->getCurrentPlayer() == player) {
            nameText += " [当前]";
            display.lblName->setStyleSheet("color: #2196F3;");
        } else {
            display.lblName->setStyleSheet("color: black;");
        }
        display.lblName->setText(nameText);

        // 资金
        display.lblFunds->setText(QString("资金: %1 比特").arg(player->getFunds()));

        // 地产数量
        display.lblProperties->setText(
            QString("地产: %1 (价值%2)")
            .arg(player->getPropertyCount())
            .arg(player->getTotalPropertyValue()));

        // 道具
        QMap<int, int> items = player->getItems();
        if (items.isEmpty()) {
            display.lblItems->setText("道具: 无");
        } else {
            QStringList itemList;
            ItemManager* mgr = m_game->getItemManager();
            for (auto it = items.begin(); it != items.end(); ++it) {
                itemList.append(QString("%1x%2").arg(mgr->getItemName(it.key())).arg(it.value()));
            }
            display.lblItems->setText("道具: " + itemList.join(", "));
        }

        // 总资产进度条
        display.barAssets->setValue(player->getTotalAssets());
    }
}

void PlayerInfoWidget::onFundsChanged(Player* player, int newAmount) {
    Q_UNUSED(player)
    Q_UNUSED(newAmount)
    updateDisplay();
}

void PlayerInfoWidget::onPropertyChanged(Player* player) {
    Q_UNUSED(player)
    updateDisplay();
}
