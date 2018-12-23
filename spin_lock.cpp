#include <iostream>
#include <thread>
#include <atomic>

class SpinLock {
private:
	std::atomic<int> state;
public:
	SpinLock(): state(0) {}
	void lock() {
		while(1) {
			int x = 0;
			if(state.compare_exchange_strong(x, 1)) break;
		}
	}
	void unlock() {
		state.store(0);
	}
};

int main() {
	std::thread t1, t2;
	int a = 0;
	SpinLock sl;
	auto f = [&a, &sl](int p){
		sl.lock();
		a += p;
		sl.unlock();
	};
	t1 = std::thread(f, 1);
	t2 = std::thread(f, 2);
	t1.join(); t2.join();
	std::cout << a << std::endl;
}
