#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;
#define int long long
#define rep(i, n) for(int i = 0; i < n; i++)

#define T 8
#define D 7
#define S 7LL

int dfs(vector<bool>& v, pair<int, int> now) {
	if(now.first < 0 || now.first >= S || now.second < 0 || now.second >= S) {
		return 0;
	}
	if(now == make_pair(S - 1, S - 1)) {
		return 1;
	}
	if(v[now.first * S + now.second]) {
		return 0;
	}
	v[now.first * S + now.second] = true;
	int all = 0;
	all += dfs(v, make_pair(now.first - 1, now.second));
	all += dfs(v, make_pair(now.first + 1, now.second));
	all += dfs(v, make_pair(now.first, now.second - 1));
	all += dfs(v, make_pair(now.first, now.second + 1));
	v[now.first * S + now.second] = false;
	return all;
}
int calc(vector<pair<int, int>> p) {
	vector<bool> v(S * S, false);
	for(auto i: p) {
		if(i != p.back()) {
			v[i.first * S + i.second] = true;
		}
	}
	return dfs(v, p.back());
}

mutex mu;
vector<vector<pair<int, int>>> q;
int ans = 0;

void multi() {
	while(1) {
		vector<pair<int, int>> n;
		{
			lock_guard<mutex> lg(mu);
			if(q.empty()) {
				break;
			}
			n = q.back();
			q.pop_back();
		}
		int a = calc(n);
		{
			lock_guard<mutex> lg(mu);
			ans += a;
		}
	}
}

void prepare(vector<bool>& v, vector<pair<int, int>>& p) {
	if(p.back().first < 0 || p.back().first >= S || p.back().second < 0 || p.back().second >= S) {
		return;
	}
	if(v[p.back().first * S + p.back().second]) {
		return;
	}
	if(p.size() == D) {
		q.push_back(p);
	}
	else {
		v[p.back().first * S + p.back().second] = true;
		auto l = p.back();
		p.push_back({l.first - 1, l.second});
		prepare(v, p);
		p.pop_back();
		p.push_back({l.first + 1, l.second});
		prepare(v, p);
		p.pop_back();
		p.push_back({l.first, l.second - 1});
		prepare(v, p);
		p.pop_back();
		p.push_back({l.first, l.second + 1});
		prepare(v, p);
		p.pop_back();
		v[p.back().first * S + p.back().second] = false;
	}
}

signed main() {
	vector<bool> v(S * S, false);
	vector<pair<int, int>> p{{0, 0}};
	prepare(v, p);
	thread th[T];
	rep(i, T) {
		th[i] = thread(multi);
	}
	rep(i, T) {
		th[i].join();
	}
	cout << ans << endl;
}
