#include "Physics.cuh"

#include <vector>
#include <chrono>

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
        void setUP_cuda();

        //getters and setters
        int get_N() const {return _N;}
        int getTimeSteps() const {return _timeSteps;}; 
};


// I did not know which extension to use .cpp or .cu ??
// déclarons les dans le meme fichier


//constructor
NBody_cuda::NBody_cuda(int Nu , float tf, int timeSteps): _N(Nu), _tf(tf), _timeSteps(timeSteps) 
{
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
void d_updateAcceleration(int index, vector *d_pos,vector *d_acc, Mass *d_mass, int _N) 
{
   
  vector netForce = { 0, 0, 0 };

  for( int i = 0; i < _N; i++ ) 
  {
    if( i == index ) 
    {
      continue;
    }

    vector vectorForceToOther = {0, 0, 0};

    Force scalarForceBetween = ComputeForce(
                                  d_mass[index],
                                  d_mass[i],
                                  d_pos[index],
                                  d_pos[i]);



    v_direction( d_pos[index],d_pos[i],vectorForceToOther);

    vectorForceToOther._x *= scalarForceBetween;
    vectorForceToOther._y *= scalarForceBetween;
    vectorForceToOther._z *= scalarForceBetween;

    netForce._x += vectorForceToOther._x;
    netForce._y += vectorForceToOther._y;
    netForce._z += vectorForceToOther._z;

    printf("The force %d: %f %f %f \n", index, netForce._x, netForce._y, netForce._z);
  }

  d_acc[index] = computeAcceleration3D(d_mass[index], netForce);

  printf("The accelerations: %d : %f %f %f \n", index,d_acc[index]._x, d_acc[index]._y, d_acc[index]._z);
};

__device__
void d_updateVelocity(int index, float deltaT, vector *d_acc, vector *d_vel) 
{
  d_vel[index] = computeVelocity3D(
                                d_acc[index],
                                d_vel[index],
                                deltaT);
  printf("The velocities: %d : %f %f %f \n", index,d_vel[index]._x, d_vel[index]._y, d_vel[index]._z);
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
  
  // 1D blocks and thread organisation
  int blockidx = blockIdx.x;
  int threadidx = threadIdx.x;

  int element_id = (blockidx * threadidx) + threadidx;

  printf("the element id %d \n", element_id);

  if(element_id > _N)
    return;

  //printf("%s \n", "===== The positions in the updatePhys b4==== \n");
  for(int i=1; i<element_id; i++)
  {
    d_pos++;
  }
  printf("@ %d %f, %f, %f \n",element_id,d_pos->_x,d_pos->_y,d_pos->_z);


  d_updateAcceleration(element_id, d_pos, d_acc, d_mass, _N);
  d_updateVelocity(element_id, deltaT, d_acc, d_vel);
  d_updatePosition(element_id, deltaT, d_vel, d_pos);

  //printf("%s \n", "===== The positions in the updatePhys==== \n");
  for(int i=0; i<element_id; i++)
  {
    d_pos++;
  }
  printf("* %d %f, %f, %f \n",element_id,d_pos->_x,d_pos->_y,d_pos->_z);
};


// The execution is very different 
void NBody_cuda::setUP_cuda()
{  

  // the C syntax
  int VECTOR_SIZE_IN_BYTES = _N * sizeof(vector);
  int SCALAR_SIZE_IN_BYTES = _N * sizeof(Scalar);

  //Initializing Velocities of N bodies in GPU
  vector *h_vel = nBodyVelocity;
  vector *d_vel;
  cudaMalloc((void**) &d_vel, VECTOR_SIZE_IN_BYTES);
  cudaMemcpy(d_vel, h_vel, VECTOR_SIZE_IN_BYTES, cudaMemcpyHostToDevice);

  //Initializing acceleration of N bodies in GPU
  vector *h_acc = nBodyAcceleration;
  vector *d_acc;
  cudaMalloc((void**) &d_acc, VECTOR_SIZE_IN_BYTES);
  cudaMemcpy(d_acc, h_acc, VECTOR_SIZE_IN_BYTES, cudaMemcpyHostToDevice);
  
  //Initializing Mass of N bodies in GPU
  Mass *h_mass = nBodyMass;
  Mass *d_mass;
  cudaMalloc((void**) &d_mass, SCALAR_SIZE_IN_BYTES);
  cudaMemcpy(d_mass, h_mass, SCALAR_SIZE_IN_BYTES, cudaMemcpyHostToDevice);

    //Initializing Positions of N bodies in GPU
  vector *h_pos = nBodyPosition;
  vector *d_pos;
  cudaMalloc((void**) &d_pos, VECTOR_SIZE_IN_BYTES);
  cudaMemcpy(d_pos, h_pos, VECTOR_SIZE_IN_BYTES, cudaMemcpyHostToDevice);


  std::cout<<"===== The starting positions===="<<std::endl;
  for(int i=0; i<_N; i++)
  {
    std::cout<<h_pos->_x<<","<<h_pos->_y<<","<<h_pos->_z<<std::endl;
    h_pos++;
  }



  for (int i = 0; i < 1; ++i)
  {
    printf("The number of 32 thread blocks %d \n", (int)ceil(_N/32));

    std::cout << "AT time step "<<(float)(100 * i) << std::endl;
    updatePhysics<<<(int)ceil(_N/32), 32>>>(_N, (float)(i * 100), d_pos, d_vel, d_acc, d_mass, _N);
  }
    

  cudaMemcpy(h_pos, d_pos, VECTOR_SIZE_IN_BYTES, cudaMemcpyDeviceToHost);
  cudaMemcpy(h_vel, d_vel, VECTOR_SIZE_IN_BYTES, cudaMemcpyDeviceToHost);
  cudaMemcpy(h_acc, d_acc, VECTOR_SIZE_IN_BYTES, cudaMemcpyDeviceToHost);
  cudaFree(d_pos);
  cudaFree(d_vel);
  cudaFree(d_acc);
  cudaFree(d_mass);

  std::cout<<"===== The Final positions===="<<std::endl;
  for(int i=0; i<_N; i++)
  {
    std::cout<<h_pos->_x<<","<<h_pos->_y<<","<<h_pos->_z<<std::endl;
    h_pos++;
  }

  

}

int main() 
{
  NBody_cuda nbody = NBody_cuda(N,1.,2);

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  
  nbody.setUP_cuda();
  
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  //Computing the time
  std::cout << "Time taken by the CUDA kernel is " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
}