#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "Object.h"		// Derive from object
#include "Transform.h"	// Include our transformation data
#include "Collision.h"	// Include collision

using namespace Collision::Ndc::Data;	// Get scoped namespace

// The actor class for all deriving classes
class Actor : public Object
{
public:
	Transformv3		_trans;		// Translation structure

protected:
	bool			_act;	// Is the actor active?
	bool			_col;	// Is the actor collidable?
	bool			_mov;	// Is the actor movable?
	bool			_sel;	// Is the actor selected?
	bool			_phy;	// Does the actor have physics applied?			*
	bool			_blck;	// Does the actor intend to block other actors?	*

	unsigned int	_u_mat;		// The model matrix uniform
	unsigned int	_u_sel;		// The selected unfirom

	CollisionData*	_cd;		// Collision data

public:
	

	// Default constructor - initialise variables
	inline Actor() : _sel(false), _act(true), _col(false), _mov(false), _trans({ glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), glm::mat4(0.0f) }) { _t = ACTOR; }

	// Initial constructor
	inline Actor(const char* name, bool active, bool collidable, bool movable, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	{
		_t = ACTOR;		// Set the object type to ACTOR

		SetName(name);	// Assign the actor's name

		_act = active;	// Assign the active bool
		_col = collidable;	// Assign the collidable bool
		_mov = movable;		// Assign the movable bool
		_trans._pos = position;		// Assign the position of the actor
		_trans._sca = scale;	// Assign the scale of the actor
		_trans._rot = rotation;		// Assign the rotation of the actor
	}

	// Deconstructor
	inline ~Actor()
	{
		if (_cd)	// If collision data is valid
			delete _cd;		// Delete allocated memory
	}

	inline bool &IsActive() { return _act; }		// Return active
	inline bool	&IsCollidable() { return _col; }		// Return collidable
	inline bool &IsMovable() { return _mov; }	// Return movable
	inline bool &IsSelected() { return _sel; }	// Return selected

	inline unsigned int &GetMatrixUniformLocation() { return _u_mat; }	// Return model matrix uniform locaion
	inline Transformv3 &GetTransform() { return _trans; }	// Return transform
	inline glm::vec3 &GetPosition() { return _trans._pos; }	// Return position
	inline glm::vec3 &GetScale() { return _trans._sca; }		// Return scale
	inline glm::vec3 &GetRotation() { return _trans._rot; }		// Return rotation
	inline glm::mat4 &GetMatrix() { return _trans._mat; }	// Return model matrix
	inline CollisionData* GetCollisionData() { return _cd; }	// Return the collision object

	inline void SetMatrixUniformLocation(unsigned int value) { _u_mat = value; }	// Assign our model matrix uniform location 
	inline void SetActive(bool value) { _act = value; }		// Assign our active value
	inline void SetCollidable(bool value) { _col = value; }	// Assign our collidable value
	inline void SetMovable(bool value) { _mov = value; }	// Assign our movable value
	inline void SetSelected(bool value) { _sel = value; }	// Assign our selected value
	inline void SetPosition(glm::vec3 value) { _trans._pos = value; }	 // Assign our position as a vec3
	inline void SetScale(glm::vec3 value) { _trans._sca = value; }	 // Assign our scale as a vec3
	inline void SetRotation(glm::vec3 value) { _trans._rot = value; }	 // Assign our rotation as a vec3

	inline void SetMatrix(glm::mat4 value) { _trans._mat = value; }	 // Assign our model matrix as a mat4
	inline void SetCollisionData(CollisionData* value) { _cd = value; }		// Assign collision data

	// This function will tick the model matrix
	inline void UpdateModel()
	{
		_trans._mat = glm::scale(_trans._sca) *	// Assign the position
			glm::rotate(glm::radians(_trans._rot.x), glm::vec3(1.0f, 0.0f, 0.0f)) *		// Rotation X
			glm::rotate(glm::radians(_trans._rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) *		// Rotation Y
			glm::rotate(glm::radians(_trans._rot.z), glm::vec3(0.0f, 0.0f, 1.0f)) *		// Rotation Z
			glm::translate(_trans._pos);	// And the scale
	}

	// This function will update the collision data (interpolated objects only)
	inline void UpdateCollision()
	{
		_cd->Update(_trans._mat);	// Update transformation
	}

	// Set virtual functions for deriving classes
	inline virtual void Update(double& delta) = 0;
	inline virtual void Render() = 0;
};

#endif