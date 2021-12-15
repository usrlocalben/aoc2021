#include "lib.hxx"

constexpr int DIM{ 100 };
constexpr int STRIDE{ DIM + 1 };

auto K(IVec2 a) -> int { return (a.x<<16) | a.y; }
auto U(int a) -> IVec2 { return IVec2{ a>>16, a&0xffff }; }

constexpr array<IVec2, 4> dirs{ { {-1,0}, {1,0}, {0,-1}, {0,1} } };

int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view map{ loader.data(), loader.data() + loader.size() };

	int p1{-1}, p2{-1};
	const IVec2 init{ 0, 0 };

	{
		//p1
		constexpr IVec2 target{ DIM - 1, DIM - 1 };

		priority_queue<pii, vii, greater<pii>> queue;
		uset<int> visited;
		umap<int, int> dist;
		auto Dist = [&](int key) {
			if (auto search = dist.find(key); search != dist.end()) {
				return search->second; }
			return oo; };

		queue.push({ 0, K(init) });
		dist[K(init)] = 0;
		while (loaded(queue)) {
			auto [hdist, hposk] = queue.top(); queue.pop();
			if (visited.find(hposk) != visited.end()) continue;
			visited.insert(hposk);

			const auto hpos = U(hposk);
			if (hpos == target) {
				p1 = hdist;
				break;}

			for (auto ofs : dirs) {
				auto np = hpos + ofs;
				if (   0 <= np.x && np.x < DIM
					&& 0 <= np.y && np.y < DIM) {
					int c = map[np.y * STRIDE + np.x] - '0';
					int thisCost = Dist(hposk) + c;
					if (thisCost < Dist(K(np))) {
						dist[K(np)] = thisCost;
						queue.push({ thisCost, K(np) }); }}}}}

	{
		//p2
		constexpr IVec2 target{ DIM * 5 - 1, DIM * 5 - 1 };

		priority_queue<pii, vii, greater<pii>> queue;
		uset<int> visited;
		umap<int, int> dist;
		auto Dist = [&](int key) {
			if (auto search = dist.find(key); search != dist.end()) {
				return search->second; }
			return oo; };

		queue.push({ 0, K(init) });
		dist[K(init)] = 0;
		while (loaded(queue)) {
			auto [hdist, hposk] = queue.top(); queue.pop();
			if (visited.find(hposk) != visited.end()) continue;
			visited.insert(hposk);

			const auto hpos = U(hposk);
			if (hpos == target) {
				p2 = hdist;
				break;}

			for (auto ofs : dirs) {
				auto np = hpos + ofs;
				if (   0 <= np.x && np.x < DIM*5
					&& 0 <= np.y && np.y < DIM*5) {
					auto ns = np / DIM;
					auto nm = np % DIM;
					int c = map[nm.y * STRIDE + nm.x] - '0';
					c = (c - 1 + ns.x + ns.y) % 9 + 1;
					int thisCost = Dist(hposk) + c;
					if (thisCost < Dist(K(np))) {
						dist[K(np)] = thisCost;
						queue.push({ thisCost, K(np) }); }}}}}

	println(p1);
	println(p2);
	return 0; }
