#include "AppWindow.h"
#include "Engine.h"

AppWindow* app = NULL;

// ������ �޽��� ���ν���(�޽��� ó����).
LRESULT CALLBACK WindowProcessor(
	HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam
)
{
	if (app != NULL)
		return app->MessageProcessor(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

AppWindow::AppWindow(HINSTANCE hinstance)
{
	hwnd = NULL;
	this->hinstance = hinstance;
	screenWidth = 1600;
	screenHeight = 900;
	applicationName = TEXT("Engine24 - Render To Texture");
	windowStyle = WS_OVERLAPPEDWINDOW;
	app = this;
}

AppWindow::~AppWindow()
{

}

int AppWindow::Run(Engine * engine)
{	
	MSG msg;						// �޽��� ����
	ZeroMemory(&msg, sizeof(MSG));	// �޸� �ʱ�ȭ.

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Ÿ�̸� ����.
			engine->UpdateTimer();
			// ��Ÿ Ÿ�� ���.
			float deltaTime = engine->GetDeltaTime();

			// ��Ÿ Ÿ�� ����.
			engine->ProcessInput(deltaTime);
			engine->Update(deltaTime);
			engine->Render(deltaTime);
		}
	}

	return 0;
}

bool AppWindow::InitializeWindow()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// â ����ü �� ����.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;					// ���.
	wc.lpszClassName = TEXT("DXEngine");		// â Ŭ���� �̸�.
	wc.lpfnWndProc = WindowProcessor;			// �޽��� ó�� �Լ�.

	// Ŭ���� ���.
	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	// â �ڵ� �����.
	hwnd = CreateWindow(wc.lpszClassName, applicationName,
		windowStyle, 0, 0, screenWidth, screenHeight,
		NULL, NULL, hinstance, NULL
	);

	// â �ڵ� ���� NULL�� ��� ���� ����.
	if (hwnd == NULL)
	{
		return false;
	}

	// â �����ֱ�.
	ShowWindow(hwnd, SW_SHOW);

	return true;
}

LRESULT AppWindow::MessageProcessor(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		//case WM_KEYDOWN:
		//{
		//	if (wParam == VK_ESCAPE)
		//	{
		//		if (MessageBox(NULL,
		//			TEXT("���� �Ͻðڽ��ϱ�?"),
		//			TEXT("����"), 
		//			MB_YESNO | MB_ICONQUESTION) == IDYES)
		//		{
		//			// �� ��ư ������ â ����.
		//			DestroyWindow(this->hwnd);
		//		}
		//	}
		//}
		//return 0;

		case WM_ACTIVATEAPP:
		{
			Keyboard::ProcessMessage(msg, wParam, lParam);
			Mouse::ProcessMessage(msg, wParam, lParam);
		}
		return 0;

		// Ű �Է�.
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		{
			Keyboard::ProcessMessage(msg, wParam, lParam);
		}
		return 0;

		// ���콺 �Է�.
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHOVER:
		{
			Mouse::ProcessMessage(msg, wParam, lParam);
		}
		return 0;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		return 0;
	}

	// ���� ó���ϴ� �޽����� �ƴ� ��쿡�� ������ �⺻ ���� ����.
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
