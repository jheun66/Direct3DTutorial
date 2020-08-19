#pragma once

#define KEYMAX 256

class Control
{
private:
	enum
	{
		NONE,
		DOWN,
		UP,
		PRESS
	};

	byte curState[KEYMAX];
	byte oldState[KEYMAX];
	byte mapState[KEYMAX];

	Vector3 mousePos;
	float wheelValue;

	static Control* instance;
	Control();
	~Control();

public:
	static Control* Get() { return instance; }
	static void Create() { instance = new Control; }
	static void Delete() { delete instance; }

	void Update();

	bool Down(UINT key) { return mapState[key] == DOWN; }
	bool Up(UINT key) { return mapState[key] == UP; }
	bool Press(UINT key) { return mapState[key] == PRESS; }

	Vector3 GetMouse() { return mousePos; }
	void SetMouse(LPARAM lParam);

	float GetWheel() { return wheelValue; }
	void SetWheel(float value) { wheelValue = value; }

};