#ifndef __MODULE_PHYSICS_H__
#define __MODULE_PHYSICS_H__

#include <math.h>
#include "Module.h"
#include "External/Box2D/include/Box2D.h"

class GameObject;

#define GRAVITY_X 0.0f
#define GRAVITY_Y -10.0f

#define PIXELS_PER_METER 40.0f
#define METERS_PER_PIXELS 0.025f

#define METERS_TO_PIXELS(m) ((int)(PIXELS_PER_METER * m) / 2)
#define PIXELS_TO_METER(p) ((float)METERS_PER_PIXELS * p * 2)

class PhysBody
{
public:

	PhysBody() : body(NULL)
	{}

	PhysBody(GameObject* g) : body(NULL)
	{
		gameObject = g;
	}

	~PhysBody()
	{
		if (body != nullptr)
		{
			body->GetWorld()->DestroyBody(body);
			body = nullptr;
		}
	}

	void SetSensor(bool isSensor)
	{
		body->GetFixtureList()->SetSensor(isSensor);

		this->isSensor = isSensor;
	}

	bool GetSenser() const { return isSensor; }

	void GetPosition(int& x, int& y) const;
	b2Vec2 GetPosition();
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
	void GetCenterPosition(int& x, int& y) const;
public:
	int width = 0, height = 0;
	bool chainLoop = false;
	bool isSensor = false;
	b2Body* body = nullptr;
	GameObject* gameObject = nullptr;
};

class ModulePhysics : public Module, public b2ContactListener
{
public:

	b2World* world = nullptr;
	
	ModulePhysics(bool start_enabled = true);
	~ModulePhysics();

	bool Init(pugi::xml_node&);
	bool Start();
	UpdateStatus PreUpdate() override;
	UpdateStatus Update();
	UpdateStatus PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, GameObject* gameObject = nullptr, bool isSensor = false);
	PhysBody* CreateRectangle(iPoint pos, int width, int height, GameObject* gameObject = nullptr, b2BodyType colType = b2BodyType::b2_dynamicBody);
	PhysBody* CreateRectangleSensor(iPoint pos, int width, int height, GameObject* g = nullptr, b2BodyType colType = b2BodyType::b2_dynamicBody);
	PhysBody* CreateLine(b2Vec2 startPos, b2Vec2 dir, GameObject* gameObject);
	PhysBody* CreateChainObj(int x, int y, int* points, int size, bool loop, GameObject* gameObject = nullptr, b2BodyType colType = b2BodyType::b2_dynamicBody);
	PhysBody* CreateEdgeObj(int x, int y, int* points, int size, bool loop);
	b2Vec2 Perp(b2Vec2 vec1);
	void DotProductAngle(b2Vec2 v1,b2Vec2 v2, float& angle);
	
	void Pause(bool pause);

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

	void ShapesRender();

	// Collision layers
	short EVERY_LAYER = 0xFFFF;
	short WORLD_LAYER = 0x0001;
	short PLAYER_LAYER = 0x0002;
	short PROJECTILE_LAYER = 0x0004;
	short ENEMY_LAYER = 0x0008;

private:
	b2Body* mouseBody = nullptr;
	b2MouseJoint* mouseJoint = nullptr;
	bool debug = false;
	bool physPause = false;
};

#endif // !__MODULE_PHYSICS_H__