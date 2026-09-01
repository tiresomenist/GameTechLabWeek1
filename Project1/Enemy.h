#pragma once
#include "Object.h"

class Enemy : public Object
{
public:
	Enemy();
	virtual ~Enemy() = default; // = default 추가
	void MoveObject(float x, float y)override; // 실제움직이기
private:
	
};