#include "lib.hxx"

int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view map{ loader.data(), loader.size() };

	const int DIM = map.find('\n');
	const int STRIDE = DIM + 1;
	assert(map.size() == DIM * STRIDE);

	auto Height = [&](int x, int y) -> int {
		if (0<=x && x<DIM
			&& 0<=y && y<DIM) {
			return map[y*STRIDE + x] - '0'; }
		return 9; };

	int p1{};
	vi bs{};
	vi visited( map.size(), 0 );

	for (int y=0; y<DIM; ++y) {
		for (int x=0; x<DIM; ++x) {
			auto h = Height(x, y);
			if (   h < Height(x-1, y)
				&& h < Height(x+1, y)
				&& h < Height(x, y-1)
				&& h < Height(x, y+1)) {

				p1 += 1 + h;

				int cnt{};
				std::deque<IVec2> queue{ { IVec2{ x, y } } };
				while (!queue.empty()) {
					auto p = queue.front();  queue.pop_front();
					if (visited[p.y*STRIDE + p.x]) continue;
					visited[p.y*STRIDE + p.x] = 1;
					++cnt;
					
					auto n = IVec2{ p.x, p.y - 1 };
					auto e = IVec2{ p.x + 1, p.y };
					auto s = IVec2{ p.x, p.y + 1 };
					auto w = IVec2{ p.x - 1, p.y };
					if (Height(n.x, n.y) != 9) { queue.push_back(n); }
					if (Height(e.x, e.y) != 9) { queue.push_back(e); }
					if (Height(s.x, s.y) != 9) { queue.push_back(s); }
					if (Height(w.x, w.y) != 9) { queue.push_back(w); }}
				bs.push_back(cnt); }}}

	nth_element(begin(bs), begin(bs)+2, end(bs), std::greater<>());
	auto p2 = bs[0] * bs[1] * bs[2];

	println(p1);
	println(p2);
	return 0; }
