#ifndef SOLUTION_H
#define SOLUTION_H

class Solution {
private:
    int n, m;
    int anx[22][22], ans;
    int put[22][22], spy[22][22], tmp, spys = 0;
    int lim1, lim2;
    void puta(int x, int y, int i, int j);
    void search(int i, int j);
public:
    Solution(int r, int c);
    ~Solution();
    void SetMap(int **m_Map);
};

#endif // SOLUTION_H
