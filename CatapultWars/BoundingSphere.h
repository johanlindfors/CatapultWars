#pragma once
#include "BoundingBox.h"

class BoundingSphere
{
public:
	BoundingSphere();
	BoundingSphere(DirectX::XMFLOAT3 center, float radius);
	~BoundingSphere();

	bool Intersects(BoundingBox* box);
};

