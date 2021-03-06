# To use 
# cmake -DPARALLEL={OFF or On} -S ../ -B . (in build ofc)

cmake_minimum_required(VERSION 3.0.2)

option(PARALLEL "Enable cuda parallelism" OFF)

project(GPGPU)

SET(VERBOSE OFF)

# options de compilation
set(CMAKE_CXX_STANDARD 17)

if(VERBOSE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

# chemin vers les header files
include_directories(include)


# Ajouter les fichiers sources seqentiels
SET( NBODY_SRC
  src/Body.cpp src/Vec3.cpp src/random_init.cpp src/NBody.cpp src/Viewer.cpp src/Physics.cpp src/main.cpp
  OpenGL_libs/src/GLBatch.cpp OpenGL_libs/src/glew.c OpenGL_libs/src/GLShaderManager.cpp OpenGL_libs/src/GLTools.cpp OpenGL_libs/src/GLTriangleBatch.cpp OpenGL_libs/src/math3d.cpp
)

# Ajouter les fichiers sources CUDA
SET( NBODY_SRC_CUDA
  #.cu & .cpp
  src/Body.cpp src/Vec3.cpp src/random_init.cpp src/NBody.cpp src/Viewer.cpp src/Physics.cpp cuda_kernels/NBody_cuda.cu
  OpenGL_libs/src/GLBatch.cpp OpenGL_libs/src/glew.c OpenGL_libs/src/GLShaderManager.cpp OpenGL_libs/src/GLTools.cpp OpenGL_libs/src/GLTriangleBatch.cpp OpenGL_libs/src/math3d.cpp
)



# Here the building scheme diverges 

if(PARALLEL)
  message("================================")
  message("Using the CUDA parallelism build")
  message("================================")

  # CUDA Libraries
  find_package(CUDA)
  if(CUDA_FOUND)
    enable_language(CUDA)
    include_directories(cuda_kernels include)
    add_executable(nbody_cuda ${NBODY_SRC_CUDA})
  endif()
  # the GL libs family
  find_package(OpenGL REQUIRED)

  find_package(GLUT REQUIRED)

  # Detect and add libs et al.
  set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

  target_sources(nbody_cuda PRIVATE ${NBODY_SRC_CUDA})
  
  target_include_directories(nbody_cuda PRIVATE
    include
    cuda_kernels
    OpenGL_libs/include
    ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
    
  target_link_libraries(nbody_cuda PRIVATE
    ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} 
    )

else()
  message("========================")
  message("Using the serial version")
  message("========================")
  
  add_executable(nbody ${NBODY_SRC})

  # the GL libs family
  find_package(OpenGL REQUIRED)

  find_package(GLUT REQUIRED)

  # Detect and add libs et al.
  set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

  target_sources(nbody PRIVATE ${NBODY_SRC})

  target_include_directories(nbody PRIVATE
    include
    cuda_kernels
    OpenGL_libs/include
    ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

  target_link_directories(nbody PRIVATE)
    
  target_link_libraries(nbody PRIVATE
    ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} 
    )
endif()