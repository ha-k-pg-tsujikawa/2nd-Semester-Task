
#include "InGameScene.h"
#include "DxLib.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"

// 実行ステップ定義
enum
{
	STEP_START_JINGLE,	// 開始ジングル待ち
	STEP_INPUT,			// 入力待ち
	STEP_CLEAR_JINGLE,	// クリアジングル待ち
	STEP_END,
};
int i = 0;
int j = 0;
int chara_dir = 0;
int push_count = 0;
int clear_num = 0;
// 倉庫番のサンプルステージ
const int g_SampleStage[STAGE_HEIGHT][STAGE_WIDTH] =
{
	0,0,0,1,1,1,1,1,0,0,0,
	0,0,0,1,3,3,3,1,0,0,0,
	0,0,0,1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1,0,0,0,
	1,1,1,1,1,0,1,1,1,1,1,
	1,0,0,4,0,0,1,0,0,0,1,
	1,0,1,0,0,4,0,4,0,0,1,
	1,0,0,0,1,1,1,0,0,0,1,
	1,1,1,0,1,0,1,0,1,1,1,
	0,0,1,0,1,0,1,0,1,0,0,
	0,0,1,0,0,0,0,0,1,0,0,
	0,0,1,0,0,2,0,0,1,0,0,
	0,0,1,1,1,0,1,1,1,0,0,
};

const int g_SampleStage2[STAGE_HEIGHT][STAGE_WIDTH] =
{
	0,0,0,1,1,1,1,1,0,0,0,
	0,0,0,1,3,3,3,1,0,0,0,
	0,0,0,1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1,0,0,0,
	1,1,1,1,1,0,1,1,1,1,1,
	1,0,0,4,0,0,1,0,0,0,1,
	1,0,1,0,0,4,0,4,0,0,1,
	1,0,0,0,1,1,1,0,0,0,1,
	1,1,1,0,0,0,0,0,1,1,1,
	0,0,1,0,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,0,1,0,0,
	0,0,1,0,0,2,0,0,1,0,0,
	0,0,1,1,1,1,1,1,1,0,0,
};

InGameScene::InGameScene()
	: m_PlayerX( 0 )
	, m_PlayerY( 0 )
{
	// ステージデータを初期化
	for( int y = 0; y < STAGE_HEIGHT; y++ )
	{
		for( int x = 0; x < STAGE_WIDTH; x++ )
		{
			if (clear_num == 0)m_StageData[y][x] = g_SampleStage[y][x];
			if (clear_num == 1)m_StageData[y][x] = g_SampleStage2[y][x];
			if( m_StageData[y][x] == ObjectType_Player )
			{
				m_PlayerX = x;
				m_PlayerY = y;
				m_StageData[y][x] = ObjectType_Ground;
			}
		}
	}

	set_Step( STEP_START_JINGLE );
}

InGameScene::~InGameScene()
{
}

void InGameScene::Exec()
{
	switch ( m_Step )
	{
	case STEP_START_JINGLE:	step_StartJingle();	break;
	case STEP_INPUT:		step_Input();		break;
	case STEP_CLEAR_JINGLE:	step_ClearJingle();	break;
	default:									break;
	}
}

void InGameScene::Draw()
{
	// ステージの中身と、プレイヤーを描画(DXライブラリのDrawBoxを使用)
	// 	int DrawBox( int x1 , int y1 , int x2 , int y2 , unsigned int Color , int FillFlag ) ;
	
	// 課題(22日まで)
	// m_StageDataの配列の中身をみて、それぞれの四角形を描画する
	//	ObjectType_Ground,			// 0 なし
	//	ObjectType_Wall,			// 1 白(255, 255, 255)
	//	ObjectType_Target,			// 3 赤(255,   0,   0)
	//	ObjectType_UnsetCrate,		// 4 紫(255,   0, 255)
	//	ObjectType_SetCrate = 7,	// 7 緑(  0, 255,   0)


	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			switch (m_StageData[y][x])
			{
			case ObjectType_Ground:
				LoadGraphScreen(x * CHIP_WIDTH, y * CHIP_HEIGHT, "Res/ground.png", TRUE);
				break;
			case ObjectType_Wall:
				LoadGraphScreen(x * CHIP_WIDTH, y * CHIP_HEIGHT, "Res/block.png", TRUE);
				//DrawBox(x * CHIP_WIDTH,
				//	y * CHIP_HEIGHT,
				//	x * CHIP_WIDTH + CHIP_WIDTH,
				//	y * CHIP_HEIGHT + CHIP_HEIGHT,
				//	GetColor(255, 255, 255), TRUE);
				break;
			case ObjectType_Target:
				LoadGraphScreen(x * CHIP_WIDTH, y * CHIP_HEIGHT, "Res/crate_00.png", TRUE);
				//DrawBox(x * CHIP_WIDTH,
				//	y * CHIP_HEIGHT,
				//	x * CHIP_WIDTH + CHIP_WIDTH,
				//	y * CHIP_HEIGHT + CHIP_HEIGHT,
				//	GetColor(255, 0, 0), TRUE);
				break;
			case ObjectType_UnsetCrate:
				LoadGraphScreen(x * CHIP_WIDTH, y * CHIP_HEIGHT, "Res/crate_02.png", TRUE);
				//DrawBox(x * CHIP_WIDTH,
				//	y * CHIP_HEIGHT,
				//	x * CHIP_WIDTH + CHIP_WIDTH,
				//	y * CHIP_HEIGHT + CHIP_HEIGHT,
				//	GetColor(255, 0, 255), TRUE);
				break;
			case ObjectType_SetCrate:
				LoadGraphScreen(x * CHIP_WIDTH, y * CHIP_HEIGHT, "Res/crate_01.png", TRUE);
				//DrawBox(x * CHIP_WIDTH,
				//	y * CHIP_HEIGHT,
				//	x * CHIP_WIDTH + CHIP_WIDTH,
				//	y * CHIP_HEIGHT + CHIP_HEIGHT,
				//	GetColor(0, 255, 0), TRUE);
				break;
			}
		}
	}
	int player_up[3];
	int player_down[3];
	int player_left[3];
	int player_right[3];

	player_up[0] = LoadGraph("Res/player_02.png");
	player_up[1] = LoadGraph("Res/player_01.png");
	//player_up[2] = LoadGraph("Res/player_02.png");

	player_down[0] = LoadGraph("Res/player_05.png");
	player_down[1] = LoadGraph("Res/player_04.png");
	//player_down[2] = LoadGraph("Res/player_05.png");
	
	player_left[0] = LoadGraph("Res/player_08.png");
	player_left[1] = LoadGraph("Res/player_07.png");
	//player_left[2] = LoadGraph("Res/player_08.png");
	
	player_right[0] = LoadGraph("Res/player_11.png");
	player_right[1] = LoadGraph("Res/player_10.png");
	//player_right[2] = LoadGraph("Res/player_11.png");

	switch (chara_dir)
	{
	case 0:
		DrawRectGraph(m_PlayerX * CHIP_WIDTH + 4, m_PlayerY * CHIP_HEIGHT + 2, 4, 2, 24, 28, player_up[j], FALSE);
		break;
	case 1:
		DrawRectGraph(m_PlayerX * CHIP_WIDTH + 4, m_PlayerY * CHIP_HEIGHT + 2, 4, 2, 24, 28, player_down[j], FALSE);
		break;
	case 2:
		DrawRectGraph(m_PlayerX * CHIP_WIDTH + 4, m_PlayerY * CHIP_HEIGHT + 2, 4, 2, 24, 28, player_left[j], FALSE);
		break;
	case 3:
		DrawRectGraph(m_PlayerX * CHIP_WIDTH + 4, m_PlayerY * CHIP_HEIGHT + 2, 4, 2, 24, 28, player_right[j], FALSE);
		break;
	default:
		break;
	}	


	//DrawBox( m_PlayerX * CHIP_WIDTH,
	//		 m_PlayerY * CHIP_HEIGHT,
	//		 m_PlayerX * CHIP_WIDTH  + CHIP_WIDTH,
	//		 m_PlayerY * CHIP_HEIGHT + CHIP_HEIGHT,
	//		 GetColor( 0, 0, 255 ),
	//		 TRUE );

	if (IsClear())
	{
		DrawString(100, 200, "!! Game Clear !!", GetColor(0, 0, 255));
		DrawString(400, 200, "Push Enter to NEXT", GetColor(0, 0, 255));
	}
	else
	{
		DrawString(20, 450, "Rでリトライ", GetColor(0, 0, 0));
	}

	// 画面左上にデバッグ用の文字を黒で表示する
	if(clear_num == 0)DrawString(520, 450, "Stage 1", GetColor(0, 0, 0));
	if(clear_num == 1)DrawString(520, 450, "Stage 2", GetColor(0, 0, 0));

	DrawFormatString(400, 20, GetColor(0, 0, 0), "Number of moves : %d", push_count);
}

bool InGameScene::IsEnd() const
{
	return ( m_Step == STEP_END );
}

// 開始ジングル待ち
void InGameScene::step_StartJingle()
{
	set_Step( STEP_INPUT );
}
// 入力待ち
void InGameScene::step_Input()
{
	InputManager* pInputMng = InputManager::GetInstance();
	if (pInputMng->IsPush(KeyType_Reset))
	{
		Reset();
	}
	// 上下左右キーが押されたら、それぞれMove(押された方向キーと対応するDirType_〇〇)を呼び出す

	else if (pInputMng->IsPush(KeyType_Up))
	{
		Move(DirType_Up);
		chara_dir = 0;
	}
	else if (pInputMng->IsPush(KeyType_Down))
	{
		Move(DirType_Down);
		chara_dir = 1;
	}
	else if (pInputMng->IsPush(KeyType_Left))
	{
		Move(DirType_Left);
		chara_dir = 2;
	}
	else if (pInputMng->IsPush(KeyType_Right))
	{
		Move(DirType_Right);
		chara_dir = 3;
	}

	i++;
	if (i % 6 == 0) j++;
	if (j >= 2)j = 0;

	if (IsClear())
	{
		set_Step(STEP_CLEAR_JINGLE);
	}
}
// クリアジングル待ち
void InGameScene::step_ClearJingle()
{
	InputManager* pInputMng = InputManager::GetInstance();
	if (pInputMng->IsPush(KeyType_Enter))
	{
		if (clear_num == 0)
		{
			clear_num = 1;
			set_Step(STEP_START_JINGLE);
			Reset();
		}
		else if (clear_num == 1)
		{
			clear_num = 0;
			set_Step(STEP_END);
			Reset();
			SceneManager::SetNextScene(SceneID_Result);
		}
	}
}

// ゲームのクリア判定
bool InGameScene::IsClear() const
{
	// 2重for文でm_StageDateの配列を検索し、
	// 1つでもObjectType_UnsetCrateが見つかったらfalseを返す
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (m_StageData[y][x] == ObjectType_UnsetCrate)
			{
				return false;
			}
		}
	}

	// for文を抜けてきたらObjectType_UnsetCrateがなかったことになり、
	// 逆説的にすべてObjectType_SetCrate状態になっている(== ゲームクリア)
	return true;
}

// 初期状態にリセット
void InGameScene::Reset()
{
	// コンストラクタで初期化している部分をそのままこちらへ移設する
	// コンストラクタでResetを呼び出す
	// step_InputでKeyType_Resetが押されたら、Reset()を呼び出す

	// ※外部データを読み込んだり、ステージを増やす場合は処理内容を考える必要がある
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (clear_num == 0)m_StageData[y][x] = g_SampleStage[y][x];
			if (clear_num == 1)m_StageData[y][x] = g_SampleStage2[y][x];
			if (m_StageData[y][x] == ObjectType_Player)
			{
				m_PlayerX = x;
				m_PlayerY = y;
				m_StageData[y][x] = ObjectType_Ground;
			}
		}
	}
	chara_dir = 0;
	push_count = 0;
}

// 移動処理
void InGameScene::Move(DirType dir_)
{
	// DirTypeが範囲内か
	if (dir_ < DirType_Up && dir_ >= DirType_Max)
	{
		return;
	}
	// 判定用の一時変数
	int next_x  = m_PlayerX;	// 次の移動先X
	int next_y  = m_PlayerY;	// 次の移動先Y
	int next2_x = m_PlayerX;	// 次の次X
	int next2_y = m_PlayerY;	// 次の次Y

	// 移動方向に応じて座標を設定
	// next_x(y),next2_x(y)の値をそれぞれ設定する
	switch (dir_)
	{
	case DirType_Up:
		next_y  -= 1;
		next2_y -= 2;
		break;
	case DirType_Down:
		next_y  += 1;
		next2_y += 2;
		break;
	case DirType_Left:
		next_x  -= 1;
		next2_x -= 2;
		break;
	case DirType_Right:
		next_x  += 1;
		next2_x += 2;
		break;
	}

	// 移動先が範囲外なら、関数を抜ける
	if (next_x < 0 || next_y < 0 || next_x >= (STAGE_WIDTH-1) || next_y >= (STAGE_HEIGHT-1))
	{
		return;
	}

	// 通常移動(移動先がプレイヤーの乗れるマップチップだったらm_PlayerX(Y)をnextx(y)で上書き
	if (m_StageData[next_y][next_x] == ObjectType_Ground || m_StageData[next_y][next_x] == ObjectType_Target)
	{
		m_PlayerX = next_x;
		m_PlayerY = next_y;
		push_count++;
	}
	// 移動先に箱(クレート)がある場合
	else if (m_StageData[next_y][next_x] == ObjectType_UnsetCrate || m_StageData[next_y][next_x] == ObjectType_SetCrate)
	{
		// 2つ先が画面外かチェック
		if (next2_x < 0 || next2_y < 0 || next2_x >= (STAGE_WIDTH - 1) || next2_y >= (STAGE_HEIGHT - 1))
		{
			return;
		}
		// 2つ先が移動できない物かチェック
		// 移動できるマップチップでない場合という判定
		if (m_StageData[next2_y][next2_x] != ObjectType_Ground && m_StageData[next2_y][next2_x] != ObjectType_Target)
		{
			return;
		}

		// となりの位置を変更する
		if (m_StageData[next_y][next_x] == ObjectType_UnsetCrate)
		{
			m_StageData[next_y][next_x] = ObjectType_Ground;
		}
		else
		{
			m_StageData[next_y][next_x] = ObjectType_Target;
		}
		// 2つ先に箱を配置する
		if (m_StageData[next2_y][next2_x] == ObjectType_Ground)
		{
			m_StageData[next2_y][next2_x] = ObjectType_UnsetCrate;
		}
		else if(m_StageData[next2_y][next2_x] == ObjectType_Target)
		{
			m_StageData[next2_y][next2_x] = ObjectType_SetCrate;
		}

		// プレイヤーを移動させる
		m_PlayerX = next_x;
		m_PlayerY = next_y;
		push_count++;
	}

}

