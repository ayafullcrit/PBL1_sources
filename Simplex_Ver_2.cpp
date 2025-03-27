#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <string.h>
#define FOR(i, a, b) for (int i = a; i <= b; i++)
using namespace std;

int main()
{
    int NumOfConstrants, NumOfVariables;
    cout << "Nhap so rang buoc: ";
    cin >> NumOfConstrants;
    cout << "Nhap so bien cua da thuc: ";
    cin >> NumOfVariables;
    int N = NumOfVariables, M = NumOfConstrants;
    float f[101][101] = {0};
    //  Nhap cac he so cua da thuc Z
    cout << "Nhap he so cua da thuc Z: ";
    FOR(i, 1, N)
    {
        cin >> f[M + 1][i];
        f[M + 1][i] *= -1; // gan cac he so cua da thuc Z cho hang cuoi cua bang
    }
    //   Nhap cac rang buoc
    vector<int> ValueOfConstrants(M + 1); // mang chua ve phai cua bat phuong trinh
    fflush(stdin);                        // xoa bo nho dem (dung khi nhap string)
    FOR(i, 1, M)
    {
        string TempStr, InputLine;
        getline(cin, InputLine); // nhap dong input duoi dang mot string
        stringstream InputStream(InputLine);
        int sign = 1;
        while (InputStream >> TempStr) // tach tung tu cua dong va nhap vao xau TempStr
        {
            if (TempStr != "+" and TempStr != "-")
                if (TempStr[0] == '>' or TempStr[0] == '<')
                {
                    if (TempStr[0] == '>')
                        sign = -1; // nhan -1 cho hai ve de chuyen ve bat phuong trinh <=
                }
                else
                {
                    int xpos = TempStr.find('x'); // tim vi tri cua ki tu 'x'
                    if (xpos > TempStr.length())  // neu khong ton tai 'x' trong xau ki tu (ve phai chua gia tri)
                    {
                        ValueOfConstrants[i] = stoi(TempStr); // gan gia tri cho mang ValueOfConstrants
                    }
                    else // ton tai ki tu 'x'
                    {
                        string Factor;                        // he so cua x
                        if (xpos != 0)                        // neu x khong o dau xau ki tu
                            Factor = TempStr.substr(0, xpos); // he so cua x la xau con tu 0 den xpos-1
                        else
                            Factor = "1"; // neu x o dau xau ki tu thi he so la 1
                        // ham stoi dung de chuyen tu string sang int
                        string VariableIndex = TempStr.substr(xpos + 1, TempStr.length());
                        // chi so cua bien x la xau con tu xpos+1 den cuoi xau ki tu
                        f[i][stoi(VariableIndex)] = stoi(Factor);
                        // gan he so cho bien x trong bang simplex
                    }
                }
        }
        f[i][N + i] = 1; // gan he so cho bien phu trong bang simplex
        if (sign == -1) // bien doi tu bpt '>=' thanh bpt '<='
        {
            ValueOfConstrants[i] *= sign;
            FOR(j, 1, N)
            f[i][j] *= sign;
        }
    }

    string VariableCol[100], VariableRow[100]; // hai mang dung de luu ten cac bien trong bang
    // gan ten bien cho cac hang va cac cot nhu trong bang
    FOR(i, 1, N)
    {
        VariableCol[i] = "x" + to_string(i);
    }
    FOR(i, N + 1, N + M)
    {
        VariableCol[i] = "s" + to_string(i - N);
    }
    FOR(i, 1, M)
    {
        VariableRow[i] = "s" + to_string(i);
        f[i][N + M + 1] = ValueOfConstrants[i];
    }
    // thuc hien thuat toan simplex
    while (1)
    {
        // tim cot co he so am nho nhat
        int MostNegative = 0, NegCol = -1;
        FOR(i, 1, N)
        {
            if (f[M + 1][i] < 0 && f[M + 1][i] < MostNegative)
            {
                MostNegative = f[M + 1][i];
                NegCol = i;
            }
        }
        cout << endl;
        // in bang
        FOR(i, 0, N + M + 1)
        {
            if (i == 0)
            {
                cout << setw(10) << " ";
                continue;
            }
            else if (i == N + M + 1)
            {
                cout << setw(10) << "Value";
                continue;
            }
            cout << setw(10) << VariableCol[i];
        }
        cout << endl;
        FOR(i, 1, M + 1)
        {
            FOR(k, 0, (N + M + 2) * 10)
                cout << '-';
            cout << endl;
            if (i == M + 1)
            {
                cout << setw(10) << "Z";
            }
            else
                cout << setw(10) << VariableRow[i];
            FOR(j, 1, N + M + 1)
            {
                cout << setw(10) << f[i][j];
            }
            cout << endl;
        }
        // neu khong tim thay (bang toi uu) thi dung thuat toan
        if (NegCol <= -1)
            break;

        // tim hang co ti so nho nhat trong cot
        float Ratio = INT_MAX;
        int NegRow = -1;
        FOR(i, 1, M)
        {
            if (f[i][NegCol] <= 0)
                continue;
            float RatioI = (f[i][N + M + 1]) / (f[i][NegCol]);
            if (RatioI < Ratio)
            {
                Ratio = RatioI;
                NegRow = i;
            }
        }
        // chia cac phan tu trong hang cho phan tu tai diem xoay
        FOR(i, 1, N + M + 1)
        {
            if (i != NegCol)
                f[NegRow][i] /= f[NegRow][NegCol];
        }
        // bien doi tuong tu phep khu Gauss
        f[NegRow][NegCol] = 1;
        FOR(i, 1, M + 1)
        {
            if (i != NegRow)
            {
                float Factor = f[i][NegCol];
                FOR(j, 1, N + M + 1)
                {
                    f[i][j] -= Factor * f[NegRow][j];
                }
            }
        }
        VariableRow[NegRow] = VariableCol[NegCol];
    }
    // tim cac gia tri cua x theo cac he so tren bang da bien doi
    float x[100];
    FOR(i, 1, M)
    {
        if (VariableRow[i][0] == 'x')
        {
            int Index = stoi(VariableRow[i].substr(1, VariableRow[i].length() - 1));
            x[Index] = f[i][N + M + 1];
        }
    }
    // in ra ket qua
    cout << endl
         << "Gia tri toi da cua Z la:" << f[M + 1][N + M + 1];
    cout << endl;
    cout << "Gia tri cac bien la:" << endl;
    FOR(i, 1, N)
    {
        cout << "x" << i << " = " << x[i];
        if (i < N)
            cout << " , ";
    }
}

/*
2
2
3 5
x2 <= 4
x1 <= 3

2
2
3 5
x1 + 2x2 <= 6
3x1 + 2x2 <= 12

2 2
40 30
2x1 + x2 <= 16
x1 + x2 <= 9

3
2
3 5
2x1 + 4x2 <= 40
3x1 + 2x2 <= 36
6x1 + 5x2 <= 60
*/
