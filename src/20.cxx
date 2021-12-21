#include "lib.hxx"

int W, H, STRIDE;

auto K(int x, int y) -> int64_t { return (int64_t(x)<<32)|(y&0xffffffff); }

int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view map{ loader.data(), loader.data() + loader.size() };

	auto table = loader.data();
	ConsumeLine(map);  // skip table
	for (int i=0; i<512; ++i) {
		loader.data()[i] = loader.data()[i] == '#'; }
	ConsumeLine(map); // emptyline

	W = map.find("\n");
	STRIDE = W + 1;
	H = map.size() / STRIDE;

	auto Map = [&](int x, int y) -> int {
		if (   -50 <= x && x < 50
			&& -50 <= y && y < 50) {
			int ix = x + 50;
			int iy = 99-(y + 50);
			return map[iy*STRIDE + ix] == '#'; }
		return 0; };

	uset<int64_t> pxa{}, pxb{};
	for (int y=-60; y<60; ++y) {
		for (int x=-60; x<60; ++x) {
			if (Map(x, y)) pxa.insert(K(x,y)); }}

	auto Map2 = [&](int x, int y) -> int {
		return pxa.find(K(x,y)) != end(pxa); };

	auto f = [&](int x, int y) -> int {
		int ax{};
		for (int oy=1; oy>=-1; --oy) {
			for (int ox=-1; ox<=1; ++ox) {
				ax <<= 1;
				ax |= Map2(x+ox, y+oy); }}
		return table[ax]; };

	auto g = [&](int x, int y) -> int {
		int ax{};
		for (int oy=1; oy>=-1; --oy) {
			for (int ox=-1; ox<=1; ++ox) {
				ax <<= 1;
				ax |= f(x+ox, y+oy); }}
		return table[ax]; };

	int p1;
	for (int n=0; n<50; n+=2) {
		if (n==2) {
			p1 = pxa.size(); }
		// print("."); flush(fast_io::c_stdout());
		int sz = 60 + (n+1)*2;
		for (int y=-sz; y<sz; ++y) {
			for (int x=-sz; x<sz; ++x) {
				if (g(x,y)) pxb.insert(K(x,y)); }}
		pxa.swap(pxb);
		pxb.clear(); }

	println(p1);
	println(pxa.size());
	return 0; }
