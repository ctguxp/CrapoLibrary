# Store *.cpp in src files

#Need Unicode definitions
add_definitions(-D_UNICODE -DUNICODE -D_LIB -Wall -WX -EHa -nologo -Yupch.h -I$(SolutionDir)../corlib)

# Add files
file(GLOB_RECURSE INC_FILES *.h)
file(GLOB_RECURSE SRC_FILES *.cpp)

set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "-Ycpch.h")

# Library project
add_library(System ${SRC_FILES} ${INC_FILES} ${INC_HPP_FILES} )

install(TARGETS System ARCHIVE DESTINATION static/lib)
install(FILES ${INC_FILES} DESTINATION include)