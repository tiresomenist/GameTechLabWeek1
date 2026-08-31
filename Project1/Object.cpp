#include <vector>
#include "Object.h"


struct FVector {
	float x, y, z;
};

class Object {



private: 

	FVector Location;

	void MoveObject(float x, float y) {}

};

class Enemy : public Object {

	FVector Location;

	void MoveObject(float x, float y) {
		Location.x += x;
		Location.y += y;
	}

};

class Player : public Object {

	FVector Location;

	bool IsWall(float x, float y) {

		return (Location.x+x >= -1 && Location.x+x <= 1 && Location.y+y >= -1 && Location.y+y <= 1);

	}

	void MoveObject(float x, float y) {
		if (IsWall(x, y)) {
			// 벽 충돌 처리 함수
		} else {
			Location.x += x;
			Location.y += y;
		}
	}

};