#
# CMakeLists.txt
#
# Copyright (C) 2015, GreenSocs ltd.
#
# Developped by Frederic Konrad <fred.konrad@greensocs.com>
# and Guillaume Delbrgue <guillaume.delbergue@greensocs.com>
#

cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

project(NVDLA_VIRTUAL_PLATFORM)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(INCLUDE_DIRS include)

include_directories(${INCLUDE_DIRS})

if (AWS_FPGA_PRESENT)
    set(AWS_FPGA_CFLAGS "-DAWS_FPGA_PRESENT=1")
else ()
    set(AWS_FPGA_CFLAGS "-DAWS_FPGA_PRESENT=0")
endif ()

if (REGISTER_ACCESS_TRACE)
    set(TRACE_CFLAGS "-DREGISTER_ACCESS_TRACE=1")
endif ()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSC_INCLUDE_DYNAMIC_PROCESSES -Wall  ${AWS_FPGA_CFLAGS} ${TRACE_CFLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSC_INCLUDE_DYNAMIC_PROCESSES -Wall  -DDEBUG_LOG=1 ${AWS_FPGA_CFLAGS} ${TRACE_CFLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSC_INCLUDE_DYNAMIC_PROCESSES -Wall  ${AWS_FPGA_CFLAGS} ${TRACE_CFLAGS}")

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

#REQUIRED LIBRARIES.
find_package(SystemC)
if(SystemC_FOUND)
    include_directories(${SystemC_INCLUDE_DIRS})
else()
    message( FATAL_ERROR "SystemC library not found." )
endif()

find_package(TLM)
if(TLM_FOUND)
    include_directories(${TLM_INCLUDE_DIRS})
else()
    message( FATAL_ERROR "TLM library not found." )
endif()

find_package(Lua REQUIRED)
if(LUA_FOUND)
    include_directories(${LUA_INCLUDE_DIR})
else()
    message( FATAL_ERROR "LUA library not found." )
endif()

# find_package(NvdlaCmod)
# if(NVDLA_CMOD_FOUND)
#     link_directories(${NVDLA_CMOD_LIBRARY_DIR})
# else()
#     message( FATAL_ERROR "NVDLA CMOD library not found." )
# endif()

# if(AWS_FPGA_PRESENT)
#     find_package(AWSSDK)
#     if(AWSSDK_FOUND)
#         include_directories(${AWSSDK_INCLUDE_DIRS})
#         link_directories(${AWSSDK_LIBRARY_DIRS})
#     else()
#         message( FATAL_ERROR "AWSSDK library not found." )
#     endif()
# endif()

add_subdirectory(libs/greenlib)

include_directories(libs/greenlib/greencontrol/include
		    libs/greenlib/greenmemory/include
		    libs/greenlib/greenscript/include
		    libs/greenlib/greensignalbus/include
		    libs/greenlib/greensocket/include
		    libs/greenlib/greenmessage/include
		    libs/greenlib/greenrouter/include
		    libs/greenlib/greenserialsocket/include
		    libs/greenlib/greensignalsocket/include
		    libs/greenlib/gsgpsocket/include
		    libs/greenlib/greenreg/include
		    libs/greenlib/include
)
link_directories(${CMAKE_SOURCE_DIR}/libs/greenlib/greenreg/lib
		 ${CMAKE_SOURCE_DIR}/libs/greenlib/greencontrol/lib
		 ${CMAKE_SOURCE_DIR}/libs/greenlib/greenmemory/lib
		 ${CMAKE_SOURCE_DIR}/libs/greenlib/greenscript/lib
		 ${CMAKE_SOURCE_DIR}/libs/greenlib/greensignalbus/lib
		 ${CMAKE_SOURCE_DIR}/libs/greenlib/greensocket/lib
		 ${CMAKE_SOURCE_DIR}/libs/greenlib/greenmessage/lib
		 ${CMAKE_SOURCE_DIR}/libs/greenlib/greenrouter/lib
		 ${CMAKE_SOURCE_DIR}/libs/greenlib/greenserialsocket/lib
		 ${CMAKE_SOURCE_DIR}/libs/greenlib/greensignalsocket/lib
		 ${CMAKE_SOURCE_DIR}/libs/greenlib/gsgpsocket/lib
)

set(GREENLIB_FOUND TRUE)
set(GREENLIB_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/libs/greenlib/greenreg/lib)
set(GREENLIB_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs/greenlib/greencontrol/include)
set(GREENLIB_FOUND TRUE)
set(GREENLIB_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/libs/greenlib/greenreg/lib)
set(GREENLIB_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs/greenlib/greencontrol/include)

add_subdirectory(libs/tlm2c libs/tlm2c.build/lib)
include_directories(libs/tlm2c/include)
link_directories(${CMAKE_BINARY_DIR}/libs/tlm2c.build)
file(COPY libs/tlm2c/include DESTINATION libs/tlm2c.build/)

set(TLM2C_FOUND TRUE)
set(TLM2C_LIBRARY_DIRS ${CMAKE_BINARY_DIR}/libs/tlm2c.build)
set(TLM2C_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/libs/tlm2c/include)

add_subdirectory(libs/log libs/log.build/lib)
include_directories(libs/log/include)
link_directories(${CMAKE_BINARY_DIR}/libs/log.build)
file(COPY libs/log/include DESTINATION libs/log.build/)

set(LOG_FOUND TRUE)
set(LOG_LIBRARY_DIRS ${CMAKE_BINARY_DIR}/libs/log.build)
set(LOG_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/libs/log/include)

add_subdirectory(models/cpu models/cpu.build)
include_directories(models/cpu/include)
link_directories(${CMAKE_BINARY_DIR}/models/cpu.build)

set(SIMPLECPU_FOUND TRUE)
set(SIMPLECPU_LIBRARY_DIRS ${CMAKE_BINARY_DIR}/models/cpu.build)
set(SIMPLECPU_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/models/cpu/include)

add_subdirectory(models/memory models/memory.build)
include_directories(models/memory/include)
link_directories(${CMAKE_BINARY_DIR}/models/memory.build)

# add_subdirectory(models/nvdla models/nvdla.build)
# include_directories(models/nvdla/include)
# link_directories(${CMAKE_BINARY_DIR}/models/nvdla.build)

# if(AWS_FPGA_PRESENT)
#     add_subdirectory(fpga/aws-fpga/fpga_sc_wrapper fpga/aws-fpga/fpga_sc_wrapper.build)
#     include_directories(fpga/aws-fpga/fpga_sc_wrapper/include)
#     link_directories(${CMAKE_BINARY_DIR}/fpga/aws-fpga/fpga_sc_wrapper.build)
# endif()

# add_subdirectory(fpga/aws-fpga/cosim_sc_wrapper fpga/aws-fpga/cosim_sc_wrapper.build)
# include_directories(fpga/aws-fpga/cosim_sc_wrapper/include)
# link_directories(${CMAKE_BINARY_DIR}/fpga/aws-fpga/cosim_sc_wrapper.build)

### QBOX ###
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(QBOX_DEBUG_BUILD --enable-debug)
else ()
    set(QBOX_DEBUG_BUILD --disable-debug-info)
endif ()
ExternalProject_Add(qbox
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/qbox
	INSTALL_DIR ${CMAKE_BINARY_DIR}/libs/qbox.build
	BINARY_DIR ${CMAKE_BINARY_DIR}/libs/qbox.build
	TMP_DIR ${CMAKE_BINARY_DIR}/libs/qbox.build/tmp
	STAMP_DIR ${CMAKE_BINARY_DIR}/libs/qbox.build/stampdir
	CONFIGURE_COMMAND ${CMAKE_SOURCE_DIR}/libs/qbox/configure --disable-werror --disable-pa --disable-linux-aio --disable-rbd  --disable-vnc --disable-sdl --disable-sdl-image --target-list=riscv64-softmmu,riscv32-softmmu --qbox --library ${QBOX_DEBUG_BUILD} --with-tlm2c=${CMAKE_BINARY_DIR}/libs/tlm2c.build --prefix=${CMAKE_BINARY_DIR}/libs/qbox.build --disable-pie
	PREFIX ${CMAKE_BINARY_DIR}/libs/qbox.build
	BUILD_COMMAND $(MAKE)
)
ExternalProject_Add_Step(qbox force-build
            DEPENDEES update
            DEPENDERS build
            ALWAYS 1
)
add_dependencies(qbox tlm2c)
link_directories(${CMAKE_BINARY_DIR}/libs/qbox.build/)
install(FILES ${CMAKE_BINARY_DIR}/libs/qbox.build/libqbox-riscv64-softmmu${CMAKE_SHARED_LIBRARY_SUFFIX} DESTINATION lib)
install(FILES ${CMAKE_BINARY_DIR}/libs/qbox.build/libqbox-riscv32-softmmu${CMAKE_SHARED_LIBRARY_SUFFIX} DESTINATION lib)

### Toplevel ###
add_executable(riscv_toplevel src/riscv_toplevel.cpp)
add_dependencies(riscv_toplevel
		simplecpu
		qbox
		#   nvdla
)

# install system c library
# get all systemc libraries including symbolic links
file(GLOB SYSC_LIBS "${SystemC_LIBRARIES}*")
install(FILES ${SYSC_LIBS} DESTINATION lib)

target_link_libraries(riscv_toplevel
		${SystemC_LIBRARIES}
		${LUA_LIBRARIES}
		qbox-riscv64-softmmu
		greenreg
		greenscript
		simplecpu
		dl
		tlm2c
		log
		# nvdla
)

# if(AWS_FPGA_PRESENT)
#     add_dependencies(riscv_toplevel fpga_sc_wrapper)
#     target_link_libraries(riscv_toplevel fpga_sc_wrapper)
#     set_target_properties(riscv_toplevel PROPERTIES COMPILE_FLAGS " -Wno-deprecated-declarations -DCONFIG_LOGLEVEL=4")
# endif()

# add_dependencies(riscv_toplevel cosim_sc_wrapper)
# target_link_libraries(riscv_toplevel cosim_sc_wrapper)
# set_target_properties(riscv_toplevel PROPERTIES COMPILE_FLAGS " -Wno-deprecated-declarations -DCONFIG_LOGLEVEL=4")

install(TARGETS riscv_toplevel DESTINATION bin)
