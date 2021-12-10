#include "lib.hxx"

struct BraceDecomp { char ty; bool open; };

auto Decomp(char ch) -> BraceDecomp {
	switch (ch) {
	case '(': return { '(', true };
	case ')': return { '(', false };
	case '[': return { '[', true };
	case ']': return { '[', false };
	case '{': return { '{', true };
	case '}': return { '{', false };
	case '<': return { '<', true };
	case '>': return { '<', false };
	default: break; }
	throw std::runtime_error("decomp: bad char"); }

auto Score1(char ty) -> int {
	switch (ty) {
	case '(': return 3;
	case '[': return 57;
	case '{': return 1197;
	case '<': return 25137;
	default: break; }
	throw std::runtime_error("score: bad char"); }

auto Score2(char ty) -> int {
	switch (ty) {
	case '(': return 1;
	case '[': return 2;
	case '{': return 3;
	case '<': return 4;
	default: break; }
	throw std::runtime_error("score2: bad char"); }

int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view text{ loader.data(), loader.size() };

	std::vector<char> stack{};  stack.reserve(256);

	int p1{};
	vector<int64_t> p2s{};
	while (!text.empty()) {
		auto line = ConsumeLine(text);
		stack.clear();

		bool good{true};
		int ax1{};
		for (auto ch : line) {
			auto brace = Decomp(ch);
			if (brace.open) {
				stack.push_back(brace.ty); }
			else {
				auto expected = stack.back();  stack.pop_back();
				if (expected != brace.ty) {
					if (good) {
						ax1 += Score1(brace.ty);
						good = false; }}}}
		p1 += ax1;

		if (good && !stack.empty()) {
			int64_t ax2{};
			while (!stack.empty()) {
				char needed = stack.back(); stack.pop_back();
				ax2 = ax2 * 5 + Score2(needed); }
			p2s.push_back(ax2); }}

	println(p1);

	nth_element(begin(p2s), begin(p2s) + p2s.size() / 2, end(p2s));
	println(p2s[p2s.size() / 2]);
	return 0; }
