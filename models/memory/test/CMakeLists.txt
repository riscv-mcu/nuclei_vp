#
# CMakeLists.txt
#
# Copyright (C) 2014, GreenSocs ltd.
#
# Developed by Frederic Konrad <fred.konrad@greensocs.com>
#              Guillaume Delbergue <guillaume.delbergue@greensocs.com>
#

add_executable(simplememory-test SimpleMemoryTest.cpp)
add_executable(dualportmemory-test DualPortMemoryTest.cpp)
add_executable(byteenable-test ByteEnableTest.cpp)

set(TEST_LINK_LIBRARIES ${SystemC_LIBRARIES})
if(MINGW)
    list(APPEND TEST_LINK_LIBRARIES mman)
endif(MINGW)

target_link_libraries(simplememory-test ${TEST_LINK_LIBRARIES})
target_link_libraries(dualportmemory-test ${TEST_LINK_LIBRARIES})
target_link_libraries(byteenable-test ${TEST_LINK_LIBRARIES})

add_test(NAME simplememory COMMAND ./simplememory-test)
add_test(NAME dualportmemory COMMAND ./dualportmemory-test)
add_test(NAME byteenable COMMAND ./byteenable-test)
