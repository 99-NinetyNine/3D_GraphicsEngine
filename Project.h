#pragma once
#include "IOHandler.h"
#include "MathHandler.h"
#include <vector>

struct triangle
{
	OVec3 p[3];
};

struct mesh
{
	std::vector<triangle> tris;
};

class Engine : public IOHandler
{
public:
	Engine();

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime)override;

private:
	mesh meshCube;
	OMat4 matProj;
	float fTheta;
};

