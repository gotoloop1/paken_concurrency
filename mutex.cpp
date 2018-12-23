#include <iostream>
#include <thread>
#include <mutex>

int main() {
	std::thread t1, t2;
	int a = 0;
	std::mutex m;
	auto f = [&a, &m](int p){
		m.lock();
		a += p;
		m.unlock();
	};
	t1 = std::thread(f, 1);
	t2 = std::thread(f, 2);
	t1.join(); t2.join();
	std::cout << a << std::endl;
}
