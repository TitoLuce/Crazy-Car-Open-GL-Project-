#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Application.h"

#include "Bullet/include/btBulletDynamicsCommon.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	vec3 GetCMCoordinates();
	vec3 Quaternion2EulerAngles(btQuaternion* q);

private:

	void CalculateViewMatrix();

public:
	int zoom;
	int camMode;
	int angle;
	vec3 X, Y, Z, Position, Reference;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};