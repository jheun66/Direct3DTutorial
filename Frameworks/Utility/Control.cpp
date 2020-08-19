#include "Framework.h"

Control* Control::instance = nullptr;

Control::Control()
	:curState{}, oldState{}, mapState{}, mousePos(0,0,0),
	wheelValue(0)
{


}

Control::~Control()
{
}

void Control::Update()
{
	memcpy(oldState, curState, sizeof(oldState));

	ZeroMemory(curState, sizeof(curState));
	ZeroMemory(mapState, sizeof(mapState));

	// 0, 1, 128, 129 반환 : 토글 형식
	GetKeyboardState(curState);
	
	for (UINT i = 0; i < KEYMAX; i++)
	{
		// toggle 상관없이 구하기 위해 (1 제거하고 생각하기)
		byte key = curState[i] & 0x80;
		curState[i] = key ? 1 : 0;

		int old = oldState[i];
		int cur = curState[i];

		if (old == 0 && cur == 1)
			mapState[i] = DOWN;
		else if (old == 1 && cur == 0)
			mapState[i] = UP;
		else if (old == 1 && cur == 1)
			mapState[i] = PRESS;
		else
			mapState[i] = NONE;

	}

}

void Control::SetMouse(LPARAM lParam)
{
	float x= static_cast<float>(LOWORD(lParam));
	float y= static_cast<float>(HIWORD(lParam));
	if (x <= WIN_WIDTH)
		mousePos.x = x;
	if (y <= WIN_HEIGHT)
		mousePos.y = y;
}

