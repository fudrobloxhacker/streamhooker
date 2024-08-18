#include <Windows.h>
#include <Wininet.h>
#include "MinHook.h"
#include <stdio.h>
#pragma comment(lib, "Wininet.lib")

using InternetOpenUrlA_t = HINTERNET(*)(HINTERNET, LPCSTR, LPCSTR, DWORD, DWORD, DWORD_PTR);
InternetOpenUrlA_t origin = nullptr;

HINTERNET hook(
    HINTERNET hInternet,
    LPCSTR lpszUrl,
    LPCSTR lpszHeaders,
    DWORD dwHeadersLength,
    DWORD dwFlags,
    DWORD_PTR dwContext
) {
    printf("\ncalled\n");
    printf("hInternet -> %p\n", hInternet);
    printf("lpszUrl (Image Link) -> %s\n\n", lpszUrl);

    return origin(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        if (MH_Initialize() != MH_OK) {
            printf("unable to init\n");
            return false;
        }

        HMODULE wininetbase = GetModuleHandleA("Wininet.dll");
        LPVOID openurladdy = GetProcAddress(wininetbase, "InternetOpenUrlA");

        printf("wininet base: %p\n", wininetbase);
        printf("openurl addy: %p\n", openurladdy);

        if (MH_CreateHook(openurladdy, hook, reinterpret_cast<LPVOID*>(&origin)) != MH_OK) {
            printf("unable to create hook\n");
            return false;
        }

        if (MH_EnableHook(openurladdy) != MH_OK) {
            printf("unable to enable hook\n");
            return false;
        }
    }

    return true;
}