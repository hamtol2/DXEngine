#include "DXUtil.h"

std::wstring ConcatString(LPCTSTR & target, LPCTSTR source)
{
	std::wstring concatString(target);
	concatString = concatString + source;
	return concatString;
}

bool IsError(HRESULT hr, LPCTSTR msg)
{
	// ���� ������ ���� �޽��� �����ִ� �޽��� â ����.
	if (hr < 0)
	{
		MessageBox(NULL, msg, TEXT("����"), MB_OK);
		return true;
	}

	return false;
}