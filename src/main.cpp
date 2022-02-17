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

using std::cout;
using std::cerr;
using std::endl;

// Free glut is apparently waaaay easier (and still maintained) compraed to GLUT, la lib qu'on a choisi pour surf mod.


#define debug std::cout<<"debug"<<std::endl;



const int APP_WIDTH = 1080;
const int APP_HEIGHT = 720;
const int NBODY_COUNT = 6;
const char* APP_NAME = "Test Freeglut";


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


// Ahhhh gl and oop
//Do the simul


// add window and initialize using glut (not glfw or stuff), glew apparently can be used to bind stuff

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
const static GLclampf sBodyColors[1][4] = {
   {0.9f, 0.1f, 0.2f, 1.0f}   // Dark red
};

NBody* simul = new NBody(NBODY_COUNT, 10., 2);


void drawBodies( CStopWatch *timeKeeper, M3DVector4f *lightPosition ) {
   // compute displacement and new vectors
   static float previousTime = 0.0f;
   //sleep(3); 
   float currentTime = timeKeeper->GetElapsedSeconds();
   simul->updatePhysics( currentTime - previousTime );
   previousTime = currentTime;

   for( int i = 0; i < NBODY_COUNT; i++ ) {
      // Save
      sModelViewMatrixStack.PushMatrix();
      sModelViewMatrixStack.Translate(0.0f, 0.0f, -1000.0f);
      sModelViewMatrixStack.Rotate(1.0f, 0.0f, 1.0f, 1.0f);

      // update position with regard to new values
      sBodyFrames[i].SetOrigin( simul->_bodies[i]->_position._x,
                                simul->_bodies[i]->_position._y,
                                simul->_bodies[i]->_position._z );
      // draw
      sModelViewMatrixStack.MultMatrix( sBodyFrames[i] );
      sShaderManager.UseStockShader( GLT_SHADER_POINT_LIGHT_DIFF,
                                     sTransformPipeline.GetModelViewMatrix(),
                                     sTransformPipeline.GetProjectionMatrix(),
                                     lightPosition,
                                     sBodyColors[0] );
      sBodyBatch[i].Draw();
      // Restore
      sModelViewMatrixStack.PopMatrix();
   }
   std::cout<<"====================================================================="<<std::endl;
   std::cout<<"The simulation at time "<<currentTime<<std::endl;
   simul->display_bodies();
}

void onRenderScene( void ) {
   // Clear the buffer
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   static CStopWatch timeKeeper;
   // Place camera
   M3DMatrix44f mCamera;
   sCameraFrame.GetCameraMatrix( mCamera );
   sModelViewMatrixStack.PushMatrix( mCamera );
   // Transform the light position into eye coordinates
   M3DVector4f lightPos = { sMainLightPos[0],
                            sMainLightPos[1],
                            sMainLightPos[2],
                            sMainLightPos[3]
                          };
   M3DVector4f lightEyePos;
   m3dTransformVector4( lightEyePos, lightPos, mCamera );
   // Call the drawing functions
   drawBodies( &timeKeeper, &lightEyePos );
   // Switch the buffers to bring the drawing on screen
   glutSwapBuffers();
   glutPostRedisplay();
}


void registerCallbacks() {
   glutReshapeFunc( onChangeSize );
   glutDisplayFunc( onRenderScene );
}

// Our function
void setupBodies() {
    

    for( int i = 0; i < simul->get_N(); i++ ) {
            // Porting the local variable to GPU
            sBodyRadius[i] = simul->_bodies[i]->_radius;

            gltMakeSphere( sBodyBatch[i], sBodyRadius[i], 30, 50 );
            sBodyFrames[i].SetOrigin( simul->_bodies[i]->_position._x,
                                        simul->_bodies[i]->_position._y,
                                        simul->_bodies[i]->_position._z );
    }
}


// Render context, things that are just in the tuto
void setupRenderContext() {
   sShaderManager.InitializeStockShaders();
   glEnable( GL_DEPTH_TEST );
   setupBodies();
   glClearColor( sBackgroundColor[0],
                 sBackgroundColor[1],
                 sBackgroundColor[2],
                 sBackgroundColor[3] );
   glEnable( GL_LINE_SMOOTH );
}




// Main loop (thanks to the eric dave tutorial)
int main( int argc, char **argv ) 
{
    //Do the OpenGL stuff

    setupWindow( argc, argv );
    registerCallbacks();
    // Initialize GLEW
    GLenum anError = glewInit();

    if( anError != 0 ) {
        fprintf( stderr, "GLEW Error: %s\n",
                glewGetErrorString( anError ) );

        if( DEBUG == true ) {
            cerr << " done" << endl;
        }

        return 1;
    }

    setupRenderContext();
    glutMainLoop();
    return 0;
}
