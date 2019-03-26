#ifndef __MESH_H__
#define __MESH_H__

#include "Actor.h"	// Get our deriving class
#include "Chunk.h"	// Get access to the Chunk struct
#include "VertexData.h"		// Get access to the vertex data struct
#include "Material.h"	// Get access to the material class
#include "Vao.h"	// Get access to the ebo class
#include "Cubemap.h"	// Get access to cubemap data

// A list of mesh types
enum MeshTypes
{
	M_STATIC,
	M_SKELETAL,
};

// This abstract class will contain the basic information for our Mesh heiarchy
class Mesh : public Actor
{
protected:
	unsigned int			_mt;	// This will define our mesh type
	unsigned int			_ct;	// This will define our collision type
	unsigned int			_num_indices;	// Our index count for vao rendering
	Vao*					_vao;	// Our ebo will create our geometry
	VertexData				_vd;	// This will contain our vertex data
	Cubemap*				_cubemap;	// The cubemap ptr
	std::vector<Chunk>		_chunks;	// This will contain an array of chunks (elements)
	std::vector<Material*>	_mats;	// This will contain our material data

public:
	// Default constructor
	inline Mesh() { _t = MESH; }

	// Deconstructor
	inline ~Mesh() { if (_vao) delete _vao; }

	inline unsigned int &GetMeshType() { return _mt; }	// Return our mesh type
	inline unsigned int &GetCollisionType() { return _mt; }	// Return our mesh type
	inline unsigned int &GetNumIndices() { return _num_indices; }	// Return the number of indices
	inline Vao* GetVao() { return _vao; }	// Return our element buffer object
	inline VertexData &GetVertexData() { return _vd; }		// Return our vertex data
	inline Cubemap* GetCubemap() { return _cubemap; }	// Return the cubemap ptr
	inline std::vector<Chunk> &GetChunks() { return _chunks; }	// This returns our chunk list
	inline std::vector<Material*> &GetMaterials() { return _mats; }		// Return our materials

	inline void SetMeshType(unsigned int value) { _mt = value;  }	// Assign a value to our mesh type
	inline void SetNumIndices(unsigned int value) { _num_indices = value; }		// Assign a value to our num_indices
	inline void SetVao(Vao* value) { _vao = value; }	// Assign a value to our ebo
	inline void SetVertexData(VertexData value) { _vd = value; }	// Assign a value to our vertex data
	inline void SetCubemap(Cubemap* value) { _cubemap = value; }	// Assign value ptr to cubemap ptr
	inline void SetChunks(std::vector<Chunk> &value) { _chunks = value; }	// Assign a value to our chunks
	inline void SetMaterials(std::vector<Material*> &value) { _mats = value; }	// Assign a value to our materials

	// Virtual voids
	inline virtual void Update(double &delta) {}
	inline virtual void Render() {}
};

#endif