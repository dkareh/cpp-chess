cmake_minimum_required(VERSION 3.25)
project(cpp-chess LANGUAGES CXX)

option(CMAKE_COLOR_DIAGNOSTICS "Enable colored diagnostics" ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

add_executable(
	chess
	"src/displays/AsciiDisplay.cpp"
	"src/displays/LetterDisplay.cpp"
	"src/displays/TwoLetterDisplay.cpp"
	"src/Board.cpp"
	"src/chess960.cpp"
	"src/Display.cpp"
	"src/Game.cpp"
	"src/main.cpp"
	"src/Menu.cpp"
	"src/Piece.cpp"
	"src/safe_ctype.cpp"
)

target_include_directories(chess PRIVATE include)
target_compile_features(chess PRIVATE cxx_std_17)
set_target_properties(
	chess PROPERTIES
	CXX_EXTENSIONS OFF
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	DEBUG_POSTFIX -d
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_options(chess PRIVATE -Wall -Wextra -pedantic)
endif()

if(WIN32)
	target_sources(chess PRIVATE "src/displays/WindowsConsoleDisplay.cpp")
	target_compile_definitions(chess PRIVATE CHESS_ON_WINDOWS)
endif()

install(TARGETS chess)
