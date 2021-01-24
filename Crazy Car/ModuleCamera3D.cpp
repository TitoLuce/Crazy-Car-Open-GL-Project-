#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModuleCamera3D.h"
#include <cmath>


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	zoom = 5;
	angle = 15;
	camMode = 1;

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 0.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	float speed = 3.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	vec3 EulerAngles = Quaternion2EulerAngles(&App->player->vehicle->vehicle->getRigidBody()->getWorldTransform().getRotation());
	
	
	Reference = GetCMCoordinates();

	X = rotate(vec3(1, 0, 0), EulerAngles.y * 180 / M_PI, vec3(0, 1, 0));
	Z = rotate(vec3(0, 0, 1), EulerAngles.y * 180 / M_PI, vec3(0, 1, 0));
	Y = rotate(vec3(0, 1, 0), EulerAngles.y * 180 / M_PI, vec3(0, 1, 0));

	X = rotate(X, 180, vec3(0, 1, 0));
	Y = rotate(Y, 180, vec3(0, 1, 0));
	Z = rotate(Z, 180, vec3(0, 1, 0));

	if (camMode == 1)
	{
		zoom = 5;
		angle = 15;
	}
	else if (camMode == 2)
	{
		zoom = 200;
		angle = 90;
	}

	Y = rotate(Y, -angle, X);
	Z = rotate(Z, -angle, X);

	Position = Reference + Z * 50.0f;
	
	// Mouse motion ----------------

	//if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	//{
	//	int dx = -App->input->GetMouseXMotion();
	//	int dy = -App->input->GetMouseYMotion();

	//	float Sensitivity = 0.25f;

	//	Position -= Reference;

	//	if(dx != 0)
	//	{
	//		float DeltaX = (float)dx * Sensitivity;

	//		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//	}

	//	if(dy != 0)
	//	{
	//		float DeltaY = (float)dy * Sensitivity;

	//		Y = rotate(Y, DeltaY, X);
	//		Z = rotate(Z, DeltaY, X);

	//		if(Y.y < 0.0f)
	//		{
	//			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//			Y = cross(Z, X);
	//		}
	//	}

	//	Position = Reference + Z * length(Position);
	//}

	// Recalculate matrix -------------

	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

vec3 ModuleCamera3D::GetCMCoordinates()
{
	vec3 a = { App->player->vehicle->vehicle->getRigidBody()->getWorldTransform().getOrigin().getX(),
		App->player->vehicle->vehicle->getRigidBody()->getWorldTransform().getOrigin().getY() + zoom,
		App->player->vehicle->vehicle->getRigidBody()->getWorldTransform().getOrigin().getZ()};
	return a;
}

vec3  ModuleCamera3D::Quaternion2EulerAngles(btQuaternion* q0)
{
	struct Quaternion
	{
		float w, x, y, z;
	}q;
	vec3 angles;
	q.w = q0->getW();
	q.x = q0->getX();
	q.y = q0->getZ();
	q.z = q0->getY();
	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (z-axis rotation)
	double sinp = 2 * (q.w * q.y - q.z * q.x);
	if (std::abs(sinp) >= 1)
		angles.z = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		angles.z = std::asin(sinp);

	// yaw (y-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.y = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}