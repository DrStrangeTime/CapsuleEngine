#ifndef __INTERPOLATION_H__
#define __INTERPOLATION_H__

// A namepsce that stores all interpolation formulas
namespace Interpolation
{
	// A struct containing all keyframe data
	struct Keyframe
	{
		glm::vec3 _position;

		inline Keyframe(glm::vec3 position)
		{
			_position = position;
		}
	};

	// This float function will return a position between a and b using f as a percentage
	static float lerpf(float a, float b, float f)
	{
		return a + f * (b - a);		// Return the position
	}

	// This function will interpolate an object in constant form
	static inline void Linestep(glm::vec3 &in_position, glm::vec3 &in_velocity, glm::vec3 look_vector, float speed, double delta)
	{
		in_velocity = look_vector * speed;	// Update our velocity vector
		in_position += in_velocity * (float)delta;	// Update our position
	}

	// This function will interpolate an object in constant form
	static inline void Smoothstep(glm::vec3 &in_position, glm::vec3 &in_velocity, glm::vec3 look_vector, float speed, double delta)
	{
		in_velocity = look_vector * speed;	// Update our velocity vector
		in_position += in_velocity * (float)delta;	// Update our position
	}
}

#endif