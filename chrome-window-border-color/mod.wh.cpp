// ==WindhawkMod==
// @id              chrome-window-border-color
// @name            Chrome Window Border Color
// @description     Chromeの枠線の色を変えるやつ
// @version         0.1
// @author          𝐊𝐚𝐞𝐝𝐞
// @github          https://github.com/maplion0069
// @twitter         https://x.com/Maplion0069
// @homepage        https://www.example.com/
// @include         chrome.exe
// @compilerOptions -ldwmapi
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
*/
// ==/WindhawkModReadme==

#include <dwmapi.h>

using ShowWindow_t = decltype(&ShowWindow);
ShowWindow_t ShowWindow_Original;

BOOL WINAPI ShowWindow_Hook(
  HWND hWnd,
  int  nCmdShow
) {
    Wh_Log(L"ShowWindow");

    auto ret = ShowWindow_Original(
        hWnd,
        nCmdShow
    );

    // COLORREF crBorderColor = RGB(0xDF, 0xE1, 0xE6);
    COLORREF crBorderColor = RGB(0xBF, 0xC1, 0xC6);
    // COLORREF crBorderColor = DWMWA_COLOR_NONE;
    DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &crBorderColor, sizeof(COLORREF));

    return ret;
}


void LoadSettings() {

}

// The mod is being initialized, load settings, hook functions, and do other
// initialization stuff if required.
BOOL Wh_ModInit() {
    Wh_Log(L"Init");

    LoadSettings();

    Wh_SetFunctionHook((void*)ShowWindow,
                       (void*)ShowWindow_Hook,
                       (void**)&ShowWindow_Original);

    return TRUE;
}

// The mod is being unloaded, free all allocated resources.
void Wh_ModUninit() {
    Wh_Log(L"Uninit");
}

// The mod setting were changed, reload them.
void Wh_ModSettingsChanged() {
    Wh_Log(L"SettingsChanged");

    LoadSettings();
}
