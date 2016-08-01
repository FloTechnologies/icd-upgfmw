cmake_minimum_required(VERSION 2.6)

PROJECT(upgfmw C)
ADD_DEFINITIONS(-Os -Wall -Werror --std=gnu99 -g3 -Wmissing-declarations)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

ADD_EXECUTABLE(upgfmw upgfmw.c libgw.c)

INSTALL(TARGETS upgfmw
	RUNTIME DESTINATION sbin
)
