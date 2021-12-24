#include "lib.hxx"

struct Op { Vol v; bool on; };
auto Encloses(const Op& a, const Op& b) {
	return Encloses(a.v, b.v); }
auto Volume(const Op& a) {
	return Volume(a.v); }

using Ops = vector<Op>;

void Clip(Vol cv, const Ops& ops, Ops& out) {
	for (const auto& op : ops) {
		auto lo = vmax(op.v.l, cv.l);
		auto hi = vmin(op.v.h, cv.h);
		if (lo.x < hi.x && lo.y < hi.y && lo.z < hi.z) {
			out.push_back({ { lo, hi }, op.on }); }}}

auto OT(Vol myvol, const Ops& ops, int d=0) -> int64_t {
	if (ops.empty()) return 0;

	if (ops.size() == 1) {
		return ops[0].on * Volume(ops[0]); }

	IVec3 lmax = ops[0].v.l;
	IVec3 hmin = ops[0].v.h;
	bool allSame{true};
	for (const auto& op : ops) {
		allSame = allSame && (myvol == op.v);
		lmax = vmax(lmax, op.v.l);
		hmin = vmin(hmin, op.v.h); }

	if (allSame) {
		return ops.back().on * Volume(myvol); }

	Ops subOps{}; subOps.reserve(ops.size());
	int64_t ax{};

	// alternate hi/lo cut position
	IVec3 cutpos{ d&1 ? hmin : lmax };

	for (const auto& sv : Divide(myvol, cutpos)) {
		Clip(sv, ops, subOps);
		if (!subOps.empty()) {
			ax += OT(sv, subOps, d+1);
			subOps.clear(); }}
	return ax; }


int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view text{ loader.data(), loader.data() + loader.size() };
	fast_io::ibuffer_view view{ text };

	string act;
	int x1,x2,y1,y2,z1,z2;
	Vol extent1{ {-50,-50,-50}, {51,51,51} };
	Vol extent2{ { oo, oo, oo }, { -oo, -oo, -oo } };
	Ops ops{};
	while (scan<true>(view, act, " x=", x1, "..", x2, ",y=", y1, "..", y2, ",z=", z1, "..", z2, "\n")) {
		++x2, ++y2, ++z2;  // convert to half-open
		Vol v{ { x1, y1, z1 }, { x2, y2, z2 } };
		extent2.l = vmin(extent2.l, v.l);
		extent2.h = vmax(extent2.h, v.h);
		ops.push_back(Op{ v, act=="on" }); }

	Ops p1ops{};
	Clip(extent1, ops, p1ops);
	auto p1 = OT(extent1, p1ops);
	println(p1);

	auto p2 = OT(extent2, ops);
	println(p2);
	return 0; }
