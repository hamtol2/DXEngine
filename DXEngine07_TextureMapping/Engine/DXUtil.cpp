#include "DXUtil.h"

bool IsError(HRESULT hr, LPCTSTR msg)
{
	// 오류 있으면 오류 메시지 보여주는 메시지 창 띄우기.
	if (hr < 0)
	{
		MessageBox(NULL, msg, TEXT("오류"), MB_OK);
		return true;
	}

	return false;
}