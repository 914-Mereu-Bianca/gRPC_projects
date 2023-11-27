#include <iostream>
#include <thread>
#include <vector>

void my_fct(int i) {
    std::cout<<i<<" ";
    std::this_thread::sleep_for(std::chrono::seconds(4));
}

int main() {
    std::vector<std::thread> t;

    for(int i = 0; i < 4000; i++) {
        t.push_back(std::thread(&my_fct, i));
    }


    for (auto &t1: t) {
        t1.join();
    }


    return 0;
}