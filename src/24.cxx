#include "lib.hxx"

vector<int> k1{}, k2{}, zd{};
int M, len;

int clamp(int x, int a=1, int b=9) {
	return x < a ? a : (x > b ? b : x);}

auto BTdn(uint64_t key, int64_t z, int n) -> uint64_t {
	if (n == len) return z==0 ? key : 0;
	auto x = z%M + k1[n];
	z /= zd[n];
	
	int hi = zd[n] == M ? clamp(x) : 9;
	int lo = zd[n] == M ? clamp(x) : 1;
	for (int w = hi; w>=lo; --w) {
		auto zz = x != w ? (z*M + w + k2[n]) : z;
		auto res = BTdn(key*10+w, zz, n+1);
		if (res) return res; }
	return 0;}


auto BTup(uint64_t key, int64_t z, int n) -> uint64_t {
	if (n == len) return z==0 ? key : 0;
	auto x = z%M + k1[n];
	z /= zd[n];

	int hi = zd[n] == M ? clamp(x) : 9;
	int lo = zd[n] == M ? clamp(x) : 1;
	for (int w=lo; w<=hi; ++w) {
		auto zz = x != w ? (z*M + w + k2[n]) : z;
		auto res = BTup(key*10+w, zz, n+1);
		if (res) return res; }
	return 0;}


int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view pgm{ loader.data(), loader.data() + loader.size() };

	int li{-1};
	while (!pgm.empty()) {
		li++;
		auto line = ConsumeLine(pgm);
		auto ins = ConsumeWord(line);
		fast_io::ibuffer_view bv{ line };
		if (ins == "inp")
			li = 0;
		else if (ins == "mod") {
			scan(bv, "x", M);
			/*println("modulus is ", M);*/}
		else if (ins == "add" && li == 5) {
			int tmp;
			scan(bv, "x", tmp);
			k1.emplace_back(tmp);}
		else if (ins == "add" && li == 15) {
			int tmp;
			scan(bv, "y", tmp);
			k2.emplace_back(tmp);}
		else if (ins == "div") {
			int tmp;
			scan(bv, "z", tmp);
			zd.emplace_back(tmp);}
		else {
			/*println(li, "[", line, "]");*/ }}
	len = k1.size();

	auto p1 = BTdn(0, 0, 0);
	auto p2 = BTup(0, 0, 0);
	print(p1, "\n", p2, "\n");
	return 0; }

/*

w = input
x = z%26
z = z/wild
x += key[n]
x = x!=w
y = 25*x + 1
z *= y
y = w+k2[n] * x
z += y

*/
