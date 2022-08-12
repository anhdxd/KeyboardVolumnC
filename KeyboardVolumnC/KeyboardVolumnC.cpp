#include <windows.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include "KeyboardVolumnC.h"
#include <strsafe.h>
#include <commctrl.h>
#define NUMHOOKS 1

HWND gh_hwndMain;
LRESULT WINAPI MainWndProc(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

    HRESULT hr;
    float pfLevel = 0;
    IAudioEndpointVolume* audio = NULL;
    IMMDevice* defaultDevice = NULL;
    IMMDeviceEnumerator* deviceEnumerator = NULL;
    BOOL bMute;
    HKEY hkey;
    WCHAR AutoStartPath[260];
    RegisterHotKey(0, 1, MOD_ALT, 0x31);
    RegisterHotKey(0, 1, MOD_ALT , 0x32);
    RegisterHotKey(0, 1, MOD_ALT , 0x33);

    // Auto Start Windows
    GetModuleFileNameW(0, AutoStartPath, MAX_PATH); 
    RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey);
    int i = RegSetValueExW(hkey, L"VolumnKeyboardAnhdz", NULL, REG_SZ, (const BYTE*)AutoStartPath, lstrlen(AutoStartPath) * sizeof(WCHAR) + 1);
    RegCloseKey(hkey);

    // Init
    hr = CoInitialize(NULL);
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);

    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&audio);
    defaultDevice->Release();
    defaultDevice = NULL;



    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            audio->GetMasterVolumeLevelScalar(&pfLevel);
            if (HIWORD(msg.lParam) == 0x33)
            {
                audio->SetMute(0, 0);
                audio->VolumeStepUp(0);
            }
            if (HIWORD(msg.lParam) == 0x32)
            {
                audio->SetMute(0, 0);
                audio->VolumeStepDown(0);
            }
            if (HIWORD(msg.lParam) == 0x31)
            {
                audio->GetMute(&bMute);
                if(bMute)
                    audio->SetMute(0, 0);
                else
                    audio->SetMute(1, 0);
            }
               
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
LRESULT WINAPI MainWndProc(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hwndMain, uMsg, wParam, lParam);

}
