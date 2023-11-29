#include "lib.hxx"

int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view map{ loader.data(), loader.data() + loader.size() };

	int h{};
	for (char ch : map) {
		h += ch == '\n';}
	int stride = map.size() / h;
	int w = stride - 1;
	// print("w=",w," h=",h," stride=",stride,"\n");

	umap<IVec2, char, IVec2Hash> mapa{}, mapb{}, cmp{};
	for (int y=0; y<h; ++y) {
		for (int x=0; x<w; ++x) {
			auto ch = map[y*stride+x];
			if (ch != '.') {
				mapa[IVec2{x,y}] = ch; }}}

	const array<pair<const IVec2, const char>, 2> dirs{{ {{1,0},'>'}, {{0,1},'v'} }};
	IVec2 dim{ w, h };
	int p1{};
	while (1) {
		for (auto [dir, sel] : dirs) {
			mapb.clear();
			for (auto [cur, typ] : mapa) {
				auto next = (cur + dir) % dim;
				mapb[ typ==sel && mapa.find(next)==end(mapa) ? next : cur ] = typ;}
			mapa.swap(mapb); }

		++p1;
		if (mapa == cmp) break;
		cmp = mapa;}
	print(p1);
	return 0; }
