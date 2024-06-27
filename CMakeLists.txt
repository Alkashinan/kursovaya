# cmake_minimum_required(VERSION 3.24)
# project(cmake_sfml_1)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(SFML_STATIC_LIBRARIES TRUE) #только если статическая сборка библиотеки
# find_package(SFML 2.6.1 COMPONENTS graphics window system)

# add_executable(cmake_sfml rightnow.cpp)
# target_link_libraries(cmake_sfml sfml-graphics sfml-window sfml-system)

cmake_minimum_required( VERSION 3.10 )

project( Draw VERSION 1.0 LANGUAGES CXX )
set( CMAKE_CXX_STANDARD 17 )

# choose version of SFML depending on compiler it was built
#set( SFML_COMPILER_TYPE vs15_x64 )
set( SFML_COMPILER_TYPE gcc_x64 )
#----------------------------------------------------------

# set INC and SRC files
set( SRC 
    main.cpp 
)
set( INC 
    inc/
    SFML_${SFML_COMPILER_TYPE}/include/
)

set( CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/SFML_${SFML_COMPILER_TYPE}" )
find_package( SFML 2.6.1 REQUIRED network audio graphics window system )
set( SFML_LIBS sfml-network sfml-audio sfml-graphics sfml-window sfml-system )

find_package( OpenGL REQUIRED )
set( OPEN_GL_LIBS opengl32 )

add_executable( ${PROJECT_NAME} ${SRC} )
target_include_directories( ${PROJECT_NAME} PUBLIC ${INC} )
target_link_libraries( ${PROJECT_NAME} PUBLIC ${SFML_LIBS} ${OPEN_GL_LIBS} )
