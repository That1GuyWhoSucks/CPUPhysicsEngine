#include <windows.h> // >:(

#include "Renderer.h"
#include "main.h"
#include "Constants.h"

HDC g_memDC;
static const RECT CENTER = {SCREEN_WIDTH_HALF, SCREEN_HEIGHT_HALF, SCREEN_WIDTH_HALF, SCREEN_HEIGHT_HALF};

LRESULT CALLBACK WindowProc(HWND hwnd, const UINT uMsg, WPARAM wParam, const LPARAM lParam) {

    switch (uMsg) {
        case WM_SETFOCUS: {
            ClipCursor(&CENTER);
            ShowCursor(0);
            return 0;
        }
        case WM_KILLFOCUS: {
            ClipCursor(NULL);
            ShowCursor(1);
            return 0;
        }
        case WM_INPUT: { // gets mouse stuff
            UINT dwSize = sizeof(RAWINPUT);
            static BYTE lpb[sizeof(RAWINPUT)];

            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
            const RAWINPUT* raw = (RAWINPUT*)lpb;

            if (raw->header.dwType == RIM_TYPEMOUSE) {
                const RAWMOUSE mouse = raw->data.mouse;
                if (mouse.usFlags & MOUSE_MOVE_ABSOLUTE) {

                } else if (mouse.lLastX != 0 || mouse.lLastY != 0) {
                    RendererOnMouse(mouse.lLastX, mouse.lLastY);
                }
            }
            return 0;
        }
        case WM_PAINT: {
            if (working) return 1; // if polled while working don't draw to avoid reading half finished stuff
            working = 1; // currently just bails, could make it wait idk
            PAINTSTRUCT paint;
            HDC hWndDc = BeginPaint(hwnd, &paint);
            BitBlt(hWndDc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                   g_memDC, 0, 0, SRCCOPY);
            working = 0;
            EndPaint(hwnd, &paint);
            return 0;
        } case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        } case WM_KEYDOWN: {
            RendererOnKeyDown(wParam);
            TranslateMessage((MSG*) &wParam);
            return 0;
        } case WM_KEYUP: {
            RendererOnKeyUp(wParam);
            TranslateMessage((MSG*) &wParam);
            return 0;
        } default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, const int nShowCmd) {
    RAWINPUTDEVICE Rid[1]; // to get raw mouse
    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x02;
    Rid[0].dwFlags = RIDEV_NOLEGACY;
    Rid[0].hwndTarget = 0;
    if (!RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]))) {
        return 1;
    }
    static const char CLASS_NAME[] = "Sample Window Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "FUCK YOU WINDOWS",
        WS_OVERLAPPEDWINDOW,
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,

        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (hwnd == NULL) {
        return 0;
    }

    void *pixelMap = NULL;
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = SCREEN_WIDTH;
    bmi.bmiHeader.biHeight = SCREEN_HEIGHT;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    HDC hdc = GetDC(hwnd); // drawing surface
    HBITMAP hbitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pixelMap, NULL, 0); // place for pixels
    g_memDC = CreateCompatibleDC(hdc);
    SelectObject(g_memDC, hbitmap);
    ReleaseDC(hwnd, hdc);

    Init(&hwnd, pixelMap);
    ShowWindow(hwnd, nShowCmd);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (SUICIDE_FLAG) {
            break;
        }
    }
    Destroy();
    return 0;
}