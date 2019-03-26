#ifndef __GEOMETRY_PASS_H__
#define __GEOMETRY_PASS_H__

#include "Pass.h"	// Include abstract class
#include "TextureCache.h"

/*
* Geometry pass class: This class will store the geometry pass for each vertex. 
* The geometry pass stores a range of data in the form of textures including,
* world positions, normals, albedo and specularity 
*/
class GeometryPass : public Pass
{
private:
	GLuint	_u_camera_pos;
	GLuint	_u_selection_colour;
	GLuint	_u_wire_mate;

	bool	_wire_mate;	// Polygon mode variable
	unsigned int cubemap;
public:
	unsigned int skyboxVAO, skyboxVBO;

	inline GeometryPass(std::vector<GLuint> shader_programs)
	{
		Create(shader_programs);		// Create the post effect
	}

	// Initialise geometry pass
	inline void Create(std::vector<GLuint> shader_programs)
	{
		_shader_programs = shader_programs;
		_wire_mate = false;	// Set default polygon mode

		glUseProgram(_shader_programs[0]);	// Use shader program

		_u_camera_pos = glGetUniformLocation(_shader_programs[0], "camera_pos");
		_u_selection_colour = glGetUniformLocation(_shader_programs[0], "selection_colour");
		_u_wire_mate = glGetUniformLocation(_shader_programs[0], "wire_mate");

		// Initialise default map content
		Content::_map = new Map("Scene_01");
		Content::_map->AddActor(new PlayerController(_shader_programs[0], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.8f, 5.0f), CAMERA_FOV, CAMERA_SPEED, CAMERA_LOOK_SENSITIVITY, CAMERA_NEAR, CAMERA_FAR, GetUpdatedAspectRatio()), PLAYER);
		

		// load in the texture for the first model in the scene from the cache and its material ids
		unsigned ids[] = { 0, 1, 2, 3, 4 };
		TextureCache::GetTexture(_shader_programs[0], "Astroid", ids);
	}

	inline bool &IsWireMode() { return _wire_mate; }	// Return the current polygon mode
	inline void SetWireMode(bool value) { _wire_mate = value; }		// Set the current polygon mode

	// Virtual functions
	inline virtual void Update(double delta) { Content::_map->Update(delta); }
	inline virtual void Render()
	{
		glUseProgram(_shader_programs[0]);	// Use shader program
		
		//glDisable(GL_CULL_FACE);


		glUniform1i(_u_wire_mate, _wire_mate);	// Send polygon mode to shader
		glUniform3fv(_u_camera_pos, 1, glm::value_ptr(Content::_map->GetPlayerController()->GetPosition()));		// Bind the camera position uniform location

		Content::_map->GetPlayerController()->Render();	// Render the camera
		
		for (Actor* a : Content::_map->GetActors())		// Iterate through each actor in map
		{
			if (a->GetObjectType() == MESH) // If object type is type mesh
			{
				if (_wire_mate)		// If wire mode is toggled
				{
					glDisable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Assign current polygon mode
				}

				a->Render(); // render mesh actor

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Assign current polygon mode
				glEnable(GL_CULL_FACE);
				glEnable(GL_DEPTH_TEST);	// Enable depth test
			}
		}
	}
};

#endif