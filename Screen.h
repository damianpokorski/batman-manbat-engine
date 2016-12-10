#pragma once

#include "Engine.h"

namespace Manbat {

	class Screen
	{
		public:
			enum ScreenEnum{
				none,
				startScreen,
				loadingScreen,
				gameLogic,
				levelSelectionScreen,
				pauseScreen,
				creditsScreen
			};
			ScreenEnum target;
			ScreenEnum self;
			bool isActive;

			Screen();

			std::vector < Sprite*> Content2D;
			std::vector <ImprovedMesh*> Content3D;

			Scene* Level;
			// Initializes the level
			void virtual Init() = 0;
			// Handles updates
			void virtual Update(float delta) = 0;
			// Handles rendering of 3d components
			void virtual Render3D()=0;
			// 
			void virtual Render2D()=0;
			virtual void Deactivate(){};
			void virtual InputEvent(IEvent* e) = 0;
			
			float time;
			enum AnimationRepeat {
				None,
				PingPong
			};
			std::map<std::string, float> AnimationCache;
			float animationValue(std::string animationID, float duration, AnimationRepeat loop) {
				// check add animation if it ddoesn't exist
				if (AnimationCache.find(animationID) == AnimationCache.end()) {
					AnimationCache.insert(std::pair<std::string, float>(animationID, time));
				}
				// Not that i'm sure the animation is within the map - proceed to do the math
				switch (loop) {
				case None:
					// if animation start + duration < current time -  animation is finished
					if ((AnimationCache.find(animationID)->second + duration) < time) {
						return 1;
					}
					else {
						// return timepassed / duration = animation progress.
						return Math::roundf(((time - AnimationCache.find(animationID)->second) / duration)*100)/100;
					}
					break;
				case PingPong:
					// still on the first loop
					if ((AnimationCache.find(animationID)->second + duration) > time) {
						return (time - AnimationCache.find(animationID)->second) / duration;
					}// Animation loops
					else {
						float direction = 1.0f;
						float timehandled = time;
						while (timehandled > (AnimationCache.find(animationID)->second + duration)) {
							// if -1 = 1 if 1 = -1
							direction = direction*-1;
							timehandled -= duration;
						}
						if (direction == 1) {
							return Math::roundf(((timehandled - AnimationCache.find(animationID)->second) / duration)*100)/100;
						}
						else {
							return Math::roundf((1 - (timehandled - AnimationCache.find(animationID)->second) / duration)*100)/100;
						}

					}
					break;
				}
				return 0;
			}
			// reset animation to current time
			void animationReset(std::string animationID) {
				if (AnimationCache.find(animationID) != AnimationCache.end()) {
					AnimationCache.find(animationID)->second = time;
				}
			};
			// use animation as timer... since its easier than boost one.
			bool animationAsTimer(std::string animationID, float duration, bool initTick = false) {
				float temp = animationValue(animationID, duration, None);
				if (temp == 1.0f) {
					animationReset(animationID);
					return true;
				}
				else {
					if (temp == 0 && initTick) {
						return true;
					}
					return false;
				}
			};
	};
};