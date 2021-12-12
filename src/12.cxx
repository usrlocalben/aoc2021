#include "lib.hxx"

umap<string, uset<string>> links{};
uset<string> visited{};
string extra{};

void DFS1(const string& pos, int& ax) {
	if (pos == "end") {
		++ax;
		return; }

	const bool limit{ toupper(pos[0]) != pos[0] };
	if (limit) visited.insert(pos);

	for (auto out : links[pos]) {
		if (visited.find(out) == end(visited)) {
			DFS1(out, ax); }}

	if (limit) visited.erase(pos); }


void DFS2(const string& pos, int& ax) {
	if (pos == "end") {
		++ax;
		return; }

	const bool limit{ toupper(pos[0]) != pos[0] };
	if (limit) {
		if (visited.find(pos) == end(visited)) {
			visited.insert(pos); }
		else {
			assert(extra.empty());
			extra = pos; }}

	for (auto out : links[pos]) {
		if (out == "start") continue;
		if (visited.find(out) == end(visited) || extra.empty()) {
			DFS2(out, ax); }}

	if (limit) {
		if (extra == pos) {
			extra.clear(); }
		else {
			visited.erase(pos); }}}


int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view text{ loader.data(), loader.data() + loader.size() };

	// "xc-ay\nstart-EZ\n..."
	while (!text.empty()) {
		auto line = ConsumeLine(text);
		string aa{ ConsumeToken(line, '-') };
		string bb{ line };

		// println(aa, "-", bb);
		links[aa].insert(bb);
		links[bb].insert(aa); }

	int p1{}, p2{};
	DFS1("start", p1);
	assert(visited.empty());
	DFS2("start", p2);
	println(p1);
	println(p2);
	return 0; }
