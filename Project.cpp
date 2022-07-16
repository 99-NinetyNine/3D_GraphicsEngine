#include "Project.h"

Engine::Engine()
{
	m_sAppName = L"hello";
}

bool Engine::OnUserCreate()
{
	meshCube.tris = {

		// SOUTH
		{ OVec3(0.0f, 0.0f, 0.0f),    OVec3(0.0f, 1.0f, 0.0f),    OVec3(1.0f, 1.0f, 0.0f) },
		{ OVec3(0.0f, 0.0f, 0.0f),    OVec3(1.0f, 1.0f, 0.0f),    OVec3(1.0f, 0.0f, 0.0f) },
		{ OVec3(1.0f, 0.0f, 0.0f),    OVec3(1.0f, 1.0f, 0.0f),    OVec3(1.0f, 1.0f, 1.0f) },
		{ OVec3(1.0f, 0.0f, 0.0f),    OVec3(1.0f, 1.0f, 1.0f),    OVec3(1.0f, 0.0f, 1.0f) },
		{ OVec3(1.0f, 0.0f, 1.0f),    OVec3(1.0f, 1.0f, 1.0f),    OVec3(0.0f, 1.0f, 1.0f) },
		{ OVec3(1.0f, 0.0f, 1.0f),    OVec3(0.0f, 1.0f, 1.0f),    OVec3(0.0f, 0.0f, 1.0f) },
		{ OVec3(0.0f, 0.0f, 1.0f),    OVec3(0.0f, 1.0f, 1.0f),    OVec3(0.0f, 1.0f, 0.0f) },
		{ OVec3(0.0f, 0.0f, 1.0f),    OVec3(0.0f, 1.0f, 0.0f),    OVec3(0.0f, 0.0f, 0.0f) },
		{ OVec3(0.0f, 1.0f, 0.0f),    OVec3(0.0f, 1.0f, 1.0f),    OVec3(1.0f, 1.0f, 1.0f) },
		{ OVec3(0.0f, 1.0f, 0.0f),    OVec3(1.0f, 1.0f, 1.0f),    OVec3(1.0f, 1.0f, 0.0f) },
		{ OVec3(1.0f, 0.0f, 1.0f),    OVec3(0.0f, 0.0f, 1.0f),    OVec3(0.0f, 0.0f, 0.0f) },
		{ OVec3(1.0f, 0.0f, 1.0f),    OVec3(0.0f, 0.0f, 0.0f),    OVec3(1.0f, 0.0f, 0.0f) },

	};

	// Projection Matrix
	float fNear = 2.1f;
	float fFar = 400.0f;
	float fFov = 90.0f;
	float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

	matProj.m_mat[0][0] = fAspectRatio * fFovRad;
	matProj.m_mat[1][1] = fFovRad;
	matProj.m_mat[2][2] = fFar / (fFar - fNear);
	matProj.m_mat[3][2] = (-fFar * fNear) / (fFar - fNear);
	matProj.m_mat[2][3] = 1.0f;
	matProj.m_mat[3][3] = 0.0f;


	return true;
}

bool Engine::OnUserUpdate(float fElapsedTime)
{
	// Clear Screen
	Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, BG_BLACK);

	// Set up rotation matrices
	OMat4 matRotZ, matRotX;
	fTheta += 1.0f * fElapsedTime;

	// Rotation Z
	matRotZ.setIdentity();
	matRotZ.setRotationZ(fTheta);

	// Rotation X
	matRotX.setIdentity();
	matRotX.setRotationX(fTheta);


	// Draw Triangles
	for (auto tri : meshCube.tris)
	{
		triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

		// Rotate in Z-Axis

		triRotatedZ.p[0] = matRotZ * tri.p[0];
		triRotatedZ.p[1] = matRotZ * tri.p[1];
		triRotatedZ.p[2] = matRotZ * tri.p[2];

		// Rotate in X-Axis
		triRotatedZX.p[0] = matRotX * triRotatedZ.p[0];
		triRotatedZX.p[1] = matRotX * triRotatedZ.p[1];
		triRotatedZX.p[2] = matRotX * triRotatedZ.p[2];

		// Offset into the screen
		triTranslated = triRotatedZX;
		triTranslated.p[0].m_z = triRotatedZX.p[0].m_z + 3.0f;
		triTranslated.p[1].m_z = triRotatedZX.p[1].m_z + 3.0f;
		triTranslated.p[2].m_z = triRotatedZX.p[2].m_z + 3.0f;

		// Project triangles from 3D --> 2D
		triProjected.p[0] = matProj * triTranslated.p[0];
		triProjected.p[1] = matProj * triTranslated.p[1];
		triProjected.p[2] = matProj * triTranslated.p[2];

		// Scale into view
		triProjected.p[0].m_x += 1.0f; triProjected.p[0].m_y += 1.0f;
		triProjected.p[1].m_x += 1.0f; triProjected.p[1].m_y += 1.0f;
		triProjected.p[2].m_x += 1.0f; triProjected.p[2].m_y += 1.0f;
		triProjected.p[0].m_x *= 0.5f * (float)ScreenWidth();
		triProjected.p[0].m_y *= 0.5f * (float)ScreenHeight();
		triProjected.p[1].m_x *= 0.5f * (float)ScreenWidth();
		triProjected.p[1].m_y *= 0.5f * (float)ScreenHeight();
		triProjected.p[2].m_x *= 0.5f * (float)ScreenWidth();
		triProjected.p[2].m_y *= 0.5f * (float)ScreenHeight();

		// Rasterize triangle
		DrawTriangle(triProjected.p[0].m_x, triProjected.p[0].m_y,
			triProjected.p[1].m_x, triProjected.p[1].m_y,
			triProjected.p[2].m_x, triProjected.p[2].m_y,
			PIXEL_SOLID, FG_RED);

	}


	return true;

}
