#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

IDirect3DDevice9* device = nullptr;

void InitD3D(HWND hWnd) {
    IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (!d3d) {
        // Handle error
        return;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;

    if (FAILED(d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &device))) {
        // Handle error
        return;
    }

    d3d->Release();
}

void RenderFrame() {
    if (device) {
        device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

        device->BeginScene();

        // Draw blue sphere code here

        device->EndScene();
        device->Present(nullptr, nullptr, nullptr, nullptr);
    }
}

void CleanD3D() {
    if (device) {
        device->Release();
    }
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        CleanD3D();
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        RenderFrame();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create a window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("D3D Example"), NULL };
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindow(wc.lpszClassName, _T("Direct3D 9 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, GetDesktopWindow(), NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    InitD3D(hWnd);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            RenderFrame();
        }
    }

    // Clean up Direct3D
    CleanD3D();

    UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}
