#include "lib.hxx"

auto K(int p1, int p2, int s1, int s2) -> int {
	int ax{p1};
	ax <<= 4; ax |= p2;
	ax <<= 5; ax |= s1;
	ax <<= 5; ax |= s2;
	return ax; }

auto U(int a) -> tuple<int, int, int, int> {
	int s2 = a&0x1f; a>>=5;
	int s1 = a&0x1f; a>>=5;
	int p2 = a&0xf; a>>=4;
	int p1 = a;
	return { p1, p2, s1, s2 }; }

constexpr int SIDES{100};
constexpr int SPACES{10};

int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view text{ loader.data(), loader.data() + loader.size() };
	fast_io::ibuffer_view view{ text };

	int64_t d{};

	array<int64_t, 2> init{};
	for (int n=0; n<2; ++n) {
		int pn;
		scan(view, "Player ", pn, " starting position: ", init[n], "\n");
		init[n] -= 1;
		assert(pn == n+1); }

	{ // p1
		auto pos = init;

		int rc{};
		auto Roll = [&]() -> int {
			rc++;
			return ((d++) % SIDES) + 1; };

		array<int, 2> sc{};

		int p=0;
		while (1) {
			int many = Roll()+Roll()+Roll();
			pos[p] += many;
			sc[p] += pos[p] % SPACES + 1;
			if (sc[p] >= 1000) {
				break; }
			p = p==0; }

		// p is winner
		int l = p==0;
		println(sc[l] * rc); }


	// p2....
	umap<int, int64_t> ga{}, gb{};
	ga[K(init[0],init[1],0,0)] = 1;
	int64_t p2{};
	int n=0;
	while (!ga.empty()) {
		// println("states: ", ga.size()); flush(fast_io::c_stdout());

		bool hero = n%2==0;

		for (auto [state, many] : ga) {
			auto [p0, p1, s0, s1] = U(state);

			p0 += 2;
			for (auto prob : array<int,7>{{ 1, 3, 6, 7, 6, 3, 1 }}) {
				p0 = (p0+1)%SPACES;
				int ns = s0 + p0 + 1;
				if (ns >= 21) {
					p2 += many * prob * hero; }
				else {
					// swap order
					gb[K(p1,p0,s1,ns)] += many * prob; }}}

		++n;
		ga.swap(gb); gb.clear(); }

	println(p2);
	return 0; }
