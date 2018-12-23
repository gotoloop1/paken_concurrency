#include <iostream>
#include <thread>
#include <future>

int fibonacci(int x) {
	if(x <= 2) return 1;
	else return fibonacci(x - 1) + fibonacci(x - 2);
}

int main() {
	std::thread t;
	std::promise<int> p;
	std::future<int> f = p.get_future();
	auto calc = [&p](int x){
		p.set_value(fibonacci(x));
	};
	t = std::thread(calc, 21);
	int a = fibonacci(20);
	std::cout << a + f.get() << std::endl;
	t.join();
}
