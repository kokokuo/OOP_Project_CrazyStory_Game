/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
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
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
*/
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <dsound.h>
#include <dmusici.h>
#include "gamelib.h"
#include "mygame.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
////////////////////////////////////////////////////////////////////////////
CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
	IsClickStart = IsClickHelp = IsClickExit = isHelp = false;
	start_x = 180 , start_y = 50;		//�]�wstart ��m
	help_x = 70 , help_y = 127;			//�]�whelp ��m
	exit_x = 290 , exit_y = 127;		//�]�wexit ��m
}
void CGameStateInit::OnInit()
{
	ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
	// �}�l���J���
	menu.LoadBitmap("Bitmaps/menu/menu.bmp");					//���I��		
	aboutGame.LoadBitmap("Bitmaps/menu/aboutGame.bmp");			//aboutGame�e��
	start.LoadBitmap("Bitmaps/menu/start.bmp",RGB(0,0,0) );		//start�Ϥ�
	help.LoadBitmap("Bitmaps/menu/help.bmp",RGB(0,0,0) );		//help�Ϥ�
	exit.LoadBitmap("Bitmaps/menu/exit.bmp",RGB(0,0,0) );		//exit�Ϥ�
	bottom.LoadBitmap("Bitmaps/menu/bottom.bmp",RGB(0,0,0) );	//bottom�Ϥ�
	CAudio::Instance()->Load(AUDIO_MENU,  "sounds/Maple Story�D�D��.wav");	//���I������
	CAudio::Instance()->Load(AUDIO_decision,  "sounds/Decision.wav");		//��ܽT�w����
}
void CGameStateInit::OnBeginState()
{
	CAudio::Instance()->Play(AUDIO_MENU, true);		//����I������
}
void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ESC = 27;		//ESC��
	const char KEY_SPACE = ' ';		//�ť���
	if (nChar == KEY_SPACE && !isHelp){			//�b���OaboutGame�e�����U�ťաA ������GAME_STATE_RUN
		CAudio::Instance()->Play(AUDIO_decision,false);
		CAudio::Instance()->Stop(AUDIO_MENU);
		GotoGameState(GAME_STATE_RUN);			
	}
	else if (nChar == KEY_SPACE)	//aboutGame�e���A���U�ťաA����aboutGame
		isHelp=false;
	if (nChar == KEY_ESC && !isHelp)			//�b���OaboutGame�e�����UESC�A�����C������k
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);		// �����C��
	else if (nChar == KEY_ESC)		//aboutGame�e���A���UESC�A����aboutGame
		isHelp=false;
}
void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(isHelp){}	//�baboutGame�e���A�Ҥ��i�����ʧ@
	else if(point.x >= start_x && point.x <= (start_x+start.Width()) && point.y >= start_y && point.y <= (start_y+start.Height()) ){
		IsClickStart = true;							//���start
		bottom_x = start_x , bottom_y = start_y;
	}
	else if(point.x >= help_x && point.x <= (help_x+help.Width()) && point.y >= help_y && point.y <= (help_y+help.Height()) ){
		IsClickHelp = true;								//���help
		bottom_x = help_x , bottom_y = help_y;
	}
	else if(point.x >= exit_x && point.x <= (exit_x+exit.Width()) && point.y >= exit_y && point.y <= (exit_y+exit.Height()) ){
		IsClickExit = true;								//���exit
		bottom_x = exit_x , bottom_y = exit_y;
	}
	else 
		IsClickStart = IsClickHelp = IsClickExit = false;	
	
	if(IsClickStart || IsClickHelp || IsClickExit)
		CAudio::Instance()->Play(AUDIO_decision,false);	//��ܼ��񭵮�

	OnShow();
}
void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(isHelp)	//�baboutGame�e���A�^����e��
		isHelp = false;
	else if(point.x >= start_x && point.x <= (start_x+start.Width()) && point.y >= start_y && point.y <= (start_y+start.Height()) ){
		CAudio::Instance()->Stop(AUDIO_MENU);		//���start�A������GAME_STATE_RUN
		GotoGameState(GAME_STATE_RUN);				
	}
	else if(point.x >= help_x && point.x <= (help_x+help.Width()) && point.y >= help_y && point.y <= (help_y+help.Height()) ){
		isHelp = true;								//���help�A������aboutGame�e��
	}
	else if(point.x >= exit_x && point.x <= (exit_x+exit.Width()) && point.y >= exit_y && point.y <= (exit_y+exit.Height()) ){
		CAudio::Instance()->Stop(AUDIO_MENU);		//���exit�A�����C��
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);
	}
	IsClickStart = IsClickHelp = IsClickExit = false;
}
void CGameStateInit::OnMove(){
	static bool flag=false;		//�b�̤@�}�l�ɡA���� OnBeginState() �A�u����@��
	if(flag==false){
		OnBeginState();
		flag=true;
	}
}
void CGameStateInit::OnShow()
{
	menu.SetTopLeft(0,0);				//���I�����
	menu.ShowBitmap();
	start.SetTopLeft(start_x,start_y);	//start�Ϥ����
	start.ShowBitmap();
	help.SetTopLeft(help_x,help_y);		//helpt�Ϥ����
	help.ShowBitmap();
	exit.SetTopLeft(exit_x,exit_y);		//exit�Ϥ����
	exit.ShowBitmap();
		if( IsClickStart || IsClickHelp || IsClickExit ){
		bottom.SetTopLeft(bottom_x,bottom_y);	//�p�G��ܡA��ܿ��
		bottom.ShowBitmap();
	}
	if(isHelp){
		aboutGame.SetTopLeft(0,0);		//aboutGame�e�����
		aboutGame.ShowBitmap();
	}
}
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Win)
/////////////////////////////////////////////////////////////////////////////
CGameStateWin::CGameStateWin(CGame *g)
: CGameState(g)
{
	m_die.SetDelayCount(30);	//��ۣ���`����
}
void CGameStateWin::OnBeginState()
{
	CAudio::Instance()->Play(AUDIO_gameWin, true);	//����I������
	counter = 30 * 51; // 51 seconds
	mx = 368,my = -100;	//��l��m
	tx = 134,ty = my+80;
}
void CGameStateWin::OnInit()
{
	TheEnd.LoadBitmap("Bitmaps/map/TheEnd.bmp",RGB(128,0,255));		//TheEnd�Ϥ�
	mushroom.AddBitmap("Bitmaps/menu/mush_0.bmp",RGB(255,0,0) );	//��ۣ���ʹϤ�
	mushroom.AddBitmap("Bitmaps/menu/mush_1.bmp",RGB(255,0,0) );
	mushroom.AddBitmap("Bitmaps/menu/mush_2.bmp",RGB(255,0,0) );
	m_die.AddBitmap("Bitmaps/menu/die01.bmp",RGB(255,0,0) );		//��ۣ�����Ϥ�
	m_die.AddBitmap("Bitmaps/menu/die02.bmp",RGB(255,0,0) );
	m_die.AddBitmap("Bitmaps/menu/die03.bmp",RGB(255,0,0) );
	m_die.AddBitmap("Bitmaps/menu/die04.bmp",RGB(255,0,0) );
	CAudio::Instance()->Load(AUDIO_gameWin,  "sounds/gameWin.wav");	//��������
	ShowInitProgress(100);// �̲׶i�׬�100%
}
void CGameStateWin::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CAudio::Instance()->Stop(AUDIO_gameWin);	//�p�G����L�U����A��������GAME_STATE_INIT
	GotoGameState(GAME_STATE_INIT);
}
void CGameStateWin::OnLButtonUp(UINT nFlags, CPoint point)
{
	CAudio::Instance()->Stop(AUDIO_gameWin);	//�p�G���U�ƹ�����A��������GAME_STATE_INIT
	GotoGameState(GAME_STATE_INIT);
}
void CGameStateWin::OnMove()
{
	static int flag=false;
	int num = mushroom.GetCurrentBitmapNumber();
	if(my<=150 && num == 1 && !flag){	//�b�����Ϥ����ʡATheEnd�B��ۣ��F�w�I������
		my+=10;
		flag=true;
		if(ty < 368) ty = my+80;
	}
	else if(num==1)					
		flag=false;
	if( counter<30*44 && !m_die.IsFinalBitmap() )
		m_die.OnMove();
	mushroom.OnMove();
	counter--;
	if (counter < 0){
		CAudio::Instance()->Stop(AUDIO_gameWin);
		GotoGameState(GAME_STATE_INIT);				//�p�Ƶ����A�۰ʤ��^GAME_STATE_INIT
	}
}
void CGameStateWin::OnShow()
{
	mushroom.SetTopLeft(mx,my);	//�]�w�Ϥ���m
	m_die.SetTopLeft(mx,my);		
	if(counter < 30*44)		//��F�ɶ� �������
		m_die.OnShow();
	else
		mushroom.OnShow();
	TheEnd.SetTopLeft(tx,ty);
	TheEnd.ShowBitmap();		//TheEnd�Ϥ����
}
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
/////////////////////////////////////////////////////////////////////////////
CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{}
void CGameStateOver::OnMove()
{
	counter--;
	if (counter < 0)		//�p�Ƶ�������
		GotoGameState(GAME_STATE_INIT);
}
void CGameStateOver::OnBeginState()
{
	CAudio::Instance()->Play(AUDIO_gameOver, false);//����I������
	counter = 30 * 5; // 5 seconds
}
void CGameStateOver::OnInit()
{
	ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
		overBitmap.LoadBitmap("Bitmaps/map/gameOver.bmp",RGB(0,0,0));	//gameOver�ϥ�
	CAudio::Instance()->Load(AUDIO_gameOver,  "sounds/gameOver.mid");	//gameOver����
	ShowInitProgress(100);// �̲׶i�׬�100%
}
void CGameStateOver::OnShow()
{
	overBitmap.SetTopLeft(148,188);
	overBitmap.ShowBitmap();//gameOver��ܹϤ�
}
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////
CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g)
{
	isMovingSP_1=isMovingSP_2=false;//��l
}
CGameStateRun::~CGameStateRun()
{
	//---------------------------------�R���Ǫ����
	for(int j=0;j<5;j++){
		for(int i=0;i<(int)stage[j].size();i++)
			delete stage[j][i];
		stage[j].clear();
	}
	//---------------------------------
}
void CGameStateRun::OnBeginState()
{
	static int MapId=map.GetMapId();
	static bool IsInit=true;
	bool over=false;	//gameOver���A
	bool win=false;		//TheEnd���A
	//---------------------------------------------------�O�_�����C��
	if(hero.GetLife()==0 && !hero.IsAlive() && IsInit)
		over=true;
	else if( !hero.IsAlive() && IsInit )
		hero.SetLife(hero.GetLife()-1);
	//---------------------------------------------------�]�m�O�_��l
	if(!hero.IsAlive() || map.GetIsWin())
		IsInit=false;
	//---------------------------------------------------�O�_�}�l��l���
	int countTime_total = 130;	
	static int countTime = countTime_total;
		if(!IsInit){
		CAudio::Instance()->Stop(AUDIO_map01);		//����Ҧ�����
		CAudio::Instance()->Stop(AUDIO_map02);
		CAudio::Instance()->Stop(AUDIO_map03);
		CAudio::Instance()->Stop(AUDIO_map04);
		countTime--;
		if(countTime==0 || IsInit==true || over){
			IsInit = true;
			countTime = countTime_total;
		}
		if(over ){		//	over=true ������ GameOver
			MapId=0;
			hero.SetLife(2);
			GotoGameState(GAME_STATE_OVER);
		}
	}
	//---------------------------------------------------�O�_���� Loading �e��
	if( IsInit && (map.GetIsWin() || (hero.IsAlive() && MapId==0)) ){
		hero.SetLife(2);
		for(int i=0;i<5;i++){
			map.changeLoading();
			Sleep(400);
		}
	}
	//---------------------------------------------------�H���B�a�ϡB�ޯ��l
	if(IsInit){
		//------------------------���d����
		if(map.GetIsWin()==true) MapId++;
		if( MapId > map.GetMapTotal()-1 ) win=true;
		if(win){			
			MapId=0;
			hero.SetLife(2);
			GotoGameState(GAME_STATE_WIN);
		}
		map.SetMapId(MapId);
		//------------------------�Ǫ����(���d)
		enemy.clear();
		enemy=stage[MapId];
		//------------------------�a��init
		map.init();
		//------------------------�D��init
		int ix,iy;
		map.GetInit_h_xy(&ix,&iy);
		hero.SetX1(ix),	hero.SetY2(iy);
			//���]�w�D����l��l
		hero.init(&map);
		//------------------------�D���ޯ�init
		for(int i=0;i<(int)hero.GetSkill().size();i++)
			hero.GetSkill()[i]->init(&hero,&map);
		//------------------------�Ǫ�init
		for(int i=0;i< (int)enemy.size();i++)
			enemy[i]->init(&map);
		//-----------------------------------------------//�������d����
		if(MapId==0 && !win && !over)
			CAudio::Instance()->Play(AUDIO_map01,true);	
		else if(MapId==1)
			CAudio::Instance()->Play(AUDIO_map02,true);	
		else if(MapId==2)
			CAudio::Instance()->Play(AUDIO_map03,true);	
		else if(MapId==3)
			CAudio::Instance()->Play(AUDIO_map04,true);	
	}
}
void CGameStateRun::OnMove()							
{
	map.onMove();						//�H���B����a�B ���ʹC������
	hero.onMove(&map,enemy);
	for(int i=0;i< (int)enemy.size();i++)
		enemy[i]->onMove(&map,&hero);
	//--------------------------------------------�O�_���(�Ѿl�Ǫ��ƶq)
	int count=0;
	for(int i=0;i< (int)enemy.size();i++)
		if(enemy[i]->IsAlive())
			count++;
	//---------------------------------------------------��� �ƶq/�ƶqtotal
	num.SetInteger( (int)enemy.size()-count );			
	num_total.SetInteger( map.GetWinEnemy_number() );
	//---------------------------------------------------
	if(isMovingSP_2)		//�K��2 ��MP�캡
		if( hero.addMP(5) )
			isMovingSP_2=false;
	if( (int)enemy.size()-count >= map.GetWinEnemy_number() ) //���here����
		map.SetIsGoal(true);
	if( ( map.GetIsGoal() && map.IsReach(hero.GetX2(),hero.GetY2()) ) || isMovingSP_1 ){
		isMovingSP_1=false;			//�L������(�t�K��1)
		map.SetIsWin(true);	
	}
	//---------------------------------------------------���L������
	static bool flag_stage=false;
	if( map.GetIsWin() && !flag_stage){			
		CAudio::Instance()->Play(AUDIO_stageOver, false);
		flag_stage=true;
	}
	else if (!map.GetIsWin() )
		flag_stage=false;
	//---------------------------------------------------�O�_����or����
	if(map.GetIsWin()==true || hero.IsAlive()==false)
		OnBeginState();
	
}
void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
	ShowInitProgress(33);		// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
	num.LoadBitmap();			//�ƶq
	num_total.LoadBitmap();		//�ƶq�`��
	map.LoadBitmap();			//map Ū��
	hero.LoadBitmap();			//hero Ū��
	stageOver.LoadBitmap("Bitmaps/map/stageOver.bmp",RGB(0,0,0) );	//stageOver�Ϥ�
	slope.LoadBitmap("Bitmaps/number/slope.bmp",RGB(128,0,255));	//��u�Ϥ�
	hero.LoadAudio();			//����Ū��
	CAudio::Instance()->Load(AUDIO_stageOver,  "sounds/stageOver.mid");	
	CAudio::Instance()->Load(AUDIO_map01,  "sounds/map_01.wav");	
	CAudio::Instance()->Load(AUDIO_map02,  "sounds/map_02.wav");
	CAudio::Instance()->Load(AUDIO_map03,  "sounds/map_03.wav");
	CAudio::Instance()->Load(AUDIO_map04,  "sounds/map_04.wav");
	//-----------------------------------------------------------
	for(int j=0;j< map.GetMapTotal();j++){//Ū���d�Ǫ����
		string temp[5]={"01","02","03","04","05"};
		map.LoadEnemyData((char *)("File/Data"+temp[j]+".txt").c_str());
		//--------------------�إߩǪ����(�C��)
		for(int i=0;map.GetEnemy_id()[i]!=0;i++){
			int id=map.GetEnemy_id()[i];
			int pos_x=map.GetEnemy_pos_x()[i];
			int pos_y=map.GetEnemy_pos_y()[i];
			if(id==1)
				stage[j].push_back( new Enemy_son01(pos_x,pos_y) );
			else if(id==2)
				stage[j].push_back( new Enemy_son02(pos_x,pos_y) );
			else if(id==3)
				stage[j].push_back( new Enemy_son03(pos_x,pos_y) );
			else if(id==4)
				stage[j].push_back( new Enemy_son04(pos_x,pos_y) );
			else if(id==5)
				stage[j].push_back( new Enemy_son05(pos_x,pos_y) );
		}
		//----------------------------------
		for(int i=0;i< (int)stage[j].size();i++)
			stage[j][i]->LoadBitmap();	//�Ǫ��Ϥ�
	}
	ShowInitProgress(50);
}
void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard���b�Y
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
	const char KEY_JUMP	 = ' '; 
	const char KEY_A = 65;	//�ޯ�A
	const char KEY_S = 83;	//�ޯ�S
	const char KEY_D = 68;	//�ޯ�D
	const char KEY_Z = 90;	//����Z
	const char SP1 = 80;
	const char SP2 = 81;

	if (nChar == KEY_LEFT )
			hero.SetMovingLeft(true);
	if (nChar == KEY_RIGHT)
			hero.SetMovingRight(true);	
	if (nChar == KEY_DOWN)
		hero.SetMovingDown(true);
	if (nChar == KEY_UP)
		hero.SetMovingUp(true);
	
	if(!hero.IsAttack() && !hero.IsSkill() ){ //�b�S�����B�S��ޯ�
		if (nChar == KEY_JUMP)
			hero.SetMovingJump(true);//���D���A
		if( nChar == KEY_UP){
			for(int i=0;i<(int)map.GetRope().size();i++)	//�p�G�b÷�l�d��   ��÷
				if( map.GetRope()[i]->isClimb(&hero) && !hero.IsClimb())
					hero.SetMovingClimb(true);
		}
	}
	if( !hero.IsClimb()){	//��÷�ɡA�������
		if (nChar == KEY_Z )
			hero.SetMovingAttack(true);
		if (nChar == KEY_A )
				hero.SetMovingSkill_A(true);
		if (nChar == KEY_S )
				hero.SetMovingSkill_S(true);
		if (nChar == KEY_D )
				hero.SetMovingSkill_D(true);
	}
	if(nChar == SP1)			//�ޯ�
		isMovingSP_1=true;
	if(nChar == SP2)
		isMovingSP_2=true;
}
void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard���b�Y
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
	if (nChar == KEY_LEFT)
		hero.SetMovingLeft(false);
	if (nChar == KEY_RIGHT)
		hero.SetMovingRight(false);
	if (nChar == KEY_DOWN)
		hero.SetMovingDown(false);
	if (nChar == KEY_UP)
		hero.SetMovingUp(false);
}
void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point){}  // �B�z�ƹ����ʧ@
void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point){}	// �B�z�ƹ����ʧ@
void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point){}  // �B�z�ƹ����ʧ@
void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point){}	// �B�z�ƹ����ʧ@
void CGameStateRun::OnShow()
{
	map.onShow();					//�a�����
	for(int i=0;i<(int)enemy.size();i++)
		enemy[i]->onShow(&map);		//�Ǫ����
	hero.onShow(&map);				//�D�����
	stageOver.SetTopLeft(148,188);
	if( map.GetIsWin() )			
		stageOver.ShowBitmap();		//�L���ϥ����
	//------------------------------
	int ad_x;
	if(num_total.GetInteger()<10)	//�վ�Ǫ����m
		ad_x=40;
	else
		ad_x=0;
	num.SetTopLeft(630+ad_x,10);	//�p��Ǫ��� ���
	slope.SetTopLeft(630+ad_x,6);
	num_total.SetTopLeft(780,10);
	num.ShowBitmap();
	slope.ShowBitmap();
	num_total.ShowBitmap();
	//--------------------------------------------------��ܴ��ո��
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// ���� font f; 160���16 point���r
	fp=pDC->SelectObject(&f);					// ��� font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[120];								// Demo �Ʀr��r�ꪺ�ഫ
	sprintf(str,"hero_x=%d hero_y=%d",hero.GetX1(),hero.GetY2());
//	pDC->TextOut(5,map.GetPixH()-25,str);
	pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}
//###################################################################################################################################
//�C���a�ϻ�ê
Map::Map(){  
	const int Mapid_init=0;//��l���d�]�w
	Mapid_now = Mapid_init;
	PixW=800,PixH=600;
	BarrierW = 90;	//�a�ϸ̥H�}�C�ӬݨC�@����h��(�I)
	BarrierH = 30;	//�a�ϸ̥H�}�C�ӬݨC�@�氪�h��(�I)
	numB_x=0;	//x�b�@�h�֮�
	numB_y=0;	//y�b�@�h�֮�
	MapTotal=4;		//���d�Ƴ]�w
	init();
	load.SetDelayCount(1);
	mushroom.SetDelayCount(1);
}
Map::~Map(){
	//------------------------------------------ �R���Ŷ�
	for(int i=0;i<(int)movebarrier.size();i++)
		delete movebarrier[i];
	movebarrier.clear();
	for(int i=0;i<(int)rope.size();i++)
		delete rope[i];
	rope.clear();
	//------------------------------------------
}
void Map::init(){
	SetIsWin(false);
	SetIsGoal(false);
	//------------------------------------------ �R���Ŷ�
	for(int i=0;i<(int)movebarrier.size();i++)
		delete movebarrier[i];
	movebarrier.clear();
	for(int i=0;i<(int)rope.size();i++)
		delete rope[i];
	rope.clear();
	//------------------------------------------ Ū���d�����
	for(int i=0;i<300;i++)
		for(int j=0;j<300;j++)
			map[i][j]=0;
	string temp[5]={"01","02","03","04","05"};
	for(int i=0;i<5;i++)
		if(i==Mapid_now)
			LoadMapData((char *)("File/Map"+temp[i]+".txt").c_str());
	sx=0;
	sy=200;
	//------------------------------------�t�m���d Movebarrier�BRope �Ŷ�
	for(int j=0;j< numB_y;j++)
		for(int i=0;i< numB_x;i++){
			if(map[i][j]==9)//��ê��������
				addMovebarrier(3,i*BarrierW,j*BarrierH,0,0,0,0);
			if(map[i][j]==8)//��ê���p�Ӫ�
				addMovebarrier(1,i*BarrierW,j*BarrierH,0,0,0,0);
			if(map[i][j]==7)//�P�W
				addMovebarrier(2,i*BarrierW,j*BarrierH,0,0,0,0);
			if(map[i][j]==10)//÷�l�u��
				addRope(10,i*BarrierW,j*BarrierH);
			if(map[i][j]==11)//÷�l�y����
				addRope(11,i*BarrierW,j*BarrierH);
			if(map[i][j]==12)//÷�l����
				addRope(12,i*BarrierW,j*BarrierH);
			if(map[i][j]==6)//÷�l�ܪ���
				addRope(6,i*BarrierW,j*BarrierH);
		}
	//------------------------------------
	SetMovebarrier_Bitmap();	// MovebarrierŪ��
	SetRope_Bitmap();			// RopeŪ��
	MapW = BarrierW * numB_x;	//�a�Ϫ��e
	MapH = BarrierH * numB_y;	
}
void Map::LoadEnemyData(const char *fn){	
	for(int i=0;i<50;i++)		//Ū�����w���d Enemy ���
		enemy_id[i]=0;
	ifstream ifs(fn);
	string s;
	int i=0;
	while( !ifs.eof() ){		// enemy : enemy_id : enemy_pos_x,enemy_pos_y
		getline(ifs,s,':');
		getline(ifs,s,':');
			enemy_id[i]=atoi(s.c_str ());
		getline(ifs,s,',');
			enemy_pos_x[i]=atoi(s.c_str ());
		getline(ifs,s);
			enemy_pos_y[i++]=atoi(s.c_str ());
	}
	ifs.close();
}
void Map::LoadMapData(const char *fn){
	//-------------------------------------------/Ū�����w���d Map ���	
	ifstream ifs(fn);
	int id,data[6];
	string s;
	int tx,ty;
	//-----------------------	map(nx,ny): numB_x,numB_y
	getline(ifs,s,':');
	getline(ifs,s,',');
	numB_x = atoi(s.c_str ());
	getline(ifs,s);
	numB_y = atoi(s.c_str ());
	//-----------------------	hero(x,y): init_x,init_y		
	getline(ifs,s,':');
	getline(ifs,s,',');
	init_hx = atoi(s.c_str ());
	getline(ifs,s);
	init_hy = atoi(s.c_str ());
	//-----------------------	Enemy_number: win_Enemy_number
	getline(ifs,s,':');
	getline(ifs,s);
	win_Enemy_number = atoi(s.c_str ());
	//-----------------------	Map �}�C���
	for(ty=0; ty < numB_y ;ty++){
		for(tx=0; tx < numB_x-1 ; tx++){
			getline(ifs,s,',');
			map[tx][ty]=atoi(s.c_str ());
		}
		getline(ifs,s);
		map[tx][ty]=atoi(s.c_str ());
	}
	getline(ifs,s);	//�Ť@��
	//-----------------------	Movebarrier ���
	while( !ifs.eof() ){
		getline(ifs,s,',');		
		id=atoi(s.c_str ());
		if(!ifs.eof()){
			for(int i=0;i<6;i++){
				if(i==5)
					getline(ifs,s);
				else
					if(i%2==1)
						getline(ifs,s,',');
					else		
						getline(ifs,s,' ');
				data[i]=atoi(s.c_str ());	//id,x y,x_area y_area,x_speed y_speed
			}
			addMovebarrier(id,data[0],data[1],data[2],data[3],data[4],data[5]);  //�t�mMovebarrier
		}
	}
	ifs.close();
}
void Map::LoadBitmap(){
	string temp[8]={"0","1","2","3","4","5","6","7"};		
	for(int i=0 ; i <= 5 ; i++)
		floor[i].LoadBitmap((char *)("Bitmaps/map/floor_"+temp[i]+".bmp").c_str(),RGB(128,0,255)); //�a�O�ϥ�
	for(int i=0 ; i <= 3 ; i++)
		barrier[i].LoadBitmap((char *)("Bitmaps/map/barrier_"+temp[i]+".bmp").c_str(),RGB(128,0,255));//Movebarrier�ϥ�
	for(int i=0 ; i <= 2 ; i++)
		back_map[i].LoadBitmap("Bitmaps/map/back_0.bmp",RGB(0,0,0));//���d2�I����
	for(int i=3 ; i <= 4 ; i++)
		back_map[i].LoadBitmap("Bitmaps/map/back_1.bmp",RGB(0,0,0));//���d3�I����
	for(int i=5 ; i <= 6 ; i++)
		back_map[i].LoadBitmap("Bitmaps/map/back_2.bmp",RGB(0,0,0));//���d1�I����
	for(int i=7 ; i <= 8 ; i++)
		back_map[i].LoadBitmap("Bitmaps/map/back_3.bmp",RGB(0,0,0));//���d0�I����
	for(int i=0 ; i <= 7 ; i++)
		Rope_map[i].LoadBitmap((char *)("Bitmaps/rope/rope"+temp[i]+".bmp").c_str(),RGB(128,0,255));//Rope�ϥ�
	changeBitmap.LoadBitmap("Bitmaps/menu/changStage.bmp");//����Loading�e��
	for(int i=0;i<3;i++){
		load.AddBitmap((char *)("Bitmaps/menu/loading_"+temp[i]+".bmp").c_str(),RGB(255,0,0));//Loading�ϥ�
		mushroom.AddBitmap((char *)("Bitmaps/menu/mush_"+temp[i]+".bmp").c_str(),RGB(255,0,0));//��ۣ�ϥ�
	}
	goal.LoadBitmap("BitMaps/map/goal.bmp",RGB(0,0,0) );//��F�a�I�ϥ�
}
void Map::changeLoading() {
	//------------------------------------------------------------------
	CDDraw::BltBackColor(DEFAULT_BG_COLOR);		// �N Back Plain ��W�w�]���C��
	//----------------------------------------
	changeBitmap.SetTopLeft(0,0);
	changeBitmap.ShowBitmap();		//����Loading�e��
	load.SetTopLeft(280,270);
	load.OnShow();
	mushroom.SetTopLeft(200,250);
	mushroom.OnShow();
	//----------------------------------------
	CDDraw::BltBackToPrimary();					// �N Back Plain �K��ù�
	//-------------------------------------------------------------------
	load.OnMove();
	mushroom.OnMove();
}
void Map::SetMovebarrier_Bitmap(){
	for(int i=0;i<(int)movebarrier.size();i++){		//�̷�Movebarrier ID Ū��
		if(movebarrier[i]->GetId()==0)
			movebarrier[i]->SetBitmap(&barrier[0]);
		else if(movebarrier[i]->GetId()==1)
			movebarrier[i]->SetBitmap(&barrier[1]);
		else if(movebarrier[i]->GetId()==2)
			movebarrier[i]->SetBitmap(&barrier[2]);
		else if(movebarrier[i]->GetId()==3)
			movebarrier[i]->SetBitmap(&barrier[3]);
	}
}
void Map::SetRope_Bitmap(){
	for(int i=0;i<(int)rope.size();i++){			//�̷�Rope ID Ū��
		if(rope[i]->GetId() == 6)
			rope[i]->SetBitmap(&Rope_map[5]);
		else if(rope[i]->GetId() == 10)
			rope[i]->SetBitmap(&Rope_map[6]);
		else if(rope[i]->GetId() == 11)
			rope[i]->SetBitmap(&Rope_map[7]);
		else if(rope[i]->GetId() == 12)
			rope[i]->SetBitmap(&Rope_map[4]);
	}	
}
void Map::addMovebarrier(int fid,int fx,int fy,int fx_area,int fy_area,int fx_speed,int fy_speed){	//�t�mMovebarrier �Ŷ� (Vector)
	movebarrier.push_back(new MovingBarrier(fid,fx,fy,fx_area,fy_area,fx_speed,fy_speed));
}
void Map::addRope(int r_id,int rx,int ry){	//�t�mRope �Ŷ� (Vector)
	rope.push_back(new Rope(r_id,rx,ry));
}
void Map::onMove(){
	for(int i=0;i < (int)movebarrier.size();i++)	//����Movebarrier�����ʭn��
		movebarrier[i]->onMove();
}
void Map::onShow(){
	if(Mapid_now==0)		
		for(int i=7;i<=8;i++){				//�̷����d�l��ܭI����
			back_map[i].SetTopLeft(-sx/10+(i-7)*800,0);
			back_map[i].ShowBitmap();
		}
	if(Mapid_now==1)
		for(int i=5;i<=5;i++){
			back_map[i].SetTopLeft(-sx/10,-sy/12-35);
			back_map[i].ShowBitmap();
		}
	if(Mapid_now==2)
		for(int i=0;i<=2;i++){
			back_map[i].SetTopLeft(-sx/5+i*640,-sy/5-50);
			back_map[i].ShowBitmap();
		}
	if(Mapid_now==3){
		for(int i=3;i<=4;i++){
			back_map[i].SetTopLeft(-sx/6,-sy/6+(i-3)*1200-150);
			back_map[i].ShowBitmap();
		}
	}
	//-------------------------------------------��ܩҦ��� Movebarrier 
	for(int i=0;i < (int)movebarrier.size();i++)
		movebarrier[i]->onShow(this);
	//-------------------------------------------�̷�floor ID �L�X�a�O��
	for(int j=numB_y-1;j >=0 ; j--)
		for(int i=0;i < numB_x ;i++){		
			int x = i*BarrierW - sx; 
			int y = j*BarrierH - sy;
			switch (map[i][j]){
				case 1:
					floor[1].SetTopLeft(x-31, y-10); 
					floor[1].ShowBitmap();
					break;
				case 2:
					floor[2].SetTopLeft(x-31, y-10); 
					floor[2].ShowBitmap();
					break;
				case 3:
					floor[3].SetTopLeft(x-31, y-10); 
					floor[3].ShowBitmap();
					break;
				case 4:
					floor[4].SetTopLeft(x-31, y-10); 
					floor[4].ShowBitmap();
					break;
				case 5:
					floor[5].SetTopLeft(x-31, y-10); 
					floor[5].ShowBitmap();
					break;
				case 20:
					goal.SetTopLeft(x,y-120);
					break;
			}			
				
		}
	//-------------------------------------------
	if(isGoal)	//�F������ �L�Xhere
		goal.ShowBitmap();
	for(int i=0;i < (int)rope.size();i++) //��ܩҦ�Rope
		rope[i]->onShow(this);
}
bool Map::IsEmpty(int x, int y){		
	int gx = x / BarrierW;			// �ഫ����y��(��ư��k)			
	int gy = y / BarrierH;			// �ഫ����y��(��ư��k)	
	return map[gx][gy] == 0;		// ���]0 �N��Ū�
}
bool Map::IsFloor(int x,int y){
	int gx = x / BarrierW;					
	int gy = y / BarrierH;			
	for(int i=0;i < numB_x;i++){	//�̷�floor ID �P�_���I���L�b�a�O�W	
		if(map[i][gy]==1)
			if( i*BarrierW-31 <= x && x <= i*BarrierW+90)
				return true;
		if(map[i][gy]==3)
			if( i*BarrierW <= x && x <= i*BarrierW+120)
				return true;
		if(map[i][gy]==4)
			if( i*BarrierW <= x && x <= i*BarrierW+150)
				return true;
		if(map[i][gy]==5)
			if( i*BarrierW <= x && x <= i*BarrierW+150)
				return true;
	}
	if(map[gx][gy] == 2)
		return true;
	else
		return false;
}
bool Map::IsWall(int x,int y){
	int gx = x / BarrierW;					
	int gy = y / BarrierH;			
	for(int i=0;i < numB_x;i++)			//�̷�floor ID �P�_���I�O�_�Ofloor(Wall)	
		for(int j=0;j < numB_y;j++){
			if(map[i][j]==1)
				if( i*BarrierW-31 < x && x < i*BarrierW+90)
					if(j*BarrierH < y && y < j*BarrierH+floor[1].Height()-10)
						return true;
			if(map[i][j]==3)
				if( i*BarrierW < x && x < i*BarrierW+120)
					if(j*BarrierH < y && y < j*BarrierH+floor[3].Height()-10)
						return true;
			if(map[i][j]==4)
				if( i*BarrierW < x && x < i*BarrierW+150)
					if(j*BarrierH < y && y < j*BarrierH+floor[4].Height()-10)
						return true;
			if(map[i][j]==5)
				if( i*BarrierW < x && x < i*BarrierW+150)
					if(j*BarrierH < y && y < j*BarrierH+floor[5].Height()-10)	
						return true;
		}
	if(map[gx][gy] == 2)
		return true;
	else
		return false;
}
bool Map::IsNoland(int x,int y){
	int count;
	for(count=0; y+count<=GetMapH() ;count++){	
		if( !IsFloor(x,y+count) && !IsMoveBarrier(x,x,y+count) ){}//�P�_���I�U�����Lfloor�BMovebarrier�i��
		else break;
	}
	if( y+count >= GetMapH())
		return true;
	else
		return false;
}
bool Map::IsMoveBarrier(int x1,int x2,int y){

	for(int i=0;i<(int)movebarrier.size();i++)	//�Q�ΰj��P�_x1,x2,y���L�b�Ҧ�Movebarrier�����䤤�@�ӤW
		if(x1>=movebarrier[i]->GetX1() && x2<=movebarrier[i]->GetX2())
			if(y==movebarrier[i]->GetY())
				return true;
	
/*	for(int i=0;i<(int)movebarrier.size();i++)			//���q�Ω�ץ�(x�y�нd��Ӥj��)�A�bMovebarrier��Movebarrier�������ҽk�a�a
		for(int j=0;j<(int)movebarrier.size();j++)		//���[�W�o�q��A�|�y���C�������Z(�j��Ӧh�h!)�A�G�H�]�p�a�ϮɡA�קK�����p
			if( movebarrier[i]->HitRectangle(movebarrier[j]->GetX1(),movebarrier[j]->GetX2(),movebarrier[j]->GetY()) ){
				int b_X1,b_X2;

				if(movebarrier[i]->GetX1() <= movebarrier[j]->GetX1())		b_X1=movebarrier[i]->GetX1();
				else	b_X1=movebarrier[j]->GetX1();

				if(movebarrier[i]->GetX2() >= movebarrier[j]->GetX2())		b_X2=movebarrier[i]->GetX2();
				else	 b_X2=movebarrier[j]->GetX2();
				
				if(x1>=b_X1 && x2<=b_X2)
					if(y==movebarrier[i]->GetY())
						return true;
			}
*/	return false;
}
bool Map::IsReach(int x,int y){
	int gx = x / BarrierW;					//���I�O�_��L���a�I
	int gy = y / BarrierH;
	if(map[gx][gy]==20 || map[gx][gy-1]==20 || map[gx][gy+1]==20)
		return true;
	else 
		return false;
}
int Map::ScreenX(int x){			// x ���a�Ϫ��I�y��
	return x - sx;				// �^�ǿù���x �I�y��
}
int Map::ScreenY(int y) {			// y ���a�Ϫ�y �I�y��
	return y - sy;				// �^�ǿù����I�y��
}
int Map::GetMapId(){ return Mapid_now; }
int Map::GetMapTotal(){return MapTotal;}
int Map::GetSX(){return sx;}
int Map::GetSY(){return sy;}
int Map::GetMapW(){return MapW;}
int Map::GetMapH(){return MapH;}
int Map::GetPixW(){return PixW;}
int Map::GetPixH(){return PixH;}
int Map::GetWinEnemy_number(){return win_Enemy_number;}
int* Map::GetEnemy_id(){ return enemy_id; }
int* Map::GetEnemy_pos_x(){return enemy_pos_x;}
int* Map::GetEnemy_pos_y(){return enemy_pos_y;}
void Map::GetInit_h_xy(int *Hx,int *Hy){ *Hx=init_hx,*Hy=init_hy; }
bool Map::GetIsWin(){ return isWin; }
bool Map::GetIsGoal(){ return isGoal; }
void Map::SetSX(int x){	sx+=x;}
void Map::SetSY(int y){	sy+=y;}
void Map::SetMapId(int id){ Mapid_now = id;}
void Map::SetIsWin(bool flag){ isWin=flag;}
void Map::SetIsGoal(bool flag){ isGoal=flag;}
vector<MovingBarrier *>Map::GetMovingBarrier(){return movebarrier;}
vector<Rope *>Map::GetRope(){ return rope;}
//-----------------------------------------------------------------------------------------
MovingBarrier::MovingBarrier(int fid,int fx,int fy,int fx_area,int fy_area,int fx_speed,int fy_speed){ //f_speed: �ɮת�x,y�ײv�p��or�t�� 
	id = fid;
	x = fix_x = fx;
	y = fix_y = fy;
	x_area = fx_area;
	y_area = fy_area;
	x_speed = fx_speed;
	y_speed = fy_speed;
	is_end[0] =is_end[1] = true;

	if(id==0)	dx=110;
	if(id==3)	dx=154+5;
	if(id==1 || id==2)	dx=60+5;
}
void MovingBarrier::SetBitmap(CMovingBitmap *Bitmap){
	barrier = Bitmap;		//Ū��
}
void MovingBarrier::onShow(Map *m){	
	int dy=0;	//�̷�MovingBarrier ID �L��y�y��
	if(id==0) dy=7;
	else if(id==3) dy=10;
	else if(id==1 || id==2) dy=2;
	barrier->SetTopLeft(m->ScreenX(x),m->ScreenY(y-dy));
	barrier->ShowBitmap ();
}
void MovingBarrier::onMove (){	//Movebarrier�����ʭn��
	if(GetFx_area()>0){
		if(x<this->GetFx()+this->GetFx_area() && is_end[0]){ //���ɮy�� < �}�l�y��+�d��(��)
			x+=x_speed;
			if(x>=this->GetFx()+this->GetFx_area()) is_end[0]=false;	//�W�L ����
		}
		else if(x>this->GetFx() && !is_end[0]){		//���ɮy�� > �}�l�y�� && ����
			x-=x_speed;
			if(x<=this->GetFx()) is_end[0]=true;	//�C�� ����
		}
	}
	else{
		if(x>this->GetFx()+this->GetFx_area() && is_end[0]){ //����W�z�ʧ@�ۦP(���Ƹ����P�Ӥw)
			x+=x_speed;
			if(x<=this->GetFx()+this->GetFx_area()) is_end[0]=false;
		}
		else if(x<this->GetFx() && !is_end[0]){
			x-=x_speed;
			if(x>=this->GetFx()) is_end[0]=true;
		}	
	}
	x_change = x - last_x;	//�Ω�H�����ê������
	last_x=x;
	//-------------------
	if(GetFy_area()>0){
		if(y<this->GetFy()+this->GetFy_area() && is_end[1]){	//�P�W
			y+=y_speed;
			if(y>=this->GetFy()+this->GetFy_area()) is_end[1]=false;
		}
		else if(y>this->GetFy() && !is_end[1]){
			y-=y_speed;
			if(y<=this->GetFy()) is_end[1]=true;
		}
	}
	else{
		if(y>this->GetFy()+this->GetFy_area() && is_end[1]){
			y+=y_speed;
			if(y<=this->GetFy()+this->GetFy_area()) is_end[1]=false;
		}
		else if(y<this->GetFy() && !is_end[1]){
			y-=y_speed;
			if(y>=this->GetFy()) is_end[1]=true;
		}
	}
	y_change = y - last_y;
	last_y=y;
}
int MovingBarrier::GetX1(){return x;}
int MovingBarrier::GetX2(){return x+dx;}
int MovingBarrier::GetY(){return y;}
int MovingBarrier::GetId(){return id;}
int MovingBarrier::GetXchange(){return x_change;}
int MovingBarrier::GetYchange(){return y_change;}
void MovingBarrier::SetX(int mx){
	x = mx;
	fix_x = mx;
}
void MovingBarrier::SetY(int my){
	y = my;
	fix_y = my;
}
const int MovingBarrier::GetFx(){return fix_x;}//���o�T�w��x
const int MovingBarrier::GetFy(){return fix_y;}//���o�T�w��y
const int  MovingBarrier::GetFx_area(){return x_area;} //���o�T�w��xoffest
const int  MovingBarrier::GetFy_area(){return y_area;} //���o�T�w��yoffest
bool MovingBarrier::HitRectangle(int tx1,int tx2, int ty){
	int x1 = GetX1();	// �� x�y��
	int x2 = GetX2();	// �k x�y��
	int y = GetY();			
	return (tx2 >= x1 && tx1 <= x2 && ty == y );	// �˴��y���x�λP�ѼƯx�άO�_���涰
}
//###################################################################################################################################
Rope::Rope(int id,int rx,int ry){
	r_id =id;
	x=rx+18;
	y=ry-30+1;
}
void Rope::SetBitmap(CMovingBitmap *Bitmap){	//�P�W
	bitmap=Bitmap;
}
void Rope::onShow(Map *m){
	bitmap->SetTopLeft(m->ScreenX(x),m->ScreenY(y));
	bitmap->ShowBitmap();
}
bool Rope::isClimb(Chero *h){	//�D���O�_���I��÷�l
	return HitRectangle(h->GetClimb_x1(),h->GetClimb_y1(),h->GetClimb_x2(),h->GetY2()-30);
}
int Rope::GetId(){return r_id;}
int Rope::GetX1(){return x;}
int Rope::GetX2(){return x + bitmap->Width();}
int Rope::GetY1(){return y;}
int Rope::GetY2(){return y + bitmap->Height();}
bool Rope::HitRectangle(int tx1, int ty1, int tx2, int ty2){	// �O�_�I��Ѽƽd�򪺯x��
	int x1 = GetX1();				// �y�����W��x�y��
	int y1 = GetY1();				// �y�����W��y�y��
	int x2 = GetX2();	// �y���k�U��x�y��
	int y2 = GetY2();	// �y���k�U��y�y��
	return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);	// �˴��y���x�λP�ѼƯx�άO�_���涰
}
//###################################################################################################################################
//�C���D��
Chero::Chero(){
	climb_x1=50,climb_x2=75;	//÷�l �P�_�d��
	left_x=40,right_x=90;		//�H������A�H���k�� �y��
	height=84;
	HP_total=HP=100;
	MP_total=200;
	MP=150;
	life=2;
	addSkill();
}
Chero::~Chero(){
	//---------------------------------�R���H���ޯ�
	for(int i=0;i<(int)hero_skill.size();i++)
		delete hero_skill[i];
	hero_skill.clear();
	//---------------------------------
}
void Chero::addSkill(){
	hero_skill.push_back (new Skill_son01);//�t�m skill �Ŷ� (vector)
	hero_skill.push_back (new Skill_son02);
	hero_skill.push_back (new Skill_son03);
	hero_skill.push_back (new Skill_son04);
	hero_skill.push_back (new Skill_son05);
}
void Chero::init(Map *m){
	const int INITIAL_VELOCITY = 14;	// ��l�W�ɳt��
	const int INITIAL_DOWNVELOCITY =1;	// ��l�U���t��
	Velocity = init_Velocity = INITIAL_VELOCITY;
	downVelocity = init_downVelocity = INITIAL_DOWNVELOCITY;
	countBeHit = superTime = 30;	//��   (X ��30) ��	 �L�Įɶ�:���ˮɦ��u�Ȫ��ɶ��w�K�@���Q�Ǫ���
	countHit = attackTime = 10;	//�w���ɶ���(X ��30) ��
	//--------------------------
	isMovingLeft = isMovingRight= isMovingUp = isMovingDown = false;
	isMovingAttack = isMovingSkill_A = isMovingSkill_S = isMovingSkill_D = isMovingClimb = isMovingJump= false;
	hit[0]=hit[1]=hit[2]=false;
	isLeft=beHit=isSkill=isClimb=false;		//isLeft:���ʮɦp�G���U�Ӫ��ɭԭn���V���ʮɪ���V �p:���S���U�ӮɧY���V�k
	is_alive = true;
	//--------------------------	
	HP=100,MP=150;
	//�������Ϥ��ɶ��Y��
	for(int i=0;i< 2;i++){			//i=0���k,i=1����
		Attack1[i].SetDelayCount(4);//�q��1 
		Attack2[i].SetDelayCount(4);//�q��2
		Attack3[i].SetDelayCount(4);//�q��3
	}
	//------------------
	Walk[0].SetDelayCount(6);
	Walk[1].SetDelayCount(6);
	climb_Rope.SetDelayCount(8);
	//-------------------------���H���@�}�l�N���b�a�W
	for(int i=0 ; GetY2()+i <(m->GetMapH()) ; i++)
		if( m->IsFloor(GetX1(),GetY2()+i) || m->IsMoveBarrier(GetX1(),GetX2(),GetY2()+i) ){
			SetY2(GetY2()+i);
			break;
		}
	y-=100;
	//-------------------------
}	
void Chero::jump(Map *m){
	if (Velocity > 0)
		y -= (Velocity)--;		// y�b�W��(����velocity���I)//�����O�v�T�A�U�����W�ɳt�׭��C
	else
		onfloor(m);
	for(;GetY2() < m->GetSY()+(m->GetPixH()/2)-20;)		//-20���ù����վ�,��ù��W�L�@�b��,�קK�����ê�����,���|���ù��_�a(���U)���Pı 		
		m->SetSY(-1);					//�j��Ӧ���ù�������
}
void Chero::climb(Map *m,bool *Climb_down){
	int i;
	const int STEP_SIZE=4;	//��÷�t��
	vector<Rope *> rope=m->GetRope();
	for(i=0;i<(int)rope.size();i++){		//�P�_������÷�l
		if( rope[i]->isClimb(this) && isClimb ){
			SetX( rope[i]->GetX1()-53-x );	//�]x�y��(����÷�l)
			break;
		}
	}
	if(i!=(int)rope.size()){		//������÷�l
		if(isMovingUp && (GetClimb_y1()-STEP_SIZE)>rope[i]->GetY1())
			y-=STEP_SIZE;
		if(isMovingDown)
			y+=STEP_SIZE;
		if(isMovingUp || isMovingDown)
			climb_Rope.OnMove();
		if( GetY2() > rope[i]->GetY2()+20 )
			*Climb_down=true;
	}
	//----------------------------------------------------------------
	for(;GetY2() > m->GetSY()+(m->GetPixH()/2) && GetY2() < m->GetMapH()-(m->GetPixH()/2)+70;)//+70 ���D���ȹ���m�ܩ�������I
		m->SetSY(1);
	for(;GetY2() < m->GetSY()+(m->GetPixH()/2)-20;)
		m->SetSY(-1);
}
void Chero::onfloor(Map *m){
	const int foot1 = GetX1()+20;	//60
	const int foot2 = GetX2()-20;	//70
	vector<MovingBarrier *>barrier=m->GetMovingBarrier();
	Velocity = 0;	//�b�U�����q�����
	int count_floor=0;	//��l�����a����
	int number;//�ΨӰO���b����movebarrier�W
	//------------------------------------------------------------------------�U��
	if( isMovingDown && IsTouchBarrier(barrier,count_floor,&number))
		y+=1;					//��bMovearrier�W && �����U
	SetMovingDown(false);
	//------------------------------------------------------------------------�p�����a����
	for(count_floor=0 ; GetY2()+count_floor<=m->GetMapH()+100 ; count_floor++)	
		if( !m->IsFloor( foot1,GetY2()+count_floor) && !m->IsFloor( foot2,GetY2()+count_floor) && !IsTouchBarrier(barrier,count_floor,&number) ){}
		else break;
	//------------------------------------------------------------------------
	if (count_floor >= downVelocity)
		y += (downVelocity)++;		// y�b�U��(����velocity���I)// �����O�v�T�A�U�����U���t�׼W�[
	else{
		y +=count_floor;	//�@����n��a
		SetMovingJump(false);
		Velocity = init_Velocity;		// ���]�W�ɪ�l�t��
		downVelocity = init_downVelocity;	// ���]�U����l�t��	
	}
	//-----------------------------------------------------------------------��ۻ�ê������
	int dx,dy,adjust=0;
	if( IsTouchBarrier(barrier,0,&number) ){//number:���b���ӻ�ê���W
		dx= barrier[number]->GetXchange();
		dy= barrier[number]->GetYchange();
		if(dy>=0)	adjust=0;	//Movebarrier�W��
		else	adjust=dy;	//Movebarrier�U��
	}
	if( IsTouchBarrier(barrier,adjust,&number) ){
		x+=dx;
		y+=dy;
	}
	//------------------------------------------------------------------------�ȹ�����
	for(;GetY2() > m->GetSY()+(m->GetPixH()/2) && GetY2() < m->GetMapH()-(m->GetPixH()/2)+70;)//+70 ���D���ȹ���m�ܩ�������I
		m->SetSY(1);
	for(;GetY2() < m->GetSY()+(m->GetPixH()/2)-20;)
		m->SetSY(-1);
}
bool Chero::IsTouchBarrier(vector<MovingBarrier *> b,int count,int *number){
	const int foot1 = GetX1()+20;	
	const int foot2 = GetX2()-20;	
	for(int i=0;i<(int)b.size();i++){ //��Ҧ�Movebarrier�P�_
		if( foot1 >= b[i]->GetX1() && foot2 <= b[i]->GetX2()){	//�bx�d�� 
			if( GetY2()+count == b[i]->GetY() ){	//y���|�ܪ��ɭ�(���k)
				*number=i;
				return true;
			}
			if( GetY2()+count+b[i]->GetYchange() == b[i]->GetY() ){ //y�|���ܪ��ɭ�(�W�U)
				*number=i;
				return true;
			}
		}
	}
	return false;
}
void Chero::LoadBitmap(){
	string temp[5]={"0","1","2","3","4"};
	string rl[2]={"r","l"};
	for(int j=0 ; j <= 1 ; j++){
		Jump[j].LoadBitmap( (char *)("Bitmaps/hero/jump_"+rl[j]+".bmp").c_str(),RGB(128,0,255) );
		for(int i=0 ; i <= 4 ; i++){
			if(i<4){
				Alert[j].AddBitmap( (char *)("Bitmaps/hero/alert_"+rl[j]+temp[i]+".bmp").c_str(),RGB(128,0,255) );	//ĵ��
				Walk[j].AddBitmap( (char *)("Bitmaps/hero/walk_"+rl[j]+temp[i]+".bmp").c_str(),RGB(128,0,255) );	//����
				Attack1[j].AddBitmap( (char *)("Bitmaps/hero/hit_"+rl[j]+"1"+temp[i]+".bmp").c_str(), RGB(128,0,255));	//�Ĥ@�q����
				Attack2[j].AddBitmap( (char *)("Bitmaps/hero/hit_"+rl[j]+"2"+temp[i]+".bmp").c_str(), RGB(128,0,255));	//�ĤG�q����
				Attack3[j].AddBitmap( (char *)("Bitmaps/hero/hit_"+rl[j]+"3"+temp[i]+".bmp").c_str(), RGB(128,0,255));	//�ĤT�q����
			}
			else{
				Alert[j].AddBitmap( (char *)("Bitmaps/hero/alert_"+rl[j]+temp[i]+".bmp").c_str(),RGB(128,0,255) );	//ĵ��
				Attack3[j].AddBitmap( (char *)("Bitmaps/hero/hit_"+rl[j]+"3"+temp[i]+".bmp").c_str(), RGB(128,0,255));	//�ĤT�q����
			}
		}
	}
	climb_Rope.AddBitmap("Bitmaps/hero/Rope_0.bmp",RGB(128,0,255));//��÷
	climb_Rope.AddBitmap("Bitmaps/hero/Rope_1.bmp",RGB(128,0,255));
	lifeBitmap.LoadBitmap("Bitmaps/hero/life/heart.bmp",RGB(0,0,255));	//�R��
	life_0.LoadBitmap("Bitmaps/hero/life/0.bmp",RGB(0,0,255));	//�Ʀr0
	life_1.LoadBitmap("Bitmaps/hero/life/1.bmp",RGB(0,0,255));	//�Ʀr1
	life_2.LoadBitmap("Bitmaps/hero/life/2.bmp",RGB(0,0,255));	//�Ʀr2
	for(int i=0; i < (int)hero_skill.size() ;i++)//�ޯ�Ū��
		hero_skill[i]->LoadBitmap();
}
void Chero::LoadAudio(){//Ū����
	CAudio::Instance()->Load(AUDIO_Jump,  "sounds/Jump.wav");	
	CAudio::Instance()->Load(AUDIO_hit,  "sounds/hit.wav");
	CAudio::Instance()->Load(AUDIO_skill,  "sounds/skill.wav");
	CAudio::Instance()->Load(AUDIO_bomb,  "sounds/bomb.wav");
	CAudio::Instance()->Load(AUDIO_die,  "sounds/die.mid");
}
void Chero::AttackEnemy(Map *m){ //�����ĤH
//    //--------------------------------------------------------------	
	if(Attack1[0].IsFinalBitmap())			//�����ʧ@�����A�~��A��
		Attack1[0].Reset();
	else if(Attack1[1].IsFinalBitmap())
		Attack1[1].Reset();
	if(Attack2[0].IsFinalBitmap())
		Attack2[0].Reset();
	else if(Attack2[1].IsFinalBitmap())
		Attack2[1].Reset();
	if(Attack3[0].IsFinalBitmap())
		Attack3[0].Reset();
	else if(Attack3[1].IsFinalBitmap())
		Attack3[1].Reset();
//    //--------------------------------------------------------------�P�qfalse (������)
	if( Attack1[0].IsFinalBitmap()||Attack1[1].IsFinalBitmap() ) 
		hit[0]=false;
	if( Attack2[0].IsFinalBitmap()||Attack2[1].IsFinalBitmap() ) 
		hit[1]=false;
	if( Attack3[0].IsFinalBitmap()||Attack3[1].IsFinalBitmap() ) 
		hit[2]=false;	
//    //--------------------------------------------------------------�ޯ�ɡA�q�Ƨ������k�s
	if( IsSkill() ){
		for(int i=0;i<2;i++){
			Attack1[i].Reset();
			Attack2[i].Reset();
			Attack3[i].Reset();
			hit[i]=false;
		}
		hit[2]=false;//0,1,2 �T�ۭ��](�[�W��)
		countHit = attackTime;
		SetMovingAttack(false);
	}
//    //--------------------------------------------------------------�����w���ɶ�
	if(countHit < attackTime && hit[2]==false){
		countHit--;
		SetMovingAttack(false);
	}
	if(countHit==0)
		countHit = attackTime;
//    //--------------------------------------------------------------�T�s�q�����P�_
	if(isSkill){}
	else if( isMovingAttack && hit[0]==false && hit[1]==false && hit[2]==false){
		hit[0]=true;
		SetMovingAttack(false);			
	}
	else if(hit[0]){	//�����P�_,�������A�Ϥ��~��
		Attack1[0].OnMove();
		Attack1[1].OnMove();
		
		if(isMovingAttack){
			hit[1]=true;
		}
		SetMovingAttack(false);
	}
	else if(hit[1]){	//�����P�_,�������A�Ϥ��~��
		Attack2[0].OnMove();
		Attack2[1].OnMove();
		
		if(isMovingAttack){
			countHit--;
			hit[2]=true;
		}
		SetMovingAttack(false);
	}
	else if(hit[2]){	//�����P�_,�������A�Ϥ��~��
		Attack3[0].OnMove();
		Attack3[1].OnMove();
	}
//    //--------------------------------------------------------------�ޯ঩ MP
	int number;
	vector<MovingBarrier *>barrier=m->GetMovingBarrier();
	if(!isSkill){
		if(isMovingSkill_A && !hero_skill[3]->IsRun() && MP>=40){//�ΨӧP�_�O�_��ޯ�
			hero_skill[3]->SetRun(true);
			MP-=40;
			if(MP<=0) MP=1;
		}
		if(m->IsFloor(x+75,GetY2()) || IsTouchBarrier(barrier,0,&number))
			if(isMovingSkill_S && !hero_skill[4]->IsRun() && MP>=50){
				hero_skill[4]->SetRun(true);
				MP-=50;
				if(MP<=0) MP=1;
			}
	}
	SetMovingSkill_A(false);
	SetMovingSkill_S(false);
}
void Chero::onMove(Map *m,vector<Enemy_basic *> enemy){ // �`�N�G�o�Ohero�A���Omap class
	const int STEP_SIZE = 7;
	const int center = ( GetX1()+GetX2() )/2;
	static int temp=0;
	Alert[0].OnMove();
	Alert[1].OnMove();
	if( IsAlive() ){
		Skill_now();
//�Q����//--------------------------------------------------------------
		for(int i=0;i<(int)enemy.size();i++)	//�ΰj���Ū�P�@�ت���̨C�@�ӼĤH���O�����m +i���V�U�@�O�ӳs���m
			if( enemy[i]->IsAlive() && !isSkill)
				beHit_by_Enemy(enemy[i]);//�Q����function
		if(beHit)
			countBeHit--;
		if(countBeHit==0 && HP>0){
			beHit=false;	//��ܵL�Įɶ��w����
			countBeHit=superTime;	//�^���l��
		}
		//----------------------------------------------���`����
		if( GetY2()>=m->GetMapH()+100 ) HP=0;
		if(HP<=0 ) 
			SetIsAlive(false);	//���`
		//----------------------------------------------���`����
		static bool flag_die=false;
		if( !is_alive && !flag_die){
			CAudio::Instance()->Play(AUDIO_die, false);
			flag_die=true;
		}
		else if (is_alive )
			flag_die=false;
// ���� //--------------------------------------------------------------
		AttackEnemy(m);//����function
		//��MP-------------------------------------------
		for(int j=0;j<(int)enemy.size();j++)
			for(int i=0;i<(int)hero_skill.size();i++)
				if(enemy[j]->IsbeHit() && enemy[j]->IsAlive() && !IsSkill())
					if(MP<=MP_total){
						temp+=1;
						MP+=temp/5;
						if(temp >5) temp=0;
					}
// ���� //--------------------------------------------------------------
		last_x = x;			//���W�@�y�СA�P�_�y��(���k)�K��
		//�k����
		if( (hit[0] || hit[1] || hit[2] || isSkill) && isMovingJump==false && m->IsEmpty(GetX2(),GetY2()-1) ){}	//�a�������ɡA���i���k����	���|�X�{(�����Ϥ����A�ư�)			
		else if (isMovingRight && GetX2()< m->GetMapW() && !m->IsWall( GetX2(),GetY2()-1) ){		//���� (�k��S���)
			Walk[0].OnMove();
			x += STEP_SIZE;
		}
		//������
		if( (hit[0] || hit[1] || hit[2] || isSkill) && isMovingJump==false){}	//�a�������ɡA���i���k����	���|�X�{(�����Ϥ����A�ư�)			
		else if (isMovingLeft && GetX1() > 0 && !m->IsWall( GetX1(),GetY2()-1) ){		//���� (����S���)
			Walk[1].OnMove();
			x -= STEP_SIZE;
		}
// ��÷ //--------------------------------------------------------------
		last_y=y;		//���W�@�y�СA�P�_�y��(��)�K��
		if(isMovingClimb && !isClimb){
			isClimb=true;	//�����W&&�쥻�S��
			SetMovingClimb(false); //����]�^ ��l
			SetMovingJump(false);
		}
		bool Climb_down=false;
		if(isClimb){
			climb(m,&Climb_down);
			if(isMovingJump || Climb_down){
				isClimb=false;
				Velocity = init_Velocity;
				downVelocity = init_downVelocity;
			} 
		}
// ��ģ //--------------------------------------------------------------
		if (isMovingJump)				//��ģ
			jump(m);
		else if( !isClimb )					//��a
			onfloor(m);
		
		if(Velocity == init_Velocity-1)
			CAudio::Instance()->Play(AUDIO_Jump, false);
// �ۥ� // -------------------------------------------------------------
		for(int i=0;i < (int)hero_skill.size();i++)
			hero_skill[i]->onMove(this,m);
// �վ� // -------------------------------------------------------------
// �ȹ� //
		for(; center > m->GetSX()+(m->GetPixW()/2) && center < m->GetMapW()-(m->GetPixW()/2);)	//�ù���۽վ�	
			m->SetSX(1);
		for(; center < m->GetSX()+(m->GetPixW()/2) && center > (m->GetPixW()/2);)		
			m->SetSX(-1);
	}	
}
void Chero::onShow(Map *m) // �`�N�G�o�Ohero�A���Omap class
{
	int number=0;
// ��q //--------------------------------------------------------------
	int percentHP=(HP*100)/HP_total,percentMP=(MP*100)/MP_total;
	ShowHP(percentHP,m);
	ShowMP(percentMP,m);
	lifeBitmap.SetTopLeft(224,48);
	life_0.SetTopLeft(291,48),life_1.SetTopLeft(291,48),life_2.SetTopLeft(291,48);
	lifeBitmap.ShowBitmap();
	if(IsAlive())
		if(life==0)
			life_0.ShowBitmap();
		else if(life==1)
			life_1.ShowBitmap();
		else if(life==2)
			life_2.ShowBitmap();
// ���� //--------------------------------------------------------------
	if( !IsAlive()){} //�����A�H�����L�X
// �Q�� //--------------------------------------------------------------
	else if(beHit && countBeHit%4 == 0){}//countBeHit %4 ==0 --->%4(�{�{�t�סA�V�j�V�C) countBeHit����ɨ���Ͼl�Ƭ�0���Ȩæb���Ӯɭ԰{�{�Ϥ��@��	
// �ޯ� //--------------------------------------------------------------
	else if(hero_skill[3]->IsRun()){}
	else if(hero_skill[4]->IsRun()){}
// ���� //--------------------------------------------------------------
	else if(hit[0]){
		if(isLeft){
			Attack1[1].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
			Attack1[1].OnShow();
		}
		else{
			Attack1[0].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
			Attack1[0].OnShow();
		}
	}
	else if(hit[1]){
		if(isLeft){
			Attack2[1].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
			Attack2[1].OnShow();
		}
		else{
			Attack2[0].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
			Attack2[0].OnShow();
		}
	}
	else if(hit[2]){
		if(isLeft){
			Attack3[1].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
			Attack3[1].OnShow();
		}
		else{
			Attack3[0].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
			Attack3[0].OnShow();
		}
	}
// ��÷ //--------------------------------------------------------------
	else if(isClimb){
		climb_Rope.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
		climb_Rope.OnShow();
	}
// ��ģ //--------------------------------------------------------------
	else if(!(IsTouchBarrier(m->GetMovingBarrier(),0,&number)) && !isLeft && last_y!=y && x>=last_x){	//�P�ɫ����M�����b�k��(���b����)
		Jump[0].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
		Jump[0].ShowBitmap();
	}
	else if(!(IsTouchBarrier(m->GetMovingBarrier(),0,&number)) &&  isLeft && last_y!=y && x<=last_x){	//�O�_���V����P�ɧP�_���L����
		Jump[1].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
		Jump[1].ShowBitmap();
	}
// ���� //--------------------------------------------------------------
	else if(x>last_x && isMovingRight){											//�b�k�䲾��
		Walk[0].SetTopLeft(m->ScreenX(x), m->ScreenY(y)); 
		Walk[0].OnShow();
		isLeft=false;
	}
	else if(x<last_x && isMovingLeft){											//�b���䲾��
		Walk[1].SetTopLeft(m->ScreenX(x), m->ScreenY(y)); 
		Walk[1].OnShow();
		isLeft=true;
	}
// ��a //--------------------------------------------------------------
	else if(!isLeft){								//���V�k�� �y�ШS����
		Alert[0].SetTopLeft(m->ScreenX(x), m->ScreenY(y)); 
		Alert[0].OnShow();
	}
	else if( isLeft){								//���V���� �y�ШS����
		Alert[1].SetTopLeft(m->ScreenX(x), m->ScreenY(y)); 
		Alert[1].OnShow();
	}
// �ޯ� //--------------------------------------------------------------
	for(int i=0;i<(int)hero_skill.size();i++)
		hero_skill[i]->onShow(this,m);
}
void Chero::SetMovingDown(bool flag){isMovingDown = flag;}
void Chero::SetMovingLeft(bool flag){isMovingLeft = flag;}
void Chero::SetMovingRight(bool flag){isMovingRight = flag;}
void Chero::SetMovingUp(bool flag){isMovingUp = flag;}
void Chero::SetMovingAttack(bool flag){isMovingAttack = flag;}
void Chero::SetMovingSkill_A(bool flag){isMovingSkill_A = flag;}
void Chero::SetMovingSkill_S(bool flag){isMovingSkill_S = flag;}
void Chero::SetMovingSkill_D(bool flag){isMovingSkill_D = flag;}
void Chero::SetMovingClimb(bool flag){isMovingClimb = flag;}
void Chero::SetMovingJump(bool flag){isMovingJump = flag;}
bool Chero::IsLeft(){return isLeft;}
bool Chero::IsClimb(){ return isClimb;}
int Chero::IsAttack(){
	if(hit[0])
		return 1;
	else if(hit[1])
		return 2;
	else if(hit[2])
		return 3;
	return 0;
}
void Chero::Skill_now(){
	if(hero_skill[3]->IsRun()||hero_skill[4]->IsRun())
		isSkill=true;
	else
		isSkill=false;
}
bool Chero::IsAlive(){return is_alive;}
bool Chero::IsSkill(){return isSkill;}
void Chero::SetIsAlive(bool flag){is_alive = flag;}
int Chero::GetX1(){return x+left_x;}
int Chero::GetY1(){return y + 20;}
int Chero::GetX2(){return x + right_x;}
int Chero::GetY2(){return y + height;}
int Chero::GetMP(){return MP;}
bool Chero::addMP(int num){ 
	if(MP <= MP_total)
		MP+=num;
	if( MP >= MP_total){
		MP=MP_total;
		return true;
	}
	else 
		return false;
}
int Chero::GetLife(){return life;}
void Chero::SetLife(int num){life=num;}
int Chero::GetClimb_x1(){ return x+climb_x1;}
int Chero::GetClimb_x2(){ return x+climb_x2;}
int Chero::GetClimb_y1(){ return y+15;}
bool Chero::GetIsMovingUp(){ return isMovingUp;}
void Chero::SetX(int move){ x+=move; }
void Chero::SetX1(int sx){ x=sx-left_x; }
void Chero::SetY2(int sy){ y=sy-height; }
vector<Skill_basic *> Chero::GetSkill(){return hero_skill;}
CAnimation Chero::GetHitAnimation(){
//------------------------------------�Ĥ@�q����	
	if(!isLeft && hit[0])
		return Attack1[0];
	else if(hit[0])
		return Attack1[1];
//------------------------------------�ĤG�q����	
	else if(!isLeft && hit[1])
		return Attack2[0];
	else if(hit[1])
		return Attack2[1];
//------------------------------------�ĤT�q����
	else if(!isLeft && hit[2])
		return Attack3[0];
	else 
		return Attack3[1];	
}
void Chero::beHit_by_Enemy(Enemy_basic *e){
	bool condition=HitRectangle(e->GetX1(), e->GetY1(),e->GetX2(), e->GetY2());	//�I���
	int hit_HP=0;
	if(countBeHit == superTime && !beHit){ //�̷�Enemy ID �P�_�D������
		if(condition && e->GetEnemyId()!=4)
			hit_HP=-1;
		else if(e->isHitHero(this) && e->GetEnemyId()==2)
			hit_HP=-5;
		else if(e->isHitHero(this) && e->GetEnemyId()==3)
			hit_HP=-5;
		else if(condition && e->GetEnemyId()==4)
			hit_HP=-10;
		else if(e->isHitHero(this) && e->GetEnemyId()==5)
			hit_HP=-20;
		//----------------���Q���� hit_HP!=0
		if(hit_HP!=0){
			HP += hit_HP;
			beHit=true;
			countBeHit--;
		}
	}
}
void Chero::ShowHP(int percent,Map *m){ //��ܦ��
	if (!SHOW_LOAD_PROGRESS)
		return;
	const int bar_width = 300;		
	const int bar_height = 16;		
	const int pen_width = 1;
	const int x1 = 5;
	const int y1 = 10; 
	const int x2 = x1 + bar_width;
	const int y2 = y1 + bar_height;
	const int progress_x1 = x1 + pen_width;
	const int progress_x2 = progress_x1 + percent * (bar_width-2*pen_width) / 100;
	const int progress_x2_end = x2 - pen_width;
	const int progress_y1 = y1 + pen_width;
	const int progress_y2 = y2 - pen_width;
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CPen *pp, p(PS_NULL, 0, RGB(0,0,0));		// �M��pen
	pp = pDC->SelectObject(&p);
	CBrush *pb, b(RGB(110,100,80));				// �e progress��
	pb = pDC->SelectObject(&b);
	pDC->Rectangle(x1,y1,x2,y2);				
	CBrush b1(RGB(215,197,183) );				// �e progrss����
	pDC->SelectObject(&b1);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2_end,progress_y2);
	CBrush b2(RGB(245,45,45));					// �e progrss�i��
	pDC->SelectObject(&b2);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2,progress_y2);
	pDC->SelectObject(pp);						// ���� pen
	pDC->SelectObject(pb);						// ���� brush
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}
void Chero::ShowMP(int percent,Map *m){ //���MP��
	if (!SHOW_LOAD_PROGRESS)
		return;
	const int bar_width = 300;		
	const int bar_height = 16;		
	const int pen_width = 1;
	const int x1 = 5;
	const int y1 = 30; 
	const int x2 = x1 + bar_width;
	const int y2 = y1 + bar_height;
	const int progress_x1 = x1 + pen_width;
	const int progress_x2 = progress_x1 + percent * (bar_width-2*pen_width) / 100;
	const int progress_x2_end = x2 - pen_width;
	const int progress_y1 = y1 + pen_width;
	const int progress_y2 = y2 - pen_width;
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CPen *pp, p(PS_NULL, 0, RGB(0,0,0));		// �M��pen
	pp = pDC->SelectObject(&p);
	CBrush *pb, b(RGB(110,100,80));				// �e progress��
	pb = pDC->SelectObject(&b);
	pDC->Rectangle(x1,y1,x2,y2);				
	CBrush b1(RGB(214,215,250) );				// �e progrss����
	pDC->SelectObject(&b1);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2_end,progress_y2);
	CBrush b2(RGB(31,26,226));					// �e progrss�i��
	pDC->SelectObject(&b2);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2,progress_y2);
	pDC->SelectObject(pp);						// ���� pen
	pDC->SelectObject(pb);						// ���� brush
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}
bool Chero::HitRectangle(int tx1, int ty1, int tx2, int ty2){	// �O�_�I��Ѽƽd�򪺯x��
	int x1 = GetX1() + 20;				// �y�����W��x�y��
	int y1 = GetY1() + 20;				// �y�����W��y�y��
	int x2 = GetX2() - 20;	// �y���k�U��x�y��
	int y2 = GetY2() - 20;	// �y���k�U��y�y��
	return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);// �˴��y���x�λP�ѼƯx�άO�_���涰
}
//###################################################################################################################################
//�ޯ�
Skill_basic::Skill_basic(){
	isRun=false;
}
int Skill_basic::GetX1(){ return x; }
int Skill_basic::GetY1(){ return y; }
void Skill_basic::SetX1(int sx){x = sx;}
void Skill_basic::SetY1(int sy){y = sy;}
bool Skill_basic::IsRun(){return isRun;}
void Skill_basic::SetRun(bool flag){isRun=flag;}
bool Skill_basic::isHitRectangle(int tx1, int ty1, int tx2, int ty2){	
	int x1 = GetX1();	// �y�����W��x�y��
	int y1 = GetY1();	// �y�����W��y�y��
	int x2 = GetX2();	// �y���k�U��x�y��
	int y2 = GetY2();	// �y���k�U��y�y��
	return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}
//--------------------------------------------------------------------------�ޯ�01(����1)
Skill_son01::Skill_son01(){}
void Skill_son01::LoadBitmap(){
	hit[0].LoadBitmap("Bitmaps/hero/hit_0.bmp",RGB(128,0,255));
	hit[1].LoadBitmap("Bitmaps/hero/hit_1.bmp",RGB(128,0,255));	
}
void Skill_son01::onMove(Chero *h,Map *m){
	if(!h->IsLeft() )					//�D���۹��m
		SetX1(h->GetX1()-40+45);
	else if(h->IsLeft() )
		SetX1(h->GetX1()-40-10);
	SetY1(h->GetY1()-20+38);
}
void Skill_son01::onShow(Chero *h,Map *m){
	bool flag_hit=false;		//����L�X
	if(h->GetHitAnimation().GetCurrentBitmapNumber()==2 && h->IsAttack()==1 && !h->IsLeft() ){
		hit[0].SetTopLeft(m->ScreenX(h->GetX1()-40+45), m->ScreenY(h->GetY1()-20+38) );
		hit[0].ShowBitmap();
		flag_hit=true;
	}
	else if(h->GetHitAnimation().GetCurrentBitmapNumber()==2 && h->IsAttack()==1 && h->IsLeft() ){
		hit[1].SetTopLeft(m->ScreenX(h->GetX1()-40-10), m->ScreenY(h->GetY1()-20+38));
		hit[1].ShowBitmap();
		flag_hit=true;
	}
	//-----------------------------------����
	static bool flag_music=false;
	if( !flag_music && flag_hit){
		CAudio::Instance()->Play(AUDIO_hit, false);
		flag_music = true;
	}
	else if(!flag_hit)
		flag_music = false;
}
int Skill_son01::GetX2(){ return x+hit[0].Width(); }
int Skill_son01::GetY2(){ return y+hit[0].Height(); }
//--------------------------------------------------------------------------
Skill_son02::Skill_son02(){}
void Skill_son02::LoadBitmap(){
	hit[0].LoadBitmap("Bitmaps/hero/hit_2.bmp",RGB(128,0,255));
	hit[1].LoadBitmap("Bitmaps/hero/hit_3.bmp",RGB(128,0,255));
}
void Skill_son02::onMove(Chero *h,Map *m){
	if(!h->IsLeft() )
		SetX1(h->GetX1()-40+98);	//�P�W
	else if(h->IsLeft() )
		SetX1(h->GetX1()-40-18);
	SetY1(h->GetY1()-20+54);
	//-----------------------------------�|�ھڧA���ۦ�����
	if(h->IsAttack()==2){
		if(!h->IsLeft() && h->GetX2() < m->GetMapW() && !m->IsWall(h->GetX2(),h->GetY2()-1) ){
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==1 && !m->IsNoland(h->GetX2()+15/4,h->GetY2()) )//��벾��(�k)
				h->SetX(15/4);
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==2 && !m->IsNoland(h->GetX2()+25/4,h->GetY2()) ) 
				h->SetX(25/4);
		}
		else if(h->IsLeft() && h->GetX1() > 0 && !m->IsWall(h->GetX1(),h->GetY2()-1) ){			
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==1 && !m->IsNoland(h->GetX1()-15/4,h->GetY2()) )//��벾��(��)
				h->SetX(-15/4);
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==2 && !m->IsNoland(h->GetX1()-25/4,h->GetY2()) )
				h->SetX(-25/4);	
		}
	}
}
void Skill_son02::onShow(Chero *h,Map *m){
	bool flag_hit=false;			//����L�X
	if( (h->GetHitAnimation().GetCurrentBitmapNumber()==2 || h->GetHitAnimation().GetCurrentBitmapNumber()==3) ){
		if( h->IsAttack()==2 && !h->IsLeft() ){
			hit[0].SetTopLeft(m->ScreenX(h->GetX1()-40+98), m->ScreenY(h->GetY1()-20+54) );
			hit[0].ShowBitmap();
			flag_hit=true;
		}
		else if (h->IsAttack()==2 && h->IsLeft() ){
			hit[1].SetTopLeft(m->ScreenX(h->GetX1()-40-18), m->ScreenY(h->GetY1()-20+54) );
			hit[1].ShowBitmap();
			flag_hit=true;
		}
	}
	//-----------------------------------����
	static bool flag_music=false;
	if( !flag_music && flag_hit){
		CAudio::Instance()->Play(AUDIO_hit, false);
		flag_music = true;
	}
	else if(!flag_hit)
		flag_music = false;
}
int Skill_son02::GetX2(){ return x+hit[0].Width(); }
int Skill_son02::GetY2(){ return y+hit[0].Height(); }
//--------------------------------------------------------------------------
Skill_son03::Skill_son03(){ }
void Skill_son03::LoadBitmap(){
	hit[0].LoadBitmap("Bitmaps/hero/hit_0.bmp",RGB(128,0,255));
	hit[1].LoadBitmap("Bitmaps/hero/hit_1.bmp",RGB(128,0,255));	
}
void Skill_son03::onMove(Chero *h,Map *m){
	if(!h->IsLeft() )
		SetX1(h->GetX1()-40+50);	//�P�W
	else if(h->IsLeft() )
		SetX1(h->GetX1()-40-13);
	SetY1(h->GetY1()-20+38);
	//-----------------------------------�|�ھڧA���ۦ�����
	if(h->IsAttack()==3){
		if(!h->IsLeft() && h->GetX2() < m->GetMapW() && !m->IsWall(h->GetX2(),h->GetY2()-1) ){
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==2 && !m->IsNoland(h->GetX2()+16/4,h->GetY2()) )//��벾��(�k)
				h->SetX(16/4);
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==3 && !m->IsNoland(h->GetX2()+15/4,h->GetY2()) ) 
				h->SetX(16/4);
		}
		else if(h->IsLeft() && h->GetX1() > 0 && !m->IsWall(h->GetX1(),h->GetY2()-1) ){			
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==2 && !m->IsNoland(h->GetX1()-16/4,h->GetY2()))//��벾��(��)
				h->SetX(-16/4);
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==3 && !m->IsNoland(h->GetX1()-16/4,h->GetY2()) )
				h->SetX(-16/4);	
		}
	}
}
void Skill_son03::onShow(Chero *h,Map *m){	
	bool flag_hit=false;			//����L�X
	if(h->GetHitAnimation().GetCurrentBitmapNumber()==3 && h->IsAttack()==3 && !h->IsLeft() ){
		hit[0].SetTopLeft(m->ScreenX(h->GetX1()-40+50), m->ScreenY(h->GetY1()-20+38) );
		hit[0].ShowBitmap();
		flag_hit=true;
	}
	else if(h->GetHitAnimation().GetCurrentBitmapNumber()==3 && h->IsAttack()==3 && h->IsLeft() ){
		hit[1].SetTopLeft(m->ScreenX(h->GetX1()-40-13), m->ScreenY(h->GetY1()-20+38));
		hit[1].ShowBitmap();
		flag_hit=true;
	}
	//-----------------------------------����
	static bool flag_music=false;
	if( !flag_music && flag_hit){
		CAudio::Instance()->Play(AUDIO_hit, false);
		flag_music = true;
	}
	else if(!flag_hit)
		flag_music = false;
}
int Skill_son03::GetX2(){ return x+hit[0].Width(); }
int Skill_son03::GetY2(){ return y+hit[0].Height(); }
//--------------------------------------------------------------------------
Skill_son04::Skill_son04(){
	hit[0].SetDelayCount(2);
	hit[1].SetDelayCount(2);
	menu.SetDelayCount(5);
}
void Skill_son04::init(Chero *h,Map *m){
	if( !h->IsAlive() || m->GetIsWin() ){
		for(int i=0;i<2;i++)
			hit[i].Reset();
		isRun=false;
	}
}
void Skill_son04::LoadBitmap(){
	string temp[11]={"0","1","2","3","4","5","6","7","8","9","10"};
	string rl[2]={"r","l"};
	for(int j=0;j<2;j++){			//Ū��
		for(int i=0;i<25;i++){
			if(i<11)	
				hit[j].AddBitmap( (char *)("Bitmaps/hero/skill01/effect_"+rl[j]+temp[i]+".bmp").c_str(), RGB(255,214,8));
			else if(i<20)
				hit[j].AddBitmap( (char *)("Bitmaps/hero/skill01/effect_"+rl[j]+temp[1]+temp[i-10]+".bmp").c_str(), RGB(255,214,8));
			else 
				hit[j].AddBitmap( (char *)("Bitmaps/hero/skill01/effect_"+rl[j]+temp[2]+temp[i-20]+".bmp").c_str(), RGB(255,214,8));
		}
		for(int i=0;i<10;i++)
			hit[j].AddBitmap( (char *)("Bitmaps/hero/skill01/special_"+rl[j]+temp[i]+".bmp").c_str(), RGB(255,214,8));
	}
	menu.AddBitmap("Bitmaps/hero/skill01/menu_0.bmp",RGB(128,0,255));
	menu.AddBitmap("Bitmaps/hero/skill01/menu_1.bmp",RGB(128,0,255));
	menu.AddBitmap("Bitmaps/hero/skill01/menu_2.bmp",RGB(128,0,255));
}
void Skill_son04::onMove(Chero *h,Map *m){
	static int num_past = 0;
	if(hit[0].IsFinalBitmap() ){
		for(int i=0;i<2;i++)
			hit[i].Reset();
		isRun=false;
	}
	num_past=hit[0].GetCurrentBitmapNumber();
	//-----------------------------------
	if(isRun)
		for(int i=0;i<2;i++)	//�b�����ɡA����onMove
			hit[i].OnMove();
	if(hit[0].GetCurrentBitmapNumber()<27){ //�j���27�i�ɡA������x�y��
		if(!h->IsLeft() )
			SetX1(h->GetX1()-40-30);
		else if(h->IsLeft() )
			SetX1(h->GetX1()-40-32);
	}		//�@���P�D�������@�˰���
	SetY1(h->GetY1()-20-37);	
	//-----------------------------------�]�m�Ϥ�����t��
	for(int i=0;i<2;i++){
		if(hit[i].GetCurrentBitmapNumber()>=10 && hit[i].GetCurrentBitmapNumber()<=24)
			hit[i].SetDelayCount(1);
		else
			hit[i].SetDelayCount(2);
	}
	//----------------------------------�䥴����
	int count;
	if(hit[0].GetCurrentBitmapNumber()==27){
		if(!h->IsLeft()){
			for(count=0;!m->IsWall(h->GetX2()+count,h->GetY2()-1) && (h->GetX2()+count)< m->GetMapW() && count<=40 ;count++){}		
			if(count>=40){
				if(!m->IsNoland(h->GetX2()+40,h->GetY2()) )	
					h->SetX(40);
			}
			else{ 
				if(!m->IsNoland(h->GetX2()+count,h->GetY2()) )
					h->SetX(count);
			}
		}
		else{
			for(count=0;!m->IsWall(h->GetX1()-count,h->GetY2()-1) && (h->GetX1()-count)>0 && count<=33;count++){}	
			if(count>=33){
				if( !m->IsNoland(h->GetX1()-33,h->GetY2()) )
					h->SetX(-33);
			}
			else{
				if(!m->IsNoland(h->GetX1()-count,h->GetY2()) )
 					h->SetX(-count);
			}	
		}
	}
	//-----------------------------------�ޯ�ϥ�(�i�_�ϥΧޯ�)
	if(menu.GetCurrentBitmapNumber()==0 && h->GetMP()>=40 && !h->IsSkill())
		menu.OnMove();
	else if(menu.GetCurrentBitmapNumber()!=0 && h->IsSkill()){
		menu.OnMove();
		if(!isRun)
			menu.Reset();
	}
	//-----------------------------------����
	int num_now=hit[0].GetCurrentBitmapNumber();
	if(num_past!=num_now && ( num_now==11 || num_now==16 || num_now==20 ) )
		CAudio::Instance()->Play(AUDIO_hit, false);
	if(num_past!=num_now && num_now == 27)
		CAudio::Instance()->Play(AUDIO_hit, false);

	static bool flag_skill=false;
	if(num_now==5 && !flag_skill){
		CAudio::Instance()->Play(AUDIO_skill, false);
		flag_skill=true;
	}
	else if(hit[0].GetCurrentBitmapNumber()!=9)
		flag_skill=false;
}
void Skill_son04::onShow(Chero *h,Map *m){
	if(isRun){
		if(!h->IsLeft()){	
			hit[0].SetTopLeft( m->ScreenX(x),m->ScreenY(y));
			hit[0].OnShow();
		}
		else{
			hit[1].SetTopLeft( m->ScreenX(x),m->ScreenY(y));
			hit[1].OnShow();
		}
	}
	menu.SetTopLeft(10,50);
	menu.OnShow();
}
void Skill_son04::HitEnemy(Enemy_basic *e){	
	int *num = e->hit_skill_num();
	int now_num=hit[0].GetCurrentBitmapNumber();//�ޯॴ��Ǫ����媬�p
	if( e->HitRectangle(GetX1(),GetY1(),GetX2(),GetY2()) ){
		if(num[0] != now_num && (now_num>10 && now_num<23) )
			e->HP_reduce(15);
		if(num[0] != now_num && (now_num>27 && now_num<32) )
			e->HP_reduce(35);
		if(now_num>10)
			e->SetbeHit(true);
	}
	num[0]=now_num;
	e->Set_hit_skill_num(num);
}
int Skill_son04::GetX2(){ return x+hit[0].Width(); }
int Skill_son04::GetY2(){ return y+hit[0].Height(); }
//--------------------------------------------------------------------------
Skill_son05::Skill_son05(){ 
	for(int i=0;i<2;i++){
		hit[i].SetDelayCount(3);
		bomb0[i].SetDelayCount(5);
		bomb1[i].SetDelayCount(5);
		bomb2[i].SetDelayCount(5);
	}
	menu.SetDelayCount(5);
}
void Skill_son05::init(Chero *h,Map *m){
	if( !h->IsAlive() || m->GetIsWin() ){
		for(int i=0;i<2;i++){
			hit[i].Reset();
			bomb0[i].Reset();
			bomb1[i].Reset();
			bomb2[i].Reset();
		}
		isRun=false;
	}
}
void Skill_son05::LoadBitmap(){
	string temp[13]={"0","1","2","3","4","5","6","7","8","9","10","11","12"};
	string rl[2]={"r","l"};
	for(int j=0;j<2;j++){
		for(int i=0;i<13;i++)
			hit[j].AddBitmap( (char *)("Bitmaps/hero/skill02/effect_"+rl[j]+temp[i]+".bmp").c_str(), RGB(96,96,96));
		for(int i=0;i<5;i++){
			bomb0[j].AddBitmap( (char *)("Bitmaps/hero/skill02/hit0_"+rl[j]+temp[i]+".bmp").c_str(), RGB(96,96,96));
			bomb1[j].AddBitmap( (char *)("Bitmaps/hero/skill02/hit1_"+rl[j]+temp[i]+".bmp").c_str(), RGB(96,96,96));
			bomb2[j].AddBitmap( (char *)("Bitmaps/hero/skill02/hit2_"+rl[j]+temp[i]+".bmp").c_str(), RGB(96,96,96));
		}
	}
	menu.AddBitmap("Bitmaps/hero/skill02/menu_0.bmp",RGB(128,0,255));
	menu.AddBitmap("Bitmaps/hero/skill02/menu_1.bmp",RGB(128,0,255));
	menu.AddBitmap("Bitmaps/hero/skill02/menu_2.bmp",RGB(128,0,255));
}
void Skill_son05::HitEnemy(Enemy_basic *e){
	int *num = e->hit_skill_num();
	int now_num = bomb0[0].GetCurrentBitmapNumber();
	int _bx,_by;
	for(int i=0;i<2;i++){		//�̧ǧP�_���u0,1,2 ���媬�p
		_by=y+by;
		_bx=x+bx0[i];
		if(e->HitRectangle(_bx,_by,_bx+bomb0[i].Width(),_by+bomb0[i].Height()) ){
			if(num[1] != now_num && now_num!=3 && now_num!=4)	e->HP_reduce(40);
			if(now_num>0) e->SetbeHit(true);
		}
		_bx=x+bx1[i];
		if(e->HitRectangle(_bx,_by,_bx+bomb0[i].Width(),_by+bomb0[i].Height()) ){
			if(num[1] != now_num && now_num!=0 && now_num!=4)	e->HP_reduce(40);
			if(now_num>1) e->SetbeHit(true);
		}
		_bx=x+bx2[i];
		if(e->HitRectangle(_bx,_by,_bx+bomb0[i].Width(),_by+bomb0[i].Height()) ){
			if(num[1] != now_num && now_num!=0 && now_num!=1)	e->HP_reduce(40);
			if(now_num>2) e->SetbeHit(true);
		}
	}
	num[1]=now_num;
	e->Set_hit_skill_num (num);
}
void Skill_son05::onMove(Chero *h,Map *m){
	static int num_past = 0;
	if( bomb2[0].IsFinalBitmap() ){	//�]��l
		for(int i=0;i<2;i++){
			hit[i].Reset();
			bomb0[i].Reset();
			bomb1[i].Reset();
			bomb2[i].Reset();
		}
		isRun=false;
	}
	num_past=bomb2[0].GetCurrentBitmapNumber();
	//------------------------�P�H���۹��m
	SetX1(h->GetX1()-40+41);
	SetY1(h->GetY2()-96);
	//------------------------��ޯ��
	if(isRun)
		for(int i=0;i<2;i++){
			if(!hit[i].IsFinalBitmap())
				hit[i].OnMove();
			if(hit[i].GetCurrentBitmapNumber()>8){
				bomb0[i].OnMove();
				bomb1[i].OnMove();
				bomb2[i].OnMove();
			}
		}
	//------------------------�ޯ�ϥ� �P�W
	if(menu.GetCurrentBitmapNumber()==0 && h->GetMP()>=40 && !h->IsSkill())
		menu.OnMove();
	else if(menu.GetCurrentBitmapNumber()!=0 && h->IsSkill()){
		menu.OnMove();
		if(!isRun)
			menu.Reset();
	}
	//------------------------����
	static bool flag_bomb=false;
	if(hit[0].GetCurrentBitmapNumber()==9 && !flag_bomb){
		CAudio::Instance()->Play(AUDIO_bomb, false);
		flag_bomb=true;
	}
	else if(hit[0].GetCurrentBitmapNumber()!=9)
		flag_bomb=false;
	int num_now=bomb2[0].GetCurrentBitmapNumber();
	if(num_now!=num_past && num_now<=2)
		CAudio::Instance()->Play(AUDIO_bomb, false);
	static bool flag_skill=false;
	if(hit[0].GetCurrentBitmapNumber()==2 && !flag_skill){
		CAudio::Instance()->Play(AUDIO_skill, false);
		flag_skill=true;
	}
	else if(hit[0].GetCurrentBitmapNumber()!=2)
		flag_skill=false;
}
void Skill_son05::SetBx(Map *m){
	hx = m->ScreenX(x);
	hy = m->ScreenY(y);
	by = 29;
	//----------------�]�w�z���϶��j
	bx0[0] = 37-25+dx;
	bx1[0] = bx0[0]+59;
	bx2[0] = bx1[0]+60;
	//----------------
	bx0[1] = -75+dx;
	bx1[1] = bx0[1]-57;
	bx2[1] = bx1[1]-60;
}
void Skill_son05::onShow(Chero *h,Map *m){
	if( !h->IsLeft() )  dx=10;
	else  dx=0;
	SetBx(m);
	for(int i=0;i<2;i++){	//�]�m���u��m
		hit[i].SetTopLeft(hx,hy);
		bomb0[i].SetTopLeft(hx+bx0[i],hy+by);
		bomb1[i].SetTopLeft(hx+bx1[i],hy+by);
		bomb2[i].SetTopLeft(hx+bx2[i],hy+by);
	}
	//--------------------------����ޯ�� ���
	if(isRun){
		if(!hit[0].IsFinalBitmap() && !hit[1].IsFinalBitmap()){
			if(!h->IsLeft())	
				hit[0].OnShow();
			else
				hit[1].OnShow();
		}
		if(hit[0].GetCurrentBitmapNumber()>8 && hit[1].GetCurrentBitmapNumber()>8){
			for(int i=0;i<2;i++){
				bomb2[i].OnShow();
				bomb1[i].OnShow();
				bomb0[i].OnShow();
			}
		}
	}
	//--------------------------�ޯ�ϥ�
	menu.SetTopLeft(50,50);
	menu.OnShow();
}
int Skill_son05::GetX2(){ return x+hit[0].Width(); }
int Skill_son05::GetY2(){ return y+hit[0].Height(); }
//--------------------------------------------------------------------------
EnemySkill_son01::EnemySkill_son01()
{
	height = 68;
	IsEndPicture = false;
	const int INITIAL_DOWNVELOCITY =1;
	downVelocity = init_downVelocity = INITIAL_DOWNVELOCITY;
}
void EnemySkill_son01::SetIsEndPicture(bool flag){ IsEndPicture = flag;}
void EnemySkill_son01::EonMove(Enemy_basic * e03,Map *m)
{
	const int MOVE_SIZE = 3;
	CAnimation attack=e03->GetAttack();
	bool temp1=(hit[0].GetCurrentBitmapNumber()!=0)&&(hit[0].GetCurrentBitmapNumber()!=3);
	bool temp2=(hit[1].GetCurrentBitmapNumber()!=0)&&(hit[1].GetCurrentBitmapNumber()!=3);
	//--------------------------------------------------��l
	if(hit[0].IsFinalBitmap()||hit[1].IsFinalBitmap()){
		IsEndPicture = true;
		hit[0].Reset();
		hit[1].Reset();
	}
	//--------------------------------------------------���� ����
	if(!IsEndPicture && attack.GetCurrentBitmapNumber()>=5){
		if(e03->GetIsLeft()){
			if(hit[0].GetCurrentBitmapNumber()<2)x-=MOVE_SIZE;
		}
		else{
			if(hit[1].GetCurrentBitmapNumber()<2)x+=MOVE_SIZE;
		}
		if(hit[0].GetCurrentBitmapNumber()!=3)	
			hit[0].OnMove();
		if(hit[1].GetCurrentBitmapNumber()!=3)
			hit[1].OnMove();		
		if(temp1 || temp2)//�����[���O
			onfloor(m ,e03);
	}
	else {
		downVelocity = init_downVelocity;	// ���]�U����l�t��	
		if(e03->GetIsLeft())
			this->SetX1(e03->GetX1()-40);
		else
			this->SetX1(e03->GetX2()+40-18); //���F�n�M���䪺�Ϲ��,�ҥH�n�����������e��
		this->SetY1(e03->GetY1()-30);//�`�ƭȬ��Ϥ����a���y��,�C�i�Ҩ̼˰�,�Y�K��L�i�������o�i���ΧC
	}
}
void EnemySkill_son01::onfloor(Map *m,Enemy_basic * e03){
	int count_floor=0;	//��l�����a����
	for(count_floor=0 ; ; count_floor++)
		if( m->IsEmpty( (x),(GetY2())+count_floor) ){}
		else break;
	if (count_floor >= downVelocity)
		y += (downVelocity)++;		// y�b�U��(����velocity���I)// �����O�v�T�A�U�����U���t�׼W�[
	else{
		y +=count_floor;	//�@����n��a
		downVelocity = init_downVelocity;	// ���]�U����l�t��	
		if(!e03->GetIsLeft())hit[0].OnMove();
		else hit[1].OnMove();
	}
}
void EnemySkill_son01::LoadBitmap()
{
	string temp[7]={"0","1","2","3","4","5","6"};
	string rl[2]={"r","l"};
	Vanish[0].LoadBitmap("Bitmaps/enemy/monky/banana_r_6.bmp",RGB(0,255,255));
	Vanish[1].LoadBitmap("Bitmaps/enemy/monky/banana_l_6.bmp",RGB(0,255,255));
	for(int i=0;i<2;i++)
		for(int j=0;j<4;j++)
			hit[i].AddBitmap((char *)("Bitmaps/enemy/monky/banana_"+rl[i]+"_"+temp[j]+".bmp").c_str(),RGB(0,255,255));
}
void EnemySkill_son01::EonShow(Enemy_basic  * e03,Map * m)
{	
	int esx =GetX1() - m->GetSX();
	int esy =GetY1() - m->GetSY();
	bool temp=(e03->GetAttack().GetCurrentBitmapNumber()>=5);
	//-------------------------------------------------	
	if(!e03->GetIsLeft() && !IsEndPicture && temp){  //���k�᭻��
		hit[0].SetTopLeft(esx,esy);
		hit[0].OnShow();
	}
	else if(e03->GetIsLeft() && !IsEndPicture && temp){	//�����᭻��
		hit[1].SetTopLeft(esx,esy);
		hit[1].OnShow();
	}
	else{
		Vanish[0].SetTopLeft(esx,esy);	//����
		Vanish[0].ShowBitmap();
	}
}
int EnemySkill_son01::GetX2(){return x + hit[0].Width();}
int EnemySkill_son01::GetY2(){return y + hit[0].Height();}
//--------------------------------------------------------------------------
EnemySkill_son02::EnemySkill_son02()
{
	IsEndPicture = false;
}
void EnemySkill_son02::SetIsEndPicture(bool flag){ IsEndPicture = flag;}
void EnemySkill_son02::EonMove(Enemy_basic * e05,Map *m)
{
	const int MOVE_SIZE = 4;
	CAnimation attack=e05->GetAttack();
	//-----------------------------------	��l
	if(hit[0].IsFinalBitmap()||hit[1].IsFinalBitmap()){
		IsEndPicture = true;
		hit[0].Reset();
		hit[1].Reset();
	}
	//-----------------------------------	���e�� ����
	if(!IsEndPicture && attack.GetCurrentBitmapNumber()>=4){
		if(e05->GetIsLeft())
				x-=MOVE_SIZE;
		else
				x+=MOVE_SIZE;
		hit[0].OnMove();
		hit[1].OnMove();		
	}
	else {
		if(e05->GetIsLeft())
			this->SetX1(e05->GetX1()-23);
		else
			this->SetX1(e05->GetX2()+23); //���F�n�M���䪺�Ϲ��,�ҥH�n�����������e��
		this->SetY1(e05->GetY1()+25);//�`�ƭȬ��Ϥ����a���y��,�C�i�Ҩ̼˰�,�Y�K��L�i�������o�i���ΧC
	}
}
void EnemySkill_son02::LoadBitmap()
{
	string temp[7]={"0","1","2","3","4","5","6"};
	string rl[2]={"r","l"};
	Vanish[0].LoadBitmap("Bitmaps/enemy/monky/banana_r_6.bmp",RGB(0,255,255));
	Vanish[1].LoadBitmap("Bitmaps/enemy/monky/banana_l_6.bmp",RGB(0,255,255));
	for(int i=0;i<2;i++)
		for(int j=0;j<4;j++)
			hit[i].AddBitmap((char *)("Bitmaps/enemy/pinkMonky/ball_"+rl[i]+"_"+temp[j]+".bmp").c_str(),RGB(0,255,255));
}
void EnemySkill_son02::EonShow(Enemy_basic  * e05,Map * m)
{	
	bool temp=(e05->GetAttack().GetCurrentBitmapNumber()>=4);
	//----------------------------------------------------------
	if(!e05->GetIsLeft() && !IsEndPicture && temp){			//���k����e��
		hit[0].SetTopLeft(m->ScreenX(x),m->ScreenY(y));
		hit[0].OnShow();
	}
	else if(e05->GetIsLeft() && !IsEndPicture && temp){		//��������e��
		hit[1].SetTopLeft(m->ScreenX(x),m->ScreenY(y));
		hit[1].OnShow();
	}
	else{
		Vanish[0].SetTopLeft(m->ScreenX(x),m->ScreenY(y));	//����
		Vanish[0].ShowBitmap();
	}
}

int EnemySkill_son02::GetX2(){return x + hit[0].Width();}
int EnemySkill_son02::GetY2(){return y + hit[0].Height();}
//--------------------------------------------------------------------------
//###################################################################################################################################
//�ĤH
Enemy_basic::Enemy_basic():x(0),y(0){
	const int INITIAL_DOWNVELOCITY =1;
	downVelocity = init_downVelocity = INITIAL_DOWNVELOCITY;
	hitSkill_num[0]=hitSkill_num[1]=0;
	count_P_Direction=33; //�Ψӵ�����V�ɯ��[�F��,���{���X��33����~�|�A�M�w��V
}
int Enemy_basic::SetTotal_HP(){
	return HP_total;
};
void Enemy_basic::init(Map *m){
	isAttack=beHit=isLeft=false;
	HP=SetTotal_HP();
	SetIsAlive(true);//�]�w�C�@���ĤH��l�Ʀs��
	//---------------------------------------------------------------------------------
	srand((int)time(NULL));//�üƺؤl
	int direction = rand()%2+1;  //�]�w��V �üƨ��o�ƭ�1��2 �M�w�ĤH��l��� 1���� 
	SetIsLeft(direction);
	SetX(FIX_X-left_x);	  //�]�w�ĤH����l�� X�y��
	SetY(FIX_Y-height);
	//--------------------------------------------------------------�}�l���b�a�W
	for(int i=0 ; (GetY2()+i)<(m->GetMapH()) ; i++)
		if( m->IsFloor(GetX1()+center,GetY2()+i) || m->IsMoveBarrier(GetX1(),GetX2(),GetY2()+i) ){
			SetY(y+i);
			break;
		}
}
void Enemy_basic::onShow(Map* m){
	int ex = x - m->GetSX();
	int ey = y - m->GetSY();
	if( IsAlive() ){
		if(beHit){
			if(!isLeft){
				BeHit[0].SetTopLeft(ex,ey);
				BeHit[0].ShowBitmap();
			}
			else{
				BeHit[1].SetTopLeft(ex,ey);
				BeHit[1].ShowBitmap();
			}
		}
		 else if(GetEnemyId()!=1  && isAttack){
			if(!isLeft){
				Attack[0].SetTopLeft(ex,ey);
				Attack[0].OnShow();
			}
			else{
				Attack[1].SetTopLeft(ex,ey);
				Attack[1].OnShow();
			}
		}
		else if(!isLeft && last_x < x){
			Move[0].SetTopLeft(ex,ey);
			Move[0].OnShow();
		}
		else if(isLeft && last_x > x){
			Move[1].SetTopLeft(ex,ey);
			Move[1].OnShow();
		}
		else if(last_x == x){
			if(!isLeft){
				Stand[0].SetTopLeft(ex,ey);
				Stand[0].OnShow();
			}
			else{
				Stand[1].SetTopLeft(ex,ey);
				Stand[1].OnShow();
			}
		}
		int HP_percent=(int)( ((double)HP/HP_total)*100 );
		ShowHP(HP_percent,m);
	}
}
void Enemy_basic::onMove(Map* m,Chero *h){
	srand((int)time(NULL));//�üƺؤl �Ψӹ�C���Ǫ������ʽd����ü�!
	int direction =rand()%9+1; //1����
	//--------------------------------------------��l
	for(int i=0;i<2;i++){
		Move[i].OnMove();
		Stand[i].OnMove();
		if(isAttack )
			Attack[i].OnMove();
	}
	//--------------------------------------------
	last_x=x; //�W�@�Ӯy���x�s �Φb�K�ϧP�_
	if(IsAlive() ){//�s����
		if(h->IsAttack()!=hit_num ||h->IsAttack()==0)
			beHit=false;
		beHit_by_Hero(h,m);		//�Q���媬�p
		HitAI(h,m);			//����AI
		if(!isAttack && !InArea(h) ){//���Ǫ��i�H���n�@�����ʰ����i�H���@�U
			if(count_wait==waitTime ){	
				Walking(h,m);	//�Ǫ����ʪ��d��
				count_move--;
				if(count_move<0)
				count_wait--;
			}
			else{
				count_wait--;		//ĵ�ٶ��j
				this->SetIsLeft(direction);
			if(count_wait<0){
					moveTime=rand()%250+100;
					waitTime=rand()%250+50;
					count_move=moveTime;
				count_wait=waitTime;
				}
			}
		}
		onfloor(m);		//���O
		//-------------------------
		if(HP<=0){//�S���
			beHit=false; //�]�w���A�����`		
			SetIsAlive(false);
		}
		hit_num=h->IsAttack();
	}
	else{			//���`	
		HP=HP_total;
		isAttack=false;
	}
}
void Enemy_basic::Walking(Chero *h,Map *m){
	//����(ĵ��)�d��------
	srand((int)time(NULL));//�üƺؤl �Ψӹ�C���Ǫ������ʽd����ü�!
	MOVE_RANGE = rand()%200+100; //�d��b100��200����
	//-------------------
	if(last_y!=y){}
	else if( !isLeft ){	//(�k���)
		if( ( m->IsFloor(GetX2()+STEP_SIZE,GetY2()) || m->IsMoveBarrier(GetX2()+STEP_SIZE,GetX2()+STEP_SIZE,GetY2()) ) && GetX2()+STEP_SIZE <=GetFx()+MOVE_RANGE && !m->IsWall(GetX2()+STEP_SIZE,GetY2()-1) && GetX2()+STEP_SIZE < m->GetMapW() )
			x+=STEP_SIZE;		//��(�a�W�λ�ê�� �k�i��)&&(�d��)&&(�k��S���)  �k��
		else
			isLeft=true;
	}	
	else{			//(�����)
		if( ( m->IsFloor(GetX1()-STEP_SIZE,GetY2()) || m->IsMoveBarrier(GetX1()-STEP_SIZE,GetX2()-STEP_SIZE,GetY2()) ) && GetX1()-STEP_SIZE >=GetFx()-MOVE_RANGE && !m->IsWall(GetX1()-STEP_SIZE,GetY2()-1) && GetX1()-STEP_SIZE > 0)
			x-=STEP_SIZE;		//��(�a�W�λ�ê�� ���i��)&&(�d��)&&(����S���)  ����
		else
			isLeft=false;
	}
}
void Enemy_basic::onfloor(Map *m){
	
	int count_floor=0;	//��l�����a����
	int number=0;
	int center = ( GetX1()+GetX2() )/2;
	vector<MovingBarrier *>barrier=m->GetMovingBarrier();
	last_y=y;
	//--------------------------------���a����
	for(count_floor=0;;count_floor++){
		if( !m->IsFloor(center,GetY2()+count_floor) && !IsTouchBarrier(barrier,count_floor,&number)  && GetY2()+count_floor<=m->GetMapH()+100){}
		else break;
	}
	//-------------------------------
	if (count_floor >= downVelocity)
		y += (downVelocity)++;		// y�b�U��(����velocity���I)// �����O�v�T�A�U�����U���t�׼W�[
	else{
		y +=count_floor;	//�@����n��a
		downVelocity = init_downVelocity;	// ���]�U����l�t��	
	}
	if(last_y!=y) SetX(x);	//�q�]�}�lX�y��
}
bool Enemy_basic::IsTouchBarrier(vector<MovingBarrier *> b,int count,int *number){
	for(int i=0;i<(int)b.size();i++){
		if(GetX1() >= b[i]->GetX1() && GetX2() <= b[i]->GetX2()){ //�P�W
			if( GetY2()+count == b[i]->GetY() ){
				*number=i;
				return true;
			}
			if( GetY2()+count+b[i]->GetYchange() == b[i]->GetY() ){
				*number=i;
				return true;
			}
		}
	}
/*	for(int i=0;i<(int)b.size();i++)		//���q�Ω�ץ�(x�y�нd��Ӥj��)�A�bMovebarrier��Movebarrier�������ҽk�a�a
		for(int j=0;j<(int)b.size();j++)	//���[�W�o�q��A�|�y���C�������Z(�j��Ӧh�h!)�A�G�H�]�p�a�ϮɡA�קK�����p
			if( b[i]->HitRectangle(b[j]->GetX1(),b[j]->GetX2(),b[j]->GetY()) ){
				int b_X1,b_X2;

				if(b[i]->GetX1() <= b[j]->GetX1())		b_X1=b[i]->GetX1();
				else	b_X1=b[j]->GetX1();

				if(b[i]->GetX2() >= b[j]->GetX2())		b_X2=b[i]->GetX2();
				else	 b_X2=b[j]->GetX2();
				
				if(GetX1()>=b_X1 && GetX2()<=b_X2){
					if(GetY2()+count==b[i]->GetY()){
						*number=i;
						return true;
					}
					if( GetY2()+count+b[i]->GetYchange() == b[i]->GetY() ){
						*number=i;
						return true;
					}
				}
			}*/
	return false;
}
int Enemy_basic::GetX1(){ return x + left_x; }
int Enemy_basic::GetY1(){ return y; }
int Enemy_basic::GetX2(){ return x + right_x; }
int Enemy_basic::GetY2(){ return y + height; }
int Enemy_basic::GetRang(){return MOVE_RANGE;}
int Enemy_basic::GetFx() const {return fix_x;}
int Enemy_basic::GetEnemyId(){return EnemyId;}
void Enemy_basic::SetX(int x_pos){
	x = x_pos;
	fix_x = x_pos;
}
void Enemy_basic::SetY(int y_pos){y = y_pos;}
bool Enemy_basic::IsAlive(){return is_alive;}
void Enemy_basic::SetIsAlive(bool alive){is_alive = alive;}
bool Enemy_basic::IsAttack(){return isAttack;}
bool Enemy_basic::IsbeHit(){return beHit;}
bool Enemy_basic::GetIsLeft(){return isLeft;}
void Enemy_basic::SetbeHit(bool flag){ beHit=flag; }
int* Enemy_basic::hit_skill_num(){ return hitSkill_num;}
void Enemy_basic::Set_hit_skill_num(int *num){
	hitSkill_num[0] = num[0];
	hitSkill_num[1] = num[1];
}
void Enemy_basic::SetIsLeft(int d){
	if(count_P_Direction==0){
		if(!isLeft)
			isLeft = true; //1������
		else
			isLeft = false;
		count_P_Direction=33;
	}
	else 
		count_P_Direction--;
}
void Enemy_basic::HP_reduce(int r){ HP-=r;}
void Enemy_basic::ShowHP(int percent,Map *m){
	if (!SHOW_LOAD_PROGRESS)
		return;
	const int bar_width = ( GetX2()-GetX1() )*2;//60;		38
	const int bar_height = 6;		
	const int x1=m->ScreenX(GetX1()-bar_width/4);
	const int x2 = x1 + bar_width;
	const int y1 = m->ScreenY(GetY1()-15); //-15
	const int y2 = y1 + bar_height;
	const int pen_width = 1;//bar_height / 5;
	const int progress_x1 = x1 + pen_width;
	const int progress_x2 = progress_x1 + percent * (bar_width-2*pen_width) / 100;
	const int progress_x2_end = x2 - pen_width;
	const int progress_y1 = y1 + pen_width;
	const int progress_y2 = y2 - pen_width;
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CPen *pp, p(PS_NULL, 0, RGB(0,0,0));		// �M��pen
	pp = pDC->SelectObject(&p);
	CBrush *pb, b(RGB(255,255,0));				// �e���� progress��
	pb = pDC->SelectObject(&b);
	pDC->Rectangle(x1,y1,x2,y2);				
	CBrush b1(RGB(220,160,160) );//DEFAULT_BG_COLOR);				// �e�¦� progrss����
	pDC->SelectObject(&b1);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2_end,progress_y2);
	CBrush b2(RGB(255,0,0));					// �e���� progrss�i��
	pDC->SelectObject(&b2);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2,progress_y2);
	pDC->SelectObject(pp);						// ���� pen
	pDC->SelectObject(pb);						// ���� brush
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}
void Enemy_basic::beHit_by_Hero(Chero *h,Map *m){//�Q�D����
	vector<Skill_basic *> skill = h->GetSkill();
	CAnimation  hit = h->GetHitAnimation();
	int dx=30/4;									//���媬�p
//  �q�Ƥ@  //----------------------------------------
	if(h->IsAttack()==1 && (int)hit.GetCurrentBitmapNumber()==2)	
		if( skill[0]->isHitRectangle(GetX1(),GetY1(),GetX2(),GetY2()) )	{
			if(!beHit){
				HP_reduce(10);	
				beHit=true;
			}
			if(GetX1()-dx > 0 && GetX2()+dx < m->GetMapW()){
				if(!h->IsLeft() &&  !m->IsWall(GetX2()+dx,GetY2()-1) && !m->IsNoland(GetX2()+dx,GetY2()) )
					x+=dx;	//�Q������
				else if(!m->IsWall(GetX1()-dx,GetY2()-1) && !m->IsNoland(GetX1()-dx,GetY2()) )
					x-=dx;
			}
		}
//  �q�ƤG  //----------------------------------------
	if(h->IsAttack()==2 && hit.GetCurrentBitmapNumber()==2)
		if( skill[1]->isHitRectangle(GetX1(),GetY1(),GetX2(),GetY2()) ){	
			if(!beHit){
				HP_reduce(10);
				beHit=true;
			}
			if(GetX1()-dx > 0 && GetX2()+dx < m->GetMapW()){
				if(!h->IsLeft() &&  !m->IsWall(GetX2()+dx,GetY2()-1) && !m->IsNoland(GetX2()+dx,GetY2()) )
					x+=dx;	//�Q������
				else if(!m->IsWall(GetX1()-dx,GetY2()-1) && !m->IsNoland(GetX1()-dx,GetY2()) )
					x-=dx;
			}
		}
//  �q�ƤT  //----------------------------------------
	if(h->IsAttack()==3 && hit.GetCurrentBitmapNumber()==3)
		if( skill[2]->isHitRectangle(GetX1(),GetY1(),GetX2(),GetY2()) ){	
			if(!beHit){
				HP_reduce(15);
				beHit=true;
			}
			if(GetX1()-dx > 0 && GetX2()+dx < m->GetMapW()){
				if(!h->IsLeft() &&  !m->IsWall(GetX2()+dx,GetY2()-1) && !m->IsNoland(GetX2()+dx,GetY2()) )
					x+=dx;	//�Q������
				else if(!m->IsWall(GetX1()-dx,GetY2()-1) && !m->IsNoland(GetX1()-dx,GetY2()) )
					x-=dx;
			}
		}
//   �ۦ�   //----------------------------------------
	for(int i=0;i<(int)skill.size();i++)
		skill[i]->HitEnemy(this);
}
bool Enemy_basic::isHitHero(Chero *h){
	return false;
}
bool Enemy_basic::HitRectangle(int tx1, int ty1, int tx2, int ty2){	// �O�_�I��Ѽƽd�򪺯x��
	int x1 = GetX1();				// �y�����W��x�y��
	int y1 = GetY1();				// �y�����W��y�y��
	int x2 = GetX2();	// �y���k�U��x�y��
	int y2 = GetY2();	// �y���k�U��y�y��
	return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}
//###################################################################################################################################
Enemy_son01::Enemy_son01(int fx,int fy){
	EnemyId=1;
	x=FIX_X=fx , y=FIX_Y=fy;
	left_x=0 ,right_x=43;
	height=54;
	HP_total=150;
	//----------------------
	center=(left_x+right_x)/2;
	STEP_SIZE = 2; 
	Move[0].SetDelayCount(6);
	Move[1].SetDelayCount(6);
}
void Enemy_son01::LoadBitmap(){
	string temp[3]={"01","02","03"};
	string rl[2]={"r","l"};
	for(int j=0;j<2;j++){
		BeHit[j].LoadBitmap((char *)("Bitmaps/enemy/devil_cat/behit_"+rl[j]+".bmp").c_str(),RGB(0,255,128) );
		for(int i=0;i<3;i++){
				Move[j].AddBitmap((char *)("Bitmaps/enemy/devil_cat/move_"+rl[j]+temp[i]+".bmp").c_str(),RGB(0,255,128) );
				Stand[j].AddBitmap((char *)("Bitmaps/enemy/devil_cat/move_"+rl[j]+temp[i]+".bmp").c_str(),RGB(0,255,128) );
			}
	}
}
void Enemy_son01::onMove(Map *m,Chero *h){
	for(int i=0;i<2;i++){
		Move[i].OnMove();
		Stand[i].OnMove();
	}
	//--------------------
	last_x=x; //�W�@�Ӯy���x�s �Φb�K�ϧP�_
	if(IsAlive() ){		//�s����
		if( h->IsAttack()!=hit_num || !h->IsAttack() )
			beHit=false;
		beHit_by_Hero(h,m);	//�Q��
		HitAI(h,m);	//����AI
		if(!isAttack && !InArea(h))		
			Walking(h,m);	//�Ǫ����ʪ��d��
		onfloor(m);	//���O
		//--------------------	
		if(HP<=0){//�S���
			beHit=false; //�]�w���A�����`		
			SetIsAlive(false);
		}
		hit_num=h->IsAttack();
	}
	else{		//���`	
		HP=HP_total;
		isAttack=false;
	}
}
bool Enemy_son01::InArea(Chero *h){
	return false;
}
void Enemy_son01::HitAI(Chero *h,Map *m){}
//----------------------------------------------------------------------------
Enemy_son02::Enemy_son02(int fx,int fy){
	EnemyId=2;
	x=FIX_X=fx , y=FIX_Y=fy;
	left_x=205 ,right_x=268;
	height=243;
	HP_total=500;
	center=(left_x+right_x)/2;
	//------------------------
	countHit=HitTime=30;//�������j
	count_move=moveTime=rand()%250+100; //���ʮɶ�
	count_wait=waitTime=rand()%200+50;		//ĵ�ٶ��j
	STEP_SIZE = 5;	//���ʶq
	for(int i=0;i<2;i++){
		Move[i].SetDelayCount(4);
		Attack[i].SetDelayCount(6);
	}
}
void Enemy_son02::LoadBitmap(){
	string temp[12]={"0","1","2","3","4","5","6","7","8","9","10","11"};
	string rl[2]={"r","l"};
	for(int j=0;j<2;j++){
		BeHit[j].LoadBitmap((char *)("Bitmaps/enemy/ox/behit_"+rl[j]+".bmp").c_str(),RGB(128,0,255));
		for(int i=0;i<12;i++){
			if(i<6)
				Move[j].AddBitmap((char *)("Bitmaps/enemy/ox/move_"+rl[j]+temp[i]+".bmp").c_str(),RGB(128,0,255) );
			if(i<5 )
				Attack[j].AddBitmap((char *)("Bitmaps/enemy/ox/attack_"+rl[j]+temp[i]+".bmp").c_str(),RGB(128,0,255) );
			if(i<8)
				Stand[j].AddBitmap((char *)("Bitmaps/enemy/ox/stand_"+rl[j]+temp[i]+".bmp").c_str(),RGB(128,0,255) );
		}
	}
}
bool Enemy_son02::InArea(Chero *h){
	int Area=300;
	int Center = (GetX1()+GetX2())/2; //���Ǫ�X�b�������I
	int hero_center=(h->GetX1()-40+h->GetX2())/2;
	bool condition1=(GetX2()+Area)>(h->GetX1());	//		�Ǫ����H
	bool condition2=(GetX1()-Area)<(h->GetX2());	//	�H���Ǫ�
	if( condition1 && condition2 && h->GetY2()<=GetY2() && h->GetY2()>=GetY1()){ //�b�d��(���k)(�W�U) 
		if(Center<= hero_center)	//		�Ǫ����H
			isLeft=false;
		else						//	�H���Ǫ�
			isLeft=true;			
		return true;	
	}
	return false;
}
void Enemy_son02::HitAI(Chero *h,Map *m){
	//---------------------------------------------�d�� �|�l�H
	if(!isAttack && InArea(h) && countHit==HitTime){
		if( ( m->IsFloor(GetX2()+STEP_SIZE,GetY2()) || m->IsMoveBarrier(GetX2()+STEP_SIZE,GetX2()+STEP_SIZE,GetY2()) ) && GetX2()+60 <= h->GetX1() && !m->IsWall(GetX2()+STEP_SIZE,GetY2()-1) )
			x+=STEP_SIZE;
		else if( ( m->IsFloor(GetX1()-STEP_SIZE,GetY2()) || m->IsMoveBarrier(GetX1()-STEP_SIZE,GetX2()-STEP_SIZE,GetY2()) ) && GetX1()-60 >=h->GetX2() && !m->IsWall(GetX1()-STEP_SIZE,GetY2()-1) )
			x-=STEP_SIZE;
	}
	int area=100;
	//---------------------------------------------
	if( !isAttack && !beHit){//�S���H�A�S�Q��
		if(countHit==HitTime){
			if( (GetX2()+area)>(h->GetX1()) && (GetX1()-area)<(h->GetX2())  && h->GetY2()<=GetY2() && h->GetY2()>=GetY1()){
				isAttack=true;					//�d��
				countHit--;
			}
		}
		else if(countHit<HitTime)//�������j
			countHit--;
	}		
	if(countHit<0)
		countHit=HitTime;
	//---------------------------------------------
	if(Attack[0].IsFinalBitmap()||Attack[1].IsFinalBitmap()){ 
		Attack[0].Reset();
		Attack[1].Reset();	
		isAttack=false;
	}
}
void Enemy_son02::onShow(Map* m){
	if(isAttack && beHit){
		if(!isLeft){
			Attack[0].SetTopLeft(m->ScreenX(x),m->ScreenY(y));
			Attack[0].OnShow();
		}
		else{
			Attack[1].SetTopLeft(m->ScreenX(x),m->ScreenY(y));
			Attack[1].OnShow();
		}
		int HP_percent=(int)( ((double)HP/HP_total)*100 );
		ShowHP(HP_percent,m);
	}
	else
		Enemy_basic::onShow(m);
}
bool Enemy_son02::isHitHero(Chero *h){	//�����P�_
	if(!isLeft && (Attack[0].GetCurrentBitmapNumber()==2 || Attack[0].GetCurrentBitmapNumber()==3) )	
		return h->HitRectangle(GetX2()+23,GetY1()-130,GetX2()+186,GetY2());	
	else if(isLeft && (Attack[0].GetCurrentBitmapNumber()==2 || Attack[0].GetCurrentBitmapNumber()==3) )
		return h->HitRectangle(GetX1()-185,GetY1()-130,GetX1()-22,GetY2());
	return false;
}
int Enemy_son02::GetY1(){
	return y+120;
}
//----------------------------------------------------------------------------Enemy_son01::Enemy_son01(){
Enemy_son03::Enemy_son03(int fx,int fy){
	EnemyId=3;
	x=FIX_X=fx , y=FIX_Y=fy;
	left_x=48 ,right_x=81;		//�]�w�d��
	height=72;						//�}��a�I
	HP_total=500;
	center=(left_x+right_x)/2;
	//------------------------------
	countHit=HitTime=30;//�������j
	count_move=moveTime=rand()%250+100; //���ʮɶ�
	count_wait=waitTime=rand()%200+50;		//ĵ�ٶ��j
	MOVE_RANGE = rand()%300+100; //�d��b100��500����
	STEP_SIZE = 2;	//���ʶq
	//---------------------------------
	Move[0].SetDelayCount(6);
	Move[1].SetDelayCount(6);
	Attack[0].SetDelayCount(6);
	Attack[1].SetDelayCount(6);
}
void Enemy_son03::LoadBitmap(){
	string temp[11]={"0","1","2","3","4","5","6","7","8","9","10"};
	string rl[2]={"r","l"};
	Vanish[0].LoadBitmap("Bitmaps/enemy/monky/die_r_3.bmp",RGB(0,255,255));
	Vanish[1].LoadBitmap("Bitmaps/enemy/monky/die_r_3.bmp",RGB(0,255,255));
	for(int j=0;j<2;j++){
		BeHit[j].LoadBitmap((char *)("Bitmaps/enemy/monky/behit_"+rl[j]+".bmp").c_str(),RGB(0,255,255));
		for(int i=0;i<11;i++){
			if(i < 4)
				Move[j].AddBitmap((char *)("Bitmaps/enemy/monky/move_"+rl[j]+"_"+temp[i]+".bmp").c_str(),RGB(0,255,255) );
			if(i<1)
				Stand[j].AddBitmap((char *)("Bitmaps/enemy/monky/stand_"+rl[j]+"_"+temp[i]+".bmp").c_str(),RGB(0,255,255));
			
			Attack[j].AddBitmap((char *)("Bitmaps/enemy/monky/attack_"+rl[j]+"_"+temp[i]+".bmp").c_str(),RGB(0,255,255) );
		}
	}
	es01.LoadBitmapA();
}
bool Enemy_son03::InArea(Chero *h){//�����d��
	int Area=100;
	int Center = (GetX1()+GetX2())/2; //���Ǫ�X�b�������I
	int hero_center=(h->GetX1()-40+h->GetX2())/2;
	bool condition1=(GetX2()+Area)>(h->GetX1());	//		�ǡ��H
	bool condition2=(GetX1()-Area)<(h->GetX2());	//	�H����
	if( condition1 && condition2 && h->GetY1()<=GetY2()+150 && h->GetY2()>=GetY1()){
		if(Center<= hero_center )//		�ǡ��H
			isLeft=false;	
		else			//	�H����
			isLeft=true;	
		return true;	
	}
	return false;
}
void Enemy_son03::HitAI(Chero *h,Map *m){//�������A
	int area=100;
	if(!isAttack && InArea(h) ){} //�d�򤺡A�]�w��V
	//------------------------------
	if( !isAttack && !beHit){//�S���H�A�S�Q��
		es01.SetIsEndPicture(false);
		if(countHit==HitTime){//�������j
			if( (GetX2()+area)>(h->GetX1()) && (GetX1()-area)<(h->GetX2())  && h->GetY1()<=GetY2()+150 && h->GetY2()>=GetY1() && isAttack==false){
				isAttack=true;
				countHit--;
			}
		}
		else if(countHit<HitTime)
			countHit--;
	}
	//--------------------
	if(countHit<0)
		countHit=HitTime;
	es01.EonMove(this,m);
	if(Attack[0].IsFinalBitmap()||Attack[1].IsFinalBitmap()){ 
		Attack[0].Reset();
		Attack[1].Reset();
		isAttack=false;
	}
}
bool Enemy_son03::isHitHero(Chero *h){ //�����Ϥ���˨�D����Func
	if(h->HitRectangle(es01.GetX1(),es01.GetY1(),es01.GetX2(),es01.GetY2()) && isAttack && GetAttack().GetCurrentBitmapNumber()>=5)
		return true;
	return false;
}
void  Enemy_son03::onShow(Map* m){
	if(isAttack)	//�ޯ�SHOW
		es01.EonShow (this,m);
	Enemy_basic::onShow(m);//�Ǫ�SHOW
}
int Enemy_son03::GetY1(){
	return y+20; //�U�l���Y�Y�y��
}
CAnimation Enemy_son03::GetAttack(){
	if(!isLeft)
		return Attack[0];
	else
		return Attack[1];
}
//-----------------------------------------------------
Enemy_son04::Enemy_son04(int fx,int fy){
	EnemyId=4;//�Ψӿ��ѥD���Q���ɭӧO������q
	x=FIX_X=fx , y=FIX_Y=fy;
	left_x=60 ,right_x=153;
	height=134;
	HP_total=100;//��Ӫ���q
	center=(left_x+right_x)/2;
	//---------------------------
	countHit=HitTime=30;//�������j
	count_move=moveTime=rand()%250+100; //���ʮɶ�
	count_wait=waitTime=rand()%200+50;		//ĵ�ٶ��j
	//----------------------------	
	STEP_SIZE = 3; 
	MOVE_RANGE = rand()%300+100; //�d��b100��500����
	Move[0].SetDelayCount(6);
	Move[1].SetDelayCount(6);
}
void Enemy_son04::LoadBitmap(){
	string temp[11]={"0","1","2","3","4","5","6","7","8","9","10"};
	string rl[2]={"r","l"};
	Vanish[0].LoadBitmap("Bitmaps/enemy/toy/die_r_5.bmp",RGB(0,255,255));
	Vanish[1].LoadBitmap("Bitmaps/enemy/toy/die_l_5.bmp",RGB(0,255,255));
	for(int j=0;j<2;j++){
		BeHit[j].LoadBitmap((char *)("Bitmaps/enemy/toy/hit_"+rl[j]+".bmp").c_str(),RGB(0,255,255));
		for(int i=0;i<10;i++){
			Stand[j].AddBitmap((char *)("Bitmaps/enemy/toy/stand_"+rl[j]+"_"+temp[i]+".bmp").c_str(),RGB(0,255,255));
			if(i<7)
				Move[j].AddBitmap((char *)("Bitmaps/enemy/toy/move_"+rl[j]+"_"+temp[i]+".bmp").c_str(),RGB(0,255,255) );
			if(i<5)
				Attack[j].AddBitmap((char *)("Bitmaps/enemy/toy/attack_"+rl[j]+"_"+temp[i]+".bmp").c_str(),RGB(0,255,255) );				
		}
	}
}
bool Enemy_son04::InArea(Chero *h){
	int Area=200;
	int h_center =( h->GetX1()+h->GetX2() )/2 ;
	int Center = ( GetX1()+GetX2() )/2; //���Ǫ�X�b�������I
	bool condition1=(GetX2()+Area)>(h->GetX1()); //�D���b�Ǫ����k��d�� 
	bool condition2=(GetX1()-Area)<(h->GetX2()); //�D���b�Ǫ�������d�� 
	if( condition1 && condition2 && h->GetY2()<=GetY2() && h->GetY2()>=GetY1()+80){ //�涰�ҥH�O�D���b�Ǫ��������k��d��
		if(!isAttack){
			if( h_center > Center)		//�P�W
				isLeft=false;
			else
				isLeft=true;
		}
		return true;	
	}
	return false;
}

void Enemy_son04::HitAI(Chero *h,Map *m){
	int h_area=100;
	int attack_speed=STEP_SIZE+7;
	int h_center =( h->GetX1()+h->GetX2() )/2 ;
	int Center = (GetX1()+GetX2())/2; //���Ǫ�X�b�������I
	if( InArea(h) && !beHit){ //�Y�b�d�򤺥B�S�Q�����ܴN�l���D��
		isAttack = true; //�]���������A���������Ϥ�
		if( !isLeft && Center<=(h_center + h_area) ){ //���F�i�H�s��ļ��D�� ���Y�ثe�b�ӥD���k�䪺�Y�@�ӽd�򤺥B�b���䪬�A
			bool onland=m->IsMoveBarrier(GetX1()+attack_speed,GetX2()+attack_speed,GetY2()) || m->IsFloor(GetX2()+attack_speed,GetY2());
			if( onland && Center+attack_speed <=(h_center + h_area) && !m->IsWall(GetX2()+attack_speed,GetY2()-1) )//�Y�W�X���k�䪺�d���B�k��Ǫ��e�����@�I(x2�y��)(���ʽd��)�O�Ū� �ܴ���V���A 
				x+=(attack_speed); //�V�k����
			else
				isLeft = true ;
		}
		else if( isLeft && Center>=(h_center-h_area) ){ //�����b�D���k��d��� �A�ܴ���V
			bool onland=m->IsMoveBarrier(GetX1()-attack_speed,GetX2()-attack_speed,GetY2()) || m->IsFloor(GetX1()-attack_speed,GetY2());
			if( onland && Center-attack_speed >= (h_center - h_area) && !m->IsWall(GetX1()-attack_speed,GetY2()-1) ) //�W�L�D������d���B����Ǫ��e�����@�I(x1�y��)(���ʽd��)�O�Ū� �A�ܴ���V 
				x-=(attack_speed);//����
			else
				isLeft = false ;
		}
		else
			if(isLeft)
				isLeft=false;
			else
				isLeft=true;
	}
	else
		isAttack =false;
}
bool Enemy_son04::isHitHero(Chero *h){
	return false;
}
int Enemy_son04::GetY1(){
	return y+11; 
}
//-------------------------------------------------------------------
Enemy_son05::Enemy_son05(int fx,int fy){
	EnemyId=5;
	x=FIX_X=fx , y=FIX_Y=fy;		
	left_x=60 ,right_x=90;		//�]�w�d��
	height=83;						//�}��a�I
	HP_total=300;
	center=(left_x+right_x)/2;
	//--------------------------	
	countHit=HitTime=30;//�������j
	count_move=moveTime=rand()%250+100; //���ʮɶ�
	count_wait=waitTime=rand()%200+50;		//ĵ�ٶ��j
	MOVE_RANGE = rand()%300+100; //�d��b100��500����
	//--------------------------	
	STEP_SIZE = 2;	//���ʶq
	Move[0].SetDelayCount(6);
	Move[1].SetDelayCount(6);
	Attack[0].SetDelayCount(6);
	Attack[1].SetDelayCount(6);
}
void Enemy_son05::LoadBitmap(){
	string temp[13]={"0","1","2","3","4","5","6","7","8","9","10","11","12"};
	string rl[2]={"r","l"};
	Vanish[0].LoadBitmap("Bitmaps/enemy/monky/die_r_3.bmp",RGB(0,255,255));
	Vanish[1].LoadBitmap("Bitmaps/enemy/monky/die_r_3.bmp",RGB(0,255,255));
	for(int j=0;j<2;j++){
		BeHit[j].LoadBitmap((char *)("Bitmaps/enemy/pinkMonky/hit_"+rl[j]+".bmp").c_str(),RGB(0,255,255));
		for(int i=0;i<13;i++){
			if(i < 6)
				Move[j].AddBitmap((char *)("Bitmaps/enemy/pinkMonky/move_"+rl[j]+"_"+temp[i]+".bmp").c_str(),RGB(0,255,255) );
			if(i<4)
				Stand[j].AddBitmap((char *)("Bitmaps/enemy/pinkMonky/stand_"+rl[j]+"_"+temp[i]+".bmp").c_str(),RGB(0,255,255));
			Attack[j].AddBitmap((char *)("Bitmaps/enemy/pinkMonky/attack_"+rl[j]+"_"+temp[i]+".bmp").c_str(),RGB(0,255,255) );
		}
	}
	es02.LoadBitmap();
}
bool Enemy_son05::InArea(Chero *h){//�����d��
	int Area=150;
	int Center = (GetX1()+GetX2())/2; //���Ǫ�X�b�������I
	int hero_center=(h->GetX1()+h->GetX2())/2;
	bool condition1=(GetX2()+Area)>(h->GetX1());
	bool condition2=(GetX1()-Area)<(h->GetX2());
	if( condition1 && condition2 && h->GetY2()<=GetY2() && h->GetY2()>=GetY1()){
		if(Center<= hero_center )
			isLeft=false;					//�P�W
		else
			isLeft=true;
		return true;	
	}
	return false;
}
void Enemy_son05::HitAI(Chero *h,Map *m){//�������A
	int area=150;
	if(!isAttack && InArea(h) ){}	//�d��  �]�w��V
	if( !isAttack && !beHit){	//�S���H�A�S�Q��
		es02.SetIsEndPicture(false);
		if(countHit==HitTime){//���j�ɶ�
			if( (GetX2()+area)>(h->GetX1()) && (GetX1()-area)<(h->GetX2())  && h->GetY2()<=GetY2() && h->GetY2()>=GetY1() && isAttack==false){
				isAttack=true;
				countHit--;
			}
		}
		else if(countHit<HitTime)
			countHit--;
	}
	//-----------------------
	if(countHit<0)
		countHit=HitTime;
	es02.EonMove(this,m);
	if(Attack[0].IsFinalBitmap()||Attack[1].IsFinalBitmap()){ 
		Attack[0].Reset();
		Attack[1].Reset();
		isAttack=false;
	}
}
bool Enemy_son05::isHitHero(Chero *h){ //�����Ϥ���˨�D����Func
	if(h->HitRectangle(es02.GetX1(),es02.GetY1(),es02.GetX2(),es02.GetY2()) && isAttack && GetAttack().GetCurrentBitmapNumber()>=4)
		return true;
	return false;
}
void  Enemy_son05::onShow(Map* m){
	if(isAttack )
		es02.EonShow(this,m);
	Enemy_basic::onShow(m);
}
int Enemy_son05::GetY1(){
	return y+20; //�U�l���Y�Y�y��
}
CAnimation Enemy_son05::GetAttack(){
	if(!isLeft)
		return Attack[0];
	else
		return Attack[1];
}
}//Code Ending