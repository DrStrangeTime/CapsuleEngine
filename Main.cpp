#include "Window.h"		// Get WINAPI scope
#include "Console.h"	// Get access to our console for debugging
#include "Context.h"	// Get our device and rendering context
#include "TimeStep.h"	// Set up a timestep for our looping function
#include "RendererMaster.h"	// Include our renderer 

// This is the main WinAPI entry point for creating our window application
int WINAPI WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;	// Our message variable for returning callbacks
	TimeStepCounter tsc;	// Our timestep counter for smooth interpolation

	OpenConsole();	// Open the console for debugging purposes
	CreateWnd("Capsule Engine", 1920, 1080, true);	// Create our window
	Keyboard::Initialise();		// Initialise our keycodes
	RendererMaster::Initialise();		// Initialise our OpenGL renderer
	tsc.Initialise();	// Initialise our timestep

	while (_running)	// This is our looping function until _running == false
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // If we have a message to process, process it...
		{		
			if (msg.message == WM_QUIT)		// If the user has quite...
				_running = false;	// Set running to false if we have a message to quit  
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else	// If we don't have a message to process... 
		{
			tsc.Analyse();	// Begin analysing our timestep
			RendererMaster::Update(tsc._delta);
			RendererMaster::Render();		// Render our OpenGL world
			tsc.Reset();	// Reset the current delta time
		}
	}

	DestroyWnd();	// Destroy the window

	return (int)msg.wParam;		// Return the application message
}