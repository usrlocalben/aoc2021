#include "lib.hxx"

int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view map{ loader.data(), loader.data() + loader.size() };
	fast_io::ibuffer_view view{ map };

	int l, r, b, t;
	// "target area: x=195..238, y=-93..-67"
	scan(view, "target area: x=", l, "..", r, ", y=", b, "..", t);
	if (r < l) swap(r, l);
	if (t < b) swap(t, b);

	const IVec2 TL{ l, t };
	const IVec2 BR{ r, b };

	// assert(TL.y < 0);

	auto Play = [&](IVec2 v) -> bool {
		IVec2 p{0,0};

		/*if (v.y > 0) {
			// skip +Y time
			int t = v.y * 2 + 1;
			v.y = -v.y - 1;
			int tx = min(t, v.x);
			p.x = TS(tx);
			v.x -= tx;
			assert(v.x >= 0); }*/

		while (p.y >= BR.y) {
			if (Hit(TL, BR, p)) return true;
			p += v;
			if (v.x > 0) --v.x;
			v.y--; }
		return false; };

	auto PlayX = [&](int v) -> bool {
		int p{};
		while (v && p <= BR.x) {
			if (TL.x <= p && p <= BR.x) return true;
			p += v;
			if (v > 0) --v; }
		return false; };

	auto PlayY = [&](int v) -> bool {
		int p{};
		if (v > 0) v = -v - 1; // skip +Y time
		while (p >= BR.y) {
			if (BR.y <= p && p <= TL.y) return true;
			p += v;
			--v; }
		return false; };

	const int velXLimit{ r };  // any faster and overshoot X on step1
	const int velYLimit{ -b - 1 };  // any faster and overshoot Y coming down

	vi goody{}, goodx{};
	for (int ivy=velYLimit; ivy>=b; --ivy) {
		if (PlayY(ivy)) {
			goody.push_back(ivy); }}
	for (int ivx=velXLimit; ivx>=sqrt(l); --ivx) {
		if (PlayX(ivx)) {
			goodx.push_back(ivx); }}

	optional<int> p1{};
	uset<int> combos{};
	for (auto ivy : goody) {
		for (auto ivx : goodx) {
			if (Play(IVec2{ ivx, ivy })) {
				if (!p1) {
					p1 = ivy; }
				int key = (ivy<<16)|(ivx&0xffff);
				combos.insert(key); }}}

	println(TS(*p1));
	println(combos.size());
	return 0; }
