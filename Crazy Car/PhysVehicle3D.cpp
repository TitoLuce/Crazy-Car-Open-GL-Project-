#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());
	chassis.color = Red;
	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	Cube llantern(info.llantern_size.x, info.llantern_size.y, info.llantern_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&llantern.transform);
	btVector3 llanternoffset(info.llantern_offset.x, info.llantern_offset.y, info.llantern_offset.z);
	llanternoffset = llanternoffset.rotate(q.getAxis(), q.getAngle());

	llantern.color = Black;
	llantern.transform.M[12] += llanternoffset.getX();
	llantern.transform.M[13] += llanternoffset.getY();
	llantern.transform.M[14] += llanternoffset.getZ();

	Cube rlantern(info.rlantern_size.x, info.rlantern_size.y, info.rlantern_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rlantern.transform);
	btVector3 rlanternoffset(info.rlantern_offset.x, info.rlantern_offset.y, info.rlantern_offset.z);
	rlanternoffset = rlanternoffset.rotate(q.getAxis(), q.getAngle());

	rlantern.color = Black;
	rlantern.transform.M[12] += rlanternoffset.getX();
	rlantern.transform.M[13] += rlanternoffset.getY();
	rlantern.transform.M[14] += rlanternoffset.getZ();

	Cube cabin(info.cabin_size.x, info.cabin_size.y, info.cabin_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cabin.transform);
	btVector3 cabinoffset(info.cabin_offset.x, info.cabin_offset.y, info.cabin_offset.z);
	cabinoffset = cabinoffset.rotate(q.getAxis(), q.getAngle());

	cabin.color = Red;
	cabin.transform.M[12] +=cabinoffset.getX();
	cabin.transform.M[13] += cabinoffset.getY();
	cabin.transform.M[14] += cabinoffset.getZ();

	cabin.Render();
	chassis.Render();
	llantern.Render();
	rlantern.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}