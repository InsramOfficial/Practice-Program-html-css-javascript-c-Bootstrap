#include <iostream>
#include <thread>

int shared_variable = 0;

void increment_shared_variable() {
    for (int i = 0; i < 1000000; ++i) {
        shared_variable++;
    }
}

int main() {
    std::thread thread1(increment_shared_variable);
    std::thread thread2(increment_shared_variable);

    thread1.join();
    thread2.join();

    std::cout << "Shared variable value: " << shared_variable << std::endl;

    return 0;
}
