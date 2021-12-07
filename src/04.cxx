#include "lib.hxx"

constexpr int DIM = 5;
constexpr int zstride = DIM * DIM;

int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view data{ loader.data(), loader.data() + loader.size() };

	auto numstr = ConsumeLine(data);

	vi nums{};  nums.reserve(numstr.size()/2);
	vi card{};  card.reserve(100 * zstride);
	umap<int, vi> nummap{};

	do_nums(numstr, [&](int x) {
		nums.push_back(x); });

	do_nums(data, [&](int x) {
		auto idx = card.size();
		card.push_back(x);
		nummap[x].push_back(idx); });

	assert(card.size() % zstride == 0);

	vi mark(card.size(), 0);
	vi colCnt(card.size() / zstride * DIM, 0);
	vi rowCnt(card.size() / zstride * DIM, 0);
	umap<int, int> scores{};
	int firstBoard{-1}, lastBoard{};
	for (auto num : nums) {
		for (auto idx : nummap[num]) {
			int board = idx / zstride;
			if (scores.find(board) != end(scores)) continue;
			mark[idx] = 1;
			int cell{ idx % zstride };
			int row = cell / DIM;
			int col = cell % DIM;
			++colCnt[board*DIM + col];
			++rowCnt[board*DIM + row];
			if (colCnt[board*DIM + col]==DIM ||
				rowCnt[board*DIM + row]==DIM) {
				if (firstBoard == -1) firstBoard = board;
				lastBoard = board;

				int us{};
				for (int i=0; i<zstride; ++i) {
					us += card[board*zstride + i] * (1 - mark[board*zstride + i]); }
				scores[board] = num * us; }}}

	println(scores[firstBoard]);
	println(scores[lastBoard]);
	return 0; }
