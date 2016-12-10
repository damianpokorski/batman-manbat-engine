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
		virtual void Init();
		virtual void Update(float delta);
		virtual void Render2D();
		virtual void Render3D();
		virtual void InputEvent(IEvent * e);
		// Menu states
		enum MenuStates
		{
			Character_Select,		// Cycling characters with LEFT / RIGHT
			Character_Edit,			// Selecting a stat to edit
			Name_Edit,				// Editing name
			Primary_Class_Edit,		// Left / Right - Selecting primary class
			Secondary_Class_Edit,	// Left / Right - Selecting secondary class
			Character_Save,			// Saves the character preset
			Complete				// Finishing the menu
		};
		MenuStates MenuState = MenuStates::Character_Select;

		// Character stats struct - why make it a class, if its only holding data?
		struct CharacterStats
		{
			std::string Name = "New";
			int Health = 60;
			int Accuracy = 60;
			int Armour = 60;
			int Speed = 60;
			int Strength = 60;
			int Wins = 0;
			std::string PrimaryClass;
			std::string SecondaryClass;
		};
		CharacterStats CharacterStat;
		// Character loader properties
		int characterNumber = 0;
		int selectedCharacter = 0;
		tinyxml2::XMLDocument XMLDoc;
		Timer MenuTimer;
		BitmapFont* bmpFont;
		std::vector<std::string> characterClasses;
		std::vector<Sprite*> hats;
		std::vector<Sprite*> props;
		Sprite* menuScreen;
		cSound* menuTheme;
		cSound* cursorFX;

		// Flags
		int  StatFlag = 0; // Ints are not really flags are they?
		bool  ShiftFlag = false;
		int  ClassFlag = 0;
		bool  NewFlag = false;

		// Character loader functionality
		std::string Render();
		// Crud operations
		void Reload();
		bool SaveCharacter();
		bool LoadCharacter();
		bool DeleteCharacter();
		// 
		void UpdateStats();
		void Navigate(int navigateValue);
		void HandleInput(int input);
		// navigate

		// Convert to alpha
		char ConvertToAlpha(int input, bool caps = false);
	};
};