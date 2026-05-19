#include "ui/GameBoardWidget.h"
#include "core/Game.h"
#include "core/GameBoard.h"
#include "tiles/Tile.h"
#include "players/Player.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtMath>

GameBoardWidget::GameBoardWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(500, 500);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GameBoardWidget::setGame(Game* game) {
    m_game = game;
    update();
}

void GameBoardWidget::updatePlayerPosition(Player* player, int position) {
    Q_UNUSED(player)
    Q_UNUSED(position)
    update();
}

void GameBoardWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 背景
    painter.fillRect(rect(), QColor(240, 240, 240));

    if (!m_game) return;

    drawBoard(painter);
    drawTiles(painter);
    drawPlayers(painter);
}

void GameBoardWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    update();
}

void GameBoardWidget::drawBoard(QPainter& painter) {
    // 绘制棋盘边框
    QRect boardRect = rect().adjusted(20, 20, -20, -20);
    painter.setPen(QPen(QColor(100, 100, 100), 3));
    painter.setBrush(QColor(200, 220, 200));
    painter.drawRoundedRect(boardRect, 15, 15);
}

void GameBoardWidget::drawTiles(QPainter& painter) {
    if (!m_game || !m_game->getBoard()) return;

    GameBoard* board = m_game->getBoard();
    int tileCount = board->tileCount();
    if (tileCount == 0) return;

    m_tileCenters.resize(tileCount);
    m_tileRects.resize(tileCount);

    QRect boardRect = rect().adjusted(30, 30, -30, -30);
    int cols = 6; // 每行6个格子
    int rows = 4; // 4行

    for (int i = 0; i < tileCount; ++i) {
        QRect tileRect = getTileRect(i);
        m_tileRects[i] = tileRect;
        m_tileCenters[i] = tileRect.center();

        Tile* tile = board->getTile(i);
        if (!tile) continue;

        // 绘制格子背景
        QColor tileColor = tile->getColor();

        // 高亮当前有玩家的格子
        for (Player* p : m_game->getAllPlayers()) {
            if (p->getPosition() == i) {
                tileColor = tileColor.lighter(110);
                break;
            }
        }

        painter.setBrush(tileColor);
        painter.setPen(QPen(QColor(80, 80, 80), 1));
        painter.drawRoundedRect(tileRect, 8, 8);

        // 绘制格子名称
        painter.setPen(Qt::black);
        QFont font = painter.font();
        font.setPointSize(8);
        painter.setFont(font);

        QString displayText = tile->getDisplayText();
        QRect textRect = tileRect.adjusted(4, 4, -4, -4);
        painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, displayText);
    }
}

void GameBoardWidget::drawPlayers(QPainter& painter) {
    if (!m_game) return;

    for (Player* player : m_game->getAllPlayers()) {
        int pos = player->getPosition();
        if (pos < m_tileCenters.size()) {
            QPoint center = m_tileCenters[pos];
            QColor color = player->getPlayerColor();

            // 绘制玩家棋子（圆形）
            int offset = player->isAI() ? 15 : -15;
            QPoint playerPos = center + QPoint(offset, -10);

            painter.setBrush(color);
            painter.setPen(QPen(Qt::black, 2));
            painter.drawEllipse(playerPos, 12, 12);

            // 绘制玩家名称
            painter.setPen(Qt::black);
            QFont font = painter.font();
            font.setPointSize(7);
            painter.setFont(font);
            painter.drawText(playerPos.x() - 15, playerPos.y() - 15, 30, 12,
                             Qt::AlignCenter, player->getName().left(2));
        }
    }
}

QRect GameBoardWidget::getTileRect(int index) const {
    QRect boardRect = rect().adjusted(35, 35, -35, -35);
    int tileCount = m_game ? m_game->getBoard()->tileCount() : 20;

    // 环形布局
    int cols = 6;
    int rows = 4;
    int tileW = boardRect.width() / cols;
    int tileH = boardRect.height() / rows;

    // 计算环形路径上的位置
    int row, col;
    if (index < cols) {
        // 顶行：从左到右
        row = 0;
        col = index;
    } else if (index < cols + rows - 1) {
        // 右列：从上到下
        row = index - cols + 1;
        col = cols - 1;
    } else if (index < cols * 2 + rows - 2) {
        // 底行：从右到左
        row = rows - 1;
        col = cols * 2 + rows - 3 - index;
    } else {
        // 左列：从下到上
        row = tileCount - index;
        col = 0;
    }

    int x = boardRect.left() + col * tileW;
    int y = boardRect.top() + row * tileH;

    return QRect(x, y, tileW - 4, tileH - 4);
}

QPoint GameBoardWidget::getTileCenter(int index) const {
    if (index < m_tileRects.size()) {
        return m_tileRects[index].center();
    }
    return QPoint();
}
