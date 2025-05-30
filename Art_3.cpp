#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <windows.h>
#include <conio.h>
#include <math.h>
#define FOR(i, a, b) for (int i = a; i <= b; i++)
#define MAX 100
#define Coord pair<int, int>
#define fi first
#define se second
const double EPS = 0.0001; // sai so cho phep
using namespace std;
void Phase2(); // forward declaration
void NhapTuBanPhim();
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
int N, M, TotalSubVariable, NumOfConstrants, NumOfVariables;
int SubVariableIndex;             // bien chua chi so cua bien phu
int ArtificalIndex;               // bien chua chi so cac bien nhan tao
double f[MAX + 1][MAX + 1] = {0}; // mang de luu ket qua o bang simplex
string x[MAX];                    // hai mang dung de luu ten cac bien trong bang
string FileName, LMS = "";
double ZFactor[MAX + 1]; // mang de luu he so cua da thuc Z
bool IsFileInput, IsMax, IsPossible = 1, IsInput;
int Phase = 1;
vector<string> OutVector, VariableRow(MAX + 1), VariableCol(MAX + 1); // mang de luu cac rang buoc da chuyen ve dang <=
vector<int> ArtificalRow(MAX + 1), SubRow(MAX + 1);                   // mang chua ve phai cua bat phuong trin
vector<double> ValueOfConstrants;                                     // mang chua ve phai cua bat phuong trinh
ifstream FileCin;                                                     // bien de luu file input
int LeftMargin = 55;
int TopMargin = 2;
// ham bat event nhan phim
bool HitKey()
{
    while (1)
    {
        if (kbhit())
            return 1;
    }
    return 0;
}
void PrintTitle()
{
    SetWordColor(0x09);
    char RightTop = 187, LeftTop = 201, LeftBottom = 200, RightBottom = 188;
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
    SetWordColor(0x0b);
    cout << "        DO AN LAP TRINH TINH TOAN";
    FOR(i, 1, 15)
    cout << " ";
    SetWordColor(0x09);
    cout << UpBorder;
    gotoXY(LeftMargin, TopMargin + 2);
    cout << UpBorder;
    FOR(i, 1, 10)
    cout << " ";
    SetWordColor(0x0b);
    cout << "De tai: Giai bai toan quy hoach tuyen tinh ";
    FOR(i, 1, 5)
    cout << " ";
    SetWordColor(0x09);
    cout << UpBorder;
    gotoXY(LeftMargin, TopMargin + 3);
    cout << UpBorder;
    gotoXY(LeftMargin + 59, TopMargin + 3);
    cout << UpBorder;
    gotoXY(LeftMargin, TopMargin + 4);
    cout << UpBorder;
    SetWordColor(0x0b);
    cout << "    Giao vien huong dan:                                  ";
    SetWordColor(0x09);
    cout << UpBorder;
    gotoXY(LeftMargin, TopMargin + 5);
    cout << UpBorder;
    SetWordColor(0x0b);
    cout << "    1. Nguyen Tan Khoi                                    ";
    SetWordColor(0x09);
    cout << UpBorder;
    gotoXY(LeftMargin, TopMargin + 6);
    cout << UpBorder;
    gotoXY(LeftMargin + 59, TopMargin + 6);
    cout << UpBorder;
    TopMargin++;
    gotoXY(LeftMargin, TopMargin + 6);
    cout << UpBorder;
    SetWordColor(0x0b);
    cout << "    Sinh vien thuc hien:                                  ";
    SetWordColor(0x09);
    cout << UpBorder;
    gotoXY(LeftMargin, TopMargin + 7);
    cout << UpBorder;
    SetWordColor(0x0b);
    cout << "    1. Nguyen Dang Phong                                  ";
    SetWordColor(0x09);
    cout << UpBorder;
    gotoXY(LeftMargin, TopMargin + 8);
    cout << UpBorder;
    SetWordColor(0x0b);
    cout << "    2. Ngo Van Dac Tri                                    ";
    SetWordColor(0x09);
    cout << UpBorder;
    gotoXY(LeftMargin, TopMargin + 9);
    cout << UpBorder;
    SetWordColor(0x0b);
    cout << "    3. Tran Le Minh                                       ";
    SetWordColor(0x09);
    cout << UpBorder;
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
    SetWordColor(0x02);
    gotoXY(LeftMargin, TopMargin);
    cout << "=========================== MENU ===========================";
    gotoXY(LeftMargin, TopMargin + 1);
    SetWordColor(0x0e);
    cout << "1. Nhap du lieu moi tu ban phim: "; // 33
    gotoXY(LeftMargin, TopMargin + 2);
    cout << "2. Nhap du lieu moi tu file: "; // 29
    gotoXY(LeftMargin, TopMargin + 3);
    cout << "3. Giai bai toan vua nhap: "; // 27
    gotoXY(LeftMargin, TopMargin + 4);
    cout << "4. Ket thuc chuong trinh! "; // 25
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
void EndOfFunction()
{
    if (HitKey())
    {
        ResetConsole();
        return;
    }
}
void PrintSimplexTable()
{
    SetWordColor(0x0e);
    int TTS = TotalSubVariable;
    // In phan tieu de cua cac cot
    char x = char(186);
    char y = char(205);
    cout << char(201);
    int CellSize = 12;
    FOR(k, 1, (N + TTS + 2) * (CellSize + 1) - 1)
    {
        if (k % (CellSize + 1) == 0)
        {
            cout << char(203);
            continue;
        }
        cout << y;
    }
    cout << char(187) << endl;
    cout << x << setw(CellSize);
    FOR(i, 0, N + TTS)
    {
        cout << VariableCol[i];
        cout << x << setw(CellSize);
    }
    cout << setw(CellSize) << "Gia tri" << x << endl;
    VariableRow[M + 1] = "Z";

    // In cac hang trong cot
    FOR(i, 1, M + 1)
    {
        // in dong ngan cach
        cout << char(204); // LMS
        FOR(k, 1, (N + TTS + 2) * (CellSize + 1))
        {
            if (k % (CellSize + 1) == 0)
            {
                if (k == (N + TTS + 2) * (CellSize + 1))
                    cout << char(185);
                else
                    cout << char(206);

                continue;
            }
            cout << y;
        }
        cout << endl; //<< LMS;
        cout << x << setw(CellSize) << VariableRow[i] << x << setw(CellSize);
        FOR(j, 1, N + TTS + 1)
        {
            cout << setprecision(3) << fixed;
            cout << f[i][j] << x;
            if (j <= N + TTS)
                cout << setw(CellSize);
        }
        cout << endl;
    }
    // in dong ngan cach cuoi cung
    cout << char(200); // LMS
    FOR(k, 1, (N + TTS + 2) * (CellSize + 1) - 1)
    {
        if (k % (CellSize + 1) == 0)
            cout << char(202);
        else
            cout << y;
    }
    cout << char(188);

    cout << endl
         << endl;
}
void InitTheTable()
{
    TotalSubVariable = SubVariableIndex + ArtificalIndex;
    FOR(i, 1, LeftMargin)
    LMS += " ";
    // Simplex method
    // in ra cac rang buoc da chuyen ve dang <=
    SetWordColor(0x09);
    cout << "Tu de bai ta co: " << endl
         << endl;
    SetWordColor(0x0e);
    FOR(i, 0, OutVector.size() - 1)
    {
        if (OutVector[i].size() > 0)
            cout << OutVector[i] << endl;
    }
    FOR(i, 1, N)
    {
        VariableCol[i] = "x" + to_string(i);
    }
    FOR(i, N + 1, N + SubVariableIndex)
    {
        VariableCol[i] = "s" + to_string(i - N);
    }
    FOR(i, N + SubVariableIndex + 1, N + TotalSubVariable + 1)
    {
        VariableCol[i] = "a" + to_string(i - N - SubVariableIndex);
    }
    // gan cac ten bien vao mang
    int AIndex, SIndex;
    AIndex = SIndex = 1;
    FOR(i, 1, M)
    {
        f[i][N + TotalSubVariable + 1] = ValueOfConstrants[i]; // gan gia tri cot Gia tri
        // Gan ten cho cac bien co so
        if (ArtificalRow[AIndex] == i)
        {
            VariableRow[i] = "a" + to_string(AIndex);
            AIndex++;
        }
    }
    FOR(i, 1, M)
    {
        if (SubRow[SIndex] == i) // neu hang i la hang phu
        {
            if (VariableRow[i][0] != 'a')
                VariableRow[i] = "s" + to_string(SIndex);
            SIndex++;
        }
    }
    // sua lai cac chi so doi voi bang bien nhan tao
    if (ArtificalIndex > 0)
    {
        f[M + 1][N + SubVariableIndex + ArtificalIndex + 1] = 0;
        FOR(j, 1, N + SubVariableIndex) // update W o cot thu i
        {
            f[M + 1][j] = 0;
            FOR(i, 1, M)
            {
                if (VariableRow[i][0] == 'a')
                {
                    f[M + 1][j] -= f[i][j];
                    if (j == 1)
                        f[M + 1][N + SubVariableIndex + ArtificalIndex + 1] -= f[i][N + SubVariableIndex + ArtificalIndex + 1];
                }
            }
        }
    }
    SetWordColor(0x03);
    cout << endl
         << "Ta co bang simplex da duoc chuan hoa: " << endl;
    PrintSimplexTable();
}
void DoSimplex()
{
    int LastRow = N + TotalSubVariable + 1;
    // thuc hien thuat toan simplex
    // Thuat toan Simplex 2 pha
    while (1)
    {
        // tim cot co he so am nho nhat
        double MostNegative = 1;
        int NegCol = -1;
        FOR(i, 1, N + TotalSubVariable)
        {
            if (f[M + 1][i] + EPS < 0)
            {
                if (f[M + 1][i] < MostNegative)
                {
                    MostNegative = f[M + 1][i];
                    NegCol = i;
                }
                bool PositiveFlag = 0;
                FOR(j, 1, M)
                {
                    if (f[j][i] > 0)
                    {
                        PositiveFlag = 1; // neu co phan tu duong trong cot thi khong phai la cot toi uu
                        break;
                    }
                }
                if (!PositiveFlag)
                {
                    SetWordColor(0x03);
                    cout << "Bai toan tren khong co cuc bien, khong co nghiem toi uu!" << endl
                         << "An phim bat ki de quay lai!";
                    EndOfFunction();
                    return;
                }
            }
        }
    	SetWordColor(0x0b);
        if (NegCol <= -1)
        {
            cout << endl
                 << " Bang da toi uu! " << endl
                 << endl;
            break;
        }
        cout << "Cot co he so am nho nhat la: " << VariableCol[NegCol] << " = " << f[M + 1][NegCol] << endl;
        cout << "=> Bien vao la: " << VariableCol[NegCol] << endl;
        // neu khong tim thay (bang toi uu) thi dung thuat toan
        if (NegCol <= -1)
            break;
        cout << endl
             << "Tren cot " << VariableCol[NegCol] << " co cac hang: " << endl;
        // tim hang co ti so nho nhat trong cot
        double Ratio = INT_MAX;
        int NegRow = -1;
        FOR(i, 1, M)
        {
            if (fabs(f[i][NegCol]) <= EPS)
                continue; // neu phan tu bang 0 thi bo qua
            double RatioI = f[i][LastRow] / (f[i][NegCol]);
            if (RatioI <= 0 + EPS)
                continue; // neu ti so am thi bo qua
            cout << "Hang " << VariableRow[i] << " co ti so: " << f[i][LastRow] << "/" << f[i][NegCol] << " = " << f[i][LastRow] / f[i][NegCol] << endl;
            if (RatioI < Ratio)
            {
                Ratio = RatioI;
                NegRow = i;
            }
        }
        cout << endl
             << "Hang co ti so nho nhat la: " << VariableRow[NegRow] << " = " << f[NegRow][LastRow] << "/" << f[NegRow][NegCol] << " = " << Ratio << endl;
        cout << "=> Bien ra la: " << VariableRow[NegRow] << endl;
        cout << "=> Diem xoay la: " << VariableRow[NegRow] << " / " << VariableCol[NegCol] << endl;
        // chia cac phan tu trong hang cho phan tu tai diem xoay
        FOR(i, 1, LastRow)
        {
            if (i != NegCol)
                f[NegRow][i] /= f[NegRow][NegCol];
        }
        // bien doi tuong tu phep khu Gauss
        f[NegRow][NegCol] = 1;
        cout << endl
             << "Bang sau khi chia cac phan tu trong hang cho phan tu tai diem xoay: " << endl;
        PrintSimplexTable();
        FOR(i, 1, M + 1)
        {
            if (i != NegRow)
            {
                double Factor = f[i][NegCol];
                FOR(j, 1, LastRow)
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
}
void Phase1()
{
    SetWordColor(0x0b);
    cout << "Pha 1: " << endl
         << endl;
    SetWordColor(0x0e);
    Phase = 1;
    InitTheTable();          // khoi tao bang simplex
    IsPossible = 1;          // khoi tao bien co nghiem toi uu
    if (ArtificalIndex == 0) // neu khong co bien nhan tao thi khong can pha 1
    {
        Phase = 2;
        Phase2();
        return;
    }
    DoSimplex();
    // xet ket qua cua pha 1
    FOR(i, 1, M)
    {
        if (VariableRow[i][0] == 'a')
        {
            IsPossible = 0; // neu co bien nhan tao thi khong co nghiem toi uu
        }
    }
    if (fabs(f[M + 1][N + TotalSubVariable + 1]) > EPS)
    {
        IsPossible = 0; // neu Z khac 0 thi khong co nghiem toi uu
        // cout << "?!?";
    }
    if (!IsPossible)
    {
        SetWordColor(0x0b);
        cout << "Sau Pha 1, Z = " << f[M + 1][N + TotalSubVariable + 1] << " != 0" << endl;
        cout << "=> Bai toan khong co nghiem toi uu!" << endl;
        SetWordColor(0x09);
        cout << "An phim bat ki de quay lai!" << endl;
        EndOfFunction();
        return;
    }

    if (SubVariableIndex == 0)
    {
        double Res = 0;
        SetWordColor(0x0b);
        cout << "Theo bang, ta thay he co nghiem: " << endl;
        FOR(i, 1, N)
        {
            cout << "x" << i << " = " << f[i][N + TotalSubVariable + 1];
            Res -= ZFactor[i] * f[i][N + TotalSubVariable + 1];
            if (i < N)
                cout << " ,";
        }
        cout << endl
             << "Z = " << Res;
        cout << endl;
        SetWordColor(0x09);
        cout << "An phim bat ki de quay lai!" << endl;
        EndOfFunction();
        return;
    }
    Phase = 2;
    return;
}
void Phase2()
{
    SetWordColor(0x0b);
    cout << "Bai toan khong con bien nhan tao, ta thuc hien pha 2:" << endl
         << endl;
    // set up
    //  xoa cac hang chua bien nhan tao
    FOR(i, 1, M)
    {
        if (VariableRow[i][0] == 'a')
        {
            VariableRow.erase(VariableRow.begin() + i); // xoa bien nhan tao
        }
    }
    // xoa cac cot co bien nhan tao
    FOR(i, 1, M + 1)
    {
        f[i][N + SubVariableIndex + 1] = f[i][N + TotalSubVariable + 1]; // xoa cot Z
    }
    ArtificalIndex = 0;                  // reset bien nhan tao
    TotalSubVariable = SubVariableIndex; // reset bien phu
    // gan lai cac gia tri cho hang Z
    //  FOR(j)
    FOR(i, 1, N)
    {
        f[M + 1][i] = ZFactor[i]; // gan lai he so cua da thuc Z
    }
    FOR(i, 1, M)
    {
        if (VariableRow[i][0] == 'x')
            FOR(j, 1, N + TotalSubVariable + 1)
            {
                int Index = stoi(VariableRow[i].substr(1, VariableRow[i].length() - 1));
                f[M + 1][j] -= f[i][j] * ZFactor[Index]; // cap nhat he so cua da thuc Z
            }
        // gan lai he so cua da thuc Z
    }
    PrintSimplexTable();
    DoSimplex(); // thuc hien thuat toan don hinh
    // tim cac gia tri cua x theo cac he so tren bang da bien doi
    double x[100];
    FOR(i, 1, N)
    x[i] = 0;
    FOR(i, 1, M)
    {
        if (VariableRow[i][0] == 'x')
        {
            int Index = stoi(VariableRow[i].substr(1, VariableRow[i].length() - 1));
            x[Index] = f[i][N + TotalSubVariable + 1];
        }
    }
    // in ra ket qua
    SetWordColor(0x0b);
    cout << endl
         << "Gia tri ";
    if (IsMax)
        cout << "toi da ";
    else
        cout << "toi thieu ";
    cout << "cua Z la: " << f[M + 1][N + TotalSubVariable + 1] * (2 * IsMax - 1);
    cout << endl;
    cout << "Gia tri cac bien la: " << endl;
    FOR(i, 1, N)
    {
        cout << "x" << i << " = " << x[i];
        if (i < N)
            cout << " , ";
    }
    SetWordColor(0x09);
    cout << endl
         << "An phim bat ki de quay lai!" << endl;
    EndOfFunction();
}
void TwoPhase()
{
    system("cls");
    if (!IsInput)
    {
        SetWordColor(0x04);
        cout << "Chua nhap du lieu, vui long nhap du lieu truoc!" << endl;
        EndOfFunction();
        return;
    }
    InitTheTable(); // khoi tao bang simplex
    Phase1();
    if (Phase == 2)
    {
        Phase2();
    }
    // reset cac bien
    IsInput = 0;
    IsFileInput = 0;
    IsMax = 0;
    IsPossible = 1;
    NumOfConstrants = 0;
    NumOfVariables = 0;
    SubVariableIndex = 0;
    ArtificalIndex = 0;
    OutVector.clear();
    VariableRow.clear();
    VariableRow.resize(MAX + 1);
    VariableCol.clear();
    VariableCol.resize(MAX + 1);
    ValueOfConstrants.clear();
    ValueOfConstrants.resize(MAX + 1);
    ArtificalRow.clear();
    ArtificalRow.resize(MAX + 1);
    SubRow.clear();
    SubRow.resize(MAX + 1);
    FOR(i, 1, M + 1)
    FOR(j, 1, N + TotalSubVariable + 1)
    f[i][j] = 0;

    EndOfFunction();
}
Coord MouseCoord()
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

    INPUT_RECORD inputRecord;
    DWORD events;
    COORD mousePos;

    while (true)
    {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);

        if (inputRecord.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;

            if (mouseEvent.dwEventFlags == 0 && (mouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
            {
                mousePos = mouseEvent.dwMousePosition;
                return {mousePos.X, mousePos.Y};
            }
        }
    }
}
void NotValid()
{
	system("cls");
	SetWordColor(0x04);
    cout << "Nhap khong hop le!" << endl;
    Sleep(20);
    if(IsFileInput)
    {
    	FileCin.close();
	}
	cout << "An phim bat ky de quay lai!";
    EndOfFunction(); // quay lai nhap lai du lieu
    return;
}
string ModifyString(string Input) // ham chuan hoa xau
{
    string Res;
    string tem;
    stringstream ss(Input);
    while (ss >> tem)
    {
        Res += tem; // them khoang trang giua cac tu
        Res += ' ';
    }
    Res.erase(Res.begin() + Res.size() - 1); // xoa khoang trang cuoi cung
    return Res;
}
bool Validdouble(string x)
{
    if (x.empty())
        return 0;
    int dotCount = 0;
    for (char c : x)
    {
        if (c == '.')
            dotCount++;
        else if (!isdigit(c))
            return 0; // neu co ki tu khac so va dau cham thi khong hop le
    }
    return dotCount <= 1; // chi cho phep toi da 1 dau cham
}
void doubleInput(double &a)
{
    SetWordColor(0x03);
    string Temp;
    if (IsFileInput)
    {
        FileCin >> Temp;
    }
    else
        cin >> Temp;
    Temp = ModifyString(Temp); // chuan hoa xau
    if (!Validdouble(Temp))
    {
        NotValid();
        return;
    }
    stringstream ss(Temp);
    ss >> a;
    if (IsFileInput)
        cout << a;
}
void ConstrantsInput(string &x)
{
    SetWordColor(0x03);
    x = ModifyString(x); // chuan hoa xau rang buoc
    // check valid constrants
    if (x.empty())
    {
        NotValid();
        return;
    }
    int uppercount = 0, lowercount = 0, equal = 0, xcount = 0;
    for (char c : x)
    {
        if (c == ' ')
            continue;
        if (c == '>')
            uppercount++;
        else if (c == '<')
            lowercount++;
        else if (c == '=')
            equal++;
        else if (c == 'x')
            xcount++;
        else if (!isdigit(c) && c != '+' && c != '-' && c != ' ' && c != '.')
        {
            NotValid();
            return; // neu co ki tu khac so, dau cham, dau cong, dau tru thi khong hop le
        }
    }
    if (uppercount > 1 || lowercount > 1 || equal != 1 || xcount > 10 || xcount < 1)
    {
        NotValid();
        return; // neu co qua nhieu dau >, <, = hoac khong co bien x thi khong hop le
    }
}
void IntegerInput(int &x)
{
    SetWordColor(0x03);
    string Temp;
    if (IsFileInput)
    {
        FileCin >> Temp;
    }
    else
        cin >> Temp;
    cout << "";
    Temp = ModifyString(Temp); // chuan hoa xau
    if (Temp.find_first_not_of("0123456789") != string::npos)
    {
        NotValid();
        return; // neu co ki tu khac so thi khong hop le
    }
    x = stoi(Temp);
    if (IsFileInput)
        cout << x;
}
void NhapTuBanPhim()
{
    system("cls");
    IsInput = 1;
    string StrMax;
    SetWordColor(0x0e);
    cout << "Toi thieu hoa/Toi da hoa: ";
    SetWordColor(0x03);
    if (IsFileInput)
    {
        getline(FileCin, StrMax);
    }
    else
        getline(cin, StrMax);
    StrMax = ModifyString(StrMax); // chuan hoa xau
    if (StrMax == "Toi thieu hoa")
        IsMax = 0;
    else if (StrMax == "Toi da hoa")
        IsMax = 1;
    else
    {
        NotValid();
        return;
    } // neu khong hop le thi bao loi
    SetWordColor(0x03);
    if (IsFileInput)
        cout << StrMax << endl; // in ra dong da nhap
    // NumOfConstrants
    SetWordColor(0x0e);
    cout << "So rang buoc: ";
    IntegerInput(NumOfConstrants);
    if (IsFileInput)
        cout << endl;
    // NumOfVariables
    SetWordColor(0x0e);
    cout << "So bien cua da thuc: ";
    IntegerInput(NumOfVariables);

    if (IsFileInput)
        cout << endl;
    N = NumOfVariables, M = NumOfConstrants;
    //  Nhap cac he so cua da thuc Z
    SetWordColor(0x0e);
    cout << "Cac he so cua da thuc Z: ";
    FOR(i, 1, N)
    {
        doubleInput(f[M + 1][i]);
        if (IsFileInput)
            cout << " ";
        f[M + 1][i] *= -1;        // gan cac he so cua da thuc Z cho hang cuoi cua bang
        ZFactor[i] = f[M + 1][i]; // luu he so cua da thuc Z vao mang ZFactor
        if (IsMax == 0)
        {
            f[M + 1][i] *= -1; // neu la toi thieu thi doi dau he so cua da thuc Z
            ZFactor[i] *= -1;  // luu he so cua da thuc Z vao mang ZFactor
        }
    }
    if (IsFileInput)
        cout << endl;
    SetWordColor(0x0e);
    cout << "Cac rang buoc :" << endl;
    //   Nhap cac rang buoc
    ValueOfConstrants.clear();
    ValueOfConstrants.resize(M + 1); // mang chua ve phai cua bat phuong trinh
    cin.ignore();                    // xoa bo nho dem (dung khi nhap string)
    if (IsFileInput)
        FileCin.ignore();
    fflush(stdin);
    OutVector.clear();
    OutVector.resize(M + 1);
    // Cac bien de xu li cac rang buoc

    SubVariableIndex = 0; // bien chua chi so cua bien phu
    ArtificalIndex = 0;   // bien chua chi so cac bien nhan tao
    double s[MAX + 1];    // mang a va s de luu cac he so cua bien nhan tao va bien phu
    // Xu li cac rang buoc
    SetWordColor(0x03);
    FOR(i, 1, M) // xu li rang buoc thu i
    {
        // fflush(stdin);
        string TempStr, InputLine, OutputLine;
        if (IsFileInput)
        {
            getline(FileCin, InputLine);
        }
        else
            getline(cin, InputLine); // nhap dong input duoi dang mot string
        ConstrantsInput(InputLine);  // chuan hoa xau rang buoc
        if (IsFileInput)
            cout << InputLine << endl; // in ra dong da nhap
        stringstream InputStream(InputLine);
        int sign = 1, PrepSign = 1;    // bien danh dau dau cua pt
        while (InputStream >> TempStr) // tach tung tu cua dong va nhap vao xau TempStr
        {
            if (TempStr == "+")
                PrepSign = 1;
            else if (TempStr == "-")
                PrepSign = -1;
            else
            {
                // neu khong phai la ki tu + hoac -
                if (TempStr[0] == '>' or TempStr[0] == '<' or TempStr[0] == '=')
                {
                    if (TempStr[0] == '>')
                        sign = -1;
                    else if (TempStr[0] == '=')
                        sign = 0;
                    if (TempStr.size() > 2)
                    {
                        NotValid(); // neu co qua nhieu ki tu trong xau rang buoc thi khong hop le
                        return; 
                    }
                }
                else
                {
                    int xpos = TempStr.find('x'); // tim vi tri cua ki tu 'x'
                    if (xpos > TempStr.length())  // neu khong ton tai 'x' trong xau ki tu (ve phai chua gia tri)
                    {
                        stringstream TempStream(TempStr);
                        TempStream >> ValueOfConstrants[i]; // gan gia tri cho mang ValueOfConstrants
                        // ValueOfConstrants[i] = stoi(TempStr); // gan gia tri cho mang ValueOfConstrants
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
                        double FactorValue;
                        stringstream FactorStream(Factor);
                        FactorStream >> FactorValue; // chuyen he so sang kieu double
                        f[i][stoi(VariableIndex)] = PrepSign * FactorValue; // gan he so cua bien x vao bang simplex
                        // gan he so cho bien x trong bang simplex
                    }
                }
            }
        }
        // chuyen cac bpt ve thanh pt
        bool IsArtifical = 0, IsSub = 0;
        if (sign != 0)
        {
            SubVariableIndex++;
            s[SubVariableIndex] = sign;
            SubRow[SubVariableIndex] = i; // luu chi so hang co bien phu
            IsSub = 1;
        }
        if (sign != 1)
        {
            IsArtifical = 1;
            ArtificalIndex++;
            ArtificalRow[ArtificalIndex] = i; // luu chi so hang co bien nhan tao
            // a[ArtificalIndex] = 1;
        }
        // in ra rang buoc da chuyen ve dang pt
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
            OutputLine += TempStr1 + " ";
        }
        // them ten bien phu vao ve phai vao xau
        if (IsSub)
        {
            string SubVariable = "s" + to_string(SubVariableIndex); // ten bien phu
            if (IsArtifical)
                SubVariable = "-" + SubVariable;
            OutputLine.replace(OutputLine.find("="), 2, "+ " + SubVariable + " = ");
        }
        if (IsArtifical)
        {
            string ArtificalVariable = "a" + to_string(ArtificalIndex); // ten bien nhan tao
            OutputLine.replace(OutputLine.find("="), 2, "+ " + ArtificalVariable + " = ");
        }
        OutVector.push_back(OutputLine); // luu cac rang buoc da chuyen ve dang <=
    }
    TotalSubVariable = SubVariableIndex + ArtificalIndex;
    FOR(i, N + 1, N + 1 + SubVariableIndex)
    VariableCol[i] = "s" + to_string(i - N);
    FOR(i, N + 1 + SubVariableIndex, N + 1 + SubVariableIndex + ArtificalIndex)
    VariableCol[i] = "a" + to_string(i - N - SubVariableIndex);
    SetWordColor(0x0b);
    cout << endl
         << "Nhap bai toan thanh cong, an phim bat ki de quay lai!" << endl;

    // mot phan cua ham InitTheTable nhung viet o day

    FOR(i, 1, SubVariableIndex)
    {
        f[SubRow[i]][N + i] = s[i];
    }
    FOR(i, 1, ArtificalIndex)
    {
        f[ArtificalRow[i]][N + SubVariableIndex + i] = 1;
    }
    EndOfFunction();
}
void NhapTuFile()
{
    system("cls");
    SetWordColor(0x0e);
    cout << "Nhap ten file input: ";
    SetWordColor(0x03);
    cin >> FileName;
    IsFileInput = 1;
    FileCin.open(FileName.c_str());
    if (!FileCin)
    {
        SetWordColor(0x04);
        cout << "Khong the mo file, vui long kiem tra lai ten file!" << endl;
        cout << "An phim bat ki de quay lai!" << endl;
        EndOfFunction();
        return;
    }
    NhapTuBanPhim();
    IsFileInput = 0; // reset bien IsFileInput
    FileCin.close();
}
int main()
{
    Resize(2100, 1100);
    PrintMainContent();
    int L1 = 33, L2 = 29, L3 = 27, L4 = 25;
    while (1)
    {
        Coord click = MouseCoord();
        if (click.se <  15 or click.se > 18)
            continue;
        if (click.fi < 55 or click.fi > 80)
            continue;
        if (click.se == 15 and click.fi >= 55 and click.fi <= 55 + L1)
            NhapTuBanPhim();
        else if (click.se == 16 and click.fi >= 55 and click.fi <= 55 + L2)
            NhapTuFile();
        else if (click.se == 17 and click.fi >= 55 and click.fi <= 55 + L3)
            TwoPhase();
        else if (click.se == 18 and click.fi >= 55 and click.fi <= 55 + L4)
        {
            HWND consoleWindow = GetConsoleWindow();
            ShowWindow(consoleWindow, SW_HIDE);
            return 0;
        }
        else
            continue;
    }
}
/*
Toi thieu hoa
3
2
3000.5 5000.5
3.1x1 + 2x2 >= 27.1
2x1 + 9x2 = 20
x1 + x2 <= 39.1

Toi thieu hoa
3
3
10000 10000 10000
x1 + x2 + x3 <= 100
x1 + 2x2 + 3x3 <= 75
3x1 + 5x2 + 7x3 = 50

Toi da hoa
3
3
1 1 1
1000x1 + 1000x2 + 1000x3 <= 250000
500x1 + 20000x2 + 70000x3 = 1000000
100x1 + 50x2 + 80x3 <= 500000


Toi da hoa
2
2
2 3
x1 + 2x2 <= 10
x1 + x2 <= 4

Toi da hoa
3
2
3 5
3x1 + 2x2 = 27
2x1 + 9x2 >= 20
x1 + x2 <= 39

Toi thieu
2
2
1 -2
x1 + x2 = 3
2x1	- x2 = 1

Toi thieu hoa
2
2
1 1
x1 + x2 >= 5
x1 + 2x2 >= 9

Toi thieu
2
2
1 1
x1 + x2 <= 5
2x1	+ x2 <= 8
*/
/*
Toi thieu
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

Toi thieu
2
2
8 6
4x1 + 2x2 <= 60
2x1 + 4x2 <= 48
*/