#include "lib.hxx"

int main(int, char **argv) {
	fast_io::ibuf_file input{ string_view{argv[1]} };

	int num;
	int p1{}, prev{9999999};
	int p2{}, sum[3] = { 0 };
	for (int n{}; scan<true>(input, num, "\n"); ++n) {

		// p1
		p1 += num > prev;
		prev = num;

		// p2
		if (n == 0) {
			sum[0] += num; }
		else if (n == 1) {
			sum[0] += num; sum[1] += num; }
		else if (n == 2) {
			sum[0] += num; sum[1] += num; sum[2] += num; }
		else {
			auto laneStarting = n % 3;
			auto laneFinishing = (laneStarting + 1) % 3;
			auto laneSum = sum[laneStarting];
			sum[laneStarting] = 0;
			for (int i=0; i<3; ++i) {
				sum[i] += num; }
			p2 += sum[laneFinishing] > laneSum; }}

	println(p1);
	println(p2);
	return 0; }

