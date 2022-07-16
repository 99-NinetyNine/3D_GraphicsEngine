#include "Project.h"

#include <algorithm>
#include <fstream>
#include <strstream>

OVec3 CalcDirLight(Engine::DirLight light, OVec3 normal, OVec3 viewDir);
OVec3 CalcPointLight(Engine::PointLight light, OVec3 normal, OVec3 fragPos, OVec3 viewDir);
OVec3 CalcSpotLight(Engine::SpotLight light, OVec3 normal, OVec3 fragPos, OVec3 viewDir);


Engine::Engine()
{
	m_sAppName = L"hello";
}

bool Engine::OnUserCreate()
{
	
	pointLightPositions[0] = OVec3(2.0f, 3.0f, -4.0f);
	pointLightPositions[1]=OVec3(2.0f,3.0f,-4.0f),
	pointLightPositions[2]=OVec3(-2.0f,3.0f,4.0f),
	pointLightPositions[3]=OVec3(2.0f,-3.0f,4.0f),

	meshRocket.LoadFromObjectFile("rocket.obj");
	meshAxes.LoadFromObjectFile("model.txt");




	// Projection Matrix
	float fNear = 0.1f;
	float fFar = 500.0f;
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
	if (GetKey(VK_UP).bHeld)
		vCamera.m_y += 8.0f * fElapsedTime;	// Travel Upwards

	if (GetKey(VK_DOWN).bHeld)
		vCamera.m_y -= 8.0f * fElapsedTime;	// Travel Downwards


	// Dont use these two in FPS mode, it is confusing :P
	if (GetKey(VK_LEFT).bHeld)
		vCamera.m_x -= 8.0f * fElapsedTime;	// Travel Along X-Axis

	if (GetKey(VK_RIGHT).bHeld)
		vCamera.m_x += 8.0f * fElapsedTime;	// Travel Along X-Axis
	///////


	OVec3 vForward = vLookDir * 8.0f * fElapsedTime;

	// Standard FPS Control scheme, but turn instead of strafe
	if (GetKey(L'W').bHeld)
		vCamera = vCamera + vForward;

	if (GetKey(L'S').bHeld)
		vCamera = vCamera - vForward;

	if (GetKey(L'A').bHeld)
		fYaw -= 2.0f * fElapsedTime;

	if (GetKey(L'D').bHeld)
		fYaw += 2.0f * fElapsedTime;




	// Clear Screen
	Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, BG_CYAN);

	// Create "Point At" Matrix for camera
	OVec3 vUp(0, 1, 0);
	OVec3 vTarget(0, 0, 1);
	OMat4 matCameraRot;
	matCameraRot.setRotationY(fYaw);
	vLookDir = matCameraRot * vTarget;
	vTarget = vCamera + vLookDir;
	viewPos = vTarget;
	OMat4 matCamera;
	matCamera.LookAtMatrix(vCamera, vTarget, vUp);

	// Make view matrix from camera
	matCamera.inverse();
	matView = matCamera;


	setTRS(OVec3(0.0f, 0.0f, 5.0f), OVec3(fTheta*2.0f, fTheta , fTheta), OVec3(1));
	drawModel(meshRocket);

	setTRS(OVec3(25.0f, -20.0f, 20.0f), OVec3(0, fTheta * 0.5f, fTheta), OVec3(0.3));
	drawModel(meshAxes);


	return true;
}

void Engine::drawModel(mesh& model)
{
	// Draw Triangles
	vector<triangle> vecTrianglesToRaster;
	for (auto tri : model.tris)
	{
		triangle triProjected, triTransformed, triViewed;

		triTransformed.p[0] = matWorld * tri.p[0];
		triTransformed.p[1] = matWorld * tri.p[1];
		triTransformed.p[2] = matWorld * tri.p[2];

		// Use Cross-Product to get surface normal
		OVec3 normal, line1, line2;
		line1 = triTransformed.p[1] - triTransformed.p[0];
		line2 = triTransformed.p[2] - triTransformed.p[0];

		normal = OVec3::cross(line1, line2);
		normal = OVec3::normalize(normal);

		OVec3 vCameraRay = triTransformed.p[0] - vCamera;

		if (OVec3::dot(normal, vCameraRay) < 0.0f)
		{
			// Illumination
			OVec3 light_direction = { 0.0f, 0.0f, -1.0f };
			light_direction = OVec3::normalize(light_direction);

			// How similar is normal to light direction
			float dp = max(0.1f, OVec3::dot(light_direction, normal));
			/*
			float dp1 = setIllumination(triTransformed.p[0], normal);
			float dp2 = setIllumination(triTransformed.p[1], normal);
			float dp3 = setIllumination(triTransformed.p[2], normal);
			dp = dp1 + dp2 + dp3;
			if (dp > 2.0f)dp = .9f;
			else if (dp < 0.0001)dp = 0.1f;
			*/
			CHAR_INFO c = GetColour(dp);//dp1/3+dp2/3+dp3/3);


			triProjected.col = c.Attributes;
			triProjected.sym = c.Char.UnicodeChar;

			//triProjected.col
			// Project triangles from 3D --> 2D
			triViewed.p[0] = matView * triTransformed.p[0];
			triViewed.p[1] = matView * triTransformed.p[1];
			triViewed.p[2] = matView * triTransformed.p[2];
			triViewed.col = triProjected.col;
			triViewed.sym = triProjected.sym;

			// Clip Viewed Triangle against near plane, this could form two additional
			// additional triangles. 
			int nClippedTriangles = 0;
			triangle clipped[2];
			nClippedTriangles = Triangle_ClipAgainstPlane(OVec3(0.0f, 0.0f, 0.1f), OVec3(0.0f, 0.0f, 1.0f), triViewed, clipped[0], clipped[1]);

			for (int n = 0; n < nClippedTriangles; n++)
			{
				// Project triangles from 3D --> 2D
				triProjected.p[0] = matProj * clipped[n].p[0];
				triProjected.p[1] = matProj * clipped[n].p[1];
				triProjected.p[2] = matProj * clipped[n].p[2];
				triProjected.col = clipped[n].col;
				triProjected.sym = clipped[n].sym;


				/*triProjected.p[0] = triProjected.p[0]/ triProjected.p[0].w;
				triProjected.p[1] = triProjected.p[1]/ triProjected.p[1].w;
				triProjected.p[2] = triProjected.p[2]/ triProjected.p[2].w;*/

				// X/Y are inverted so put them back
				triProjected.p[0].m_x *= -1.0f;
				triProjected.p[1].m_x *= -1.0f;
				triProjected.p[2].m_x *= -1.0f;
				triProjected.p[0].m_y *= -1.0f;
				triProjected.p[1].m_y *= -1.0f;
				triProjected.p[2].m_y *= -1.0f;

				// Offset verts into visible normalised space
				OVec3 vOffsetView(1, 1, 0);
				triProjected.p[0] = triProjected.p[0] + vOffsetView;
				triProjected.p[1] = triProjected.p[1] + vOffsetView;
				triProjected.p[2] = triProjected.p[2] + vOffsetView;
				triProjected.p[0].m_x *= 0.5f * (float)ScreenWidth();
				triProjected.p[0].m_y *= 0.5f * (float)ScreenHeight();
				triProjected.p[1].m_x *= 0.5f * (float)ScreenWidth();
				triProjected.p[1].m_y *= 0.5f * (float)ScreenHeight();
				triProjected.p[2].m_x *= 0.5f * (float)ScreenWidth();
				triProjected.p[2].m_y *= 0.5f * (float)ScreenHeight();

				// Store triangle for sorting
				vecTrianglesToRaster.push_back(triProjected);

			}
		}

	}
	// Sort triangles from back to front
	sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2)
		{
			float z1 = (t1.p[0].m_z + t1.p[1].m_z + t1.p[2].m_z) / 3.0f;
			float z2 = (t2.p[0].m_z + t2.p[1].m_z + t2.p[2].m_z) / 3.0f;
			return z1 > z2;
		});

	for (auto& triProjected : vecTrianglesToRaster)
	{
		// Rasterize triangle
		FillTriangle(triProjected.p[0].m_x, triProjected.p[0].m_y,
			triProjected.p[1].m_x, triProjected.p[1].m_y,
			triProjected.p[2].m_x, triProjected.p[2].m_y,
			triProjected.sym, triProjected.col);

		DrawTriangle(triProjected.p[0].m_x, triProjected.p[0].m_y,
			triProjected.p[1].m_x, triProjected.p[1].m_y,
			triProjected.p[2].m_x, triProjected.p[2].m_y,
			PIXEL_SOLID, FG_BLACK);
	}
}


void Engine::setTRS(OVec3 tranlate, OVec3 rotate, OVec3 scale)
{
	matWorld.setIdentity();

	OMat4 temp;
	temp.setTranslation(tranlate);
	matWorld *= temp;


	temp.setIdentity();
	temp.setRotationX(rotate.m_x);
	matWorld *= temp;

	temp.setIdentity();
	temp.setRotationY(rotate.m_y);
	matWorld *= temp;

	temp.setIdentity();
	temp.setRotationZ(rotate.m_z);
	matWorld *= temp;

	temp.setIdentity();
	temp.setScale(scale);
	matWorld *= temp;


}

float Engine::setIllumination(OVec3 FragPos,OVec3 Normal)
{
		
	PointLight pointLights[4];
	for (int i = 0;i < 4;i++)
	{
		pointLights[i].position = pointLightPositions[i];//pointLightPositions
		pointLights[i].ambient = OVec3(0.05f, 0.05f, 0.05f);
		pointLights[i].diffuse = OVec3(0.8f, 0.8f, 0.8f);
		pointLights[i].specular = OVec3(1.0f, 1.0f, 1.0f);
		pointLights[i].constant = 1.0f;
		pointLights[i].linear = 0.5;
		pointLights[i].quadratic = 0.032f;
	}


	
	DirLight dirLight;
	SpotLight spotLight;


	// properties
	OVec3 norm = OVec3::normalize(Normal);
	OVec3 viewDir = OVec3::normalize(viewPos - FragPos);

	// directional lighting
	OVec3 result = CalcDirLight(dirLight, norm, viewDir);
	//  point lights
	for (int i = 0; i < 4; i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	
	return ((result.m_x + result.m_y + result.m_z)/3.0f);
}


OVec3 CalcDirLight(Engine::DirLight light, OVec3 normal, OVec3 viewDir)
{
	OVec3 lightDir = OVec3::normalize(light.direction);
	lightDir = lightDir * -1;
	// diffuse shading
	float diff = max(OVec3::dot(normal, lightDir), 0.0);
	// specular shading
	OVec3 reflectDir = OVec3::reflect(lightDir*-1, normal);
	float spec = pow(max(OVec3::dot(viewDir, reflectDir), 0.0), 2.0f);
	// combine results
	OVec3 ambient = light.ambient;
	OVec3 diffuse = light.diffuse * diff;
	OVec3 specular = light.specular * spec;

	return (ambient + diffuse + specular);
}

	// calculates the color when using a point light.
OVec3 CalcPointLight(Engine::PointLight light, OVec3 normal, OVec3 fragPos, OVec3 viewDir)
{
	OVec3 lightDir = OVec3::normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(OVec3::dot(normal, lightDir), 0.0);
	// specular shading
	OVec3 reflectDir = OVec3::reflect(lightDir*-1, normal);
	float spec = pow(max(OVec3::dot(viewDir, reflectDir), 0.0), 2.0f);
	// attenuation
	float distance = OVec3::length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// combine results
	OVec3 ambient = light.ambient;
	OVec3 diffuse = light.diffuse;
	OVec3 specular = light.specular*spec;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
	// calculates the color when using a spot light.
OVec3 CalcSpotLight(Engine::SpotLight light, OVec3 normal, OVec3 fragPos, OVec3 viewDir)
{
	OVec3 lightDir = OVec3::normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(OVec3::dot(normal, lightDir), 0.0);
	// specular shading
	OVec3 reflectDir = OVec3::reflect(lightDir*-1, normal);
	float spec = pow(max(OVec3::dot(viewDir, reflectDir), 0.0), 2.0f);
	// attenuation
	float distance = OVec3::length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// spotlight intensity
	float theta = OVec3::dot(lightDir, OVec3::normalize((light.direction)*-1));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = OVec3::clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// combine results
	OVec3 ambient = light.ambient;
	OVec3 diffuse = light.diffuse * diff;
	OVec3 specular = light.specular * spec;
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);

};

CHAR_INFO Engine::GetColour(float lum)
{
	short bg_col, fg_col;
	wchar_t sym;
	int pixel_bw = (int)(13.0f * lum);
	switch (pixel_bw)
	{
	case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;

	case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
	case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
	case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
	case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;

	case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
	case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
	case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
	case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID; break;

	case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
	case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
	case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
	case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
	default:
		bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID;
	}

	CHAR_INFO c;
	c.Attributes = bg_col | fg_col;
	c.Char.UnicodeChar = sym;
	return c;
}

int Engine::Triangle_ClipAgainstPlane(OVec3 plane_p, OVec3 plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2)
{
	// Make sure plane normal is indeed normal
	plane_n = OVec3::normalize(plane_n);

	// Return signed shortest distance from point to plane, plane normal must be normalised
	auto dist = [&](OVec3& p)
	{
		OVec3 n = OVec3::normalize(p);
		return (plane_n.m_x * p.m_x + plane_n.m_y * p.m_y + plane_n.m_z * p.m_z -OVec3:: dot(plane_n, plane_p));
	};

	// Create two temporary storage arrays to classify points either side of plane
	// If distance sign is positive, point lies on "inside" of plane
	OVec3* inside_points[3];  int nInsidePointCount = 0;
	OVec3* outside_points[3]; int nOutsidePointCount = 0;

	// Get signed distance of each point in triangle to plane
	float d0 = dist(in_tri.p[0]);
	float d1 = dist(in_tri.p[1]);
	float d2 = dist(in_tri.p[2]);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[0]; }
	if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[1]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[1]; }
	if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[2]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[2]; }

	// Now classify triangle points, and break the input triangle into 
	// smaller output triangles if required. There are four possible
	// outcomes...

	if (nInsidePointCount == 0)
	{
		// All points lie on the outside of plane, so clip whole triangle
		// It ceases to exist

		return 0; // No returned triangles are valid
	}

	if (nInsidePointCount == 3)
	{
		// All points lie on the inside of plane, so do nothing
		// and allow the triangle to simply pass through
		out_tri1 = in_tri;

		return 1; // Just the one returned original triangle is valid
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		// Triangle should be clipped. As two points lie outside
		// the plane, the triangle simply becomes a smaller triangle

		// Copy appearance info to new triangle
		out_tri1.col = in_tri.col;
		out_tri1.sym = in_tri.sym;

		// The inside point is valid, so keep that...
		out_tri1.p[0] = *inside_points[0];

		// but the two new points are at the locations where the 
		// original sides of the triangle (lines) intersect with the plane
		out_tri1.p[1] = OVec3::Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
		out_tri1.p[2] = OVec3::Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

		return 1; // Return the newly formed single triangle
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		// Triangle should be clipped. As two points lie inside the plane,
		// the clipped triangle becomes a "quad". Fortunately, we can
		// represent a quad with two new triangles

		// Copy appearance info to new triangles
		out_tri1.col = in_tri.col;
		out_tri1.sym = in_tri.sym;

		out_tri2.col = in_tri.col;
		out_tri2.sym = in_tri.sym;

		// The first triangle consists of the two inside points and a new
		// point determined by the location where one side of the triangle
		// intersects with the plane
		out_tri1.p[0] = *inside_points[0];
		out_tri1.p[1] = *inside_points[1];
		out_tri1.p[2] =OVec3:: Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above
		out_tri2.p[0] = *inside_points[1];
		out_tri2.p[1] = out_tri1.p[2];
		out_tri2.p[2] = OVec3::Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

		return 2; // Return two newly formed triangles which form a quad
	}
}