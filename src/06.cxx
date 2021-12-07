#include "lib.hxx"

int main(int, char **argv) {
	array<int64_t, 9> slot{};

	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	do_nums(loader, [&](int x) {
		slot[x]++; });

	auto Generate = [&slot](int N) {
		int64_t tmp{};
		for (int n=0; n<N; ++n) {
			tmp = slot[0];
			for (int i=0; i<8; ++i) {
				slot[i] = slot[i + 1]; }
			slot[6] += tmp;
			slot[8] = tmp; }};

	Generate(80);
	println(accumulate(begin(slot), end(slot), 0LL));
	Generate(256 - 80);
	println(accumulate(begin(slot), end(slot), 0LL));
	return 0; }
