#include "Physics.cuh"
#include "GL_context.hpp"

#include <vector>
#include <chrono>
#include<unistd.h>



bool DEMO_CUDA = true;




//https://gist.github.com/lebedov/bca3c70e664f54cdf8c3cd0c28c11a0f

class NBody_cuda 
{
    private:
        int _N;
        float _tf;
        int _timeSteps;
        float _deltaT;
    public:
        std::vector<Body*> _bodies;
        
        NBody_cuda(int, float, int);
        ~NBody_cuda() 
        { 
            for(int i=0; _bodies.size(); i++)
            {
                delete _bodies[i];
            }
        };

        void print_cycle(float);
        void print_sim();

        //Physics
        void display_bodies();
        void setUP_cuda(float , vector *, vector *, vector *, Mass *);

        //getters and setters
        int get_N() const {return _N;}
        int getTimeSteps() const {return _timeSteps;}; 
};


// I did not know which extension to use .cpp or .cu ??
// déclarons les dans le meme fichier


//constructor
NBody_cuda::NBody_cuda(int N_b , float tf, int timeSteps): _N(N_b), _tf(tf), _timeSteps(timeSteps) 
{
  _bodies.resize(_N);
  if(!DEMO_CUDA)
  {
    for(int i=1; i<_N; i++)
    {
      _bodies[i] = new Body(
        Vec3(nBodyPosition[i]._x,nBodyPosition[i]._y,nBodyPosition[i]._z),randomParticleVelocity(),
        Vec3(nBodyVelocity[i]._x, nBodyVelocity[i]._y, nBodyVelocity[i]._z),
        nBodyMass[i], random_radius()
                                    );
    }
  }else{
    printf("Initializing begun \n");
    _bodies[0] = new Body(Vec3(0.,0.,0.),
                                 Vec3(0.,0.,0.),
                                  randomParticleacceleration(),
                                  10e16,
                                  35
                                  );
    for(int i=1; i<_N; i++)
    {
      _bodies[i] = new Body(randomParticlePosition(),
                            randomParticleVelocity(),
                              randomParticleacceleration(),
                              random_mass(),
                              random_radius()
                              );
    }
  }
  
  std::cout<<"=====Constructed the NBody class in cuda===="<<std::endl;
};

// useless tbh
void NBody_cuda::display_bodies()
{

  for( int i = 0; i < _N; i++ ) 
  {
    printf("\nBody %d:\nMass: %f\nPosition(x ,y, z): %f, %f, %f\nVelocity(x, y, z): %f, %f, %f\nAcceleration(x ,y, z): %f, %f, %f\n\n",
      i + 1, 
      _bodies[i]->_mass,
      _bodies[i]->_position._x, _bodies[i]->_position._y, _bodies[i]->_position._z,

      _bodies[i]->_velocity._x, _bodies[i]->_velocity._y, _bodies[i]->_velocity._z,
      _bodies[i]->_acceleration._x, _bodies[i]->_acceleration._y, _bodies[i]->_acceleration._z);
  }
};


// compute acceleration per thread
__device__
void d_updateAcceleration(int index, vector *d_pos,vector* d_vel, vector *d_acc, Mass *d_mass, int _N) 
{
   
  vector netForce = { 0, 0, 0 };

  for( int i = 0; i < _N; i++ ) 
  {
    if( i == index ) 
    {
      continue;
    }

    vector vectorForceToOther = {0., 0., 0.};

    Force scalarForceBetween = ComputeForce(
                                  d_mass[index],
                                  d_mass[i],
                                  d_pos[index],
                                  d_pos[i],
                                  d_vel[index]);

    //printf("scalarForceBetween %d : %d %f \n",index, i, scalarForceBetween);

    v_direction( d_pos[index],d_pos[i],vectorForceToOther);

    vectorForceToOther._x *= scalarForceBetween;
    vectorForceToOther._y *= scalarForceBetween;
    vectorForceToOther._z *= scalarForceBetween;

    //printf("The vectorForceToOther %d: %d: %f %f %f \n", index, i, vectorForceToOther._x, vectorForceToOther._y, vectorForceToOther._z);

    netForce._x += vectorForceToOther._x;
    netForce._y += vectorForceToOther._y;
    netForce._z += vectorForceToOther._z;

    //printf("The netForce %d: %d: %f %f %f \n", index, i, netForce._x, netForce._y, netForce._z);
  }

  d_acc[index] = computeAcceleration3D(d_mass[index], netForce);

  //printf("The accelerations: %d : %f %f %f \n", index,d_acc[index]._x, d_acc[index]._y, d_acc[index]._z);
};

__device__
void d_updateVelocity(int index, float deltaT, vector *d_acc, vector *d_vel) 
{
  d_vel[index] = computeVelocity3D(
                                d_acc[index],
                                d_vel[index],
                                deltaT);
  //printf("The velocities: %d : %f %f %f \n", index,d_vel[index]._x, d_vel[index]._y, d_vel[index]._z);
};

__device__
void d_updatePosition(int index, float deltaT, vector *d_vel, vector *d_pos) 
{

  d_pos[index] = computePosition3D( 
                              d_vel[index],
                              d_pos[index],
                              deltaT);
  printf("The positions: %d : %f %f %f \n", index,d_pos[index]._x, d_pos[index]._y, d_pos[index]._z);
};


__global__
void updatePhysics(int bodies, float deltaT, vector *d_pos, vector *d_vel, vector *d_acc, Mass *d_mass, int _N)
{
  printf("Inside the update physics \n");
  // 1D blocks and thread organisation
  int blockidx = blockIdx.x;
  int threadidx = threadIdx.x;

  int element_id = (blockidx * threadidx) + threadidx;

  //printf("the element id %d \n", element_id);

  if(element_id > _N)
    return;

  //printf("%s \n", "===== The positions in the updatePhys b4==== \n");
  /* for(int i=1; i<element_id; i++)
  {
    d_pos++;
  } */
  //printf("@ %d %f, %f, %f \n",element_id,d_pos->_x,d_pos->_y,d_pos->_z);


  d_updateAcceleration(element_id, d_pos, d_vel, d_acc, d_mass, _N);
  d_updateVelocity(element_id, deltaT, d_acc, d_vel);
  d_updatePosition(element_id, deltaT, d_vel, d_pos);

  //printf("%s \n", "===== The positions in the updatePhys==== \n");
  /* printf("* %d %f, %f, %f \n",element_id,(d_pos+element_id)->_x,(d_pos+element_id)->_y,(d_pos+element_id)->_z); */

};


// Determine first time acess
bool FIRST_TIME = true;


// The execution is very different 
// Compute through GPU the update at time time and update the body ppoisitons
void NBody_cuda::setUP_cuda(float time, vector *h_pos, vector *h_vel, vector *h_acc, Mass *h_mass)
{  

  // the C syntax
  int VECTOR_SIZE_IN_BYTES = _N * sizeof(vector);
  int SCALAR_SIZE_IN_BYTES = _N * sizeof(Scalar);

  //Initializing Velocities of N bodies in GPU
  //vector *h_vel = nBodyVelocity;
  vector *d_vel;
  cudaMalloc((void**) &d_vel, VECTOR_SIZE_IN_BYTES);
  cudaMemcpy(d_vel, h_vel, VECTOR_SIZE_IN_BYTES, cudaMemcpyHostToDevice);

  //Initializing acceleration of N bodies in GPU
  //vector *h_acc = nBodyAcceleration;
  vector *d_acc;
  cudaMalloc((void**) &d_acc, VECTOR_SIZE_IN_BYTES);
  cudaMemcpy(d_acc, h_acc, VECTOR_SIZE_IN_BYTES, cudaMemcpyHostToDevice);
  
  //Initializing Mass of N bodies in GPU
  //Mass *h_mass = nBodyMass;
  Mass *d_mass;
  cudaMalloc((void**) &d_mass, SCALAR_SIZE_IN_BYTES);
  cudaMemcpy(d_mass, h_mass, SCALAR_SIZE_IN_BYTES, cudaMemcpyHostToDevice);

  //Initializing Positions of N bodies in GPU
  //vector *h_pos = nBodyPosition;
  vector *d_pos;
  cudaMalloc((void**) &d_pos, VECTOR_SIZE_IN_BYTES);
  cudaMemcpy(d_pos, h_pos, VECTOR_SIZE_IN_BYTES, cudaMemcpyHostToDevice);


  // Print and update
  /*  std::cout<<"===== The starting positions===="<<std::endl;
  for(int i=0; i<_N; i++)
  {
    std::cout<<(h_pos+i)->_x<<","<<(h_pos+i)->_y<<","<<(h_pos+i)->_z<<std::endl;
  } */



  //exit(0);

  printf("The number of 32 thread blocks %d \n", (int)ceil(_N/32));

  std::cout << "AT time "<<time << std::endl;
  
  updatePhysics<<<(int)ceil(_N/32), 32>>>(_N, time*100., d_pos, d_vel, d_acc, d_mass, _N);

    

  cudaMemcpy(h_pos, d_pos, VECTOR_SIZE_IN_BYTES, cudaMemcpyDeviceToHost);
  cudaMemcpy(h_vel, d_vel, VECTOR_SIZE_IN_BYTES, cudaMemcpyDeviceToHost);
  cudaMemcpy(h_acc, d_acc, VECTOR_SIZE_IN_BYTES, cudaMemcpyDeviceToHost);
  cudaFree(d_pos);
  cudaFree(d_vel);
  cudaFree(d_acc);
  cudaFree(d_mass);

  /* std::cout<<"===== The Final positions===="<<std::endl;
  for(int i=0; i<_N; i++)
  {
    std::cout<<h_pos->_x<<","<<h_pos->_y<<","<<h_pos->_z<<std::endl;
    _bodies[i]->_position = {h_pos->_x, h_pos->_y,h_pos->_z};
    h_pos++; 
  } */
  
  //printf("Finsihed Modifying the positions, thses ones \n");
  for(int i=0; i<_N; i++)
  {
    //std::cout<<(h_pos+i)->_x<<","<<(h_pos+i)->_y<<","<<(h_pos+i)->_z<<std::endl;
    _bodies[i]->_position = {(h_pos+i)->_x, (h_pos+i)->_y,(h_pos+i)->_z};
  }
}


// Class pointer
NBody_cuda* nbody_cuda = new NBody_cuda(Nu,10.,2);


//Initializing Qs of N bodies in GPU
vector *h_vel = nBodyVelocity;
vector *h_acc = nBodyAcceleration;
Mass *h_mass = nBodyMass;
vector *h_pos = nBodyPosition;


// Drawing the bodies through a class pointer is impossibléééé !!

void drawBodies( CStopWatch *timeKeeper, M3DVector4f *lightPosition) {
  // compute displacement and new vectors
  static float previousTime = 0.0f;
  //sleep(3); 
  float currentTime = timeKeeper->GetElapsedSeconds();

  printf("In the set up of the bodies \n");
  

  /* std::cout<<"===== The Present positions===="<<std::endl;
  for(int i=0; i<Nu; i++)
  {
    std::cout<<(h_pos+i)->_x<<","<<(h_pos+i)->_y<<","<<(h_pos+i)->_z<<std::endl;
  } */

  std::cout<<"The time "<<currentTime<<std::endl;

  nbody_cuda->setUP_cuda( currentTime, h_pos, h_vel, h_acc,h_mass );
  
  previousTime = currentTime;

  std::cout<<"#### In the GL Draw ####"<<std::endl;

  for( int i = 0; i < nbody_cuda->get_N() ; i++ ) {
    // Save
    sModelViewMatrixStack.PushMatrix();
    sModelViewMatrixStack.Translate(0.0f, 0.0f, -1000.0f);
    sModelViewMatrixStack.Rotate(1.0f, 0.0f, 1.0f, 1.0f);

    // update position with regard to new values
    sBodyFrames[i].SetOrigin( nbody_cuda->_bodies[i]->_position._x,
                              nbody_cuda->_bodies[i]->_position._y,
                              nbody_cuda->_bodies[i]->_position._z );

    std::cout<<nbody_cuda->_bodies[i]->_position._x<<","<<
            nbody_cuda->_bodies[i]->_position._y<<","<<
            nbody_cuda->_bodies[i]->_position._z<<std::endl; 
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
}

static bool paused = false;

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
   glutKeyboardFunc(handleKeypress);
}


// Our function
void setupBodies()
{
    std::cout<<"#### In the GL ####"<<std::endl;
    for( int i = 0; i < nbody_cuda->get_N(); i++ ) {
            // Porting the local variable to GPU
            sBodyRadius[i] = nbody_cuda->_bodies[i]->_radius;

            gltMakeSphere( sBodyBatch[i], sBodyRadius[i], 30, 50 );
            sBodyFrames[i].SetOrigin( 
            nbody_cuda->_bodies[i]->_position._x,
            nbody_cuda->_bodies[i]->_position._y,
            nbody_cuda->_bodies[i]->_position._z 
            );
            std::cout<<nbody_cuda->_bodies[i]->_position._x<<","<<
            nbody_cuda->_bodies[i]->_position._y<<","<<
            nbody_cuda->_bodies[i]->_position._z<<std::endl; 
    }
    std::cout<<std::endl;
}


// Render context, things that are just in the tuto
void setupRenderContext() 
{
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

    printf("Set up the window \n");
    setupWindow( argc, argv );

    printf("Reading callbacks (none for now) \n");
    if(!paused)
    {
      registerCallbacks();
    }
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


    printf("The GL context \n");
    setupRenderContext();

    printf("The main loop \n");
    glutMainLoop();
    return 0;
}


/* int main() 
{
  NBody_cuda nbody = NBody_cuda(Nu,1.,2);

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  
  
  
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  //Computing the time
  std::cout << "Time taken by the CUDA kernel is " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
} */