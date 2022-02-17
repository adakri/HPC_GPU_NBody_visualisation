cmake_minimum_required(VERSION 3.0.2)

project(Surf_Modelling)

# options de compilation
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


# chemin vers les header files
include_directories(include)


# Ajouter les fichiers NBODY_SRC
SET( NBODY_SRC
  #.cu & .cpp
  src/Body.cpp src/Vec3.cpp src/random_init.cpp src/NBody.cpp src/Viewer.cpp src/Physics.cpp src/main.cpp 
  OpenGL_libs/src/GLBatch.cpp OpenGL_libs/src/glew.c OpenGL_libs/src/GLShaderManager.cpp OpenGL_libs/src/GLTools.cpp OpenGL_libs/src/GLTriangleBatch.cpp OpenGL_libs/src/math3d.cpp
)

add_executable(nbody ${NBODY_SRC})

# the GL libs family
find_package(OpenGL REQUIRED)

find_package(GLUT REQUIRED)

# Detect and add libs et al.
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})



target_sources(nbody PRIVATE ${NBODY_SRC})

target_include_directories(nbody PRIVATE
  external_libs/SFML-2.5.1/include 
  include
  cuda_kernels
  OpenGL_libs/include
  ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_directories(nbody PRIVATE external_libs/SFML-2.5.1/lib)
  
target_link_libraries(nbody PRIVATE
  sfml-graphics
  sfml-window
  sfml-system
  ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} 
  )