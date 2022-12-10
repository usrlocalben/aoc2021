#include "lib.hxx"

constexpr int WIDTH = 13;
constexpr int STRIDE = 14;
constexpr int POP = 8;

constexpr array<int, 4> COST{{ 1, 10, 100, 1000 }};


/*
auto EntityChar(const Game& g, IVec2 coord) -> char {
	for (int i=0; i<POP; ++i) {
		if (g.e[i] == coord) {
			int ty = i / 2;
			return ty + 'A'; }}
	return '.'; };

auto WhoCanEnter(const Game& g, int x) -> bool {
	array<char, 4> r1{}, r2{};
	for (int i=0; i<POP; ++i) {
		const auto& e = g.e[i];
		if (e.x == 2) {
			(e.y == 1 ? r1 : r2)[e.x/2-1] = 
		if (e.y==1) {
			if (e[i]
	if (x==2) {
*/
		

auto operator<(IVec2 a, IVec2 b) -> bool { if (a.y < b.y) return true; if (a.y > b.y) return false; return a.x < b.x; }

auto Swap(IVec2& a, IVec2& b) {
	swap(a.x, b.x);
	swap(a.y, b.y);}

/*
auto K(IVec2 p) -> int64_t { return p.y*11+p.x; }
auto U2(int64_t a) -> IVec2 {
	int y = a / 11;
	int x = a % 11;
	return IVec2{ x, y }; }
*/

/*
auto K(const Game& g) -> int64_t {
	int64_t k{};
	for (int i=0; i<POP; ++i) {
		k <<= 5;
		k |= K(g.e[i]); }
	k <<= 5;
	k |= K(g.prev.pos);
	k <<= 3;
	k |= g.prev.who;
	return k; }

auto U(int64_t k) -> Game {
	Game g;
	g.prev.who = k&7;  k >>= 3;
	g.prev.pos = U2(k&0x1f);  k >>= 5;
	for (int i=0; i<POP; ++i) {
		g.e[7-i] = U2(k&0x1f);  k >>= 5; }
	return g; }
*/

/*auto Win(const IVec2 * const g) -> bool {
	return g[0]==IVec2{2,1} && g[2]==IVec2{4,1} && g[4]==IVec2{6,1} && g[6]==IVec2{8,1} &&
	       g[1]==IVec2{2,2} && g[3]==IVec2{4,2} && g[5]==IVec2{6,2} && g[7]==IVec2{8,2}; }*/

void perr_(IVec2 a) {
	perr("(", a.x, ",", a.y, ")"); }

/*
void perr_(Game g) {
	auto EntityChar = [&g](IVec2 coord) -> char {
		for (int i=0; i<POP; ++i) {
			if (g.e[i] == coord) {
				int ty = i / 2;
				return ty + 'A'; }}
		return '.'; };

	using fast_io::mnp::chvw;
	// perr("---------\n");
	perr("/############\n");

	perr("#");
	for (int x=0; x<11; ++x) {
		perr(chvw(EntityChar(IVec2{ x, 0 }))); }
	perr("#\n");

	perr("###");
	perr(chvw(EntityChar(IVec2{ 2, 1 })));
	perr("#");
	perr(chvw(EntityChar(IVec2{ 4, 1 })));
	perr("#");
	perr(chvw(EntityChar(IVec2{ 6, 1 })));
	perr("#");
	perr(chvw(EntityChar(IVec2{ 8, 1 })));
	perr("###\n");

	perr("  #");
	perr(chvw(EntityChar(IVec2{ 2, 2 })));
	perr("#");
	perr(chvw(EntityChar(IVec2{ 4, 2 })));
	perr("#");
	perr(chvw(EntityChar(IVec2{ 6, 2 })));
	perr("#");
	perr(chvw(EntityChar(IVec2{ 8, 2 })));
	perr("#\n"); }
*/

auto Pack(IVec2 *pos) -> std::uint64_t {
	std::uint64_t ax{};
	for (int i=0; i<8; i+=2) {
		int ta = pos[i+0].y*16 + pos[i+0].x;
		int tb = pos[i+1].y*16 + pos[i+1].x;
		if (tb < ta) {
			swap(ta, tb); }
		ax = (ax<<8) | ta;
		ax = (ax<<8) | tb; }
	return ax;}

void Unpack(std::uint64_t a, IVec2 *pos) {
	for (int i=7; i>=0; --i, a>>=8) {
		int tmp = a & 0xff;
		pos[i].x = tmp % 16;
		pos[i].y = tmp / 16; }}


int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view map{ loader.data(), loader.data() + loader.size() };

	uint64_t initKey{};
	{
		IVec2 pos[8];
		array<char, 4> ids{};
		for (int y=2; y<=3; ++y) {
			for (int x=3; x<=9; x+=2) {
				// bring into y [0,2] and x [0, 10]
				int ty = map[y*STRIDE+x] - 'A';
				int id = ids[ty]++;
				pos[ty*2 + id] = IVec2{x-1, y-1}; }}
		initKey = Pack(pos);}

	using pll = pair<int64_t, int64_t>;
	using vll = vector<pll>;
	int64_t p1{};
	priority_queue<pll, vll, greater<pll>> queue{};
	uset<int64_t> visited{};
	umap<int64_t, int> dist{};
	auto Dist = [&](int64_t k) {
		if (auto search = dist.find(k); search != end(dist)) {
			return search->se; }
		return oo; };
	queue.push({ 0, initKey });
	dist[initKey] = 0;
	std::vector<IVec2> nexts{};
	while (loaded(queue)) {
		auto [hdist, hposk] = queue.top(); queue.pop();
		if (visited.find(hposk) != end(visited)) continue;
		visited.insert(hposk);

		IVec2 pos[8];
		Unpack(hposk, pos);

		bool allWin = true;
		for (int i=0; i<8; ++i) {

			int typ = i/2;
			int other = typ*2 + (1-(i%2));
			auto mate = pos[other];
			int selfx = pos[i].x;
			int roomx = typ*2 + 2;
			IVec2 coordTop{ roomx, 1 };
			IVec2 coordBot{ roomx, 2 };

			if (pos[i] == coordTop && mate==coordBot) continue; // win
			if (pos[i] == coordBot && mate==coordTop) continue; // win
			allWin = false;

			nexts.clear();
			if (pos[i].y == 0) {
				int whoTop = -1;
				int whoBot = -1;
				for (int bi=0; bi<8; ++bi) {
					if (pos[bi] == coordTop) whoTop = bi;
					if (pos[bi] == coordBot) whoBot = bi; }
				if (whoTop != -1) continue;  // room is full
				if (whoBot != -1 && whoBot != other) continue; // wrong type in room

				int wayLeft  = std::min(selfx, roomx);
				int wayRight = std::max(selfx, roomx);
				bool blocked = false;
				for (int bi=0; bi<8; ++bi) {
					if (bi != i  // not self
					    && pos[bi].y == 0  // in hallway
						&& wayLeft < pos[bi].x && pos[bi].x < wayRight  // in the way
						) {
						blocked = true;
						break; }}
				if (blocked) continue;  // way is blocked

				// way is clear, room is ready, go!
				nexts.push_back(whoBot == -1 ? coordBot : coordTop); }

			else if (pos[i].y==1 || pos[i].y==2 ) {
				// in room, but not winning
				for (int dx=0; dx<11; ++dx) {
					if (dx==2 || dx==4 || dx==6 || dx==8) continue; // can't block rooms
					auto sx = pos[i].x;
					int left = std::min(sx, dx);
					int right = std::max(sx, dx);

					bool blocked = false;
					for (int bi=0; bi<8; ++bi) {
						if (pos[bi].y==0 && left <= pos[bi].x && pos[bi].x <= right) {
							blocked = true;
							break; }}
					if (blocked) continue; // someone is in the way

					nexts.push_back({ dx, 0 }); }}

			// queue all computed moves for pos[i]
			for (auto finalDest : nexts) {
				auto delta = finalDest - pos[i];
				int moves = abs(delta.x) + abs(delta.y);
				int cost = moves * COST[typ];
				Swap(finalDest, pos[i]);
				auto canKey = Pack(pos);
				Swap(finalDest, pos[i]);
				auto canCost = Dist(hposk) + cost;
				if (canCost < Dist(canKey)) {
					dist[canKey] = canCost;
					queue.push({ canCost, canKey }); }}}

		if (allWin) {
			p1 = hdist;
			break; }}
				
	println(p1);
	return 0; }
