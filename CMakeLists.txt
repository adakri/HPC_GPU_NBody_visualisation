PROJECT(N_Body_Simulation_CUDA)

#The CMake Minimum version that is required. The FindCUDA script
#is distributed since version 3.8
CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
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
  src/Body.cpp src/Vec3.cpp src/Physics.cpp src/random_init.cpp src/NBody.cpp src/Viewer.cpp src/main.cpp
)




MESSAGE( "NBODY_SRC = " ${NBODY_SRC} )
#########################################################
# FIND OPENGL
#########################################################
set(OpenGL_GL_PREFERENCE GLVND) # Un warning bizarre sinon

find_package(OpenGL REQUIRED )
cuda_include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
else()
    message(STATUS "OPENGL included at ${OPENGL_INCLUDE_DIR}")
endif(NOT OPENGL_FOUND)
#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
cuda_include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
else()
    message(STATUS "GLUT included at ${GLUT_INCLUDE_DIR}")
endif(NOT GLUT_FOUND)
#########################################################
# FIND GLFW
#########################################################
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})
add_definitions(${GLFW_DEFINITIONS})
message(STATUS "GLFW included at ${GLFW_INCLUDE_DIR}")
#########################################################
# FIND GLM
#########################################################
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
#########################################################
# FIND SFML
#########################################################
find_package (SFML REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIRS})
link_directories(${SFML_LIBRARY_DIRS})
add_definitions(${SFML_DEFINITIONS})
if(NOT SFML_FOUND)
    message(ERROR " SFML not found!")
endif(NOT SFML_FOUND)



set(ALL_LIBRARIES ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLFW_LIBRARIES} ${GLM_LIBRAIRIES} ${SFML_LIBRARY})

add_executable( nbody ${NBODY_SRC} )

#CUDA_ADD_EXECUTABLE( nbody ${NBODY_SRC} ${NBODY_HEADERS} 
#                     OPTIONS ${GENCODE} ${DEVICE_DEBUGGING})

target_link_libraries(nbody ${ALL_LIBRARIES} )


       