#ifndef __DEFERRED_H__
#define __DEFERRED_H__

#include "Rect.h"	// Get the rect object
#include "GBufferPass.h"	// Include the gbuffer
#include "GeometryPass.h"	// Get geometry pass
#include "LightPass.h"	// Get light pass
#include "SsaoPass.h"	// Get ssao pass
#include "CPicker.h" // Get bloom pass

#include "PBR.h"

#define VT_REALISTIC 	0 	// Final view
#define VT_UNLIT 		1 	// Albedo view
#define VT_WIRE 		2 	// Wire view
#define VT_DETAIL 		3 	// Normal detail view
#define VT_SSAO 		4 	// SSAO view

#define BLOOM_INTENSITY (float)0.6f		// Default bloom intensity

// This class contains all deffered rendering passes
class Deferred
{
public:
	static std::vector<ShaderProgram*>	shaders;	// The list of shader programs
	static std::vector<Pass*>			passes;		// The list of rendering passes
	static std::vector<PostFX*>			post_effects; // The list of post processing effects

	static Fbo*							_fbo_final;		// The final frame buffer

	static PBR::IBL* _ibl;

	static int8_t						_current_view_t;	// The current view type
	static GLuint						_u_view_type;		// Uniform for adjusting view types
	static GLuint						_final_textures[4];		// Both final result and bright colour textures
	static GLuint						_bloom_intensity;	// Bloom intensity value
public:
	// Initialise all deferred passes
	inline static void Initialise(size_t width, size_t height)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		 // Initialise screen rectangles
		_screen_rect = new Rect((double)width, (double)height, 1.0f, true);

		// Initialise geometry pass shader
		shaders.push_back(new ShaderProgram);
		shaders[0]->AddShaderAttachment("GBuffer.v", GL_VERTEX_SHADER);	// Geometry vertex
		shaders[0]->AddShaderAttachment("GBuffer.f", GL_FRAGMENT_SHADER);	// Geometry fragment
		shaders[0]->LinkProgram();		// Link both shader attachments

		passes.push_back(new GeometryPass({ shaders[0]->GetProgram() }));		// Initialise the geometry pass

		// Initialise light pass shader
		shaders.push_back(new ShaderProgram);
		shaders[1]->AddShaderAttachment("Rect.v", GL_VERTEX_SHADER);	// Screen rect shader
		shaders[1]->AddShaderAttachment("Light.f", GL_FRAGMENT_SHADER);	// Light fragment
		shaders[1]->LinkProgram();		// Link both shader attachments
		passes.push_back(new LightPass(shaders[1]->GetProgram()));	// Initialise the light pass

		// Get global uniforms from light shader
		_u_view_type = glGetUniformLocation(shaders[1]->GetProgram(), "view_type");		// Buffer view type

		// Initialise GBuffer pass
		passes.push_back(new GBufferPass(shaders[1]->GetProgram(), width, height));		// Initialise the gbuffer

		// Initialise ssao pass shaders
		shaders.push_back(new ShaderProgram);
		shaders[2]->AddShaderAttachment("Rect.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[2]->AddShaderAttachment("ssao.f", GL_FRAGMENT_SHADER);	// ssao fragment
		shaders[2]->LinkProgram();		// link both shader attachments
		shaders.push_back(new ShaderProgram);
		shaders[3]->AddShaderAttachment("Rect.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[3]->AddShaderAttachment("ssaoblur.f", GL_FRAGMENT_SHADER);	// ssaoblur fragment
		shaders[3]->LinkProgram();		// link both shader attachments
		passes.push_back(new SsaoPass({ shaders[2]->GetProgram(), shaders[3]->GetProgram() }, width, height, 32));	// initialise the light pass

		shaders.push_back(new ShaderProgram);
		shaders[4]->AddShaderAttachment("Rect.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[4]->AddShaderAttachment("GaussH.f", GL_FRAGMENT_SHADER);	// guassian blur fragment
		shaders[4]->LinkProgram();		// link both shader attachments
		shaders.push_back(new ShaderProgram);
		shaders[5]->AddShaderAttachment("Rect.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[5]->AddShaderAttachment("GaussV.f", GL_FRAGMENT_SHADER);	// guassian blur fragment
		shaders[5]->LinkProgram();		// link both shader attachments
		shaders.push_back(new ShaderProgram);
		shaders[6]->AddShaderAttachment("firstpass.v", GL_VERTEX_SHADER);	// firstpass vertex shader
		shaders[6]->AddShaderAttachment("firstpass.f", GL_FRAGMENT_SHADER);	// firstpass fragment
		shaders[6]->LinkProgram();		// link both shader attachments

		// initialise the final gather 
		shaders.push_back(new ShaderProgram);
		shaders[7]->AddShaderAttachment("Rect.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[7]->AddShaderAttachment("finalgather.f", GL_FRAGMENT_SHADER);	// final gather fragment
		shaders[7]->LinkProgram();		// link both shader attachments

		shaders.push_back(new ShaderProgram);
		shaders[8]->AddShaderAttachment("Rect.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[8]->AddShaderAttachment("sharpen.f", GL_FRAGMENT_SHADER);	// final gather fragment
		shaders[8]->LinkProgram();		// link both shader attachments

		shaders.push_back(new ShaderProgram);
		shaders[9]->AddShaderAttachment("colourID.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[9]->AddShaderAttachment("colourID.f", GL_FRAGMENT_SHADER);	// final gather fragment
		shaders[9]->LinkProgram();		// link both shader attachments

		shaders.push_back(new ShaderProgram);
		shaders[10]->AddShaderAttachment("light_scatter.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[10]->AddShaderAttachment("light_scatter.f", GL_FRAGMENT_SHADER);	// final gather fragment
		shaders[10]->LinkProgram();		// link both shader attachments
	
		shaders.push_back(new ShaderProgram);
		shaders[11]->AddShaderAttachment("radial_blur.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[11]->AddShaderAttachment("radial_blur.f", GL_FRAGMENT_SHADER);	// final gather fragment
		shaders[11]->LinkProgram();		// link both shader attachments

		shaders.push_back(new ShaderProgram);
		shaders[12]->AddShaderAttachment("HDR_Sky.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[12]->AddShaderAttachment("HDR_Sky.f", GL_FRAGMENT_SHADER);	// final gather fragment
		shaders[12]->LinkProgram();		// link both shader attachments

		shaders.push_back(new ShaderProgram);
		shaders[13]->AddShaderAttachment("EQU_HDR_Sky.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[13]->AddShaderAttachment("EQU_HDR_Sky.f", GL_FRAGMENT_SHADER);	// final gather fragment
		shaders[13]->LinkProgram();		// link both shader attachments

		shaders.push_back(new ShaderProgram);
		shaders[14]->AddShaderAttachment("convolution.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[14]->AddShaderAttachment("convolution.f", GL_FRAGMENT_SHADER);	// final gather fragment
		shaders[14]->LinkProgram();		// link both shader attachments

		shaders.push_back(new ShaderProgram);
		shaders[15]->AddShaderAttachment("convolution.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[15]->AddShaderAttachment("prefilter.f", GL_FRAGMENT_SHADER);	// final gather fragment
		shaders[15]->LinkProgram();		// link both shader attachments

		shaders.push_back(new ShaderProgram);
		shaders[16]->AddShaderAttachment("brdf.v", GL_VERTEX_SHADER);	// screen rect shader
		shaders[16]->AddShaderAttachment("brdf.f", GL_FRAGMENT_SHADER);	// final gather fragment
		shaders[16]->LinkProgram();		// link both shader attachments

		post_effects.push_back(new Bloom({ shaders[4]->GetProgram(), shaders[5]->GetProgram() }, width, height, BLOOM_INTENSITY));
		post_effects.push_back(new ColorCorrection(shaders[7]->GetProgram(), 2.5f, -0.02f));
		post_effects.push_back(new Shadowmapping({ shaders[6]->GetProgram(), shaders[4]->GetProgram(), shaders[5]->GetProgram() }, SHADOW_QUALITY));
		post_effects.push_back(new VolumetricLightPass({ shaders[10]->GetProgram(), shaders[11]->GetProgram(), shaders[4]->GetProgram(), shaders[5]->GetProgram() }, 1920, 1080));

		_fbo_final = new Fbo(width, height, { new FboAttachment(width, height, GL_RGBA, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0) }, false);

		_final_textures[0] = glGetUniformLocation(shaders[7]->GetProgram(), "scene"); // load in the scene texture from the final gather shader
		_final_textures[1] = glGetUniformLocation(shaders[7]->GetProgram(), "bloom"); // load in the bloom texture from the final gather shader
		_final_textures[2] = glGetUniformLocation(shaders[7]->GetProgram(), "lightScattering"); // load in the bloom texture from the final gather shader
		_final_textures[3] = glGetUniformLocation(shaders[7]->GetProgram(), "noiseTexture");
		_bloom_intensity = glGetUniformLocation(shaders[7]->GetProgram(),  "bloom_intensity");

		_ibl = new PBR::IBL("Milkyway_small.hdr", { shaders[13]->GetProgram(), shaders[14]->GetProgram(), shaders[15]->GetProgram(), shaders[16]->GetProgram() });

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);	// Clear background colour
	}

	// Destroy all allocated memory
	inline static void Destroy()
	{
		passes.clear();		// Destroy gbuffer data
		shaders.clear();	// Delete all shader programs
		post_effects.clear();
		
		delete _ibl;
		delete _fbo_final;

		if (_screen_rect) delete _screen_rect;	// Delete screen rectangle
	}

	// Update some deferred passes
	inline static void Update(double delta)
	{
		passes[GEOMETRY_PASS]->Update(delta);		// Update the geometry pass
	}

	// Render all deferred passes
	inline static void Render()
	{
		glDisable(GL_BLEND);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear last buffers

		//// ------------------------- SHADOWING PASS -------------------------- // 
		static_cast<Shadowmapping*>(post_effects[2])->Render();
		//// ------------------------------------------------------------------- //

		// ----------------------------- GEOMETRY ----------------------------- //
		passes[GBUFFER_PASS]->GetFbos()[0]->Bind();		// Bind the world gbuffer frame buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear last buffers

		passes[GEOMETRY_PASS]->Render();// Render geometry

		passes[GBUFFER_PASS]->GetFbos()[0]->Unbind();	// Unbind last frame buffer
		// ------------------------------------------------------------------- //

		//// ------------------------------ SSAO ------------------------------- //
		passes[SSAO_PASS]->Render();
		//// ------------------------------------------------------------------- //
		//

		static_cast<VolumetricLightPass*>(post_effects[3])->Render();

		// ------------------------------ LIGHT ------------------------------------- //
		{
			static_cast<Bloom*>(post_effects[0])->getFbo()->Bind(); // start to capture the scene into a fbo (frame buffer object)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			passes[GBUFFER_PASS]->Render();	// Render the gbuffer colour attachments
			passes[LIGHT_PASS]->Render();	// Parse the camera position

			glUniform1i(_u_view_type, _current_view_t);		// Assign global view type

			glm::mat4 shadow_bias = glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)) * glm::translate(glm::vec3(1.0, 1.0, 1.0));
			glm::mat4 shadow_matrix = shadow_bias * static_cast<Shadowmapping*>(post_effects[2])->getLightSpaceMatrix();
			
			glUniformMatrix4fv(((LightPass*)passes[LIGHT_PASS])->_u_lsm, 1, GL_FALSE, glm::value_ptr(static_cast<Shadowmapping*>(post_effects[2])->getLightSpaceMatrix()));
			glUniformMatrix4fv(((LightPass*)passes[LIGHT_PASS])->_u_shadow_matrix, 1, GL_FALSE, glm::value_ptr(shadow_matrix));

			glm::mat4 model;
			for (Actor* a : Content::_map->GetActors())		// Iterate through each actor
			{
				model = a->GetMatrix();	// Get model matrix
				glUniformMatrix4fv(((LightPass*)passes[LIGHT_PASS])->_u_mat, 1, GL_FALSE, glm::value_ptr(model));	// Send model matrix to buffer
			}

			glActiveTexture(GL_TEXTURE8);
			glBindTexture(GL_TEXTURE_2D, static_cast<Shadowmapping*>(post_effects[2])->_v_blur->GetAttachments()[0]->_texture);
			glActiveTexture(GL_TEXTURE9);
			glBindTexture(GL_TEXTURE_2D, ((SsaoPass*)passes[SSAO_PASS])->GetFbos()[1]->GetAttachments()[0]->_texture);

			_ibl->Render();

			_screen_rect->Render(1); // Render to screen rectangle

			static_cast<Bloom*>(post_effects[0])->getFbo()->Unbind();
			//// --------------------------------------------------------------------------- //
		}
		// -------------------------------------------------------------------------- // 

		// ------------------------------ POSTFX ------------------------------------- //
		{
			static_cast<Bloom*>(post_effects[0])->Render();
		}
		// --------------------------------------------------------------------------- //

		// Image Sharpening
		_fbo_final->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaders[8]->UseProgram(); // bind the final gather shader (using additive blending to blend all the postFX needed)
		glUniform1i(_final_textures[0], 0);	// Bind the final scene texture
		glActiveTexture(GL_TEXTURE0); // unit 1
		glBindTexture(GL_TEXTURE_2D, static_cast<Bloom*>(post_effects[0])->getFbo()->GetAttachments()[0]->_texture); // bind the scene texture
		_screen_rect->Render(1); // Render to screen rectangle
		_fbo_final->Unbind();

		// ------------------------------ FINAL GATHER ------------------------------- //
		shaders[7]->UseProgram(); // bind the final gather shader (using additive blending to blend all the postFX needed)

		glUniform1i(_final_textures[0], 0); // the captured scene texture
		glUniform1i(_final_textures[1], 1); // the bloom texture
		glUniform1i(_final_textures[2], 2); // the bloom texture
	
		glUniform1f(_bloom_intensity, static_cast<Bloom*>(post_effects[0])->GetBloomIntensity()); // bloom intensity (higher the value the less bloom)
		glUniform1f(static_cast<ColorCorrection*>(post_effects[1])->_u_contrast, static_cast<ColorCorrection*>(post_effects[1])->getContrast()); // contrast of the final scene
		glUniform1f(static_cast<ColorCorrection*>(post_effects[1])->_u_brightness, static_cast<ColorCorrection*>(post_effects[1])->getBrightness()); // brightness of the final scene

		glActiveTexture(GL_TEXTURE0); // unit 0
		glBindTexture(GL_TEXTURE_2D, _fbo_final->GetAttachments()[0]->_texture); // bind the scene texture
		glActiveTexture(GL_TEXTURE1); // unit 1
		glBindTexture(GL_TEXTURE_2D, static_cast<Bloom*>(post_effects[0])->_v_blur->GetAttachments()[0]->_texture); // bind the bloom texture
		glActiveTexture(GL_TEXTURE2); // unit 1
		glBindTexture(GL_TEXTURE_2D, static_cast<VolumetricLightPass*>(post_effects[3])->GetLightScatterTexture()); // bind the bloom texture

		_screen_rect->Render(1); // Render to screen rectangle
		// ---------------------------------------------------------------------------- //
		//
		//passes[GBUFFER_PASS]->GetFbos()[0]->ResolveToForward(passes[GBUFFER_PASS]->GetFbos()[0]);

		//glDisable(GL_CULL_FACE);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.

		//// shader 
		//shaders[12]->UseProgram();

		//// uniforms
		//glm::mat4 view = Content::_map->GetCamera()->GetViewMatrix();
		//glm::mat4 projection = glm::perspective(Content::_map->GetCamera()->GetFov(), (float)1920.0f / (float)1080.0f, 0.1f, 100.0f);
		//glUniformMatrix4fv(glGetUniformLocation(shaders[12]->GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(shaders[12]->GetProgram(), "proj"), 1, GL_FALSE, glm::value_ptr(projection));
		//glUniform1i(glGetUniformLocation(shaders[12]->GetProgram(), "environmentMap"), 0);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, _ibl->GetEnvironmentMap());
		//
		//Primitives::cube();
	}
};

// Define all static variables
std::vector<ShaderProgram*>		Deferred::shaders;
std::vector<Pass*>				Deferred::passes;		// The list of post effect passes
std::vector<PostFX*>			Deferred::post_effects; // The list of post processing effects

int8_t							Deferred::_current_view_t(0);
GLuint							Deferred::_u_view_type;
GLuint						    Deferred::_final_textures[4];
GLuint							Deferred::_bloom_intensity;
Fbo*							Deferred::_fbo_final;

PBR::IBL* Deferred::_ibl;

#endif