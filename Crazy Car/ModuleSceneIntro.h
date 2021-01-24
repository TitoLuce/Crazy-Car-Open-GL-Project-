#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#include "Color.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "glut/glut.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void CreatePendulum(const float& x, const float& z);

	void CreateRamp(const float& x, const float& y, const float& z, const float& inclination, const float& rotation);

	void Angles2AxisAngle(const float& alpha_x, const float& beta_y, vec3& u, float& angle);
	//void CreateRoad();

	//void CreateCurve(float angle, float radiusC, int type);

	void CreateTrack();

	void PrintText(float x, float y, float z, Color color, const char* string)
	{
		glColor3f(color.r, color.g, color.b);
		glRasterPos3f(x, y, z);
		int len, i;
		len = (int)strlen(string);
		for (i = 0; i < len; ++i)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
		}
	}

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	p2DynArray<Primitive*> primitives;

	float XPos = -10.0f;
	float ZPos = -100.0f;

	float seconds;
	int laps;

	char timer[10] = { "\0" };
	char lap[10] = { "\0" };

	bool win;
	bool started;

	p2DynArray <Cube*> map;
	p2DynArray<Sphere*> pendulumBall_shape;
	p2DynArray<PhysBody3D*> pendulumBall_body;
};
