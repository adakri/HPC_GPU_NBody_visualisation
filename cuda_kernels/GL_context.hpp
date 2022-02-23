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


using std::cout;
using std::cerr;
using std::endl;

// Free glut is apparently waaaay easier (and still maintained) compraed to GLUT, la lib qu'on a choisi pour surf mod.


const int APP_WIDTH = 1080;
const int APP_HEIGHT = 720;
const int NBODY_COUNT = 1000;
const char* APP_NAME = "==NBODY SIMULATION CUDA==";


// GL variables
static GLFrame sCameraFrame;

const static GLclampf sBackgroundColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const static M3DVector4f sMainLightPos = { 0.0f, 10.0f, 5.0f, 1.0f };

static GLTriangleBatch sBodyBatch[NBODY_COUNT];
static GLFrame sBodyFrames[NBODY_COUNT];
static GLfloat sBodyRadius[NBODY_COUNT];


// Variables
// Matrices and shaders
static GLShaderManager sShaderManager;
static GLFrustum sViewFrustrum;
static GLMatrixStack sProjectionMatrixStack;
static GLMatrixStack sModelViewMatrixStack;
static GLGeometryTransform sTransformPipeline;

//Debugging
const bool DEBUG = false;

void setupWindow( int argc, char **argv ) 
{
   gltSetWorkingDirectory( argv[0] );
   glutInit( &argc, argv );
   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitWindowPosition(500, 150);
   glutInitWindowSize( APP_WIDTH, APP_HEIGHT );
   glutCreateWindow( APP_NAME );
   //glutFullScreen();
}

///////////////////////////////////////////////////////////////////////
// Callbacks
// GL constant
const static float APP_CAMERA_FOV = 50.0f;
const static float APP_CAMERA_NEAR = 1.0f;
const static float APP_CAMERA_FAR = 10000.f;


// The callback when dragging or rendering
void onChangeSize( int aNewWidth, int aNewHeight ) {
   glViewport( 0, 0, aNewWidth, aNewHeight );
   sViewFrustrum.SetPerspective( APP_CAMERA_FOV,
                                 float( aNewWidth ) / float( aNewHeight ),
                                 APP_CAMERA_NEAR,
                                 APP_CAMERA_FAR );
   sProjectionMatrixStack.LoadMatrix(
      sViewFrustrum.GetProjectionMatrix() );
   sTransformPipeline.SetMatrixStacks( sModelViewMatrixStack,
                                       sProjectionMatrixStack );
}

///////////////////////////////////////////////////////////////////////
// Drawing
const static GLclampf sBodyColors[4][4] = {
   {0.9f, 0.1f, 0.2f, 1.0f},
   {0.1f, 0.9f, 0.2f, 1.0f},
   {0.5f, 0.5f, 0.0f, 1.0f},
   {0.1f, 0.1f, 0.9f, 1.0f} };