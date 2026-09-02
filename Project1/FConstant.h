#pragma once
#include "FVector.h"

struct FConstant
{
	FVector offset;
	float Padding1;

	FVector scale;
	float Padding2;

	FVector Rotation;
	float Padding3;
};