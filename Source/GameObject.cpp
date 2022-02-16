#include "GameObject.h"
#include "ModulePhysics.h"

GameObject::GameObject()
{
}

//TODO: Poner Applicationi* en constructor
GameObject::GameObject(std::string name, std::string tag , Application* _app)
{
	this->name = name;
	this->_app = _app;
	this->tag = tag;
}

GameObject::GameObject(GameObject& obj)
{
	this->name = obj.name;
	this->tag = obj.tag;
	this->pBody = obj.pBody;
}

GameObject::~GameObject()
{
	if(pBody != nullptr)
	{
		delete pBody;
		pBody = nullptr;
	}
}

void GameObject::OnCollisionEnter(PhysBody* col)
{
}

void GameObject::OnCollisionExit(PhysBody* col)
{

}

void GameObject::OnTriggerEnter(PhysBody* trigger, PhysBody* col)
{
}

void GameObject::OnTriggerStay(PhysBody* trigger, PhysBody* col)
{
}

void GameObject::OnTriggerExit(PhysBody* trigger, PhysBody* col)
{
}

void GameObject::Start()
{
	//Cargar texturas
}

void GameObject::PreUpdate()
{
}

void GameObject::Update()
{
}

void GameObject::PostUpdate()
{
	// Dibujar texturas
	for (int i = 0; i < MAX_GAMEOBJECT_TEXTURES; i++)
	{
		if (renderObjects[i].texture != nullptr && renderObjects[i].draw)
		{
			renderObjects[i].destRect.x = GetDrawPosition(i).x;
			renderObjects[i].destRect.y = GetDrawPosition(i).y;
			renderObjects[i].rotation = GetDegreeAngle();
			
			_app->renderer->AddTextureRenderQueue(renderObjects[i].texture, { renderObjects[i].destRect.x,renderObjects[i].destRect.y },
			renderObjects[i].section, renderObjects[i].scale, renderObjects[i].layer, renderObjects[i].orderInLayer,
			renderObjects[i].rotation, renderObjects[i].flip, renderObjects[i].speedRegardCamera);
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
		b2Vec2 position;

		position = pBody->body->GetPosition();

		position.x = METERS_TO_PIXELS(position.x) - pBody->gameObject->renderObjects[index].textureCenterX;
		position.y = METERS_TO_PIXELS(position.y) - pBody->gameObject->renderObjects[index].textureCenterY;

		return { (int)position.x, (int)position.y };
	}

	return this->position;
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
		b2Vec2 position;

		position = pBody->body->GetPosition();
		position.x = METERS_TO_PIXELS(position.x);
		position.y = METERS_TO_PIXELS(position.y);

		return { (int)position.x, (int)position.y };
	}

	return this->position;
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

void GameObject::SetLinearVelocity(iPoint vel)
{
	b2Vec2 v = { (float)vel.x, (float)vel.y };

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
	renderObjects[index].texture = _app->textures->Load(texName, true);
	renderObjects[index].destRect.w = _app->textures->config.child(texName.c_str()).attribute("width").as_int();
	renderObjects[index].destRect.h = _app->textures->config.child(texName.c_str()).attribute("height").as_int();
	renderObjects[index].layer = _app->textures->config.child(texName.c_str()).attribute("layer").as_int(0);
	renderObjects[index].orderInLayer = _app->textures->config.child(texName.c_str()).attribute("orderInLayer").as_float(1.0);
	renderObjects[index].scale = _app->textures->config.child(texName.c_str()).attribute("scale").as_float(1);
	renderObjects[index].section.x = _app->textures->config.child(texName.c_str()).attribute("sectionX").as_int(0);
	renderObjects[index].section.y = _app->textures->config.child(texName.c_str()).attribute("sectionY").as_int(0);
	renderObjects[index].textureCenterX = (renderObjects[index].destRect.w / 2) * renderObjects[index].scale;
	renderObjects[index].textureCenterY = (renderObjects[index].destRect.h / 2) * renderObjects[index].scale;
}