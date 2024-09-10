cmake_minimum_required(VERSION 3.5)
set(projectName "cache")
project(${projectName} CXX ASM_NASM)

if(LINUX)

set(CMAKE_NASM_LINK_EXECUTABLE "ld <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CAN_USE_ASSEMBLER TRUE)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

enable_language(ASM_NASM)

endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_CXX_FLAGS_DEBUG
"-no-pie -Wno-maybe-uninitialized -Wno-switch -Wno-conversion                       \
-Wno-unused-variable -Wno-pointer-arith -g -D _DEBUG -ggdb3 -O0 -Wall -Wextra       \
-Weffc++ -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align        \
-Wchar-subscripts -Wconditionally-supported -Wctor-dtor-privacy -Wempty-body        \
-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2                \
-Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked         \
-Winit-self -Wredundant-decls -Wsign-promo -Wstrict-null-sentinel                   \
-Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods            \
-Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wsync-nand -Wundef       \
-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix    \
-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs     \
-Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector                \
-fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Werror=vla        \
-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,\
float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,\
object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,\
undefined,unreachable,vla-bound,vptr")

set(CMAKE_CXX_FLAGS_RELEASE
"-no-pie -Wall -Wno-maybe-uninitialized -Wno-switch -Wno-unused                     \
-Wno-conversion -Wno-unused-variable -Wno-pointer-arith -Wextra -march=native       \
-mavx512f -O3 -DNDEBUG")

if(LINUX)
set(MLIB_SOURCES "libs/mlib/src/CRC32 with loop unrolling.asm")
else()
set(MLIB_SOURCES "")
endif()

FILE(GLOB MLIB_SOURCES_CPP libs/mlib/src/*.cpp)

add_library(mlib STATIC ${MLIB_SOURCES_CPP})
target_include_directories(mlib PUBLIC headers tests libs/mlib/headers)

set(SOURCES
    src/main.cpp
)

add_executable(${projectName} ${SOURCES})
target_link_libraries(${projectName} mlib)
