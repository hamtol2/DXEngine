#include "AppWindow.h"
#include "Engine.h"

AppWindow* app = NULL;

// 윈도우 메시지 프로시저(메시지 처리자).
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
	applicationName = TEXT("Engine20 - Rasterizer State");
	windowStyle = WS_OVERLAPPEDWINDOW;
	app = this;
}

AppWindow::~AppWindow()
{

}

int AppWindow::Run(Engine * engine)
{	
	MSG msg;						// 메시지 변수
	ZeroMemory(&msg, sizeof(MSG));	// 메모리 초기화.

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 타이머 갱신.
			engine->UpdateTimer();
			// 델타 타임 얻기.
			float deltaTime = engine->GetDeltaTime();

			// 델타 타임 전달.
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

	// 창 구조체 값 설정.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;					// 배경.
	wc.lpszClassName = TEXT("DXEngine");		// 창 클래스 이름.
	wc.lpfnWndProc = WindowProcessor;			// 메시지 처리 함수.

	// 클래스 등록.
	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	// 창 핸들 만들기.
	hwnd = CreateWindow(wc.lpszClassName, applicationName,
		windowStyle, 0, 0, screenWidth, screenHeight,
		NULL, NULL, hinstance, NULL
	);

	// 창 핸들 값이 NULL인 경우 실패 리턴.
	if (hwnd == NULL)
	{
		return false;
	}

	// 창 보여주기.
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
		//			TEXT("종료 하시겠습니까?"),
		//			TEXT("종료"), 
		//			MB_YESNO | MB_ICONQUESTION) == IDYES)
		//		{
		//			// 예 버튼 눌리면 창 삭제.
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

		// 키 입력.
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		{
			Keyboard::ProcessMessage(msg, wParam, lParam);
		}
		return 0;

		// 마우스 입력.
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

	// 직접 처리하는 메시지가 아닌 경우에는 윈도우 기본 동작 실행.
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
