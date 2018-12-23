#include <iostream>
#include <random>
#include <thread>
#include <mutex>
#include <condition_variable>

const int th_num = 32;
const int size = 1000000;
const int turn = 1000;

int main() {
	bool d[size];
	std::mutex m1, m2;
	std::condition_variable cv1, cv2;
	int r1 = 0, r2 = 0;
	auto f = [&d, &m1, &m2, &cv1, &cv2, &r1, &r2](int k) {
		int b = size * k / th_num;
		int e = size * (k + 1) / th_num;
		for(int _ = 0; _ < turn; _++) {
			std::vector<bool> now(e - b + 2);
			now[0] = d[(b - 1 + size) % size];
			now[e - b + 1] = d[e % size];
			{
				std::lock_guard<std::mutex> lg(m1);
				if(r1 == 0) r1 = th_num;
				r1--;
				if(r1 == 0) cv1.notify_all();
					//他のタスクにそれぞれが管理する領域の変更を許可する
			}
			for(int i = 0; i < e - b; i++)
				now[i + 1] = d[b + i];
			std::vector<bool> nex(e - b);
			for(int i = 0; i < e - b; i++) {
				if(!now[i] && !now[i + 1] && !now[i + 2]) nex[i] = false;
				else if(now[i] && !now[i + 1] && !now[i + 2]) nex[i] = false;
				else if(now[i] && now[i + 1] && now[i + 2]) nex[i] = false;
				else nex[i] = true;
			}
			{
				std::unique_lock<std::mutex> ul(m1);
				cv1.wait(ul, [&r1](){return r1 == 0;});
			}
			for(int i = 0; i < e - b; i++) d[b + i] = nex[i];
			{
				std::unique_lock<std::mutex> ul(m2);
				if(r2 == 0) r2 = th_num;
				r2--;
				if(r2 == 0) cv2.notify_all();
					//他のタスクに自分が管理する領域の読み取りを許可する
				cv2.wait(ul, [&r2](){return r2 == 0;});
			}
		}
	};
	std::mt19937 mt(0);
	for(int i = 0; i < size; i++) d[i] = mt() % 2;
	std::thread th[th_num];
	for(int i = 0; i < th_num; i++) th[i] = std::thread(f, i);
	for(int i = 0; i < th_num; i++) th[i].join();
	int count = 0;
	for(int i = 0; i < size; i++) if(d[i]) count++;
	std::cout << count << std::endl;
}
