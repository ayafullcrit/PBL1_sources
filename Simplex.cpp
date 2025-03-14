#include <iostream>
#include <iomanip>
#define FOR(i, a, b) for (int i = a; i <= b; i++)
using namespace std;
int main()
{
    int NumOfConstrants, NumOfVariables;
    cin >> NumOfConstrants >> NumOfVariables;
    int N = NumOfVariables, M = NumOfConstrants;
    int Row = M, Col = N + M;
    float f[101][101] = {0};
    //  cout << "Nhap cac he so cua Z:";
    FOR(i, 1, N)
    {
        cin >> f[M + 1][i];
        f[M + 1][i] *= -1;
    }
    //   cout << "Nhap cac he so cua cac rang buoc:" << endl;
    FOR(i, 1, M)
    {
        FOR(j, 1, N)
        {
            cin >> f[i][j];
        }
        cin >> f[i][N + M + 1];
        f[i][N + i] = 1;
    }
    while (1)
    {
        int MostNegative = 0, NegCol = -1;
        FOR(i, 1, N)
        {
            if (f[M + 1][i] < 0 && f[M + 1][i] < MostNegative)
            {
                MostNegative = f[M + 1][i];
                NegCol = i;
            }
        }
        cout<<endl;
        FOR(i, 1, M + 1)
        {
            FOR(j, 1, M + N + 1)
            {
                cout << setw(5) << f[i][j] << " ";
            }
            cout << endl;
        }
        if (NegCol == -1)
            break;
        float Ratio = INT_MAX;
        int NegRow = -1;
        FOR(i, 1, M)
        {
            if(f[i][NegCol] <= 0) continue;
            float RatioI = (f[i][N + M + 1]) / (f[i][NegCol]);
            if (RatioI < Ratio)
            {
                Ratio = RatioI;
                NegRow = i;
            }
        }
        FOR(i, 1, N + M + 1)
        {
            if (i != NegCol)
                f[NegRow][i] /= f[NegRow][NegCol];
        }
        f[NegRow][NegCol] = 1;
        FOR(i, 1, M + 1)
        {
            if (i != NegRow)
            {
                float Factor = f[i][NegCol];
                FOR(j, 1, M + N + 1)
                {
                    f[i][j] -= Factor * f[NegRow][j];
                }
            }
        }
    }
    cout << f[M + 1][M + N + 1];
}
/*
2 2
8 6
4 2 60
2 4 48

3 2
3 2
-1 1 4
1 2 14
5 2 30
*/