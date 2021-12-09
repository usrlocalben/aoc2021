#include "lib.hxx"

auto ToBits(string_view s) -> int {
	int ax{};
	for (const auto ch : s) {
		int b = 1 << (ch - 'a');
		ax |= b; }
	return ax; }


int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view text{ loader.data(), loader.size() };

	array<char, 128> decoder{};
	decoder[0b1110111] = 0;
	decoder[0b0100100] = 1;
	decoder[0b1011101] = 2;
	decoder[0b1101101] = 3;
	decoder[0b0101110] = 4;
	decoder[0b1101011] = 5;
	decoder[0b1111011] = 6;
	decoder[0b0100101] = 7;
	decoder[0b1111111] = 8;
	decoder[0b1101111] = 9;

	int p1{};
	int p2{};
	while (!text.empty()) {
		auto line = ConsumeLine(text);

		array<int, 8> masks;
		masks.fill(0x7f);

		// masks for patters with 2, 3, 4, 5, 6 length
		for (int i=0; i<10; ++i) {
			auto pat = ConsumeWord(line);
			masks[pat.size()] &= ToBits(pat); }

		ConsumeWord(line);  // " | "

		array<int, 8> pin{};
		pin.fill(0x7f);

		int bits = masks[2];
		pin[0] &= ~bits;
		pin[1] &= ~bits;
		pin[2] &=  bits;
		pin[3] &= ~bits;
		pin[4] &= ~bits;
		pin[5] &=  bits;
		pin[6] &= ~bits;

		bits = masks[3];
		pin[0] &=  bits;
		pin[1] &= ~bits;
		pin[2] &=  bits;
		pin[3] &= ~bits;
		pin[4] &= ~bits;
		pin[5] &=  bits;
		pin[6] &= ~bits;

		bits = masks[4];
		pin[0] &= ~bits;
		pin[1] &=  bits;
		pin[2] &=  bits;
		pin[3] &=  bits;
		pin[4] &= ~bits;
		pin[5] &=  bits;
		pin[6] &= ~bits;

		bits = masks[5];
		pin[0] &=  bits;
		pin[1] &= ~bits;
		pin[2] &= ~bits;
		pin[3] &=  bits;
		pin[4] &= ~bits;
		pin[5] &= ~bits;
		pin[6] &=  bits;

		bits = masks[6];
		pin[0] &=  bits;
		pin[1] &=  bits;
		pin[2] &= ~bits;
		pin[3] &= ~bits;
		pin[4] &= ~bits;
		pin[5] &=  bits;
		pin[6] &=  bits;

		array<string_view, 4> nums{};
		int ax{};
		for (int i=0; i<4; ++i) {
			auto w = ConsumeWord(line);
			p1 += (w.size() == 2
			       || w.size() == 4
			       || w.size() == 3
			       || w.size() == 7);

			auto tmp = ToBits(w);
			int out{};
			for (int n=0; n<7; ++n) {
				out |= ((pin[n] & tmp) == pin[n]) << n; }
			ax = ax * 10 + decoder[out]; }
		p2 += ax; }

	println(p1);
	println(p2);
	return 0; }
