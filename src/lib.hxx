#pragma once

struct IVec2 {
	int x{}, y{};
	auto operator+=(IVec2 r) -> IVec2& { x += r.x; y += r.y; return *this; }
	auto operator+(IVec2 r) -> IVec2 { return r += *this; } };


/*
#include <memory_resource>
std::byte buf[16];
std::pmr::monotonic_buffer_resource pool(buf, sizeof(buf));
std::pmr::string dir(&pool);
*/
