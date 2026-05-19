#pragma once
#include "tiles/Tile.h"

/**
 * @brief 上机课格 - 走到这一格会被跳过一个回合
 *
 * 对应文档中的"上机课"格：
 * - 模拟编程实验课
 * - 走到这里需要暂停一回合（忙着写代码/找Bug）
 */
class LabClassTile : public Tile {
    Q_OBJECT
public:
    explicit LabClassTile(int id, const QString& name,
                          const QString& description,
                          QObject* parent = nullptr);

    void onPlayerEnter(Player* player, Game* game) override;

    QColor getColor() const override;
    QString getDisplayText() const override;
};
