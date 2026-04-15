#include <windows.h>
#include "Renderer.h"
#include "main.h"
#include "Constants.h"

HDC g_memDC;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            RendererRender();
            PAINTSTRUCT paint;
            HDC hWndDc = BeginPaint(hwnd, &paint);
            BitBlt(hWndDc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                   g_memDC, 0, 0, SRCCOPY);
            EndPaint(hwnd, &paint);
            return 0;
        } case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        } case WM_CHAR: {
            RendererOnInput(wParam);
            return 0;
        } default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
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
        CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT,

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
    ShowWindow(hwnd, nCmdShow);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Destroy();
    return 0;
}