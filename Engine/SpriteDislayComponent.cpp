#include "SpriteDislayComponent.h"



namespace Engine::Component
{
	void CSpriteDisplayComponent::RegisterClassLUA(lua_State*& L)
	{
		using namespace luabridge;
		try
		{
			//Register CActor in lua
			getGlobalNamespace(L)
				.beginClass<sf::Sprite>("Sprite")
				.addConstructor<void(*)(void)>()
				.endClass();

			Engine::Component::CComponent::RegisterClassLUA(L);

			getGlobalNamespace(L)
				.deriveClass<CSpriteDisplayComponent,Engine::Component::CComponent>("CSpriteDisplayComponent")
				.addConstructor<void(*)(sf::Sprite sprite,Context * WorldContext, std::string path)>()
				.addFunction("GetClassID", &CSpriteDisplayComponent::GetClassID)
				.addFunction("Init", &CSpriteDisplayComponent::Init)
				.addFunction("LoadSprite",&CSpriteDisplayComponent::LoadSprite)
				.addFunction("AddToActor", &CSpriteDisplayComponent::AddToActor)
				.endClass();

			
		}
		catch (LuaException e)
		{
			std::cout << "Failed to register class in LUA" << std::endl;
		}
		catch (std::exception e)
		{
			std::cout << "Failed to register class in LUA " << e.what() << std::endl;
		}
	}


	CSpriteDisplayComponent::CSpriteDisplayComponent(sf::Sprite sprite, Context* WorldContext, std::string path):CComponent(WorldContext,path)
	{
		this->SetSprite(sprite);
	}

	sf::Vector2f CSpriteDisplayComponent::GetWorldLocation() const
	{
		if (this->IsOwnerValid())
		{
			return owner->GetActorLocation() + this->m_relativeLocation;
		}
		else
		{
			return sf::Vector2f();
		}
		
	}

	void CSpriteDisplayComponent::SetSpriteTexture(sf::Texture& texture)
	{
		this->m_sprite.setTexture(texture);
	}

	void CSpriteDisplayComponent::Draw(sf::RenderWindow& window)
	{
		window.draw(this->m_sprite);
	}

	bool CSpriteDisplayComponent::LoadSprite(std::string name)
	{
		if (this->WorldContext != nullptr)
		{
			this->m_sprite.setTexture(this->WorldContext->TextureResources->GetTextureByName(name)->m_texture);
		}
		else
		{
			return false;
		}
	}

	void CSpriteDisplayComponent::Update(sf::Time)
	{
	}

	CSpriteDisplayComponent::~CSpriteDisplayComponent()
	{
	}
}