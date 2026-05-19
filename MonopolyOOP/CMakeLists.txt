# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.5)
project(Monopoly)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(
    src/core
    src/items
    src/players
    src/questions
    src/tiles
    src/ui
)

set (SRCS
    resources/resources.qrc
    src/core/Dice.cpp
    src/core/Dice.h
    src/core/Game.cpp
    src/core/Game.h
    src/core/GameBoard.cpp
    src/core/GameBoard.h
    src/items/Item.cpp
    src/items/Item.h
    src/items/ItemManager.cpp
    src/items/ItemManager.h
    src/players/AIPlayer.cpp
    src/players/AIPlayer.h
    src/players/HumanPlayer.cpp
    src/players/HumanPlayer.h
    src/players/Player.cpp
    src/players/Player.h
    src/questions/Question.h
    src/questions/QuestionBank.cpp
    src/questions/QuestionBank.h
    src/tiles/ChanceTile.cpp
    src/tiles/ChanceTile.h
    src/tiles/ClassTemplateTile.cpp
    src/tiles/ClassTemplateTile.h
    src/tiles/DestinyTile.cpp
    src/tiles/DestinyTile.h
    src/tiles/FunctionTemplateTile.cpp
    src/tiles/FunctionTemplateTile.h
    src/tiles/InheritanceTile.cpp
    src/tiles/InheritanceTile.h
    src/tiles/LabClassTile.cpp
    src/tiles/LabClassTile.h
    src/tiles/PenaltyTile.cpp
    src/tiles/PenaltyTile.h
    src/tiles/PropertyTile.cpp
    src/tiles/PropertyTile.h
    src/tiles/ShopTile.cpp
    src/tiles/ShopTile.h
    src/tiles/Tile.cpp
    src/tiles/Tile.h
    src/ui/DiceWidget.cpp
    src/ui/DiceWidget.h
    src/ui/GameBoardWidget.cpp
    src/ui/GameBoardWidget.h
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/ui/PlayerInfoWidget.cpp
    src/ui/PlayerInfoWidget.h
    src/ui/QuestionDialog.cpp
    src/ui/QuestionDialog.h
    src/main.cpp
    CMakeLists.txt
    README.md
)


add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
