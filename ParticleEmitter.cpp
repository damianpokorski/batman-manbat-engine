#include "Engine.h"

namespace Manbat{

	//constructor
	ParticleEmitter::ParticleEmitter(){
		//initialise vars
		p_image = NULL;						//NULL image texture
		p_max = 100;							//max 100 particles
		p_direction = 0;						//direction (angle)
		p_alphaMin = 254;					//min alpha value
		p_alphaMax = 255;					//max alpha value
		p_minR = 0; p_maxR = 255;			//min  / max red val
		p_minG = 0; p_maxG = 255;			//min / max green val
		p_minB = 0; p_maxB = 255;			//min/ max blue val
		p_spread = 10;						//spread
		p_velocity = 1.0f;						//velocity
		p_scale = 1.0f;						//scale
	}

	//load a texture for the particle image
	bool ParticleEmitter::Load(std::string imageFile){

		//if already a texture, release memory 
		if (p_image){
			delete p_image;
			p_image = NULL;
		}

		//create new Texture object 
		p_image = new Texture();

		//load the texture from the image file
		if (!p_image->Load(imageFile)){
			Debug << "Error loading particle image \n";
			return false;
		}
		return true;
	}

	//destructor
	ParticleEmitter::~ParticleEmitter(){
		//delete image texture 
		delete p_image;
		//delete each sprite in the particles array
		for (size_t i = 0; i < p_particles.size(); i++) {
			delete p_particles[i];
			p_particles[i] = NULL;
		}
		//clear rthe particles STL vector
		p_particles.clear();
	}

	//Add individual particles
	void ParticleEmitter::Add(){
		//velocities on x and y
		double vx, vy;

		//new sprite; set image and position
		Sprite *p = new Sprite();
		p->setImage(p_image);
		p->setPosition(p_position.getX(), p_position.getY());

		//add some randomness to the spread
		double variation = (rand() % p_spread - p_spread / 2) / 100.0f;

		//set linear velocity
		double dir = Math::wrapAngleDegs(p_direction - 90.0f);
		dir = Math::toRadians(dir);
		vx = cos(dir) + variation;
		vy = sin(dir) + variation;
		p->setVelocity(vx*p_velocity, vy *p_velocity);

		//set random color based on ranges
		int r = rand() % (p_maxR - p_minR) + p_minR;
		int g = rand() % (p_maxG - p_minG) + p_minG;
		int b = rand() % (p_maxB - p_minB) + p_minB;
		int a = rand() % (p_alphaMax - p_alphaMin) + p_alphaMin;
		Color col(r, g, b, a);
		p->setColor(col);

		//set the scale
		p->setScale(p_scale, p_scale);

		//add particle to the emitter
		p_particles.push_back(p);
	}

	//Render method (with optional rotation)
	void ParticleEmitter::Render(bool rotate = false){

		//for each sprite in the particles vector
		//BOOST_FOREACH(Sprite* sprite, p_particles)
		for(size_t i = 0; i < p_particles.size(); i++)
		{
			//if rotate set, give random rotation
			if (rotate){
				p_particles[i]->setRotation(rand() % 5);
			}
			//render the sprite
			p_particles[i]->Render();
		}
	}

	//Update method
	void ParticleEmitter::Update(float deltaTime){
		static Timer timer;

		//do we need to add a new particle?
		if ((int)p_particles.size() < p_max){
			//trivial but necessary slowdown
			//to add new particles
			if (timer.Stopwatch(100)) Add();
		}

		//for each sprite in the particle vector
		//BOOST_FOREACH(Sprite* sprite, p_particles){
		for(size_t i = 0; i < p_particles.size(); i++){
			//update particle's position
			p_particles[i]->Update(deltaTime * 50.0f);

			//is particle's beyond the emitter's range?
			double dist = Math::Distance(p_particles[i]->getPosition(),
				p_position);
			if (dist > p_length){
				//reset particle to the origin
				p_particles[i]->setX(p_position.getX());
				p_particles[i]->setY(p_position.getY());
			}
		}
	}

	//set min / max alpha values
	void ParticleEmitter::setAlphaRange(int min, int max){
		p_alphaMin = min; p_alphaMax = max;
	}

	//update RGB min / max colour range
	void ParticleEmitter::setColorRange(int r1, int g1, int b1,
		int r2, int g2, int b2)
	{
		p_minR = r1; p_maxR = r2;
		p_minG = g1; p_maxG = g2;
		p_minB = b1; p_maxB = b2;
	}
}