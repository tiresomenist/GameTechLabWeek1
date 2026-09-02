#pragma once

#include "object.h"

class UItem : public Object
{
public:
	UItem();
	virtual ~UItem() = default;

	virtual void MoveObject(float x, float y) final;





};