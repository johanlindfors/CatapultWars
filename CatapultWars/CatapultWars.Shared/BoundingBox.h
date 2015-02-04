#pragma once

using namespace DirectX;

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(XMFLOAT3 min, XMFLOAT3 max);
	~BoundingBox();
};

