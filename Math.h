#ifndef __MATH_H__
#define __MATH_H__

#define __PI	3.14159		// For radial calculations

#include <random>	// Include random generators
#include <glm\glm.hpp>	// Get glm variables
#include <vector>	// Get dynamic arrays

// This contains some extra math functions
namespace Math
{
	// This function will reset vertex data after rotating
	static void FreezeRotation(std::vector<glm::vec3> &v, glm::vec3 &rotation)
	{
		for (unsigned int i = 0; i < v.size(); i++)
		{
			v[i] = glm::vec4(v[i], 1.0f)	* glm::rotate(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
				* glm::rotate(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}

	// This returns the lowest value of x[i]
	static float Minf(std::vector<float> &x)
	{
		float it = x[0];	// Assign iterator to first element

		for (unsigned int i = 1; i < x.size(); i++)		// Iterate through each value...
			if (x[i] < it)	// If a lower value has been found...
				it = x[i];	// Assign to it

		return it;	// Return result
	}

	// This returns the lowest value of x[i]
	static glm::vec2 Minv2(std::vector<glm::vec2> &x)
	{
		glm::vec2 it = x[0];	// Assign iterator to first element

		for (unsigned int i = 1; i < x.size(); i++)		// Iterate through each value...
			if (glm::all(glm::lessThan(x[i], it)))	// If a lower value has been found...
				it = x[i];	// Assign to it

		return it;	// Return result
	}

	// This returns the lowest value of x[i]
	static glm::vec3 Minv3(std::vector<glm::vec3> &x)
	{
		glm::vec3 it = x[0];	// Assign iterator to first element

		for (unsigned int i = 1; i < x.size(); i++)		// Iterate through each value...
			if (glm::all(glm::lessThan(x[i], it)))	// If a lower value has been found...
				it = x[i];	// Assign to it

		return it;	// Return result
	}

	// This returns the greatest value of x[i]
	static float Maxf(std::vector<float> &x)
	{
		float it = x[0];	// Assign iterator to first element

		for (unsigned int i = 1; i < x.size(); i++)		// Iterate through each value...
			if (x[i] > it)	// If a greater value has been found...
				it = x[i];	// Assign to it

		return it;	// Return result
	}

	// This returns the greatest value of x[i]
	static glm::vec2 Maxv2(std::vector<glm::vec2> &x)
	{
		glm::vec2 it = x[0];	// Assign iterator to first element

		for (unsigned int i = 1; i < x.size(); i++)		// Iterate through each value...
			if (!glm::all(glm::lessThan(x[i], it)))	// If a greater value has been found...
				it = x[i];	// Assign to it

		return it;	// Return result
	}

	// This returns the greatest value of x[i]
	static glm::vec3 Maxv3(std::vector<glm::vec3> &x)
	{
		glm::vec3 it = x[0];	// Assign iterator to first element

		for (unsigned int i = 1; i < x.size(); i++)		// Iterate through each value...
			if (!glm::all(glm::lessThan(x[i], it)))	// If a greater value has been found...
				it = x[i];	// Assign to it

		return it;	// Return result
	}

	// This returns the greatest value of x[i] in terms of component size
	static glm::vec3 MaxComponentSizev3(std::vector<glm::vec3> &x)
	{
		glm::vec3 it = x[0];	// Result

		for (unsigned int i = 1; i < x.size(); i++)		// Iterate through each value...
		{
			if ( ((glm::abs(x[i].x)) + (glm::abs(x[i].y)) + (glm::abs(x[i].z))) > ((glm::abs(it.x)) + (glm::abs(it.y)) + (glm::abs(it.z))) )	// Compare component sizes
				it = x[i];	// Assign larger component size
		}

		return it;	// Return result
	}

	// This returns a scalar value of the greatest component value
	static float MaxComponentValueAbsv3(std::vector<glm::vec3> &x)
	{
		std::vector<float> comp_abs;	// Create temp list of vertex component values in absolute
		for (unsigned int i = 1; i < x.size(); i++)		// Iterate through each vertex
		{
			for (unsigned int j = 0; j < 3; j++)	// Iterate through each component
				comp_abs.push_back(glm::abs(x[i][j]));	// Assign new component value
		}

		float r = Maxf(comp_abs);	// Get max compoonent value

		return r;	// Return result
	}

	// This returns the average value of x[i]
	static int Avgi(std::vector<int> &x)
	{
		int it = 0;

		for (unsigned int i = 0; i < x.size(); i++)
			it += x[i];

		return (it) / x.size();
	}

	// This returns the average value of x[i]
	static float Avgf(std::vector<float> &x)
	{
		float it = 0;

		for (unsigned int i = 0; i < x.size(); i++)
			it += x[i];

		return (it) / x.size();
	}

	// This returns the average value of x[i]
	static glm::vec2 Avgv2(std::vector<glm::vec2> &x)
	{
		glm::vec2 it = glm::vec2(0.0f);

		for (unsigned int i = 0; i < x.size(); i++)
			it += x[i];

		return (it) / static_cast<float>(x.size());
	}

	// This returns the average value of x[i]
	static glm::vec3 Avgv3(std::vector<glm::vec3> x)
	{
		glm::vec3 it = glm::vec3(0.0f);

		for (unsigned int i = 0; i < x.size(); i++)
			it += x[i];

		return (it) / static_cast<float>(x.size());
	}

	// This will return a normal direction vector
	static glm::vec3 CalcNormal(glm::vec3 x[3])
	{
		glm::vec3 v0 = x[1] - x[0];
		glm::vec3 v1 = x[2] - x[0];

		glm::vec3 n = glm::cross(v0, v1);

		return glm::normalize(n);	// Return the normal
	}

	static glm::vec3 AverageTriangle(glm::vec3 x[3])
	{
		return	(glm::vec3((x[0].x + x[1].x + x[2].x) / 3.0f,
			(x[0].y + x[1].y + x[2].y) / 3.0f,
			(x[0].z + x[1].z + x[2].z) / 3.0f));
	}

	// A function that checks if a vector is normalised
	inline bool IsTypeUnit(glm::vec3 &v)
	{
		return (v.x <= 1 && v.y <= 1 && v.z <= 1); 	// Return true if vector is unit length
	}

	// This returns if the number is divisible or not
	static bool IsDivisible(int x, int n)
	{
		return (x % n) == 0;	// Return whether or not x is a multiple of n
	}
}

#endif