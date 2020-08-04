#include "stdafx.h"
#include "LoadingSceen.h"
#include "Map/Shop.h"
#include <conio.h>

LoadingSceen::LoadingSceen()
{
}


LoadingSceen::~LoadingSceen()
{
}

HRESULT LoadingSceen::init()
{
	LoadingNum = 0;
	MaxLoadingNum = 18;
	m_Pos = D3DXVECTOR2(0.0f, 100.0f);
	Level = 1;
	m_DivNum = 10;
	status = future_status::timeout;
	SOUNDMANAGER->play("MainMenu", GAMEMANAGER->GetMusicVolum());
	bFirst = false;
	ImageName = "LoadingImg1";
	ImageDelayTime = 0.0f;

	return S_OK;
}

void LoadingSceen::release()
{
}

void LoadingSceen::update()
{
	if (!bFirst)
	{
		ImageName = "LoadingImg1";   //로딩 이미지 파일 명

		data = async([&]() {MapLoadData(); });      // async로 데이터 로드 실행
		bFirst = true;
	}

	status = data.wait_for(chrono::milliseconds(60));        // 60밀리세컨드 정도 대기한다
	if (status == future_status::ready)           // 현재 상태가 시작 준비 상태이면(로딩이 전부 끝난 상태라면)
	{
		if (KEYMANAGER->isKeyDown(VK_SPACE))
		{
			if (GAMEMANAGER->Getm_CurrentLevel() > -1)
			{
				SCENEMANAGER->ChangeScene("FileSelect");
			}
			else
			{
				SOUNDMANAGER->stop("MainMenu");
				SCENEMANAGER->ChangeScene("LobbyStage");
			}
		}
	}
}

void LoadingSceen::render()
{
	ImageDelayTime += g_ETime;

	if (ImageDelayTime > 3.5f)
	{
		if (ImageName == "LoadingImg1")
		{
			ImageName = "LoadingImg2";
		}
		else
		{
			ImageName = "LoadingImg1";
		}

		ImageDelayTime = 0.0f;
	}
	g_ptCam.x = 0;
	g_ptCam.y = 0;
	Image * img = IMGMANAGER->GetImage(ImageName);
	img->Position(WINSIZEX / 2, WINSIZEY / 2);
	img->Scale(WINSIZEX, WINSIZEY);
	img->Render();

	if (status == future_status::timeout)
	{
		img = IMGMANAGER->GetImage("ConsolLoading");
		img->Position(WINSIZEX / 2, 100);
		img->Scale(100, 100);
		img->Render();
	}
	else if(status == future_status::ready)
	{
		img = IMGMANAGER->GetImage("Continue");
		img->Position(WINSIZEX / 2, 100);
		img->Scale(500, 100);
		img->Render();
	}
}


void LoadingSceen::MapLoadData()
{
	if (!bLoding)
	{
		GameDataLoad();
		CHARTERMANAGER->PlayerInit();
		UIMANAGER->PlayerUIinit();
		bLoding = true;
	}
	random_device rd;
	RandomGet Random(rd());
	MAPMANAGER->GetAllMap()->release();
	MAPMAKERMANAGER->MapCreatInit(150, 150, 10, 0.2f, 0.6f, 0.2f, 6);
	// 맵 생성 부분
	queue<BspTreeNode*> Node;
	// 루트 노드를 가져온다
	Node = MAPMAKERMANAGER->GetRootNode();
	// 나눌 갯수만큼 반복
	for (int i = 0; i < m_DivNum; i++)
	{
		int Size = Node.size();
		// 노드 갯수 만큼 반복
		for (int j = 0; j < Size; j++)
		{
			MAPMAKERMANAGER->Division(Node, Random);  // 나눌 노드와 랜덤 값 입력
		}
		LoadingNum++;
	}
	
	MAPMAKERMANAGER->RoomMaker();
	LoadingNum++;
	MAPMAKERMANAGER->Coalescence();
	LoadingNum++;
	MAPMAKERMANAGER->FillData();
	LoadingNum++;
	MAPMANAGER->GetAllMap()->init();
	int Clevel = GAMEMANAGER->Getm_CurrentLevel();
	char FloorName[256];
	if (Clevel != -1)
	{
		sprintf_s(FloorName, "Floor%d", Clevel % 5);
	}
	else
	{
		sprintf_s(FloorName, "Floor0");
	}

	MAPMANAGER->MapInit(150, 150, 50, 50, FloorName);
	MAPMANAGER->BeginTileTexUpdate();
	MAPMANAGER->TexMapUpdate();
	CHARTERMANAGER->EnemyReset();

	if (GAMEMANAGER->Getm_CurrentLevel() > -1)
	{
		MAPMANAGER->MapSetting();
		CHARTERMANAGER->GetPlayer()->PlayerInitPos();
		if (GAMEMANAGER->Getm_Mode() == MODE_STORY)
		{
			if (GAMEMANAGER->Getm_CurrentLevel() != 5)
			{
				MAPMANAGER->Getm_Shop()->Init();
			}
		}
		else
		{
			MAPMANAGER->Getm_Shop()->Init();
		}
		MAPMANAGER->CrateStair();
		MAPMANAGER->CreateKey();
		CHARTERMANAGER->GetPlayer()->Setm_PlayerSight(2);
		CHARTERMANAGER->GetPlayer()->Setm_BeatTime(false);
		CHARTERMANAGER->GetPlayer()->Setm_Life(CHARTERMANAGER->GetPlayer()->Getm_MaxLife());
		CHARTERMANAGER->GetPlayer()->Setm_Alive(true);
		TRAPMANAGER->CreateTrap();
		CHARTERMANAGER->CreateEnemy(GAMEMANAGER->Getm_CurrentLevel(), GAMEMANAGER->Getm_CurrentLevel(), GAMEMANAGER->Getm_CurrentLevel());
	}
	return;
}

void LoadingSceen::GameDataLoad()
{
	// 맵 이미지 추가
	InstanceImgManager->AddImage("Floor0", _T("MapImg/floor_zone1.bmp"), 3, 2, 150 * 150);
	InstanceImgManager->AddImage("Floor1", _T("MapImg/zone2_floorA.png"), 3, 4, 150 * 150);
	InstanceImgManager->AddImage("Floor2", _T("MapImg/zone3_floor.png"), 3, 2, 150 * 150);
	InstanceImgManager->AddImage("Floor3", _T("MapImg/zone4_floor.png"), 3, 2, 150 * 150);
	InstanceImgManager->AddImage("Floor4", _T("MapImg/zone5_floor.png"), 3, 2, 150 * 150);
	IMGMANAGER->AddImage("Wall0", _T("MapImg/Wall.png"), 15, 1, "PS2");
	IMGMANAGER->AddImage("Wall1", _T("MapImg/wall_zone2png.png"), 8, 1, "PS2");
	IMGMANAGER->AddImage("Wall2", _T("MapImg/zone3wall.png"), 8, 1, "PS2");
	IMGMANAGER->AddImage("Wall3", _T("MapImg/zone4wall.png"), 8, 1, "PS2");
	IMGMANAGER->AddImage("Wall4", _T("MapImg/zone5wall.png"), 13, 1, "PS2");
	IMGMANAGER->AddImage("LobbyWall", _T("MapImg/LobbyWall.png"), "PS2");
	IMGMANAGER->AddImage("ShopWall", _T("MapImg/ShopWall.png"), 5, 1, "PS2");
	IMGMANAGER->AddImage("Black", _T("MapImg/TEMP_overlay_black.png"), "PS2");
	IMGMANAGER->AddImage("Door", _T("ObjectImg/Door.png"), "PS2");
	IMGMANAGER->AddImage("DoorSide", _T("ObjectImg/DoorSide.png"), "PS2");
	IMGMANAGER->AddImage("Torch", _T("ObjectImg/wall_torch.bmp"), 4, 1, "PS2");
	IMGMANAGER->AddImage("Box", _T("ObjectImg/Box.png"), 4, 1, "PS2");
	IMGMANAGER->AddImage("Stairs", _T("ObjectImg/stairs.png"), "PS2");
	IMGMANAGER->AddImage("LockStairs", _T("ObjectImg/stairs_locked.png"), "PS2");
	IMGMANAGER->AddImage("LockDoor", _T("MapImg/door_locked_front.png"), "PS2");
	IMGMANAGER->AddImage("LockDoorSide", _T("MapImg/door_locked_side.png"), "PS2");
	IMGMANAGER->AddImage("ShopFloor", _T("MapImg/TEMP_shop_floor.png"), 3, 1, "PS2");
	IMGMANAGER->AddImage("UnbrokenWall", _T("MapImg/UnbrokenWall.png"), "PS2");
	// 캐릭터 이미지 추가
	IMGMANAGER->AddImage("Player_Head", _T("Player/player1_heads_xmas.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Player_Body", _T("Player/player1_armor_body.png"), 4, 10, "PS2");
	IMGMANAGER->AddImage("Jp1_Head", _T("Player/jp1_heads.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jp1_Body", _T("Player/jp1_armor_body.png"), 4, 10, "PS2");
	IMGMANAGER->AddImage("Jp2_Head", _T("Player/jp2_heads.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jp2_Body", _T("Player/jp2_armor_body.png"), 4, 10, "PS2");
	IMGMANAGER->AddImage("Jp3_Head", _T("Player/jp3_heads.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jp3_Body", _T("Player/jp3_armor_body.png"), 4, 1, "PS2");
	IMGMANAGER->AddImage("Jp4_Head", _T("Player/jp4_heads.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jp4_Body", _T("Player/jp4_armor_body.png"), 4, 10, "PS2");
	IMGMANAGER->AddImage("Jp5_Head", _T("Player/jp5_heads.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jp5_Body", _T("Player/jp5_armor_body.png"), 4, 10, "PS2");
	IMGMANAGER->AddImage("Jp6_Head", _T("Player/jp6_heads.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jp6_Body", _T("Player/jp6_armor_body.png"), 4, 10, "PS2");
	IMGMANAGER->AddImage("Jp7_Head", _T("Player/jp7_heads.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jp7_Body", _T("Player/jp7_armor_body.png"), 4, 10, "PS2");
	IMGMANAGER->AddImage("Jp8_Head", _T("Player/jp8_heads.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jp8_Body", _T("Player/jp8_armor_body.png"), 4, 10, "PS2");
	IMGMANAGER->AddImage("Jp9_Head", _T("Player/jp9_heads.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jp9_Body", _T("Player/jp9_armor_body.png"), 4, 10, "PS2");
	IMGMANAGER->AddImage("Jp10_Head", _T("Player/jp10_heads.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jp10_Body", _T("Player/jp10_armor_body.png"), 4, 10, "PS2");
	IMGMANAGER->AddImage("Jp11_Head", _T("Player/jp11_heads.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jp11_Body", _T("Player/jp11_armor_body.png"), 4, 10, "PS2");
	IMGMANAGER->AddImage("PlayerShadow", _T("Player/TEMP_shadow_standard.png"), "PS2");
	// UI이미지 추가
	IMGMANAGER->AddImage("Heart", _T("UIImg/TEMP_beat_heart.png"), 2, 1, "PS2");
	IMGMANAGER->AddImage("Note1", _T("UIImg/TEMP_beat_marker.png"), "PS2");
	IMGMANAGER->AddImage("Note2", _T("UIImg/TEMP_beat_marker_red.png"), "PS2");
	IMGMANAGER->AddImage("NoteTarget", _T("UIImg/TEMP_beat_target.png"), "PS2");
	IMGMANAGER->AddImage("EnemyHeart", _T("UIImg/TEMP_heart_small.png"), "PS2");
	IMGMANAGER->AddImage("EnemyHeartEmpty", _T("UIImg/TEMP_heart_empty_small.png"), "PS2");
	// Player UI 이미지 추가
	IMGMANAGER->AddImage("PlayerDiamond", _T("UIImg/diamond.png"), "PS2");
	IMGMANAGER->AddImage("PlayerLife", _T("UIImg/heart.png"), "PS2");
	IMGMANAGER->AddImage("PlayerLifeEmpty", _T("UIImg/heart_empty.png"), "PS2");
	IMGMANAGER->AddImage("PlayerLifeHub", _T("UIImg/heart_half.png"), "PS2");
	IMGMANAGER->AddImage("PlayerCoin", _T("UIImg/hud_coins.png"), "PS2");
	IMGMANAGER->AddImage("hudslot1", _T("UIImg/hud_slot_1.png"), "PS2");
	IMGMANAGER->AddImage("hudslot2", _T("UIImg/hud_slot_2.png"), "PS2");
	IMGMANAGER->AddImage("hudslot3", _T("UIImg/hud_slot_3.png"), "PS2");
	IMGMANAGER->AddImage("hudslot5", _T("UIImg/hud_slot_5.png"), "PS2");
	IMGMANAGER->AddImage("hudslot4", _T("UIImg/hud_slot_4.png"), "PS2");
	IMGMANAGER->AddImage("hudslot6", _T("UIImg/hud_slot_6.png"), "PS2");
	IMGMANAGER->AddImage("hudslot7", _T("UIImg/hud_slot_7.png"), "PS2");
	IMGMANAGER->AddImage("hudslot8", _T("UIImg/hud_slot_8.png"), "PS2");
	IMGMANAGER->AddImage("hudslotaction1", _T("UIImg/hud_slot_action1.png"), "PS2");
	IMGMANAGER->AddImage("hudslotaction2", _T("UIImg/hud_slot_action2.png"), "PS2");
	IMGMANAGER->AddImage("hudslotaction2empty", _T("UIImg/hud_slot_action2_empty.png"), "PS2");
	IMGMANAGER->AddImage("hudslotbomb", _T("UIImg/hud_slot_bomb.png"), "PS2");
	IMGMANAGER->AddImage("hudslotboots", _T("UIImg/hud_slot_boots.png"), "PS2");
	IMGMANAGER->AddImage("hudslotreload", _T("UIImg/hud_slot_reload.png"), "PS2");
	IMGMANAGER->AddImage("hudslotspellcool", _T("UIImg/hud_slot_spell_cool.png"), "PS2");
	IMGMANAGER->AddImage("hudslotspell1", _T("UIImg/hud_slot_spell1.png"), "PS2");
	IMGMANAGER->AddImage("hudslotspell2", _T("UIImg/hud_slot_spell2.png"), "PS2");
	IMGMANAGER->AddImage("hudslotweapon2", _T("UIImg/hud_slot_weapon2.png"), "PS2");
	IMGMANAGER->AddImage("hudslotweapon2empty", _T("UIImg/hud_slot_weapon2_empty.png"), "PS2");
	IMGMANAGER->AddImage("DigitalNum", _T("UIImg/TEMP_digits.png"), 10, 1, "PS2");
	IMGMANAGER->AddImage("DigitalNum2", _T("UIImg/TEMP_digits.png"), 10, 1, "PS2");
	IMGMANAGER->AddImage("XImg", _T("UIImg/X.bmp"), "PS2");
	IMGMANAGER->AddImage("MissImg", _T("UIImg/TEMP_missed.png"), "PS2");
	IMGMANAGER->AddImage("PerfectImg", _T("UIImg/TEMP_perfect.png"), "PS2");
	// 적이미지 추가
	IMGMANAGER->AddImage("Skeleton", _T("Enemy/skeleton.png"), 8, 2, "PS2");
	IMGMANAGER->AddImage("Slime", _T("Enemy/slime_green.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("Jombie", _T("Enemy/zombie_snake.png"), 72, 2, "PS2");
	IMGMANAGER->AddImage("Minotaur", _T("Enemy/minotaur.png"), 9, 2, "PS2");
	IMGMANAGER->AddImage("ShopKeeper", _T("Enemy/shopkeeper.png"), 8, 2, "PS2");
	//Item 이미지 추가
	IMGMANAGER->AddImage("shovelbasic", _T("Item/shovel_basic.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("weaponbroadsword", _T("Item/weapon_broadsword.png"), "PS2");
	IMGMANAGER->AddImage("weapondagger", _T("Item/weapon_dagger.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Coin1", _T("Item/resource_coin1.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Coin2", _T("Item/resource_coin2.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Coin3", _T("Item/resource_coin3.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Coin4", _T("Item/resource_coin4.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Coin5", _T("Item/resource_coin5.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Coin6", _T("Item/resource_coin6.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Coin7", _T("Item/resource_coin7.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Coin8", _T("Item/resource_coin8.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Coin9", _T("Item/resource_coin9.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Coin10", _T("Item/resource_coin10.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("BackPack", _T("Item/hud_backpack.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("HeartItem", _T("Item/misc_heart_container.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("LeatherArmor", _T("Item/armor_leather.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("LockKey", _T("Item/error_locked.png"), "PS2");
	IMGMANAGER->AddImage("BroadSword", _T("Item/weapon_broadsword.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Chainmail", _T("Item/armor_chainmail.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Platemail", _T("Item/armor_platemail.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("weaponwhip", _T("Item/weapon_whip.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("FeetGreaves", _T("Item/feet_greaves.png"), 1, 2, "PS2");
	IMGMANAGER->AddImage("Headhelmet", _T("Item/head_helmet.png"), 1, 2, "PS2");
	// 이펙트 이미지 추가
	IMGMANAGER->AddImage("DaggerEffect", _T("Effect/swipe_dagger.png"), 3, 1, "PS2");
	IMGMANAGER->AddImage("LongswordEffect", _T("Effect/swipe_longsword.png"), 4, 1, "PS2");
	IMGMANAGER->AddImage("EnemyEffect", _T("Effect/swipe_enemy.png"), 5, 1, "PS2");
	IMGMANAGER->AddImage("BroadSwordEffect", _T("Effect/swipe_broadsword.png"), 3, 1, "PS2");
	IMGMANAGER->AddImage("WhipEffect", _T("Effect/swipe_whip_A.png"), 4, 1, "PS2");
	// 함정 이미지 추가
	IMGMANAGER->AddImage("TeleportTrap", _T("TrapImg/teleporttrap.png"), 2, 2, "PS2");
	IMGMANAGER->AddImage("MoveTrap", _T("TrapImg/bouncetrap.png"), 2, 6, "PS2");
	IMGMANAGER->AddImage("SpikeTrap", _T("TrapImg/spiketrap.png"), 1, 6, "PS2");
	IMGMANAGER->AddImage("FireSpikeTrap", _T("TrapImg/firetrap_decorative.png"), 4, 2, "PS2");
	IMGMANAGER->AddImage("ObjectBox", _T("ObjectImg/Box.png"), 4, 1, "PS2");
	// 폰트 이미지 추가
	IMGMANAGER->AddImage("TextNum", _T("Font/FontNum.png"), 10, 1, "PS2");
	IMGMANAGER->AddImage("Text", _T("Font/Font.png"), 26, 1, "PS2");
	// 파일 선택 창 이미지 추가
	IMGMANAGER->AddImage("Introduce", _T("LoadingImg/Introduce.png"), "PS2");
	// UI이미지 추가
	IMGMANAGER->AddImage("PopupBack", _T("UIImg/TEMP_popup_back.png"), "PS2");
	IMGMANAGER->AddImage("PopupBlack", _T("UIImg/TEMP_overlay_black.png"), "PS2");
	IMGMANAGER->AddImage("BorderConer1", _T("UIImg/border_corner1.png"), "PS2");
	IMGMANAGER->AddImage("BorderConer2", _T("UIImg/border_corner2.png"), "PS2");
	IMGMANAGER->AddImage("BorderConer3", _T("UIImg/border_corner3.png"), "PS2");
	IMGMANAGER->AddImage("BorderConer4", _T("UIImg/border_corner4.png"), "PS2");
	IMGMANAGER->AddImage("BorderHorizontal", _T("UIImg/border_horizontal.png"), "PS2");
	IMGMANAGER->AddImage("BorderVertical", _T("UIImg/border_vertical.png"), "PS2");
	IMGMANAGER->AddImage("Calibration", _T("UIImg/TEMP_calibration_square.png"), "PS2");
	// 폰트 이미지 추가
	IMGMANAGER->AddImage("DeathFont1", _T("Font/game_death1.png"), "PS2");
	IMGMANAGER->AddImage("DeathFont2", _T("Font/game_death2.png"), "PS2");
	IMGMANAGER->AddImage("UpDown", _T("Font/options_button_comboupdown.png"), "PS2");
	IMGMANAGER->AddImage("Runsummary", _T("Font/game_runsummary.png"), "PS2");
	IMGMANAGER->AddImage("Killedby", _T("Font/game_killedby.png"), "PS2");
	IMGMANAGER->AddImage("Score", _T("Font/game_score.png"), "PS2");
	IMGMANAGER->AddImage("QuickStart", _T("Font/game_quickrestart.png"), "PS2");
	IMGMANAGER->AddImage("GameLobbyRe", _T("Font/game_lobby.png"), "PS2");
	IMGMANAGER->AddImage("SkeletonName", _T("Font/enemybig_whiteskeleton.png"), "PS2");
	IMGMANAGER->AddImage("greyfont", _T("Font/GrayNum.png"), 10, 1, "PS2");
	IMGMANAGER->AddImage("FastMove1", _T("Font/lobby_fastmove1.png"), "PS2");
	IMGMANAGER->AddImage("FastMove2", _T("Font/lobby_fastmove2.png"), "PS2");
	IMGMANAGER->AddImage("OptionOk", _T("Font/options_ok.png"),"PS2");
	IMGMANAGER->AddImage("OptionNo", _T("Font/options_no.png"), "PS2");
	IMGMANAGER->AddImage("OptionOkSelect", _T("Font/Select/options_ok.png"), "PS2");
	IMGMANAGER->AddImage("OptionNoSelect", _T("Font/Select/options_no.png"), "PS2");
	IMGMANAGER->AddImage("OptionPause", _T("Font/options_pause.png"), "PS2");
	IMGMANAGER->AddImage("OptionTitle", _T("Font/options_optionstitle.png"), "PS2");
	IMGMANAGER->AddImage("OptionButton", _T("Font/options_options.png"), "PS2");
	IMGMANAGER->AddImage("OptionExit", _T("Font/options_exitgame.png"), "PS2");
	IMGMANAGER->AddImage("AudioOption", _T("Font/options_audiooptions.png"), "PS2");
	IMGMANAGER->AddImage("AudioOptionTitle", _T("Font/options_audiooptionstitle.png"), "PS2");
	IMGMANAGER->AddImage("MusicVolumOption", _T("Font/options_musicvolume.png"), "PS2");
	IMGMANAGER->AddImage("SFXVolumOption", _T("Font/options_soundvolume.png"), "PS2");
	IMGMANAGER->AddImage("GameContinue", _T("Font/options_continue.png"), "PS2");
	IMGMANAGER->AddImage("SkinChangeOption", _T("Font/options_changeskin.png"), "PS2");
	IMGMANAGER->AddImage("SkinChangeOptionTitle", _T("Font/options_changeskintitle.png"), "PS2");
	IMGMANAGER->AddImage("DefaultCharter", _T("Font/options_defaultcharacter.png"), "PS2");
	IMGMANAGER->AddImage("MusicVolumOptionSelect", _T("Font/Select/options_musicvolume.png"), "PS2");
	IMGMANAGER->AddImage("SFXVolumOptionSelect", _T("Font/Select/options_soundvolume.png"), "PS2");
	IMGMANAGER->AddImage("GameContinueSelect", _T("Font/Select/options_continue.png"), "PS2");
	IMGMANAGER->AddImage("SkinChangeOptionSelect", _T("Font/Select/options_changeskin.png"), "PS2");
	IMGMANAGER->AddImage("AudioOptionSelect", _T("Font/Select/options_audiooptions.png"), "PS2");
	IMGMANAGER->AddImage("OptionExitSelect", _T("Font/Select/options_exitgame.png"), "PS2");
	IMGMANAGER->AddImage("OptionButtonSelect", _T("Font/Select/options_options.png"), "PS2");
	IMGMANAGER->AddImage("CharterEditor", _T("Font/leveleditor_character.png"), "PS2");
	IMGMANAGER->AddImage("CharterEditorSelect", _T("Font/Select/leveleditor_character.png"), "PS2");
	IMGMANAGER->AddImage("OptionQuickStart", _T("Font/options_quickrestart.png"), "PS2");
	IMGMANAGER->AddImage("OptionQuickStartSelect", _T("Font/Select/options_quickrestart.png"), "PS2");
	IMGMANAGER->AddImage("OptionLobby", _T("Font/options_quit_lobby.png"), "PS2");
	IMGMANAGER->AddImage("OptionLobbySelect", _T("Font/Select/options_quit_lobby.png"), "PS2");
	IMGMANAGER->AddImage("LobbyZone1", _T("Font/lobby_zone1.png"), "PS2");
	IMGMANAGER->AddImage("LobbyZone2", _T("Font/lobby_zone2.png"), "PS2");
	IMGMANAGER->AddImage("LobbyZone3", _T("Font/lobby_zone3.png"), "PS2");
	IMGMANAGER->AddImage("LobbyZone4", _T("Font/lobby_zone4.png"), "PS2");
	IMGMANAGER->AddImage("LobbyTutorial", _T("Font/lobby_tutorial.png"), "PS2");
	IMGMANAGER->AddImage("LobbyLeveleditor", _T("Font/lobby_leveleditor.png"), "PS2");
	IMGMANAGER->AddImage("LobbyLocalcoop", _T("Font/lobby_localcoop.png"), "PS2");
	IMGMANAGER->AddImage("LobbyBoss", _T("Font/lobby_bosses.png"), "PS2");
	IMGMANAGER->AddImage("LobbyCharter", _T("Font/lobby_character.png"), "PS2");
	IMGMANAGER->AddImage("LobbyDailychallenge", _T("Font/lobby_dailychallenge.png"), "PS2");
	IMGMANAGER->AddImage("LobbySeed", _T("Font/lobby_seeded.png"), "PS2");
	IMGMANAGER->AddImage("LobbySelect", _T("Font/lobby_select.png"), "PS2");
	IMGMANAGER->AddImage("LobbyAllZoneMode", _T("Font/lobby_allzonesmode.png"), "PS2");
	IMGMANAGER->AddImage("LobbyAllZone", _T("Font/lobby_allzones.png"), "PS2");
	IMGMANAGER->AddImage("LobbyMode", _T("Font/lobby_mode.png"), "PS2");
	IMGMANAGER->AddImage("LobbyExtra", _T("Font/lobby_extramodes1.png"), "PS2");
	// 선택 이미지 추가
	IMGMANAGER->AddImage("LobbySelect", _T("Font/Select/game_lobby.png"), "PS2");
	IMGMANAGER->AddImage("QuickStartSelect", _T("Font/Select/options_quickrestart.png"), "PS2");
	// 보스 이미지 추가
	IMGMANAGER->AddImage("BossWall", _T("MapImg/BossWall.png"), 5, 1, "PS2");
	InstanceImgManager->AddImage("BossFloor", _T("MapImg/boss_floor_A.png"), 3, 2, 100 * 100);
	IMGMANAGER->AddImage("Boss", _T("boss/king_conga.png"), 7, 2, "PS2");
	IMGMANAGER->AddImage("BossChair", _T("boss/king_conga_throne.png"), "PS2");
	IMGMANAGER->AddImage("BottomBlade", _T("boss/bg_bottomblade.png"), "PS2");
	IMGMANAGER->AddImage("BgGradient", _T("boss/bg_gradient.png"), "PS2");
	IMGMANAGER->AddImage("TopBlade", _T("boss/bg_topblade.png"), "PS2");
	IMGMANAGER->AddImage("TopBlade1", _T("boss/bg_topblade1.png"), "PS2");
	IMGMANAGER->AddImage("BossName", _T("boss/boss_kingcongasplash.png"), "PS2");
	IMGMANAGER->AddImage("BossScreen", _T("boss/bosscreen_kingconga.png"), "PS2");
	IMGMANAGER->AddImage("BossLine", _T("boss/TEMP_conga_line.png"), "PS2");
	// 배경음 추가
	SOUNDMANAGER->addSound("Zone1", _T("sound/zone1_1.mp3"), true);
	SOUNDMANAGER->addSound("ShopKeeper", _T("sound/zone1_1_shopkeeper.ogg"), true);
	SOUNDMANAGER->addSound("LobbySound", _T("sound/lobby.mp3"), true);
	// 효과음 추가
	SOUNDMANAGER->addSound("OpenDoor", "sound/sfx/obj_door_open.ogg", false);
	SOUNDMANAGER->addSound("CoinPickUp", "sound/sfx/sfx_pickup_gold_01.ogg", false);
	SOUNDMANAGER->addSound("WallFail", "sound/sfx/mov_dig_fail.ogg", false);
	SOUNDMANAGER->addSound("WallBrick", "sound/sfx/mov_dig_brick.ogg", false);
	SOUNDMANAGER->addSound("Attack", "sound/sfx/sfx_player_hit_ST.ogg", false);
	SOUNDMANAGER->addSound("Death", "sound/sfx/sfx_player_death_ST.ogg", false);
	SOUNDMANAGER->addSound("SkeletonAttack", "sound/sfx/en_skel_attack_melee.ogg", false);
	SOUNDMANAGER->addSound("SkeletonDeath", "sound/sfx/en_skel_death.ogg", false);
	SOUNDMANAGER->addSound("SkeletonHit", "sound/sfx/en_skel_hit.ogg", false);
	SOUNDMANAGER->addSound("BossAnnouncer", "sound/sfx/vo_announcer_kingconga.ogg", false);
	SOUNDMANAGER->addSound("BossAttack", "sound/sfx/en_kingconga_attack_01.ogg", false);
	SOUNDMANAGER->addSound("BossCry", "sound/sfx/en_kingconga_cry_01.ogg", false);
	SOUNDMANAGER->addSound("BossDeath", "sound/sfx/en_kingconga_death.ogg", false);
	SOUNDMANAGER->addSound("BossHit", "sound/sfx/en_kingconga_hit_01.ogg", false);
	SOUNDMANAGER->addSound("SlimeAttack", "sound/sfx/en_slime_attack.ogg", false);
	SOUNDMANAGER->addSound("SlimeDeath", "sound/sfx/en_slime_death.ogg", false);
	SOUNDMANAGER->addSound("SlimeHit", "sound/sfx/en_slime_hit.ogg", false);
	SOUNDMANAGER->addSound("JombieAttack", "sound/sfx/en_zombie_attack.ogg", false);
	SOUNDMANAGER->addSound("JombieDeath", "sound/sfx/en_zombie_death.ogg", false);
	SOUNDMANAGER->addSound("UISelectUp", "sound/sfx/sfx_ui_select_up.ogg", false);
	SOUNDMANAGER->addSound("UISelectDown", "sound/sfx/sfx_ui_select_down.ogg", false);
	SOUNDMANAGER->addSound("UISelectBack", "sound/sfx/sfx_ui_back.ogg", false);
	SOUNDMANAGER->addSound("UISelectStart", "sound/sfx/sfx_ui_start.ogg", false);
	SOUNDMANAGER->addSound("UISelectToggle", "sound/sfx/sfx_ui_toggle.ogg", false);
	SOUNDMANAGER->addSound("MinotaursAttack", "sound/sfx/en_minotaur_attack.ogg", false);
	SOUNDMANAGER->addSound("MinotaursDeath", "sound/sfx/en_minotaur_death.ogg", false);
	SOUNDMANAGER->addSound("MinotaursHit", "sound/sfx/en_minotaur_hit.ogg", false);
	SOUNDMANAGER->addSound("CheastOpen", "sound/sfx/obj_chest_open.ogg", false);
	SOUNDMANAGER->addSound("Trapbounce", "sound/sfx/obj_trap_bounce.ogg", false);
	SOUNDMANAGER->addSound("Trapspike", "sound/sfx/obj_trap_spike.ogg", false);
}


