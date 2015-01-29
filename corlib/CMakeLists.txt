# Store *.cpp in src files

#Need Unicode definitions
add_definitions(-D_UNICODE -DUNICODE)

file(GLOB_RECURSE INC_FILES *.h)
file(GLOB_RECURSE INC_HPP_FILES *.hpp)
file(GLOB_RECURSE SRC_FILES *.cpp)

add_library(corlib ${SRC_FILES} ${INC_FILES} ${INC_HPP_FILES} )