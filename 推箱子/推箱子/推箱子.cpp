#include <iostream>

using namespace std;

const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";

const int gStageWidth = 8;
const int gStageHight = 5;

//游戏中物品及状态
enum Object {
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN,
};

//初始化，转化为游戏中物品
void initialize(Object* state,const int w,const int h,const char* gStageDate) {
	const char* d = gStageDate;
	int x = 0;
	int y = 0;
	while (*d != '\0') {
		Object t;
		switch (*d) {
		case ' ':t = OBJ_SPACE; break;
		case '#':t = OBJ_WALL; break;
		case '.':t = OBJ_GOAL; break;
		case 'p':t = OBJ_MAN; break;
		case 'o':t = OBJ_BLOCK; break;
		case 'P':t = OBJ_MAN_ON_GOAL; break;
		case 'O':t = OBJ_BLOCK_ON_GOAL; break;
		case '\n':
			x = 0;
			++y;
			t = OBJ_UNKNOWN;
			break;
		default:t = OBJ_UNKNOWN; break;
		}
		++d;
		if (t != OBJ_UNKNOWN) {
			state[y * w + x] = t;
			++x;
		}
	}
}
void draw(const Object *state,const int w,const int h) {
	const char font[] = { ' ','#','.','o','O','p','P' };
	for (int i = 0; i < h;i++) {
		for (int g = 0; g < w; g++) {
			Object a=state[i*w+g];
			cout << font[a];
		}
		cout << "\n";
	}
}
void update(Object *state,const int h,const int w,char input) {
	int dx = 0, dy = 0;
	switch (input) {
	case 'w':dy = -1; break;
	case 's':dy = 1; break;
	case 'a':dx = -1; break;
	case 'd':dx = 1; break;
	}
	int i = -1;
	//检测玩家位置
	for (i = 0; i < w * h; ++i) {
		if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	//转换玩家坐标
	int x = i % w;
	int y = i / w;
	//移动后坐标
	int tx = x + dx;
	int ty = y + dy;
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	//取得玩家数组中位置和到达的位置
	int p = x + y * w;
	int tp = tx + ty * w;
	if (state[tp] == OBJ_GOAL || state[tp] == OBJ_SPACE) {
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
		state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL) {
		//箱子目标位置
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) {
			return;
		}
		//箱子在数组中的目标位置
		int tp2 = tx2 + ty2 * w;
		//可以移动
		if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL) {
			state[tp2] = (state[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}
bool checkClear(const Object *state,const int h,const int w) {
	for (int i = 0; i < h * w; i++) {
		if (state[i] == OBJ_BLOCK) {
			return false;
		}
	}
	return true;
}
int main()
{
	Object* state = new Object[gStageWidth * gStageHight];
	initialize(state, gStageWidth, gStageHight, gStageData);
	while (true) {
		draw(state, gStageWidth, gStageHight);
		if (checkClear(state, gStageHight, gStageWidth)) {
			break;
		}
		cout << "a:left s:down d:right w:up.conmand?\n";
		char input;
		cin >> input;
		update(state, gStageHight, gStageWidth, input);
	}
	cout << "congradulations,you win!!\n";
	delete[] state;
	state = 0;
	return 0;
}