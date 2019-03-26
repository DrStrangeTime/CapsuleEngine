#ifndef __COLLISION_H__
#define __COLLISION_H__

#define CAT_POINT					0	// Collision arbitrary type corner
#define CAT_EDGE					1	// Collision arbitrary type edge
#define CAT_TRIANGLE				2	// Collision arbitrary type plane
#define COLLISION_TYPE_NONE			3	// Collision type none
#define COLLISION_TYPE_CUBE			4	// Collision type box
#define COLLISION_TYPE_PER_VERTEX	5	// Collision type per-vertex
#define COLLISION_TYPE_CUSTOM		6	// Collision type custom

#include <glew.h>	// Include gl
#include "Math.h"	// Include vector math
#include "Globals.h"	// Get global data

// A namespace block to store all collision data and functions
namespace Collision
{
	// Normal device coordinate collision (3d)
	namespace Ndc
	{
		// A namespace for storing collision arbitrary data
		namespace Data
		{
			// Abstract arbitrary class
			struct Arbitrary
			{
				uint8_t t;	// Arbitrary type

				// Default constructor
				inline Arbitrary() : t(0) {}

				// Virtual void
				inline virtual void Update() = 0;
			};

			// Point arbitrary
			struct Point : public Arbitrary
			{
				glm::vec3 p;	// Position

				inline Point(glm::vec3 position)
				{
					t = CAT_POINT;	// Assign arbitrary type

					p = position;	// Assign position
				}

				// Update
				inline virtual void Update() {}
			};

			// Edge arbitrary
			struct Edge : public Arbitrary
			{
				glm::vec3 p_0;	// Edge 0
				glm::vec3 p_1;	// Edge 1
				glm::vec3 a;	// Average
				glm::vec3 v;	// Vector
				glm::vec3 d;	// Direction

				// Initial contructor
				inline Edge(glm::vec3 point_0, glm::vec3 point_1)
				{
					t = CAT_EDGE;	// Assign arbitrary type

					p_0 = point_0;	// Assign edge 0
					p_1 = point_1;	// Assign edge 1

					Update();	// Update
				}

				// Update
				inline virtual void Update()
				{
					a = (p_0 + p_1) / 2.0f;		// Assign average
					v = p_1 - p_0;	// Assign vector
					d = glm::normalize(p_1 - p_0);	// Assign direction
				}
			};

			// Triangle data structure
			struct Triangle : public Arbitrary
			{
				glm::vec3	a;		// Average
				glm::vec3	n;		// Normal
				Point*		p[3];	// Three points
				Edge*		e[3];	// Three edges
				
				// Constructor
				inline Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
				{
					t = CAT_TRIANGLE;	// Assign arbitrary type

					p[0] = new Point(v0);	// Create point 0
					p[1] = new Point(v1);	// Create point 1
					p[2] = new Point(v2);	// Create point 2

					e[0] = new Edge(v0, v1);	// Create edge 0
					e[1] = new Edge(v1, v2);	// Create edge 1
					e[2] = new Edge(v2, v0);	// Create edge 2

					Update();	// Update vector data
				}

				// Deconstructor
				inline ~Triangle()
				{
					for (unsigned int i = 0; i < 3; i++)	// Iterate for all three vertices
					{
						if (p[i])	delete p[i];	// Delete points
						if (e[i])	delete e[i];	// Delete edges
					}
				}

				// Update vectors
				inline virtual void Update()
				{
					a = (p[0]->p + p[1]->p + p[2]->p) / 3.0f;	// Calculate average
					n = glm::cross(p[1]->p - p[0]->p, p[2]->p - p[0]->p);	// Calculate normal

					e[0]->p_0 = p[0]->p; e[0]->p_1 = p[1]->p;	// Caclulate edge 0
					e[1]->p_0 = p[1]->p; e[1]->p_1 = p[2]->p;	// Caclulate edge 1
					e[2]->p_0 = p[2]->p; e[2]->p_1 = p[0]->p;	// Caclulate edge 2

					for (unsigned int i = 0; i < 3; i++)	// Iterate through each point
					{
						p[i]->Update();		// Update points
						e[i]->Update();		// Update edges
					}
				}
			};

			// A structure for storing an array of hit points on a colliding object, and a normal velocity vector
			struct HitData
			{
				bool					stuck;			// Is the colliding object stuck?
				glm::vec3				normal;			// The normal vector representing velocity
				std::vector<glm::vec3>	hit_points;		// An array of points of collision
			};

			// A struct for storing all arbitrary data
			struct CollisionData
			{
				float					r;		// Radii value
				uint8_t					ct;		// Collision type
				glm::vec3				a;		// Average position
				HitData					hd;		// Hit data
				std::vector<glm::vec3>	pd;		// Vertex position data
				std::vector<glm::vec3>  pd_o;	// Original vertex position data
				std::vector<Triangle*>	t;		// List of triangle data

				// Initial constructor
				inline CollisionData(uint8_t collision_type, std::vector<glm::vec3> triangle_data)
				{
					ct = collision_type;	// Assign collision type
					r = 0.0f;	// Initialise max radii value
					hd = HitData({ false, {} });	// Initialise hit data
					Assign(collision_type, triangle_data);	// Assign data
				}

				// Deconstructor
				inline ~CollisionData()
				{
					t.clear();	// Clear arbitrary data
				}

				// Assign indexed vertex data to arbitrary data
				inline void Assign(uint8_t collision_type, std::vector<glm::vec3> triangle_data)
				{
					if (collision_type == COLLISION_TYPE_CUBE)	// If collision type is cubic
					{
						float min_x, max_x;		// Min and max for x
						float min_y, max_y;		// Min and max for y
						float min_z, max_z;		// Min and max for z
						float l, h, d;			// Length, height and depth

						std::vector<float> x_components;	// Temp list of x components
						std::vector<float> y_components;	// Temp list of y components
						std::vector<float> z_components;	// Temp list of z components

						std::vector<Point*> points;		// Temp point data

						for (unsigned int i = 0; i < triangle_data.size(); i++)	// Iterate through each vertex
						{
							x_components.push_back(triangle_data[i].x);		// Add x component
							y_components.push_back(triangle_data[i].y);		// Add y component
							z_components.push_back(triangle_data[i].z);		// Add z component
						}

						min_x = Math::Minf(x_components);	// Get min x value from all vertices
						min_y = Math::Minf(y_components);	// Get min y value from all vertices
						min_z = Math::Minf(z_components);	// Get min z value from all vertices
						max_x = Math::Maxf(x_components);	// Get max x value from all vertices
						max_y = Math::Maxf(y_components);	// Get max y value from all vertices
						max_z = Math::Maxf(z_components);	// Get max z value from all vertices

						l = abs((max_x - min_x) / 2.0f);	// Get length
						h = abs((max_y - min_y) / 2.0f);	// Get Height
						d = abs((max_z - min_z) / 2.0f);	// Get Depth

						glm::vec3 ao = glm::vec3((min_x + max_x) / 2.0f, (min_y + max_y) / 2.0f, (min_z + max_z) / 2.0f);	// Calculate average offset					

						t.push_back(new Triangle(ao + glm::vec3(-l, -h, d), ao + glm::vec3(l, -h, d), ao + glm::vec3(-l, h, d)));		// Create triangle
						t.push_back(new Triangle(ao + glm::vec3(-l, h, d), ao + glm::vec3(l, -h, d), ao + glm::vec3(l, h, d)));			// Create triangle
						t.push_back(new Triangle(ao + glm::vec3(-l, h, d), ao + glm::vec3(l, h, d), ao + glm::vec3(-l, h, -d)));		// Create triangle
						t.push_back(new Triangle(ao + glm::vec3(-l, h, -d), ao + glm::vec3(l, h, -d), ao + glm::vec3(-l, -h, -d)));		// Create triangle
						t.push_back(new Triangle(ao + glm::vec3(-l, h, -d), ao + glm::vec3(l, h, -d), ao + glm::vec3(-l, -h, -d)));		// Create triangle
						t.push_back(new Triangle(ao + glm::vec3(-l, -h, -d), ao + glm::vec3(l, h, -d), ao + glm::vec3(-l, -h, d)));		// Create triangle
						t.push_back(new Triangle(ao + glm::vec3(-l, -h, -d), ao + glm::vec3(l, -h, -d), ao + glm::vec3(-l, -h, d)));	// Create triangle
						t.push_back(new Triangle(ao + glm::vec3(-l, -h, d), ao + glm::vec3(l, -h, -d), ao + glm::vec3(l, -h, d)));		// Create triangle
						t.push_back(new Triangle(ao + glm::vec3(l, -h, d), ao + glm::vec3(l, -h, -d), ao + glm::vec3(l, h, d)));		// Create triangle
						t.push_back(new Triangle(ao + glm::vec3(l, h, d), ao + glm::vec3(l, -h, -d), ao + glm::vec3(l, h, -d)));		// Create triangle
						t.push_back(new Triangle(ao + glm::vec3(-l, -h, -d), ao + glm::vec3(-l, -h, d), ao + glm::vec3(-l, h, -d)));	// Create triangle
						t.push_back(new Triangle(ao + glm::vec3(-l, h, -d), ao + glm::vec3(-l, -h, d), ao + glm::vec3(-l, h, d)));		// Create triangle

						points.push_back(new Point(ao + glm::vec3(-l, h, d)));		// Front top left		[0]
						points.push_back(new Point(ao + glm::vec3(l, h, d)));		// Front top right		[1]
						points.push_back(new Point(ao + glm::vec3(-l, -h, d)));		// Front bottom left	[2]
						points.push_back(new Point(ao + glm::vec3(l, -h, d)));		// Front bottom right	[3]
						points.push_back(new Point(ao + glm::vec3(-l, h, -d)));		// Back top left		[4]
						points.push_back(new Point(ao + glm::vec3(l, h, -d)));		// Back top right		[5]
						points.push_back(new Point(ao + glm::vec3(-l, -h, -d)));	// Back bottom left		[6]
						points.push_back(new Point(ao + glm::vec3(l, -h, -d)));		// Back bottom right	[7]


						for (unsigned int i = 0; i < 8; i++)	// Iterate through each corner
							pd.push_back(points[i]->p);		// Assign vertex position data
					}
					
					else if ((collision_type == COLLISION_TYPE_PER_VERTEX) || (collision_type == COLLISION_TYPE_CUSTOM))	// If collision type per-vertex or custom
					{
						std::vector<Point*> points;		// Temp point data

						for (unsigned int i = 0; i < triangle_data.size(); i += 3)	// Iterate through each indexed vertex position
						{
							glm::vec3 v0 = triangle_data[i];		// Vertex 0 in tri offset
							glm::vec3 v1 = triangle_data[i + 1];	// Vertex 1 in tri offset
							glm::vec3 v2 = triangle_data[i + 2];	// Vertex 2 in tri offset

							points.push_back(new Point(v0));	// Create temp point 0
							points.push_back(new Point(v1));	// Create temp point 1
							points.push_back(new Point(v2));	// Create temp point 2

							t.push_back(new Triangle(v0, v1, v2));	// Create triangle
						}

						// -------------------------- CLEAN UP DUPLICTE POINTS! --------------------------------------
						for (unsigned int i = 0; i < points.size(); i++)		// Iterate through each edge
						{
							for (unsigned int j = 0; j < points.size(); j++)		// Iterate through each edge again
							{
								if (i != j)		// If i is not the same element as j
								{
									if (points[i]->p == points[j]->p)		// If both values are the same
									{
										points.erase(points.begin() + i);		// Remove duplicate
										i > 0 ? i-- : i = 0;	// Re-assign iteration
									}
								}
							}
						}

						// --------------------------- ASSIGN OPTIMISED POINT DATA ------------------------------
						for (unsigned int i = 0; i < points.size(); i++)	// Iterate through each corner
							pd.push_back(points[i]->p);		// Assign to temp container

						points.clear();		// Clear temp point data
					}
						
					else	// Otherwise if collision type is invalid
					{
						std::cerr << "Collision Error: Invalid collision type!\n";	// Print out error message
						return;		// Return from function
					}
					
					a = Math::Avgv3(pd);	// Calculate average position
					glm::vec3 max_local = glm::abs(Math::MaxComponentSizev3(pd)) - a;	// Calculate the max vector in local space
					r = glm::length(max_local);		// Calculate length of radii

					pd_o = pd;	// Assign position data to the original position list
				}


				// Update
				inline void Update(glm::mat4 m)
				{
					// --------------------- UPDATE TRIANGLE DATA ---------------------
					glm::vec4 res(glm::vec4(0.0f));		// Result for each vertex position

					for (unsigned int i = 0; i < t.size(); i++)		// Iterate through each triangle
					{
						for (unsigned int j = 0; j < 3; j++)	// Iterate through each vertex
						{
							glm::vec4 v_opt(glm::vec4(t[i]->p[j]->p, 1.0f));	// Optimise vertex position
							res = glm::vec4(m * v_opt);		// Calculate transform result
						
							t[i]->p[j]->p = glm::vec3(res.x / res.w, res.y / res.w, res.z / res.w);		// Convert transform result back to vec3
						}

						t[i]->Update();		// Update all arbitrary data
					}

					// --------------------- UPDATE POINT DATA ---------------------
					for (unsigned int i = 0; i < pd.size(); i++)	// Iterate through each point
					{
						glm::vec4 v_opt = glm::vec4(pd_o[i], 1.0f);		// Optimise vertex position
						res = glm::vec4(m * v_opt);		// Calculate transform result
						
						pd[i] = glm::vec3(res.x / res.w, res.y / res.w, res.z / res.w);		// Convert transform result back to vec3
					}
					
					a = Math::Avgv3(pd);	// Recalculate average position
				}
			};
		}


		// A namespace for calculating arbitrary detection
		namespace Detection
		{
			// This function will return the radial distance between two collisions
			inline static float DistABRadii(Data::CollisionData* a, Data::CollisionData* b)
			{
				return glm::distance(a->a, b->a) - (a->r + b->r);	// Return radial distance  between object a and b
			}

			// This function will return the distance between point a and point b
			inline static float DistPointToPoint(glm::vec3& point_0, glm::vec3& point_1)
			{
				return glm::distance(point_0, point_1);		// Return final result
			}

			// This function will return a scalar value where magnitude between a point and an infinity plane
			inline static float DistPointToInfinityPlane(glm::vec3& point, Data::Triangle *plane)
			{
				return glm::abs(glm::dot(plane->n, plane->a - point));		// Return final result
			}

			// This function will return the dot product between a ray and plane normal
			inline static float DotRayToInfinityPlaneNormal(glm::vec3& ray, Data::Triangle* plane)
			{
				return -glm::dot(ray, plane->n);	// Return final result
			}

			// This function will return the distance between a point and infinity plane in respect to it's normal
			inline static float DistPointToInfinityPlaneNormal(glm::vec3& point, Data::Triangle* plane)
			{
				float r(0.0f);	// Initialise result

				float point_mag = glm::dot(plane->n, plane->a - point);		// Calculate mangnitude between point and infinity plane
				float normal_dot = glm::dot(-plane->n, plane->n);	// Calculate the dot of the infinity plane normal

				r = point_mag / normal_dot;		// Check distance

				return r;	// Return final result
			}

			// This function will return the distance between a point to an edges segement
			inline static float DistPointToEdge(glm::vec3& point, Data::Edge* edge)
			{
				float r(0.0f);	// Initialise result
				glm::vec3 normal = glm::cross(edge->d, point - edge->p_0);	// Calculate normal alignment between edge segment and point

				r = glm::length(normal);	// Assign result

				return r;	// Return final result
			}

			// This function will return the distance between two edge arbitraries
			inline static float DistEdgeToEdge(Data::Edge* edge_0, Data::Edge* edge_1)
			{
				glm::vec3 av = edge_0->v;	// Get the size of edge 0
				glm::vec3 bv = edge_1->v;	// Get the size of edge 1

				glm::vec3 normal = glm::cross(av, bv);	// Find the noraml between both line segments
				float normal_mag = glm::length(normal);		// Calculate the magnitude of normal

				glm::vec3 clamp_normal = normal * 1.0f / normal_mag;	// Clamp the unit normal to magnitude

				float dist = glm::abs(glm::dot(edge_0->p_0 - edge_1->p_0, clamp_normal));	// Get the absolute value of the magnitude between (e1p0 - e2p0) and the clamped normal

				return dist;	// Return result
			}
			
			// This function will return the Barycentric coordinate between a point to triangle in respect to a normal
			inline static glm::vec3 BaryCentricCoord(glm::vec3& point, glm::vec3& normal, Data::Triangle* triangle)
			{
				glm::vec3 r(glm::vec3(0.0f));	// Initialise result

				glm::vec3 v0 = triangle->p[0]->p - point;	// Define vector from point to triangle v0
				glm::vec3 v1 = triangle->p[1]->p - point;	// Define vector from point to triangle v1
				glm::vec3 v2 = triangle->p[2]->p - point;	// Define vector from point to triangle v2

				glm::vec3 v0v1 = glm::cross(v0, v1);	// Calculate normal from v0 and v1
				glm::vec3 v1v2 = glm::cross(v1, v2);	// Calculate normal from v1 and v2
				glm::vec3 v2v0 = glm::cross(v2, v0);	// Calculate normal from v2 and v0

				float v0v1m = glm::dot(v0v1, normal);	// Calculate magnitude for v0 to v1
				float v1v2m = glm::dot(v1v2, normal);	// Calculate magnitude for v0 to v1
				float v2v0m = glm::dot(v2v0, normal);	// Calculate magnitude for v0 to v1

				r = glm::vec3(v0v1m, v1v2m, v2v0m);		// Assign result

				return glm::vec3(v0v1m, v1v2m, v2v0m);	// Return barycentric position
			}

			// This function will check for point to edge projection intersection
			inline static bool PointInsideEdgeProjection(glm::vec3& point, Data::Edge* edge)
			{
				float el = glm::length(edge->v);	// Calculate length of edge
				float ip = glm::dot(point - edge->p_0, edge->v);	// Calculate inner product between length of edge and point minus edge p0

				return (ip >= 0.0f && ip <= el * el);	// Return result
			}

			// This function will check for point to triangle projection intersection
			inline static bool PointInsideTriangleProjection(glm::vec3& point, Data::Triangle* triangle)
			{
				glm::vec3 bcc = BaryCentricCoord(point, -triangle->n, triangle);	// Calculate barycentric coord with triangle normal

				return (glm::all(glm::lessThan(bcc, glm::vec3(0.0f))));		// Return result
			}
		}


		// A namespace for checking collision intersection
		namespace Intersection
		{
			// This function will return true if radii A has intersected with radii B
			inline static bool IntersectRadii(Data::CollisionData* a, Data::CollisionData* b)
			{
				return (Detection::DistABRadii(a, b) <= 0);		// Return statement
			}

			// This function will return true if verices intersect between object a and b
			inline static bool IntersectVertex(float speed_a, glm::vec3 vel_a, Data::CollisionData* obj_a, Data::CollisionData* obj_b)
			{
				// THINGS TO CHECK
				// 1. Check for nearby radii intersection
				// 2. Check for nearest triangles (facing toward veclotiy vector)
				// 3. When the distance is less or equal to zero, return true

				// --------------------- CHECK FOR NEARBY COLLISION ---------------------
				if (Detection::DistABRadii(obj_a, obj_b) <= speed_a)	// If distance between collision objects a and b are less or equal to the speed of velocity
				{
					// -------------------------------- POINTS --------------------------------
					for (unsigned int i = 0; i < obj_a->pd.size(); i++)		// Iterate through each point from object a
					{
						glm::vec3 p = obj_a->pd[i];		// Temp reference to object a, point i
						
						for (unsigned int j = 0; j < obj_b->t.size(); j++)	// Iterate through each triangle of collision object b
						{
							Triangle*	t = obj_b->t[j];	// Temp reference to object b, triangle j

							// ------------------------ CHECK FOR TRIANGLE INTERSECTION ------------------------
							if (glm::distance(t->a, p) <= (obj_b->r))	// If the distance between point i is less or equal to the radius of obj b
							{
								if (Detection::DotRayToInfinityPlaneNormal(vel_a, t) > 0.0f)	// If the nearby triangles from object b are facing object a's velocity vector
								{
									if (Detection::PointInsideTriangleProjection(p, t))		// If point i is within a triangle's projection
									{
										if (Detection::DistPointToInfinityPlane(p, t) <= 0.0f)	// If the distance between point i and a triangle is less or equal to zero
										{
											return true;	// Return true as point i has intersected with triangle j
										}
									}
								}
							}

							for (int k = 0; k < 3; k++)		// Iterate through each vertex (TRI = 3 vertices)
							{
								// ------------------------ CHECK FOR POINT INTERSECTION -----------------------
								if (Detection::DistPointToPoint())

								// ------------------------ CHECK FOR EDGE INTERSECTION ------------------------

							}
						}
					}
				
					
				}

				return false;	// Return false by default
			}
		}

		// A namespace for calculating collision response functions
		namespace Response
		{
			// This function will calculate a new velocity vector depending on nearby collision, and the amount of collisions
			inline static void CalculateNewVelocity(Data::CollisionData* a, Data::CollisionData* b)
			{

			}
		}
	}



}

#endif