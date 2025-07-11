// ==WindhawkMod==
// @id              andapp-next-mod
// @name            AndAppNext Mod
// @description     AndAppクライアントの不満点を改善するMod
// @version         0.1
// @author          𝐊𝐚𝐞𝐝𝐞
// @github          https://github.com/maplion0069
// @twitter         https://x.com/Maplion0069
// @include         AndAppNext.exe
// @compilerOptions -lComctl32
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
* 起動したら最小化させる
* 最小化したらタスクバーから消す
*/
// ==/WindhawkModReadme==

#include <thread>
#include <commctrl.h>
#include <windhawk_utils.h>

using CreateWindowExW_t = decltype(&CreateWindowExW);
CreateWindowExW_t CreateWindowExW_Original;
HWND WINAPI CreateWindowExW_Hook(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
    auto hWnd = CreateWindowExW_Original(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

    constexpr auto lpszAndAppWindowName = L"AndAppNext";
    constexpr auto dwAndAppWindowNameLen = 10; // wcslen(lpszAndAppWindowName);
    if (lpWindowName && wcsncmp(lpWindowName, lpszAndAppWindowName, dwAndAppWindowNameLen) == 0) {
        Wh_Log(L"CreateWindowExW_Hook: lpClassName => %s", (lpClassName != nullptr ? lpClassName : L"(null)"));
        Wh_Log(L"CreateWindowExW_Hook: lpWindowName => %s", (lpWindowName != nullptr ? lpWindowName : L"(null)"));
        Wh_Log(L"CreateWindowExW_Hook: dwStyle => %x", dwStyle);
        Wh_Log(L"CreateWindowExW_Hook: dwExStyle => %x", dwExStyle);

        WindhawkUtils::SetWindowSubclassFromAnyThread(
            hWnd,
            [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, DWORD_PTR dwRefData) WINAPI -> LRESULT {
                // Wh_Log(L"hWnd => %x", hWnd);
                // Wh_Log(L"uMsg => 0x%x", uMsg);

                if (WM_ACTIVATEAPP == uMsg) {
                    Wh_Log(L"uMsg => 0x%x (WM_ACTIVATEAPP)", uMsg);
                    Wh_Log(L"wParam => 0x%x (%s)", wParam, ((bool)wParam ? L"true": L"false"));

                    // タスクバーの表示切り替え
                    auto dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
                    if ((bool)wParam) {
                        dwExStyle ^= WS_EX_TOOLWINDOW;
                    } else {
                        dwExStyle |= WS_EX_TOOLWINDOW;
                    }
                    SetWindowLongPtr(hWnd, GWL_EXSTYLE, dwExStyle);

                    // 最小化アニメーションを少し待ってから隠す
                    if (!(bool)wParam) {
                        std::thread([hWnd]{
                            std::this_thread::sleep_for(std::chrono::milliseconds(250));
                            ShowWindow(hWnd, SW_HIDE);
                        }).detach();
                    }
                }
                return DefSubclassProc(hWnd, uMsg, wParam, lParam);
            },
            NULL
        );
        // TODO: RemoveWindowSubclassFromAnyThreadはWindhawkUtils::detail::SubclassProcWrapper任せで良いものか？

        // ウインドウ生成されるのを確認したいので、すこし待ってから最小化
        std::thread([hWnd]{
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
            SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
        }).detach();
    }

    return hWnd;
}

// The mod is being initialized, load settings, hook functions, and do other
// initialization stuff if required.
BOOL Wh_ModInit() {
    Wh_Log(L"__clang__: %d", __clang__ );
    Wh_Log(L"__clang_major__: %d", __clang_major__  );
    Wh_Log(L"__clang_minor__: %d", __clang_minor__  );
    Wh_Log(L"__clang_patchlevel__: %d", __clang_patchlevel__  );

    Wh_Log(L"Init");

    Wh_SetFunctionHook((void*)CreateWindowExW, (void*)CreateWindowExW_Hook,
                       (void**)&CreateWindowExW_Original);

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
