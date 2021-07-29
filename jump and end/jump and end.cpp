#include <easyx.h>


//棋子类
class Bit {

//因为要在其他位置使用
//就用公开数据
public:
	
	//此棋子位是否有棋子
	bool isThere;
	
	//此棋子位的编号
	int no;
	

	//下面用链表形式组织数据结构

	//左右的棋子
	Bit* beside[2];

	//左斜向的棋子
	Bit* left[2];

	//右斜向的棋子
	Bit* right[2];


	Bit() {

		//初始是有棋子的
		isThere = true;

		//初始编号无意义
		no = -1;

		//链表尾取NULL
		beside[0] = 0; beside[1] = 0;
		left[0] = 0; left[1] = 0;
		right[0] = 0; right[1] = 0;

	}


};


//数组表示15个棋子的状态
//0表示有棋子(蓝)
//1表示无棋子(白)
//2表示被选择的棋子(深蓝)
//3表示错误棋子(红)
int mod[15]{};


IMAGE back,  //白背景
	  f,	 //无棋子(白)
	  t,	 //有棋子(蓝)
	  red,	 //错误棋子(红)
	  slect; //被选择的棋子(深蓝)


//棋盘类
class Board {

public:

	//棋盘上的棋子
	Bit* bit[15];

	//构造棋盘，参数a是缺失棋子位置，line是生成行数，这里 line=5
	Board(int a, int line = 5) {

		//构造链表结构

		//c是当前生成的棋子编号
		int c = 0;

		
		//行
		for (int i = 0; i < line; i++) {


			//列
			for (int j = 0; j <= i; j++) {


				//建立新的棋子
				Bit* b = new Bit;

				//新棋子的编号是c
				b->no = c;

				//将新棋子加入棋盘
				bit[c] = b; 


				//第一个棋子不处理
				if (i == 0) {}



				//行首棋子被左斜棋子邻接
				else if (j == 0) {

					bit[c - i]->left[1] = b;
					b->left[0] = bit[c - i];

				}



				//行尾棋子被旁边棋子和右斜棋子邻接
				else if (j == i) {

					bit[c - 1]->beside[1] = b;
					b->beside[0] = bit[c - 1];
					bit[c - i - 1]->right[1] = b;
					b->right[0] = bit[c - i - 1];

				}



				//其他棋子被旁边棋子.右斜棋子和左斜棋子邻接
				else {

					bit[c - 1]->beside[1] = b;
					b->beside[0] = bit[c - 1];
					bit[c - i - 1]->right[1] = b;
					b->right[0] = bit[c - i - 1];
					bit[c - i]->left[1] = b;
					b->left[0] = bit[c - i];

				}
				

				//下一个棋子
				c++;

			}


		}

		//参数a是缺失棋子位置
		bit[a]->isThere = false;

	}
	

	//是否胜利
	bool is_win() {

		int a = 0;

		for (int i = 0; i < 15; i++) {

			if (bit[i]->isThere) a++;


			//如果棋盘上有棋子的位置超过一个，则未胜利
			if (a > 1)
				return false;

		}

		//如果棋盘上有棋子的位置未超过一个，则胜利
		return true;
	}


	//棋子移动函数，返回值表示该移动是否合法
	//参数a,b表示将a棋子移动到b位置
	bool move(int a,int b) {


		//若a位置无棋子或b位置有棋子，移动非法
		if (!bit[a]->isThere || bit[b]->isThere)
			return false;



		//c表示向前移动还是向后移动
		int c;
		if (a > b)c = 0;
		else c = 1;



		//声明一个棋子指针变量，表示中间位置
		Bit* d;


		//读取a位置旁边的棋子(左右位置由c确定)
		d = bit[a]->beside[c];

		//旁边有棋子位
		if (d != 0) {

			
			//旁边有棋子
			if (d->isThere == true && d->beside[c] != 0) {

				
				//旁边棋子位的旁边是b位置
				if (d->beside[c]->no == b) {
					
					//满足以上条件则移动合法

					//a位置棋子消失
					bit[a]->isThere = false;

					//b位置棋子出现
					bit[b]->isThere = true;

					//d位置棋子被消除
					d->isThere = false;

					//d位置状态设为1
					mod[d->no] = 1;

					//移动合法
					return true;

				}

			}

		}


		//与上面同理
		d = bit[a]->left[c];

		if (d != 0) {
			if (d->isThere == true && d->left[c] != 0) {

				if (d->left[c]->no == b) {

					bit[a]->isThere = false;
					bit[b]->isThere = true;
					d->isThere = false;
					mod[d->no] = 1;

					return true;
				}

			}
		}


		//与上面同理
		d = bit[a]->right[c];

		if (d != 0) {

			if (d->isThere == true && d->right[c] != 0 ) {

				if (d->right[c]->no == b) {

					bit[a]->isThere = false;
					bit[b]->isThere = true;
					d->isThere = false;

					mod[d->no] = 1;
					return true;
				}

			}

		}


		//若不满足上面的任何条件，则非法
		return false;

	}
};


//鼠标输入函数
int input() {

	//a是被选择的棋盘位置
	int a; 

	//鼠标信息
	MOUSEMSG m;

	while (1) {

		//循环获取鼠标信息
		m = GetMouseMsg();

		//a是棋子位置
		a = 0;

		//行
		for (int i = 0; i < 5; i++) {

			//列
			for (int j = 0; j <= i; j++) {

				//<x,y>是棋子在界面上的位置
				int x = 450 - 70 * i + 140 * j; int y = 100 * i + 20 * (i + 1);


				//如果鼠标在a位置点击，则返回位置a
				if (m.x >= x && m.x <= x + 100 && m.y >= y && m.y <= y + 100 && m.uMsg == WM_LBUTTONDOWN) {
					return a;
				}


				//下一个位置
				a++;

			}

		}


	}


}


//渲染器，重加载整个棋盘
void output() {

	//a表示棋子位置
	int a = 0;


	//按棋子位置一个一个的加载

	//行
	for (int i = 0; i < 5; i++) {

		//列
		for (int j = 0; j <= i; j++) {

			//<x,y>表示，棋子在界面上的位置
			int x = 450 - 70 * i + 140 * j;
			int y = 100 * i + 20 * (i + 1);


			//有棋子
			if (mod[a] == 0)
				putimage(x, y, &t);


			//无棋子
			else if (mod[a] == 1)
				putimage(x, y, &f);


			//被选择的棋子
			else if (mod[a] == 2)
				putimage(x, y, &slect);


			//错误棋子
			else if (mod[a] == 3)
				putimage(x, y, &red);


			a++;

		}

	}

}

int main() {

	//加载图片
	

	// 白背景
	loadimage(&back, L"images\\back.png", 1100, 1043);


	//无棋子(白)
	loadimage(&f, L"images\\false.png", 100, 100);


	//有棋子(蓝)
	loadimage(&t, L"images\\true.png", 100, 100);


	//错误棋子(红)
	loadimage(&red, L"images\\red.png", 100, 100);


	//被选择的棋子(深蓝)
	loadimage(&slect, L"images\\slected.png", 100, 100);


	//开辟游戏空间
	initgraph(1100, 700);

	//加载白背景
	putimage(0, 0, &back);

	//加载棋盘界面(无棋盘类)
	output();


	//鼠标输入一个棋子缺失的位置
	int c = input();


	//构造棋盘
	Board* brd = new Board(c);


	//棋子缺失位置的状态设为1
	mod[c] = 1;


	//重加载
	output();


	//m,n表示棋子从m移动到n
	int m = c, n;



	//如果未结束，就持续循环
	while(!brd->is_win()) {


		//选择一个位置给m
		while (mod[m] != 2) {

			//输入m
			m = input();

			//如果m的位置有棋子，就选定m
			if (brd->bit[m]->isThere) {

				mod[m] = 2;

				//重加载
				output();
			}

		}

		//输入n
		n = input();


		//棋子从m移动到n是合法的
		if (brd->move(m, n)) {

			//m棋子消失
			mod[m] = 1;

			//n棋子出现
			mod[n] = 0;

			//重加载
			output();

		}


		//非法则显示错误棋子
		else {

			//m恢复状态
			mod[m] = 0;

			//用o变量记录n位置的状态
			int o = mod[n];

			//n位置提示错误
			mod[n] = 3;
			output();


			//恢复n位置的状态
			mod[n] = o;

			//闪烁0.1秒后恢复
			Sleep(100);
			output();
		}
	}

	//关闭游戏界面
	closegraph();

	return 0;

}
