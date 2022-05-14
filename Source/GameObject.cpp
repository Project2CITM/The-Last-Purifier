#include "GameObject.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "Scene.h"

GameObject::GameObject()
{
}

//TODO: Poner Applicationi* en constructor
GameObject::GameObject(std::string name, std::string tag)
{
	this->name = name;
	this->app = Application::GetInstance();
	this->tag = tag;
	app->scene->scenes[app->scene->currentScene]->AddGameObject(this);
}

GameObject::GameObject(GameObject& obj)
{
	this->name = obj.name;
	this->tag = obj.tag;
	this->pBody = obj.pBody;
}

GameObject::~GameObject()
{
	if(pBody != nullptr) RELEASE(pBody);
}

void GameObject::OnCollisionEnter(PhysBody* col)
{
}

void GameObject::OnCollisionExit(PhysBody* col)
{

}

void GameObject::OnTriggerEnter(std::string trigger, PhysBody* col)
{
}

void GameObject::OnTriggerStay(std::string trigger, PhysBody* col)
{
}

void GameObject::OnTriggerExit(std::string trigger, PhysBody* col)
{
}

void GameObject::Start()
{
}

void GameObject::PreUpdate()
{
}

void GameObject::Update()
{
	// Update position use b2Body, is this exist
	position = GetPosition();
}

void GameObject::PostUpdate()
{
	// Dibujar texturas
	for (int i = 0; i < MAX_GAMEOBJECT_TEXTURES; i++)
	{
		if (!renderObjects[i].draw) continue;

		switch (renderObjects[i].type)
		{
		case RenderType::RENDER_TEXTURE:
			if (renderObjects[i].texture != nullptr)
			{
				UpdateOrderInLayer(i);
				renderObjects[i].destRect.x = GetDrawPosition(i).x;
				renderObjects[i].destRect.y = GetDrawPosition(i).y;
				renderObjects[i].rotation = GetDegreeAngle();

				app->renderer->AddRenderObjectRenderQueue(renderObjects[i]);
			}
			break;
		case RenderType::RENDER_CIRCLE:
		case RenderType::RENDER_LINE:
		case RenderType::RENDER_RECT:
			app->renderer->AddRenderObjectRenderQueue(renderObjects[i]);
			break;	
		}		
	}
}

void GameObject::CleanUp()
{
}

bool GameObject::CompareTag(std::string tag)
{
	if(this->tag == tag)
	{
		return true;
	}

	return false;
}

iPoint GameObject::GetDrawPosition(int index)
{
	if (pBody != nullptr)
	{
		b2Vec2 pos;

		pos = pBody->body->GetPosition();

		pos.x = METERS_TO_PIXELS(pos.x) - (int)((float)renderObjects[index].textureCenterX * renderObjects[index].scale);
		pos.y = METERS_TO_PIXELS(pos.y) - (int)((float)renderObjects[index].textureCenterY * renderObjects[index].scale);

		return { (int)pos.x, (int)pos.y };
	}

	return position;
}

void GameObject::UpdateOrderInLayer(int index)
{
	if (!renderObjects[index].orderOnHeight) return;
	if (pBody != nullptr)
	{
		renderObjects[index].orderInLayer = GetPosition().y + pBody->height;
	}
	else
	{
		renderObjects[index].orderInLayer = GetPosition().y + renderObjects[index].destRect.h;
	}
}

float GameObject::GetDegreeAngle()
{
	if (pBody != nullptr)
	{
		float agle = 0;

		agle = (pBody->body->GetAngle() * 180) / b2_pi;

		return agle;
	}
	
	return this->rotation;
}

iPoint GameObject::GetPosition()
{
	if (pBody != nullptr)
	{
		b2Vec2 pos;

		pos = pBody->body->GetPosition();
		pos.x = METERS_TO_PIXELS(pos.x);
		pos.y = METERS_TO_PIXELS(pos.y);

		return { (int)pos.x, (int)pos.y };
	}

	return this->position;
}

iPoint GameObject::GetScreenPosition()
{
	iPoint pos = GetPosition();
	pos.x = (int)(-app->renderer->camera->x + pos.x * app->window->scale);
	pos.y = (int)(-app->renderer->camera->y + pos.y * app->window->scale);
	return pos;
}

b2Vec2 GameObject::GetLinearVelocity()
{
	if (pBody != nullptr)
	{
		return pBody->body->GetLinearVelocity();
	}

	return { 0,0 };
}

void GameObject::SetPosition(iPoint pos)
{
	if (pBody != nullptr)
	{
		pBody->body->SetTransform(b2Vec2((PIXELS_TO_METER(pos.x)), (PIXELS_TO_METER(pos.y))), pBody->body->GetAngle());
	}
	else
	{
		this->position = pos;
	}
}

void GameObject::SetRotation(float angle)
{
	if (pBody != nullptr)
	{
		pBody->body->SetTransform(pBody->body->GetPosition(), DEGTORAD * angle);
	}
	else
	{
		this->rotation = angle;
	}
}

void GameObject::SetLinearVelocity(b2Vec2 vel)
{
	if (pBody != nullptr)
	{
		pBody->body->SetLinearVelocity(vel);
	}
	else
	{
		printf_s("%s don't have pBody!!!\n", name);
	}
}

void GameObject::SetLinearVelocity(fPoint vel)
{
	b2Vec2 v = { vel.x, vel.y };

	if (pBody != nullptr)
	{
		pBody->body->SetLinearVelocity(v);
	}
	else
	{
		printf_s("%s don't have pBody!!!\n", name);
	}
}

void GameObject::InitRenderObjectWithXml(std::string texName, int index)
{
	if (texName == "null") texName = name;

	renderObjects[index].name = texName;
	renderObjects[index].texture = app->textures->Load(texName, true);
	renderObjects[index].destRect.w = app->textures->config.child(texName.c_str()).attribute("width").as_int();
	renderObjects[index].destRect.h = app->textures->config.child(texName.c_str()).attribute("height").as_int();
	renderObjects[index].layer = app->textures->config.child(texName.c_str()).attribute("layer").as_int(0);
	renderObjects[index].orderInLayer = app->textures->config.child(texName.c_str()).attribute("orderInLayer").as_float(1.0);
	renderObjects[index].scale = app->textures->config.child(texName.c_str()).attribute("scale").as_float(1);
	renderObjects[index].section.x = app->textures->config.child(texName.c_str()).attribute("sectionX").as_int(0);
	renderObjects[index].section.y = app->textures->config.child(texName.c_str()).attribute("sectionY").as_int(0);
	renderObjects[index].textureCenterX = (renderObjects[index].destRect.w / 2) * renderObjects[index].scale;
	renderObjects[index].textureCenterY = (renderObjects[index].destRect.h / 2) * renderObjects[index].scale;
}