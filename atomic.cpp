#include <iostream>
#include <thread>
#include <atomic>

int main() {
	std::thread t1, t2;
	std::atomic<int> a(0);
	t1 = std::thread([&a](){a = 1;});
	t2 = std::thread([&a](){a = 2;});
	t1.join(); t2.join();
	std::cout << a << std::endl;
}
