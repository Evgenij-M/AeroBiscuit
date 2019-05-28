cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(AeroBiscuit)

set(CMAKE_CXX_STANDARD 17)

set(MAIN_SOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}/AeroBiscuit")
set(MAIN_AEROSPIKE_DIRECTORY "${MAIN_SOURCE_DIRECTORY}/Aerospike")

add_executable(AeroBiscuit
			   ${MAIN_SOURCE_DIRECTORY}/main.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeClient.hpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeClient.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeTransaction.hpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeTransaction.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeBin.hpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeBin.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeOperation.hpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeOperation.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeQueue.hpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeError.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeError.hpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeCallback.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeCallback.hpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeRecord.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeRecord.hpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeKey.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeKey.hpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeUtilities.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeUtilities.hpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeConfiguration.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeConfiguration.hpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeTransactionPolicy.cpp
			   ${MAIN_AEROSPIKE_DIRECTORY}/AerospikeTransactionPolicy.hpp
			  )

target_include_directories(${PROJECT_NAME} PRIVATE "${MAIN_SOURCE_DIRECTORY}")
target_include_directories(${PROJECT_NAME} PRIVATE "${MAIN_AEROSPIKE_DIRECTORY}")

if(APPLE)
	target_compile_options(${PROJECT_NAME} PRIVATE -mmacosx-version-min=10.9)

	target_include_directories(${PROJECT_NAME} PRIVATE "/usr/local/include")
	target_link_libraries(${PROJECT_NAME} "/usr/local/lib/libaerospike.a")

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_DARWIN_UNLIMITED_SELECT -DAS_USE_LIBUV")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/lib -L/usr/local/opt/openssl/lib -luv -lssl -lcrypto -lpthread -lm -lz")
elseif(UNIX)
	target_include_directories(${PROJECT_NAME} PRIVATE "/usr/include")
	target_link_libraries(${PROJECT_NAME} "/usr/lib/libaerospike.a")

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAS_USE_LIBUV")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/usr/lib64 -lrt -ldl -luv -lssl -lcrypto -lpthread -lm -lz")
endif()


find_package(Boost COMPONENTS system REQUIRED)
target_link_libraries(${PROJECT_NAME} ${Boost_SYSTEM_LIBRARY})


if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES Relwithdebinfo)
	target_compile_definitions(${PROJECT_NAME} PRIVATE "DEBUG=1")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -frtti")

	if(APPLE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti")
	elseif(UNIX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -frtti")
	endif()

else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -fvisibility=hidden -fvisibility-inlines-hidden -frtti")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -fvisibility=hidden -fvisibility-inlines-hidden -frtti")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND strip "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
endif()


set(CMAKE_VERBOSE_MAKEFILE on)