#include "lib.hxx"

int main(int, char **argv) {
	using fast_io::mnp::chvw;
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view text{ loader.data(), loader.data() + loader.size() };

	int px[1000], py[1000], N{};
	while (1) {
		auto line = ConsumeLine(text);
		if (line.empty()) break;
		// "x,y"
		scan(fast_io::ibuffer_view{line}, px[N], ",", py[N]);
		++N; }

	int p1{};
	while (!text.empty()) {
		auto line = ConsumeLine(text);
		auto fa = ConsumeToken(line, '=');
		char axis = fa[fa.size() - 1];
		int k = atoi(line.data());

		int* const coord = axis == 'x' ? px : py;
		for (int i=0; i<N; ++i) {
			if (coord[i] > k) {
				coord[i] = -coord[i] + 2*k; }}
		if (!p1) {
			uset<int> uniq{};
			for (int i=0; i<N; ++i) {
				int key = py[i]<<16 | px[i];
				uniq.insert(key); }
			p1 = uniq.size(); }}

	println(p1);

	int sx{}, sy{};
	for (int i=0; i<N; ++i) {
		sx = max(sx, px[i]);
		sy = max(sy, py[i]); }
	++sx, ++sy;
	// assert(sx==39 && sy==6);

	vector<char> map(sx*sy, ' ');
	for (int i=0; i<N; ++i) {
		map[py[i]*sx + px[i]] = '#'; }
	for (int y=0; y<sy; ++y) {
		println(string_view{ &map[y*sx], size_t(sx) }); }

	return 0; }
