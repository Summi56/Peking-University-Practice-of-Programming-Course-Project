#pragma once
#include "tiles/PropertyTile.h"

/**
 * @brief 继承与派生格 - 展示C++继承机制的游戏元素
 *
 * 对应文档中的"继承与派生"格：
 * - 这个格子是某格子的派生类
 * - 进入这个格子除了触发本格事件外，也会触发基类格的事件
 * - 使用虚函数卡可以在派生类格不触发基类格子事件
 */
class InheritanceTile : public PropertyTile {
    Q_OBJECT
public:
    InheritanceTile(int id, const QString& name, const QString& description,
                    int price, int baseRent, int baseTileId,
                    QObject* parent = nullptr);

    // 重写虚函数 - 进入时同时触发基类效果
    void onPlayerEnter(Player* player, Game* game) override;

    // 获取基类格子ID
    int getBaseTileId() const { return m_baseTileId; }

    // 检查是否使用了虚函数卡（阻断继承链）
    void setVirtualFunctionBlocked(bool blocked) { m_virtualBlocked = blocked; }
    bool isVirtualFunctionBlocked() const { return m_virtualBlocked; }

    // 重写升级 - 继承格升级同时增强基类效果
    void upgrade() override;

    // 重写租金计算 - 包含基类加成
    int calculateRent() const override;

    QColor getColor() const override;

    QString getDisplayText() const override;

private:
    int m_baseTileId;       // 基类格子ID
    bool m_virtualBlocked;  // 是否被虚函数卡阻断
    int m_inheritanceBonus; // 继承加成比例(%)
};
