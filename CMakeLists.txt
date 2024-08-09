cmake_minimum_required(VERSION 3.25)
project(cpp-chess CXX)

set(CHESS_DEFAULT_BUILD_TYPE "Debug")

# Source: https://www.kitware.com/cmake-and-the-default-build-type/
# Set the build type if it was not specified (only in single-config generators).
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${CHESS_DEFAULT_BUILD_TYPE}' as none was specified.")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "${CHESS_DEFAULT_BUILD_TYPE}")
endif()

# By default, install cpp-chess inside the build directory.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_BINARY_DIR}/out")
endif()

option(CMAKE_COLOR_DIAGNOSTICS "Enable colored diagnostics" ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_MESSAGE LAZY)

add_executable(
	chess
	"src/Board.cpp"
	"src/chess960.cpp"
	"src/Game.cpp"
	"src/main.cpp"
	"src/Menu.cpp"
	"src/Piece.cpp"
	"src/safe_ctype.cpp"
	"src/TerminalUserInterface.cpp"
	"src/ui/AsciiUi.cpp"
	"src/ui/LetterUi.cpp"
	"src/ui/TwoLetterUi.cpp"
)

target_compile_features(chess PRIVATE cxx_std_17)
set_target_properties(
	chess PROPERTIES
	CXX_EXTENSIONS OFF
	CXX_STANDARD_REQUIRED ON
	DEBUG_POSTFIX -d
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_options(chess PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wno-sign-conversion)
	target_compile_options(chess PRIVATE $<$<CONFIG:Debug>:-g3 -fsanitize-trap=undefined>)
endif()

if(WIN32)
	target_sources(chess PRIVATE "src/ui/WindowsConsoleUi.cpp")
	target_compile_definitions(chess PRIVATE CHESS_ON_WINDOWS)
endif()

install(TARGETS chess)
