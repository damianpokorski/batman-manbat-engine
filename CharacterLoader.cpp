#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {
	void CharacterLoader::Init()
	{
		// Classes
		characterClasses.push_back("Sheriff");
		characterClasses.push_back("Outlaw");
		characterClasses.push_back("Doctor");
		characterClasses.push_back("Bartender");
		characterClasses.push_back("Drunk");
		// Hats
		hats.push_back(new Sprite());
		hats.back()->Load("hatsnprops/sheriffhat.png");
		hats.push_back(new Sprite());
		hats.back()->Load("hatsnprops/outlawhat.png");
		hats.push_back(new Sprite());
		hats.back()->Load("hatsnprops/doctorhat.png");
		hats.push_back(new Sprite());
		hats.back()->Load("hatsnprops/bartenderhat.png");
		hats.push_back(new Sprite());
		hats.back()->Load("hatsnprops/drunkhat.png");
		// props
		props.push_back(new Sprite());
		props.back()->Load("hatsnprops/sheriffprop.png");
		props.push_back(new Sprite());
		props.back()->Load("hatsnprops/outlawprop.png");
		props.push_back(new Sprite());
		props.back()->Load("hatsnprops/doctorprop.png");
		props.push_back(new Sprite());
		props.back()->Load("hatsnprops/bartenderprop.png");
		props.push_back(new Sprite());
		props.back()->Load("hatsnprops/drunkprop.png");
		// Font
		bmpFont = new BitmapFont();
		bmpFont->Load("western.tga");
		bmpFont->setColumns(16);
		bmpFont->setCharSize(32, 32);
		bmpFont->loadWidthData("western.dat");

		// Menu BG
		menuScreen = new Sprite();
		menuScreen->Load("menuscreenbg.png");
		menuScreen->setPosition(0, 0);
		Content2D.push_back(menuScreen);


		// Cursor fx
		cursorFX = new cSound("cursor.wav");
		Reload();

	}

	void CharacterLoader::Update(float delta)
	{
		// Options
		time += delta;
	}

	void CharacterLoader::Render2D()
	{
		
		std::ostringstream os;
		if(MenuState != MenuStates::Complete)
		{
			for (size_t i = 0; i < Content2D.size(); i++)
			{
				Content2D[i]->Render();
			}
			Color fontColor = Color(100, 10, 10, 200);
			bmpFont->setScale(1.0f);
			int y = 210;

			os << "Number of Characters: ";
			bmpFont->Print(140, 210, os.str(), fontColor);
			os.clear(); os.str("");

			os << characterNumber;
			bmpFont->Print(410, 210, os.str(), fontColor);
			os.clear(); os.str("");

			os << "Current Character: ";
			bmpFont->Print(140, 240, os.str(), fontColor);
			os.clear(); os.str("");

			os << selectedCharacter;
			bmpFont->Print(410, 240, os.str(), fontColor);
			os.clear(); os.str("");
			//notes for where the columns begin
			//x1 = 140 / x2 410 / y1 =29-
			if (selectedCharacter != 0) {
				//vectors for the each display column
				std::vector<std::string>column1;
				std::vector<std::string>column2;

				y = 290;
				/*Column 1 - defined labels */
				column1.push_back("Name: ");
				column1.push_back("Health: ");
				column1.push_back("Accuracy: ");
				column1.push_back("Armour: ");
				column1.push_back("Speed: ");
				column1.push_back("Strength: ");
				column1.push_back("Primary class: ");
				column1.push_back("Secondary class: ");
				column1.push_back("Wins: ");

				column2.push_back(CharacterStat.Name);
				os << CharacterStat.Health;

				column2.push_back(os.str());
				os.clear(); os.str("");
				os << CharacterStat.Accuracy;

				column2.push_back(os.str());
				os.clear(); os.str("");
				os << CharacterStat.Armour;

				column2.push_back(os.str());
				os.clear(); os.str("");
				os << CharacterStat.Speed;

				column2.push_back(os.str());
				os.clear(); os.str("");
				os << CharacterStat.Strength;

				column2.push_back(os.str());
				os.clear(); os.str("");

				column2.push_back(CharacterStat.PrimaryClass);
				column2.push_back(CharacterStat.SecondaryClass);

				os << CharacterStat.Wins;
				column2.push_back(os.str());
				os.clear(); os.str("");

				//loop through columns and display text
				for (int i = 0; i < column1.size(); i++) {

					if (StatFlag == 1 && i == 0 ||
						StatFlag == 2 && i == 6 ||
						StatFlag == 3 && i == 7) {
						fontColor.Set(10, 100, 100, 200);
						if (MenuState == MenuStates::Name_Edit ||
							MenuState == MenuStates::Primary_Class_Edit ||
							MenuState == MenuStates::Secondary_Class_Edit) {
							fontColor.Set(100, 100, 100, 200);
						}

					}
					else {
						fontColor.Set(100, 10, 10, 200);
					}

					bmpFont->Print(140, y, column1[i], fontColor);
					bmpFont->Print(410, y, column2[i], fontColor);
					y += 30;
				}

				for (int i = 0; i < characterClasses.size(); i++) {
					if (CharacterStat.PrimaryClass== characterClasses[i]) {
						hats[i]->setPosition(600, 300);
						hats[i]->Render();
					}
				}

				for (int i = 0; i < characterClasses.size(); i++) {
					if (CharacterStat.SecondaryClass == characterClasses[i]) {
						props[i]->setPosition(740, 320);
						props[i]->Render();
					}
				}

			}
			os << "Menu State: " << " - Stat Flag: " << StatFlag;

			bmpFont->setScale(0.75f);
			bmpFont->Print(140, 655,
				"Navigate: Arrows - Edit: E  -Save: S - Load: L - New: N",
				Color(10, 0, 0, 200));
			bmpFont->Print(140, 625, os.str(), Color(10, 0, 0, 200));
		}

	}

	void CharacterLoader::Render3D()
	{

	}

	void CharacterLoader::InputEvent(IEvent* e) {
		switch (e->GetID()) {
			case EventType::EVENT_KEYRELEASE:
				//Debug << "[Scarlet Skyline][Keyboard Release]" << ConvertToAlpha(((KeyReleaseEvent*)e)->keycode) << std::endl;
				this->HandleInput(((KeyPressEvent*)e)->keycode);
			//case EventType::EVENT_KEYPRESS:
			//	Debug << "[Scarlet Skyline][Keyboard Down]" << ConvertToAlpha(((KeyPressEvent*)e)->keycode) << std::endl;
			//	this->HandleInput(((KeyPressEvent*)e)->keycode);
			break;
			default: break;
		}
	}

	void CharacterLoader::Reload()
	{
		// Load xml file
		XMLDoc.LoadFile("characters.xml");
		characterNumber = 0;
		for(tinyxml2::XMLElement* parent = XMLDoc.FirstChildElement("characters")->FirstChildElement("char"); parent!= NULL; parent = parent->NextSiblingElement())
		{
			Debug << "[XML] Loaded a character '" << parent->FirstChildElement("name")->GetText() << "'" << std::endl;
			characterNumber++;
		}
		// Navigate back and forth to ensure a full refresh.
		Navigate(-1);
		Navigate(1);
	}

	bool CharacterLoader::SaveCharacter()
	{

		std::ostringstream os;

		if (NewFlag) {
			tinyxml2::XMLNode *charnode =
				XMLDoc.NewElement("char");
			XMLDoc.FirstChildElement("characters")->
				InsertEndChild(charnode);
		}

		tinyxml2::XMLNode *nameNode = XMLDoc.NewElement("name");
		const char* nameText =CharacterStat.Name.c_str();

		tinyxml2::XMLNode *healthNode = XMLDoc.NewElement("health");
		os <<CharacterStat.Health;
		std::string health = os.str();
		const char* healthText = health.c_str();
		os.clear(); os.str("");

		tinyxml2::XMLNode *accuracyNode = XMLDoc.NewElement("accuracy");
		os <<CharacterStat.Accuracy;
		std::string accuracy = os.str();
		const char* accuracyText = accuracy.c_str();
		os.clear(); os.str("");

		tinyxml2::XMLNode *armourNode = XMLDoc.NewElement("armour");
		os <<CharacterStat.Armour;
		std::string armour = os.str();
		const char* armourText = armour.c_str();
		os.clear(); os.str("");

		tinyxml2::XMLNode *speedNode = XMLDoc.NewElement("speed");
		os <<CharacterStat.Speed;
		std::string speed = os.str();
		const char* speedText = speed.c_str();
		os.clear(); os.str("");

		tinyxml2::XMLNode *strNode = XMLDoc.NewElement("str");
		os <<CharacterStat.Strength;
		std::string strength = os.str();
		const char* strText = strength.c_str();
		os.clear(); os.str("");

		tinyxml2::XMLNode *pclassNode = XMLDoc.NewElement("pclass");
		const char* pclassText =CharacterStat.PrimaryClass.c_str();

		tinyxml2::XMLNode *sclassNode = XMLDoc.NewElement("sclass");
		const char* sclassText =CharacterStat.SecondaryClass.c_str();

		tinyxml2::XMLNode *winsNode = XMLDoc.NewElement("wins");
		os <<CharacterStat.Wins;
		std::string wins = os.str();
		const char* winsText = wins.c_str();
		os.clear(); os.str("");

		if (NewFlag) {
			tinyxml2::XMLNode *parent =
				XMLDoc.FirstChildElement("characters")
				->LastChildElement("char");

			parent->InsertEndChild(nameNode);
			parent->FirstChildElement("name")->SetText(nameText);

			parent->InsertEndChild(healthNode);
			parent->FirstChildElement("health")->SetText(healthText);

			parent->InsertEndChild(accuracyNode);
			parent->FirstChildElement("accuracy")->SetText(accuracyText);

			parent->InsertEndChild(armourNode);
			parent->FirstChildElement("armour")->SetText(armourText);

			parent->InsertEndChild(speedNode);
			parent->FirstChildElement("speed")->SetText(speedText);

			parent->InsertEndChild(strNode);
			parent->FirstChildElement("str")->SetText(strText);

			parent->InsertEndChild(pclassNode);
			parent->FirstChildElement("pclass")->SetText(pclassText);

			parent->InsertEndChild(sclassNode);
			parent->FirstChildElement("sclass")->SetText(sclassText);

			parent->InsertEndChild(winsNode);
			parent->FirstChildElement("wins")->SetText(winsText);

			NewFlag = false;
		}
		else {
			int counter = 0;

			for (tinyxml2::XMLElement*parent =
				XMLDoc.FirstChildElement("characters")
				->FirstChildElement("char");
				parent != NULL;
				parent = parent->NextSiblingElement()) {

				counter++;

				if (counter == selectedCharacter) {

					parent->DeleteChildren();

					parent->InsertEndChild(nameNode);
					parent->FirstChildElement("name")->SetText(nameText);

					parent->InsertEndChild(healthNode);
					parent->FirstChildElement("health")->SetText(healthText);

					parent->InsertEndChild(accuracyNode);
					parent->FirstChildElement("accuracy")->SetText(accuracyText);

					parent->InsertEndChild(armourNode);
					parent->FirstChildElement("armour")->SetText(armourText);

					parent->InsertEndChild(speedNode);
					parent->FirstChildElement("speed")->SetText(speedText);

					parent->InsertEndChild(strNode);
					parent->FirstChildElement("str")->SetText(strText);

					parent->InsertEndChild(pclassNode);
					parent->FirstChildElement("pclass")->SetText(pclassText);

					parent->InsertEndChild(sclassNode);
					parent->FirstChildElement("sclass")->SetText(sclassText);

					parent->InsertEndChild(winsNode);
					parent->FirstChildElement("wins")->SetText(winsText);

				}
			}
		}
		XMLDoc.SaveFile("characters.xml");

		return true;
	}

	bool CharacterLoader::LoadCharacter()
	{
		return true;
	}

	bool CharacterLoader::DeleteCharacter()
	{

		int counter = 0;
		for (
			tinyxml2::XMLElement* parent =
			XMLDoc.FirstChildElement("characters")
			->FirstChildElement("char");

			parent != NULL;

			parent = parent->NextSiblingElement()) {
			counter++;

			if (counter == selectedCharacter) {
				XMLDoc.DeleteNode(parent);
				break;
			}
		}
		XMLDoc.SaveFile("characters.xml");

		Reload();
		return true;
	}

	void CharacterLoader::UpdateStats()
	{

		CharacterStat.Accuracy = 60;
		CharacterStat.Armour = 60;
		CharacterStat.Health = 60;
		CharacterStat.Speed = 60;
		CharacterStat.Strength = 60;

		//Primary class
		if (CharacterStat.PrimaryClass == "Outlaw") {
			CharacterStat.Speed += 25;
			CharacterStat.Health -= 25;
		}

		if (CharacterStat.PrimaryClass == "Sheriff") {
			CharacterStat.Accuracy += 25;
			CharacterStat.Armour -= 25;
		}

		if (CharacterStat.PrimaryClass == "Doctor") {
			CharacterStat.Health += 25;
			CharacterStat.Strength -= 25;
		}

		if (CharacterStat.PrimaryClass == "Bartender") {
			CharacterStat.Strength += 25;
			CharacterStat.Speed -= 25;
		}

		if (CharacterStat.PrimaryClass == "Drunk") {
			CharacterStat.Armour += 25;
			CharacterStat.Accuracy -= 25;
		}

		//Secondary class
		if (CharacterStat.SecondaryClass == "Outlaw") {
			CharacterStat.Speed += 12;
			CharacterStat.Health -= 12;
		}

		if (CharacterStat.SecondaryClass == "Sheriff") {
			CharacterStat.Accuracy += 12;
			CharacterStat.Armour -= 12;
		}

		if (CharacterStat.SecondaryClass == "Doctor") {
			CharacterStat.Health += 12;
			CharacterStat.Strength -= 12;
		}

		if (CharacterStat.SecondaryClass == "Bartender") {
			CharacterStat.Strength += 12;
			CharacterStat.Speed -= 12;
		}

		if (CharacterStat.SecondaryClass == "Drunk") {
			CharacterStat.Armour += 12;
			CharacterStat.Accuracy -= 12;
		}
	}

	void CharacterLoader::Navigate(int navigateValue)
	{
		selectedCharacter += navigateValue;
		if (selectedCharacter < 1) {
			selectedCharacter = characterNumber;
		}
		if (selectedCharacter > characterNumber) {
			selectedCharacter = 1;
		}
		int counter = 0;
		for (tinyxml2::XMLElement* parent =
			XMLDoc.FirstChildElement("characters")
			->FirstChildElement("char");
			parent != NULL;
			parent = parent->NextSiblingElement()) {
			counter++;
			if (counter == selectedCharacter) {

				CharacterStat.Name= parent->FirstChildElement("name")->GetText();
				CharacterStat.Health = std::stoi(parent->FirstChildElement("health")->GetText());
				CharacterStat.Accuracy = std::stoi(parent->FirstChildElement("accuracy")->GetText());
				CharacterStat.Armour = std::stoi(parent->FirstChildElement("armour")->GetText());
				CharacterStat.Speed = std::stoi(parent->FirstChildElement("speed")->GetText());
				CharacterStat.Strength = std::stoi(parent->FirstChildElement("str")->GetText());
				CharacterStat.PrimaryClass = parent->FirstChildElement("pclass")->GetText();
				CharacterStat.SecondaryClass =parent->FirstChildElement("sclass")->GetText();
				CharacterStat.Wins = std::stoi(parent->FirstChildElement("wins")->GetText());
			}
		}
	}

	void CharacterLoader::HandleInput(int input)
	{
		//if (time < 0.25f)
		//{
		//	return;
		//}
		//time -= 0.25f;
		switch (MenuState) {
		case MenuStates::Character_Select: {
			//navigate left / right 
			if (input == DIK_RIGHT) Navigate(1);
			if (input == DIK_LEFT) Navigate(-1);
			//switch
			if (input == DIK_E) {
				MenuState = MenuStates::Character_Edit;
				StatFlag = true;
			}
			//switch to menu  complete (load)
			if (input == DIK_L) MenuState = MenuStates::Complete;

			if (input == DIK_D) DeleteCharacter();

			if (input == DIK_N) {

				characterNumber++;
				selectedCharacter = characterNumber;

				CharacterStat.Name = "Outsider";
				CharacterStat.PrimaryClass = "Outlaw";
				CharacterStat.SecondaryClass = "Bartender";
				CharacterStat.Wins = 0;

				UpdateStats();
				MenuState = MenuStates::Character_Edit;
				StatFlag = 1;

				NewFlag = true;
				cursorFX->Play();
			}

		}
								break;
		case MenuStates::Character_Edit:
		{
			//up / down to cycle through stat flag
			if (input == DIK_DOWN) {
				StatFlag++; 
				if (StatFlag > 3)
				{
					StatFlag = 1;
				}
				cursorFX->Play();
			}
			if (input == DIK_UP) {
				StatFlag--; 
				if (StatFlag < 1)
				{
					StatFlag = 3;
				}
				cursorFX->Play();
			}
			Debug << "[Character][StatFlag] Changed to:" << StatFlag;
			if (input == DIK_E || input == DIK_RETURN) {
				if (StatFlag == 1) {
					MenuState = MenuStates::Name_Edit;
					cursorFX->Play();
				}

				if (StatFlag == 2) {
					MenuState = MenuStates::Primary_Class_Edit;
					ClassFlag = 1;
					cursorFX->Play();
					CharacterStat.PrimaryClass = characterClasses[ClassFlag];
					UpdateStats();
				}

				if (StatFlag == 3) {
					MenuState = MenuStates::Secondary_Class_Edit;
					ClassFlag = 1;
					cursorFX->Play();
					CharacterStat.SecondaryClass = characterClasses[ClassFlag];
					UpdateStats();
				}
			}
			if (input == DIK_S) {
				MenuState = MenuStates::Character_Select; 
				SaveCharacter(); 
				StatFlag = 0;
				cursorFX->Play();
			}
		}
		break;
		case MenuStates::Name_Edit:
		{
			std::ostringstream nameStream;
			if (CharacterStat.Name.size() < 20) {
				nameStream << CharacterStat.Name;
				nameStream << ConvertToAlpha(input, ShiftFlag);
				CharacterStat.Name = nameStream.str();
			}
			if (input == DIK_LEFT || input == DIK_BACK) {
				if (CharacterStat.Name.size() > 0) {
					//CharacterStat.Name.pop_back();
					CharacterStat.Name = CharacterStat.Name.substr(0, CharacterStat.Name.size() - 2);
					Debug << "[Character][StatFlag] Changed to:" << StatFlag;
					cursorFX->Play();
				}
			}
			if (input == DIK_RETURN) {
				MenuState = MenuStates::Character_Edit;
				cursorFX->Play();
			}
		}
		break;
		case MenuStates::Primary_Class_Edit:
		{
			if (input == DIK_LEFT) {
				ClassFlag--;
				if (ClassFlag < 0) ClassFlag = 4;
				CharacterStat.PrimaryClass = characterClasses[ClassFlag];
				UpdateStats();
				cursorFX->Play();
				Debug << "Primary class now is " << characterClasses[ClassFlag] << std::endl;
			}
			if (input == DIK_RIGHT) {
				ClassFlag++;
				if (ClassFlag >4) ClassFlag = 0;
				CharacterStat.PrimaryClass = characterClasses[ClassFlag];
				UpdateStats();
				cursorFX->Play();
				Debug << "Primary class now is " << characterClasses[ClassFlag] << std::endl;
			}
			if (input == DIK_RETURN) {
				MenuState = MenuStates::Character_Edit;
				cursorFX->Play();
			}

		}
		break;
		case MenuStates::Secondary_Class_Edit:
		{
			if (input == DIK_LEFT) {
				ClassFlag--;
				if (ClassFlag < 0) ClassFlag = 4;
				CharacterStat.SecondaryClass = characterClasses[ClassFlag];
				UpdateStats();
				cursorFX->Play();
				Debug << "Secondary class now is " << characterClasses[ClassFlag] << std::endl;
			}
			if (input == DIK_RIGHT) {
				ClassFlag++;
				if (ClassFlag >4) ClassFlag = 0;
				CharacterStat.SecondaryClass = characterClasses[ClassFlag];
				UpdateStats();
				cursorFX->Play();
				Debug << "Secondary class now is " << characterClasses[ClassFlag] << std::endl;
			}
			if (input == DIK_RETURN) {
				MenuState = MenuStates::Character_Edit;
				cursorFX->Play();
			}
		}
		break;
		}
	}

	char CharacterLoader::ConvertToAlpha(int input, bool caps)
	{
		int m_DIK[26] = { 
			DIK_A, DIK_B, DIK_C, DIK_D, DIK_E, DIK_F, DIK_G, DIK_H, DIK_I, DIK_J,
			DIK_K, DIK_L, DIK_M, DIK_N, DIK_O, DIK_P, DIK_Q, DIK_R, DIK_S, DIK_T,
			DIK_U, DIK_V, DIK_W, DIK_X, DIK_Y, DIK_Z };
		char m_alpha[26] = {
			'A','B' , 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
			'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
			'U', 'V' ,'W' ,'X' ,'Y' ,'Z' };
		for(int i = 0; i<26; i++)
		{
			if(m_DIK[i]==input)
			{
				return m_alpha[i];
			}
		}
		return ' ';
	};
};