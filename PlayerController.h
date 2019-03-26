#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Camera.h"		// Include camera
#include "AttachmentMesh.h"		// Include mesh attachments

// This class entends from Camera.h and contains overrides to player mechanics
class PlayerController : public Camera
{
private:
	std::vector<AttachmentMesh*>	_attachments;	// A vector list of mesh attachments

public:
	// Initial contructor
	inline PlayerController(unsigned int shader_program, glm::vec3 position, glm::vec3 position_offset, float fov, float speed, float sensitivity, float n, float f, float ratio)
	{
		_t = PLAYER_CONTROLLER;		// Assign objet type

		InitialiseCamera(shader_program, position, position_offset, fov, speed, sensitivity, n, f, ratio);	// Initialise camera data

		_trans._pos = _trans._pos + (position_offset);	// Reposition camera position from offset
	}

	// Deconstructor
	inline ~PlayerController()
	{
		_attachments.clear();	// Clear all attachments
	}

	inline std::vector<AttachmentMesh*> &GetAttachments() { return _attachments; }	// Return attachment list

	// Assign a new attachment to controller
	inline void AssignAttachment(AttachmentMesh* a)
	{
		_attachments.push_back(a);	// Assign passed mesh attachment to list
	}

	// Remove an attachment from controller
	inline void RemoveAttachment(unsigned int i)
	{
		_attachments.erase(_attachments.begin() + i);	// Remove specific attachment
	}

	// Update virtual void
	inline virtual void Update(double &delta)
	{
		UpdateView();	// Update view matrix
		
		for (AttachmentMesh* a : _attachments)	// Iterate through each attachment
		{
			a->mesh->SetMatrix(glm::inverse(GetViewMatrix()) * glm::translate(a->offset));	// Assign camera view matrix with mesh position offset
		}

		//if (ControlActive())	// If camera is moving
		//	_attachments[0]->mesh->UpdateCollision();	// Update collision

		//std::cout << _attachments[0]->mesh->GetCollisionData()->a.z << "\n";
	}

	// Redner virtual void
	inline virtual void Render()
	{
		UpdateCamUniforms();	// Pass matrix uniform data

		for (AttachmentMesh* a : _attachments)	// Iterate through each attachment
			a->mesh->Render();	// Render mesh attachments
	}
};

#endif