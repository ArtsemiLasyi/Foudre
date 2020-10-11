#include "framework.h"
#include "Foudre.h"
#include "ProgramInfo.h"
#include "roundbutton.h"

#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100

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
RoundButton bbRewindBack(1006);
RoundButton bbRewindForward(1007);

// Объявление функций
void ProcessWM_PAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void ProcessWM_CREATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void ProcessWM_COMMAND(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/// <summary>
/// 
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
/// 
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
    //wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FOUDRE);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPICON));

    return RegisterClassExW(&wcex);
}

/// <summary>
/// 
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
/// 
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

        case WM_COMMAND:
        {
            ProcessWM_COMMAND(hWnd, message, wParam, lParam);


            int wmId = LOWORD(wParam);

            switch (wmId)
            {
                case IDM_ABOUT:
                {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                }
                break;

                case IDM_EXIT:
                {
                    DestroyWindow(hWnd);
                }
                break;

                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

        case WM_PAINT:
        {
            ProcessWM_PAINT(hWnd, message, wParam, lParam);
        }
        break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/// <summary>
/// 
/// </summary>
/// <param name="hDlg"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

/// <summary>
/// 
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
    
    LOGBRUSH lb;
    lb.lbStyle = BS_SOLID;
    lb.lbColor = RGB(135, 115, 238);
    lb.lbHatch = HS_BDIAGONAL;
    HBRUSH brush = CreateBrushIndirect(&lb);
    FillRect(hdc, &rect, brush);
    /*
    HPEN hPen = CreatePen(PS_DOT, 5, RGB(0, 0, 0));
    SelectObject(hdc, hPen);
    SetBkColor(hdc, RGB(135, 115, 238));
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    */
    EndPaint(hWnd, &ps);
}

/// <summary>
/// 
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
    bbRewindBack.LoadButton(MAKEINTRESOURCE(IDB_BBREWINDBACK));
    bbRewindForward.LoadButton(MAKEINTRESOURCE(IDB_BBREWINDFORWARD));

    RECT rect;
    GetClientRect(hWnd, &rect);
    int y = rect.bottom - bbPlay.GetHeight();

    bbPlay.Create(hWnd, (rect.right * 4 / 12) - bbPlay.GetWidth() / 2, y);
    bbPause.Create(hWnd, (rect.right * 3 / 12) - bbPause.GetWidth() / 2, y);
    bbStop.Create(hWnd, (rect.right * 5 / 12) - bbStop.GetWidth() / 2, y);
    bbNext.Create(hWnd, (rect.right * 6 / 12) - bbNext.GetWidth() / 2, y);
    bbPrevious.Create(hWnd, (rect.right * 2 / 12) - bbPrevious.GetWidth() / 2, y);
    bbRewindBack.Create(hWnd, (rect.right * 1 / 12) - bbRewindBack.GetWidth() / 2, y);
    bbRewindForward.Create(hWnd, (rect.right * 7 / 12) - bbRewindForward.GetWidth() / 2, y);
}

/// <summary>
/// 
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
    }
    else if (LOWORD(wParam) == bbPause.GetMenuValue())
    {
        MessageBox(hWnd, L"Pause", L"Pause", MB_OK);
    }
    else if (LOWORD(wParam) == bbStop.GetMenuValue())
    {
        MessageBox(hWnd, L"Stop", L"Stop", MB_OK);
    }
    else if (LOWORD(wParam) == bbNext.GetMenuValue())
    {
        MessageBox(hWnd, L"Next", L"Next", MB_OK);
    }
    else if (LOWORD(wParam) == bbPrevious.GetMenuValue())
    {
        MessageBox(hWnd, L"Previous", L"Previous", MB_OK);
    }
    else if (LOWORD(wParam) == bbRewindBack.GetMenuValue())
    {
        MessageBox(hWnd, L"RewindBack", L"RewindBack", MB_OK);
    }
    else if (LOWORD(wParam) == bbRewindForward.GetMenuValue())
    {
        MessageBox(hWnd, L"RewindForward", L"RewindForward", MB_OK);
    }
}