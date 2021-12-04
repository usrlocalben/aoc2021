#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define umap unordered_map
#define uset unordered_set
using vi = std::vector<int>;

struct IVec2 {
	int x{}, y{};
	auto operator+=(IVec2 r) -> IVec2& { x += r.x; y += r.y; return *this; }
	auto operator+(IVec2 r) -> IVec2 { return r += *this; } };

auto ConsumeLine(std::string_view& text) -> std::string_view {
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
