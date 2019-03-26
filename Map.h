#ifndef __MAP_H__
#define __MAP_H__

#include "Glsl.h"		// Shader data
#include "Mouse.h"		// Get mouse data
#include "PlayerController.h"		// Get deriving class
#include "Skybox.h"		// Get skybox for exterior
#include "StaticMesh.h"		// Get static mesh class
#include "AnimMesh.h"	// Get anim mesh class
#include "Light.h"

// The map class will be our 3D canvas
class Map : public Object
{
private:
	char*						_name;		// The map name

	PlayerController*			_player_controller;	// The main camera
	Skybox*						_skybox;	// The skybox
	Light*						_light;		// The lights
	
	std::vector<Actor*>			_actors;	// Our actor list

public:
	// Default constructor
	inline Map() { _t = MAP; }

	// Initial constructor
	inline Map(char* name)
	{
		_t = MAP;	// Assign our actor tpye to map
		_name = name;	// Assign our name variable
	}

	// Deconstructor will clear all allocated memory
	inline ~Map()
	{
		_actors.clear();	// Clear the actor vector list

		if (_player_controller) delete _player_controller;	// Delete camera
		if (_skybox) delete _skybox;	// Delete skybox
	}

	// This will be parsed to the Callback function
	inline void KeyDownCallback()
	{
	}

	// This will be parsed to the Callback function
	inline void KeyUpCallback()
	{
	}

	// This will be parsed to the Callback function
	inline void MouseMotionCallback()
	{
		_player_controller->ProccessMouseRotation(Mouse::GetOffsetX(), Mouse::GetOffsetY(), CAMERA_LOOK_SENSITIVITY);	// Update the camera rotation in respect to the mouse
	}

	// Get the player controller object
	inline PlayerController* GetPlayerController()
	{
		return _player_controller;		// Return camera
	}

	// Get the skybox object
	inline Skybox* GetSkybox()
	{
		return _skybox;		// Return skybox
	}

	// Get the list of actors
	inline std::vector<Actor*> &GetActors()
	{
		return _actors;		// Return the list of actors
	}

	// Insert actor to vector
	inline void AddActor(Actor* actor, unsigned int ptr_type)
	{
		switch (ptr_type)
		{
		case PLAYER:	// If type camera
			_player_controller = ((PlayerController*)actor);	// Assign player location
			break;
		case SKYBOX:	// If type camera
			_skybox = ((Skybox*)actor);		// Assign camera location
			break;
		case LIGHT:
			_light  = ((Light*)actor);		// Assign light location
			break;
		}			

		_actors.push_back(actor);	// Push back allocated memory to vector list
	}
	
	// The update function will check for logic
	virtual inline void Update(double &delta)
	{		
		_player_controller->FreeMove(delta);	// Update camera interpolation and check for collision

		for (Actor* a : _actors)	// Iterate through our actor list...
		{
			a->Update(delta);		// Update all of the actors
		}
		
	}

	// This will render all actors in the world
	virtual inline void Render() {}
};

#endif