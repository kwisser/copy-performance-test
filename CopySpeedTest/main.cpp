#include <iostream>
#include <chrono>

int main() {

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    int x = 56;
    for(int i=0; i<1000000000;i++){
        x = x*i;
    }
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration difference = end - start;
    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(difference).count();
    std::cout << "Test Took: " << ms << "ms." << std::endl;
    return 0;
}
