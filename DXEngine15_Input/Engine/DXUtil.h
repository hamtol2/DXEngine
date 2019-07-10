#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace Memory
{
	template <class T> void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = NULL;
		}
	}

	template <class T> void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = NULL;
		}
	}

	// 배열 메모리 해제용.
	template <class T> void SafeDeleteArray(T& t)
	{
		if (t)
		{
			delete[] t;
			t = NULL;
		}
	}
}

// C4316 경고 해결용.
// 16바이트 정렬을 위한 연산자 오버로딩 클래스.
template<size_t T>
class AlignedAllocation
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, T);
	}

	static void operator delete(void* pointer)
	{
		_aligned_free(pointer);
	}
};

// 문자열 더해주는 함수.
std::wstring ConcatString(LPCTSTR& target, LPCTSTR source);

// 오류 확인용 함수.
bool IsError(HRESULT hr, LPCTSTR msg);