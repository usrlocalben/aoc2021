#include "lib.hxx"

struct Node {
	int vl{};
	Node* nl{nullptr};
	int vr{};
	Node* nr{nullptr}; };

auto Parse(string_view s) -> Node* {
	assert(s[0] == '[');
	int mid = 1;
	int d{};
	while (1) {
		if (s[mid]=='[') ++d;
		else if (s[mid]==']') --d;
		else if (s[mid]==',') {
			if (d==0) break; }
		else {}
		++mid;}
	auto sl = s.substr(1, mid - 1);
	auto sr = s.substr(mid + 1, s.size() - mid - 2);

	Node& out = *new Node{};

	if (sl.size() == 1) {
		out.vl = sl[0] - '0'; }
	else {
		out.nl = Parse(sl); }
	if (sr.size() == 1) {
		out.vr = sr[0] - '0'; }
	else {
		out.nr = Parse(sr); }

	return &out; }

void Print(const Node* n) {
	print("[");

	if (n->nl) {
		Print(n->nl); }
	else {
		print(n->vl); }

	print(",");

	if (n->nr) {
		Print(n->nr); }
	else {
		print(n->vr); }

	print("]"); }



int* to_the_left;
Node* explode_parent;
Node* will_explode;
int* to_the_right;
auto MaybeExplode(Node *n, Node *p=nullptr, int d=0) -> bool {
	if (d==0) {
		to_the_left = nullptr;
		will_explode = nullptr;
		to_the_right = nullptr; }

	if (to_the_right) return false;

	if (d==4 && !n->nl && !n->nr && !will_explode) {
		// explode
		explode_parent = p;
		will_explode = n;
		return false; }

	if (n->nl) {
		MaybeExplode(n->nl, n, d+1); }
	else {
		if (!will_explode) {
			to_the_left = &(n->vl); }
		else {
			if (!to_the_right) {
				to_the_right = &(n->vl); }}}

	if (n->nr) {
		MaybeExplode(n->nr, n, d+1); }
	else {
		if (!will_explode) {
			to_the_left = &(n->vr); }
		else {
			if (!to_the_right) {
				to_the_right = &(n->vr); }}}

	if (d==0 && will_explode) {
		if (to_the_left) {
			*to_the_left += will_explode->vl; }
		if (to_the_right) {
			*to_the_right += will_explode->vr; }
		if (explode_parent->nl == will_explode) {
			explode_parent->nl = nullptr;
			explode_parent->vl = 0; }
		else {
			explode_parent->nr = nullptr;
			explode_parent->vr = 0; }
		return true; }
	return false;}


auto MaybeSplit(Node *n) -> bool {

	if (n->nl) {
		if (MaybeSplit(n->nl)) {
			return true; }}
	else {
		if (n->vl > 9) {
			int x = n->vl / 2;
			int y = n->vl - x;
			n->nl = new Node;
			n->nl->vl = x;
			n->nl->vr = y;
			return true; }}

	if (n->nr) {
		if (MaybeSplit(n->nr)) {
			return true; }}
	else {
		if (n->vr > 9) {
			int x = n->vr / 2;
			int y = n->vr - x;
			n->nr = new Node;
			n->nr->vl = x;
			n->nr->vr = y;
			return true; }}
	return false; }


auto Add(Node* a, Node* b) -> Node* {
	if (a == nullptr) {
		return b; }
	Node* out = new Node{};
	out->nl = a;
	out->nr = b;
	while (1) {
		if (MaybeExplode(out)) continue;
		if (MaybeSplit(out)) continue;
		break; }
	return out; }


auto Magnitude(Node *n) -> int {
	int ax{};
	if (n->nl) {
		ax += Magnitude(n->nl) * 3; }
	else {
		ax += n->vl * 3; }
	if (n->nr) {
		ax += Magnitude(n->nr) * 2; }
	else {
		ax += n->vr * 2; }
	return ax; }


int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view text{ loader.data(), loader.data() + loader.size() };
	vector<string_view> lines{};  lines.reserve(100);

	Node* sum{};
	while (!text.empty()) {
		auto l = ConsumeLine(text);
		lines.push_back(l);  // p2
		auto* root =  Parse(l);
		sum = Add(sum, root); }

	// p1
	println(Magnitude(sum));

	int best{};
	for (size_t j=0; j<lines.size()-1; ++j) {
		for (size_t k=j+1; k<lines.size(); ++k) {

			auto a = Parse(lines[j]);
			auto b = Parse(lines[k]);
			a = Add(a, b);
			best = max(best, Magnitude(a));

			a = Parse(lines[k]);
			b = Parse(lines[j]);
			a = Add(a, b);
			best = max(best, Magnitude(a)); }}

	println(best);
	return 0; }
