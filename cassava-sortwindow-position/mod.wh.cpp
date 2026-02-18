// ==WindhawkMod==
// @id              cassava-sortwindow-position
// @name            Cassava Editor Sort Window Position
// @description     Cassava Editorのソート画面の座標を調整するやつ
// @version         0.1
// @author          𝐊𝐚𝐞𝐝𝐞
// @github          https://github.com/maplion0069
// @twitter         https://x.com/Maplion0069
// @include         Cassava.exe
// ==/WindhawkMod==

/// Cassava Editorのメインウインドウのハンドル
HWND hMainWindow = nullptr;

// MARK: - CreateWindowExW Hook
using CreateWindowExW_t = decltype(&CreateWindowExW);
CreateWindowExW_t CreateWindowExW_Original;
HWND WINAPI CreateWindowExW_Hook(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
    Wh_Log(L"CreateWindowExW_Hook");
    auto hWnd = CreateWindowExW_Original(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    
    if (lpWindowName && wcscmp(lpWindowName, L"Cassava Editor") == 0 && !hMainWindow) {
        Wh_Log(L"メインウインドウのハンドルは保持しとく");
        hMainWindow = hWnd;
    }

    return hWnd;
}

// MARK: - ShowWindow Hook
using ShowWindow_t = decltype(&ShowWindow);
ShowWindow_t ShowWindow_Original;
BOOL WINAPI ShowWindow_Hook(HWND hWnd, int nCmdShow) {
    Wh_Log(L"ShowWindow_Hook");
    Wh_Log(L"nCmdShow = %d", nCmdShow);

    auto dwLength = GetWindowTextLength(hWnd);
    auto lpszWindowText = new wchar_t[dwLength + 1]();
    GetWindowText(hWnd, lpszWindowText, dwLength + 1);
    if (lpszWindowText && wcscmp(lpszWindowText, L"ソート") == 0 && hMainWindow && nCmdShow == SW_SHOWNORMAL) {
        RECT rcMainWindow = {0};
        GetWindowRect(hMainWindow, &rcMainWindow);
        Wh_Log(L"left => %d, top = %d", rcMainWindow.left, rcMainWindow.top);
        Wh_Log(L"right => %d, bottom = %d", rcMainWindow.right, rcMainWindow.bottom);

        RECT rcSortWindow = {0};
        GetWindowRect(hWnd, &rcSortWindow);
        Wh_Log(L"left => %d, top = %d", rcSortWindow.left, rcSortWindow.top);
        Wh_Log(L"right => %d, bottom = %d", rcSortWindow.right, rcSortWindow.bottom);

        // メインウインドウの左下側に移動
        int dwNewPosX = +8 + rcMainWindow.left;
        int dwNewPosY = -8 + rcMainWindow.bottom - (rcSortWindow.bottom - rcSortWindow.top);
        Wh_Log(L"x => %d, y = %d", dwNewPosX, dwNewPosY);
        SetWindowPos(hWnd, nullptr, dwNewPosX, dwNewPosY, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
    }
    delete[] lpszWindowText;

    return ShowWindow_Original(hWnd, nCmdShow);
}

// The mod is being initialized, load settings, hook functions, and do other
// initialization stuff if required.
BOOL Wh_ModInit() {
    Wh_Log(L"Init");

    Wh_SetFunctionHook(
        (void*)CreateWindowExW,
        (void*)CreateWindowExW_Hook,
        (void**)&CreateWindowExW_Original
    );

    Wh_SetFunctionHook(
        (void*)ShowWindow,
        (void*)ShowWindow_Hook,
        (void**)&ShowWindow_Original
    );

    return TRUE;
}

// The mod is being unloaded, free all allocated resources.
void Wh_ModUninit() {
    Wh_Log(L"Uninit");
}

// The mod setting were changed, reload them.
void Wh_ModSettingsChanged() {
    Wh_Log(L"SettingsChanged");
}
