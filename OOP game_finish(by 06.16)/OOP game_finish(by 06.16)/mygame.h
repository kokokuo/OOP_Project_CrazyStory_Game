/*
 * mygame.h: ���ɮ��x�C��������class��interface
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
enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
	AUDIO_Jump,			//�H�����D����
	AUDIO_MENU,			//���e���I������
	AUDIO_decision,		//����ܭ���
	AUDIO_hit,			//�H����������
	AUDIO_skill,		//�H���ޯ�_�Y����
	AUDIO_bomb,			//�����z�} ����
	AUDIO_gameOver,		//�C��Game Over����
	AUDIO_gameWin,		//�C��The End�t��
	AUDIO_stageOver,	//�C���L������
	AUDIO_die,			//�H�����`����
	AUDIO_map01,		//���d01 �I������
	AUDIO_map02,		//���d02 �I������
	AUDIO_map03,		//���d03 �I������
	AUDIO_map04			//���d04 �I������
};
namespace game_framework {
//###############################################################################################################################
class MovingBarrier;
class Rope;
class Enemy_basic;
class Map{		//--------------�a�� Map Class
public:
	Map();						//--�غc�l
	~Map();						//--�Ѻc�l
	void init();				//--��l��
	void onShow();				//--Show�X�e��
	void onMove();				//--���ʭn��
	void LoadBitmap();			//--Ū��
	void LoadMapData(const char *);		//--Ū�a����
	void LoadEnemyData(const char *);	//--Ū���d�Ǫ����
	void changeLoading();		//--����Loading �e�����
	bool IsEmpty(int,int);		//--�P�_���I�b�a�ϰ}�C�O�_���Ū�
	bool IsReach(int,int);		//--�P�_���I�O�_��F�L���a�I
	bool IsFloor(int,int);		//--�P�_���I�O�_���a��
	bool IsWall(int,int);		//--�P�_���I�l�_�����
	bool IsNoland(int,int);		//--�P�_���I�U��O�_�S���F��(�S���a�O�B��ê��)
	bool IsMoveBarrier(int,int,int);	//--�P�_���I�O�_����ê��
	int GetSX();				//--���o�ȹ��y�Ц�m sx
	int GetSY();				//--���o�ȹ��y�Ц�m sy
	void GetInit_h_xy(int *,int *);		//--���o�H�����d�}�l��m
	void SetSX(int);			//--����sx (X �[��h��)
	void SetSY(int);			//--����sy (Y �[��h��)
	int ScreenX(int);			//--�ഫx�y�� (�H�a�Ϯy�� �ন �ȹ��y��)
	int ScreenY(int);			//--�ഫy�y�� (�H�a�Ϯy�� �ন �ȹ��y��)
	int GetMapW();				//--���o�����a�Ϫ��e
	int GetMapH();				//--���o�����a�Ϫ���
	int GetPixW();				//--���o�a�ϸѪR�� ���k
	int GetPixH();				//--���o�a�ϸѪR�� �W�U
	int GetMapTotal();			//--���o���d�`��
	int GetMapId();				//--���o�{�b���dId
	void SetMapId(int);			//--�]�m�{�b���dId
	bool GetIsWin();			//--���oIsWin�����A(���d�L��)
	void SetIsWin(bool);		//--�]�mIsWin�����A
	bool GetIsGoal();			//--���oIsGoal�����A(�O�_�w��F�L���a�I)
	void SetIsGoal(bool);		//--�]�mIsGoal�����A
	int GetWinEnemy_number();	//--���o�����d�Ǫ�(���D)���˼�
	void addMovebarrier(int,int,int,int,int,int,int);	//--�إߤ@�ӻ�ê��(�t�m�Ŷ���Vector)
	void addRope(int,int,int);			//--�إߤ@��÷�l(�t�m�Ŷ���Vector)
	void SetMovebarrier_Bitmap();		//--�]�w��ê���Ϥ�
	void SetRope_Bitmap();				//--�]�w÷�l�Ϥ�
	int* GetEnemy_id();					//--���o���d�Ǫ���ƪ�id�}�C(����)
	int* GetEnemy_pos_x();				//--���o���d�Ǫ���ƪ�pos_x�}�C(�}�lx�y��)
	int* GetEnemy_pos_y();				//--���o���d�Ǫ���ƪ�pox_y�}�C(�}�ly�y��)
	vector<MovingBarrier *>GetMovingBarrier();	//--���o������ê��Vector���
	vector<Rope *>GetRope();					//--���o����÷�lVectoe���
private:
	CMovingBitmap floor[10],barrier[10],back_map[10],Rope_map[10],goal;	//�a���ϡB��ê���ϡB�I���ϡB÷�l�ϡB��F�ؼй�
	CMovingBitmap changeBitmap;	//�����e������
	CAnimation load,mushroom;	//Loading�B��ۣ�ʵe(�����e��)	
	vector<MovingBarrier *> movebarrier;	//������ê�����
	vector<Rope *> rope;		//����÷�l���
	int map[300][300];			//�a�ϰ}�C�j�p
	int Mapid_now,MapTotal;		//�{�b���dId�A���d�`��
	int enemy_id[50],enemy_pos_x[50],enemy_pos_y[50];	//�����Ǫ�(id�B�}�l��mx,y) �}�C
	int sx,sy;					//�ȹ���m(���a�Ϯy��)
	int init_hx,init_hy;		//�H��������l��m
	int MapW,MapH;				//��ڦa�Ϥj�p(���B�e)
	int PixW,PixH;				//�ѪR��(����)��x�e
	int BarrierW,BarrierH;		//��ê���j�p
	int numB_x,numB_y;			//�����a�ϰ}�Cmap[numB_x][numB_y]�j�p
	int win_Enemy_number;		//�����d�Ǫ�(���D)���˼�
	bool isWin,isGoal;			//���d�L�����A�B��F�L���a�I���A
};
//---------------------------------------------------------------
class Chero;
class Rope{		//--------------÷�l Rope Class
public:
	Rope(int,int,int);			//--�غc�l
	void onShow(Map *);			//--���÷�l
	void SetBitmap(CMovingBitmap *);	//--Ū��
	bool isClimb(Chero *);		//--���o��÷�l���A
	int GetId();				//--���o÷�lId
	int GetX1();				//--���o÷�lx�y��(���W)
	int GetX2();				//--���o÷�ly�y��
	int GetY1();				//--���o÷�lx�y��(�k�U)
	int GetY2();				//--���o÷�ly�y��
	bool HitRectangle(int,int,int,int);	//�O�_��Ĳ��
private:
	CMovingBitmap *bitmap;		//÷�l�Ϥ�
	int x,y,r_id;				//÷�lx,y�y�СB÷�lId
};
//---------------------------------------------------------------
class MovingBarrier{	//--------------��ê�� MovingBarrier Class
	public:
		MovingBarrier();					//--�غc�l
		MovingBarrier(int,int,int,int,int,int,int);	//--�غc�l
		void SetBitmap(CMovingBitmap *);	//--Ū���Ϥ�
		void onShow(Map *);					//--��ê�����
		void onMove();						//--���ʭn��
		bool HitRectangle(int,int,int);		//--�O�_��Ĳ
		int GetX1();						//--���o���Wx�y��
		int GetX2();						//--���o�k�Wx�y��	
		int GetY();							//--���o�W��y�y��
		int GetId();						//--���o��ê��Id
		const int GetFx();					//--���o�}�l��x�y��	
		const int GetFy();					//--���o�}�l��y�y��
		const int GetFx_area();				//--���o x���ʽd��
		const int GetFy_area();				//--���o y���ʽd��
		int GetXchange();					//--���o x�ܤƶq
		int GetYchange();					//--���o y�ܤƶq
		void SetX(int);						//--�]�mx�y��
		void SetY(int);						//--�]�my�y��
	private:
		CMovingBitmap *barrier;			//ê�٪��Ϥ�
		int id,dx;						//��ê��Id�B���PId��x2(�Px1�t)
		int x,y,last_x,last_y;			//x,y�y�СB�W�Ӫ�x,y�y��(�ΨӰO��)
		int x_change,y_change;			//x,y�������ܤƶq
		bool is_end[2];					//x,y���ʵ������A
		int fix_x,fix_y;				//�}�l��x,y�y���I
		int x_speed,y_speed;			//x,y���ʶq
		int x_area,y_area;				//x,y���ʽd��
};
//---------------------------------------------------------------
class Skill_basic;
class Chero{		//--------------�D��	Chero Class
public:
	Chero();					//--�غc�l
	~Chero();					//--�Ѻc�l(�R���Ŷ�)
	void LoadBitmap();			//--Ū����
	void LoadAudio();			//--Ū������
	void onShow(Map*);			//--�D�����
	void onMove(Map*,vector<Enemy_basic *>);	//--���ʭn�� 
	void AttackEnemy(Map *);	//--�����ʧ@
	void climb(Map *,bool *);	//--��÷�ʧ@
	void jump(Map*);			//--��ģ�ʧ@
	void onfloor(Map *);		//--���O
	void ShowHP(int ,Map *);	//--��ܦ��
	void ShowMP(int ,Map *);	//--���MP��
	void init(Map *);			//--��l��
	int GetX1();				//--���ox1(�H�������y��)
	int GetX2();				//--���ox1(�H���k���y��)
	int GetY1();				//--���oy1(�H���Y���y��)
	int GetY2();				//--���oy2(�H���}��y��)
	int GetClimb_x1();			//--���o��÷�y��x1
	int GetClimb_x2();			//--���o��÷�y��x2
	int GetClimb_y1();			//--���o��÷�y��y1
	bool addMP(int);			//--�W�[MP�캡(�K�ޥ�)
	int GetMP();				//--���oMP��
	bool GetIsMovingUp();		//--���o��÷���A(���W)
	int GetLife();				//--���o�ͩR��
	void SetLife(int);			//--�]�m�ͩR��
	void SetX(int);				//--����x�y��(x+�����ܤ�)
	void SetX1(int);			//--�]�w�H����m(�C���}�l) x1��
	void SetY2(int);			//--�]�w�H����m(�C���}�l) y2��
	int IsAttack();				//--���o�����ʧ@(�Ĥ@�B�G�B�T�q)
	bool IsClimb();				//--���o�O�_��÷���A
	CAnimation GetHitAnimation();	//--���o�H�������ʧ@��Animation
	void SetMovingDown(bool);		//--�]�w���A MovingDown
	void SetMovingLeft(bool);		//--�]�w���A MovingLeft
	void SetMovingRight(bool);		//--�]�w���A MovingRight
	void SetMovingUp(bool);			//--�]�w���A MovingUp
	void SetMovingJump(bool);		//--�]�w���A MovingJump
	void SetMovingAttack(bool);		//--�]�w���A MovingAttack
	void SetMovingSkill_A(bool);	//--�]�w���A MovingSkill_A
	void SetMovingSkill_S(bool);	//--�]�w���A MovingSkill_S
	void SetMovingSkill_D(bool);	//--�]�w���A MovingSkill_D
	void SetMovingClimb(bool);		//--�]�w���A MovingClimb
	bool IsLeft();				//--���oisLeft ���A 
	bool IsAlive();				//--���ois_alive ���A
	bool IsSkill();				//--���oisSkill ���A
	bool IsTouchBarrier(vector<MovingBarrier*>,int,int *); //--�O�_��b��ê���W
	void SetIsAlive(bool);		//--�]�mis_alive
	void Skill_now();			//--�]�misSkill���A
	void beHit_by_Enemy(Enemy_basic *);	//--�H���Q�Ǫ����� �ʧ@
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	//--�O�_��Ĳ
	void addSkill();		//--�إߧޯ�(�t�m�Ŷ���Vector)
	vector<Skill_basic *> GetSkill();	//--���o�ޯ�Vectoer
private:
	CAnimation Alert[2],Walk[2],Attack1[2],Attack2[2],Attack3[2],climb_Rope;	//ĵ�١B�����B����1�B����2�B����3�B��÷ �ʧ@
	CMovingBitmap Jump[2],BeHit,lifeBitmap,life_0,life_1,life_2;		//���B�Q���B�R�ߡB�Ʀr0�B�Ʀr1�B�Ʀr2
	vector<Skill_basic	*>	hero_skill;		//�ޯ� Vector
	bool isMovingDown;			// ��L�� �ʧ@(��÷�����U)
	bool isMovingLeft;			// ��L�� �ʧ@(����)
	bool isMovingRight;			// ��L�� �ʧ@(�k��)
	bool isMovingUp;			// ��L�� �ʧ@(��÷�� ���W)
	bool isMovingAttack;		// ��LZ �ʧ@(�O�_������)
	bool isMovingJump;			// ��LSpace �ʧ@(�O�_���D)
	bool isMovingClimb;			// �O�_��÷ ���A
	bool isMovingSkill_A;		// ��LA �ʧ@(�ޯ�1)
	bool isMovingSkill_S;		// ��LS �ʧ@(�ޯ�2)
	bool isMovingSkill_D;		// ��LD �ʧ@(�ޯ���� �O�d)
	int x,y,last_x,last_y;		//x,y�y�СB�W��x,y�y��(�O�Ф�)
	int HP,HP_total,MP,MP_total,life;	//HP�ȡBHP�`�q�BMP�ȡBMP�`�q�B�ͩR��
	int superTime,countBeHit,attackTime,countHit;	//�L�Įɶ��˼ơB�L�Įɶ��B�����w���ɶ��B�w���ɶ��˼�
	bool is_alive,isLeft,beHit,isClimb,hit[3],isSkill;	//�s���B���V���B�Q���B��÷���B�����q�ơB�ޯ� ���A
	int init_Velocity,init_downVelocity;	// ��l�t��(�W�ɡB�U��)
	int Velocity,downVelocity;			// �ثe���t��(�I/��)(�W�ɡB�U��)
	int left_x,right_x,height;		//�H�������B�k���B����(���K���I)
	int climb_x1,climb_x2;			//��÷�I(���K���I)
};
//---------------------------------------------------------------
//�ޯ�
class Skill_basic{
public:
	Skill_basic();			//--�غc�l
	virtual void LoadBitmap()=0;	//--Ū��
	virtual void onMove(Chero *,Map *){};	//--���ʭn��
	virtual void onShow(Chero *,Map *){};	//--��ܧޯ�
	virtual void EonMove(Enemy_basic *,Map *){};	//--����(�Ǫ���)
	virtual void EonShow(Enemy_basic *,Map *){};	//--��ܧޯ�(�Ǫ���)
	virtual void SetIsEndPicture(bool){};	//--�]�m�O�_�ޯत(�Ǫ���)
	virtual void HitEnemy(Enemy_basic *){};	//--����Ǫ��� �ʧ@
	virtual void init(Chero *,Map *){};		//--��l��
	int GetX1();			//--���ox1�y��
	int GetY1();			//--���oy1�y��
	virtual int GetX2()=0;	//--���ox2�y��
	virtual int GetY2()=0;	//--���oy2�y��
	void SetX1(int);		//--�]�mx1�y��
	void SetY1(int);		//--�]�my1�y��
	bool IsRun();			//--���o�O�_�ޯत
	void SetRun(bool);		//--�]�misRun���A
	bool isHitRectangle(int,int,int,int);	//�O�_��Ĳ
protected:	
	int x,y;		//�K�Ϯy��
	int height;		//����
	int downVelocity,init_downVelocity;	//��l�t��(�Ǫ��ޯ঳���O)
	bool isRun;		//�ޯत���A
};
//-------------------------------------
class Skill_son01 :public Skill_basic{
public:
	Skill_son01();	//--�غc�l
	int GetX2();			//--	�� �PSkill_basic���\��
	int GetY2();	
	void LoadBitmap();	
	void onMove(Chero *,Map *);
	void onShow(Chero *,Map *);
private:
	CMovingBitmap hit[2];	//�ޯ�Ϥ�
};
//-------------------------------------
class Skill_son02 :public Skill_basic{
public:
	Skill_son02();	//--�غc�l
	int GetX2();			//--	�� �PSkill_basic���\��
	int GetY2();
	void LoadBitmap();
	void onMove(Chero *,Map *);
	void onShow(Chero *,Map *);
private:
	CMovingBitmap hit[2];	//�ޯ�Ϥ�
};
//-------------------------------------
class Skill_son03 :public Skill_basic{
public:
	Skill_son03();	//--�غc�l
	int GetX2();			//--	�� �PSkill_basic���\��
	int GetY2();
	void LoadBitmap();
	void onMove(Chero *,Map *);
	void onShow(Chero *,Map *);
private:
	CMovingBitmap hit[2];	//�ޯ�Ϥ�
};
//-------------------------------------
class Skill_son04 :public Skill_basic{
public:
	Skill_son04();	//--�غc�l
	void init(Chero *,Map *);	//--��l��
	int GetX2();			//--	�� �PSkill_basic���\��
	int GetY2();
	void LoadBitmap();
	void onMove(Chero *,Map *);
	void onShow(Chero *,Map *);
	void HitEnemy(Enemy_basic *);
private:
	CAnimation hit[2],menu;	//�ޯ�B�ޯ�ϥ� �ʵe
};
//-------------------------------------
class Skill_son05 :public Skill_basic{
public:	
	Skill_son05();	//--�غc�l
	void init(Chero *,Map *);	//--��l��
	void SetBx(Map *);			//--�]�mBx
	int GetX2();			//--	�� �PSkill_basic���\��
	int GetY2();
	void LoadBitmap();
	void onMove(Chero *,Map *);
	void onShow(Chero *,Map *);
	void HitEnemy(Enemy_basic *);
private:
	CAnimation hit[2];	//�ޯ� �ʵe
	CAnimation bomb0[2],bomb1[2],bomb2[2],menu;	//�z�}(1,2,3)�B�ޯ�ϥ� �ʵe
	int hx,hy,by,bx0[2],bx1[2],bx2[2],dx;	//�H��x,y�B�z�}��(1,2,3)x�y�СB�վ��z�}��x�ܤ�
};
//-------------------------------------
class EnemySkill_son01 :public Skill_basic{
public:
	EnemySkill_son01();	//--�غc�l
	void onfloor(Map *m,Enemy_basic * e03);	//--���O
	void SetIsEndPicture(bool);		//--�]�mIsEndPicture
	int GetX2();			//--	�� �PSkill_basic���\��
	int GetY2();
	void LoadBitmap();
	void EonMove(Enemy_basic *,Map *);
	void EonShow(Enemy_basic  *,Map *);
private:
	CAnimation hit[2];	//�ޯ� �ʵe
	CMovingBitmap Vanish[2];	//���� �Ϥ�
	bool IsEndPicture;	//���� �Ϥ�
};
//---------------------------------------------------------------
class EnemySkill_son02 :public Skill_basic{
public:
	EnemySkill_son02();	//--�غc�l
	void onfloor(Map *m,Enemy_basic * e05);	//--���O
	void SetIsEndPicture(bool);		//--�]�mIsEndPicture
	int GetX2();			//--	�� �PSkill_basic���\��
	int GetY2();
	void LoadBitmap();
	void EonMove(Enemy_basic *,Map *);
	void EonShow(Enemy_basic  *,Map *);
private:
	CAnimation hit[2];	//�ޯ� �ʵe
	CMovingBitmap Vanish[2];	//���� �Ϥ�
	bool IsEndPicture;	//���� �Ϥ�
};
//---------------------------------------------------------------
//�ĤH
class Enemy_basic{
public:
	Enemy_basic();						//--�غc�l
	virtual void LoadBitmap() = 0;		//--Ū��
	virtual void HitAI(Chero *,Map *)=0;	//--����AI
	virtual bool InArea(Chero *)=0;		//--�P�_�d��
	virtual void onMove(Map*,Chero *);	//--���ʭn��
	virtual void onShow(Map*);			//--��ܩǪ�
	virtual void init(Map *);			//--��l��
	void onfloor(Map *);				//--���O
	bool IsTouchBarrier(vector<MovingBarrier*>,int,int *);	//--�O�_��b��ê���W
	virtual void Walking(Chero *,Map *);	//--�����ʧ@
	void ShowHP(int,Map *);				//--��ܦ��
	void HP_reduce(int);				//--���g�ʧ@
	virtual int SetTotal_HP();			//--���]��q(�캡)
	virtual int GetX1();				//--���o����x�y��(���K���I)
	virtual int GetX2();				//--���o�k��x�y��
	virtual int GetY1();				//--���o�Y���y��
	virtual int GetY2();				//--���o�}�y��
	int GetFx() const ;					//--���o�}�l��x�y��
	int GetEnemyId();					//--���o�Ǫ�Id
	void SetX(int);						//--�]�wX�y��			
	void SetY(int);						//--�]�wY�y��
	int GetRang();						//--���o�d��
	bool GetIsLeft();					//--���oisLeft
	void SetIsLeft(int);				//--�]�misLeft
	bool IsAlive();						//--���ois_alive
	bool IsAttack();					//--���oisAttack
	bool IsbeHit();						//--���obeHit
	virtual CAnimation GetAttack(){return false;};	//--���o�����ʵe
	void SetIsAlive(bool);				//--�]�mis_alive
	void SetbeHit(bool);				//--�]�mbeHit
	int* hit_skill_num();				//--���ohit_num
	void Set_hit_skill_num(int*);		//--�]�mhitSkill_num
	virtual void beHit_by_Hero(Chero *,Map *);	//--�Q�D������ �ʧ@
	virtual bool isHitHero(Chero *);	//--�����D�� �ʧ@
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	//--�O�_�I��Ѽƽd�򪺯x��
protected:
	CAnimation Move[2],Stand[2],Attack[4];	//���ʡB���ۡB���� �ʵe
	CMovingBitmap BeHit[2],Vanish[2];	//�Q�����B���� �Ϥ�
	int x,y,last_x,last_y;		//x,y�y�СB�W��x,y�y��(�O�Х�)
	int fix_x,FIX_X,FIX_Y;	//�}�lx,y�y��
	int EnemyId;	//�Ǫ�Id
	int downVelocity,init_downVelocity;	//�U���t�סB��l�U���t��
	int HP,HP_total;	//HP�q�BHP�`�q
	int countHit,HitTime;	//�˼ƥΡB�������j�ɶ�
	int STEP_SIZE,MOVE_RANGE;	//���ʶq�B���ʽd��
	bool is_alive,isLeft,isAttack,beHit;	//�s���B���V���B�����B�Q�� ���A
	int waitTime,count_wait;	//ĵ�ٮɶ��B�˼ƥ�
	int count_move,moveTime;	//�˼ƥΡB���ʮɶ�
	int count_P_Direction;		//�Ω��V�P�_
	int left_x,right_x,center,height;	//�Ϥ��U�ƭ�
	int hit_num,hitSkill_num[2];	//�����Ϥ���
};
//---------------------------------------------------------------
//�ĤH
class Enemy_son01 : public Enemy_basic{
public:
	Enemy_son01(int ,int );		//--�غc�l
	void LoadBitmap();			//--�P Enemy_basic ���\��
	void onMove(Map *,Chero *);
	bool InArea(Chero *);
	void HitAI(Chero *,Map *);
private:
};
//-----------------
class Enemy_son02 : public Enemy_basic{
public:
	Enemy_son02(int ,int );		//--�غc�l
	void LoadBitmap();			//--�P Enemy_basic ���\��
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
	Enemy_son03(int ,int );		//--�غc�l
	void LoadBitmap();			//--�P Enemy_basic ���\��
	void onShow(Map*);
	CAnimation GetAttack();
	bool InArea(Chero *);
	void HitAI(Chero *,Map *);
	bool isHitHero(Chero *);
	int	GetY1();
private:
	EnemySkill_son01 es01;		//�Ǫ��ޯ�(class)
};
//-----------------
class Enemy_son04 : public Enemy_basic{
public:
	Enemy_son04(int ,int );		//--�غc�l
	void LoadBitmap();			//--�P Enemy_basic ���\��
	bool isHitHero(Chero *);
	bool InArea(Chero *);
	void HitAI(Chero *,Map *);
	int	GetY1();
private:

};
//-----------------
class Enemy_son05 : public Enemy_basic{
public:
	Enemy_son05(int ,int );		//--�غc�l
	void LoadBitmap();			//--�P Enemy_basic ���\��
	void onShow(Map*);
	CAnimation GetAttack();
	bool InArea(Chero *);
	void HitAI(Chero *,Map *);
	bool isHitHero(Chero *);
	int	GetY1();
private:
	EnemySkill_son02 es02;	//�Ǫ��ޯ�(class)
};
//###############################################################################################################################
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////
class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
	void OnInit();  								//--�C������Ȥιϧγ]�w
	void OnBeginState();							//--�]�w�C�������һݪ��ܼ�
	void OnKeyUp(UINT, UINT, UINT); 				//--�B�z��LUp���ʧ@
	void OnLButtonDown(UINT nFlags, CPoint point);  //--�B�z�ƹ����ʧ@
	void OnLButtonUp(UINT nFlags, CPoint point);	//--�B�z�ƹ����ʧ@
protected:
	void OnMove();									//--���ʹC������
	void OnShow();									//--��ܳo�Ӫ��A���C���e��
private:
	CMovingBitmap menu,aboutGame;					//���e���BaboutGame�e��
	CMovingBitmap start,help,exit,bottom;			//start�Bhelp�Bexit�Bbottom �Ϥ�
	bool IsClickStart,IsClickHelp,IsClickExit;		//start�Bhelp�Bexit��ܧP�_
	bool isHelp;									//�O�_�b aboutGame ���A
	int start_x,start_y,help_x,help_y,exit_x,exit_y,bottom_x,bottom_y;	//start�Bhelp�Bexit�Bbottom (x,y�y��) 
};
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////
class CGameStateRun : public CGameState {
public:
	CGameStateRun(CGame *g);
	~CGameStateRun();
	void OnBeginState();							//--�]�w�C�������һݪ��ܼ�
	void OnInit();  								//--�C������Ȥιϧγ]�w
	void OnKeyDown(UINT, UINT, UINT);				//--�B�z��LDown���ʧ@
	void OnKeyUp(UINT, UINT, UINT);					//--�B�z��LUp���ʧ@
	void OnLButtonDown(UINT nFlags, CPoint point);  //--�B�z�ƹ����ʧ@
	void OnLButtonUp(UINT nFlags, CPoint point);	//--�B�z�ƹ����ʧ@
	void OnRButtonDown(UINT nFlags, CPoint point);  //--�B�z�ƹ����ʧ@
	void OnRButtonUp(UINT nFlags, CPoint point);	//--�B�z�ƹ����ʧ@
protected:
	void OnMove();									//--���ʹC���C������
	void OnShow();									//--��ܳo�Ӫ��A���C���e��
private:
	CInteger		num,num_total;					//�Ʀr��ܥΪ���
	CMovingBitmap	stageOver,slope;				//stageOver�B�Ʀr�׽u �Ϥ�
	bool			isMovingSP_1,isMovingSP_2;		//�K��1,2 ���A
	Map				map;							//�a�Ϫ���
	Chero			hero;							//�H������
	vector<Enemy_basic *>	enemy,stage[5];			//����(���V�����Ǫ�)�B�C���d�Ǫ����O����Ŷ�(�@�}�l�Цn)
};
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(The End)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////
class CGameStateWin : public CGameState {
public:
	CGameStateWin(CGame *g);
	void OnBeginState();							//--�]�w�C�������һݪ��ܼ�
	void OnInit();									//--�C������Ȥιϧγ]�w
	void OnKeyUp(UINT, UINT, UINT); 				//--�B�z��LUp���ʧ@
	void OnLButtonUp(UINT nFlags, CPoint point);	//--�B�z�ƹ����ʧ@
protected:
	void OnMove();									//--���ʹC������
	void OnShow();									//--��ܳo�Ӫ��A���C���e��
private:
	int counter;	//�˼Ƥ��p�ƾ�
	int mx,my;		//��ۣ x,y�y��
	int tx,ty;		//The End�Ϥ� x,y�y��
	CAnimation mushroom,m_die;	//��ۣ�ʵe
	CMovingBitmap TheEnd;		//The End�Ϥ�
};
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////
class CGameStateOver : public CGameState {
public:
	CGameStateOver(CGame *g);
	void OnBeginState();							//--�]�w�C�������һݪ��ܼ�
	void OnInit();									//--�C������Ȥιϧγ]�w
protected:
	void OnMove();									//--���ʹC������
	void OnShow();									//--��ܳo�Ӫ��A���C���e��
private:
	int counter;	// �˼Ƥ��p�ƾ�
	CMovingBitmap overBitmap;	//Game Over�ϥ�
};
}	//Code Ending