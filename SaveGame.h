#pragma once
#include "Engine.h"

namespace Manbat {
	class SaveGame {
		public:
		static std::map<std::string, bool> LoadGame() {
			std::map<std::string, bool> saveData;
				
			std::ifstream level("SaveData.txt");
				
			if (level.is_open())
			{
				std::string line;
				int y = 0;

				std::string levelName;
				bool unlocked;

				while (level.good())
				{
					getline(level, line);
					Debug << line << std::endl;
					if (y % 2 == 0) {
						levelName = line;
					}
					else {
						unlocked = (line == "true");
						saveData.insert(std::pair<std::string, bool>(levelName, unlocked));
					}
					y++;
				}
				level.close();
			}
			else
			{
				Debug << "Unable to open file" << std::endl << std::endl;
			}
			return saveData;
		};
		static bool IsLevelUnlocked(std::string LevelName) {
			if (LevelName == "Tutorial") {
				return true;
			}
			else {
				std::map<std::string, bool> saveData = SaveGame::LoadGame();
				if (saveData.find(LevelName) != saveData.end()) {
					return saveData.find(LevelName)->second;
				}
			}
			return false;
		};
		static void UnlockLevel(std::string LevelName) {
			std::map<std::string, bool> saveData = SaveGame::LoadGame();
			std::ofstream SaveFile;
			//SaveFile.open("SaveData.txt", ios::out | ios::trunc | ios::binary);
			SaveFile.open("SaveData.txt");
			for (std::map<std::string, bool>::iterator i = saveData.begin(); i != saveData.end(); ++i) {
				SaveFile << i->first << std::endl;
				if (i->first == LevelName) {
					SaveFile << "true" << std::endl;
				}
				else {
					if (i->second) {
						SaveFile << "true" << std::endl;
					}
					else {
						SaveFile << "false" << std::endl;
					}
				}
			}
			SaveFile.close();
		};
		static void ResetProgress() {
			std::map<std::string, bool> saveData = SaveGame::LoadGame();
			std::ofstream SaveFile;
			//SaveFile.open("SaveData.txt", ios::out | ios::trunc | ios::binary);
			SaveFile.open("SaveData.txt");
			for (std::map<std::string, bool>::iterator i = saveData.begin(); i != saveData.end(); ++i) {
				SaveFile << i->first << std::endl;
				SaveFile << "false" << std::endl;
			}
			SaveFile.close();
		}
	};
}
