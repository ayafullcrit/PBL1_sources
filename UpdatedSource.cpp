#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <windows.h>
#include <conio.h>
#define FOR(i, a, b) for (int i = a; i <= b; i++)
#define MAX 100
#define Coord pair<int,int>
#define fi first
#define se second
using namespace std;
// cac bien va cau truc cho thao tac voi console

// cac ham thao tac voi console
void gotoXY(short x, short y)
{
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void SetWordColor(WORD wAttributes)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, wAttributes);
}
void Resize(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
// cac bien can thiet cho thuat toan simplex
int N, M, NumOfConstrants, NumOfVariables;
float f[MAX + 1][MAX + 1] = {0};
string VariableRow[MAX], VariableCol[MAX], x[MAX]; // hai mang dung de luu ten cac bien trong bang
int LeftMargin = 55;
int TopMargin = 2;
void PrintTitle()
{
    SetWordColor(0x03);
    char RightTop = 187 ,LeftTop = 201, LeftBottom = 200, RightBottom = 188;
    char Border = 205, UpBorder = 186;
    gotoXY(LeftMargin, TopMargin);
    cout << LeftTop;
    FOR(i, 1, 58)
    cout << Border;
    cout << RightTop;
    gotoXY(LeftMargin, TopMargin + 1);
    cout << UpBorder;
    FOR(i, 1, 10)
        cout << " ";
    cout << "        DO AN LAP TRINH TINH TOAN";
    FOR(i, 1, 15)
        cout << " ";
    cout << UpBorder;
    gotoXY(LeftMargin, TopMargin + 2);
    cout << UpBorder;
    FOR(i, 1, 10)
        cout << " ";
    cout << "De tai: Giai bai toan quy hoach tuyen tinh ";
    FOR(i, 1, 5)
        cout << " ";
    cout << UpBorder;
    gotoXY(LeftMargin, TopMargin + 3); cout << UpBorder;
    gotoXY(LeftMargin + 59, TopMargin + 3); cout << UpBorder;
    gotoXY(LeftMargin, TopMargin + 4);
    cout << UpBorder << "    Giao vien huong dan:                                  " << UpBorder;
    gotoXY(LeftMargin, TopMargin + 5);
    cout << UpBorder << "    1. Nguyen Tan Khoi                                    " << UpBorder;
    gotoXY(LeftMargin, TopMargin + 6); cout << UpBorder;
    gotoXY(LeftMargin + 59, TopMargin + 6); cout << UpBorder;
    TopMargin++;
    gotoXY(LeftMargin, TopMargin + 6);
    cout << UpBorder << "    Sinh vien thuc hien:                                  " << UpBorder;
    gotoXY(LeftMargin, TopMargin + 7);
    cout << UpBorder << "    1. Nguyen Dang Phong                                  " << UpBorder;
    gotoXY(LeftMargin, TopMargin + 8);
    cout << UpBorder << "    2. Ngo Van Dac Tri                                    " << UpBorder;
    gotoXY(LeftMargin, TopMargin + 9);
    cout << UpBorder << "    3. Tran Le Minh                                       " << UpBorder;
    gotoXY(LeftMargin, TopMargin + 10);
    cout << LeftBottom;
    FOR(i, 1, 58)
    cout << Border;
    cout << RightBottom;
    // in ra 8 dong
    TopMargin = 14;
}
void PrintMenu()
{
    SetWordColor(0x0E);
    gotoXY(LeftMargin, TopMargin);
    cout << "=========================== MENU ===========================";
    gotoXY(LeftMargin, TopMargin + 1);
    cout << "1. Bai toan 1: ";
    gotoXY(LeftMargin, TopMargin + 2);
    cout << "2. Bai toan 2: ";
    gotoXY(LeftMargin, TopMargin + 3);
    cout << "3. Bai toan 3: ";
    gotoXY(LeftMargin, TopMargin + 4);
    cout << "4. Ket thuc chuong trinh!";
}
void PrintMainContent()
{
	PrintTitle();
	PrintMenu();
}
void ResetConsole()
{
	system("cls");
	TopMargin = 2;
	PrintMainContent();
}
void PrintSimplexTable()
{
    cout << " ";
    FOR(k, 1, (N + M + 2) * 11 - 1)
    cout << "_";
    cout << endl;
    cout << "|" << setw(10);
    FOR(i, 0, N + M)
    {
        cout << VariableCol[i];
        cout << "|" << setw(10);
    }
    cout << setw(12) << "Value|\n";
    VariableRow[M + 1] = "Z";
    FOR(i, 1, M + 2)
    {
        cout << "|";
        int uCount = 0;
        FOR(k, 1, (N + M + 2) * 11)
        {
            if (uCount == 10)
            {
                cout << '|';
                uCount = 0;
                continue;
            }
            uCount++;
            cout << '-';
        }
        if (i == M + 2)
            break;
        cout << endl;
        cout << "|" << setw(10) << VariableRow[i] << "|" << setw(10);

        FOR(j, 1, N + M + 1)
        {
            cout << f[i][j] << "|";
            if (j <= N + M)
                cout << setw(10);
        }
        cout << endl;
    }
    cout << endl
         << endl;
}
void InitTheTable()
{
    string FileName;
    cout << "Nhap ten file input: ";
    cin >> FileName;
    ifstream file(FileName.c_str());
    // Simplex method
    int NumOfConstrants, NumOfVariables;
    file >> NumOfConstrants;
    file >> NumOfVariables;
    cout << "So rang buoc: " << NumOfConstrants << endl;
    cout << "So bien cua da thuc: " << NumOfVariables << endl;
    N = NumOfVariables, M = NumOfConstrants;
    //  Nhap cac he so cua da thuc Z
    cout << "Cac he so cua da thuc Z: ";
    FOR(i, 1, N)
    {
        file >> f[M + 1][i];
        cout << f[M + 1][i] << " ";
        f[M + 1][i] *= -1; // gan cac he so cua da thuc Z cho hang cuoi cua bang
    }
    cout << endl
         << "Cac rang buoc :" << endl;
    //   Nhap cac rang buoc
    vector<int> ValueOfConstrants(M + 1); // mang chua ve phai cua bat phuong trinh
    file.ignore();                        // xoa bo nho dem (dung khi nhap string)
    vector<string> OutVector;
    int SubVariableIndex = 0; // bien chua chi so cua bien phu
    FOR(i, 1, M)
    {
        string TempStr, InputLine, OutputLine;
        getline(file, InputLine);              // nhap dong input duoi dang mot string
        cout << i << ":" << InputLine << endl; // in ra dong input
        stringstream InputStream(InputLine);
        int sign = 1;
        while (InputStream >> TempStr) // tach tung tu cua dong va nhap vao xau TempStr
        {
            if (TempStr != "+")
                if (TempStr[0] == '>' or TempStr[0] == '<')
                {
                    if (TempStr[0] == '>')
                    {
                        sign = -1; // nhan -1 cho hai ve de chuyen ve bat phuong trinh <=
                    }
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
        if (sign == -1)  // bien doi tu bpt '>=' thanh bpt '<='
        {
            ValueOfConstrants[i] *= sign;
            FOR(j, 1, N)
            f[i][j] *= sign;
        }
        stringstream TempOutputStream(InputLine);
        string TempStr1;
        while (TempOutputStream >> TempStr1)
        {
            if (TempStr1[0] == '+')
            {
                OutputLine += TempStr1 + " ";
                continue;
            }
            if (TempStr1[0] == '>' or TempStr1[0] == '<')
                TempStr1.erase(0, 1); // xoa ki tu '>' hoac '<' o vi tri 0
            else if (sign == -1)
            {
                if (TempStr1[0] != '-')
                    TempStr1 = "-" + TempStr1;
                else
                    TempStr1 = TempStr1.erase(0, 1);
            }
            OutputLine += TempStr1 + " ";
        }
        SubVariableIndex++;                                                      // chi so cua bien phu
        string SubVariable = "h" + to_string(SubVariableIndex);                  // ten bien phu
        OutputLine.replace(OutputLine.find("="), 2, "+ " + SubVariable + " = "); // them ten bien phu va ve phai vao xau
        OutVector.push_back(OutputLine);                                         // luu cac rang buoc da chuyen ve dang <=
    }
    // in ra cac rang buoc da chuyen ve dang <=
    cout << endl
         << "Tu de bai ta co: " << endl
         << endl;
    for (string t : OutVector)
    {
        cout << t << endl;
    }
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
    cout << endl
         << "Ta co bang simplex da duoc chuan hoa: " << endl;
    PrintSimplexTable();
}
void DoSimplex()
{
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
        if (NegCol <= -1)
        {
            cout << " Bang da toi uu! " << endl;
            break;
        }
        cout << "Cot co he so am nho nhat la: " << VariableCol[NegCol] << " = " << f[M + 1][NegCol] << endl;
        cout << "=> Bien vao la: " << VariableCol[NegCol] << endl
             << endl;

        // neu khong tim thay (bang toi uu) thi dung thuat toan
        if (NegCol <= -1)
            break;
        cout << "Tren cot " << VariableCol[NegCol] << " co cac hang: " << endl;
        // tim hang co ti so nho nhat trong cot
        float Ratio = INT_MAX;
        int NegRow = -1;
        FOR(i, 1, M)
        {
            if (f[i][NegCol] <= 0)
                continue;
            cout << "Hang " << VariableRow[i] << " co ti so: " << f[i][N + M + 1] << "/" << f[i][NegCol] << '=' << f[i][N + M + 1] / f[i][NegCol] << endl;
            float RatioI = (f[i][N + M + 1]) / (f[i][NegCol]);
            if (RatioI < Ratio)
            {
                Ratio = RatioI;
                NegRow = i;
            }
        }
        cout << endl
             << "Hang co ti so nho nhat la: " << VariableRow[NegRow] << " = " << f[NegRow][N + M + 1] << "/" << f[NegRow][NegCol] << '=' << Ratio << endl;
        cout << "=> Bien ra la: " << VariableRow[NegRow] << endl;
        cout << "=> Diem xoay la: " << VariableRow[NegRow] << " / " << VariableCol[NegCol] << endl;
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
        cout << endl
             << " Bang sau khi cap nhat cac gia tri: " << endl;
        PrintSimplexTable();
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
         << "Gia tri toi da cua Z la: " << f[M + 1][N + M + 1];
    cout << endl;
    cout << "Gia tri cac bien la: " << endl;
    FOR(i, 1, N)
    {
        cout << "x" << i << " = " << x[i];
        if (i < N)
            cout << " , ";
    }
}
Coord MouseCoord()
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

    INPUT_RECORD inputRecord;
    DWORD events;
    COORD mousePos;

    while (true) {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);

        if (inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;

            if (mouseEvent.dwEventFlags == 0 && (mouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
                mousePos = mouseEvent.dwMousePosition;
                return {mousePos.X,mousePos.Y};
            }
        }
    }
}
bool HitKey()
{
	while(1){
		if(kbhit()) return 1;
	}
	return 0;
}
void SolveProblem1()
{
	system("cls");
	cout << "Giai bai toan 1!" << endl;
	cout << "An phim bat ki de quay lai!";
	if(HitKey()) {
		ResetConsole();
		return ;
	}
}
void SolveProblem2()
{
	system("cls");
	cout << "Giai bai toan 2!" << endl;
	cout << "An phim bat ki de quay lai!";
	if(HitKey()) {
		ResetConsole();
		return ;
	}
}
void SolveProblem3()
{
	system("cls");
	cout << "Giai bai toan 3! " << endl;
	cout << "An phim bat ki de quay lai!";
	if(HitKey()) {
		ResetConsole();
		return ;
	}
}
int main()
{
	Resize(2100,1100);
	PrintMainContent();
	while(1)
	{
		Coord click = MouseCoord();
		if(click.se < 15 or click.se > 18) continue;
		if(click.fi < 55 or click.fi > 80) continue;
		if(click.se == 15 and click.fi >= 55 and click.fi <= 70) SolveProblem1();
		else if(click.se == 16 and click.fi >= 55 and click.fi <= 70) SolveProblem2();
		else if(click.se == 17 and click.fi >= 55 and click.fi <= 70) SolveProblem3();
		else if(click.se == 18 and click.fi >= 55 and click.fi <= 80) return 0;
		else continue;
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

2
2
40 30
2x1 + x2 <= 16
x1 + x2 <= 9

3
2
3 5
-2x1 + -4x2 >= -40
3x1 + 2x2 <= 36
6x1 + 5x2 <= 60

2
2
8 6
4x1 + 2x2 <= 60
2x1 + 4x2 <= 48
*/
