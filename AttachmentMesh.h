#ifndef __ATTACHMENT_MESH_H__
#define __ATTACHMENT_MESH_H__

#include "Mesh.h"	// Include deriving class

// This class will provide a mesh attachment ability to (idealy) interpolating camera objects
class AttachmentMesh : public Actor
{
public:
	glm::vec3	offset;		// Position offset (if any)
	Mesh*		mesh;		// Mesh object

public:
	// Default constructor
	inline AttachmentMesh() {}

	// Initial constructor
	inline AttachmentMesh(unsigned int shader_program, Mesh* m, glm::vec3 o)
	{
		if (m)	// If mesh object has been initialised
			mesh = m;	// Assign mesh reference object
		else	// Otherwise
		{
			std::cerr << "Error: Mesh parsed to AttachmentMesh params is not valid!\n";		// Print error message
			return;		// Return
		}

		offset = o;		// Assign offset
	}

	// Virtual voids
	inline virtual void Update(double& delta) {}
	inline virtual void Render() {}
};

#endif