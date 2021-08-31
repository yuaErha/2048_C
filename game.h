#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void init(size_t size); //初始化游戏

void show(); // 显示游戏界面

int insert_one(); // 插入一个新的点

void move_group(int *arr[]); // 移动一组

void play(); // 游戏操作

void game_over(); // 游戏结束
