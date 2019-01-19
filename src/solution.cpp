/*
 * This is the core algorithm
 */
# include "solution.h"
# include <iostream>
# include <cstring>
# include <cstdio>

using namespace std;

// go数组表示两维坐标（x,y)的5个待判断方向（中，东，西，南，北）
const int go[5][2] = { {0,0},{0,1},{0,-1},{1,0},{-1,0} };

Solution::Solution(int r, int c) : n(r), m(c) {
    int i, j;
    tmp = 0;
    lim1 = lim2 = 0;
    ans = 0;
    for(i = 0; i < 22; i++)
        for(j = 0; j < 22; j++) {
            anx[i][j] = 0;
            put [i][j] = 0;
            spy[i][j] = 0;
        }
}

Solution::~Solution() { }

void Solution::SetMap(int **m_Map)
{
    ans = n * m / 3 + 2; // ans为初始最优值
    lim1 = n * m, lim2 = n * m + m / 4 + 5; // 设置lim1，lim2两个限制条件（剪枝）

    for (int i = 0; i <= n + 1; ++i)
        spy[i][0] = spy[i][m + 1] = 1;
    for (int i = 0; i <= m + 1; ++i)
        spy[0][i] = spy[n + 1][i] = 1;
    search(1, 1); // 从（1，1）开始

    printf("%d\n", ans); // 输出最优解需要的机器人数
    // 打印机器人放置放置矩阵
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            m_Map[i-1][j-1] = anx[i][j];
            cout << anx[i][j] << ' ';
        }
        cout << endl;
    }
}

void Solution::puta(int x, int y, int i, int j) {
    put[x][y] = 1; // 在此处放置机器人
    tmp++;
    // 上、下、左、右设为被监视
    for (int i = 0; i < 5; ++i)
        if (++spy[x + go[i][0]][y + go[i][1]] == 1) spys++;

    search(i, j + 1); // 继续搜索

    put[x][y] = 0; // 撤回放置的机器人，返回上一状态（回溯）
    tmp--;
    for (int i = 0; i < 5; ++i)
        if (--spy[x + go[i][0]][y + go[i][1]] == 0) spys--;
}

void Solution::search(int i, int j)
{
    if (tmp >= ans) return; // 当放置机器人的个数超过当前最优值时，剪去
    while (i <= n && spy[i][j]) // 已放置的不再被搜索
    {
        ++j;
        if (j > m)	++i, j = 1; // 换行
    }
    if (i > n) // 扫描结束更新最优解，拷贝放置矩阵
    {
        ans = tmp;
        memcpy(anx, put, sizeof(put));
        return;
    }

    int reach = spys + (ans - tmp) * 5;
    if (reach <= lim1) return; // 考虑当前可放入机器人与监视情况的关系（剪枝策略6）
    if (i < n - 1 && reach <= lim2) return; // 剪枝策略7

    // 在（i，j+1）没有被监视的情况下才考虑放在（i，j）
    if ((i == n && j == m) || spy[i][j + 1] == 0) puta(i, j, i, j);
    // 在i < n的情况下才考虑放在（i+1，j）
    if (i < n) puta(i + 1, j, i, j);
    // 在（i，j+1）或（i，j+2）没有被监视的情况下才考虑放在（i，j+1）
    if (j < m && (spy[i][j + 1] == 0 || spy[i][j + 2] == 0)) puta(i, j + 1, i, j);
}
