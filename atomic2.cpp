#include <iostream>
#include <thread>
#include <atomic>

int main() {
	std::thread t1, t2;
	std::atomic<int> a(0);
	auto f = [&a](int p){
		while(1) {
			int x = a;
			if(a.compare_exchange_strong(x, x + p)) {
				break;
			}
		}
	};
	t1 = std::thread(f, 1);
	t2 = std::thread(f, 2);
	t1.join(); t2.join();
	std::cout << a << std::endl;
}
