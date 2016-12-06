#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {
	void CharacterLoader::Init()
	{
	}

	void CharacterLoader::Update(float delta)
	{
		// Options
		time += delta;
	}

	void CharacterLoader::Render2D()
	{
		for (size_t i = 0; i < Content2D.size(); i++)
		{
			Content2D[i]->Render();
		}
	}

	void CharacterLoader::Render3D()
	{

	}

	void CharacterLoader::InputEvent(IEvent* e) {

	};
};