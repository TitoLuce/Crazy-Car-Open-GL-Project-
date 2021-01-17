#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//App->camera->Move(vec3(1.0f, 1.0f, 1.0f));
	//App->camera->LookAt(vec3(0, 0, 0));
	CreateRoad();
	CreateCurve(9, 35, 1);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (int i = 0; i < primitives.Count(); i++)
	{
		primitives[i]->Update();
		primitives[i]->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}
void ModuleSceneIntro::CreateRoad()
{
	const int Length = 7;


	float Height = 10.0f;
	float Radius = 2.0f;
	for (int n = 0; n < Length; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));
		
		s->SetPos(XPos, 5.0f, ZPos);
		XPos += Radius + Radius + Radius / 2;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}
}

void ModuleSceneIntro::CreateCurve(float angle, float radiusC, int type)
{
	const int Length = 10;
	float centerX = XPos - radiusC;
	float centerZ = ZPos - radiusC;
	float ang = 0.0f;

	float Height = 10.0f;
	float Radius = 2.0f;
	for (int n = 0; n < Length; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(centerX + radiusC * (-cos(DEGTORAD * ang)), 5.0f, centerZ + radiusC * sin(DEGTORAD * ang));
		//XPos += Radius + Radius + Radius / 2;
		ang += angle;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}
}