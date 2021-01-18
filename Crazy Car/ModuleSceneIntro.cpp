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
	//CreateRoad();
	//CreateCurve(9, 35, 1);
	CreateTrack();

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

void ModuleSceneIntro::CreateTrack()
{
	float Height = 10.0f;
	float Radius = 2.0f;

	//Inner walls

	for (int n = 0; n < 40; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(XPos, 5.0f, ZPos);
		ZPos += Radius + Radius + Radius / 2;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	float centerX = XPos - 35;
	float centerZ = ZPos - 5;
	float ang = 0.0f;

	for (int n = 0; n < 20; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(centerX + 35 * (-cos(DEGTORAD * ang)), 5.0f, centerZ + 35 * sin(DEGTORAD * ang));
		ang += 9.0f;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	XPos = -80.0f;
	ZPos = -100.0f;

	for (int n = 0; n < 40; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(XPos, 5.0f, ZPos);
		ZPos += Radius + Radius + Radius / 2;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	centerX = -45;
	centerZ = -95;
	ang=0;

	for (int n = 0; n < 20; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(centerX + 35 * cos(DEGTORAD * ang), 5.0f, centerZ + 35 * -sin(DEGTORAD * ang));
		ang += 9.0f;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	//Outer walls
	XPos = 30.0f;
	ZPos = -100.0f;

	for (int n = 0; n < 40; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(XPos, 5.0f, ZPos);
		ZPos += Radius + Radius + Radius / 2;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	centerX = XPos - 75;
	centerZ = ZPos - 5;
	ang = 0.0f;

	for (int n = 0; n < 40; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(centerX + 75 * (-cos(DEGTORAD * ang)), 5.0f, centerZ + 75 * sin(DEGTORAD * ang));
		ang += 4.5f;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	XPos = -120.0f;
	ZPos = -100.0f;

	for (int n = 0; n < 40; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(XPos, 5.0f, ZPos);
		ZPos += Radius + Radius + Radius / 2;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	centerX = -45;
	centerZ = -95;
	ang = 0;

	for (int n = 0; n < 40; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(centerX + 75 * cos(DEGTORAD * ang), 5.0f, centerZ + 75 * -sin(DEGTORAD * ang));
		ang += 4.5f;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}
}