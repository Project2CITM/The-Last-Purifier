#include "GameObject.h"

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "physics";
	world = NULL;	
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Init(pugi::xml_node& config)
{
	this->config = config;

	return true;
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(0, 0));
	b2BodyDef bd;
	mouseBody = world->CreateBody(&bd);

	LOG("Tedt��");

	world->SetContactListener(this);
	
	return true;
}
 
UpdateStatus ModulePhysics::PreUpdate()
{
	world->Step(App->frameTime, 6, 2);

	return UPDATE_CONTINUE;
}

UpdateStatus ModulePhysics::Update()
{
	return UPDATE_CONTINUE;
}

UpdateStatus ModulePhysics::PostUpdate()
{
	/*
	if (!App->debug->debugTakeObject)
		return UPDATE_CONTINUE;

	// If mouse button 1 is pressed ...
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetType() != b2BodyType::b2_dynamicBody) continue;

		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				PhysBody* pBody = (PhysBody*)b->GetUserData();

				// test if the current body contains mouse position
				if (pBody && pBody->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
				{
					b2MouseJointDef def;
					def.bodyA = mouseBody;
					def.bodyB = pBody->body;
					def.target = b2Vec2(PIXELS_TO_METER(App->input->GetMouseX()), PIXELS_TO_METER(App->input->GetMouseY()));
					def.dampingRatio = 0.5f;
					def.frequencyHz = 2.0f;
					def.maxForce = 100.0f * pBody->body->GetMass();
					mouseJoint = (b2MouseJoint*)world->CreateJoint(&def);
				}
			}

			bool hasMouseJoint = false;
			b2JointEdge* tempJoint = b->GetJointList();

			while (tempJoint != nullptr)
			{
				if (tempJoint->joint == mouseJoint)
				{
					hasMouseJoint = true;
				}
				tempJoint = tempJoint->next;
			}

			if (!hasMouseJoint) break;

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && b->GetJointList() != nullptr && mouseJoint != nullptr)
			{
				b2Vec2 nextPos = { (float)App->input->GetMouseX(),(float)App->input->GetMouseY() };

				nextPos.x = PIXELS_TO_METER(nextPos.x);
				nextPos.y = PIXELS_TO_METER(nextPos.y);

				mouseJoint->SetTarget(nextPos);
			
			}

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && b->GetJointList() != nullptr && mouseJoint != nullptr)
			{
				world->DestroyJoint(mouseJoint);
				mouseJoint = nullptr;
			}
		}
	}
	*/
	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, GameObject* gameObject, bool isSensor)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXELS_TO_METER(x), PIXELS_TO_METER(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXELS_TO_METER(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.isSensor = isSensor;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = radius;

	pbody->body->SetUserData(pbody);

	pbody->gameObject = gameObject;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(iPoint pos, int width, int height, GameObject* gameObject)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXELS_TO_METER(pos.x), PIXELS_TO_METER(pos.y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METER(width), PIXELS_TO_METER(height));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = width;
	pbody->height = height;

	pbody->body->SetUserData(pbody);

	pbody->gameObject = gameObject;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(iPoint pos, int width, int height, GameObject* g)
{
	b2BodyDef body;
	body.type = b2_kinematicBody;
	body.position.Set(PIXELS_TO_METER(pos.x), PIXELS_TO_METER(pos.y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METER(width), PIXELS_TO_METER(height));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = width;
	pbody->height = height;

	pbody->body->SetUserData(pbody);

	pbody->gameObject = g;

	return pbody;
}

PhysBody* ModulePhysics::CreateLine(b2Vec2 startPos, b2Vec2 dir, GameObject* gameObject)
{
	b2Vec2* p = new b2Vec2[2];
	p[0] = { 0,0 };
	p[1] = { PIXELS_TO_METER(dir.x), PIXELS_TO_METER(dir.y) };

	PhysBody* pbody = new PhysBody();
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXELS_TO_METER(startPos.x), PIXELS_TO_METER(startPos.y));

	pbody->body = world->CreateBody(&body);

	b2ChainShape shape;
	shape.CreateChain(p, 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	pbody->body->CreateFixture(&fixture);

	pbody->width = pbody->height = 0;
	pbody->body->SetUserData(pbody);

	pbody->gameObject = gameObject;

	RELEASE(p);

	return pbody;
}

PhysBody* ModulePhysics::CreateChainObj(int x, int y, int* points, int size, bool loop, GameObject* gameObject)
{
	PhysBody* pbody = new PhysBody();
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXELS_TO_METER(x), PIXELS_TO_METER(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXELS_TO_METER(points[i * 2 + 0]);
		p[i].y = PIXELS_TO_METER(points[i * 2 + 1]);
	}
	
	if (loop)
	{
		shape.CreateLoop(p, size / 2);
		pbody->chainLoop = true;
	}
	else
	{
		shape.CreateChain(p, size / 2);
	}
		
	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	b->SetUserData(pbody);

	delete p;

	pbody->body = b;

	pbody->width = pbody->height = 0;

	pbody->gameObject = gameObject;

	return pbody;
}

PhysBody* ModulePhysics::CreateEdgeObj(int x, int y, int* points, int size, bool loop)
{
	return nullptr;
}

b2Vec2 ModulePhysics::Perp(b2Vec2 vec1)
{
	b2Vec2 perpend;
	perpend.x = -vec1.y;
	perpend.y = vec1.x;

	return perpend;
};

void ModulePhysics::DotProductAngle(b2Vec2 v1, b2Vec2 v2, float& angle) 
{
	int dot;
	dot = v1.x * v2.x + v1.y * v2.y;

	float modulev1 = sqrt(pow(v1.x, 2) + pow(v1.y, 2));
	float modulev2 = sqrt(pow(v2.x, 2) + pow(v2.y, 2));

	float theta = dot / (modulev1 * modulev2);


	angle = acos(theta);

}

void ModulePhysics::Pause(bool pause)
{
	physPause = pause;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{	
	PhysBody* a = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();

	PhysBody* b = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (a && a->gameObject)
	{
		a->gameObject->OnCollisionEnter(b);
	}	

	if (b && b->gameObject)
	{
		b->gameObject->OnCollisionEnter(a);
	}	
	
	//LOG("collision!!");
}

void ModulePhysics::EndContact(b2Contact* contact)
{
	PhysBody* a = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();

	PhysBody* b = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (a && a->gameObject)
	{
		a->gameObject->OnCollisionExit(b);
	}

	if (b && b->gameObject)
	{
		b->gameObject->OnCollisionExit(a);
	}

}

void ModulePhysics::ShapesRender()
{
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		// If GameObject not enable, not render
		PhysBody* pb = (PhysBody*)b->GetUserData();
		//if (pb && !pb->gameObject->enable) continue;

		// If body not active, continue.
		if (!b->IsActive()) continue;

		// Render object collision
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->AddCircleRenderQueue(iPoint{ METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y) }, METERS_TO_PIXELS(shape->m_radius), SDL_Color{ 0,0,0,255 },3);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				PhysBody* g = (PhysBody*)b->GetUserData();

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
					{
						App->renderer->AddLineRenderQueue(iPoint{ METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y) }, iPoint{ METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y) },
							g->gameObject->adjustToGrid, SDL_Color{ 255, 100, 100, 255 }, 3, 100);
					}
					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				App->renderer->AddLineRenderQueue(iPoint{ METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y) }, iPoint{ METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y) },
					g->gameObject->adjustToGrid, SDL_Color{ 255, 100, 100, 255 }, 3, 100);					
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				SDL_Color color = { 0,0,0,255 };

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					// Variable debug, luego se borra
					PhysBody* g = (PhysBody*)b->GetUserData();

					if (g->isSensor)
					{
						color = { 0,0,0,100 };
					}
					else
					{
						color = { 0,0,0,255 };
					}
					// TODO BUG!!!!
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
					{
						App->renderer->AddLineRenderQueue(iPoint{ METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y) }, iPoint{ METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y) },
							g->gameObject->adjustToGrid, color, 3, 100);					
					}
					prev = v;
				
				}
				PhysBody* bb = (PhysBody*)f->GetBody()->GetUserData();
				if (bb->chainLoop)
				{
					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->AddLineRenderQueue(iPoint{ METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y) }, iPoint{ METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y) },
						false, color, 3, 100);
				}
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				App->renderer->AddLineRenderQueue(iPoint{ METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y) }, iPoint{ METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y) },
					false, SDL_Color{ 100, 100, 255, 255}, 3, 100);
			}
			break;
			}
		}
	}
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

/// <summary>
/// Get pos (0,0)
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void PhysBody::GetPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

b2Vec2 PhysBody::GetPosition() 
{
	b2Vec2 pos = body->GetPosition();

	pos.x = METERS_TO_PIXELS(pos.x);
	pos.y = METERS_TO_PIXELS(pos.y);

	return pos;
}

void PhysBody::GetCenterPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();

	x = METERS_TO_PIXELS(pos.x);
	y = METERS_TO_PIXELS(pos.y);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Fixture* f = body->GetFixtureList();

	b2Vec2 b = { PIXELS_TO_METER(x),PIXELS_TO_METER(y) };

	for (int i = 0; f != nullptr; f = f->GetNext())
	{
		if (f->GetShape()->TestPoint(body->GetTransform(),b))
		{
			return true;
		}
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	b2Fixture* f = body->GetFixtureList();

	b2RayCastInput input;
	input.p1 = b2Vec2(PIXELS_TO_METER(x1), PIXELS_TO_METER(y1));
	input.p2 = b2Vec2(PIXELS_TO_METER(x2), PIXELS_TO_METER(y2));

	// multiplo de distancia entre p1 y p2
	// ej distancia = 2,  maxFraction = 2,  distancia de deteccion = 2*2 = 4
	input.maxFraction = 1.0f;

	b2RayCastOutput* output = nullptr;

	for (int i = 0; f != nullptr; f = f->GetNext())
	{
		f->RayCast(output, input, 0);

		if (output != nullptr)
		{
			normal_x = output->normal.x;
			normal_y = output->normal.y;

			float distance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));

			distance *= output->fraction;

			delete output;

			return distance;
		}
	}

	return -1;
}
