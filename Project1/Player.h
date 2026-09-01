#pragma once
#include "Object.h"

class Player : public Object
{
public:
	Player();
	virtual ~Player() = default; // = default 추가
	bool IsWall(float x, float y);
private:
	void MoveObject(float x, float y)override; // 실제움직이기

};