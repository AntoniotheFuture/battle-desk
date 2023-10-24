// battle-desk.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "battle-desk.h"
#include <vector>
#include <iostream>
#include <string>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
std::vector<std::string> screenList;


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProcB(HWND, UINT, WPARAM, LPARAM);


// values limit
INT MIN_X_SPLIT = 2;
INT MAX_X_SPLIT = 30;
INT MIN_Y_SPLIT = 2;
INT MAX_Y_SPLIT = 30;
INT MIN_FONT_SIZE = 5;
INT MAX_FONT_SIZE = 100;
INT MIN_LINE_WIDTH = 2;
INT MAX_LINE_WIDTH = 100;

INT SCREEN = 0;
bool ENABLE = TRUE;
COLORREF DRAW_COLOR = RGB(255, 0, 0);
COLORREF LINE_STROKE_COLOR = RGB(255, 255, 255);
BYTE ALPHA = 255;
INT X_SPLIT = 6;
INT Y_SPLIT = 4;
INT LINE_WIDTH = 10;
BOOL OPPOSED = TRUE; 
BOOL FULL_GRID = FALSE;
BOOL STROKE = TRUE;
INT FONT_SIZE = 30;

HWND lineWidthSlider; 
HWND lineWidthText;
HWND fontSizeSlider;
HWND fontSizeText;

HWND colorPicker;
HWND strokePicker;


HWND screenSelect;
HWND enableCheck;
HWND xSplitSelect;
HWND ySplitSelect;

HWND opposedCheck;
HWND gridCheck;



HWND helpBtn;

HWND screenHwnd;

WNDCLASSEX wct;

void GetScreenList() {
    int screenCount = GetSystemMetrics(SM_CMONITORS);

    for (int i = 0; i < screenCount; i++) {
        DISPLAY_DEVICE displayDevice;
        displayDevice.cb = sizeof(DISPLAY_DEVICE);
        if (EnumDisplayDevices(nullptr, i, &displayDevice, 0)) {
            std::wstring wstr(displayDevice.DeviceName);
            std::string str(wstr.begin(), wstr.end());
            screenList.push_back(str);
        }
    }
}

void readCofig() {

}

void saveConfig() {

}


void renderScreen() {
    if (!ENABLE) {
        CloseWindow(screenHwnd);
        UpdateWindow(screenHwnd);
        return;
    }
    if (IsWindow(screenHwnd) == 0) {
        screenHwnd = CreateWindow(wct.lpszClassName, _T("绘制线条和文字示例"), WS_POPUP, 0, 0, 1920, 1080, NULL, NULL, hInst, NULL);
    }
    SetWindowLong(screenHwnd, GWL_EXSTYLE, GetWindowLong(screenHwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(screenHwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    // 置于顶部
    SetWindowPos(screenHwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    ShowWindow(screenHwnd, SW_SHOWDEFAULT);
    // UpdateWindow(screenHwnd);
    RedrawWindow(screenHwnd, NULL, NULL, RDW_INVALIDATE);
}

// render to all screens
void render() {

    renderScreen();
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    GetScreenList();

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BATTLEDESK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    wct = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProcB, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("MyWindow"), NULL };
    RegisterClassEx(&wct);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BATTLEDESK));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BATTLEDESK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
      CW_USEDEFAULT, 0, 400, 400, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


void drawForm(HWND hWnd)
{
    // icons
    HBITMAP screenIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP7), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP xSplitIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP8), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP ySplitIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP9), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP lineWidthIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP5), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP opposedIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP6), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP gridIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP3), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP colorPadIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP fontSizeIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP helpIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 0, 0, 0);

    HWND hImage1 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 10, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage2 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 60, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage3 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 200, 60, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage4 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 110, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage5 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 200, 110, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage6 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 300, 110, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage7 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 160, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage8 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 200, 160, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage9 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 210, 50, 50, hWnd, NULL, hInst, NULL);
    
    SendMessageW(hImage1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)screenIcon);
    SendMessageW(hImage2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)xSplitIcon);
    SendMessageW(hImage3, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)ySplitIcon);
    SendMessageW(hImage4, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lineWidthIcon);
    SendMessageW(hImage5, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)opposedIcon);
    SendMessageW(hImage6, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)gridIcon);
    SendMessageW(hImage7, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)colorPadIcon);
    SendMessageW(hImage8, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fontSizeIcon);
    SendMessageW(hImage9, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)helpIcon);


    // screen select
    screenSelect = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 60, 16, 250, 400, hWnd, (HMENU)ID_SCREEN, hInst, NULL);
    for (const std::string& screen : screenList) {
        std::wstring wstr(screen.begin(), screen.end());
        SendMessage(screenSelect, CB_ADDSTRING, 0, (LPARAM)wstr.c_str());
    }
    SendMessage(screenSelect, CB_SETCURSEL, 0, 0);
    enableCheck = CreateWindowW(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 330, 16, 30, 30, hWnd, (HMENU)ID_ENABLE, hInst, NULL);

    // split select
    xSplitSelect = CreateWindowEx(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 60, 66, 120, 200, hWnd, (HMENU)ID_X_SPLIT, hInst, NULL);
    for (int i = MIN_X_SPLIT; i <= MAX_X_SPLIT; i++)
    {
        wchar_t buffer[10];
        _itow_s(i, buffer, 10);
        SendMessage(xSplitSelect, CB_ADDSTRING, 0, (LPARAM)buffer);
    }

    ySplitSelect = CreateWindowEx(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 250, 66, 120, 200, hWnd, (HMENU)ID_Y_SPLIT, hInst, NULL);
    for (int i = MIN_Y_SPLIT; i <= MAX_Y_SPLIT; i++)
    {
        wchar_t buffer[10];
        _itow_s(i, buffer, 10);
        SendMessage(ySplitSelect, CB_ADDSTRING, 0, (LPARAM)buffer);
    }

    // line width
    lineWidthSlider = CreateWindowEx(0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS, 60, 116, 100, 30, hWnd, (HMENU)ID_LINE_WIDTH, hInst, NULL);
    SendMessage(lineWidthSlider, TBM_SETRANGE, TRUE, MAKELONG(MIN_LINE_WIDTH, MAX_LINE_WIDTH));
    SendMessage(lineWidthSlider, TBM_SETTICFREQ, 2, 0);

    lineWidthText = CreateWindowEx(0, L"STATIC", L"0", WS_CHILD | WS_VISIBLE, 160, 116, 30, 30, hWnd, NULL, hInst, NULL);

    // opposed
    opposedCheck = CreateWindowW(L"Button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 250, 116, 30, 30, hWnd, (HMENU)ID_OPPOESD, hInst, NULL);

    // grid
    gridCheck = CreateWindowW(L"Button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 350, 116, 30, 30, hWnd, (HMENU)ID_GRID, hInst, NULL);


    // colors
    colorPicker = CreateWindow(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 60, 166, 30, 30, hWnd, (HMENU)ID_COLORPICKER, hInst, NULL);
    SetBkColor(GetDC(colorPicker), DRAW_COLOR);
    strokePicker = CreateWindow(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 100, 166, 30, 30, hWnd, (HMENU)ID_STROKECOLORPICKER, hInst, NULL);
    SetBkColor(GetDC(strokePicker), LINE_STROKE_COLOR);

    // font size
    fontSizeSlider = CreateWindowEx(0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS, 250, 166, 100, 30, hWnd, NULL, hInst, NULL);
    SendMessage(fontSizeSlider, TBM_SETRANGE, TRUE, MAKELONG(MIN_FONT_SIZE, MAX_FONT_SIZE));
    SendMessage(fontSizeSlider, TBM_SETTICFREQ, 2, 0);

    fontSizeText = CreateWindowEx(0, L"STATIC", L"0", WS_CHILD | WS_VISIBLE, 350, 166, 30, 30, hWnd, (HMENU)ID_FONT_SIZE, hInst, NULL);

    // help
    HWND staticText = CreateWindowW(L"STATIC", L"https://github.com/AntoniotheFuture/battle-desk", WS_VISIBLE | WS_CHILD | SS_NOTIFY | SS_LEFT | SS_WORDELLIPSIS,
        60, 216, 300, 30, hWnd,
        (HMENU)ID_HELP,
        NULL,
        NULL
    );

    HDC hdcButton = GetDC(staticText);  // 获取按钮的设备上下文

    // 设置文本颜色为蓝色
    SetTextColor(hdcButton, RGB(0, 0, 255));  // RGB(0, 0, 255) 表示蓝色

    ReleaseDC(staticText, hdcButton);  // 释放设备上下文

    render();
}


//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_EXIT:
            {
                DestroyWindow(hWnd);
                break;
            }
            case ID_SCREEN:
            {
                break;
            }
            case ID_ENABLE:
            {
                int checkBoxState = SendMessage(enableCheck, BM_GETCHECK, 0, 0);
                ENABLE = (checkBoxState == BST_CHECKED);
                render();
                break;
            }
            case ID_X_SPLIT:
            {

                break;
            }
            case ID_Y_SPLIT:
            {
                break;
            }
            case ID_LINE_WIDTH:
            {
                break;
            }
            case ID_OPPOESD:
            {
                int checkBoxState = SendMessage(opposedCheck, BM_GETCHECK, 0, 0);
                OPPOSED = (checkBoxState == BST_CHECKED);
                render();
                break;
            }
            case ID_GRID:
            {
                int checkBoxState = SendMessage(gridCheck, BM_GETCHECK, 0, 0);
                FULL_GRID = (checkBoxState == BST_CHECKED);
                render();
                break;
            }
            case ID_COLORPICKER:
            {
                if (HIWORD(wParam) == BN_CLICKED) {
                    CHOOSECOLOR cc;
                    COLORREF colorCustom[16] = { 0 }; // 自定义颜色数组

                    memset(&cc, 0, sizeof(cc));
                    cc.lStructSize = sizeof(cc);
                    cc.hwndOwner = hWnd; // 父窗口句柄
                    cc.lpCustColors = colorCustom;
                    cc.rgbResult = DRAW_COLOR;
                    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

                    if (ChooseColor(&cc)) {
                        DRAW_COLOR = cc.rgbResult;
                        RedrawWindow(colorPicker, NULL, NULL, RDW_INVALIDATE);
                        render();
                    }
                }
                break;
            }
            case ID_STROKECOLORPICKER:
            {
                if (HIWORD(wParam) == BN_CLICKED) {
                    CHOOSECOLOR cc;
                    COLORREF colorCustom[16] = { 0 }; // 自定义颜色数组

                    memset(&cc, 0, sizeof(cc));
                    cc.lStructSize = sizeof(cc);
                    cc.hwndOwner = hWnd; // 父窗口句柄
                    cc.lpCustColors = colorCustom;
                    cc.rgbResult = LINE_STROKE_COLOR;
                    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

                    if (ChooseColor(&cc)) {
                        LINE_STROKE_COLOR = cc.rgbResult;
                        RedrawWindow(strokePicker, NULL, NULL, RDW_INVALIDATE);
                        render();
                    }
                }
                break;
            }
            case ID_FONT_SIZE:
            {
                break;
            }
            case ID_HELP:
                ShellExecute(NULL, L"open", L"https://github.com/AntoniotheFuture/battle-desk", NULL, NULL, SW_SHOWNORMAL);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            
        }
        break;
    case WM_CREATE:
        {
            drawForm(hWnd);
        }
        break;
    case WM_DRAWITEM:
       
        if (wParam == ID_COLORPICKER)
        {
            LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;

            // 绘制按钮背景
            HBRUSH hBrush = CreateSolidBrush(DRAW_COLOR);
            HBRUSH hBorderBrush = CreateSolidBrush(RGB(50, 50, 50));
            FillRect(lpDIS->hDC, &lpDIS->rcItem, hBrush);
            FrameRect(lpDIS->hDC, &lpDIS->rcItem, hBorderBrush);
            DeleteObject(hBrush);
            DeleteObject(hBorderBrush);
            return TRUE;
        }
        if (wParam == ID_STROKECOLORPICKER)
        {
            LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;

            // 绘制按钮背景
            HBRUSH hBrush = CreateSolidBrush(LINE_STROKE_COLOR);
            HBRUSH hBorderBrush = CreateSolidBrush(RGB(50, 50, 50));
            FillRect(lpDIS->hDC, &lpDIS->rcItem, hBrush);
            FrameRect(lpDIS->hDC, &lpDIS->rcItem, hBorderBrush);
            DeleteObject(hBrush);
            DeleteObject(hBorderBrush);
            return TRUE;
        }
        
        break;
    case WM_CTLCOLORBTN: {
        HDC hDC = (HDC)wParam;
        SetBkColor(hDC, DRAW_COLOR); // 设置按钮的背景颜色为红色
        return (LRESULT)CreateSolidBrush(DRAW_COLOR);
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_HSCROLL:
        {
            int sliderValue = SendMessage(lineWidthSlider, TBM_GETPOS, 0, 0);
            wchar_t valueText[16];
            swprintf_s(valueText, L"%d", sliderValue);
            SetWindowText(lineWidthText, valueText);

            int sliderValue2 = SendMessage(fontSizeSlider, TBM_GETPOS, 0, 0);
            wchar_t valueText2[16];
            swprintf_s(valueText2, L"%d", sliderValue2);
            SetWindowText(fontSizeText, valueText2);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProcB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_PAINT: {
            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // 清除窗口上之前绘制的内容（使用白色背景覆盖）
            //RECT clientRect;
            //GetClientRect(hWnd, &clientRect);
            //HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            //FillRect(hdc, &clientRect, whiteBrush);
            //DeleteObject(whiteBrush);
            // 设置绘制颜色和字体

            HFONT font = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, _T("Arial"));
            SelectObject(hdc, font);

            // 创建画笔对象，设置线条颜色和粗细
            static HPEN hPen = CreatePen(PS_SOLID, LINE_WIDTH, DRAW_COLOR);

            int width = 1920;
            int height = 1080;
            int markWidth = 100;
            int markHeigth = 100;

            // 选择画笔对象
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

            int gridWidth = width / X_SPLIT;
            int gridHeight = height / Y_SPLIT;

            int textStartX = (gridWidth + FONT_SIZE) / 2;
            int textStartY = (gridHeight + FONT_SIZE) / 2;
            if (FULL_GRID) {
                for (int x = 1; x < X_SPLIT; x++)
                {
                    int startX = gridWidth * x;
                    MoveToEx(hdc, startX, 0, NULL);
                    LineTo(hdc, startX, height);
                    TextOut(hdc, textStartX, 20, _T("A"), 14);
                    TextOut(hdc, textStartX, height - 20 - FONT_SIZE, _T("A"), 14);
                    textStartX += gridWidth;
                }
                TextOut(hdc, textStartX, 20, _T("A"), 14);
                TextOut(hdc, textStartX, height - 20 - FONT_SIZE, _T("A"), 14);

                for (int y = 1; y < Y_SPLIT; y++)
                {
                    int startY = gridHeight * y;
                    MoveToEx(hdc, 0, startY, NULL);
                    LineTo(hdc, width, startY);
                    TextOut(hdc, 20, textStartY, _T("1"), 14);
                    TextOut(hdc, width - 20 - FONT_SIZE, textStartY, _T("1"), 14);
                    textStartY += gridHeight;
                }
                TextOut(hdc, textStartY, 20, _T("1"), 14);
                TextOut(hdc, textStartY, height - 20 - FONT_SIZE, _T("1"), 14);
            }
            else {
                for (int x = 1; x < X_SPLIT; x++)
                {
                    int startX = gridWidth * x;
                    MoveToEx(hdc, startX, 0, NULL);
                    LineTo(hdc, startX, markHeigth);
                    TextOut(hdc, textStartX, 20, _T("A"), 14);
                    if (OPPOSED) {
                        MoveToEx(hdc, startX, height - markHeigth, NULL);
                        LineTo(hdc, startX, height);
                        TextOut(hdc, textStartX, height - 20 - FONT_SIZE, _T("A"), 14);
                    }
                    textStartX += gridWidth;
                }
                TextOut(hdc, textStartX, 20, _T("A"), 14);
                if (OPPOSED) {
                    TextOut(hdc, textStartX, height - 20 - FONT_SIZE, _T("A"), 14);
                }

                for (int y = 1; y < Y_SPLIT; y++)
                {
                    int startY = gridHeight * y;
                    MoveToEx(hdc, 0, startY, NULL);
                    LineTo(hdc, markWidth, startY);
                    TextOut(hdc, 20, textStartY, _T("1"), 14);
                    if (OPPOSED) {
                        MoveToEx(hdc, width - markWidth, startY, NULL);
                        LineTo(hdc, width, startY);
                        TextOut(hdc, width - 20 - FONT_SIZE, textStartY, _T("1"), 14);
                    }
                    textStartY += gridHeight;
                }
                TextOut(hdc, 20, textStartY, _T("1"), 14);
                if (OPPOSED) {
                    TextOut(hdc, width - 20 - FONT_SIZE, textStartY, _T("1"), 14);
                }
            }

            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            // 清理资源
            DeleteObject(font);

            EndPaint(screenHwnd, &ps);
            return 0;
        }
        default: {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
