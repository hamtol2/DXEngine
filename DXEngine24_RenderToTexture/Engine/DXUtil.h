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

	// �迭 �޸� ������.
	template <class T> void SafeDeleteArray(T& t)
	{
		if (t)
		{
			delete[] t;
			t = NULL;
		}
	}
}

// C4316 ��� �ذ��.
// 16����Ʈ ������ ���� ������ �����ε� Ŭ����.
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

// ���ڿ� �����ִ� �Լ�.
std::wstring ConcatString(LPCTSTR& target, LPCTSTR source);

// ���� Ȯ�ο� �Լ�.
bool IsError(HRESULT hr, LPCTSTR msg);