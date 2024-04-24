#include <iostream>
// thread
#include <thread>
// mutex
#include <mutex>
using namespace std;
mutex mtx;
void myMethod () {
mtx.lock();
cout<< "values are ::";
for (int z=0; z< 10; ++z) {
cout<< z;
cout<< '\n';
}
cout<< "End of output !!";
mtx.unlock();
}
int main ()
{
cout<< "Demo for Mutex in C++";
// cretaingtherad here
thread thread1 (myMethod);
thread thread2 (myMethod);
thread thread3 (myMethod);
// therad
thread1.join();
thread2.join();
thread3.join();
return 0;
}
