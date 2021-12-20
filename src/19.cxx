#include "lib.hxx"

auto K(IVec3 a) -> int64_t {
	int64_t x(a.x & 0xffff);
	int64_t y(a.y & 0xffff);
	int64_t z(a.z & 0xffff);
	return (x<<32)|(y<<16)|z; }

struct Affine {
	int tx{}, ty{}, tz{};
	IVec3 t{}; };

auto Transform(IVec3 p, Affine a, bool inverse=false) {
	if (!inverse) {
		p = RotateX(p, a.tx);
		p = RotateY(p, a.ty);
		p = RotateZ(p, a.tz);
		p = p + a.t; }
	else {
		p = p - a.t;
		p = RotateZ(p, -a.tz);
		p = RotateY(p, -a.ty);
		p = RotateX(p, -a.tx); }
	return p; }

struct Link {
	int from, to;
	Affine affine; };

using Points = vector<IVec3>;
vector<Points> S{};
umap<int, vector<Link>> links{};
array<char, 100> visited{};
auto DFS(int s) -> pair<Points, Points> {
	visited[s] = 1;

	Points out1{}; out1.reserve(1000);
	Points out2{}; out2.reserve(100);
	for (auto& link : links[s]) {
		bool forward = link.from == s;
		int other = forward ? link.to : link.from;

		if (visited[other]) continue;
		auto [otherPoints, otherCenters] = DFS(other);
		for (auto p : otherPoints) {
			out1.push_back(Transform(p, link.affine, forward)); }
		for (auto p : otherCenters) {
			out2.push_back(Transform(p, link.affine, forward)); }}

	copy(begin(S[s]), end(S[s]), back_inserter(out1));
	out2.push_back(IVec3{ 0, 0, 0 });  // p2
	return { out1, out2 }; }


int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view text{ loader.data(), loader.data() + loader.size() };

	while (!text.empty()) {
		auto block = ConsumeBlock(text);

		fast_io::ibuffer_view bv{ block };
		int sid;
		scan(bv, "--- scanner ", sid, " ---\n");
		assert(S.size() == sid);
		S.emplace_back();
		S[sid].reserve(50);
		int xx, yy, zz;
		while (scan<true>(bv, xx, ",", yy, ",", zz)) {
			S[sid].push_back({ xx, yy, zz }); }}

	vector<IVec3> tmp{};  tmp.reserve(50);
	for (int sj=0; sj<S.size()-1; ++sj) {

		for (int r0=0; r0<6; ++r0) {
			for (int r1=0; r1<4; ++r1) {
				//	r0: x0, x1,x2, x3, y1, y3 (all possible sides "up")
				//	r1: z0, z1, z2, z3 = 24 (pointing n/s/e/w)
				Affine a{};
				a.tx = r0 < 4 ? r0 : 0;
				a.ty = r0 == 4 ? 1 : (r0 == 5 ? 3 : 0);
				a.tz = r1;

				tmp.clear();
				for (auto p : S[sj]) {
					tmp.emplace_back(Transform(p, a)); }

				for (int sk=sj+1; sk<S.size(); ++sk) {
					umap<int64_t, int> dirmatch{};
					int bestCnt{};
					IVec3 bestOffset;
					for (const auto& pj : tmp) {
						for (const auto& pk : S[sk]) {
							IVec3 offset = pk - pj;
							dirmatch[K(offset)]++;
							int m = dirmatch[K(offset)];
							if (m > bestCnt) {
								bestCnt = m;
								bestOffset = offset; }}}
					if (bestCnt >= 12) {
						// println("match: sj=", sj, " sk=", sk, " r0=", r0, " r1=", r1, " offset=(", bestOffset.x, ",", bestOffset.y, ")");
						Affine forward{ a.tx, a.ty, a.tz, bestOffset };
						Link link{ sj, sk, forward };
						links[sj].push_back(link);
						links[sk].push_back(link); }}}}}

	auto [beacons, centers] = DFS(0);
	uset<int64_t> uniq{};
	for (auto p : beacons) {
		uniq.insert(K(p)); }
	println(uniq.size());

	int p2{};
	for (int j=0; j<centers.size()-1; ++j) {
		for (int k=j+1; k<centers.size(); ++k) {
			auto v = centers[j] - centers[k];
			int dist = abs(v.x) + abs(v.y) + abs(v.z);
			p2 = max(p2, dist); }}
	println(p2);
	return 0; }
