#include <iostream>
#include <string>

extern "C" 
void print_string2(const char* s) {
    
    auto s2 = std::string("huy");
    std::cout << s2 << std::endl;
    
}

// int main(int argc, char** argv) {
    // print_string(std::string("asd").c_str());
// }