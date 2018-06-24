#pragma once

#include "Engine.h"

namespace Manbat {


	class AStarNode
	{
	public:
		int x;
		int y;
		int G;
		int H;
		int F;
		int parentG;
		int parentX;
		int parentY;
		int childX;
		int childY;


	};

	class LevelNode {
	public:
		std::string name;
		int row;
		int col;
		int type;
		int obj;
	};

};
