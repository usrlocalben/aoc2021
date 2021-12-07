#include "lib.hxx"

int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view data{ loader.data(), loader.data() + loader.size() };

	const int w = data.find('\n');
	const int stride = w + 1;
	const int h = data.size() / stride;
	const int all1s = (1 << w) - 1;

	// assert(w == 12);
	// assert(h == 1000);

	vi nums(h);
	vi popcnt(w);
	for (int y=0; y<h; ++y) {

		int num{};
		for (int x=0; x<w; ++x) {
			bool bit{ data[y*stride + x] == '1' };
			num = (num << 1) | int(bit);
			popcnt[x] += bit; }

		nums[y] = num; }

	int g{};
	for (int x=0; x<w; ++x) {
		g = (g<<1) | (popcnt[x] >= h/2); }
	int ep = g ^ all1s;
	int p1{ g * ep };
	println(p1);


	// part2
	sort(begin(nums), end(nums));
	int majorHit, minorHit;

	{int path{};
	int bit = 1 << (w-1);
	auto rb{ begin(nums) };
	auto re{ end(nums) };
	while (distance(rb, re) > 1) {
		auto split = lower_bound(rb, re, path|bit);
		auto n0 = distance(rb, split);
		auto n1 = distance(split, re);
		if (n1 >= n0) {
			// common = 1
			path |= bit;
			rb = split; }
		else {
			// common = 0
			re = split; }

		bit >>= 1; }
	majorHit = *rb;}

	{int path{};
	int bit = 1 << 11;
	auto rb{ begin(nums) };
	auto re{ end(nums) };
	while (distance(rb, re) > 1) {
		auto split = lower_bound(rb, re, path|bit);
		auto n0 = distance(rb, split);
		auto n1 = distance(split, re);
		if (n0 <= n1) {
			// uncommon = 0
			re = split; }
		else {
			// uncommon = 1
			path |= bit;
			rb = split; }

		bit >>= 1; }
	minorHit = *rb;}

	auto p2 = majorHit * minorHit;
	println(p2);

	return 0; }
