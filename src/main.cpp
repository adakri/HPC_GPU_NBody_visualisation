#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

// Project libs
#include "GLTools.h"
#include "GLFrustum.h"
#include "GLMatrixStack.h"
#include "GLGeometryTransform.h"
#include "GLShaderManager.h"
#include "StopWatch.h"
#include "math3d.h"

#include "Viewer.hpp"

using std::cerr;
using std::cout;
using std::endl;

#define debug std::cout << "debug" << std::endl;

/*
Choosing if to choose the naive simulation or the BH.
*/
//=============================
//=============================
bool BARNES = true;
//=============================
//=============================

/*
Window parameters and simulation size
*/

const int APP_WIDTH = 1080;
const int APP_HEIGHT = 720;
const int NBODY_COUNT = 400;
const char *APP_NAME = "==NBODY SIMULATION Sequential==";



// GL variables =========================================================================
static GLFrame sCameraFrame;

const static GLclampf sBackgroundColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
const static M3DVector4f sMainLightPos = {0.0f, 10.0f, 5.0f, 1.0f};

// A constant number of batches and bodies ==============================================
static GLTriangleBatch sBodyBatch[NBODY_COUNT];
static GLFrame sBodyFrames[NBODY_COUNT];
static GLfloat sBodyRadius[NBODY_COUNT];

// Variables

// Matrices and shaders ================================================================
static GLShaderManager sShaderManager;
static GLFrustum sViewFrustrum;
static GLMatrixStack sProjectionMatrixStack;
static GLMatrixStack sModelViewMatrixStack;
static GLGeometryTransform sTransformPipeline;

// Debugging
const bool DEBUG = false;

// GL and OOP is not a winning mix


// Doing the simulation ================================================================
// =====================================================================================

/*
Add window and initialize using glut, off screen rendering */

void setupWindow(int argc, char **argv)
{
   gltSetWorkingDirectory(argv[0]);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowPosition(500, 150);
   glutInitWindowSize(APP_WIDTH, APP_HEIGHT);
   glutCreateWindow(APP_NAME);
   // glutFullScreen();
}

///////////////////////////////////////////////////////////////////////
// Callbacks
// GL Camera constants
const static float APP_CAMERA_FOV = 50.0f;
const static float APP_CAMERA_NEAR = 1.0f;
const static float APP_CAMERA_FAR = 10000.f;


// The callback when dragging or rendering
void onChangeSize(int aNewWidth, int aNewHeight)
{
   glViewport(0, 0, aNewWidth, aNewHeight);
   sViewFrustrum.SetPerspective(APP_CAMERA_FOV,
                                float(aNewWidth) / float(aNewHeight),
                                APP_CAMERA_NEAR,
                                APP_CAMERA_FAR);
   sProjectionMatrixStack.LoadMatrix(
       sViewFrustrum.GetProjectionMatrix());
   sTransformPipeline.SetMatrixStacks(sModelViewMatrixStack,
                                      sProjectionMatrixStack);
}

///////////////////////////////////////////////////////////////////////
// Drawing
const static GLclampf sBodyColors[1][4] = {
    {0.9f, 0.1f, 0.2f, 1.0f} // Dark red is the color we choose
};

//=========================================================================================================
// Declaring a pointer to the used class (the smart solution would have been to add a default constructor).

NBody *simul = new NBody(NBODY_COUNT, 10., 2);

bool FIRST_TIME = true;


/**
 * @brief Drawing the bodies on screen at some time with a given light position
 * 
 * @param timeKeeper 
 * @param lightPosition 
 */
void drawBodies(CStopWatch *timeKeeper, M3DVector4f *lightPosition)
{
   // compute displacement and new vectors
   static float previousTime = 0.0f;
   // sleep(3);
   float currentTime = timeKeeper->GetElapsedSeconds();

   // catching the time
   std::ofstream log;
   log.open("../log.txt", std::ios_base::app);
   if (FIRST_TIME)
      log << "SERIAL N: " << simul->get_N() << " ";

   using namespace std::chrono;

   high_resolution_clock::time_point t1 = high_resolution_clock::now();

   if (BARNES)
   {
      simul->updatePhysics2(currentTime - previousTime);
      // copy all to cuda class
   }
   else
   {
      simul->updatePhysics(currentTime - previousTime);
      // simul->updatePhysics2( currentTime - previousTime );
   }

   high_resolution_clock::time_point t2 = high_resolution_clock::now();

   duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

   if (FIRST_TIME)
      log << time_span.count() << "\n";
   FIRST_TIME = false;
   log.close();

   previousTime = currentTime;

   for (int i = 0; i < NBODY_COUNT; i++)
   {
      // Save
      sModelViewMatrixStack.PushMatrix();
      sModelViewMatrixStack.Translate(0.0f, 0.0f, -1000.0f);
      sModelViewMatrixStack.Rotate(1.0f, 0.0f, 1.0f, 1.0f);

      // update position with regard to new values
      sBodyFrames[i].SetOrigin(simul->_bodies[i]->_position._x,
                               simul->_bodies[i]->_position._y,
                               simul->_bodies[i]->_position._z);
      // draw
      sModelViewMatrixStack.MultMatrix(sBodyFrames[i]);
      sShaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
                                    sTransformPipeline.GetModelViewMatrix(),
                                    sTransformPipeline.GetProjectionMatrix(),
                                    lightPosition,
                                    sBodyColors[0]);
      sBodyBatch[i].Draw();
      // Restore
      sModelViewMatrixStack.PopMatrix();
   }
   std::cout << "=====================================================================" << std::endl;
   std::cout << "The simulation at time " << currentTime << std::endl;
   simul->display_bodies();
}

static bool paused = false;


/**
 * @brief Calback on click.
 * 
 * @param key 
 * @param x 
 * @param y 
 */
void handleKeypress(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 27:
      exit(0);
   case 'p':
      paused = !paused;
      sleep(100);
      printf("Paused for 100s \n");
   }
}

/**
 * @brief Rendering function in GL convention.
 * 
 */
void onRenderScene(void)
{
   // Clear the buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   static CStopWatch timeKeeper;
   // Place camera
   M3DMatrix44f mCamera;
   sCameraFrame.GetCameraMatrix(mCamera);
   sModelViewMatrixStack.PushMatrix(mCamera);
   // Transform the light position into eye coordinates
   M3DVector4f lightPos = {sMainLightPos[0],
                           sMainLightPos[1],
                           sMainLightPos[2],
                           sMainLightPos[3]};
   M3DVector4f lightEyePos;
   m3dTransformVector4(lightEyePos, lightPos, mCamera);
   // Call the drawing functions
   drawBodies(&timeKeeper, &lightEyePos);
   // Switch the buffers to bring the drawing on screen
   glutSwapBuffers();
   glutPostRedisplay();
}

/**
 * @brief Registering callbacks.
 * 
 */
void registerCallbacks()
{
   glutReshapeFunc(onChangeSize);
   glutDisplayFunc(onRenderScene);
   glutKeyboardFunc(handleKeypress);
}

/**
 * @brief Drawing the bodies at first frame.
 * 
 */
void setupBodies()
{

   for (int i = 0; i < simul->get_N(); i++)
   {
      // Porting the local variable to GPU
      sBodyRadius[i] = simul->_bodies[i]->_radius;

      gltMakeSphere(sBodyBatch[i], sBodyRadius[i], 30, 50);
      sBodyFrames[i].SetOrigin(simul->_bodies[i]->_position._x,
                               simul->_bodies[i]->_position._y,
                               simul->_bodies[i]->_position._z);
   }
}

/**
 * @brief GL render context.
 * 
 */
void setupRenderContext()
{
   sShaderManager.InitializeStockShaders();
   glEnable(GL_DEPTH_TEST);
   setupBodies();
   glClearColor(sBackgroundColor[0],
                sBackgroundColor[1],
                sBackgroundColor[2],
                sBackgroundColor[3]);
   glEnable(GL_LINE_SMOOTH);
}

/**
 * @brief Main loop.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char **argv)
{
   // Do the OpenGL stuff

   setupWindow(argc, argv);
   registerCallbacks();
   // Initialize GLEW
   GLenum anError = glewInit();

   if (anError != 0)
   {
      fprintf(stderr, "GLEW Error: %s\n",
              glewGetErrorString(anError));

      if (DEBUG == true)
      {
         cerr << " done" << endl;
      }

      return 1;
   }

   setupRenderContext();
   glutMainLoop();
   return 0;
}
