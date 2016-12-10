#include "Engine.h"

using namespace std;

std::auto_ptr<Manbat::Engine> g_engine(new Manbat::Engine);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	if(msg == WM_DESTROY) PostQuitMessage(0);
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

int WINAPI  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	Debug << "[Winmain] Winmain is running" << std::endl;
	srand((unsigned int)time(0));
	Debug << "[Winmain] Checking Parameters" << std::endl;
	if(strlen(lpCmdLine) > 0){
		g_engine->SetCommandLineParams(lpCmdLine);
		Debug << "[Winmain] Params : " << g_engine->GetCommandLineParams() << std::endl;
	}
	Debug << "[Winmain] Preloading..." << std::endl;
	if(!game_preload()){
		Debug << "[Winmain] Couldn't preload.. Q_Q" << std::endl;
		return 0;
	}
	Debug << "[Winmain] Initialising..." << std::endl;
	bool res = g_engine->Init(hInstance,
		g_engine->GetScreenWidth(),
		g_engine->GetScreenHeight(),
		g_engine->GetColorDepth(),
		g_engine->GetFullscreen()
		);
	if(!res){
		Debug << "[Winmain] Couldn't initialise." << std::endl;
		return 0;
	}

	MSG msg;
	memset(&msg,0,sizeof(MSG));
	Manbat::Timer timer;
	//double startTime = timer.GetElapsed();
	double startTime = timer.getSystemTime();
	Debug << "[Winmain] Timer started : " << timer.GetElapsed() << std::endl;
	Debug << "[Winmain] Entering while loop" << std::endl;
	float t;
	float deltaTime = 0.01f;
	while(msg.message!=WM_QUIT){
		if(PeekMessage(&msg,NULL, 0,0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			t = timer.getSystemTime();
			g_engine->Update((float)(t - startTime));
			//Debug << "[Engine][FPS] " << 1.0f/(t - startTime) << std::endl;
			startTime = t;
		}
	}
	Debug << "[Winmain] Exiting while loop" << std::endl;
	Debug << "[Winmain] Total run time :" << timer.GetElapsed() << std::endl;
	Debug << "[Winmain] Freeing game resource" << std::endl;
	game_end();

	Debug << "[Winmain] Shutting down engine" << std::endl;
	g_engine->Shutdown();
	ShowCursor(true);
	return 1;
}