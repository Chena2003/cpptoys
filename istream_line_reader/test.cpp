#include <iostream>
#include <sstream>
#include "istream_line_reader.h"

using namespace Chena;

int main() {
     std::istringstream input("line 1\nline 2\nline 3");
     istream_line_reader reader(input);
     
     for (const auto& line : reader) {
          std::cout << line << std::endl;
     }
     
     // for (const auto& line : istream_line_reader(std::cin)) {
     //      std::cout << line << std::endl;
     // }

     return 0;
}