# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# configure import libs
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/../../External)

add_library( assimp SHARED IMPORTED )
set_target_properties( assimp
                       PROPERTIES IMPORTED_LOCATION
                       ${EXTERNAL_DIR}/lib/${ANDROID_ABI}/libassimp.so )

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
set(FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/../../Framework)

add_library( game SHARED
             ${FRAMEWORK_DIR}/FrameBuffer.cpp
             ${FRAMEWORK_DIR}/Game.cpp
             ${FRAMEWORK_DIR}/Mesh.cpp
             ${FRAMEWORK_DIR}/Platform.cpp
             ${FRAMEWORK_DIR}/Shader.cpp
             ${FRAMEWORK_DIR}/Texture.cpp
             ${FRAMEWORK_DIR}/Util.cpp
             src/main/cpp/Example.cpp
             src/main/cpp/JNI.cpp )

target_include_directories( game PRIVATE
                            ${EXTERNAL_DIR}/include
                            ${FRAMEWORK_DIR} )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( game
                       android
                       EGL
                       GLESv3
                       assimp

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )