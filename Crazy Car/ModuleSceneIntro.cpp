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

	App->audio->PlayMusic("Assets/Subnautica Soundtrack - Abandon ship [Extended mix].wav");

	seconds = 300;
	laps = 1;
	win = false;
	started = false;

	CreateTrack();

	CreatePendulum(25, 60);
	CreatePendulum(20, 60);
	CreatePendulum(15, 60);
	CreatePendulum(10, 60);
	CreatePendulum(5, 60);
	CreatePendulum(0, 60);
	CreatePendulum(-5, 60);

	CreatePendulum(25, -80);
	CreatePendulum(20, -70);
	CreatePendulum(15, -60);
	CreatePendulum(10, -50);
	CreatePendulum(5, -40);
	CreatePendulum(0, -30);
	CreatePendulum(-5, -20);

	CreateRamp(-90, 0.6, 60, -45, 0);
	CreateRamp(-110, 0.6, -20, -45, 0);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

update_status ModuleSceneIntro::PreUpdate(float dt)
{
	if (!started)
	{
		App->camera->camMode = 2;
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			started = true;
			App->camera->camMode = 1;
		}
		return UPDATE_CONTINUE;
	}

	if (seconds > 0 && !win) { seconds -= dt; }

	return UPDATE_CONTINUE;
}

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
	for (int i = 0; i < map.Count(); i++)
	{
		map[i]->Render();
	}

	for (int i = 0; i < pendulumBall_body.Count(); i++) {
		pendulumBall_body[i]->GetTransform(&pendulumBall_shape[i]->transform);
		pendulumBall_shape[i]->Render();
	}

	if (!started)
	{ 
		PrintText(App->player->GetX() + 22, App->player->GetY() + 50.0f, App->player->GetZ() + 80, White, "Welcome to Crazy Car");
		PrintText(App->player->GetX() + 20, App->player->GetY() + 25.0f, App->player->GetZ() + 70, White, "Press Enter to start");
		return UPDATE_CONTINUE;
	}

	sprintf_s(timer, 10, "Time: %03d", (int)seconds);
	sprintf_s(lap, 10, "Lap: %d/3", laps);
	if (seconds > 0)
	{
		PrintText(App->player->GetX(), App->player->GetY() + 25.0f, App->player->GetZ(), White, timer);
		PrintText(App->player->GetX(), App->player->GetY() + 22.0f, App->player->GetZ(), White, lap);
	}
	else { PrintText(App->player->GetX(), App->player->GetY() + 23.0f, App->player->GetZ(), White, "GAME OVER"); }
	

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

	for (int n = 0; n < 60; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(XPos, 5.0f, ZPos);
		ZPos += Radius + Radius / 2;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	float centerX = XPos - 35;
	float centerZ = ZPos - 5;
	float ang = 0.0f;

	for (int n = 0; n < 30; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(centerX + 35 * (-cos(DEGTORAD * ang)), 5.0f, centerZ + 35 * sin(DEGTORAD * ang));
		ang += 6.0f;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	XPos = -80.0f;
	ZPos = -100.0f;

	for (int n = 0; n < 60; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(XPos, 5.0f, ZPos);
		ZPos += Radius + Radius / 2;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	centerX = -45;
	centerZ = -95;
	ang=0;

	for (int n = 0; n < 30; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(centerX + 35 * cos(DEGTORAD * ang), 5.0f, centerZ + 35 * -sin(DEGTORAD * ang));
		ang += 6.0f;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	//Outer walls
	XPos = 30.0f;
	ZPos = -100.0f;

	for (int n = 0; n < 60; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(XPos, 5.0f, ZPos);
		ZPos += Radius + Radius / 2;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	centerX = XPos - 75;
	centerZ = ZPos - 5;
	ang = 0.0f;

	for (int n = 0; n < 60; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(centerX + 75 * (-cos(DEGTORAD * ang)), 5.0f, centerZ + 75 * sin(DEGTORAD * ang));
		ang += 3.0f;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	XPos = -120.0f;
	ZPos = -100.0f;

	for (int n = 0; n < 60; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(XPos, 5.0f, ZPos);
		ZPos += Radius + Radius / 2;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}

	centerX = -45;
	centerZ = -95;
	ang = 0;

	for (int n = 0; n < 60; n++)
	{
		Cylinder* s = new Cylinder(Radius, Height);
		s->SetRotation(90, vec3(0, 0, 1));

		s->SetPos(centerX + 75 * cos(DEGTORAD * ang), 5.0f, centerZ + 75 * -sin(DEGTORAD * ang));
		ang += 3.0f;
		s->body = App->physics->AddBody(*s, 0.0f);
		s->color = { 0.5,0.5,0,1 };
		primitives.PushBack(s);
	}
}

void ModuleSceneIntro::CreatePendulum(const float& x, const float& z)
{
	int support_high = 15;
	Cube* support_shape = new Cube();
	Sphere* ball_shape = new Sphere(2);

	support_shape->SetPos(x, support_high, z);
	support_shape->color.Set(0, 0, 0);
	ball_shape->color.Set(10, 50, 3);
	ball_shape->SetPos(x, 0.2F, z);

	PhysBody3D* support = App->physics->AddBody(*support_shape, 0.0F);
	PhysBody3D* ball = App->physics->AddBody(*ball_shape, 200.0F);

	map.PushBack(support_shape);
	pendulumBall_shape.PushBack(ball_shape);
	pendulumBall_body.PushBack(ball);

	App->physics->AddConstraintP2P(*support, *ball, vec3(0, 0, 0), vec3(0, 12, 0));
}

void ModuleSceneIntro::CreateRamp(const float & x, const float & y, const float & z, const float & inclination, const float & rotation)
{
	Cube* c = new Cube(20, 1, 30);
	c->SetPos(x, y, z);
	mat4x4 mat = c->transform;
	vec3 u(0,0,0);
	float angle = 0.0f;
	Angles2AxisAngle(inclination*3, rotation,u,angle);
	LOG("axis %.2f %.2f %.2f angle %.2f", u.x, u.y, u.z, angle);
	c->SetRotation(angle, u);

	//c->SetRotation(rotation, vec3(0.0f, 1.0f, 0.0f));
	PhysBody3D* phys = App->physics->AddBody(*c, 0.0f);
	
	map.PushBack(c);
}

void ModuleSceneIntro::Angles2AxisAngle(const float & alpha_x, const float & beta_y, vec3 & u, float & angle)
{
	double c1 = cos(alpha_x / 2);
	double s1 = sin(alpha_x / 2);
	double c2 = cos(beta_y / 2);
	double s2 = sin(beta_y / 2);
	double c3 = cos(0 / 2);
	double s3 = cos(0 / 2);
	double c1c2 = c1 * c2;
	double s1s2 = s1 * s2;
	int w = c1c2 * c3 - s1s2 * s3;
	int x = c1c2 * s3 + s1s2 * c3;
	int y = s1 * c2*c3 + c1 * s2*s3;
	int z = c1 * s2*c3 - s1 * c2*s3;
	angle = 2 * acos(w);
	double norm = x * x + y * y + z * z;
	if (norm < 0.001) { // when all euler angles are zero angle = 0 so
		// we can set axis to anything to avoid divide by zero
		x = 1;
		y = z = 0;
	}
	else {
		norm = sqrt(norm);
		x /= norm;
		y /= norm;
		z /= norm;
	}
	u = vec3(x, y, z);
}