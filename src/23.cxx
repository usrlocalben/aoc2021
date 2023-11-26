#include "lib.hxx"

constexpr int WIDTH = 13;
constexpr int STRIDE = 14;
constexpr int POP = 8;

constexpr array<int, 4> COST{{ 1, 10, 100, 1000 }};

auto operator<(IVec2 a, IVec2 b) -> bool {
	if (a.y < b.y) return true;
	if (a.y > b.y) return false;
	return a.x < b.x; }

auto Swap(IVec2& a, IVec2& b) {
	swap(a.x, b.x);
	swap(a.y, b.y);}


using state_t = uint64_t;

auto Pack(IVec2 *pos) -> state_t {
	state_t ax{};
	for (int i=0; i<8; i+=2) {
		int ta = pos[i+0].y*16 + pos[i+0].x;
		int tb = pos[i+1].y*16 + pos[i+1].x;
		if (tb < ta) {
			swap(ta, tb); }
		ax = (ax<<8) | ta;
		ax = (ax<<8) | tb; }
	return ax;}

void Unpack(state_t a, IVec2 *pos) {
	for (int i=7; i>=0; --i, a>>=8) {
		int tmp = a & 0xff;
		pos[i].x = tmp % 16;
		pos[i].y = tmp / 16; }}


struct state2 {
	uint8_t buf[16];
	auto operator==(const state2& other) const -> bool {
		return memcmp(buf, other.buf, 16) == 0; } };

struct state2hash {
	auto operator()(const state2& d) const -> size_t {
		auto tmp = reinterpret_cast<const uint32_t*>(&d.buf[0]);
		return hash<uint32_t>{}(tmp[0]) ^ hash<uint32_t>{}(tmp[1]) ^
		       hash<uint32_t>{}(tmp[2]) ^ hash<uint32_t>{}(tmp[3]);}};

void Pack4(const IVec2 * const pos, state2& out){
	for (int i=0; i<16; i+=4) {
		int ta = pos[i+0].y*16 + pos[i+0].x;
		int tb = pos[i+1].y*16 + pos[i+1].x;
		int tc = pos[i+2].y*16 + pos[i+2].x;
		int td = pos[i+3].y*16 + pos[i+3].x;

		// bubble sort p1
		if (tb < ta) swap(ta, tb);
		if (tc < tb) swap(tb, tc);
		if (td < tc) swap(tc, td);

		// bubble sort p2
		if (tb < ta) swap(ta, tb);
		if (tc < tb) swap(tb, tc);

		// bubble sort p3
		if (tb < ta) swap(ta, tb);

		out.buf[i+0] = (uint8_t)ta;
		out.buf[i+1] = (uint8_t)tb;
		out.buf[i+2] = (uint8_t)tc;
		out.buf[i+3] = (uint8_t)td;}}

auto Pack4(IVec2 *pos) -> state2 {
	state2 tmp; Pack4(pos, tmp); return tmp;}

void Unpack4(const state2& a, IVec2 *pos) {
	for (int i=0; i<16; ++i) {
		int tmp = a.buf[i];
		pos[i].x = tmp % 16;
		pos[i].y = tmp / 16; }}



int main(int, char **argv) {
	fast_io::native_file_loader loader{ fast_io::mnp::os_c_str(argv[1]) };
	string_view map{ loader.data(), loader.data() + loader.size() };

	state_t initKey{};
	state2 init2{};
	{
		IVec2 pos[8];
		array<char, 4> ids{};
		for (int y=2; y<=3; ++y) {
			for (int x=3; x<=9; x+=2) {
				// bring into y [0,2] and x [0, 10]
				int ty = map[y*STRIDE+x] - 'A';
				int id = ids[ty]++;
				pos[ty*2 + id] = IVec2{x-1, y-1}; }}

		// copy p1 into p2 slots
		IVec2 pos2[16];
		for (int typ=0; typ<4; ++typ) {
			for (int i=0; i<2; ++i) {
				auto& p = pos[typ*2+i];
				// expand to 4 slots, move y==2 to y==4
				pos2[typ*4+i] = IVec2(p.x, p.y==2?4:1); }}
		// add new rows
		{
			int y=2;
			pos2[('D'-'A')*4+2] = IVec2(2,y);
			pos2[('C'-'A')*4+2] = IVec2(4,y);
			pos2[('B'-'A')*4+2] = IVec2(6,y);
			pos2[('A'-'A')*4+2] = IVec2(8,y);}
		{
			int y=3;
			pos2[('D'-'A')*4+3] = IVec2(2,y);
			pos2[('B'-'A')*4+3] = IVec2(4,y);
			pos2[('A'-'A')*4+3] = IVec2(6,y);
			pos2[('C'-'A')*4+3] = IVec2(8,y);}

		initKey = Pack(pos);
		init2 = Pack4(pos2); }

	int64_t p1{};
	{
		using pll = pair<int64_t, int64_t>;
		using vll = vector<pll>;
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
				break; }}}
				
	println(p1);

	int64_t p2{};
	{
		using dist__state = pair<int64_t, state2>;

		struct ds2greater {
			auto operator()(const dist__state& lhs, const dist__state& rhs) const -> bool {
				return lhs.first > rhs.first; }; };
		priority_queue<dist__state, vector<dist__state>, ds2greater> queue{};
		uset<state2, state2hash> visited{};
		umap<state2, int, state2hash> dist{};
		auto Dist = [&](state2 k) {
			if (auto search = dist.find(k); search != end(dist)) {
				return search->se; }
			return oo; };
		queue.push({ 0, init2 });
		dist[init2] = 0;
		std::vector<IVec2> nexts{};
		while (loaded(queue)) {
			auto [hdist, hposk] = queue.top(); queue.pop();
			if (visited.find(hposk) != end(visited)) continue;
			visited.insert(hposk);

			IVec2 pos[16];
			Unpack4(hposk, pos);

			// transpose positions to get room configs
			int cfg[4][4];
			for (int aa=0; aa<4; ++aa)
				for (int bb=0; bb<4; ++bb)
					cfg[aa][bb] = -1;
			for (int i=0; i<16; ++i) {
				auto py = pos[i].y;
				auto px = pos[i].x;
				int typ = i/4;
				if (1 <= py && py <= 4) {
					int room = px / 2 - 1;
					cfg[room][py-1] = typ;}}

			bool allWin = true;
			for (int i=0; i<16; ++i) {

				int typ = i/4;
				int roomx = typ*2 + 2;
				int selfx = pos[i].x;

				if (roomx == pos[i].x) {
					// in the right room, check if we're in a winning pos
					// winning pos is if all behind us are same type
					bool withMates = true;
					for (int y=pos[i].y; y<4; ++y) {
						withMates &= cfg[typ][y] == typ;}
					if (withMates) continue;  /*win*/ }

				allWin = false;

				nexts.clear();
				if (pos[i].y == 0) {
					// in hallway, want to move into room
					bool othersInRoom = false;
					for (int y=0; y<4; ++y) {
						othersInRoom |= (cfg[typ][y] != typ && cfg[typ][y] != -1);}
					if (othersInRoom) continue;  // room is not ready yet

					int wayLeft  = std::min(selfx, roomx);
					int wayRight = std::max(selfx, roomx);
					bool blocked = false;
					for (int bi=0; bi<16; ++bi) {
						if (bi != i  // not self
							&& pos[bi].y == 0  // in hallway
							&& wayLeft < pos[bi].x && pos[bi].x < wayRight  // in the way
							) {
							blocked = true;
							break; }}
					if (blocked) continue;  // way is blocked

					// way is clear, room is ready
					// move as far back in the room as we can
					int yy=3;
					while (cfg[typ][yy] != -1) yy--;
					nexts.push_back(IVec2(roomx, yy + 1)); }

				else if (1 <= pos[i].y && pos[i].y <= 4) {
					// in room, but not winning -- move to hallway
					bool canLeaveRoom = true;
					for (int yy=pos[i].y - 2; yy>=0; yy--) {
						canLeaveRoom &= cfg[pos[i].x/2-1][yy] == -1;}
					if (!canLeaveRoom) continue;

					for (int dx=0; dx<11; ++dx) {
						if (dx==2 || dx==4 || dx==6 || dx==8) continue; // can't block rooms
						auto sx = pos[i].x;
						int left = std::min(sx, dx);
						int right = std::max(sx, dx);

						bool blocked = false;
						for (int bi=0; bi<16; ++bi) {
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
					auto canKey = Pack4(pos);
					Swap(finalDest, pos[i]);
					auto canCost = Dist(hposk) + cost;
					if (canCost < Dist(canKey)) {
						dist[canKey] = canCost;
						queue.push({ canCost, canKey }); }}}

			if (allWin) {
				p2 = hdist;
				break; }}}

	println(p2);
	return 0; }
