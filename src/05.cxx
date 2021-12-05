#include "lib.hxx"

constexpr int DIM{ 1000 };

int main(int, char **argv) {
#if 0
	fast_io::native_file_loader loader{ string_view(argv[1]) };
	string_view data{ loader.data(), loader.data() + loader.size() };
	fast_io::ibuffer_view input{ data };
#else
	fast_io::ibuf_file input{ string_view(argv[1]) };
#endif

	// "491,392 -> 34,392"
	int aa,bb,cc,dd;
	vector<pair<IVec2, IVec2>> segs{};  segs.reserve(500);
	while (scan<true>(input, aa, ",", bb, " -> ", cc, ",", dd, "\n")) {
		segs.push_back({ IVec2{ aa, bb }, IVec2{ cc, dd } }); }

	vi map(DIM*DIM, 0);

	for (auto [a, b] : segs) {
		if (a.y == b.y) {
			int x1 = a.x;
			int x2 = b.x;
			if (x1 > x2) swap(x1, x2);
			for (int x=x1; x<=x2; ++x) {
				map[a.y*DIM + x]++; }}
		else if (a.x == b.x) {
			int y1 = a.y;
			int y2 = b.y;
			if (y1 > y2) swap(y1, y2);
			for (int y=y1; y<=y2; ++y) {
				map[y*DIM + a.x]++; }}
		else {
			/* skip */}}

	int64_t part1{};
	for (auto cell : map) {
		part1 += cell > 1; }
	println(part1);

	for (auto [a, b] : segs) {
		if (a.y!=b.y && a.x!=b.x) {
			IVec2 dir{ a.x < b.x ? 1 : -1, a.y < b.y ? 1 : -1 };
			for (IVec2 pos=a; pos!=b; pos+=dir) {
				map[pos.y*DIM + pos.x]++; }
			map[b.y*DIM + b.x]++; }}

	int64_t part2{};
	for (auto cell : map) {
		part2 += cell > 1; }
	println(part2);

	return 0; }

