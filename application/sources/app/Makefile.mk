include sources/app/screens/Makefile.mk

VPATH += sources/app/submarine_game
CFLAGS += -I./sources/app
CPPFLAGS += -I./sources/app
CPPFLAGS += -I./sources/app/submarine_game

VPATH += sources/app

# CPP source files
SOURCES_CPP += sources/app/task_system.cpp
SOURCES_CPP += sources/app/app.cpp
SOURCES_CPP += sources/app/app_data.cpp
SOURCES_CPP += sources/app/app_non_clear_ram.cpp
SOURCES_CPP += sources/app/app_bsp.cpp
SOURCES_CPP += sources/app/app_modbus_pull.cpp
SOURCES_CPP += sources/app/shell.cpp

SOURCES_CPP += sources/app/task_shell.cpp
SOURCES_CPP += sources/app/task_life.cpp
SOURCES_CPP += sources/app/task_fw.cpp
SOURCES_CPP += sources/app/task_list.cpp
SOURCES_CPP += sources/app/task_display.cpp

ifeq ($(TASK_ZIGBEE_OPTION),-DTASK_ZIGBEE_EN)
SOURCES_CPP += sources/app/task_zigbee.cpp
endif

SOURCES_CPP += sources/app/task_if.cpp
SOURCES_CPP += sources/app/task_rf24_if.cpp
SOURCES_CPP += sources/app/task_uart_if.cpp
SOURCES_CPP += sources/app/task_dbg.cpp
# Submarine Dodge Game
SOURCES_CPP += sources/app/submarine_game/sub_game_submarine.cpp
SOURCES_CPP += sources/app/submarine_game/sub_game_torpedo.cpp
SOURCES_CPP += sources/app/submarine_game/sub_game_obstacle.cpp
SOURCES_CPP += sources/app/submarine_game/sub_game_bang.cpp
SOURCES_CPP += sources/app/screens/scr_submarine_game.cpp
SOURCES_CPP += sources/app/submarine_game/sub_game_boss.cpp