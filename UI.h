#ifndef __UI_H__
#define __UI_H__UI

#include "ALConsole.h"	// Get ALConsole controls

// This class will statically handle all UI elements to the viewport
class UI
{
public:
	static ShaderProgram*			_text_shader;	// For our text rendering
	static std::vector<Control*>	_controls;	// The list of UI controls

	// This will initialise all UI components
	inline static void Initialise()
	{
		_text_shader = new ShaderProgram();		// Initialise the text shader program
		_text_shader->AddShaderAttachment("Text.v", GL_VERTEX_SHADER);	// Add a shader attachment
		_text_shader->AddShaderAttachment("Text.f", GL_FRAGMENT_SHADER);	// Add another a shader attachment
		_text_shader->LinkProgram();	// Link attachments and create shader program
		
		Content::_fonts.push_back(new Font(_text_shader->GetProgram(), { "core_r[16].dds" }, { 256, 96 }));		// Import the engine's default font

		_controls.push_back(new ALConsole(_text_shader, glm::vec2(36.0f, 1048.0f)));	// Initialise the ALConsole to the UI system
	}

	inline static void Callback(UINT &message, WPARAM &wParam)
	{
		for (Control* c : _controls)	// Iterate through each control
			c->Callback(message, wParam);	// Update the callback
	}


	// Update the UI
	inline static void Update(double &delta)
	{
		for (Control* c : _controls)	// Iterate through each control
		{
			if (c->active)	// If a control is active...
				c->Update(delta);	// Render the text control
		}
	}

	// Render the UI
	inline static void Render()
	{
		_text_shader->UseProgram();		// Bind the text shader
		
		for (Control* c : _controls)	// Iterate through each control
			c->Render();	// Render the text control
		
	}

	// This will destroy all UI components
	inline static void Destroy()
	{
		_controls.clear();	// Delete all controls
		delete _text_shader;	// Delete text shader
	}
};

// Static definitions
ShaderProgram*			UI::_text_shader;
std::vector<Control*>	UI::_controls;

#endif