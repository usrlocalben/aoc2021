#include "lib.hxx"

constexpr int DIM{ 10 };
constexpr int STRIDE{ 11 };

int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	char *map = loader.data();

	int p1{}, p2{};
	vector<int> queue{};  queue.reserve(DIM*DIM);
	for (int n{0}; n<100||!p2; ++n) {

		unordered_set<int> flashed{};
		for (int y=0; y<DIM; ++y) {
			for (int x=0; x<DIM; ++x) {
				const int idx = y*STRIDE + x;
				if (++map[idx] > '9') {
					queue.push_back(idx); }}}

		while (!queue.empty()) {
			int idx = queue.back(); queue.pop_back();
			if (flashed.find(idx) != end(flashed)) continue;
			flashed.insert(idx);

			const int y = idx / STRIDE;
			const int x = idx % STRIDE;

			for (int dx=-1; dx<=1; ++dx) {
				for (int dy=-1; dy<=1; ++dy) {
					if (dx==0 && dy==0) continue;
					const int cx{x+dx}, cy{y+dy};
					if (   0<=cx && cx <DIM
						&& 0<=cy && cy <DIM) {
						const int cidx = cy*STRIDE + cx;
						if (++map[cidx] > '9') {
							queue.push_back(cidx); }}}}}

		if (flashed.size() == DIM*DIM) {
			p2 = n + 1; }
		if (n < 100) {
			p1 += flashed.size(); }
		for (auto idx : flashed) {
			map[idx] = '0'; }}

	println(p1);
	println(p2);
	return 0; }
