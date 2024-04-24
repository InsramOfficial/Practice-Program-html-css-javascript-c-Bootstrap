#include <iostream>
#include <thread>

int sharedValue = 0;

void incrementSharedValue(int numIterations) {
    for (int i = 0; i < numIterations; ++i) {
        int temp = sharedValue;
        // Simulate some computation
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        sharedValue = temp + 1;
    }
}

int main() {
    const int numThreads = 2;
    const int numIterations = 1000;
    std::thread threads[numThreads];

    for (int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread(incrementSharedValue, numIterations);
    }

    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    std::cout << "SharedValue: " << sharedValue << std::endl;

    return 0;
}

