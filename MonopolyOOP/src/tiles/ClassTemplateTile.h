#pragma once
#include "tiles/Tile.h"

/**
 * @brief 类模板格 - 展示C++模板机制
 *
 * 对应文档中的"类模板"格：
 * - 这个格子可以以多种价格购买
 * - 购买后实例化为普通地产格子
 * - 类似于 vector<T> 可以实例化为 vector<int>, vector<double> 等
 */
class ClassTemplateTile : public Tile {
    Q_OBJECT
public:
    explicit ClassTemplateTile(int id, const QString& name,
                               const QString& description,
                               int basePrice, QObject* parent = nullptr);

    void onPlayerEnter(Player* player, Game* game) override;

    // 实例化选项
    struct InstantiationOption {
        QString typeName;    // 类型名称，如 "int", "double", "string"
        int cost;            // 实例化成本
        int rent;            // 收租金额
        QColor color;        // 显示颜色
    };

    QVector<InstantiationOption> getOptions() const { return m_options; }

    // 是否已经实例化
    bool isInstantiated() const { return m_instantiated; }

    // 获取当前实例化类型
    QString getInstanceType() const { return m_instanceType; }

    // 实例化（选择类型）
    void instantiate(Player* player, int optionIndex);

    // 获取当前租金
    int getCurrentRent() const { return m_currentRent; }

    // 获取当前所有者
    Player* getOwner() const { return m_owner; }

    void reset() override;
    QColor getColor() const override;
    QString getDisplayText() const override;

private:
    int m_basePrice;
    bool m_instantiated;
    QString m_instanceType;
    int m_currentRent;
    Player* m_owner;
    QVector<InstantiationOption> m_options;
};
