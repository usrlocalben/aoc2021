#include <backward.hpp>
#include <fast_io.h>
#include <fast_io_device.h>
#include "lib.hxx"

using namespace std;

constexpr int DIM = 5;
constexpr int zstride = DIM * DIM;

int main(int, char **argv) {
	fast_io::native_file_loader loader{ string_view(argv[1]) };
	string_view data{ loader.data(), loader.data() + loader.size() };

	auto numstr = ConsumeLine(data);

	vi nums{};  nums.reserve(numstr.size()/2);
	vi card{};  card.reserve(100 * zstride);
	umap<int, vi> nummap{};

	// random nums
	{
		const char *pstart = numstr.data();
		const char *pend = pstart + numstr.size();
		while (pstart < pend) {
			int x;
			auto res = std::from_chars(pstart, pend, x);
			// if (res.ec != std::errc{}) {
			//	throw std::runtime_error("bad num"); }
			pstart = res.ptr + 1;
			nums.push_back(x); }}

	// card nums
	{
		const char *pstart = data.data();
		const char *pend = pstart + data.size();
		while (pstart < pend) {
			int x;
			auto res = std::from_chars(pstart, pend, x);
			if (res.ec == std::errc{}) {
				auto idx = card.size();
				card.push_back(x);
				nummap[x].push_back(idx); }
			pstart = res.ptr + 1; }}

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
