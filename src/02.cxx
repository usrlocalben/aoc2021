#include "lib.hxx"

int main(int, char **argv) {
	fast_io::ibuf_file fd{ fast_io::mnp::os_c_str(argv[1]) };
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
