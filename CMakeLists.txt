# CMakeList.txt: проект CMake для chip8emu; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)
project(chip8emu)
set(SRC_LIST src/chip8.cpp src/chip8opcodes.cpp src/main.cpp)
set(HEADER_LIST src/chip8.h)



# Добавьте источник в исполняемый файл этого проекта.
add_executable (${PROJECT_NAME} ${SRC_LIST} ${HEADER_LIST})

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
