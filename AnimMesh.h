#ifndef __ANIM_MESH_EXTENSION__
#define __ANIM_MESH_EXTENSION__ ((char*)"Res/Content/AnimMesh/")
#endif

#ifndef __ANIM_MESH_H__
#define __ANIM_MESH_H__

#include "Mesh.h"	// Include our abstract class
#include "AnimData.h"	// Include anim data structs

// The anim mesh class will contain all the data required to render and animate a rigged vao
class AnimMesh : public Mesh
{
private:
	AnimData	_ad;	// Our anim data

public:
	// Default constructor
	inline AnimMesh() {}

	// Initial constructor
	inline AnimMesh(unsigned int shader_program, std::string name, std::vector<Material*> &materials)
	{
		_mt = M_SKELETAL;	// Set object type to MESH
		SetName(name);	// Assign name
		SetMaterials(materials);	// Assign materials
		_u_mat = glGetUniformLocation(shader_program, "mod");	// Get our model matrix uniform
		UpdateModel();	// Initialise model matrix
	}

	inline AnimData &GetAnimData() { return _ad; }	// Get our anim data
	inline void SetAnimData(AnimData value) { _ad = value; }	// Set our anin data
	
	// Update override
	inline virtual void Update(double &delta) {}	

	// Render override
	inline virtual void Render() 
	{
		glUniformMatrix4fv(_u_mat, 1, GL_FALSE, glm::value_ptr(_trans._mat));	// Bind our uniform data

		_vao->Bind();	// Bind our vao

		_mats[0]->Bind();	// Bind our material

		glDrawElements(GL_TRIANGLES, _vd.indices.size(), GL_UNSIGNED_INT, (void*)0);
	}
};

#endif