#include "stdafx.h"
#include "LightFactory.h"
#include "tinyxml.h"
#include "RevStructures.h"
LightFactory::LightFactory()
{
}


LightFactory::~LightFactory()
{
}

#pragma optimize("", off )

bool LightFactory::GenerateLights(const char* filePath)
{

	TiXmlDocument doc;
	if (!doc.LoadFile(filePath))
	{
		assert(0 && "FAILED LOADING GAME INSTACES");
		return false;
	}

	TiXmlNode* node = doc.FirstChild("lights");

	TiXmlElement* element = node->FirstChildElement();

	for (; element; element = element->NextSiblingElement())
	{
		float radius;
		float x, y, z;
		float r, g, b;
		
		element->QueryFloatAttribute("falloff", &radius);
		
		element->QueryFloatAttribute("color-r", &r);
		element->QueryFloatAttribute("color-g", &g);
		element->QueryFloatAttribute("color-b", &b);

		element->QueryFloatAttribute("pos-x", &x);
		element->QueryFloatAttribute("pos-y", &y);
		element->QueryFloatAttribute("pos-z", &z);
	

		RevPointLight pLight;
		pLight.myPos = CU::Vector3f(x, y, z);
		pLight.myColor = CU::Vector4f(r, g, b, 1);
		pLight.myRadius = radius;

		RevEngineMain::AddLightInstance(pLight);
	}

	return true;
}

#pragma optimize("", on )