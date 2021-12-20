#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <cstdint>
#include <deque>
#include <numeric>
#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <backward.hpp>
#include <fast_io.h>
#include <fast_io_device.h>

using namespace std;

#define umap unordered_map
#define uset unordered_set
using vi = vector<int>;
using pii = pair<int, int>;
using vii = vector<pair<int, int>>;
#define fi first
#define se second
#define ALL(x) begin(x), end(x)
constexpr int oo{0x3f3f3f3f};
constexpr int64_t oo64{0x3f3f3f3f3f3f3f3f};

// call func(x) for every valid int x found in text
template <class FUNC>
void do_nums(string_view text, FUNC&& func) {
	const char *pstart = text.data();
	const char *pend = pstart + text.size();
	while (pstart < pend) {
		int x;
		auto res = std::from_chars(pstart, pend, x);
		if (res.ec == std::errc{}) {
			func(x); }
		pstart = res.ptr + 1; }}

template <class FUNC>
void do_nums(fast_io::native_file_loader& loader, FUNC&& func) {
	string_view sv{ loader.data(), loader.data() + loader.size() };
	return do_nums(sv, func); }

template <class T>
auto Mod(T a, T b) -> T {
	T tmp = a % b;
	return tmp < 0 ? tmp + b : tmp; }

struct IVec2 {
	int x{}, y{};
	auto operator+=(IVec2 r) -> IVec2& { x += r.x; y += r.y; return *this; }
	auto operator==(IVec2 r) const -> bool { return x==r.x && y==r.y; }
	auto operator!=(IVec2 r) const -> bool { return x!=r.x || y!=r.y; }
	auto operator+(IVec2 r) const -> IVec2 { return r += *this; }
	auto operator/(int r) const -> IVec2 { return IVec2{ x/r, y/r }; }
	auto operator%(int r) const -> IVec2 { return IVec2{ x%r, y%r }; } };

struct IVec3 {
	int x{}, y{}, z{};
	auto operator+=(IVec3 r) -> IVec3& { x += r.x; y += r.y; z += r.z; return *this; }
	auto operator+(IVec3 r) const -> IVec3 { return r += *this; }
	auto operator-(IVec3 r) const -> IVec3 { return IVec3{ x-r.x, y-r.y, z-r.z }; }
	auto xy() const -> IVec2 { return IVec2{ x, y }; } };


auto Dot(IVec3 a, IVec3 b) -> int {
	return a.x*b.x + a.y*b.y + a.z*b.z; }

struct IMat3 {
	IVec3 r0;
	IVec3 r1;
	IVec3 r2;

	IMat3(int a00, int a01, int a02,
		  int a10, int a11, int a12,
		  int a20, int a21, int a22) :
		r0{ a00, a01, a02 },
		r1{ a10, a11, a12 },
		r2{ a20, a21, a22 } {}

	auto operator*(IVec3 v) -> IVec3 {
		return { Dot(r0, v), Dot(r1, v), Dot(r2, v) };}};

constexpr array<int, 4> ICOS{{ 1, 0, -1, 0 }};
constexpr array<int, 4> ISIN{{ 0, 1, 0, -1 }};
auto RotateX(IVec3 a, int t) -> IVec3 {
	if (t == 0) return a;
	t = Mod(t, 4);
	IMat3 m{ 1, 0, 0,
		     0, ICOS[t], -ISIN[t],
			 0, ISIN[t], ICOS[t] };
	return m * a; }
auto RotateY(IVec3 a, int t) -> IVec3 {
	if (t == 0) return a;
	t = Mod(t, 4);
	IMat3 m{ ICOS[t], 0, ISIN[t],
		     0, 1, 0,
			 -ISIN[t], 0, ICOS[t] };
	return m * a; }
auto RotateZ(IVec3 a, int t) -> IVec3 {
	if (t == 0) return a;
	t = Mod(t, 4);
	IMat3 m{ ICOS[t], -ISIN[t], 0,
		     ISIN[t], ICOS[t], 0,
			 0, 0, 1 };
	return m * a; }


auto ConsumeBlock(string_view& text) -> string_view {
	int pos = text.find(string_view{"\n\n"});
	if (pos == string_view::npos) {
		auto line = text;
		text = text.substr(0, 0);
		return line; }
	auto line = text.substr(0, pos+1);
	text = text.substr(pos + 2);
	return line; }

auto ConsumeToken(string_view& text, char delim) -> string_view {
	int pos = text.find(delim);
	auto line = text.substr(0, pos);
	text = text.substr(pos + 1);
	return line; }

auto ConsumeWord(string_view& text, char delim=' ') -> string_view {
	return ConsumeToken(text, delim); }

auto ConsumeLine(string_view& text) -> string_view {
	return ConsumeWord(text, '\n'); }

template <class T>
auto loaded(const T& c) -> bool {
	return !c.empty(); }


auto Unhex(char ch) -> int {
	if ('0' <= ch && ch <= '9') {
		return ch - '0'; }
	return ch - 'A' + 10; }


struct HexBitStream {
	string_view data_;
	int idx_{};

	HexBitStream(string_view data) : data_(data) {}

	auto size() const -> int {
		return data_.size() * 4 - idx_; }

	auto At(int i) const -> int {
		char ch = Unhex(data_[i / 4]);
		int sel = 8 >> (i % 4);
		return (ch & sel) != 0;}

	auto Peek(int n) const -> int {
		int ti = idx_;
		int out{};
		while (n--) {
			out <<= 1;
			out |= At(ti++); }
		return out; }

	auto Consume(int n) -> int {
		int out = Peek(n);
		idx_ += n;
		return out; } };

void indenterr(int n) { for (int i=0; i<n; ++i) perr(" "); }

auto TS(int x) -> int { return x * (x+1) / 2; }

// point / AABB collision
auto Hit(IVec2 tl, IVec2 br, IVec2 p) -> bool {
	return tl.x <= p.x && p.x <= br.x 
		&& br.y <= p.y && p.y <= tl.y; }

/*
auto ConsumeIntOrThrow(std::string_view& text) -> int {
	int num;
	auto res = std::from_chars(text.data(), text.data() + text.size(), num);
	if (res.ec != std::errc{}) {
		throw std::runtime_error("bad num"); }
	text
	return num; }
*/

/*
#include <memory_resource>
std::byte buf[16];
std::pmr::monotonic_buffer_resource pool(buf, sizeof(buf));
std::pmr::string dir(&pool);
*/
