#include <string_view>
#include <memory_resource>
#include <string>
#include <fast_io.h>
#include <fast_io_device.h>
using namespace std;

struct IVec2 {
	int x{}, y{};
	auto operator+=(IVec2 r) -> IVec2& { x += r.x; y += r.y; return *this; }
	auto operator+(IVec2 r) -> IVec2 { return r += *this; } };

	/*std::byte buf[16];
	std::pmr::monotonic_buffer_resource pool(buf, sizeof(buf));
	std::pmr::string dir(&pool);*/

int main(int, char **argv) {
	fast_io::ibuf_file fd{ string_view(argv[1]) };
	std::string dir;
	int m;

	IVec2 pos1{}, pos2{};
	int aim{};
	while (scan<true>(fd, dir, m)) {

		IVec2 dir1, dir2;
		switch (dir[0]) {
		case 'f':
			dir1 = IVec2{ m, 0 };
			dir2 = IVec2{ m, -aim*m };
			break;
		case 'd':
			dir1 = IVec2{ 0, -m };
			dir2 = IVec2{};
			aim += m;
			break;
		case 'u':
			dir1 = IVec2{ 0, m };
			dir2 = IVec2{};
			aim -= m;
			break; }

		pos1 += dir1;
		pos2 += dir2; }

	println(-pos1.y * pos1.x);
	println(-pos2.y * pos2.x);
	return 0; }
