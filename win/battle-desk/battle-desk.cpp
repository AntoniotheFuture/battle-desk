#include "framework.h"
#include "battle-desk.h"
#include <vector>
#include <iostream>
#include <string>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
std::vector<std::string> screenList;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProcB(HWND, UINT, WPARAM, LPARAM);

void drawBitMap(HDC, int, int);
void setCheckboxes();

// values limit
int MIN_X_SPLIT = 2;
int MAX_X_SPLIT = 25;
int MIN_Y_SPLIT = 2;
int MAX_Y_SPLIT = 25;
int MIN_FONT_SIZE = 5;
int MAX_FONT_SIZE = 100;
int MIN_LINE_WIDTH = 1;
int MAX_LINE_WIDTH = 40;

int SCREEN = 0;
bool ENABLE = TRUE;
COLORREF DRAW_COLOR = RGB(0, 255, 255);
COLORREF LINE_STROKE_COLOR = RGB(250, 250, 250);
BYTE ALPHA = 255;
int X_SPLIT = 6;
int Y_SPLIT = 4;
int LINE_WIDTH = 3;
bool STROKE = TRUE;
int FONT_SIZE = 50;
int TRANSPARENT_PERCENT = 50;

int GRID_MODE = 1;

std::wstring chars = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

HWND mainWin;

HWND lineWidthSlider; 
HWND lineWidthText;
HWND fontSizeSlider;
HWND fontSizeText;

HWND colorPicker;
HWND strokePicker;

HWND transparentSlider;
HWND transparentText;


HWND screenSelect;
HWND enableCheck;
HWND xSplitSelect;
HWND ySplitSelect;

HWND singleCheck;
HWND opposedCheck;
HWND dottedCheck;
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
    if (!ENABLE && screenHwnd != NULL) {
        SendMessage(screenHwnd, WM_CLOSE, 0, 0);
        return;
    }

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
   
    if (IsWindow(screenHwnd) == 0) {
        screenHwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, wct.lpszClassName, _T(""), WS_POPUP, 0, 0, width, height, NULL, NULL, hInst, NULL);
    }
    if (screenHwnd == 0) {
        return;
    }
    SetWindowLong(screenHwnd, GWL_EXSTYLE, GetWindowLong(screenHwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(screenHwnd, RGB(255, 255, 255), TRANSPARENT_PERCENT * 2.55, LWA_ALPHA | LWA_COLORKEY);
    SetWindowPos(screenHwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    ShowWindow(screenHwnd, SW_SHOWDEFAULT);
    InvalidateRect(screenHwnd, NULL, TRUE);

    UpdateWindow(screenHwnd);
}


// render to all screens
void render() {

    renderScreen();
}

void initTray(HWND hWnd) {
    NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(nid);
    nid.hWnd = hWnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    nid.uCallbackMessage = WM_USER;
    lstrcpyn(nid.szTip, TEXT("Battle Desk"), sizeof(nid.szTip));

    Shell_NotifyIcon(NIM_ADD, &nid);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GetScreenList();

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BATTLEDESK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    wct.cbSize = sizeof(WNDCLASSEX);
    wct.style = CS_HREDRAW | CS_VREDRAW;
    wct.lpfnWndProc = WndProcB;
    wct.hInstance = hInstance;
    wct.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wct.lpszClassName = L"TransparentWindow";
    RegisterClassEx(&wct);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BATTLEDESK));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (msg.message == WM_USER)
        {
            if (msg.lParam == WM_LBUTTONUP)
            {
                ShowWindow(mainWin, SW_RESTORE);
                SetForegroundWindow(mainWin);
            }
        }
        else
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    GdiplusShutdown(gdiplusToken);
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

   mainWin = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
      CW_USEDEFAULT, 0, 400, 400, nullptr, nullptr, hInstance, nullptr);

   if (!mainWin)
   {
      return FALSE;
   }
   
   ShowWindow(mainWin, nCmdShow);
   UpdateWindow(mainWin);
   initTray(mainWin);

   return TRUE;
}


void drawForm(HWND hWnd)
{
    // icons
    HBITMAP screenIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP7), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP xSplitIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP8), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP ySplitIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP9), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP lineWidthIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP5), IMAGE_BITMAP, 0, 0, 0);

    HBITMAP singleIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP12), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP opposedIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP6), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP dottedIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP11), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP gridIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP3), IMAGE_BITMAP, 0, 0, 0);

    HBITMAP colorPadIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP transparentIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP10), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP fontSizeIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, 0);
    HBITMAP helpIcon = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 0, 0, 0);



    HWND hImage1 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 10, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage2 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 60, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage3 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 200, 60, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage4 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 110, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage7 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 160, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage10 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 200, 160, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage8 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 200, 110, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage9 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 260, 50, 50, hWnd, NULL, hInst, NULL);

    HWND hImage12 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 210, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage5 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 100, 210, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage11 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 190, 210, 50, 50, hWnd, NULL, hInst, NULL);
    HWND hImage6 = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 280, 210, 50, 50, hWnd, NULL, hInst, NULL);    
    
    SendMessageW(hImage1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)screenIcon);
    SendMessageW(hImage2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)xSplitIcon);
    SendMessageW(hImage3, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)ySplitIcon);
    SendMessageW(hImage4, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)lineWidthIcon);
    SendMessageW(hImage5, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)opposedIcon);
    SendMessageW(hImage6, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)gridIcon);
    SendMessageW(hImage7, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)colorPadIcon);
    SendMessageW(hImage10, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)transparentIcon);
    SendMessageW(hImage8, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fontSizeIcon);
    SendMessageW(hImage9, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)helpIcon);
    SendMessageW(hImage11, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)dottedIcon);
    SendMessageW(hImage12, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)singleIcon);


    // screen select
    screenSelect = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 60, 16, 250, 400, hWnd, (HMENU)ID_SCREEN, hInst, NULL);
    for (const std::string& screen : screenList) {
        std::wstring wstr(screen.begin(), screen.end());
        SendMessage(screenSelect, CB_ADDSTRING, 0, (LPARAM)wstr.c_str());
    }
    SendMessage(screenSelect, CB_SETCURSEL, 0, 0);
    enableCheck = CreateWindowW(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 330, 16, 30, 30, hWnd, (HMENU)ID_ENABLE, hInst, NULL);
    SendMessage(enableCheck, BM_SETCHECK, ENABLE ? BST_CHECKED : BST_UNCHECKED, 0);

    // split select
    xSplitSelect = CreateWindowEx(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 60, 66, 120, 200, hWnd, (HMENU)ID_X_SPLIT, hInst, NULL);
    for (int i = MIN_X_SPLIT; i <= MAX_X_SPLIT; i++)
    {
        wchar_t buffer[10];
        _itow_s(i, buffer, 10);
        SendMessage(xSplitSelect, CB_ADDSTRING, 0, (LPARAM)buffer);
    }
    SendMessage(xSplitSelect, CB_SETCURSEL, X_SPLIT - MIN_X_SPLIT, 0);

    ySplitSelect = CreateWindowEx(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 250, 66, 120, 200, hWnd, (HMENU)ID_Y_SPLIT, hInst, NULL);
    for (int i = MIN_Y_SPLIT; i <= MAX_Y_SPLIT; i++)
    {
        wchar_t buffer[10];
        _itow_s(i, buffer, 10);
        SendMessage(ySplitSelect, CB_ADDSTRING, 0, (LPARAM)buffer);
    }
    SendMessage(ySplitSelect, CB_SETCURSEL, Y_SPLIT - MIN_Y_SPLIT, 0);

    // line width
    lineWidthSlider = CreateWindowEx(0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS, 60, 116, 100, 30, hWnd, (HMENU)ID_LINE_WIDTH, hInst, NULL);
    SendMessage(lineWidthSlider, TBM_SETRANGE, TRUE, MAKELONG(MIN_LINE_WIDTH, MAX_LINE_WIDTH));
    SendMessage(lineWidthSlider, TBM_SETTICFREQ, 1, 0);
    SendMessage(lineWidthSlider, TBM_SETPOS, TRUE, LINE_WIDTH);

    lineWidthText = CreateWindowEx(0, L"STATIC", L"0", WS_CHILD | WS_VISIBLE, 160, 116, 30, 30, hWnd, NULL, hInst, NULL);

    // font size
    fontSizeSlider = CreateWindowEx(0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS, 250, 116, 100, 30, hWnd, NULL, hInst, NULL);
    SendMessage(fontSizeSlider, TBM_SETRANGE, TRUE, MAKELONG(MIN_FONT_SIZE, MAX_FONT_SIZE));
    SendMessage(fontSizeSlider, TBM_SETTICFREQ, 2, 0);
    SendMessage(fontSizeSlider, TBM_SETPOS, TRUE, FONT_SIZE);

    fontSizeText = CreateWindowEx(0, L"STATIC", L"0", WS_CHILD | WS_VISIBLE, 350, 116, 30, 30, hWnd, (HMENU)ID_FONT_SIZE, hInst, NULL);

    // colors
    colorPicker = CreateWindow(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 60, 166, 30, 30, hWnd, (HMENU)ID_COLORPICKER, hInst, NULL);
    SetBkColor(GetDC(colorPicker), DRAW_COLOR);
    strokePicker = CreateWindow(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 100, 166, 30, 30, hWnd, (HMENU)ID_STROKECOLORPICKER, hInst, NULL);
    SetBkColor(GetDC(strokePicker), LINE_STROKE_COLOR);

    // transparent
    transparentSlider = CreateWindowEx(0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS, 250, 166, 100, 30, hWnd, (HMENU)ID_TRANSPARENT, hInst, NULL);
    SendMessage(transparentSlider, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
    SendMessage(transparentSlider, TBM_SETTICFREQ, 5, 0);
    SendMessage(transparentSlider, TBM_SETPOS, TRUE, TRANSPARENT_PERCENT);

    transparentText = CreateWindowEx(0, L"STATIC", L"0\%", WS_CHILD | WS_VISIBLE, 350, 166, 30, 30, hWnd, NULL, hInst, NULL);

    // gridmodes
    singleCheck = CreateWindowW(L"Button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 60, 216, 30, 30, hWnd, (HMENU)ID_SINGLE, hInst, NULL);
    opposedCheck = CreateWindowW(L"Button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 150, 216, 30, 30, hWnd, (HMENU)ID_OPPOESD, hInst, NULL);
    dottedCheck = CreateWindowW(L"Button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 240, 216, 30, 30, hWnd, (HMENU)ID_DOTTED, hInst, NULL);
    gridCheck = CreateWindowW(L"Button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 330, 216, 30, 30, hWnd, (HMENU)ID_GRID, hInst, NULL);
    
    setCheckboxes();

    // help
    HWND staticText = CreateWindowW(L"STATIC", L"https://github.com/AntoniotheFuture/battle-desk", WS_VISIBLE | WS_CHILD | SS_NOTIFY | SS_LEFT | SS_WORDELLIPSIS,
        60, 266, 300, 30, hWnd,
        (HMENU)ID_HELP,
        NULL,
        NULL
    );

    // version
    HWND versionText = CreateWindowW(L"STATIC", L"1.0.0", WS_VISIBLE | WS_CHILD | SS_NOTIFY | SS_LEFT | SS_WORDELLIPSIS,
        10, 316, 300, 30, hWnd,
        NULL,
        NULL,
        NULL
    );

    wchar_t valueText[16];
    swprintf_s(valueText, L"%d", LINE_WIDTH);
    SetWindowText(lineWidthText, valueText);

    wchar_t valueText2[16];
    swprintf_s(valueText2, L"%d", FONT_SIZE);
    SetWindowText(fontSizeText, valueText2);

    wchar_t valueText3[16];
    swprintf_s(valueText3, L"%d%%", TRANSPARENT_PERCENT);
    SetWindowText(transparentText, valueText3);


    HDC hdcButton = GetDC(staticText);  // 获取按钮的设备上下文

    // 设置文本颜色为蓝色
    SetTextColor(hdcButton, RGB(0, 0, 255));  // RGB(0, 0, 255) 表示蓝色

    ReleaseDC(staticText, hdcButton);  // 释放设备上下文

    render();
}

void setCheckboxes() {
    SendMessage(singleCheck, BM_SETCHECK, GRID_MODE == 0 ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(opposedCheck, BM_SETCHECK, GRID_MODE == 1 ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(dottedCheck, BM_SETCHECK, GRID_MODE == 2 ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(gridCheck, BM_SETCHECK, GRID_MODE == 3 ? BST_CHECKED : BST_UNCHECKED, 0);
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
                if (HIWORD(wParam) == CBN_SELCHANGE)
                {
                    X_SPLIT = MIN_X_SPLIT + SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
                    render();
                }
                break;
            }
            case ID_Y_SPLIT:
            {
                if (HIWORD(wParam) == CBN_SELCHANGE)
                {
                    Y_SPLIT = MIN_Y_SPLIT + SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
                    render();
                }
                break;
            }
            case ID_SINGLE:
            {
                int checkBoxState = SendMessage(singleCheck, BM_GETCHECK, 0, 0);
                if (checkBoxState == BST_CHECKED) {
                    GRID_MODE = 0;
                    setCheckboxes();
                    render();
                }
                break;
            }
            case ID_OPPOESD:
            {
                int checkBoxState = SendMessage(opposedCheck, BM_GETCHECK, 0, 0);
                if (checkBoxState == BST_CHECKED) {
                    GRID_MODE = 1;
                    setCheckboxes();
                    render();
                }
                break;
            }
            case ID_DOTTED:
            {
                int checkBoxState = SendMessage(dottedCheck, BM_GETCHECK, 0, 0);
                if (checkBoxState == BST_CHECKED) {
                    GRID_MODE = 2;
                    setCheckboxes();
                    render();
                }
                break;
            }
            case ID_GRID:
            {
                int checkBoxState = SendMessage(gridCheck, BM_GETCHECK, 0, 0);
                if (checkBoxState == BST_CHECKED) {
                    GRID_MODE = 3;
                    setCheckboxes();
                    render();
                }
                break;
            }
            case ID_COLORPICKER:
            {
                if (HIWORD(wParam) == BN_CLICKED) {
                    CHOOSECOLOR cc;
                    COLORREF colorCustom[16] = { 0 };

                    memset(&cc, 0, sizeof(cc));
                    cc.lStructSize = sizeof(cc);
                    cc.hwndOwner = hWnd;
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
            case ID_TRANSPARENT:
            {

            }
            case ID_STROKECOLORPICKER:
            {
                if (HIWORD(wParam) == BN_CLICKED) {
                    CHOOSECOLOR cc;
                    COLORREF colorCustom[16] = { 0 };

                    memset(&cc, 0, sizeof(cc));
                    cc.lStructSize = sizeof(cc);
                    cc.hwndOwner = hWnd;
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
        SetBkColor(hDC, DRAW_COLOR);
        return (LRESULT)CreateSolidBrush(DRAW_COLOR);
    }
    case WM_HSCROLL:
        {
            LINE_WIDTH = SendMessage(lineWidthSlider, TBM_GETPOS, 0, 0);
            wchar_t valueText[16];
            swprintf_s(valueText, L"%d", LINE_WIDTH);
            SetWindowText(lineWidthText, valueText);

            FONT_SIZE = SendMessage(fontSizeSlider, TBM_GETPOS, 0, 0);
            wchar_t valueText2[16];
            swprintf_s(valueText2, L"%d", FONT_SIZE);
            SetWindowText(fontSizeText, valueText2);

            TRANSPARENT_PERCENT = SendMessage(transparentSlider, TBM_GETPOS, 0, 0);
            wchar_t valueText3[16];
            swprintf_s(valueText3, L"%d%%", TRANSPARENT_PERCENT);
            SetWindowText(transparentText, valueText3);
            render();
        }
        break;
    case WM_CLOSE:
    {
        ShowWindow(hWnd, SW_HIDE);
        break;
    }
        
    //case WM_DESTROY:
    //    PostQuitMessage(0);
    //    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void DrawTransparentText(HDC hdc, const WCHAR* text, int x, int y, int alpha) {
    Gdiplus::Graphics graphics(hdc);
    Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, 0, 0, 0));
    Gdiplus::Font font(L"Arial", 50);
    graphics.DrawString(text, -1, &font, Gdiplus::PointF(x, y), &brush);
}


wchar_t* chatToWchar(const char* ch)
{
    wchar_t* wchar;
    int len = MultiByteToWideChar(CP_ACP, 0, ch, strlen(ch), NULL, 0);
    wchar = new wchar_t[len + 1];
    MultiByteToWideChar(CP_ACP, 0, ch, strlen(ch), wchar, len);
    wchar[len] = '\0';
    return wchar;
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

            RECT rect;
            GetClientRect(hWnd, &rect);
            MapWindowPoints(hWnd, GetParent(hWnd), (LPPOINT)&rect, 2);

            int width = GetSystemMetrics(SM_CXSCREEN);
            int height = GetSystemMetrics(SM_CYSCREEN);
            
            int markWidth = width * 0.05;
            int markHeigth = height * 0.05;

            int gridWidth = width / X_SPLIT;
            int gridHeight = height / Y_SPLIT;

            int textStartX = (gridWidth + FONT_SIZE) / 2;
            int textStartY = (gridHeight + FONT_SIZE) / 2;

            int strokeWidth = 2;

            Graphics graphics(hdc);

            Bitmap bitmap(width, height, PixelFormat32bppARGB);
            BYTE r = GetRValue(DRAW_COLOR);
            BYTE g = GetGValue(DRAW_COLOR);
            BYTE b = GetBValue(DRAW_COLOR);

            BYTE sr = GetRValue(LINE_STROKE_COLOR);
            BYTE sg = GetGValue(LINE_STROKE_COLOR);
            BYTE sb = GetBValue(LINE_STROKE_COLOR);

            Color mainColor(255, r, g, b);
            Color strokeColor(255, sr, sg, sb);

            Graphics bitmapGraphics(&bitmap);
            Pen pen(mainColor, LINE_WIDTH);
            pen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
            bitmapGraphics.SetSmoothingMode(SmoothingModeAntiAlias);


            FontFamily fontFamily(L"Arial");  // 字体
            Gdiplus::Font font(L"Arial", FONT_SIZE, FontStyleRegular, UnitPixel);
            Gdiplus::SolidBrush brush(mainColor);
            Gdiplus::SolidBrush strokeBrush(Color(sr, sg, sb));
            Gdiplus::StringFormat format;
            format.SetAlignment(Gdiplus::StringAlignmentCenter);
            format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

            Pen borderPen(strokeColor, strokeWidth);
            SolidBrush textBrush(mainColor);

            Gdiplus::RectF textRect(500, 500, width, height);

            GraphicsPath path;

            std::wstring text = L"Hello, GDI+";
            for (int x = 1; x <= X_SPLIT; x++)
            {
                wchar_t str = chars[x - 1];
                textRect = RectF((x - 1) * gridWidth, 0, gridWidth, markHeigth);
                path.AddString(&str, 1, &fontFamily, FontStyleRegular, FONT_SIZE + 2, textRect, &format);
                bitmapGraphics.DrawString(&str, 1, &font, textRect, &format, &brush);
                int startX = gridWidth * x;
                if (GRID_MODE > 0) {
                    textRect = RectF((x - 1) * gridWidth, height - markHeigth, gridWidth, markHeigth);
                    path.AddString(&str, 1, &fontFamily, FontStyleRegular, FONT_SIZE + 2, textRect, &format);
                    bitmapGraphics.DrawString(&str, 1, &font, textRect, &format, &brush);
                }
            }

            for (int y = 1; y <= Y_SPLIT; y++)
            {
                WCHAR intStr[10];
                swprintf(intStr, 10, L"%d", y);
                textRect = RectF(0, (y - 1) * gridHeight, markWidth, gridHeight);
                path.AddString(intStr, 1, &fontFamily, FontStyleRegular, FONT_SIZE + 2, textRect, &format);
                bitmapGraphics.DrawString(intStr, 1, &font, textRect, &format, &brush);
                if (GRID_MODE > 0) {
                    textRect = RectF(width - markWidth, (y - 1) * gridHeight, markWidth, gridHeight);
                    path.AddString(intStr, 1, &fontFamily, FontStyleRegular, FONT_SIZE + 2, textRect, &format);
                    bitmapGraphics.DrawString(intStr, 1, &font, textRect, &format, &brush);
                }
            }

            graphics.DrawPath(&borderPen, &path);

            Gdiplus::RectF strokeRect(500, 500, width, height);
            Gdiplus::RectF fillRect(500, 500, width, height);
            if (GRID_MODE == 3) {
                for (int x = 1; x < X_SPLIT; x++)
                {
                    int startX = gridWidth * x;
                    int rx = startX - LINE_WIDTH / 2;
                    int ry = 0;
                    int rw = LINE_WIDTH;
                    int rh = height;
                    strokeRect = RectF(rx - strokeWidth, ry - strokeWidth, rw + strokeWidth * 2, rh + strokeWidth * 2);
                    fillRect = RectF(rx, ry, rw, rh);
                    bitmapGraphics.FillRectangle(&strokeBrush, strokeRect);
                    bitmapGraphics.FillRectangle(&brush, fillRect);       
                }

                for (int y = 1; y < Y_SPLIT; y++)
                {
                    int startY = gridHeight * y;
                    int rx = 0;
                    int ry = startY - LINE_WIDTH / 2;
                    int rw = width;
                    int rh = LINE_WIDTH;
                    strokeRect = RectF(rx - strokeWidth, ry - strokeWidth, rw + strokeWidth * 2, rh + strokeWidth * 2);
                    fillRect = RectF(rx, ry, rw, rh);
                    bitmapGraphics.FillRectangle(&strokeBrush, strokeRect);
                    bitmapGraphics.FillRectangle(&brush, fillRect);
                }
            }
            else {
                for (int x = 1; x < X_SPLIT; x++)
                {
                    int startX = gridWidth * x;
                    int rx = startX - LINE_WIDTH / 2;
                    int ry = 0;
                    int rw = LINE_WIDTH;
                    int rh = markHeigth;
                    strokeRect = RectF(rx - strokeWidth, ry - strokeWidth, rw + strokeWidth * 2, rh + strokeWidth * 2);
                    fillRect = RectF(rx, ry, rw, rh);
                    bitmapGraphics.FillRectangle(&strokeBrush, strokeRect);
                    bitmapGraphics.FillRectangle(&brush, fillRect);

                    if (GRID_MODE > 0) {
                        ry = height - markHeigth;
                        strokeRect = RectF(rx - strokeWidth, ry - strokeWidth, rw + strokeWidth * 2, rh + strokeWidth * 2);
                        fillRect = RectF(rx, ry, rw, rh);
                        bitmapGraphics.FillRectangle(&strokeBrush, strokeRect);
                        bitmapGraphics.FillRectangle(&brush, fillRect);
                    }
                }
                for (int y = 1; y < Y_SPLIT; y++)
                {
                    int startY = gridHeight * y;
                    int rx = 0;
                    int ry = startY - LINE_WIDTH / 2;
                    int rw = markWidth;
                    int rh = LINE_WIDTH;
                    strokeRect = RectF(rx - strokeWidth, ry - strokeWidth, rw + strokeWidth * 2, rh + strokeWidth * 2);
                    fillRect = RectF(rx, ry, rw, rh);
                    bitmapGraphics.FillRectangle(&strokeBrush, strokeRect);
                    bitmapGraphics.FillRectangle(&brush, fillRect);
                    if (GRID_MODE > 0) {
                        rx = width - markWidth;
                        strokeRect = RectF(rx - strokeWidth, ry - strokeWidth, rw + strokeWidth * 2, rh + strokeWidth * 2);
                        fillRect = RectF(rx, ry, rw, rh);
                        bitmapGraphics.FillRectangle(&strokeBrush, strokeRect);
                        bitmapGraphics.FillRectangle(&brush, fillRect);
                    }
                }
            }
            if (GRID_MODE == 2) {
                int crossLength = 30;
                for (int x = 1; x < X_SPLIT; x++)
                {
                    for (int y = 1; y < Y_SPLIT; y++)
                    {
                        int startX = gridWidth * x;
                        int startY = gridHeight * y;
                        int rx = startX - LINE_WIDTH / 2;
                        int ry = startY - crossLength / 2;
                        int rw = LINE_WIDTH;
                        int rh = crossLength;
                        strokeRect = RectF(rx - strokeWidth, ry - strokeWidth, rw + strokeWidth * 2, rh + strokeWidth * 2);
                        fillRect = RectF(rx, ry, rw, rh);
                        bitmapGraphics.FillRectangle(&strokeBrush, strokeRect);
                        bitmapGraphics.FillRectangle(&brush, fillRect);
                        rx = startX - crossLength / 2;
                        ry = startY - LINE_WIDTH / 2;
                        rw = crossLength;
                        rh = LINE_WIDTH;
                        strokeRect = RectF(rx - strokeWidth, ry - strokeWidth, rw + strokeWidth * 2, rh + strokeWidth * 2);
                        fillRect = RectF(rx, ry, rw, rh);
                        bitmapGraphics.FillRectangle(&strokeBrush, strokeRect);
                        bitmapGraphics.FillRectangle(&brush, fillRect);
                    }
                    
                }
            }
            graphics.DrawImage(&bitmap, 0, 0);
            EndPaint(screenHwnd, &ps);
            return 0;
        }
        default: {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
