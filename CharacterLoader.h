#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {

	class CharacterLoader : public Screen
	{
	public:

		int CurrentOption;
		CharacterLoader() {
			isActive = false;
			self = ScreenEnum::startScreen;
			target = ScreenEnum::none;
			CurrentOption = 0;
			time = 0;
		};
		// Virtual functions overloading from screen
		virtual void Init();;
		virtual void Update(float delta);;
		virtual void Render2D();;
		virtual void Render3D();;
		virtual void InputEvent(IEvent * e);
		// Character stats struct - why make it a class?
		struct CharacterStats
		{
			std::string Name;
			int Health;
			int Accuracy;
			int Armour;
			int Speed;
			int Strength;
			int Wins;
		};
	};
};