#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

int main() {
	std::mutex x, y;
	std::thread a([&x, &y](){
		x.lock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		y.lock();
		y.unlock();
		x.unlock();
	});
	std::thread b([&x, &y](){
		y.lock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		x.lock();
		x.unlock();
		y.unlock();
	});
	a.join(); b.join();
}
