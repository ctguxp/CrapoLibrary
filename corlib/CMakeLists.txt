#Need Unicode definitions
add_definitions(-D_UNICODE -DUNICODE -D_LIB -Wall -WX -EHa -nologo -Yupch.h)

# Add files
file(GLOB_RECURSE INC_FILES *.h)
file(GLOB_RECURSE INC_HPP_FILES *.hpp)
file(GLOB_RECURSE SRC_FILES *.cpp)

set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "-Ycpch.h")

# Library project
add_library(corlib ${SRC_FILES} ${INC_FILES} ${INC_HPP_FILES} )

install(TARGETS corlib ARCHIVE DESTINATION static/lib)
install(FILES ${INC_FILES} DESTINATION include)
install(FILES ${INC_HPP_FILES} DESTINATION include)