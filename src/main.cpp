#include <iostream>
#include <cstdlib>



#include "Viewer.hpp"




#define debug std::cout<<"debug"<<std::endl;

int main(int argc, char** argv)
{
    /* std::cout<<RAND_MAX<<std::endl;
    Vec3 v = randomParticlePos();
    v.isString(); */

    /* NBody* simul = new NBody(10, 10., 5);
    simul->simulate(); */

    glutInit(&argc, argv);  

    /* Select type of Display mode:   
        Double buffer 
        RGBA color
        Alpha components supported 
        Depth buffer */  
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  

    /* get a 640 x 480 window */
    glutInitWindowSize(640, 480);  

    /* the window starts at the upper left corner of the screen */
    glutInitWindowPosition(0, 0);  

    /* Open a window */  
    window = glutCreateWindow("GL"); 

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

    /* Register the function to do all our OpenGL drawing. */
    glutDisplayFunc(&DrawSphere);  

    /* Go fullscreen.  This is the soonest we could possibly go fullscreen. */
    //glutFullScreen();

    /* Even if there are no events, redraw our gl scene. */
    glutIdleFunc(&DrawGLScene);

    /* Register the function called when our window is resized. */
    glutReshapeFunc(&ReSizeGLScene);

    /* Register the function called when the keyboard is pressed. */
    glutKeyboardFunc(&keyPressed);

    /* Initialize our window. */
    InitGL(640, 480);
    
    /* Start Event Processing Engine */  
    glutMainLoop();  
    return EXIT_SUCCESS;
}