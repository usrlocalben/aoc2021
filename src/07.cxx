#include "lib.hxx"

int main(int, char **argv) {
	vi nums{}; nums.reserve(1000);

	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	do_nums(loader, [&](int x) {
		nums.push_back(x); });

	sort(begin(nums), end(nums));

	int p1best{oo};
	int p2best{oo};
	for (int i=nums.front(); i<=nums.back(); ++i) {
		int ax1{}, ax2{};
		for (auto p : nums) {
			int dist = abs(p - i);
			ax1 += dist;
			int cost = dist * (dist+1) / 2;
			ax2 += cost; }
		p1best = min(p1best, ax1);
		p2best = min(p2best, ax2);}

	println(p1best);
	println(p2best);
	return 0; }
