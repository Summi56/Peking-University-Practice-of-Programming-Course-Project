# C++ OOP 大富翁 - 面向对象编程学习游戏

一个将C++面向对象编程知识融入经典大富翁玩法的教育游戏，使用Qt框架开发。

## 项目简介

本项目是一款"特殊的经典游戏大富翁"，在保留大富翁核心玩法的基础上，融入C++面向对象程序设计内容。玩家在游戏过程中通过买地、建房、事件触发等行为学习并巩固C++ OOP语法与设计思想。

## 核心特性

### 游戏机制
- **经典大富翁玩法**：掷骰子、买地、收租、让对手破产
- **回合制系统**：支持玩家vs电脑、双人对战、练习模式
- **多种难度**：简单/普通/困难，影响初始资金分配

### OOP知识融入
| 格子类型 | OOP概念 | 游戏效果 |
|---------|---------|---------|
| 普通地产格 | 类与对象 | 购买=定义类，升级=添加成员函数，收租=调用函数 |
| 继承与派生格 | 继承 | 进入时同时触发基类效果，可用虚函数卡阻断 |
| 类模板格 | 类模板 | 可选择不同类型实例化，不同成本和收益 |
| 函数模板格 | 函数模板 | 类似赌场，类型不同胜率不同 |
| 机会格 | 知识问答 | 回答OOP问题，答对获得道具 |
| 命运格 | 编程概念 | 随机触发与C++编程相关的奖励/惩罚 |
| 上机课格 | 实验课 | 暂停一回合，可用假条免除 |
| 商店格 | - | 购买和出售道具 |

### 道具系统
- **假条**：免除一次上机课
- **虚函数卡**：阻断继承格子的基类调用（体现虚函数机制）
- **手气卡**：重新掷骰子
- **delete卡**：删除对手的一个地产

### 答题系统
内置30+道C++ OOP知识题目，覆盖：
- 类与对象（构造函数、析构函数、拷贝控制）
- 继承与派生（继承方式、菱形继承、虚继承）
- 多态（虚函数、纯虚函数、抽象类）
- 运算符重载
- 模板编程（类模板、函数模板、特化）
- 高级特性（RAII、智能指针、移动语义）

## 技术架构

### 项目结构
```
MonopolyOOP/
├── CMakeLists.txt              # CMake构建配置
├── README.md                   # 项目说明
├── src/
│   ├── main.cpp                # 程序入口
│   ├── core/                   # 核心游戏逻辑
│   │   ├── Game.h/.cpp         # 游戏主控
│   │   ├── GameBoard.h/.cpp    # 地图管理
│   │   └── Dice.h/.cpp         # 骰子系统
│   ├── tiles/                  # 格子系统（继承体系）
│   │   ├── Tile.h/.cpp         # 抽象基类
│   │   ├── PropertyTile.h/.cpp # 地产格
│   │   ├── InheritanceTile.h/.cpp    # 继承格
│   │   ├── ClassTemplateTile.h/.cpp  # 类模板格
│   │   ├── FunctionTemplateTile.h/.cpp # 函数模板格
│   │   ├── ChanceTile.h/.cpp   # 机会格
│   │   ├── DestinyTile.h/.cpp  # 命运格
│   │   ├── LabClassTile.h/.cpp # 上机课格
│   │   ├── ShopTile.h/.cpp     # 商店格
│   │   └── PenaltyTile.h/.cpp  # 扣款格
│   ├── players/                # 玩家系统（继承体系）
│   │   ├── Player.h/.cpp       # 抽象基类
│   │   ├── HumanPlayer.h/.cpp  # 人类玩家
│   │   └── AIPlayer.h/.cpp     # AI玩家
│   ├── items/                  # 道具系统
│   │   ├── Item.h/.cpp         # 道具基类与具体道具
│   │   └── ItemManager.h/.cpp  # 道具管理器
│   ├── questions/              # 答题系统
│   │   ├── Question.h/.cpp     # 题目结构
│   │   └── QuestionBank.h/.cpp # 题库管理
│   └── ui/                     # Qt用户界面
│       ├── MainWindow.h/.cpp   # 主窗口
│       ├── GameBoardWidget.h/.cpp  # 地图绘制
│       ├── PlayerInfoWidget.h/.cpp # 玩家信息
│       ├── DiceWidget.h/.cpp   # 骰子显示
│       └── QuestionDialog.h/.cpp   # 答题对话框
└── resources/
    └── resources.qrc           # Qt资源文件
```

### 设计模式运用
1. **继承与多态**：Tile/Player/Item均采用继承体系，通过虚函数实现不同行为
2. **工厂模式**：ItemManager使用工厂函数创建道具
3. **封装**：所有类的属性私有化，通过接口访问
4. **信号与槽**：Qt信号槽机制实现UI与游戏逻辑的解耦

## 编译与运行

### 环境要求
- C++17兼容编译器（GCC 7+ / MSVC 2017+ / Clang 5+）
- Qt5或Qt6（Core、Widgets模块）
- CMake 3.16+

### 编译步骤
```bash
# 1. 进入项目目录
cd MonopolyOOP

# 2. 创建构建目录
mkdir build && cd build

# 3. 运行CMake
cmake ..

# 4. 编译
make -j$(nproc)

# 5. 运行
./MonopolyOOP
```

### Windows (Visual Studio)
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

## 扩展指南

### 添加新格子类型
1. 在 `src/tiles/` 下创建新的派生类，继承 `Tile` 或 `PropertyTile`
2. 重写 `onPlayerEnter()` 实现格子效果
3. 在 `GameBoard::initializeDefaultBoard()` 中添加新格子到地图
4. （可选）重写 `getColor()` 和 `getDisplayText()` 自定义UI显示

### 添加新道具
1. 在 `src/items/Item.h` 中定义新的道具类，继承 `Item`
2. 实现 `use()` 虚函数
3. 在 `ItemManager` 构造函数中注册工厂函数
4. 在 `getItemName/Description/Price()` 中添加道具信息

### 添加新题目
1. 在 `QuestionBank::initQuestions()` 中使用 `addQuestion()` 添加
2. 参数：题目文本、选项数组、正确答案索引、解释、分类、难度

### 修改地图布局
编辑 `GameBoard::initializeDefaultBoard()` 中的格子创建和添加顺序。

## 面向对象知识点对应表

| 游戏元素 | C++ OOP概念 | 说明 |
|---------|------------|------|
| Tile继承体系 | 类的继承与派生 | 多种格子类型继承自基类Tile |
| onPlayerEnter虚函数 | 多态/动态绑定 | 不同格子有不同进入效果 |
| 继承格触发基类效果 | 继承链调用 | 体现派生类调用基类函数 |
| 虚函数卡阻断继承 | 虚函数机制 | 运行时决定调用哪个函数 |
| 类模板格选择类型 | 类模板实例化 | vector<T>的不同实例化 |
| 函数模板格不同胜率 | 模板特化 | 不同特化有不同行为 |
| 地产升级 | 类的扩展 | 添加成员函数/增强功能 |
| 答题系统 | 知识巩固 | 覆盖OOP核心概念 |

## 开源许可

本项目仅供学习交流使用。
