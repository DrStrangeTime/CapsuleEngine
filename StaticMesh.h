#ifndef __STATIC_MESH_EXTENSION__
#define __STATIC_MESH_EXTENSION__	((char*)".mesh")
#endif

#ifndef __STATIC_MESH_URI__
#define __STATIC_MESH_URI__			((char*)"Res/Content/StaticMesh/")
#endif

#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__

#include "Mesh.h"	// Derive from

class StaticMesh : public Mesh
{
public:
	// Default constructor
	inline StaticMesh() { _mt = M_STATIC; }

	// Initial constructor
	inline StaticMesh(unsigned int shader_program, std::string name, std::vector<Material*> &materials, Cubemap* cubemap)
	{
		_mt = M_STATIC;		// Assign our mesh type
		SetName(name);	// Assign our name data
		SetMaterials(materials);	// Assign our material data
		UpdateModel();	// Initialise our model matrix

		_u_sel = glGetUniformLocation(shader_program, "selected");	// Get our model matrix uniform
		_u_mat = glGetUniformLocation(shader_program, "mod");	// Get our model matrix uniform
	}

	// Virtual functions
	inline virtual void Update(double &delta) {}
	inline virtual void Render()
	{
		glUniform1i(_u_sel, _sel);	// Bind our selected uniform data
		glUniformMatrix4fv(_u_mat, 1, GL_FALSE, glm::value_ptr(_trans._mat));	// Bind our uniform data

		_vao->Bind();	// Bind our element buffer object

		for (Chunk c : _chunks)		// Iterate through each chunk element...
		{
			_mats[c._id]->Bind();	// Bind our material(s)

			glDrawElements(
				GL_TRIANGLES,				// mode
				c._index_count,				// count
				GL_UNSIGNED_INT,			// type
				(void*)(c._index_offset));	// element array buffer offset
		}
	}
};

#endif