#include "framework.h"
#include "Foudre.h"
#include "ProgramInfo.h"
#include "roundbutton.h"
#include <shellapi.h>
#include <strsafe.h>
#include "soundtrackprogressbar.h"

#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100
#define IDM_EXIT 10000

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, ProgramInfo*);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Круглые кнопки
RoundButton bbPlay(1001);
RoundButton bbStop(1002);
RoundButton bbPause(1003);
RoundButton bbPrevious(1004);
RoundButton bbNext(1005);

// Прогрессбар
SoundtrackProgressbar progressbar;

// Структура для отображения уведомлений в области уведомлений
NOTIFYICONDATA notifyIconData;
UINT const WMAPP_NOTIFYCALLBACK = WM_APP + 1;

// Объявление функций
void ProcessWM_PAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void ProcessWM_CREATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void ProcessWM_COMMAND(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void ProcessWM_SIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void ProcessWM_LBUTTONDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void ProcessWM_NOTIFYCALLBACK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void ProcessWM_TIMER(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void CreateNotifyIcon(HWND hWnd, NOTIFYICONDATA* nf);

void CreateNotifyIconContextMenu(HWND hWnd);

/// <summary>
/// Входная точка программы
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    ProgramInfo programinfo;

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FOUDRE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow, &programinfo))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FOUDRE));

    MSG msg;

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

/// <summary>
/// Региструет главный оконный класс
/// </summary>
/// <param name="hInstance"></param>
/// <returns></returns>
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPICON));

    return RegisterClassExW(&wcex);
}

/// <summary>
/// Создает главное окно приложения
/// </summary>
/// <param name="hInstance"></param>
/// <param name="nCmdShow"></param>
/// <param name="programinfo"></param>
/// <returns></returns>
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, ProgramInfo* programinfo)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE,
      programinfo->getX(), programinfo->getY(), programinfo->getWidth(), programinfo->getHeight(), 
       nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

/// <summary>
/// Обрабатывает сообщения главного окна приложения
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            ProcessWM_CREATE(hWnd, message, wParam, lParam);
        }
        break;

        case WM_TIMER:
        {
            ProcessWM_TIMER(hWnd, message, wParam, lParam);
        }
        break;

        case WM_COMMAND:
        {
            ProcessWM_COMMAND(hWnd, message, wParam, lParam);
        }
        break;

        case WM_PAINT:
        {
            ProcessWM_PAINT(hWnd, message, wParam, lParam);
        }
        break;

        case WMAPP_NOTIFYCALLBACK:
        {
            ProcessWM_NOTIFYCALLBACK(hWnd, message, wParam, lParam);
        }
        break;

        case WM_LBUTTONDOWN:
        {
            ProcessWM_LBUTTONDOWN(hWnd, message, wParam, lParam);
        }
        break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;

        case WM_SIZE:
        {
            ProcessWM_SIZE(hWnd, message, wParam, lParam);
        }

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/// <summary>
/// Обрабатывает сообщение WM_PAINT
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
void ProcessWM_PAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc = BeginPaint(hWnd, &ps);
    GetClientRect(hWnd, &rect);

    progressbar.Paint(hdc);

    SetBkMode(hdc, TRANSPARENT);

    static LOGFONT logFontSong;
    logFontSong.lfCharSet = DEFAULT_CHARSET;
    logFontSong.lfPitchAndFamily = DEFAULT_PITCH;
    strcpy_s((char*)logFontSong.lfFaceName, _countof(logFontSong.lfFaceName), "Arial");
    logFontSong.lfHeight = 30;
    logFontSong.lfWidth = 15;
    logFontSong.lfWeight = 20;
    logFontSong.lfEscapement = 0;
    HFONT hSongFont = CreateFontIndirect(&logFontSong);
    RECT textSongRect;
    textSongRect.left = rect.right / 2 - 150;
    textSongRect.top = rect.bottom / 2 - 50;
    textSongRect.right = rect.right / 2 + 150;
    textSongRect.bottom = rect.bottom / 2 + 50;

    SelectObject(hdc, hSongFont);
    DrawText(hdc, L"Крутая песня", -1, &textSongRect, DT_CENTER);
    DeleteObject(hSongFont);


    static LOGFONT logFontSongTime;
    logFontSongTime.lfCharSet = DEFAULT_CHARSET;
    logFontSongTime.lfPitchAndFamily = DEFAULT_PITCH;
    strcpy_s((char*)logFontSongTime.lfFaceName, _countof(logFontSongTime.lfFaceName), "Arial");
    logFontSongTime.lfHeight = 20;
    logFontSongTime.lfWidth = 10;
    logFontSongTime.lfWeight = 10;
    logFontSongTime.lfEscapement = 0;
    HFONT hSongTimeFont = CreateFontIndirect(&logFontSongTime);
    RECT textSongTimeRect;
    textSongTimeRect.left = rect.left + 5;
    textSongTimeRect.top = rect.top + 65;
    textSongTimeRect.right = 200;
    textSongTimeRect.bottom = 150;

    SelectObject(hdc, hSongTimeFont);
    DrawText(hdc, L"00:00:00/00:00:00", -1, &textSongTimeRect, DT_LEFT);
    DeleteObject(hSongTimeFont);

    EndPaint(hWnd, &ps);
}

/// <summary>
/// Обрабатывает сообщение WM_CREATE
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
void ProcessWM_CREATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    bbPlay.LoadButton(MAKEINTRESOURCE(IDB_BBPLAY));
    bbPause.LoadButton(MAKEINTRESOURCE(IDB_BBPAUSE));
    bbStop.LoadButton(MAKEINTRESOURCE(IDB_BBSTOP));
    bbNext.LoadButton(MAKEINTRESOURCE(IDB_BBNEXT));
    bbPrevious.LoadButton(MAKEINTRESOURCE(IDB_BBPREVIOUS));

    RECT rect;
    GetClientRect(hWnd, &rect);
    int y = rect.bottom - bbPlay.GetHeight();

    bbPlay.Create(hWnd, (rect.right * 3 / 10) - bbPlay.GetWidth() / 2, y);
    bbPause.Create(hWnd, (rect.right * 2 / 10) - bbPause.GetWidth() / 2, y);
    bbStop.Create(hWnd, (rect.right * 4 / 10) - bbStop.GetWidth() / 2, y);
    bbNext.Create(hWnd, (rect.right * 5 / 10) - bbNext.GetWidth() / 2, y);
    bbPrevious.Create(hWnd, (rect.right * 1 / 10) - bbPrevious.GetWidth() / 2, y);

    progressbar.LoadCoordinates(0, rect.bottom / 2, rect.right, rect.bottom / 6);
}

/// <summary>
/// Обрабатывает сообщение WM_COMMAND
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
void ProcessWM_COMMAND(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (LOWORD(wParam) == bbPlay.GetMenuValue())
    {
        MessageBox(hWnd, L"Play", L"Play", MB_OK);
        SetTimer(hWnd, 1, 100, NULL);
    }
    else if (LOWORD(wParam) == bbPause.GetMenuValue())
    {
        KillTimer(hWnd, 1);
        MessageBox(hWnd, L"Pause", L"Pause", MB_OK);
    }
    else if (LOWORD(wParam) == bbStop.GetMenuValue())
    {
        KillTimer(hWnd, 1);
        MessageBox(hWnd, L"Stop", L"Stop", MB_OK);
        progressbar.SetProgress(0);
        InvalidateRect(hWnd, NULL, TRUE);
    }
    else if (LOWORD(wParam) == bbNext.GetMenuValue())
    {
        MessageBox(hWnd, L"Next", L"Next", MB_OK);
    }
    else if (LOWORD(wParam) == bbPrevious.GetMenuValue())
    {
        MessageBox(hWnd, L"Previous", L"Previous", MB_OK);
    }
    else if (LOWORD(wParam) == IDM_EXIT)
    {
        DestroyWindow(hWnd);
    }
}

/// <summary>
/// Обрабатывает сообщение WM_SIZE
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
void ProcessWM_SIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (wParam == SIZE_MINIMIZED)
    {    
        CreateNotifyIcon(hWnd, &notifyIconData);
        Shell_NotifyIcon(NIM_ADD, &notifyIconData);
        ShowWindow(hWnd, SW_HIDE);
    }

    if (wParam == SIZE_RESTORED)
    {
        Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
    }
}

/// <summary>
/// Обработка сообщений из области уведомлений
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
void ProcessWM_NOTIFYCALLBACK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(lParam))
    {
        case WM_LBUTTONDOWN:
        {
            ShowWindow(hWnd, SW_SHOW);
            ShowWindow(hWnd, SW_RESTORE);
        }
        break;

        case WM_RBUTTONDOWN:
        {
            CreateNotifyIconContextMenu(hWnd);
        }
        break;
    }
}

/// <summary>
/// Обрабатывает нажатие левой клавиши мыши
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
void ProcessWM_LBUTTONDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    POINT ptMousePos = { LOWORD(lParam), HIWORD(lParam) };
    if (progressbar.CheckPoint(ptMousePos.x, ptMousePos.y))
    {
        progressbar.SetProgress(ptMousePos.x);
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

/// <summary>
/// Обрабатывает сообщения от таймера
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
void ProcessWM_TIMER(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    progressbar.Update();
    InvalidateRect(hWnd, NULL, TRUE);
}

/// <summary>
/// Создает иконку в панели уведомлений
/// </summary>
/// <param name="hWnd"></param>
/// <param name="nf"></param>
void CreateNotifyIcon(HWND hWnd, NOTIFYICONDATA* nf)
{
    nf->hWnd = hWnd;
    nf->uID = NULL;
    nf->uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    StringCchCopy(nf->szTip, ARRAYSIZE(nf->szTip), L"Foudre");
    nf->uCallbackMessage = WMAPP_NOTIFYCALLBACK;
    nf->hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPICON));
}

/// <summary>
/// Создает в панели уведомлений контекстное меню
/// </summary>
/// <param name="hWnd"></param>
void CreateNotifyIconContextMenu(HWND hWnd)
{
    POINT point;
    GetCursorPos(&point);
    HMENU hPopupMenu = CreatePopupMenu();
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, IDM_EXIT, L"Exit");
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, L"");
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, bbPause.GetMenuValue(), L"Pause");
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, bbPlay.GetMenuValue(), L"Play");
    SetForegroundWindow(hWnd);
    TrackPopupMenuEx(hPopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, point.x, point.y, hWnd, NULL);
}