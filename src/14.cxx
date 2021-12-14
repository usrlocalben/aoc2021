#include "lib.hxx"

using Tally = array<int64_t, 26>;
umap<string, string> rule{};
umap<int, const Tally*> memo{};

void Sum(const Tally& a, const Tally& b, Tally& out) {
	for (int i=0; i<26; ++i) out[i] = a[i] + b[i]; }

auto Key(const string& pair, int level) -> int {
	return (int(pair[0])<<16) | (int(pair[1])<<8) | level; }

auto R(const string& pair, int level) -> const Tally& {
	if (auto hit = memo.find(Key(pair, level)); hit!=end(memo)) {
		return *(hit->se); }

	auto& data = *(new Tally());

	if (level == 0) {
		data[pair[0] - 'A']++;
		data[pair[1] - 'A']++; }
	else {
		auto& ins = rule[pair];
		auto& tl = R(pair.substr(0,1) + ins, level - 1);
		auto& tr = R(ins + pair.substr(1,1), level - 1);
		Sum(tl, tr, data);
		data[ins[0] - 'A']--; }
/*
	perr("CACHE: ", pair, ",", level, ": ");
	for (int i=0; i<26; ++i) {
		if (data[i] == 0) continue;
		perr(fast_io::mnp::chvw(i+'A'), ":", data[i], " "); }
	perr("\n");
*/
	memo[Key(pair, level)] = &data;
	return data; }

int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view text{ loader.data(), loader.data() + loader.size() };

	string init{ ConsumeLine(text) };
	ConsumeLine(text);
	while (!text.empty()) {
		auto line = ConsumeLine(text);
		string key{ ConsumeWord(line) };
		ConsumeWord(line);  // "->"
		string ins{ line };
		rule[key] = line; }

	int64_t p1, p2;

	{
		constexpr int N = 10;
		Tally t{};
		Sum(Tally{}, R(init.substr(0,2), N), t);
		for (int i=1; i<init.size() - 1; ++i) {
			t[init[i] - 'A']--;
			Sum(t, R(init.substr(i,2), N), t); }
		int64_t mn{oo64}, mx{0};
		for (int i=0; i<26; ++i) {
			if (t[i] == 0) continue;
			mn = min(t[i], mn);
			mx = max(t[i], mx); }
		p1 = mx - mn; }

	{
		constexpr int N = 40;
		Tally t{};
		Sum(Tally{}, R(init.substr(0,2), N), t);
		for (int i=1; i<init.size() - 1; ++i) {
			t[init[i] - 'A']--;
			Sum(t, R(init.substr(i,2), N), t); }
		int64_t mn{oo64}, mx{0};
		for (int i=0; i<26; ++i) {
			if (t[i] == 0) continue;
			mn = min(t[i], mn);
			mx = max(t[i], mx); }
		p2 = mx - mn; }

	println(p1);
	println(p2);
	return 0; }
