#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
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
constexpr int oo{0x3f3f3f3f};
constexpr int64_t oo64{0x3f3f3f3f3f3f3f3f};

struct IVec2 {
	int x{}, y{};
	auto operator+=(IVec2 r) -> IVec2& { x += r.x; y += r.y; return *this; }
	auto operator+(IVec2 r) -> IVec2 { return r += *this; } };

struct IVec3 {
	int x{}, y{}, z{};
	auto operator+=(IVec3 r) -> IVec3& { x += r.x; y += r.y; z += r.z; return *this; }
	auto operator+(IVec3 r) -> IVec3 { return r += *this; }
	auto xy() -> IVec2 { return IVec2{ x, y }; } };

auto ConsumeLine(string_view& text) -> string_view {
	int pos = text.find('\n');
	auto line = text.substr(0, pos);
	text = text.substr(pos + 1);
	return line; }

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
