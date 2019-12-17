cmake_minimum_required(VERSION 3.10)


#include头文件目录
include_directories(include/ncnn
                    src)

#source directory源文件目录
file(GLOB FACE_SRC src/*.h
                   src/*.cpp)


set(FACE_COMPILE_CODE ${FACE_SRC})

#添加ncnn库
add_library(ncnn STATIC IMPORTED )
set_target_properties(ncnn
  PROPERTIES IMPORTED_LOCATION
  e:/jna/lib/ncnn.lib)

#编译为动态库
add_library(Face SHARED ${FACE_COMPILE_CODE})

#添加工程所依赖的库
target_link_libraries( Face
                       ncnn
                       )

