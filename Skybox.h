#pragma once
#include "Engine.h"

namespace Manbat{
	class Skybox
    {
    private:
        Texture* textures[6];
        ID3DXMesh *mesh;

    public:
        Skybox(void);
        virtual ~Skybox(void);

        bool Create( std::string sharedFilename );
        void Render();

    };
};