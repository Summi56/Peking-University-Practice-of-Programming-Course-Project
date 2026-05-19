#include <QApplication>
#include "ui/MainWindow.h"

/**
 * @brief C++ OOP 大富翁 - 主入口
 *
 * 一个融合C++面向对象编程知识的教育大富翁游戏。
 * 通过游戏化的方式学习OOP核心概念：
 * - 类与对象
 * - 继承与派生
 * - 多态与虚函数
 * - 运算符重载
 * - 模板编程
 */
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // 设置应用信息
    app.setApplicationName("C++ OOP 大富翁");
    app.setOrganizationName("OOP Learning");

    // 创建并显示主窗口
    MainWindow window;
    window.show();

    return app.exec();
}
