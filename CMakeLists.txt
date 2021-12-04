PROJECT(N_Body_Simulation_CUDA)

#The CMake Minimum version that is required. The FindCUDA script
#is distributed since version 3.8
CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

#Searching CUDA
FIND_PACKAGE(CUDA)

MESSAGE( STATUS "N_body ray-traced- project is being generated" )

option(CUDA_DEVICE_DEBUGGING
  "On to enable debugging device code."
  OFF
  )  

if (CUDA_DEVICE_DEBUGGING)
  set(DEVICE_DEBUGGING -G0)
endif (CUDA_DEVICE_DEBUGGING)


include_directories(include)

SET( NBODY_SRC
  #.cu & .cpp
  src/Body.cpp src/Vec3.cpp src/Physics.cpp src/random_init.cpp src/NBody.cpp src/main.cpp
)




MESSAGE( "NBODY_SRC = " ${NBODY_SRC} )

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
cuda_include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
cuda_include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
#########################################################
# FIND GLFW
#########################################################
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})
add_definitions(${GLFW_DEFINITIONS})
#########################################################
# FIND GLM
#########################################################
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})


set(ALL_LIBRARIES ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${FREEGLUT_LIBRARY})

add_executable( nbody ${NBODY_SRC} )

#CUDA_ADD_EXECUTABLE( nbody ${NBODY_SRC} ${NBODY_HEADERS} 
#                     OPTIONS ${GENCODE} ${DEVICE_DEBUGGING})

target_link_libraries(nbody ${ALL_LIBRARIES} )


       