#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {
	void AnimatedBillboard::LoadTextures(std::string pre, std::string post, int min, int max) {
		stringstream ss;
		for (int i = min; i <= max; i++) {
			// make sure stringstream is empty
			ss.str("");
			ss.clear();
			// build filename string
			ss << pre << i << post;
			frames.push_back(g_engine->cacheTexture(ss.str()));
		}
			/*frames.push_back(g_engine->cacheTexture("Scenery/Billboard/sample.png"));*/
			/*frames.push_back(g_engine->cacheTexture("kermit/0.png"));
			frames.push_back(g_engine->cacheTexture("kermit/1.png"));
			frames.push_back(g_engine->cacheTexture("kermit/2.png"));
			frames.push_back(g_engine->cacheTexture("kermit/3.png"));
			frames.push_back(g_engine->cacheTexture("kermit/4.png"));
			frames.push_back(g_engine->cacheTexture("kermit/5.png"));
			frames.push_back(g_engine->cacheTexture("kermit/6.png"));
			frames.push_back(g_engine->cacheTexture("kermit/7.png"));
			frames.push_back(g_engine->cacheTexture("kermit/8.png"));
			frames.push_back(g_engine->cacheTexture("kermit/9.png"));
			frames.push_back(g_engine->cacheTexture("kermit/10.png"));*/
	}
}