#include "questions/QuestionBank.h"
#include <QRandomGenerator>

QuestionBank::QuestionBank(QObject *parent) : QObject(parent) {
    initQuestions();
}

void QuestionBank::initQuestions() {
    // ===== 类与对象 =====
    addQuestion(
        "在C++中，类和结构体的主要区别是什么？",
        {"类默认成员是public，结构体是private",
         "类默认成员是private，结构体是public",
         "类不能继承，结构体可以",
         "没有区别"},
        1,
        "class默认访问权限是private，struct默认是public。这是C++保留struct为了兼容C。",
        "类与对象", 1);

    addQuestion(
        "以下哪个不是类的访问修饰符？",
        {"public", "private", "protected", "internal"},
        3,
        "C++中只有public、private、protected三种访问修饰符。internal是C#的概念。",
        "类与对象", 1);

    addQuestion(
        "构造函数的作用是什么？",
        {"释放对象占用的内存",
         "初始化对象的成员变量",
         "返回对象的引用",
         "定义类的接口"},
        1,
        "构造函数用于在创建对象时初始化成员变量，没有返回值。",
        "类与对象", 1);

    addQuestion(
        "析构函数在什么时候被调用？",
        {"创建对象时",
         "对象生命周期结束时",
         "每次调用成员函数时",
         "程序开始时"},
        1,
        "析构函数在对象被销毁时自动调用，用于清理资源。栈对象离开作用域时销毁，堆对象delete时销毁。",
        "类与对象", 1);

    addQuestion(
        "拷贝构造函数的形式是什么？",
        {"ClassName(const ClassName& other)",
         "ClassName(ClassName other)",
         "ClassName(ClassName* other)",
         "void copy(ClassName& other)"},
        0,
        "拷贝构造函数接受同类对象的const引用参数，用于深拷贝。传值会引发无限递归。",
        "类与对象", 2);

    addQuestion(
        "深拷贝和浅拷贝的区别是什么？",
        {"没有区别",
         "深拷贝复制指针，浅拷贝复制指针指向的内容",
         "深拷贝复制指针指向的内容，浅拷贝只复制指针",
         "深拷贝更慢"},
        2,
        "浅拷贝只复制指针值，两个对象共享同一块内存。深拷贝分配新内存并复制内容。",
        "类与对象", 2);

    // ===== 继承与派生 =====
    addQuestion(
        "C++中的继承方式有几种？",
        {"1种", "2种", "3种", "4种"},
        2,
        "C++有public、protected、private三种继承方式。",
        "继承与派生", 1);

    addQuestion(
        "菱形继承（Diamond Inheritance）会导致什么问题？",
        {"编译错误",
         "二义性：派生类中有两份基类成员",
         "运行速度变慢",
         "内存泄漏"},
        1,
        "菱形继承中，最底层的派生类包含两份间接基类的成员，导致二义性。可用虚继承解决。",
        "继承与派生", 2);

    addQuestion(
        "虚继承（virtual inheritance）的作用是？",
        {"提高程序运行速度",
         "解决菱形继承的二义性问题",
         "允许虚函数",
         "禁止继承"},
        1,
        "虚继承确保公共基类在继承链中只有一份实例，解决菱形继承问题。",
        "继承与派生", 2);

    addQuestion(
        "protected继承后，基类的public成员在派生类中变成？",
        {"public", "protected", "private", "不可访问"},
        1,
        "protected继承时，基类public和protected成员在派生类中都变为protected。",
        "继承与派生", 2);

    // ===== 多态 =====
    addQuestion(
        "实现运行时多态（动态绑定）的关键是？",
        {"函数重载",
         "运算符重载",
         "虚函数",
         "模板"},
        2,
        "虚函数（virtual function）允许通过基类指针调用派生类的重写方法，实现运行时多态。",
        "多态", 1);

    addQuestion(
        "纯虚函数的声明形式是？",
        {"virtual void func() = 0;",
         "virtual void func() {}",
         "void func() = 0;",
         "abstract void func();"},
        0,
        "纯虚函数用 = 0 标记，包含纯虚函数的类是抽象类，不能实例化。",
        "多态", 1);

    addQuestion(
        "抽象类的特点是什么？",
        {"不能创建对象",
         "必须有虚函数",
         "不能被继承",
         "只能有纯虚函数"},
        0,
        "包含纯虚函数的类是抽象类，不能实例化，但可以被继承。派生类必须实现所有纯虚函数才能实例化。",
        "多态", 1);

    addQuestion(
        "虚函数表（vtable）存储在哪里？",
        {"栈", "堆", "数据段", "代码段"},
        3,
        "虚函数表通常存储在代码段（只读数据区），每个类一份。每个对象存储指向vtable的指针（vptr）。",
        "多态", 3);

    addQuestion(
        "以下哪种方式不能实现多态？",
        {"虚函数",
         "函数重载",
         "函数模板",
         "CRTP"},
        1,
        "函数重载是静态多态（编译时确定），虚函数是动态多态（运行时确定）。模板和CRTP也是静态多态。",
        "多态", 2);

    // ===== 运算符重载 =====
    addQuestion(
        "以下哪个运算符不能被重载？",
        {"+", "==", "::", "<<"},
        2,
        "::（作用域解析）、.（成员访问）、.*（成员指针访问）、?:（三目）和sizeof不能被重载。",
        "运算符重载", 2);

    addQuestion(
        "重载++运算符时，如何区分前置和后置？",
        {"前置参数为int",
         "后置参数为int",
         "前置返回引用",
         "无法区分"},
        1,
        "后置++带一个int哑参数：operator++(int)。前置++没有参数并返回引用。",
        "运算符重载", 2);

    // ===== 模板 =====
    addQuestion(
        "类模板和函数模板的主要区别？",
        {"类模板必须有类型参数",
         "函数模板可以自动推导类型，类模板必须显式指定",
         "类模板不能被继承",
         "没有区别"},
        1,
        "函数模板可以自动推导模板参数（如max(1,2)推导出int），类模板必须显式指定（如vector<int>）。",
        "模板", 1);

    addQuestion(
        "模板特化（Specialization）的目的是？",
        {"提高代码运行效率",
         "为特定类型提供定制化实现",
         "减少代码量",
         "避免链接错误"},
        1,
        "模板特化允许为特定类型（如vector<bool>）提供专门的实现，优化性能或改变行为。",
        "模板", 2);

    addQuestion(
        "以下哪个STL容器是顺序容器？",
        {"set", "map", "vector", "unordered_map"},
        2,
        "vector是顺序容器，元素按插入顺序连续存储。set/map是关联容器，unordered_map是哈希容器。",
        "模板", 1);

    addQuestion(
        "模板元编程（TMP）发生在什么阶段？",
        {"运行时", "编译时", "链接时", "预处理时"},
        1,
        "模板元编程在编译期执行计算，生成代码。可用于编译期断言、策略选择等。",
        "模板", 3);

    // ===== 高级特性 =====
    addQuestion(
        "RAII代表什么？",
        {"Resource Acquisition Is Initialization",
         "Random Access Input Iterator",
         "Runtime Automatic Instance Initialization",
         "Resource Allocation In Instance"},
        0,
        "RAII（资源获取即初始化）是C++核心思想：构造函数获取资源，析构函数释放资源。",
        "高级特性", 2);

    addQuestion(
        "智能指针shared_ptr使用什么机制管理内存？",
        {"垃圾回收", "引用计数", "标记清除", "手动释放"},
        1,
        "shared_ptr使用引用计数，当计数为0时自动释放内存。weak_ptr不增加引用计数。",
        "高级特性", 1);

    addQuestion(
        "以下哪个不是C++11引入的特性？",
        {"auto关键字", "lambda表达式", "概念（Concepts）", "范围for循环"},
        2,
        "Concepts是C++20特性。auto、lambda、范围for都是C++11引入的。",
        "高级特性", 2);

    addQuestion(
        "移动语义（Move Semantics）的主要目的是？",
        {"深拷贝对象",
         "转移资源所有权，避免不必要的拷贝",
         "自动释放内存",
         "实现多态"},
        1,
        "移动语义通过右值引用转移资源所有权，避免大对象的深拷贝开销。",
        "高级特性", 2);

    addQuestion(
        "final关键字的作用是？",
        {"声明常量",
         "禁止类被继承或虚函数被重写",
         "优化循环",
         "标记纯虚函数"},
        1,
        "final修饰类表示不能被继承，修饰虚函数表示不能被派生类重写。override表示重写父类虚函数。",
        "高级特性", 2);

    addQuestion(
        "以下哪个是设计模式中的'单例模式'的目的？",
        {"创建多个对象",
         "确保一个类只有一个实例",
         "简化类接口",
         "提高运行速度"},
        1,
        "单例模式确保全局只有一个实例，并提供访问点。常用于配置管理、数据库连接池等。",
        "高级特性", 1);
}

void QuestionBank::addQuestion(const QString& text,
                               const QVector<QString>& options,
                               int correct, const QString& explanation,
                               const QString& category, int difficulty) {
    m_questions.append(Question(text, options, correct, explanation, category, difficulty));
}

Question QuestionBank::getRandomQuestion() {
    if (m_questions.isEmpty()) {
        return Question();
    }
    int index = QRandomGenerator::global()->bounded(m_questions.size());
    return m_questions[index];
}

Question QuestionBank::getRandomQuestionByCategory(const QString& category) {
    QVector<Question> filtered;
    for (const auto& q : m_questions) {
        if (q.category == category) {
            filtered.append(q);
        }
    }
    if (filtered.isEmpty()) {
        return getRandomQuestion();
    }
    int index = QRandomGenerator::global()->bounded(filtered.size());
    return filtered[index];
}

Question QuestionBank::getRandomQuestionByDifficulty(int difficulty) {
    QVector<Question> filtered;
    for (const auto& q : m_questions) {
        if (q.difficulty == difficulty) {
            filtered.append(q);
        }
    }
    if (filtered.isEmpty()) {
        return getRandomQuestion();
    }
    int index = QRandomGenerator::global()->bounded(filtered.size());
    return filtered[index];
}

QVector<QString> QuestionBank::getCategories() const {
    QVector<QString> categories;
    for (const auto& q : m_questions) {
        if (!categories.contains(q.category)) {
            categories.append(q.category);
        }
    }
    return categories;
}
