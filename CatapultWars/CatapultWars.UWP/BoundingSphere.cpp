#include "pch.h"
#include "BoundingSphere.h"


BoundingSphere::BoundingSphere()
{
}

BoundingSphere::BoundingSphere(DirectX::XMFLOAT3 center, float radius)
{

}

BoundingSphere::~BoundingSphere()
{

}

bool BoundingSphere::Intersects(BoundingBox* box)
{
	return false;
}
