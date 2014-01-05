/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
*/
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////
enum AUDIO_ID {				// 定義各種音效的編號
	AUDIO_Jump,			//人物跳躍音效
	AUDIO_MENU,			//選單畫面背景音樂
	AUDIO_decision,		//選單選擇音效
	AUDIO_hit,			//人物砍擊音效
	AUDIO_skill,		//人物技能起頭音效
	AUDIO_bomb,			//必殺爆破 音效
	AUDIO_gameOver,		//遊戲Game Over音效
	AUDIO_gameWin,		//遊戲The End配樂
	AUDIO_stageOver,	//遊戲過關音效
	AUDIO_die,			//人物死亡音效
	AUDIO_map01,		//關卡01 背景音樂
	AUDIO_map02,		//關卡02 背景音樂
	AUDIO_map03,		//關卡03 背景音樂
	AUDIO_map04			//關卡04 背景音樂
};
namespace game_framework {
//###############################################################################################################################
class MovingBarrier;
class Rope;
class Enemy_basic;
class Map{		//--------------地圖 Map Class
public:
	Map();						//--建構子
	~Map();						//--解構子
	void init();				//--初始化
	void onShow();				//--Show出畫面
	void onMove();				//--移動要素
	void LoadBitmap();			//--讀圖
	void LoadMapData(const char *);		//--讀地圖檔
	void LoadEnemyData(const char *);	//--讀關卡怪物資料
	void changeLoading();		//--換關Loading 畫面顯示
	bool IsEmpty(int,int);		//--判斷該點在地圖陣列是否為空的
	bool IsReach(int,int);		//--判斷該點是否到達過關地點
	bool IsFloor(int,int);		//--判斷該點是否為地版
	bool IsWall(int,int);		//--判斷該點始否為牆壁
	bool IsNoland(int,int);		//--判斷該點下方是否沒有東西(沒有地板、障礙物)
	bool IsMoveBarrier(int,int,int);	//--判斷該點是否為障礙物
	int GetSX();				//--取得銀幕座標位置 sx
	int GetSY();				//--取得銀幕座標位置 sy
	void GetInit_h_xy(int *,int *);		//--取得人物關卡開始位置
	void SetSX(int);			//--改變sx (X 加減多少)
	void SetSY(int);			//--改變sy (Y 加減多少)
	int ScreenX(int);			//--轉換x座標 (以地圖座標 轉成 銀幕座標)
	int ScreenY(int);			//--轉換y座標 (以地圖座標 轉成 銀幕座標)
	int GetMapW();				//--取得整關地圖的寬
	int GetMapH();				//--取得整關地圖的高
	int GetPixW();				//--取得地圖解析度 左右
	int GetPixH();				//--取得地圖解析度 上下
	int GetMapTotal();			//--取得關卡總數
	int GetMapId();				//--取得現在關卡Id
	void SetMapId(int);			//--設置現在關卡Id
	bool GetIsWin();			//--取得IsWin的狀態(關卡過關)
	void SetIsWin(bool);		//--設置IsWin的狀態
	bool GetIsGoal();			//--取得IsGoal的狀態(是否已到達過關地點)
	void SetIsGoal(bool);		//--設置IsGoal的狀態
	int GetWinEnemy_number();	//--取得該關卡怪物(須求)擊倒數
	void addMovebarrier(int,int,int,int,int,int,int);	//--建立一個障礙物(配置空間→Vector)
	void addRope(int,int,int);			//--建立一個繩子(配置空間→Vector)
	void SetMovebarrier_Bitmap();		//--設定障礙物圖片
	void SetRope_Bitmap();				//--設定繩子圖片
	int* GetEnemy_id();					//--取得關卡怪物資料的id陣列(種類)
	int* GetEnemy_pos_x();				//--取得關卡怪物資料的pos_x陣列(開始x座標)
	int* GetEnemy_pos_y();				//--取得關卡怪物資料的pox_y陣列(開始y座標)
	vector<MovingBarrier *>GetMovingBarrier();	//--取得該關障礙物Vector資料
	vector<Rope *>GetRope();					//--取得該關繩子Vectoe資料
private:
	CMovingBitmap floor[10],barrier[10],back_map[10],Rope_map[10],goal;	//地版圖、障礙物圖、背景圖、繩子圖、到達目標圖
	CMovingBitmap changeBitmap;	//換關畫面的圖
	CAnimation load,mushroom;	//Loading、香菇動畫(換關畫面)	
	vector<MovingBarrier *> movebarrier;	//該關障礙物資料
	vector<Rope *> rope;		//該關繩子資料
	int map[300][300];			//地圖陣列大小
	int Mapid_now,MapTotal;		//現在關卡Id，關卡總數
	int enemy_id[50],enemy_pos_x[50],enemy_pos_y[50];	//該關怪物(id、開始位置x,y) 陣列
	int sx,sy;					//銀幕位置(位於地圖座標)
	int init_hx,init_hy;		//人物該關初始位置
	int MapW,MapH;				//實際地圖大小(高、寬)
	int PixW,PixH;				//解析度(像素)長x寬
	int BarrierW,BarrierH;		//障礙物大小
	int numB_x,numB_y;			//該關地圖陣列map[numB_x][numB_y]大小
	int win_Enemy_number;		//該關卡怪物(須求)擊倒數
	bool isWin,isGoal;			//關卡過關狀態、到達過關地點狀態
};
//---------------------------------------------------------------
class Chero;
class Rope{		//--------------繩子 Rope Class
public:
	Rope(int,int,int);			//--建構子
	void onShow(Map *);			//--顯示繩子
	void SetBitmap(CMovingBitmap *);	//--讀圖
	bool isClimb(Chero *);		//--取得爬繩子狀態
	int GetId();				//--取得繩子Id
	int GetX1();				//--取得繩子x座標(左上)
	int GetX2();				//--取得繩子y座標
	int GetY1();				//--取得繩子x座標(右下)
	int GetY2();				//--取得繩子y座標
	bool HitRectangle(int,int,int,int);	//是否接觸到
private:
	CMovingBitmap *bitmap;		//繩子圖片
	int x,y,r_id;				//繩子x,y座標、繩子Id
};
//---------------------------------------------------------------
class MovingBarrier{	//--------------障礙物 MovingBarrier Class
	public:
		MovingBarrier();					//--建構子
		MovingBarrier(int,int,int,int,int,int,int);	//--建構子
		void SetBitmap(CMovingBitmap *);	//--讀取圖片
		void onShow(Map *);					//--障礙物顯示
		void onMove();						//--移動要素
		bool HitRectangle(int,int,int);		//--是否接觸
		int GetX1();						//--取得左上x座標
		int GetX2();						//--取得右上x座標	
		int GetY();							//--取得上面y座標
		int GetId();						//--取得障礙物Id
		const int GetFx();					//--取得開始的x座標	
		const int GetFy();					//--取得開始的y座標
		const int GetFx_area();				//--取得 x移動範圍
		const int GetFy_area();				//--取得 y移動範圍
		int GetXchange();					//--取得 x變化量
		int GetYchange();					//--取得 y變化量
		void SetX(int);						//--設置x座標
		void SetY(int);						//--設置y座標
	private:
		CMovingBitmap *barrier;			//礙障物圖片
		int id,dx;						//障礙物Id、不同Id的x2(與x1差)
		int x,y,last_x,last_y;			//x,y座標、上個的x,y座標(用來記憶)
		int x_change,y_change;			//x,y的移動變化量
		bool is_end[2];					//x,y移動結束狀態
		int fix_x,fix_y;				//開始的x,y座標點
		int x_speed,y_speed;			//x,y移動量
		int x_area,y_area;				//x,y移動範圍
};
//---------------------------------------------------------------
class Skill_basic;
class Chero{		//--------------主角	Chero Class
public:
	Chero();					//--建構子
	~Chero();					//--解構子(刪除空間)
	void LoadBitmap();			//--讀圖檔
	void LoadAudio();			//--讀音效檔
	void onShow(Map*);			//--主角顯示
	void onMove(Map*,vector<Enemy_basic *>);	//--移動要素 
	void AttackEnemy(Map *);	//--攻擊動作
	void climb(Map *,bool *);	//--爬繩動作
	void jump(Map*);			//--跳耀動作
	void onfloor(Map *);		//--重力
	void ShowHP(int ,Map *);	//--顯示血條
	void ShowMP(int ,Map *);	//--顯示MP條
	void init(Map *);			//--初始化
	int GetX1();				//--取得x1(人物左面座標)
	int GetX2();				//--取得x1(人物右面座標)
	int GetY1();				//--取得y1(人物頭頂座標)
	int GetY2();				//--取得y2(人物腳踩座標)
	int GetClimb_x1();			//--取得爬繩座標x1
	int GetClimb_x2();			//--取得爬繩座標x2
	int GetClimb_y1();			//--取得爬繩座標y1
	bool addMP(int);			//--增加MP到滿(密技用)
	int GetMP();				//--取得MP值
	bool GetIsMovingUp();		//--取得爬繩狀態(往上)
	int GetLife();				//--取得生命數
	void SetLife(int);			//--設置生命數
	void SetX(int);				//--改變x座標(x+移動變化)
	void SetX1(int);			//--設定人物位置(遊戲開始) x1值
	void SetY2(int);			//--設定人物位置(遊戲開始) y2值
	int IsAttack();				//--取得攻擊動作(第一、二、三段)
	bool IsClimb();				//--取得是否爬繩狀態
	CAnimation GetHitAnimation();	//--取得人物攻擊動作的Animation
	void SetMovingDown(bool);		//--設定狀態 MovingDown
	void SetMovingLeft(bool);		//--設定狀態 MovingLeft
	void SetMovingRight(bool);		//--設定狀態 MovingRight
	void SetMovingUp(bool);			//--設定狀態 MovingUp
	void SetMovingJump(bool);		//--設定狀態 MovingJump
	void SetMovingAttack(bool);		//--設定狀態 MovingAttack
	void SetMovingSkill_A(bool);	//--設定狀態 MovingSkill_A
	void SetMovingSkill_S(bool);	//--設定狀態 MovingSkill_S
	void SetMovingSkill_D(bool);	//--設定狀態 MovingSkill_D
	void SetMovingClimb(bool);		//--設定狀態 MovingClimb
	bool IsLeft();				//--取得isLeft 狀態 
	bool IsAlive();				//--取得is_alive 狀態
	bool IsSkill();				//--取得isSkill 狀態
	bool IsTouchBarrier(vector<MovingBarrier*>,int,int *); //--是否踩在障礙物上
	void SetIsAlive(bool);		//--設置is_alive
	void Skill_now();			//--設置isSkill狀態
	void beHit_by_Enemy(Enemy_basic *);	//--人物被怪物攻擊 動作
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	//--是否接觸
	void addSkill();		//--建立技能(配置空間→Vector)
	vector<Skill_basic *> GetSkill();	//--取得技能Vectoer
private:
	CAnimation Alert[2],Walk[2],Attack1[2],Attack2[2],Attack3[2],climb_Rope;	//警戒、走路、攻擊1、攻擊2、攻擊3、爬繩 動作
	CMovingBitmap Jump[2],BeHit,lifeBitmap,life_0,life_1,life_2;		//跳、被打、愛心、數字0、數字1、數字2
	vector<Skill_basic	*>	hero_skill;		//技能 Vector
	bool isMovingDown;			// 鍵盤↓ 動作(爬繩中往下)
	bool isMovingLeft;			// 鍵盤← 動作(左移)
	bool isMovingRight;			// 鍵盤→ 動作(右移)
	bool isMovingUp;			// 鍵盤↑ 動作(爬繩中 往上)
	bool isMovingAttack;		// 鍵盤Z 動作(是否攻擊中)
	bool isMovingJump;			// 鍵盤Space 動作(是否跳躍)
	bool isMovingClimb;			// 是否爬繩 狀態
	bool isMovingSkill_A;		// 鍵盤A 動作(技能1)
	bool isMovingSkill_S;		// 鍵盤S 動作(技能2)
	bool isMovingSkill_D;		// 鍵盤D 動作(技能按鍵 保留)
	int x,y,last_x,last_y;		//x,y座標、上個x,y座標(記憶中)
	int HP,HP_total,MP,MP_total,life;	//HP值、HP總量、MP值、MP總量、生命數
	int superTime,countBeHit,attackTime,countHit;	//無敵時間倒數、無敵時間、攻擊硬直時間、硬直時間倒數
	bool is_alive,isLeft,beHit,isClimb,hit[3],isSkill;	//存活、面向左、被打、爬繩中、攻擊段數、技能 狀態
	int init_Velocity,init_downVelocity;	// 初始速度(上升、下降)
	int Velocity,downVelocity;			// 目前的速度(點/次)(上升、下降)
	int left_x,right_x,height;		//人物左面、右面、高度(離貼圖點)
	int climb_x1,climb_x2;			//爬繩點(離貼圖點)
};
//---------------------------------------------------------------
//技能
class Skill_basic{
public:
	Skill_basic();			//--建構子
	virtual void LoadBitmap()=0;	//--讀圖
	virtual void onMove(Chero *,Map *){};	//--移動要素
	virtual void onShow(Chero *,Map *){};	//--顯示技能
	virtual void EonMove(Enemy_basic *,Map *){};	//--移動(怪物的)
	virtual void EonShow(Enemy_basic *,Map *){};	//--顯示技能(怪物的)
	virtual void SetIsEndPicture(bool){};	//--設置是否技能中(怪物的)
	virtual void HitEnemy(Enemy_basic *){};	//--打到怪物的 動作
	virtual void init(Chero *,Map *){};		//--初始化
	int GetX1();			//--取得x1座標
	int GetY1();			//--取得y1座標
	virtual int GetX2()=0;	//--取得x2座標
	virtual int GetY2()=0;	//--取得y2座標
	void SetX1(int);		//--設置x1座標
	void SetY1(int);		//--設置y1座標
	bool IsRun();			//--取得是否技能中
	void SetRun(bool);		//--設置isRun狀態
	bool isHitRectangle(int,int,int,int);	//是否接觸
protected:	
	int x,y;		//貼圖座標
	int height;		//高度
	int downVelocity,init_downVelocity;	//初始速度(怪物技能有重力)
	bool isRun;		//技能中狀態
};
//-------------------------------------
class Skill_son01 :public Skill_basic{
public:
	Skill_son01();	//--建構子
	int GetX2();			//--	↓ 同Skill_basic的功能
	int GetY2();	
	void LoadBitmap();	
	void onMove(Chero *,Map *);
	void onShow(Chero *,Map *);
private:
	CMovingBitmap hit[2];	//技能圖片
};
//-------------------------------------
class Skill_son02 :public Skill_basic{
public:
	Skill_son02();	//--建構子
	int GetX2();			//--	↓ 同Skill_basic的功能
	int GetY2();
	void LoadBitmap();
	void onMove(Chero *,Map *);
	void onShow(Chero *,Map *);
private:
	CMovingBitmap hit[2];	//技能圖片
};
//-------------------------------------
class Skill_son03 :public Skill_basic{
public:
	Skill_son03();	//--建構子
	int GetX2();			//--	↓ 同Skill_basic的功能
	int GetY2();
	void LoadBitmap();
	void onMove(Chero *,Map *);
	void onShow(Chero *,Map *);
private:
	CMovingBitmap hit[2];	//技能圖片
};
//-------------------------------------
class Skill_son04 :public Skill_basic{
public:
	Skill_son04();	//--建構子
	void init(Chero *,Map *);	//--初始化
	int GetX2();			//--	↓ 同Skill_basic的功能
	int GetY2();
	void LoadBitmap();
	void onMove(Chero *,Map *);
	void onShow(Chero *,Map *);
	void HitEnemy(Enemy_basic *);
private:
	CAnimation hit[2],menu;	//技能、技能圖示 動畫
};
//-------------------------------------
class Skill_son05 :public Skill_basic{
public:	
	Skill_son05();	//--建構子
	void init(Chero *,Map *);	//--初始化
	void SetBx(Map *);			//--設置Bx
	int GetX2();			//--	↓ 同Skill_basic的功能
	int GetY2();
	void LoadBitmap();
	void onMove(Chero *,Map *);
	void onShow(Chero *,Map *);
	void HitEnemy(Enemy_basic *);
private:
	CAnimation hit[2];	//技能 動畫
	CAnimation bomb0[2],bomb1[2],bomb2[2],menu;	//爆破(1,2,3)、技能圖示 動畫
	int hx,hy,by,bx0[2],bx1[2],bx2[2],dx;	//人物x,y、爆破圖(1,2,3)x座標、調整爆破圖x變化
};
//-------------------------------------
class EnemySkill_son01 :public Skill_basic{
public:
	EnemySkill_son01();	//--建構子
	void onfloor(Map *m,Enemy_basic * e03);	//--重力
	void SetIsEndPicture(bool);		//--設置IsEndPicture
	int GetX2();			//--	↓ 同Skill_basic的功能
	int GetY2();
	void LoadBitmap();
	void EonMove(Enemy_basic *,Map *);
	void EonShow(Enemy_basic  *,Map *);
private:
	CAnimation hit[2];	//技能 動畫
	CMovingBitmap Vanish[2];	//消失 圖片
	bool IsEndPicture;	//消失 圖片
};
//---------------------------------------------------------------
class EnemySkill_son02 :public Skill_basic{
public:
	EnemySkill_son02();	//--建構子
	void onfloor(Map *m,Enemy_basic * e05);	//--重力
	void SetIsEndPicture(bool);		//--設置IsEndPicture
	int GetX2();			//--	↓ 同Skill_basic的功能
	int GetY2();
	void LoadBitmap();
	void EonMove(Enemy_basic *,Map *);
	void EonShow(Enemy_basic  *,Map *);
private:
	CAnimation hit[2];	//技能 動畫
	CMovingBitmap Vanish[2];	//消失 圖片
	bool IsEndPicture;	//消失 圖片
};
//---------------------------------------------------------------
//敵人
class Enemy_basic{
public:
	Enemy_basic();						//--建構子
	virtual void LoadBitmap() = 0;		//--讀圖
	virtual void HitAI(Chero *,Map *)=0;	//--攻擊AI
	virtual bool InArea(Chero *)=0;		//--判斷範圍
	virtual void onMove(Map*,Chero *);	//--移動要素
	virtual void onShow(Map*);			//--顯示怪物
	virtual void init(Map *);			//--初始化
	void onfloor(Map *);				//--重力
	bool IsTouchBarrier(vector<MovingBarrier*>,int,int *);	//--是否踩在障礙物上
	virtual void Walking(Chero *,Map *);	//--走路動作
	void ShowHP(int,Map *);				//--顯示血條
	void HP_reduce(int);				//--扣寫動作
	virtual int SetTotal_HP();			//--重設血量(到滿)
	virtual int GetX1();				//--取得左面x座標(離貼圖點)
	virtual int GetX2();				//--取得右面x座標
	virtual int GetY1();				//--取得頭頂座標
	virtual int GetY2();				//--取得腳座標
	int GetFx() const ;					//--取得開始的x座標
	int GetEnemyId();					//--取得怪物Id
	void SetX(int);						//--設定X座標			
	void SetY(int);						//--設定Y座標
	int GetRang();						//--取得範圍
	bool GetIsLeft();					//--取得isLeft
	void SetIsLeft(int);				//--設置isLeft
	bool IsAlive();						//--取得is_alive
	bool IsAttack();					//--取得isAttack
	bool IsbeHit();						//--取得beHit
	virtual CAnimation GetAttack(){return false;};	//--取得攻擊動畫
	void SetIsAlive(bool);				//--設置is_alive
	void SetbeHit(bool);				//--設置beHit
	int* hit_skill_num();				//--取得hit_num
	void Set_hit_skill_num(int*);		//--設置hitSkill_num
	virtual void beHit_by_Hero(Chero *,Map *);	//--被主角攻擊 動作
	virtual bool isHitHero(Chero *);	//--攻擊主角 動作
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	//--是否碰到參數範圍的矩形
protected:
	CAnimation Move[2],Stand[2],Attack[4];	//移動、站著、攻擊 動畫
	CMovingBitmap BeHit[2],Vanish[2];	//被攻擊、消失 圖片
	int x,y,last_x,last_y;		//x,y座標、上個x,y座標(記憶用)
	int fix_x,FIX_X,FIX_Y;	//開始x,y座標
	int EnemyId;	//怪物Id
	int downVelocity,init_downVelocity;	//下降速度、初始下降速度
	int HP,HP_total;	//HP量、HP總量
	int countHit,HitTime;	//倒數用、攻擊間隔時間
	int STEP_SIZE,MOVE_RANGE;	//移動量、移動範圍
	bool is_alive,isLeft,isAttack,beHit;	//存活、面向左、攻擊、被打 狀態
	int waitTime,count_wait;	//警戒時間、倒數用
	int count_move,moveTime;	//倒數用、移動時間
	int count_P_Direction;		//用於方向判斷
	int left_x,right_x,center,height;	//圖片各數值
	int hit_num,hitSkill_num[2];	//攻擊圖片值
};
//---------------------------------------------------------------
//敵人
class Enemy_son01 : public Enemy_basic{
public:
	Enemy_son01(int ,int );		//--建構子
	void LoadBitmap();			//--同 Enemy_basic 的功能
	void onMove(Map *,Chero *);
	bool InArea(Chero *);
	void HitAI(Chero *,Map *);
private:
};
//-----------------
class Enemy_son02 : public Enemy_basic{
public:
	Enemy_son02(int ,int );		//--建構子
	void LoadBitmap();			//--同 Enemy_basic 的功能
	void onShow(Map*);
	bool InArea(Chero *);
	void HitAI(Chero *,Map *);
	bool isHitHero(Chero *);
	int GetY1();
private:
};
//-----------------
class Enemy_son03 : public Enemy_basic{
public:
	Enemy_son03(int ,int );		//--建構子
	void LoadBitmap();			//--同 Enemy_basic 的功能
	void onShow(Map*);
	CAnimation GetAttack();
	bool InArea(Chero *);
	void HitAI(Chero *,Map *);
	bool isHitHero(Chero *);
	int	GetY1();
private:
	EnemySkill_son01 es01;		//怪物技能(class)
};
//-----------------
class Enemy_son04 : public Enemy_basic{
public:
	Enemy_son04(int ,int );		//--建構子
	void LoadBitmap();			//--同 Enemy_basic 的功能
	bool isHitHero(Chero *);
	bool InArea(Chero *);
	void HitAI(Chero *,Map *);
	int	GetY1();
private:

};
//-----------------
class Enemy_son05 : public Enemy_basic{
public:
	Enemy_son05(int ,int );		//--建構子
	void LoadBitmap();			//--同 Enemy_basic 的功能
	void onShow(Map*);
	CAnimation GetAttack();
	bool InArea(Chero *);
	void HitAI(Chero *,Map *);
	bool isHitHero(Chero *);
	int	GetY1();
private:
	EnemySkill_son02 es02;	//怪物技能(class)
};
//###############################################################################################################################
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////
class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
	void OnInit();  								//--遊戲的初值及圖形設定
	void OnBeginState();							//--設定每次重玩所需的變數
	void OnKeyUp(UINT, UINT, UINT); 				//--處理鍵盤Up的動作
	void OnLButtonDown(UINT nFlags, CPoint point);  //--處理滑鼠的動作
	void OnLButtonUp(UINT nFlags, CPoint point);	//--處理滑鼠的動作
protected:
	void OnMove();									//--移動遊戲元素
	void OnShow();									//--顯示這個狀態的遊戲畫面
private:
	CMovingBitmap menu,aboutGame;					//選單畫面、aboutGame畫面
	CMovingBitmap start,help,exit,bottom;			//start、help、exit、bottom 圖片
	bool IsClickStart,IsClickHelp,IsClickExit;		//start、help、exit選擇判斷
	bool isHelp;									//是否在 aboutGame 狀態
	int start_x,start_y,help_x,help_y,exit_x,exit_y,bottom_x,bottom_y;	//start、help、exit、bottom (x,y座標) 
};
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////
class CGameStateRun : public CGameState {
public:
	CGameStateRun(CGame *g);
	~CGameStateRun();
	void OnBeginState();							//--設定每次重玩所需的變數
	void OnInit();  								//--遊戲的初值及圖形設定
	void OnKeyDown(UINT, UINT, UINT);				//--處理鍵盤Down的動作
	void OnKeyUp(UINT, UINT, UINT);					//--處理鍵盤Up的動作
	void OnLButtonDown(UINT nFlags, CPoint point);  //--處理滑鼠的動作
	void OnLButtonUp(UINT nFlags, CPoint point);	//--處理滑鼠的動作
	void OnRButtonDown(UINT nFlags, CPoint point);  //--處理滑鼠的動作
	void OnRButtonUp(UINT nFlags, CPoint point);	//--處理滑鼠的動作
protected:
	void OnMove();									//--移動遊戲遊戲元素
	void OnShow();									//--顯示這個狀態的遊戲畫面
private:
	CInteger		num,num_total;					//數字顯示用物件
	CMovingBitmap	stageOver,slope;				//stageOver、數字斜線 圖片
	bool			isMovingSP_1,isMovingSP_2;		//密技1,2 狀態
	Map				map;							//地圖物件
	Chero			hero;							//人物物件
	vector<Enemy_basic *>	enemy,stage[5];			//指標(指向該關怪物)、每關卡怪物的記憶體空間(一開始創好)
};
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(The End)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////
class CGameStateWin : public CGameState {
public:
	CGameStateWin(CGame *g);
	void OnBeginState();							//--設定每次重玩所需的變數
	void OnInit();									//--遊戲的初值及圖形設定
	void OnKeyUp(UINT, UINT, UINT); 				//--處理鍵盤Up的動作
	void OnLButtonUp(UINT nFlags, CPoint point);	//--處理滑鼠的動作
protected:
	void OnMove();									//--移動遊戲元素
	void OnShow();									//--顯示這個狀態的遊戲畫面
private:
	int counter;	//倒數之計數器
	int mx,my;		//香菇 x,y座標
	int tx,ty;		//The End圖片 x,y座標
	CAnimation mushroom,m_die;	//香菇動畫
	CMovingBitmap TheEnd;		//The End圖片
};
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////
class CGameStateOver : public CGameState {
public:
	CGameStateOver(CGame *g);
	void OnBeginState();							//--設定每次重玩所需的變數
	void OnInit();									//--遊戲的初值及圖形設定
protected:
	void OnMove();									//--移動遊戲元素
	void OnShow();									//--顯示這個狀態的遊戲畫面
private:
	int counter;	// 倒數之計數器
	CMovingBitmap overBitmap;	//Game Over圖示
};
}	//Code Ending