////////////////////////////////////////////////////////////////////////////////
// Filename: frustumclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "frustumclass.h"


FrustumClass::FrustumClass()
{
}


FrustumClass::FrustumClass(const FrustumClass& other)
{
}


FrustumClass::~FrustumClass()
{
}


void FrustumClass::ConstructFrustum(float screenDepth, XMFLOAT4X4 projectionMatrix, XMFLOAT4X4 viewMatrix)
{
	float zMinimum, r;
	XMMATRIX matrix;
	XMFLOAT4X4 matrix4x4;
	XMVECTOR tempVect;

	
	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	XMMATRIX pMatrix((const float*)& projectionMatrix.m);
	XMMATRIX vMatrix((const float*)&viewMatrix.m);
	matrix = XMMatrixMultiply(vMatrix, pMatrix);
	XMStoreFloat4x4(&matrix4x4, matrix);

	// Calculate near plane of frustum.
	m_planes[0].x = matrix4x4._14 + matrix4x4._13;
	m_planes[0].y = matrix4x4._24 + matrix4x4._23;
	m_planes[0].z = matrix4x4._34 + matrix4x4._33;
	m_planes[0].w = matrix4x4._44 + matrix4x4._43;
	tempVect = XMLoadFloat4(&m_planes[0]);
	tempVect = XMPlaneNormalize(tempVect);
	XMStoreFloat4(&m_planes[0], tempVect);

	// Calculate far plane of frustum.
	m_planes[1].x = matrix4x4._14 - matrix4x4._13;
	m_planes[1].y = matrix4x4._24 - matrix4x4._23;
	m_planes[1].z = matrix4x4._34 - matrix4x4._33;
	m_planes[1].w = matrix4x4._44 - matrix4x4._43;
	tempVect = XMLoadFloat4(&m_planes[1]);
	tempVect = XMPlaneNormalize(tempVect);
	XMStoreFloat4(&m_planes[1], tempVect);

	// Calculate left plane of frustum.
	m_planes[2].x = matrix4x4._14 + matrix4x4._11;
	m_planes[2].y = matrix4x4._24 + matrix4x4._21;
	m_planes[2].z = matrix4x4._34 + matrix4x4._31;
	m_planes[2].w = matrix4x4._44 + matrix4x4._41;
	tempVect = XMLoadFloat4(&m_planes[2]);
	tempVect = XMPlaneNormalize(tempVect);
	XMStoreFloat4(&m_planes[2], tempVect);

	// Calculate right plane of frustum.
	m_planes[3].x = matrix4x4._14 - matrix4x4._11;
	m_planes[3].y = matrix4x4._24 - matrix4x4._21;
	m_planes[3].z = matrix4x4._34 - matrix4x4._31;
	m_planes[3].w = matrix4x4._44 - matrix4x4._41;
	tempVect = XMLoadFloat4(&m_planes[3]);
	tempVect = XMPlaneNormalize(tempVect);
	XMStoreFloat4(&m_planes[3], tempVect);

	// Calculate top plane of frustum.
	m_planes[4].x = matrix4x4._14 - matrix4x4._12;
	m_planes[4].y = matrix4x4._24 - matrix4x4._22;
	m_planes[4].z = matrix4x4._34 - matrix4x4._32;
	m_planes[4].w = matrix4x4._44 - matrix4x4._42;
	tempVect = XMLoadFloat4(&m_planes[4]);
	tempVect = XMPlaneNormalize(tempVect);
	XMStoreFloat4(&m_planes[4], tempVect);

	// Calculate bottom plane of frustum.
	m_planes[5].x = matrix4x4._14 + matrix4x4._12;
	m_planes[5].y = matrix4x4._24 + matrix4x4._22;
	m_planes[5].z = matrix4x4._34 + matrix4x4._32;
	m_planes[5].w = matrix4x4._44 + matrix4x4._42;
	tempVect = XMLoadFloat4(&m_planes[5]);
	tempVect = XMPlaneNormalize(tempVect);
	XMStoreFloat4(&m_planes[5], tempVect);

	return;
}


bool FrustumClass::CheckPoint(float x, float y, float z)
{
	int i;
	XMVECTOR planeVector, pointVector, planeDotPoint;
	XMFLOAT3 point(x, y, z);
	XMFLOAT4 res;


	// Check if the point is inside all six planes of the view frustum.
	for(i=0; i<6; i++) 
	{
		planeVector = XMLoadFloat4( &m_planes[i] );
		pointVector = XMLoadFloat3( &point );
		planeDotPoint = XMPlaneDotCoord(planeVector, pointVector);
		XMStoreFloat4( &res, planeDotPoint );

		if( res.x < 0.0f ) // Obojetnie czy .x czy .y czy inne, bo funkcja XMPlateDotCoord wstawia te same wartosci w kazde pole
		{
			return false;
		}
	}

	return true;
}


bool FrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;
	XMVECTOR planeVector, cornerVector, planeDotCorner;
	XMFLOAT3 cubeCorner;
	XMFLOAT4 res;


	// Check if any one point of the cube is in the view frustum.
	for(i=0; i<6; i++) 
	{
		planeVector = XMLoadFloat4(&m_planes[i]);

		cubeCorner = XMFLOAT3(xCenter - radius, yCenter - radius, zCenter - radius);
		cornerVector = XMLoadFloat3(&cubeCorner);
		planeDotCorner = XMPlaneDotCoord(planeVector, cornerVector);
		XMStoreFloat4(&res, planeDotCorner);
		if(res.x >= 0.0f) // Obojetnie czy .x czy .y czy inne, bo funkcja XMPlateDotCoord wstawia te same wartosci w kazde pole
		{
			continue;
		}

		cubeCorner = XMFLOAT3(xCenter + radius, yCenter - radius, zCenter - radius);
		cornerVector = XMLoadFloat3(&cubeCorner);
		planeDotCorner = XMPlaneDotCoord(planeVector, cornerVector);
		XMStoreFloat4(&res, planeDotCorner);
		if (res.x >= 0.0f) // Obojetnie czy .x czy .y czy inne, bo funkcja XMPlateDotCoord wstawia te same wartosci w kazde pole
		{
			continue;
		}

		cubeCorner = XMFLOAT3(xCenter - radius, yCenter + radius, zCenter - radius);
		cornerVector = XMLoadFloat3(&cubeCorner);
		planeDotCorner = XMPlaneDotCoord(planeVector, cornerVector);
		XMStoreFloat4(&res, planeDotCorner);
		if (res.x >= 0.0f) // Obojetnie czy .x czy .y czy inne, bo funkcja XMPlateDotCoord wstawia te same wartosci w kazde pole
		{
			continue;
		}

		cubeCorner = XMFLOAT3(xCenter + radius, yCenter + radius, zCenter - radius);
		cornerVector = XMLoadFloat3(&cubeCorner);
		planeDotCorner = XMPlaneDotCoord(planeVector, cornerVector);
		XMStoreFloat4(&res, planeDotCorner);
		if (res.x >= 0.0f) // Obojetnie czy .x czy .y czy inne, bo funkcja XMPlateDotCoord wstawia te same wartosci w kazde pole
		{
			continue;
		}

		cubeCorner = XMFLOAT3(xCenter - radius, yCenter - radius, zCenter + radius);
		cornerVector = XMLoadFloat3(&cubeCorner);
		planeDotCorner = XMPlaneDotCoord(planeVector, cornerVector);
		XMStoreFloat4(&res, planeDotCorner);
		if (res.x >= 0.0f) // Obojetnie czy .x czy .y czy inne, bo funkcja XMPlateDotCoord wstawia te same wartosci w kazde pole
		{
			continue;
		}

		cubeCorner = XMFLOAT3(xCenter + radius, yCenter - radius, zCenter + radius);
		cornerVector = XMLoadFloat3(&cubeCorner);
		planeDotCorner = XMPlaneDotCoord(planeVector, cornerVector);
		XMStoreFloat4(&res, planeDotCorner);
		if (res.x >= 0.0f) // Obojetnie czy .x czy .y czy inne, bo funkcja XMPlateDotCoord wstawia te same wartosci w kazde pole
		{
			continue;
		}

		cubeCorner = XMFLOAT3(xCenter - radius, yCenter + radius, zCenter + radius);
		cornerVector = XMLoadFloat3(&cubeCorner);
		planeDotCorner = XMPlaneDotCoord(planeVector, cornerVector);
		XMStoreFloat4(&res, planeDotCorner);
		if (res.x >= 0.0f) // Obojetnie czy .x czy .y czy inne, bo funkcja XMPlateDotCoord wstawia te same wartosci w kazde pole
		{
			continue;
		}
		
		cubeCorner = XMFLOAT3(xCenter + radius, yCenter + radius, zCenter + radius);
		cornerVector = XMLoadFloat3(&cubeCorner);
		planeDotCorner = XMPlaneDotCoord(planeVector, cornerVector);
		XMStoreFloat4(&res, planeDotCorner);
		if (res.x >= 0.0f) // Obojetnie czy .x czy .y czy inne, bo funkcja XMPlateDotCoord wstawia te same wartosci w kazde pole
		{
			continue;
		}

		return false;
	}

	return true;
}


bool FrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;
	XMVECTOR planeVector, centerVector, planeDotCenter;
	XMFLOAT3 sphereCenter;
	XMFLOAT4 res;


	// Check if the radius of the sphere is inside the view frustum.
	for(i=0; i<6; i++) 
	{
		planeVector = XMLoadFloat4(&m_planes[i]);
		sphereCenter = XMFLOAT3(xCenter, yCenter, zCenter);
		centerVector = XMLoadFloat3(&sphereCenter);
		planeDotCenter = XMPlaneDotCoord(planeVector, centerVector);
		XMStoreFloat4(&res, planeDotCenter);
		if (res.x < -radius) // Obojetnie czy .x czy .y czy inne, bo funkcja XMPlateDotCoord wstawia te same wartosci w kazde pole
		{
			return false;
		}
	}

	return true;
}


//bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
//{
//	int i;
//
//
//	// Check if any of the 6 planes of the rectangle are inside the view frustum.
//	for(i=0; i<6; i++)
//	{
//		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
//		{
//			continue;
//		}
//
//		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
//		{
//			continue;
//		}
//
//		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
//		{
//			continue;
//		}
//
//		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
//		{
//			continue;
//		}
//
//		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
//		{
//			continue;
//		}
//
//		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
//		{
//			continue;
//		}
//
//		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
//		{
//			continue;
//		}
//
//		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
//		{
//			continue;
//		}
//
//		return false;
//	}
//
//	return true;
//}