#include "Engine.h"

using namespace std;
namespace Manbat {

	class StartScreen: public Screen
	{
	public:

		int CurrentOption;
		StartScreen() {
			isActive = false;
			self = ScreenEnum::startScreen;
			target = ScreenEnum::none;
			CurrentOption = 0;
			time = 0;
			// Background
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/StartingScreen.png")) {
				Debug << "[Start Screen] Error creating BG in StartScreen" << std::endl;
			}
			Content2D.back()->setPosition(0, 0);
			// Start Game
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/StartingScreen_StartGame.png")) {
				Debug << "[Start Screen] Error creating StartingScreen_StartGame in StartScreen" << std::endl;
			}
			Content2D.back()->setPosition(400, 350);
			// Quit Game
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/StartingScreen_Quit.png")) {
				Debug << "[Start Screen] Error creating StartingScreen_Quit in StartScreen" << std::endl;
			}
			Content2D.back()->setPosition(450, 475);
			// Current Option indicator - "square"
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/StartingScreen_Square.png")) {
				Debug << "[Start Screen] Error creating StartingScreen_Square in StartScreen" << std::endl;
			}
			Content2D.back()->setPosition(450, 475);
			Content2D.back()->setPivot(Vector2(50, 50));
			
		};

		virtual void Init() {

		};
		virtual void Update(float delta) {
			// Options
			time += delta;

			Content2D[1]->setPosition(400 + 25 * animationValue("MenuOptions", 1, AnimationRepeat::PingPong), 350);
			Content2D[2]->setPosition(450 - 25 * animationValue("MenuOptions", 1, AnimationRepeat::PingPong), 475);
			// Square selection
			Content2D[3]->setPosition(Content2D[CurrentOption + 1]->getPosition().getX() - 125, Content2D[CurrentOption + 1]->getPosition().getY()-25);
			Content2D[3]->setRotation(Content2D[3]->getRotation() + delta * 5);
		};
		virtual void Render2D() {
			for (size_t i = 0; i < Content2D.size(); i++) {
				Content2D[i]->Render();
			}
			
		};
		virtual void Render3D() {
				
		};
		
		virtual void InputEvent(IEvent * e);

	};
};