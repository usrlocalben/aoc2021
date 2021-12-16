#include "lib.hxx"

int64_t p1{};
auto R(HexBitStream& s, int d=0) -> pair<int, int64_t> {
	const int ver = s.Consume(3);
	const int typ = s.Consume(3);

	p1 += ver;
	
	if (typ == 4) {
		// literal
		int a;
		int64_t ax{};
		int c{};
		do {
			++c;
			a = s.Consume(5);
			ax = (ax<<4) | (a&0xf); }
		while (a&0b10000);
		// indenterr(d); perrln("literal ver=", ver, " sizeInWords=", c, " value=", ax);
		return { 6 + c*5, ax }; }
	else {
		vector<int64_t> arr{};  arr.reserve(16);
		// operator
		int lt = s.Consume(1);
		int packetSize{};
		if (lt == 0) {
			// 15-bit size in BITS
			int expected = s.Consume(15);
			int payloadSize{};
			// indenterr(d); perrln("oprator ver=", ver, " sizeInBits=", expected);
			while (expected) {
				auto [consumed, num] = R(s, d+1);
				expected -= consumed;
				payloadSize += consumed;
				arr.push_back(num); }
			packetSize = 6 + 1 + 15 + payloadSize; }
		else {
			assert(lt==1);
			// 11-bit size in PACKETS
			int expected = s.Consume(11);
			// indenterr(d); perrln("oprator ver=", ver, " sizeInPackets=", expected);
			int payloadSize{};
			for (int n=0; n<expected; ++n) {
				auto [consumed, num] = R(s, d+1);
				payloadSize += consumed;
				arr.push_back(num); }
			packetSize = 6 + 1 + 11 + payloadSize; }

		int64_t result;
		switch (typ) {
		case 0: // sum
			result = accumulate(ALL(arr), 0LL); break;
		case 1: // product
			result = accumulate(ALL(arr), 1LL, [](auto a, auto b) { return a*b; }); break;
		case 2: // min
			result = accumulate(ALL(arr), oo64, [](auto a, auto b) { return min(a, b); }); break;
		case 3: // max
			result = accumulate(ALL(arr), -oo64, [](auto a, auto b) { return max(a, b); }); break;

		case 5: // GT
			result = arr[0] > arr[1]; break;
		case 6: // GT
			result = arr[0] < arr[1]; break;
		case 7: // EQ
			result = arr[0] == arr[1]; break; }
		return { packetSize, result }; }}


int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view map{ loader.data(), loader.data() + loader.size() };
	HexBitStream bits{ map };
	auto [n, p2] = R(bits);
	println(p1);
	println(p2);
	return 0; }
