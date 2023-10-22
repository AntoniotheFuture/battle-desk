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

// values limit
INT MIN_X_SPLIT = 2;
INT MAX_X_SPLIT = 30;
INT MIN_Y_SPLIT = 2;
INT MAX_Y_SPLIT = 30;
INT MIN_FONT_SIZE = 5;
INT MAX_FONT_SIZE = 100;
INT MIN_LINE_WIDTH = 2;
INT MAX_LINE_WIDTH = 100;

BOOL ENABLE = TRUE;
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

HWND helpBtn;

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

// render to all screens
void render() {

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
    HBITMAP screenIcon = (HBITMAP)LoadImageW(NULL, L"src//screen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HBITMAP xSplitIcon = (HBITMAP)LoadImageW(NULL, L"src//x_split.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HBITMAP ySplitIcon = (HBITMAP)LoadImageW(NULL, L"src//y_split.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HBITMAP lineWidthIcon = (HBITMAP)LoadImageW(NULL, L"src//line_width.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HBITMAP opposedIcon = (HBITMAP)LoadImageW(NULL, L"src//opposed.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HBITMAP gridIcon = (HBITMAP)LoadImageW(NULL, L"src//grid.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HBITMAP colorPadIcon = (HBITMAP)LoadImageW(NULL, L"src//color_pad.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HBITMAP fontSizeIcon = (HBITMAP)LoadImageW(NULL, L"src//font_size.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HBITMAP helpIcon = (HBITMAP)LoadImageW(NULL, L"src//help.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

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
    HWND screenSelect = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 60, 16, 250, 400, hWnd, NULL, hInst, NULL);
    for (const std::string& screen : screenList) {
        std::wstring wstr(screen.begin(), screen.end());
        SendMessage(screenSelect, CB_ADDSTRING, 0, (LPARAM)wstr.c_str());
    }
    SendMessage(screenSelect, CB_SETCURSEL, 0, 0);
    HWND enableCheck = CreateWindowW(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 330, 16, 30, 30, hWnd, NULL, hInst, NULL);

    // split select
    HWND xSplitSelect = CreateWindowEx(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 60, 66, 120, 200, hWnd, NULL, hInst, NULL);
    for (int i = MIN_X_SPLIT; i <= MAX_X_SPLIT; i++)
    {
        wchar_t buffer[10];
        _itow_s(i, buffer, 10);
        SendMessage(xSplitSelect, CB_ADDSTRING, 0, (LPARAM)buffer);
    }

    HWND ySplitSelect = CreateWindowEx(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 250, 66, 120, 200, hWnd, NULL, hInst, NULL);
    for (int i = MIN_Y_SPLIT; i <= MAX_Y_SPLIT; i++)
    {
        wchar_t buffer[10];
        _itow_s(i, buffer, 10);
        SendMessage(ySplitSelect, CB_ADDSTRING, 0, (LPARAM)buffer);
    }

    // line width
    lineWidthSlider = CreateWindowEx(0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS, 60, 116, 100, 30, hWnd, NULL, hInst, NULL);
    SendMessage(lineWidthSlider, TBM_SETRANGE, TRUE, MAKELONG(MIN_LINE_WIDTH, MAX_LINE_WIDTH));
    SendMessage(lineWidthSlider, TBM_SETTICFREQ, 2, 0);

    lineWidthText = CreateWindowEx(0, L"STATIC", L"0", WS_CHILD | WS_VISIBLE, 160, 116, 30, 30, hWnd, NULL, hInst, NULL);

    // opposed
    HWND opposedCheck = CreateWindowW(L"Button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 250, 116, 30, 30, hWnd, NULL, hInst, NULL);

    // grid
    HWND gridCheck = CreateWindowW(L"Button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 350, 116, 30, 30, hWnd, NULL, hInst, NULL);


    // colors
    colorPicker = CreateWindow(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 60, 166, 30, 30, hWnd, (HMENU)ID_COLORPICKER, hInst, NULL);
    SetBkColor(GetDC(colorPicker), DRAW_COLOR);
    strokePicker = CreateWindow(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 100, 166, 30, 30, hWnd, (HMENU)ID_STROKECOLORPICKER, hInst, NULL);
    SetBkColor(GetDC(strokePicker), LINE_STROKE_COLOR);

    // font size
    fontSizeSlider = CreateWindowEx(0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS, 250, 166, 100, 30, hWnd, NULL, hInst, NULL);
    SendMessage(fontSizeSlider, TBM_SETRANGE, TRUE, MAKELONG(MIN_FONT_SIZE, MAX_FONT_SIZE));
    SendMessage(fontSizeSlider, TBM_SETTICFREQ, 2, 0);

    fontSizeText = CreateWindowEx(0, L"STATIC", L"0", WS_CHILD | WS_VISIBLE, 350, 166, 30, 30, hWnd, NULL, hInst, NULL);

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
                DestroyWindow(hWnd);
                break;
            case ID_COLORPICKER:
                
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
                    }
                }
                break;
            case ID_STROKECOLORPICKER:

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
                    }
                }
                break;
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
