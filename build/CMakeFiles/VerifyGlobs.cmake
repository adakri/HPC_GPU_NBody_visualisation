# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.16

# NBODY_HEADERS at CMakeLists.txt:33 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/abdel_dakri/work/Projet_GPGPU/include/*.cpp")
set(OLD_GLOB
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/abdel_dakri/work/Projet_GPGPU/build/CMakeFiles/cmake.verify_globs")
endif()

# NBODY_SRC at CMakeLists.txt:31 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/abdel_dakri/work/Projet_GPGPU/src/*.cpp")
set(OLD_GLOB
  "/home/abdel_dakri/work/Projet_GPGPU/src/Body.cpp"
  "/home/abdel_dakri/work/Projet_GPGPU/src/Vec3.cpp"
  "/home/abdel_dakri/work/Projet_GPGPU/src/main.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/abdel_dakri/work/Projet_GPGPU/build/CMakeFiles/cmake.verify_globs")
endif()