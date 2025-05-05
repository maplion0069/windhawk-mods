// ==WindhawkMod==
// @id              venus11-startup-position
// @name            Venus11 Startup Position
// @description     びびび立ち上げ時のウインドウ位置を変えるやつ
// @version         0.1
// @author          𝐊𝐚𝐞𝐝𝐞
// @github          https://github.com/maplion0069
// @twitter         https://x.com/Maplion0069
// @homepage        https://www.example.com/
// @include         venus11_andapp.exe
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- attachPosition: right_bottom
  $description: 位置
  $options:
  - left_top: ⇖ 左上
  - left_bottom: ⇙ 左下
  - right_top: ⇗ 右上
  - right_bottom: ⇘ 右下

- attachMargin:
  - horizontal: 20
  - vertical: 20
  $description: 余白

*/
// ==/WindhawkModSettings==

using SetWindowPos_t = decltype(&SetWindowPos);
SetWindowPos_t SetWindowPos_Original;
BOOL WINAPI SetWindowPos_Hook(
    HWND hWnd,
    HWND hWndInsertAfter,
    int  X,
    int  Y,
    int  cx,
    int  cy,
    UINT uFlags
) {
    if (!(uFlags & SWP_NOMOVE)) {
        Wh_Log(L"Position Override");

        PCWSTR attachPosition = Wh_GetStringSetting(L"attachPosition");
        auto marginX = Wh_GetIntSetting(L"attachMargin.horizontal");
        auto marginY = Wh_GetIntSetting(L"attachMargin.vertical");

        if (wcscmp(attachPosition, L"left_top") == 0) {
            X = marginX;
            Y = marginY;
        }
        else if (wcscmp(attachPosition, L"left_bottom") == 0) {
            X = marginX;
            Y = GetSystemMetrics(SM_CYSCREEN) - cy - marginY;
        }
        else if (wcscmp(attachPosition, L"right_top") == 0) {
            X = GetSystemMetrics(SM_CXSCREEN) - cx - marginX;
            Y = marginY;
        }
        else if (wcscmp(attachPosition, L"right_bottom") == 0) {
            X = GetSystemMetrics(SM_CXSCREEN) - cx - marginX;
            Y = GetSystemMetrics(SM_CYSCREEN) - cy - marginY;
        }

        Wh_FreeStringSetting(attachPosition);
    }
    return SetWindowPos_Original(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

void LoadSettings() {
//    settings.xxxxx = Wh_GetIntSetting(L"xxxxx.xxx");
}

// The mod is being initialized, load settings, hook functions, and do other
// initialization stuff if required.
BOOL Wh_ModInit() {
    Wh_Log(L"Init");

    LoadSettings();

    Wh_SetFunctionHook((void*)SetWindowPos,
                       (void*)SetWindowPos_Hook,
                       (void**)&SetWindowPos_Original);

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
