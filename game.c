#include "game.h"

static int *map;
static size_t size;
static size_t len;

static int score = 0;

char topStr[512] = {0};
char midStr[512] = {0};
char bottomStr[512] = {0};

static void initStr()
{
    strcpy(topStr, "┏━━━━━");
    for (size_t i = 0; i < size - 1; i++)
    {
        strcat(topStr, "┳━━━━━");
    }
    strcat(topStr, "┓");

    strcpy(midStr, "┣━━━━━");
    for (size_t i = 0; i < size - 1; i++)
    {
        strcat(midStr, "╋━━━━━");
    }
    strcat(midStr, "┫");

    strcpy(bottomStr, "┗━━━━━");
    for (size_t i = 0; i < size - 1; i++)
    {
        strcat(bottomStr, "┻━━━━━");
    }
    strcat(bottomStr, "┛");
}

void init(size_t m_size) //初始化游戏
{
    srand((unsigned)time(NULL));
    system("clear");
    system("stty -icanon -echo"); // 开启无缓冲 开启无回显
    printf("\033[?25l");          // 隐藏光标
    printf("\033[s");             // 保存光标位置

    size = m_size;
    len = size * size;
    map = (int *)calloc(sizeof(int), len);
    initStr();

    insert_one();
    show();
}

void show() // 显示游戏界面
{
    printf("\033[u"); // 恢复光标位置
    printf("\033[K"); // 清除从光标到行尾的内容
    printf("\033[s"); // 保存光标位置
    printf("\n\t\x1b[1;33;49m2048小游戏\x1b[0m\n");
    printf("%s\n", topStr);
    size_t i, j;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            size_t num = (size_t) * (map + i * size + j);
            if (num > 0)
                printf("┃%4zu ", num);
            else
                printf("┃     ");
        }
        printf("┃\n");
        if (i == size - 1)
            printf("%s\n", bottomStr);
        else
            printf("%s\n", midStr);
    }
    printf("\n分数: \x1b[1;33;49m%d\x1b[0m\n", score);
}

int insert_one() // 插入一个新的点
{
    // 1.获取为0的点的个数;
    size_t count = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (map[i] == 0)
            ++count;
    }
    if (count == 0)
        return 1;

    // 2.随机数n
    size_t num = rand() % count;

    // 3.找到第n个数,设置为数字2
    count = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (map[i] == 0)
            if (count++ == num)
                map[i] = 2;
    }
    return 0;
}

void move_group(int *arr[]) // 移动一组
{
    int cur = 0;
    int *pHead = arr[cur];
    for (size_t i = 1; i < size; i++)
    {
        int *pNum = arr[i];
        if (*pNum == 0)
            continue;
        if (*pHead == 0)
        {
            *pHead = *pNum;
            *pNum = 0;
            continue;
        }

        if (*pHead == *pNum)
        {
            *pHead = *pHead + *pNum;
            score += *pHead;
            *pNum = 0;
            pHead = arr[++cur];
        }
        else
        {
            pHead = arr[++cur];
            if (pHead != pNum)
            {
                *pHead = *pNum;
                *pNum = 0;
            }
        }
    }
}

void play() // 游戏操作
{
    while (1)
    {
        int ch = getchar();
        switch (ch)
        {
        case 65: // 向上移动
        case 119:
            for (int i = 0; i < size; i++)
            {
                int *pArr[size];
                int index = 0;
                for (int j = 0; j < size; j++)
                {
                    pArr[index++] = (int *)map + j * size + i;
                }
                move_group(pArr);
            }
            break;
        case 97: // 向左移动
        case 68:
            for (int i = 0; i < size; i++)
            {
                int *pArr[size];
                int index = 0;
                for (int j = 0; j < size; j++)
                    pArr[index++] = (int *)map + i * size + j;
                move_group(pArr);
            }
            break;
        case 100: // 向右移动
        case 67:
            for (int i = size - 1; i >= 0; i--)
            {
                int *pArr[size];
                int index = 0;
                for (int j = size - 1; j >= 0; j--)
                    pArr[index++] = (int *)map + i * size + j;
                move_group(pArr);
            }
            break;
        case 115:
        case 66: // 向下移动
            for (int i = 0; i < size; i++)
            {
                int *pArr[size];
                int index = 0;
                for (int j = size - 1; j >= 0; j--)
                    pArr[index++] = (int *)map + j * size + i;
                move_group(pArr);
            }
            break;
        case 113:
            game_over();
            break;
        default:
            continue;
        }
        int ret = insert_one();
        if (ret == 1)
        {
            printf("\n 游戏结束!\n");
            game_over();
            return;
        }
        show();
    }
}

void game_over() // 游戏结束
{
    if (map != NULL)
    {
        free(map);
        map = NULL;
    }
    printf("\033[?25h");
    system("stty icanon echo");
    exit(0);
}
