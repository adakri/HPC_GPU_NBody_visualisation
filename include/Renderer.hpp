#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include "Shader.hpp"

class Renderer
{

public:
	virtual void initWindow()=0;

	/**
	 * Initializes the gl state
	 * @param width viewport width
	 * @param height viewport height
	 * @param params simulation parameters
	 */
	virtual void init(GLFWwindow *window, int width, int height, SimParam params)=0;

	virtual void destroy()=0;

	/**
	 * Supplies the gl state with initial particle position and velocity
	 * @param pos particle positions
	 * @param vel particle velocities
	 */
	virtual void populateParticles( 
		const std::vector<glm::vec4> pos, 
		const std::vector<glm::vec4> vel)=0;

	/**
	 * Steps the simulation once, with the parameters provided with @see init
	 */
	virtual void stepSim()=0;

	/**
	 * Renders the particles at the current step
	 * @param proj_mat projection matrix @see camera_get_proj
	 * @param view_mat view matrix @see camera_get_view
	 */
	virtual void render(glm::mat4 projMat, glm::mat4 viewMat)=0;

};

class RendererGL : public Renderer
{
public:
  void initWindow();
  void init(GLFWwindow *window, int width, int height, SimParam params);
  void destroy();
  void populateParticles( 
      const std::vector<glm::vec4> pos, 
      const std::vector<glm::vec4> vel);
  void stepSim();
  void render(glm::mat4 proj_mat, glm::mat4 view_mat);

private:
  /// Provides the gl state with window dimensions for fbo size, etc
  void setWindowDimensions(int width, int height);

  /// Generates the star flare texture
  void createFlareTexture();

  /// Creates the VAO and VBO objects
  void createVaosVbos();

  /// Loads the shaders into the gl state
  void initShaders();

  // Initializes and supplies the framebuffers with valid data
  void initFbos();

  // Supplies the gl state with nbody simulation parameters
  void setUniforms(SimParam params);

  GLuint flareTex;           ///< Texture for the star flare
  GLuint vaoParticles;       ///< Vertex definition for points
  GLuint vboParticlesPos;    ///< Particle position buffer
  GLuint ssboVelocities;     ///< Particle velocity buffer
  GLuint vaoDeferred;        ///< Vertex definition for deferred
  GLuint vboDeferred;        ///< Vertex buffer of deferred fullscreen tri

  /** Shader programs **/
  ShaderProgram programInteraction; ///< Gravity interaction step
  ShaderProgram programIntegration; ///< Position integration step
  ShaderProgram programHdr;         ///< HDR rendering step
  ShaderProgram programBlur;        ///< Bloom blurring step
  ShaderProgram programLum;         ///< Average luminance step
  ShaderProgram programTonemap;     ///< Tonemapping step

  GLuint fbos[4];             ///< FBOs (0 for hdr, 1 & 2 for blur ping pong, 3 for luminance)
  GLuint attachs[4];          ///< Respective FBO attachments.

  int texSize;               ///< Flare texture size in pixels
  int lumLod;                ///< Luminance texture level to sample from
  int blurDownscale;         ///< Downscale factor for the blurring step
  int width_;                  ///< Viewport width
  int height_;                 ///< Viewport height

  size_t numParticles;
  size_t computeIterations;

};