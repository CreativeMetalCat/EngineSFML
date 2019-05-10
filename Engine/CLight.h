#pragma once
#include "Actor.h"
#define CLASS_LIGHT 27
#include <include/lighting/LightSystem.h>
namespace Engine::Light
{
	enum ELightType
	{
		None,
		Point,
		Direction
	};

	//Base class to actor based light object
	//Light itself is made using ltbl2
	class CLight abstract : public CActor
	{
	public:
		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_LIGHT;

		int GetClassID()const { return  CLASS_LIGHT; }
		
		CLight(sf::Vector2f Location, Context* WorldContext, std::string path = "./../");
		~CLight();
	};
}
