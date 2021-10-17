# CMakeList.txt : CMake project for ZOLparser, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (ZOLparser "main.cpp"       "Token.hpp" "Parser.hpp" "Parser.cpp" "Error.hpp" "Expression.hpp" "Expression.cpp")

# TODO: Add tests and install targets if needed.
