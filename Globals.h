#ifndef __GLOBALS_H__
#define __GLOBALS_H__


#define RT_SPEED				0x0001	// The default real-time speed (1) is multiplied by delta

#define	CAMERA_NEAR				0.1f	// Define the default camera near plane
#define	CAMERA_FAR				2000.0f	// Define the default camera far plane
#define CAMERA_FOV				45.0f	// Define the default camera fov
#define CAMERA_SPEED			4.0f	// Define the default camera speed
#define CAMERA_LOOK_SENSITIVITY	0.4f	// Define the default camera sensitivity
#define CAMERA_PAN_SENSITIVITY	0.015f	// Define the default camera pan sensitivity
#define MOUSE_SENSITIVITY		1.0f	// Define the default mouse sensitivity


char*	_title = "Game Engine";	// Our windows title  
double	_rt_speed = 1;		// For adjusting the real-time speed rate

bool	_running = true;	// Whether or not the application is currently running
bool	_focused = true;	// Whether or not the application is currently focused

int		_pd_width = 0;		// Viewport width
int		_pd_height = 0;		// Viewport height

// Get the updated aspect ratio of the viewport
inline float GetUpdatedAspectRatio()
{
	return (float)_pd_width / (float)_pd_height;	// Return the viewport width / height
}

#endif