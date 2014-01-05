/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
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
// 這個class為遊戲的遊戲開頭畫面物件
////////////////////////////////////////////////////////////////////////////
CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
	IsClickStart = IsClickHelp = IsClickExit = isHelp = false;
	start_x = 180 , start_y = 50;		//設定start 位置
	help_x = 70 , help_y = 127;			//設定help 位置
	exit_x = 290 , exit_y = 127;		//設定exit 位置
}
void CGameStateInit::OnInit()
{
	ShowInitProgress(0);	// 一開始的loading進度為0%
	// 開始載入資料
	menu.LoadBitmap("Bitmaps/menu/menu.bmp");					//選單背景		
	aboutGame.LoadBitmap("Bitmaps/menu/aboutGame.bmp");			//aboutGame畫面
	start.LoadBitmap("Bitmaps/menu/start.bmp",RGB(0,0,0) );		//start圖片
	help.LoadBitmap("Bitmaps/menu/help.bmp",RGB(0,0,0) );		//help圖片
	exit.LoadBitmap("Bitmaps/menu/exit.bmp",RGB(0,0,0) );		//exit圖片
	bottom.LoadBitmap("Bitmaps/menu/bottom.bmp",RGB(0,0,0) );	//bottom圖片
	CAudio::Instance()->Load(AUDIO_MENU,  "sounds/Maple Story主題曲.wav");	//選單背景音樂
	CAudio::Instance()->Load(AUDIO_decision,  "sounds/Decision.wav");		//選擇確定音效
}
void CGameStateInit::OnBeginState()
{
	CAudio::Instance()->Play(AUDIO_MENU, true);		//播放背景音樂
}
void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ESC = 27;		//ESC鍵
	const char KEY_SPACE = ' ';		//空白鍵
	if (nChar == KEY_SPACE && !isHelp){			//在不是aboutGame畫面按下空白， 切換至GAME_STATE_RUN
		CAudio::Instance()->Play(AUDIO_decision,false);
		CAudio::Instance()->Stop(AUDIO_MENU);
		GotoGameState(GAME_STATE_RUN);			
	}
	else if (nChar == KEY_SPACE)	//aboutGame畫面，按下空白，結束aboutGame
		isHelp=false;
	if (nChar == KEY_ESC && !isHelp)			//在不是aboutGame畫面按下ESC，關閉遊戲的方法
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);		// 關閉遊戲
	else if (nChar == KEY_ESC)		//aboutGame畫面，按下ESC，結束aboutGame
		isHelp=false;
}
void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(isHelp){}	//在aboutGame畫面，皆不進行任何動作
	else if(point.x >= start_x && point.x <= (start_x+start.Width()) && point.y >= start_y && point.y <= (start_y+start.Height()) ){
		IsClickStart = true;							//選擇start
		bottom_x = start_x , bottom_y = start_y;
	}
	else if(point.x >= help_x && point.x <= (help_x+help.Width()) && point.y >= help_y && point.y <= (help_y+help.Height()) ){
		IsClickHelp = true;								//選擇help
		bottom_x = help_x , bottom_y = help_y;
	}
	else if(point.x >= exit_x && point.x <= (exit_x+exit.Width()) && point.y >= exit_y && point.y <= (exit_y+exit.Height()) ){
		IsClickExit = true;								//選擇exit
		bottom_x = exit_x , bottom_y = exit_y;
	}
	else 
		IsClickStart = IsClickHelp = IsClickExit = false;	
	
	if(IsClickStart || IsClickHelp || IsClickExit)
		CAudio::Instance()->Play(AUDIO_decision,false);	//選擇播放音效

	OnShow();
}
void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(isHelp)	//在aboutGame畫面，回到選單畫面
		isHelp = false;
	else if(point.x >= start_x && point.x <= (start_x+start.Width()) && point.y >= start_y && point.y <= (start_y+start.Height()) ){
		CAudio::Instance()->Stop(AUDIO_MENU);		//選擇start，切換至GAME_STATE_RUN
		GotoGameState(GAME_STATE_RUN);				
	}
	else if(point.x >= help_x && point.x <= (help_x+help.Width()) && point.y >= help_y && point.y <= (help_y+help.Height()) ){
		isHelp = true;								//選擇help，切換至aboutGame畫面
	}
	else if(point.x >= exit_x && point.x <= (exit_x+exit.Width()) && point.y >= exit_y && point.y <= (exit_y+exit.Height()) ){
		CAudio::Instance()->Stop(AUDIO_MENU);		//選擇exit，關閉遊戲
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);
	}
	IsClickStart = IsClickHelp = IsClickExit = false;
}
void CGameStateInit::OnMove(){
	static bool flag=false;		//在最一開始時，執行 OnBeginState() ，只執行一次
	if(flag==false){
		OnBeginState();
		flag=true;
	}
}
void CGameStateInit::OnShow()
{
	menu.SetTopLeft(0,0);				//選單背景顯示
	menu.ShowBitmap();
	start.SetTopLeft(start_x,start_y);	//start圖片顯示
	start.ShowBitmap();
	help.SetTopLeft(help_x,help_y);		//helpt圖片顯示
	help.ShowBitmap();
	exit.SetTopLeft(exit_x,exit_y);		//exit圖片顯示
	exit.ShowBitmap();
		if( IsClickStart || IsClickHelp || IsClickExit ){
		bottom.SetTopLeft(bottom_x,bottom_y);	//如果選擇，顯示選擇
		bottom.ShowBitmap();
	}
	if(isHelp){
		aboutGame.SetTopLeft(0,0);		//aboutGame畫面顯示
		aboutGame.ShowBitmap();
	}
}
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Win)
/////////////////////////////////////////////////////////////////////////////
CGameStateWin::CGameStateWin(CGame *g)
: CGameState(g)
{
	m_die.SetDelayCount(30);	//香菇死亡延遲
}
void CGameStateWin::OnBeginState()
{
	CAudio::Instance()->Play(AUDIO_gameWin, true);	//播放背景音樂
	counter = 30 * 51; // 51 seconds
	mx = 368,my = -100;	//初始位置
	tx = 134,ty = my+80;
}
void CGameStateWin::OnInit()
{
	TheEnd.LoadBitmap("Bitmaps/map/TheEnd.bmp",RGB(128,0,255));		//TheEnd圖片
	mushroom.AddBitmap("Bitmaps/menu/mush_0.bmp",RGB(255,0,0) );	//香菇移動圖片
	mushroom.AddBitmap("Bitmaps/menu/mush_1.bmp",RGB(255,0,0) );
	mushroom.AddBitmap("Bitmaps/menu/mush_2.bmp",RGB(255,0,0) );
	m_die.AddBitmap("Bitmaps/menu/die01.bmp",RGB(255,0,0) );		//香菇死掉圖片
	m_die.AddBitmap("Bitmaps/menu/die02.bmp",RGB(255,0,0) );
	m_die.AddBitmap("Bitmaps/menu/die03.bmp",RGB(255,0,0) );
	m_die.AddBitmap("Bitmaps/menu/die04.bmp",RGB(255,0,0) );
	CAudio::Instance()->Load(AUDIO_gameWin,  "sounds/gameWin.wav");	//結束音效
	ShowInitProgress(100);// 最終進度為100%
}
void CGameStateWin::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CAudio::Instance()->Stop(AUDIO_gameWin);	//如果按鍵盤下按鍵，結束跳至GAME_STATE_INIT
	GotoGameState(GAME_STATE_INIT);
}
void CGameStateWin::OnLButtonUp(UINT nFlags, CPoint point)
{
	CAudio::Instance()->Stop(AUDIO_gameWin);	//如果按下滑鼠按鍵，結束跳至GAME_STATE_INIT
	GotoGameState(GAME_STATE_INIT);
}
void CGameStateWin::OnMove()
{
	static int flag=false;
	int num = mushroom.GetCurrentBitmapNumber();
	if(my<=150 && num == 1 && !flag){	//在跳的圖片移動，TheEnd、香菇到達定點不移動
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
		GotoGameState(GAME_STATE_INIT);				//計數結束，自動切回GAME_STATE_INIT
	}
}
void CGameStateWin::OnShow()
{
	mushroom.SetTopLeft(mx,my);	//設定圖片位置
	m_die.SetTopLeft(mx,my);		
	if(counter < 30*44)		//到達時間 換圖顯示
		m_die.OnShow();
	else
		mushroom.OnShow();
	TheEnd.SetTopLeft(tx,ty);
	TheEnd.ShowBitmap();		//TheEnd圖片顯示
}
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////
CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{}
void CGameStateOver::OnMove()
{
	counter--;
	if (counter < 0)		//計數結束換關
		GotoGameState(GAME_STATE_INIT);
}
void CGameStateOver::OnBeginState()
{
	CAudio::Instance()->Play(AUDIO_gameOver, false);//播放背景音樂
	counter = 30 * 5; // 5 seconds
}
void CGameStateOver::OnInit()
{
	ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
		overBitmap.LoadBitmap("Bitmaps/map/gameOver.bmp",RGB(0,0,0));	//gameOver圖示
	CAudio::Instance()->Load(AUDIO_gameOver,  "sounds/gameOver.mid");	//gameOver音效
	ShowInitProgress(100);// 最終進度為100%
}
void CGameStateOver::OnShow()
{
	overBitmap.SetTopLeft(148,188);
	overBitmap.ShowBitmap();//gameOver顯示圖片
}
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////
CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g)
{
	isMovingSP_1=isMovingSP_2=false;//初始
}
CGameStateRun::~CGameStateRun()
{
	//---------------------------------刪除怪物資料
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
	bool over=false;	//gameOver狀態
	bool win=false;		//TheEnd狀態
	//---------------------------------------------------是否結束遊戲
	if(hero.GetLife()==0 && !hero.IsAlive() && IsInit)
		over=true;
	else if( !hero.IsAlive() && IsInit )
		hero.SetLife(hero.GetLife()-1);
	//---------------------------------------------------設置是否初始
	if(!hero.IsAlive() || map.GetIsWin())
		IsInit=false;
	//---------------------------------------------------是否開始初始資料
	int countTime_total = 130;	
	static int countTime = countTime_total;
		if(!IsInit){
		CAudio::Instance()->Stop(AUDIO_map01);		//停止所有音效
		CAudio::Instance()->Stop(AUDIO_map02);
		CAudio::Instance()->Stop(AUDIO_map03);
		CAudio::Instance()->Stop(AUDIO_map04);
		countTime--;
		if(countTime==0 || IsInit==true || over){
			IsInit = true;
			countTime = countTime_total;
		}
		if(over ){		//	over=true 切換至 GameOver
			MapId=0;
			hero.SetLife(2);
			GotoGameState(GAME_STATE_OVER);
		}
	}
	//---------------------------------------------------是否撥放 Loading 畫面
	if( IsInit && (map.GetIsWin() || (hero.IsAlive() && MapId==0)) ){
		hero.SetLife(2);
		for(int i=0;i<5;i++){
			map.changeLoading();
			Sleep(400);
		}
	}
	//---------------------------------------------------人物、地圖、技能初始
	if(IsInit){
		//------------------------關卡切換
		if(map.GetIsWin()==true) MapId++;
		if( MapId > map.GetMapTotal()-1 ) win=true;
		if(win){			
			MapId=0;
			hero.SetLife(2);
			GotoGameState(GAME_STATE_WIN);
		}
		map.SetMapId(MapId);
		//------------------------怪物選擇(關卡)
		enemy.clear();
		enemy=stage[MapId];
		//------------------------地圖init
		map.init();
		//------------------------主角init
		int ix,iy;
		map.GetInit_h_xy(&ix,&iy);
		hero.SetX1(ix),	hero.SetY2(iy);
			//↑設定主角初始位子
		hero.init(&map);
		//------------------------主角技能init
		for(int i=0;i<(int)hero.GetSkill().size();i++)
			hero.GetSkill()[i]->init(&hero,&map);
		//------------------------怪物init
		for(int i=0;i< (int)enemy.size();i++)
			enemy[i]->init(&map);
		//-----------------------------------------------//播放關卡音樂
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
	map.onMove();						//人物、角色地、 移動遊戲元素
	hero.onMove(&map,enemy);
	for(int i=0;i< (int)enemy.size();i++)
		enemy[i]->onMove(&map,&hero);
	//--------------------------------------------是否獲勝(剩餘怪物數量)
	int count=0;
	for(int i=0;i< (int)enemy.size();i++)
		if(enemy[i]->IsAlive())
			count++;
	//---------------------------------------------------顯示 數量/數量total
	num.SetInteger( (int)enemy.size()-count );			
	num_total.SetInteger( map.GetWinEnemy_number() );
	//---------------------------------------------------
	if(isMovingSP_2)		//密技2 補MP到滿
		if( hero.addMP(5) )
			isMovingSP_2=false;
	if( (int)enemy.size()-count >= map.GetWinEnemy_number() ) //顯示here條件
		map.SetIsGoal(true);
	if( ( map.GetIsGoal() && map.IsReach(hero.GetX2(),hero.GetY2()) ) || isMovingSP_1 ){
		isMovingSP_1=false;			//過關條件(含密技1)
		map.SetIsWin(true);	
	}
	//---------------------------------------------------播過關音效
	static bool flag_stage=false;
	if( map.GetIsWin() && !flag_stage){			
		CAudio::Instance()->Play(AUDIO_stageOver, false);
		flag_stage=true;
	}
	else if (!map.GetIsWin() )
		flag_stage=false;
	//---------------------------------------------------是否換關or結束
	if(map.GetIsWin()==true || hero.IsAlive()==false)
		OnBeginState();
	
}
void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	ShowInitProgress(33);		// 接個前一個狀態的進度，此處進度視為33%
	num.LoadBitmap();			//數量
	num_total.LoadBitmap();		//數量總數
	map.LoadBitmap();			//map 讀圖
	hero.LoadBitmap();			//hero 讀圖
	stageOver.LoadBitmap("Bitmaps/map/stageOver.bmp",RGB(0,0,0) );	//stageOver圖片
	slope.LoadBitmap("Bitmaps/number/slope.bmp",RGB(128,0,255));	//協線圖片
	hero.LoadAudio();			//音樂讀檔
	CAudio::Instance()->Load(AUDIO_stageOver,  "sounds/stageOver.mid");	
	CAudio::Instance()->Load(AUDIO_map01,  "sounds/map_01.wav");	
	CAudio::Instance()->Load(AUDIO_map02,  "sounds/map_02.wav");
	CAudio::Instance()->Load(AUDIO_map03,  "sounds/map_03.wav");
	CAudio::Instance()->Load(AUDIO_map04,  "sounds/map_04.wav");
	//-----------------------------------------------------------
	for(int j=0;j< map.GetMapTotal();j++){//讀關卡怪物資料
		string temp[5]={"01","02","03","04","05"};
		map.LoadEnemyData((char *)("File/Data"+temp[j]+".txt").c_str());
		//--------------------建立怪物資料(每關)
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
			stage[j][i]->LoadBitmap();	//怪物圖片
	}
	ShowInitProgress(50);
}
void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY_JUMP	 = ' '; 
	const char KEY_A = 65;	//技能A
	const char KEY_S = 83;	//技能S
	const char KEY_D = 68;	//技能D
	const char KEY_Z = 90;	//攻擊Z
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
	
	if(!hero.IsAttack() && !hero.IsSkill() ){ //在沒攻擊且沒放技能
		if (nChar == KEY_JUMP)
			hero.SetMovingJump(true);//跳躍狀態
		if( nChar == KEY_UP){
			for(int i=0;i<(int)map.GetRope().size();i++)	//如果在繩子範圍   爬繩
				if( map.GetRope()[i]->isClimb(&hero) && !hero.IsClimb())
					hero.SetMovingClimb(true);
		}
	}
	if( !hero.IsClimb()){	//爬繩時，不行攻擊
		if (nChar == KEY_Z )
			hero.SetMovingAttack(true);
		if (nChar == KEY_A )
				hero.SetMovingSkill_A(true);
		if (nChar == KEY_S )
				hero.SetMovingSkill_S(true);
		if (nChar == KEY_D )
				hero.SetMovingSkill_D(true);
	}
	if(nChar == SP1)			//技能
		isMovingSP_1=true;
	if(nChar == SP2)
		isMovingSP_2=true;
}
void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	if (nChar == KEY_LEFT)
		hero.SetMovingLeft(false);
	if (nChar == KEY_RIGHT)
		hero.SetMovingRight(false);
	if (nChar == KEY_DOWN)
		hero.SetMovingDown(false);
	if (nChar == KEY_UP)
		hero.SetMovingUp(false);
}
void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point){}  // 處理滑鼠的動作
void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point){}	// 處理滑鼠的動作
void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point){}  // 處理滑鼠的動作
void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point){}	// 處理滑鼠的動作
void CGameStateRun::OnShow()
{
	map.onShow();					//地圖顯示
	for(int i=0;i<(int)enemy.size();i++)
		enemy[i]->onShow(&map);		//怪物顯示
	hero.onShow(&map);				//主角顯示
	stageOver.SetTopLeft(148,188);
	if( map.GetIsWin() )			
		stageOver.ShowBitmap();		//過關圖示顯示
	//------------------------------
	int ad_x;
	if(num_total.GetInteger()<10)	//調整怪物表位置
		ad_x=40;
	else
		ad_x=0;
	num.SetTopLeft(630+ad_x,10);	//計算怪物表 顯示
	slope.SetTopLeft(630+ad_x,6);
	num_total.SetTopLeft(780,10);
	num.ShowBitmap();
	slope.ShowBitmap();
	num_total.ShowBitmap();
	//--------------------------------------------------顯示測試資料
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[120];								// Demo 數字對字串的轉換
	sprintf(str,"hero_x=%d hero_y=%d",hero.GetX1(),hero.GetY2());
//	pDC->TextOut(5,map.GetPixH()-25,str);
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}
//###################################################################################################################################
//遊戲地圖障礙
Map::Map(){  
	const int Mapid_init=0;//初始關卡設定
	Mapid_now = Mapid_init;
	PixW=800,PixH=600;
	BarrierW = 90;	//地圖裡以陣列來看每一格長多少(點)
	BarrierH = 30;	//地圖裡以陣列來看每一格高多少(點)
	numB_x=0;	//x軸共多少格
	numB_y=0;	//y軸共多少格
	MapTotal=4;		//關卡數設定
	init();
	load.SetDelayCount(1);
	mushroom.SetDelayCount(1);
}
Map::~Map(){
	//------------------------------------------ 刪除空間
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
	//------------------------------------------ 刪除空間
	for(int i=0;i<(int)movebarrier.size();i++)
		delete movebarrier[i];
	movebarrier.clear();
	for(int i=0;i<(int)rope.size();i++)
		delete rope[i];
	rope.clear();
	//------------------------------------------ 讀關卡資料檔
	for(int i=0;i<300;i++)
		for(int j=0;j<300;j++)
			map[i][j]=0;
	string temp[5]={"01","02","03","04","05"};
	for(int i=0;i<5;i++)
		if(i==Mapid_now)
			LoadMapData((char *)("File/Map"+temp[i]+".txt").c_str());
	sx=0;
	sy=200;
	//------------------------------------配置關卡 Movebarrier、Rope 空間
	for(int j=0;j< numB_y;j++)
		for(int i=0;i< numB_x;i++){
			if(map[i][j]==9)//障礙物長條的
				addMovebarrier(3,i*BarrierW,j*BarrierH,0,0,0,0);
			if(map[i][j]==8)//障礙物小個的
				addMovebarrier(1,i*BarrierW,j*BarrierH,0,0,0,0);
			if(map[i][j]==7)//同上
				addMovebarrier(2,i*BarrierW,j*BarrierH,0,0,0,0);
			if(map[i][j]==10)//繩子短的
				addRope(10,i*BarrierW,j*BarrierH);
			if(map[i][j]==11)//繩子稍長的
				addRope(11,i*BarrierW,j*BarrierH);
			if(map[i][j]==12)//繩子長的
				addRope(12,i*BarrierW,j*BarrierH);
			if(map[i][j]==6)//繩子很長的
				addRope(6,i*BarrierW,j*BarrierH);
		}
	//------------------------------------
	SetMovebarrier_Bitmap();	// Movebarrier讀圖
	SetRope_Bitmap();			// Rope讀圖
	MapW = BarrierW * numB_x;	//地圖長寬
	MapH = BarrierH * numB_y;	
}
void Map::LoadEnemyData(const char *fn){	
	for(int i=0;i<50;i++)		//讀取限定關卡 Enemy 資料
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
	//-------------------------------------------/讀取限定關卡 Map 資料	
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
	//-----------------------	Map 陣列資料
	for(ty=0; ty < numB_y ;ty++){
		for(tx=0; tx < numB_x-1 ; tx++){
			getline(ifs,s,',');
			map[tx][ty]=atoi(s.c_str ());
		}
		getline(ifs,s);
		map[tx][ty]=atoi(s.c_str ());
	}
	getline(ifs,s);	//空一行
	//-----------------------	Movebarrier 資料
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
			addMovebarrier(id,data[0],data[1],data[2],data[3],data[4],data[5]);  //配置Movebarrier
		}
	}
	ifs.close();
}
void Map::LoadBitmap(){
	string temp[8]={"0","1","2","3","4","5","6","7"};		
	for(int i=0 ; i <= 5 ; i++)
		floor[i].LoadBitmap((char *)("Bitmaps/map/floor_"+temp[i]+".bmp").c_str(),RGB(128,0,255)); //地板圖示
	for(int i=0 ; i <= 3 ; i++)
		barrier[i].LoadBitmap((char *)("Bitmaps/map/barrier_"+temp[i]+".bmp").c_str(),RGB(128,0,255));//Movebarrier圖示
	for(int i=0 ; i <= 2 ; i++)
		back_map[i].LoadBitmap("Bitmaps/map/back_0.bmp",RGB(0,0,0));//關卡2背景圖
	for(int i=3 ; i <= 4 ; i++)
		back_map[i].LoadBitmap("Bitmaps/map/back_1.bmp",RGB(0,0,0));//關卡3背景圖
	for(int i=5 ; i <= 6 ; i++)
		back_map[i].LoadBitmap("Bitmaps/map/back_2.bmp",RGB(0,0,0));//關卡1背景圖
	for(int i=7 ; i <= 8 ; i++)
		back_map[i].LoadBitmap("Bitmaps/map/back_3.bmp",RGB(0,0,0));//關卡0背景圖
	for(int i=0 ; i <= 7 ; i++)
		Rope_map[i].LoadBitmap((char *)("Bitmaps/rope/rope"+temp[i]+".bmp").c_str(),RGB(128,0,255));//Rope圖示
	changeBitmap.LoadBitmap("Bitmaps/menu/changStage.bmp");//換關Loading畫面
	for(int i=0;i<3;i++){
		load.AddBitmap((char *)("Bitmaps/menu/loading_"+temp[i]+".bmp").c_str(),RGB(255,0,0));//Loading圖示
		mushroom.AddBitmap((char *)("Bitmaps/menu/mush_"+temp[i]+".bmp").c_str(),RGB(255,0,0));//香菇圖示
	}
	goal.LoadBitmap("BitMaps/map/goal.bmp",RGB(0,0,0) );//到達地點圖示
}
void Map::changeLoading() {
	//------------------------------------------------------------------
	CDDraw::BltBackColor(DEFAULT_BG_COLOR);		// 將 Back Plain 塗上預設的顏色
	//----------------------------------------
	changeBitmap.SetTopLeft(0,0);
	changeBitmap.ShowBitmap();		//換關Loading畫面
	load.SetTopLeft(280,270);
	load.OnShow();
	mushroom.SetTopLeft(200,250);
	mushroom.OnShow();
	//----------------------------------------
	CDDraw::BltBackToPrimary();					// 將 Back Plain 貼到螢幕
	//-------------------------------------------------------------------
	load.OnMove();
	mushroom.OnMove();
}
void Map::SetMovebarrier_Bitmap(){
	for(int i=0;i<(int)movebarrier.size();i++){		//依照Movebarrier ID 讀圖
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
	for(int i=0;i<(int)rope.size();i++){			//依照Rope ID 讀圖
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
void Map::addMovebarrier(int fid,int fx,int fy,int fx_area,int fy_area,int fx_speed,int fy_speed){	//配置Movebarrier 空間 (Vector)
	movebarrier.push_back(new MovingBarrier(fid,fx,fy,fx_area,fy_area,fx_speed,fy_speed));
}
void Map::addRope(int r_id,int rx,int ry){	//配置Rope 空間 (Vector)
	rope.push_back(new Rope(r_id,rx,ry));
}
void Map::onMove(){
	for(int i=0;i < (int)movebarrier.size();i++)	//全部Movebarrier的移動要素
		movebarrier[i]->onMove();
}
void Map::onShow(){
	if(Mapid_now==0)		
		for(int i=7;i<=8;i++){				//依照關卡子顯示背景圖
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
	//-------------------------------------------顯示所有的 Movebarrier 
	for(int i=0;i < (int)movebarrier.size();i++)
		movebarrier[i]->onShow(this);
	//-------------------------------------------依照floor ID 印出地板圖
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
	if(isGoal)	//達成條件 印出here
		goal.ShowBitmap();
	for(int i=0;i < (int)rope.size();i++) //顯示所有Rope
		rope[i]->onShow(this);
}
bool Map::IsEmpty(int x, int y){		
	int gx = x / BarrierW;			// 轉換為格座標(整數除法)			
	int gy = y / BarrierH;			// 轉換為格座標(整數除法)	
	return map[gx][gy] == 0;		// 假設0 代表空的
}
bool Map::IsFloor(int x,int y){
	int gx = x / BarrierW;					
	int gy = y / BarrierH;			
	for(int i=0;i < numB_x;i++){	//依照floor ID 判斷該點有無在地板上	
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
	for(int i=0;i < numB_x;i++)			//依照floor ID 判斷該點是否是floor(Wall)	
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
		if( !IsFloor(x,y+count) && !IsMoveBarrier(x,x,y+count) ){}//判斷該點下面有無floor、Movebarrier可踩
		else break;
	}
	if( y+count >= GetMapH())
		return true;
	else
		return false;
}
bool Map::IsMoveBarrier(int x1,int x2,int y){

	for(int i=0;i<(int)movebarrier.size();i++)	//利用迴圈判斷x1,x2,y有無在所有Movebarrier中的其中一個上
		if(x1>=movebarrier[i]->GetX1() && x2<=movebarrier[i]->GetX2())
			if(y==movebarrier[i]->GetY())
				return true;
	
/*	for(int i=0;i<(int)movebarrier.size();i++)			//此段用於修正(x座標範圍太大者)，在Movebarrier到Movebarrier之間的模糊地帶
		for(int j=0;j<(int)movebarrier.size();j++)		//但加上這段後，會造成遊戲不順暢(迴圈太多層!)，故以設計地圖時，避免此狀況
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
	int gx = x / BarrierW;					//該點是否於過關地點
	int gy = y / BarrierH;
	if(map[gx][gy]==20 || map[gx][gy-1]==20 || map[gx][gy+1]==20)
		return true;
	else 
		return false;
}
int Map::ScreenX(int x){			// x 為地圖的點座標
	return x - sx;				// 回傳螢幕的x 點座標
}
int Map::ScreenY(int y) {			// y 為地圖的y 點座標
	return y - sy;				// 回傳螢幕的點座標
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
MovingBarrier::MovingBarrier(int fid,int fx,int fy,int fx_area,int fy_area,int fx_speed,int fy_speed){ //f_speed: 檔案的x,y斜率計算or速度 
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
	barrier = Bitmap;		//讀圖
}
void MovingBarrier::onShow(Map *m){	
	int dy=0;	//依照MovingBarrier ID 微調y座標
	if(id==0) dy=7;
	else if(id==3) dy=10;
	else if(id==1 || id==2) dy=2;
	barrier->SetTopLeft(m->ScreenX(x),m->ScreenY(y-dy));
	barrier->ShowBitmap ();
}
void MovingBarrier::onMove (){	//Movebarrier的移動要素
	if(GetFx_area()>0){
		if(x<this->GetFx()+this->GetFx_area() && is_end[0]){ //此時座標 < 開始座標+範圍(正)
			x+=x_speed;
			if(x>=this->GetFx()+this->GetFx_area()) is_end[0]=false;	//超過 換邊
		}
		else if(x>this->GetFx() && !is_end[0]){		//此時座標 > 開始座標 && 換邊
			x-=x_speed;
			if(x<=this->GetFx()) is_end[0]=true;	//低於 換邊
		}
	}
	else{
		if(x>this->GetFx()+this->GetFx_area() && is_end[0]){ //此於上述動作相同(正副號不同而已)
			x+=x_speed;
			if(x<=this->GetFx()+this->GetFx_area()) is_end[0]=false;
		}
		else if(x<this->GetFx() && !is_end[0]){
			x-=x_speed;
			if(x>=this->GetFx()) is_end[0]=true;
		}	
	}
	x_change = x - last_x;	//用於人物踩障礙物移動
	last_x=x;
	//-------------------
	if(GetFy_area()>0){
		if(y<this->GetFy()+this->GetFy_area() && is_end[1]){	//同上
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
const int MovingBarrier::GetFx(){return fix_x;}//取得固定的x
const int MovingBarrier::GetFy(){return fix_y;}//取得固定的y
const int  MovingBarrier::GetFx_area(){return x_area;} //取得固定的xoffest
const int  MovingBarrier::GetFy_area(){return y_area;} //取得固定的yoffest
bool MovingBarrier::HitRectangle(int tx1,int tx2, int ty){
	int x1 = GetX1();	// 左 x座標
	int x2 = GetX2();	// 右 x座標
	int y = GetY();			
	return (tx2 >= x1 && tx1 <= x2 && ty == y );	// 檢測球的矩形與參數矩形是否有交集
}
//###################################################################################################################################
Rope::Rope(int id,int rx,int ry){
	r_id =id;
	x=rx+18;
	y=ry-30+1;
}
void Rope::SetBitmap(CMovingBitmap *Bitmap){	//同上
	bitmap=Bitmap;
}
void Rope::onShow(Map *m){
	bitmap->SetTopLeft(m->ScreenX(x),m->ScreenY(y));
	bitmap->ShowBitmap();
}
bool Rope::isClimb(Chero *h){	//主角是否有碰到繩子
	return HitRectangle(h->GetClimb_x1(),h->GetClimb_y1(),h->GetClimb_x2(),h->GetY2()-30);
}
int Rope::GetId(){return r_id;}
int Rope::GetX1(){return x;}
int Rope::GetX2(){return x + bitmap->Width();}
int Rope::GetY1(){return y;}
int Rope::GetY2(){return y + bitmap->Height();}
bool Rope::HitRectangle(int tx1, int ty1, int tx2, int ty2){	// 是否碰到參數範圍的矩形
	int x1 = GetX1();				// 球的左上角x座標
	int y1 = GetY1();				// 球的左上角y座標
	int x2 = GetX2();	// 球的右下角x座標
	int y2 = GetY2();	// 球的右下角y座標
	return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);	// 檢測球的矩形與參數矩形是否有交集
}
//###################################################################################################################################
//遊戲主角
Chero::Chero(){
	climb_x1=50,climb_x2=75;	//繩子 判斷範圍
	left_x=40,right_x=90;		//人物左邊，人物右邊 座標
	height=84;
	HP_total=HP=100;
	MP_total=200;
	MP=150;
	life=2;
	addSkill();
}
Chero::~Chero(){
	//---------------------------------刪除人物技能
	for(int i=0;i<(int)hero_skill.size();i++)
		delete hero_skill[i];
	hero_skill.clear();
	//---------------------------------
}
void Chero::addSkill(){
	hero_skill.push_back (new Skill_son01);//配置 skill 空間 (vector)
	hero_skill.push_back (new Skill_son02);
	hero_skill.push_back (new Skill_son03);
	hero_skill.push_back (new Skill_son04);
	hero_skill.push_back (new Skill_son05);
}
void Chero::init(Map *m){
	const int INITIAL_VELOCITY = 14;	// 初始上升速度
	const int INITIAL_DOWNVELOCITY =1;	// 初始下降速度
	Velocity = init_Velocity = INITIAL_VELOCITY;
	downVelocity = init_downVelocity = INITIAL_DOWNVELOCITY;
	countBeHit = superTime = 30;	//約   (X 除30) 秒	 無敵時間:受傷時有短暫的時間已免一直被怪物打
	countHit = attackTime = 10;	//硬直時間約(X 除30) 秒
	//--------------------------
	isMovingLeft = isMovingRight= isMovingUp = isMovingDown = false;
	isMovingAttack = isMovingSkill_A = isMovingSkill_S = isMovingSkill_D = isMovingClimb = isMovingJump= false;
	hit[0]=hit[1]=hit[2]=false;
	isLeft=beHit=isSkill=isClimb=false;		//isLeft:移動時如果停下來的時候要面向移動時的方向 如:像又停下來時即面向右
	is_alive = true;
	//--------------------------	
	HP=100,MP=150;
	//攻擊的圖片時間縮改
	for(int i=0;i< 2;i++){			//i=0為右,i=1為左
		Attack1[i].SetDelayCount(4);//段數1 
		Attack2[i].SetDelayCount(4);//段數2
		Attack3[i].SetDelayCount(4);//段數3
	}
	//------------------
	Walk[0].SetDelayCount(6);
	Walk[1].SetDelayCount(6);
	climb_Rope.SetDelayCount(8);
	//-------------------------讓人物一開始就站在地上
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
		y -= (Velocity)--;		// y軸上升(移動velocity個點)//受重力影響，下次的上升速度降低
	else
		onfloor(m);
	for(;GetY2() < m->GetSY()+(m->GetPixH()/2)-20;)		//-20為螢幕的調整,當螢幕超過一半時,避免跳到障礙物後時,不會有螢幕震地(往下)的感覺 		
		m->SetSY(-1);					//迴圈來扣減螢幕的移動
}
void Chero::climb(Map *m,bool *Climb_down){
	int i;
	const int STEP_SIZE=4;	//爬繩速度
	vector<Rope *> rope=m->GetRope();
	for(i=0;i<(int)rope.size();i++){		//判斷爬哪個繩子
		if( rope[i]->isClimb(this) && isClimb ){
			SetX( rope[i]->GetX1()-53-x );	//設x座標(對應繩子)
			break;
		}
	}
	if(i!=(int)rope.size()){		//有爬到繩子
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
	for(;GetY2() > m->GetSY()+(m->GetPixH()/2) && GetY2() < m->GetMapH()-(m->GetPixH()/2)+70;)//+70 讓主角銀幕位置至於較中間點
		m->SetSY(1);
	for(;GetY2() < m->GetSY()+(m->GetPixH()/2)-20;)
		m->SetSY(-1);
}
void Chero::onfloor(Map *m){
	const int foot1 = GetX1()+20;	//60
	const int foot2 = GetX2()-20;	//70
	vector<MovingBarrier *>barrier=m->GetMovingBarrier();
	Velocity = 0;	//在下降階段不能跳
	int count_floor=0;	//初始劃離地高度
	int number;//用來記住踩在哪個movebarrier上
	//------------------------------------------------------------------------下跳
	if( isMovingDown && IsTouchBarrier(barrier,count_floor,&number))
		y+=1;					//踩在Movearrier上 && 有按下
	SetMovingDown(false);
	//------------------------------------------------------------------------計算離地高度
	for(count_floor=0 ; GetY2()+count_floor<=m->GetMapH()+100 ; count_floor++)	
		if( !m->IsFloor( foot1,GetY2()+count_floor) && !m->IsFloor( foot2,GetY2()+count_floor) && !IsTouchBarrier(barrier,count_floor,&number) ){}
		else break;
	//------------------------------------------------------------------------
	if (count_floor >= downVelocity)
		y += (downVelocity)++;		// y軸下降(移動velocity個點)// 受重力影響，下次的下降速度增加
	else{
		y +=count_floor;	//一次剛好到地
		SetMovingJump(false);
		Velocity = init_Velocity;		// 重設上升初始速度
		downVelocity = init_downVelocity;	// 重設下降初始速度	
	}
	//-----------------------------------------------------------------------跟著障礙物移動
	int dx,dy,adjust=0;
	if( IsTouchBarrier(barrier,0,&number) ){//number:站在哪個障礙物上
		dx= barrier[number]->GetXchange();
		dy= barrier[number]->GetYchange();
		if(dy>=0)	adjust=0;	//Movebarrier上移
		else	adjust=dy;	//Movebarrier下移
	}
	if( IsTouchBarrier(barrier,adjust,&number) ){
		x+=dx;
		y+=dy;
	}
	//------------------------------------------------------------------------銀幕移動
	for(;GetY2() > m->GetSY()+(m->GetPixH()/2) && GetY2() < m->GetMapH()-(m->GetPixH()/2)+70;)//+70 讓主角銀幕位置至於較中間點
		m->SetSY(1);
	for(;GetY2() < m->GetSY()+(m->GetPixH()/2)-20;)
		m->SetSY(-1);
}
bool Chero::IsTouchBarrier(vector<MovingBarrier *> b,int count,int *number){
	const int foot1 = GetX1()+20;	
	const int foot2 = GetX2()-20;	
	for(int i=0;i<(int)b.size();i++){ //跟所有Movebarrier判斷
		if( foot1 >= b[i]->GetX1() && foot2 <= b[i]->GetX2()){	//在x範圍內 
			if( GetY2()+count == b[i]->GetY() ){	//y不會變的時候(左右)
				*number=i;
				return true;
			}
			if( GetY2()+count+b[i]->GetYchange() == b[i]->GetY() ){ //y會改變的時候(上下)
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
				Alert[j].AddBitmap( (char *)("Bitmaps/hero/alert_"+rl[j]+temp[i]+".bmp").c_str(),RGB(128,0,255) );	//警戒
				Walk[j].AddBitmap( (char *)("Bitmaps/hero/walk_"+rl[j]+temp[i]+".bmp").c_str(),RGB(128,0,255) );	//走路
				Attack1[j].AddBitmap( (char *)("Bitmaps/hero/hit_"+rl[j]+"1"+temp[i]+".bmp").c_str(), RGB(128,0,255));	//第一段打擊
				Attack2[j].AddBitmap( (char *)("Bitmaps/hero/hit_"+rl[j]+"2"+temp[i]+".bmp").c_str(), RGB(128,0,255));	//第二段打擊
				Attack3[j].AddBitmap( (char *)("Bitmaps/hero/hit_"+rl[j]+"3"+temp[i]+".bmp").c_str(), RGB(128,0,255));	//第三段打擊
			}
			else{
				Alert[j].AddBitmap( (char *)("Bitmaps/hero/alert_"+rl[j]+temp[i]+".bmp").c_str(),RGB(128,0,255) );	//警戒
				Attack3[j].AddBitmap( (char *)("Bitmaps/hero/hit_"+rl[j]+"3"+temp[i]+".bmp").c_str(), RGB(128,0,255));	//第三段打擊
			}
		}
	}
	climb_Rope.AddBitmap("Bitmaps/hero/Rope_0.bmp",RGB(128,0,255));//爬繩
	climb_Rope.AddBitmap("Bitmaps/hero/Rope_1.bmp",RGB(128,0,255));
	lifeBitmap.LoadBitmap("Bitmaps/hero/life/heart.bmp",RGB(0,0,255));	//愛心
	life_0.LoadBitmap("Bitmaps/hero/life/0.bmp",RGB(0,0,255));	//數字0
	life_1.LoadBitmap("Bitmaps/hero/life/1.bmp",RGB(0,0,255));	//數字1
	life_2.LoadBitmap("Bitmaps/hero/life/2.bmp",RGB(0,0,255));	//數字2
	for(int i=0; i < (int)hero_skill.size() ;i++)//技能讀圖
		hero_skill[i]->LoadBitmap();
}
void Chero::LoadAudio(){//讀音效
	CAudio::Instance()->Load(AUDIO_Jump,  "sounds/Jump.wav");	
	CAudio::Instance()->Load(AUDIO_hit,  "sounds/hit.wav");
	CAudio::Instance()->Load(AUDIO_skill,  "sounds/skill.wav");
	CAudio::Instance()->Load(AUDIO_bomb,  "sounds/bomb.wav");
	CAudio::Instance()->Load(AUDIO_die,  "sounds/die.mid");
}
void Chero::AttackEnemy(Map *m){ //攻擊敵人
//    //--------------------------------------------------------------	
	if(Attack1[0].IsFinalBitmap())			//攻擊動作完結，才能再打
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
//    //--------------------------------------------------------------判段false (打完時)
	if( Attack1[0].IsFinalBitmap()||Attack1[1].IsFinalBitmap() ) 
		hit[0]=false;
	if( Attack2[0].IsFinalBitmap()||Attack2[1].IsFinalBitmap() ) 
		hit[1]=false;
	if( Attack3[0].IsFinalBitmap()||Attack3[1].IsFinalBitmap() ) 
		hit[2]=false;	
//    //--------------------------------------------------------------技能時，段數攻擊皆歸零
	if( IsSkill() ){
		for(int i=0;i<2;i++){
			Attack1[i].Reset();
			Attack2[i].Reset();
			Attack3[i].Reset();
			hit[i]=false;
		}
		hit[2]=false;//0,1,2 三招重設(加上行)
		countHit = attackTime;
		SetMovingAttack(false);
	}
//    //--------------------------------------------------------------攻擊硬直時間
	if(countHit < attackTime && hit[2]==false){
		countHit--;
		SetMovingAttack(false);
	}
	if(countHit==0)
		countHit = attackTime;
//    //--------------------------------------------------------------三連段攻擊判斷
	if(isSkill){}
	else if( isMovingAttack && hit[0]==false && hit[1]==false && hit[2]==false){
		hit[0]=true;
		SetMovingAttack(false);			
	}
	else if(hit[0]){	//攻擊判斷,攻擊中，圖片才動
		Attack1[0].OnMove();
		Attack1[1].OnMove();
		
		if(isMovingAttack){
			hit[1]=true;
		}
		SetMovingAttack(false);
	}
	else if(hit[1]){	//攻擊判斷,攻擊中，圖片才動
		Attack2[0].OnMove();
		Attack2[1].OnMove();
		
		if(isMovingAttack){
			countHit--;
			hit[2]=true;
		}
		SetMovingAttack(false);
	}
	else if(hit[2]){	//攻擊判斷,攻擊中，圖片才動
		Attack3[0].OnMove();
		Attack3[1].OnMove();
	}
//    //--------------------------------------------------------------技能扣 MP
	int number;
	vector<MovingBarrier *>barrier=m->GetMovingBarrier();
	if(!isSkill){
		if(isMovingSkill_A && !hero_skill[3]->IsRun() && MP>=40){//用來判斷是否放技能
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
void Chero::onMove(Map *m,vector<Enemy_basic *> enemy){ // 注意：這是hero，不是map class
	const int STEP_SIZE = 7;
	const int center = ( GetX1()+GetX2() )/2;
	static int temp=0;
	Alert[0].OnMove();
	Alert[1].OnMove();
	if( IsAlive() ){
		Skill_now();
//被攻擊//--------------------------------------------------------------
		for(int i=0;i<(int)enemy.size();i++)	//用迴圈來讀同一種物件裡每一個敵人的記憶體位置 +i指向下一是個連續位置
			if( enemy[i]->IsAlive() && !isSkill)
				beHit_by_Enemy(enemy[i]);//被攻擊function
		if(beHit)
			countBeHit--;
		if(countBeHit==0 && HP>0){
			beHit=false;	//表示無敵時間已結束
			countBeHit=superTime;	//回到初始值
		}
		//----------------------------------------------死亡條件
		if( GetY2()>=m->GetMapH()+100 ) HP=0;
		if(HP<=0 ) 
			SetIsAlive(false);	//死亡
		//----------------------------------------------死亡音效
		static bool flag_die=false;
		if( !is_alive && !flag_die){
			CAudio::Instance()->Play(AUDIO_die, false);
			flag_die=true;
		}
		else if (is_alive )
			flag_die=false;
// 攻擊 //--------------------------------------------------------------
		AttackEnemy(m);//攻擊function
		//集MP-------------------------------------------
		for(int j=0;j<(int)enemy.size();j++)
			for(int i=0;i<(int)hero_skill.size();i++)
				if(enemy[j]->IsbeHit() && enemy[j]->IsAlive() && !IsSkill())
					if(MP<=MP_total){
						temp+=1;
						MP+=temp/5;
						if(temp >5) temp=0;
					}
// 移動 //--------------------------------------------------------------
		last_x = x;			//取上一座標，判斷座標(左右)貼圖
		//右移動
		if( (hit[0] || hit[1] || hit[2] || isSkill) && isMovingJump==false && m->IsEmpty(GetX2(),GetY2()-1) ){}	//地面攻擊時，不可往右移動	不會出現(攻擊圖片狀態滑動)			
		else if (isMovingRight && GetX2()< m->GetMapW() && !m->IsWall( GetX2(),GetY2()-1) ){		//按→ (右邊沒牆壁)
			Walk[0].OnMove();
			x += STEP_SIZE;
		}
		//左移動
		if( (hit[0] || hit[1] || hit[2] || isSkill) && isMovingJump==false){}	//地面攻擊時，不可往右移動	不會出現(攻擊圖片狀態滑動)			
		else if (isMovingLeft && GetX1() > 0 && !m->IsWall( GetX1(),GetY2()-1) ){		//按← (左邊沒牆壁)
			Walk[1].OnMove();
			x -= STEP_SIZE;
		}
// 爬繩 //--------------------------------------------------------------
		last_y=y;		//取上一座標，判斷座標(跳)貼圖
		if(isMovingClimb && !isClimb){
			isClimb=true;	//有按上&&原本沒爬
			SetMovingClimb(false); //按鍵設回 初始
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
// 跳耀 //--------------------------------------------------------------
		if (isMovingJump)				//跳耀
			jump(m);
		else if( !isClimb )					//踩地
			onfloor(m);
		
		if(Velocity == init_Velocity-1)
			CAudio::Instance()->Play(AUDIO_Jump, false);
// 招示 // -------------------------------------------------------------
		for(int i=0;i < (int)hero_skill.size();i++)
			hero_skill[i]->onMove(this,m);
// 調整 // -------------------------------------------------------------
// 銀幕 //
		for(; center > m->GetSX()+(m->GetPixW()/2) && center < m->GetMapW()-(m->GetPixW()/2);)	//螢幕跟著調整	
			m->SetSX(1);
		for(; center < m->GetSX()+(m->GetPixW()/2) && center > (m->GetPixW()/2);)		
			m->SetSX(-1);
	}	
}
void Chero::onShow(Map *m) // 注意：這是hero，不是map class
{
	int number=0;
// 血量 //--------------------------------------------------------------
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
// 掛掉 //--------------------------------------------------------------
	if( !IsAlive()){} //掛掉，人物不印出
// 被打 //--------------------------------------------------------------
	else if(beHit && countBeHit%4 == 0){}//countBeHit %4 ==0 --->%4(閃爍速度，越大越慢) countBeHit扣減時取能使餘數為0的值並在那個時候閃爍圖片一次	
// 技能 //--------------------------------------------------------------
	else if(hero_skill[3]->IsRun()){}
	else if(hero_skill[4]->IsRun()){}
// 攻擊 //--------------------------------------------------------------
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
// 爬繩 //--------------------------------------------------------------
	else if(isClimb){
		climb_Rope.SetTopLeft(m->ScreenX(x), m->ScreenY(y));
		climb_Rope.OnShow();
	}
// 跳耀 //--------------------------------------------------------------
	else if(!(IsTouchBarrier(m->GetMovingBarrier(),0,&number)) && !isLeft && last_y!=y && x>=last_x){	//同時按跳和角色位在右邊(不在左邊)
		Jump[0].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
		Jump[0].ShowBitmap();
	}
	else if(!(IsTouchBarrier(m->GetMovingBarrier(),0,&number)) &&  isLeft && last_y!=y && x<=last_x){	//是否面向左邊同時判斷有無按跳
		Jump[1].SetTopLeft(m->ScreenX(x), m->ScreenY(y));
		Jump[1].ShowBitmap();
	}
// 移動 //--------------------------------------------------------------
	else if(x>last_x && isMovingRight){											//在右邊移動
		Walk[0].SetTopLeft(m->ScreenX(x), m->ScreenY(y)); 
		Walk[0].OnShow();
		isLeft=false;
	}
	else if(x<last_x && isMovingLeft){											//在左邊移動
		Walk[1].SetTopLeft(m->ScreenX(x), m->ScreenY(y)); 
		Walk[1].OnShow();
		isLeft=true;
	}
// 原地 //--------------------------------------------------------------
	else if(!isLeft){								//面向右邊 座標沒移動
		Alert[0].SetTopLeft(m->ScreenX(x), m->ScreenY(y)); 
		Alert[0].OnShow();
	}
	else if( isLeft){								//面向左邊 座標沒移動
		Alert[1].SetTopLeft(m->ScreenX(x), m->ScreenY(y)); 
		Alert[1].OnShow();
	}
// 技能 //--------------------------------------------------------------
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
//------------------------------------第一段攻擊	
	if(!isLeft && hit[0])
		return Attack1[0];
	else if(hit[0])
		return Attack1[1];
//------------------------------------第二段攻擊	
	else if(!isLeft && hit[1])
		return Attack2[0];
	else if(hit[1])
		return Attack2[1];
//------------------------------------第三段攻擊
	else if(!isLeft && hit[2])
		return Attack3[0];
	else 
		return Attack3[1];	
}
void Chero::beHit_by_Enemy(Enemy_basic *e){
	bool condition=HitRectangle(e->GetX1(), e->GetY1(),e->GetX2(), e->GetY2());	//碰到怪
	int hit_HP=0;
	if(countBeHit == superTime && !beHit){ //依照Enemy ID 判斷主角扣血
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
		//----------------有被打到 hit_HP!=0
		if(hit_HP!=0){
			HP += hit_HP;
			beHit=true;
			countBeHit--;
		}
	}
}
void Chero::ShowHP(int percent,Map *m){ //顯示血條
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
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CPen *pp, p(PS_NULL, 0, RGB(0,0,0));		// 清除pen
	pp = pDC->SelectObject(&p);
	CBrush *pb, b(RGB(110,100,80));				// 畫 progress框
	pb = pDC->SelectObject(&b);
	pDC->Rectangle(x1,y1,x2,y2);				
	CBrush b1(RGB(215,197,183) );				// 畫 progrss中心
	pDC->SelectObject(&b1);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2_end,progress_y2);
	CBrush b2(RGB(245,45,45));					// 畫 progrss進度
	pDC->SelectObject(&b2);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2,progress_y2);
	pDC->SelectObject(pp);						// 釋放 pen
	pDC->SelectObject(pb);						// 釋放 brush
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}
void Chero::ShowMP(int percent,Map *m){ //顯示MP條
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
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CPen *pp, p(PS_NULL, 0, RGB(0,0,0));		// 清除pen
	pp = pDC->SelectObject(&p);
	CBrush *pb, b(RGB(110,100,80));				// 畫 progress框
	pb = pDC->SelectObject(&b);
	pDC->Rectangle(x1,y1,x2,y2);				
	CBrush b1(RGB(214,215,250) );				// 畫 progrss中心
	pDC->SelectObject(&b1);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2_end,progress_y2);
	CBrush b2(RGB(31,26,226));					// 畫 progrss進度
	pDC->SelectObject(&b2);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2,progress_y2);
	pDC->SelectObject(pp);						// 釋放 pen
	pDC->SelectObject(pb);						// 釋放 brush
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}
bool Chero::HitRectangle(int tx1, int ty1, int tx2, int ty2){	// 是否碰到參數範圍的矩形
	int x1 = GetX1() + 20;				// 球的左上角x座標
	int y1 = GetY1() + 20;				// 球的左上角y座標
	int x2 = GetX2() - 20;	// 球的右下角x座標
	int y2 = GetY2() - 20;	// 球的右下角y座標
	return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);// 檢測球的矩形與參數矩形是否有交集
}
//###################################################################################################################################
//技能
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
	int x1 = GetX1();	// 球的左上角x座標
	int y1 = GetY1();	// 球的左上角y座標
	int x2 = GetX2();	// 球的右下角x座標
	int y2 = GetY2();	// 球的右下角y座標
	return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}
//--------------------------------------------------------------------------技能01(打擊1)
Skill_son01::Skill_son01(){}
void Skill_son01::LoadBitmap(){
	hit[0].LoadBitmap("Bitmaps/hero/hit_0.bmp",RGB(128,0,255));
	hit[1].LoadBitmap("Bitmaps/hero/hit_1.bmp",RGB(128,0,255));	
}
void Skill_son01::onMove(Chero *h,Map *m){
	if(!h->IsLeft() )					//主角相對位置
		SetX1(h->GetX1()-40+45);
	else if(h->IsLeft() )
		SetX1(h->GetX1()-40-10);
	SetY1(h->GetY1()-20+38);
}
void Skill_son01::onShow(Chero *h,Map *m){
	bool flag_hit=false;		//條件印出
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
	//-----------------------------------音效
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
		SetX1(h->GetX1()-40+98);	//同上
	else if(h->IsLeft() )
		SetX1(h->GetX1()-40-18);
	SetY1(h->GetY1()-20+54);
	//-----------------------------------會根據你的招式移動
	if(h->IsAttack()==2){
		if(!h->IsLeft() && h->GetX2() < m->GetMapW() && !m->IsWall(h->GetX2(),h->GetY2()-1) ){
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==1 && !m->IsNoland(h->GetX2()+15/4,h->GetY2()) )//突刺移動(右)
				h->SetX(15/4);
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==2 && !m->IsNoland(h->GetX2()+25/4,h->GetY2()) ) 
				h->SetX(25/4);
		}
		else if(h->IsLeft() && h->GetX1() > 0 && !m->IsWall(h->GetX1(),h->GetY2()-1) ){			
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==1 && !m->IsNoland(h->GetX1()-15/4,h->GetY2()) )//突刺移動(左)
				h->SetX(-15/4);
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==2 && !m->IsNoland(h->GetX1()-25/4,h->GetY2()) )
				h->SetX(-25/4);	
		}
	}
}
void Skill_son02::onShow(Chero *h,Map *m){
	bool flag_hit=false;			//條件印出
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
	//-----------------------------------音效
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
		SetX1(h->GetX1()-40+50);	//同上
	else if(h->IsLeft() )
		SetX1(h->GetX1()-40-13);
	SetY1(h->GetY1()-20+38);
	//-----------------------------------會根據你的招式移動
	if(h->IsAttack()==3){
		if(!h->IsLeft() && h->GetX2() < m->GetMapW() && !m->IsWall(h->GetX2(),h->GetY2()-1) ){
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==2 && !m->IsNoland(h->GetX2()+16/4,h->GetY2()) )//突刺移動(右)
				h->SetX(16/4);
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==3 && !m->IsNoland(h->GetX2()+15/4,h->GetY2()) ) 
				h->SetX(16/4);
		}
		else if(h->IsLeft() && h->GetX1() > 0 && !m->IsWall(h->GetX1(),h->GetY2()-1) ){			
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==2 && !m->IsNoland(h->GetX1()-16/4,h->GetY2()))//突刺移動(左)
				h->SetX(-16/4);
			if( h->GetHitAnimation().GetCurrentBitmapNumber()==3 && !m->IsNoland(h->GetX1()-16/4,h->GetY2()) )
				h->SetX(-16/4);	
		}
	}
}
void Skill_son03::onShow(Chero *h,Map *m){	
	bool flag_hit=false;			//條件印出
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
	//-----------------------------------音效
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
	for(int j=0;j<2;j++){			//讀圖
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
		for(int i=0;i<2;i++)	//在攻擊時，攻擊onMove
			hit[i].OnMove();
	if(hit[0].GetCurrentBitmapNumber()<27){ //大於第27張時，不改變x座標
		if(!h->IsLeft() )
			SetX1(h->GetX1()-40-30);
		else if(h->IsLeft() )
			SetX1(h->GetX1()-40-32);
	}		//一直與主角維持一樣高度
	SetY1(h->GetY1()-20-37);	
	//-----------------------------------設置圖片播放速度
	for(int i=0;i<2;i++){
		if(hit[i].GetCurrentBitmapNumber()>=10 && hit[i].GetCurrentBitmapNumber()<=24)
			hit[i].SetDelayCount(1);
		else
			hit[i].SetDelayCount(2);
	}
	//----------------------------------邊打移動
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
	//-----------------------------------技能圖示(可否使用技能)
	if(menu.GetCurrentBitmapNumber()==0 && h->GetMP()>=40 && !h->IsSkill())
		menu.OnMove();
	else if(menu.GetCurrentBitmapNumber()!=0 && h->IsSkill()){
		menu.OnMove();
		if(!isRun)
			menu.Reset();
	}
	//-----------------------------------音效
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
	int now_num=hit[0].GetCurrentBitmapNumber();//技能打到怪物扣血狀況
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
	for(int i=0;i<2;i++){		//依序判斷炸彈0,1,2 扣血狀況
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
	if( bomb2[0].IsFinalBitmap() ){	//設初始
		for(int i=0;i<2;i++){
			hit[i].Reset();
			bomb0[i].Reset();
			bomb1[i].Reset();
			bomb2[i].Reset();
		}
		isRun=false;
	}
	num_past=bomb2[0].GetCurrentBitmapNumber();
	//------------------------與人物相對位置
	SetX1(h->GetX1()-40+41);
	SetY1(h->GetY2()-96);
	//------------------------放技能時
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
	//------------------------技能圖示 同上
	if(menu.GetCurrentBitmapNumber()==0 && h->GetMP()>=40 && !h->IsSkill())
		menu.OnMove();
	else if(menu.GetCurrentBitmapNumber()!=0 && h->IsSkill()){
		menu.OnMove();
		if(!isRun)
			menu.Reset();
	}
	//------------------------音效
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
	//----------------設定爆炸圖間隔
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
	for(int i=0;i<2;i++){	//設置炸彈位置
		hit[i].SetTopLeft(hx,hy);
		bomb0[i].SetTopLeft(hx+bx0[i],hy+by);
		bomb1[i].SetTopLeft(hx+bx1[i],hy+by);
		bomb2[i].SetTopLeft(hx+bx2[i],hy+by);
	}
	//--------------------------有放技能時 顯示
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
	//--------------------------技能圖示
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
	//--------------------------------------------------初始
	if(hit[0].IsFinalBitmap()||hit[1].IsFinalBitmap()){
		IsEndPicture = true;
		hit[0].Reset();
		hit[1].Reset();
	}
	//--------------------------------------------------香蕉 移動
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
		if(temp1 || temp2)//香蕉加重力
			onfloor(m ,e03);
	}
	else {
		downVelocity = init_downVelocity;	// 重設下降初始速度	
		if(e03->GetIsLeft())
			this->SetX1(e03->GetX1()-40);
		else
			this->SetX1(e03->GetX2()+40-18); //為了要和左邊的圖對稱,所以要扣掉香蕉的寬度
		this->SetY1(e03->GetY1()-30);//常數值為圖片落地的座標,每張皆依樣高,即便其他張的物體比這張高或低
	}
}
void EnemySkill_son01::onfloor(Map *m,Enemy_basic * e03){
	int count_floor=0;	//初始劃離地高度
	for(count_floor=0 ; ; count_floor++)
		if( m->IsEmpty( (x),(GetY2())+count_floor) ){}
		else break;
	if (count_floor >= downVelocity)
		y += (downVelocity)++;		// y軸下降(移動velocity個點)// 受重力影響，下次的下降速度增加
	else{
		y +=count_floor;	//一次剛好到地
		downVelocity = init_downVelocity;	// 重設下降初始速度	
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
	if(!e03->GetIsLeft() && !IsEndPicture && temp){  //往右丟香蕉
		hit[0].SetTopLeft(esx,esy);
		hit[0].OnShow();
	}
	else if(e03->GetIsLeft() && !IsEndPicture && temp){	//往左丟香蕉
		hit[1].SetTopLeft(esx,esy);
		hit[1].OnShow();
	}
	else{
		Vanish[0].SetTopLeft(esx,esy);	//死掉
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
	//-----------------------------------	初始
	if(hit[0].IsFinalBitmap()||hit[1].IsFinalBitmap()){
		IsEndPicture = true;
		hit[0].Reset();
		hit[1].Reset();
	}
	//-----------------------------------	水蜜桃 移動
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
			this->SetX1(e05->GetX2()+23); //為了要和左邊的圖對稱,所以要扣掉香蕉的寬度
		this->SetY1(e05->GetY1()+25);//常數值為圖片落地的座標,每張皆依樣高,即便其他張的物體比這張高或低
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
	if(!e05->GetIsLeft() && !IsEndPicture && temp){			//往右丟水蜜桃
		hit[0].SetTopLeft(m->ScreenX(x),m->ScreenY(y));
		hit[0].OnShow();
	}
	else if(e05->GetIsLeft() && !IsEndPicture && temp){		//往左丟水蜜桃
		hit[1].SetTopLeft(m->ScreenX(x),m->ScreenY(y));
		hit[1].OnShow();
	}
	else{
		Vanish[0].SetTopLeft(m->ScreenX(x),m->ScreenY(y));	//死掉
		Vanish[0].ShowBitmap();
	}
}

int EnemySkill_son02::GetX2(){return x + hit[0].Width();}
int EnemySkill_son02::GetY2(){return y + hit[0].Height();}
//--------------------------------------------------------------------------
//###################################################################################################################################
//敵人
Enemy_basic::Enemy_basic():x(0),y(0){
	const int INITIAL_DOWNVELOCITY =1;
	downVelocity = init_downVelocity = INITIAL_DOWNVELOCITY;
	hitSkill_num[0]=hitSkill_num[1]=0;
	count_P_Direction=33; //用來給換方向時能更加靈活,約程式碼刷33次後才會再決定方向
}
int Enemy_basic::SetTotal_HP(){
	return HP_total;
};
void Enemy_basic::init(Map *m){
	isAttack=beHit=isLeft=false;
	HP=SetTotal_HP();
	SetIsAlive(true);//設定每一隻敵人初始化存活
	//---------------------------------------------------------------------------------
	srand((int)time(NULL));//亂數種子
	int direction = rand()%2+1;  //設定方向 亂數取得數值1到2 決定敵人初始方位 1為左 
	SetIsLeft(direction);
	SetX(FIX_X-left_x);	  //設定敵人的初始化 X座標
	SetY(FIX_Y-height);
	//--------------------------------------------------------------開始站在地上
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
	srand((int)time(NULL));//亂數種子 用來對每隻怪物的移動範圍取亂數!
	int direction =rand()%9+1; //1為左
	//--------------------------------------------初始
	for(int i=0;i<2;i++){
		Move[i].OnMove();
		Stand[i].OnMove();
		if(isAttack )
			Attack[i].OnMove();
	}
	//--------------------------------------------
	last_x=x; //上一個座標儲存 用在貼圖判斷
	if(IsAlive() ){//存活時
		if(h->IsAttack()!=hit_num ||h->IsAttack()==0)
			beHit=false;
		beHit_by_Hero(h,m);		//被扣血狀況
		HitAI(h,m);			//攻擊AI
		if(!isAttack && !InArea(h) ){//讓怪物可以不要一直走動偶爾可以停一下
			if(count_wait==waitTime ){	
				Walking(h,m);	//怪物移動的範圍
				count_move--;
				if(count_move<0)
				count_wait--;
			}
			else{
				count_wait--;		//警戒間隔
				this->SetIsLeft(direction);
			if(count_wait<0){
					moveTime=rand()%250+100;
					waitTime=rand()%250+50;
					count_move=moveTime;
				count_wait=waitTime;
				}
			}
		}
		onfloor(m);		//重力
		//-------------------------
		if(HP<=0){//沒血時
			beHit=false; //設定狀態為死亡		
			SetIsAlive(false);
		}
		hit_num=h->IsAttack();
	}
	else{			//死亡	
		HP=HP_total;
		isAttack=false;
	}
}
void Enemy_basic::Walking(Chero *h,Map *m){
	//移動(警戒)範圍------
	srand((int)time(NULL));//亂數種子 用來對每隻怪物的移動範圍取亂數!
	MOVE_RANGE = rand()%200+100; //範圍在100到200之間
	//-------------------
	if(last_y!=y){}
	else if( !isLeft ){	//(右邊時)
		if( ( m->IsFloor(GetX2()+STEP_SIZE,GetY2()) || m->IsMoveBarrier(GetX2()+STEP_SIZE,GetX2()+STEP_SIZE,GetY2()) ) && GetX2()+STEP_SIZE <=GetFx()+MOVE_RANGE && !m->IsWall(GetX2()+STEP_SIZE,GetY2()-1) && GetX2()+STEP_SIZE < m->GetMapW() )
			x+=STEP_SIZE;		//↑(地上或障礙物 右可走)&&(範圍內)&&(右邊沒牆壁)  右走
		else
			isLeft=true;
	}	
	else{			//(左邊時)
		if( ( m->IsFloor(GetX1()-STEP_SIZE,GetY2()) || m->IsMoveBarrier(GetX1()-STEP_SIZE,GetX2()-STEP_SIZE,GetY2()) ) && GetX1()-STEP_SIZE >=GetFx()-MOVE_RANGE && !m->IsWall(GetX1()-STEP_SIZE,GetY2()-1) && GetX1()-STEP_SIZE > 0)
			x-=STEP_SIZE;		//↑(地上或障礙物 左可走)&&(範圍內)&&(左邊沒牆壁)  左走
		else
			isLeft=false;
	}
}
void Enemy_basic::onfloor(Map *m){
	
	int count_floor=0;	//初始劃離地高度
	int number=0;
	int center = ( GetX1()+GetX2() )/2;
	vector<MovingBarrier *>barrier=m->GetMovingBarrier();
	last_y=y;
	//--------------------------------離地高度
	for(count_floor=0;;count_floor++){
		if( !m->IsFloor(center,GetY2()+count_floor) && !IsTouchBarrier(barrier,count_floor,&number)  && GetY2()+count_floor<=m->GetMapH()+100){}
		else break;
	}
	//-------------------------------
	if (count_floor >= downVelocity)
		y += (downVelocity)++;		// y軸下降(移動velocity個點)// 受重力影響，下次的下降速度增加
	else{
		y +=count_floor;	//一次剛好到地
		downVelocity = init_downVelocity;	// 重設下降初始速度	
	}
	if(last_y!=y) SetX(x);	//從設開始X座標
}
bool Enemy_basic::IsTouchBarrier(vector<MovingBarrier *> b,int count,int *number){
	for(int i=0;i<(int)b.size();i++){
		if(GetX1() >= b[i]->GetX1() && GetX2() <= b[i]->GetX2()){ //同上
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
/*	for(int i=0;i<(int)b.size();i++)		//此段用於修正(x座標範圍太大者)，在Movebarrier到Movebarrier之間的模糊地帶
		for(int j=0;j<(int)b.size();j++)	//但加上這段後，會造成遊戲不順暢(迴圈太多層!)，故以設計地圖時，避免此狀況
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
			isLeft = true; //1為左邊
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
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CPen *pp, p(PS_NULL, 0, RGB(0,0,0));		// 清除pen
	pp = pDC->SelectObject(&p);
	CBrush *pb, b(RGB(255,255,0));				// 畫黃色 progress框
	pb = pDC->SelectObject(&b);
	pDC->Rectangle(x1,y1,x2,y2);				
	CBrush b1(RGB(220,160,160) );//DEFAULT_BG_COLOR);				// 畫黑色 progrss中心
	pDC->SelectObject(&b1);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2_end,progress_y2);
	CBrush b2(RGB(255,0,0));					// 畫紅色 progrss進度
	pDC->SelectObject(&b2);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2,progress_y2);
	pDC->SelectObject(pp);						// 釋放 pen
	pDC->SelectObject(pb);						// 釋放 brush
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}
void Enemy_basic::beHit_by_Hero(Chero *h,Map *m){//被主角打
	vector<Skill_basic *> skill = h->GetSkill();
	CAnimation  hit = h->GetHitAnimation();
	int dx=30/4;									//扣血狀況
//  段數一  //----------------------------------------
	if(h->IsAttack()==1 && (int)hit.GetCurrentBitmapNumber()==2)	
		if( skill[0]->isHitRectangle(GetX1(),GetY1(),GetX2(),GetY2()) )	{
			if(!beHit){
				HP_reduce(10);	
				beHit=true;
			}
			if(GetX1()-dx > 0 && GetX2()+dx < m->GetMapW()){
				if(!h->IsLeft() &&  !m->IsWall(GetX2()+dx,GetY2()-1) && !m->IsNoland(GetX2()+dx,GetY2()) )
					x+=dx;	//被打移動
				else if(!m->IsWall(GetX1()-dx,GetY2()-1) && !m->IsNoland(GetX1()-dx,GetY2()) )
					x-=dx;
			}
		}
//  段數二  //----------------------------------------
	if(h->IsAttack()==2 && hit.GetCurrentBitmapNumber()==2)
		if( skill[1]->isHitRectangle(GetX1(),GetY1(),GetX2(),GetY2()) ){	
			if(!beHit){
				HP_reduce(10);
				beHit=true;
			}
			if(GetX1()-dx > 0 && GetX2()+dx < m->GetMapW()){
				if(!h->IsLeft() &&  !m->IsWall(GetX2()+dx,GetY2()-1) && !m->IsNoland(GetX2()+dx,GetY2()) )
					x+=dx;	//被打移動
				else if(!m->IsWall(GetX1()-dx,GetY2()-1) && !m->IsNoland(GetX1()-dx,GetY2()) )
					x-=dx;
			}
		}
//  段數三  //----------------------------------------
	if(h->IsAttack()==3 && hit.GetCurrentBitmapNumber()==3)
		if( skill[2]->isHitRectangle(GetX1(),GetY1(),GetX2(),GetY2()) ){	
			if(!beHit){
				HP_reduce(15);
				beHit=true;
			}
			if(GetX1()-dx > 0 && GetX2()+dx < m->GetMapW()){
				if(!h->IsLeft() &&  !m->IsWall(GetX2()+dx,GetY2()-1) && !m->IsNoland(GetX2()+dx,GetY2()) )
					x+=dx;	//被打移動
				else if(!m->IsWall(GetX1()-dx,GetY2()-1) && !m->IsNoland(GetX1()-dx,GetY2()) )
					x-=dx;
			}
		}
//   招式   //----------------------------------------
	for(int i=0;i<(int)skill.size();i++)
		skill[i]->HitEnemy(this);
}
bool Enemy_basic::isHitHero(Chero *h){
	return false;
}
bool Enemy_basic::HitRectangle(int tx1, int ty1, int tx2, int ty2){	// 是否碰到參數範圍的矩形
	int x1 = GetX1();				// 球的左上角x座標
	int y1 = GetY1();				// 球的左上角y座標
	int x2 = GetX2();	// 球的右下角x座標
	int y2 = GetY2();	// 球的右下角y座標
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
	last_x=x; //上一個座標儲存 用在貼圖判斷
	if(IsAlive() ){		//存活時
		if( h->IsAttack()!=hit_num || !h->IsAttack() )
			beHit=false;
		beHit_by_Hero(h,m);	//被打
		HitAI(h,m);	//攻擊AI
		if(!isAttack && !InArea(h))		
			Walking(h,m);	//怪物移動的範圍
		onfloor(m);	//重力
		//--------------------	
		if(HP<=0){//沒血時
			beHit=false; //設定狀態為死亡		
			SetIsAlive(false);
		}
		hit_num=h->IsAttack();
	}
	else{		//死亡	
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
	countHit=HitTime=30;//打鬥間隔
	count_move=moveTime=rand()%250+100; //移動時間
	count_wait=waitTime=rand()%200+50;		//警戒間隔
	STEP_SIZE = 5;	//移動量
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
	int Center = (GetX1()+GetX2())/2; //取怪物X軸的中心點
	int hero_center=(h->GetX1()-40+h->GetX2())/2;
	bool condition1=(GetX2()+Area)>(h->GetX1());	//		怪物→人
	bool condition2=(GetX1()-Area)<(h->GetX2());	//	人←怪物
	if( condition1 && condition2 && h->GetY2()<=GetY2() && h->GetY2()>=GetY1()){ //在範圍內(左右)(上下) 
		if(Center<= hero_center)	//		怪物→人
			isLeft=false;
		else						//	人←怪物
			isLeft=true;			
		return true;	
	}
	return false;
}
void Enemy_son02::HitAI(Chero *h,Map *m){
	//---------------------------------------------範圍內 會追人
	if(!isAttack && InArea(h) && countHit==HitTime){
		if( ( m->IsFloor(GetX2()+STEP_SIZE,GetY2()) || m->IsMoveBarrier(GetX2()+STEP_SIZE,GetX2()+STEP_SIZE,GetY2()) ) && GetX2()+60 <= h->GetX1() && !m->IsWall(GetX2()+STEP_SIZE,GetY2()-1) )
			x+=STEP_SIZE;
		else if( ( m->IsFloor(GetX1()-STEP_SIZE,GetY2()) || m->IsMoveBarrier(GetX1()-STEP_SIZE,GetX2()-STEP_SIZE,GetY2()) ) && GetX1()-60 >=h->GetX2() && !m->IsWall(GetX1()-STEP_SIZE,GetY2()-1) )
			x-=STEP_SIZE;
	}
	int area=100;
	//---------------------------------------------
	if( !isAttack && !beHit){//沒打人，沒被打
		if(countHit==HitTime){
			if( (GetX2()+area)>(h->GetX1()) && (GetX1()-area)<(h->GetX2())  && h->GetY2()<=GetY2() && h->GetY2()>=GetY1()){
				isAttack=true;					//範圍內
				countHit--;
			}
		}
		else if(countHit<HitTime)//攻擊間隔
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
bool Enemy_son02::isHitHero(Chero *h){	//攻擊判斷
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
	left_x=48 ,right_x=81;		//設定範圍
	height=72;						//腳踩地點
	HP_total=500;
	center=(left_x+right_x)/2;
	//------------------------------
	countHit=HitTime=30;//打鬥間隔
	count_move=moveTime=rand()%250+100; //移動時間
	count_wait=waitTime=rand()%200+50;		//警戒間隔
	MOVE_RANGE = rand()%300+100; //範圍在100到500之間
	STEP_SIZE = 2;	//移動量
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
bool Enemy_son03::InArea(Chero *h){//攻擊範圍中
	int Area=100;
	int Center = (GetX1()+GetX2())/2; //取怪物X軸的中心點
	int hero_center=(h->GetX1()-40+h->GetX2())/2;
	bool condition1=(GetX2()+Area)>(h->GetX1());	//		怪→人
	bool condition2=(GetX1()-Area)<(h->GetX2());	//	人←怪
	if( condition1 && condition2 && h->GetY1()<=GetY2()+150 && h->GetY2()>=GetY1()){
		if(Center<= hero_center )//		怪→人
			isLeft=false;	
		else			//	人←怪
			isLeft=true;	
		return true;	
	}
	return false;
}
void Enemy_son03::HitAI(Chero *h,Map *m){//攻擊型態
	int area=100;
	if(!isAttack && InArea(h) ){} //範圍內，設定方向
	//------------------------------
	if( !isAttack && !beHit){//沒打人，沒被打
		es01.SetIsEndPicture(false);
		if(countHit==HitTime){//攻擊間隔
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
bool Enemy_son03::isHitHero(Chero *h){ //香蕉圖片能傷到主角的Func
	if(h->HitRectangle(es01.GetX1(),es01.GetY1(),es01.GetX2(),es01.GetY2()) && isAttack && GetAttack().GetCurrentBitmapNumber()>=5)
		return true;
	return false;
}
void  Enemy_son03::onShow(Map* m){
	if(isAttack)	//技能SHOW
		es01.EonShow (this,m);
	Enemy_basic::onShow(m);//怪物SHOW
}
int Enemy_son03::GetY1(){
	return y+20; //猴子的頭頭座標
}
CAnimation Enemy_son03::GetAttack(){
	if(!isLeft)
		return Attack[0];
	else
		return Attack[1];
}
//-----------------------------------------------------
Enemy_son04::Enemy_son04(int fx,int fy){
	EnemyId=4;//用來辨識主角被打時個別扣的血量
	x=FIX_X=fx , y=FIX_Y=fy;
	left_x=60 ,right_x=153;
	height=134;
	HP_total=100;//原來的血量
	center=(left_x+right_x)/2;
	//---------------------------
	countHit=HitTime=30;//打鬥間隔
	count_move=moveTime=rand()%250+100; //移動時間
	count_wait=waitTime=rand()%200+50;		//警戒間隔
	//----------------------------	
	STEP_SIZE = 3; 
	MOVE_RANGE = rand()%300+100; //範圍在100到500之間
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
	int Center = ( GetX1()+GetX2() )/2; //取怪物X軸的中心點
	bool condition1=(GetX2()+Area)>(h->GetX1()); //主角在怪物的右邊範圍內 
	bool condition2=(GetX1()-Area)<(h->GetX2()); //主角在怪物的左邊範圍內 
	if( condition1 && condition2 && h->GetY2()<=GetY2() && h->GetY2()>=GetY1()+80){ //交集所以是主角在怪物的左邊到右邊範圍內
		if(!isAttack){
			if( h_center > Center)		//同上
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
	int Center = (GetX1()+GetX2())/2; //取怪物X軸的中心點
	if( InArea(h) && !beHit){ //若在範圍內且沒被打的話就追擊主角
		isAttack = true; //設為攻擊狀態切換攻擊圖片
		if( !isLeft && Center<=(h_center + h_area) ){ //為了可以連續衝撞主角 假若目前在該主角右邊的某一個範圍內且在左邊狀態
			bool onland=m->IsMoveBarrier(GetX1()+attack_speed,GetX2()+attack_speed,GetY2()) || m->IsFloor(GetX2()+attack_speed,GetY2());
			if( onland && Center+attack_speed <=(h_center + h_area) && !m->IsWall(GetX2()+attack_speed,GetY2()-1) )//若超出的右邊的範圍後且右邊怪物前面那一點(x2座標)(移動範圍)是空的 變換方向狀態 
				x+=(attack_speed); //向右移動
			else
				isLeft = true ;
		}
		else if( isLeft && Center>=(h_center-h_area) ){ //此為在主角右邊範圍後 再變換方向
			bool onland=m->IsMoveBarrier(GetX1()-attack_speed,GetX2()-attack_speed,GetY2()) || m->IsFloor(GetX1()-attack_speed,GetY2());
			if( onland && Center-attack_speed >= (h_center - h_area) && !m->IsWall(GetX1()-attack_speed,GetY2()-1) ) //超過主角左邊範圍後且左邊怪物前面那一點(x1座標)(移動範圍)是空的 再變換方向 
				x-=(attack_speed);//移動
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
	left_x=60 ,right_x=90;		//設定範圍
	height=83;						//腳踩地點
	HP_total=300;
	center=(left_x+right_x)/2;
	//--------------------------	
	countHit=HitTime=30;//打鬥間隔
	count_move=moveTime=rand()%250+100; //移動時間
	count_wait=waitTime=rand()%200+50;		//警戒間隔
	MOVE_RANGE = rand()%300+100; //範圍在100到500之間
	//--------------------------	
	STEP_SIZE = 2;	//移動量
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
bool Enemy_son05::InArea(Chero *h){//攻擊範圍中
	int Area=150;
	int Center = (GetX1()+GetX2())/2; //取怪物X軸的中心點
	int hero_center=(h->GetX1()+h->GetX2())/2;
	bool condition1=(GetX2()+Area)>(h->GetX1());
	bool condition2=(GetX1()-Area)<(h->GetX2());
	if( condition1 && condition2 && h->GetY2()<=GetY2() && h->GetY2()>=GetY1()){
		if(Center<= hero_center )
			isLeft=false;					//同上
		else
			isLeft=true;
		return true;	
	}
	return false;
}
void Enemy_son05::HitAI(Chero *h,Map *m){//攻擊型態
	int area=150;
	if(!isAttack && InArea(h) ){}	//範圍內  設定方向
	if( !isAttack && !beHit){	//沒打人，沒被打
		es02.SetIsEndPicture(false);
		if(countHit==HitTime){//間隔時間
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
bool Enemy_son05::isHitHero(Chero *h){ //香蕉圖片能傷到主角的Func
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
	return y+20; //猴子的頭頭座標
}
CAnimation Enemy_son05::GetAttack(){
	if(!isLeft)
		return Attack[0];
	else
		return Attack[1];
}
}//Code Ending