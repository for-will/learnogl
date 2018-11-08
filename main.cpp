#include <iostream>
#include "utils/utils.h"
int main() {
//    std::cout << "Hello, World!" << std::endl;
//    FILE* fp = fopen("src/shader/vertex.glsl", "rb");
//    if(fp != nullptr){
//        char buffer[512];
//        fread(buffer, sizeof(buffer), 1, fp);
//        fclose(fp);
//        printf("%s\n", buffer);
//    }

    std::cout << read_file("src/shader/vertex.glsl") << std::endl;

    return 0;
}