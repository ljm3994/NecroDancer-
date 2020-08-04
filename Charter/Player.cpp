#include "stdafx.h"
#include "Player.h"
#include "Map/Shop.h"

void Player::MoveCheck(ST_TILE * Tile, MOVEDIR dir)
{
	if (Tile != nullptr)
	{
		if (Tile->Property == TileProperty::PASS)
		{
			if (m_Cnt <= 0)
			{
				switch (dir)
				{
				case MOVE_LEFT:
					m_Pos.x -= 1;
					break;
				case MOVE_RIGHT:
					m_Pos.x += 1;
					break;
				case MOVE_UP:
					m_Pos.y += 1;
					break;
				case MOVE_DOWN:
					m_Pos.y -= 1;
					break;
				}
				m_Cnt = 50;
				m_Jump = 0;
				m_TileCheck = false;
			}
		}
		else if (Tile->Object == TileObject::OBJECT_DOOR)
		{
			SOUNDMANAGER->play("OpenDoor", GAMEMANAGER->GetsfxVolum());
			m_IsEarthquake = true;
			Tile->ImageKey = "";
			Tile->Object = TileObject::OBJECT_EMPTY;
			Tile->Property = TileProperty::PASS;
		}
		else if (Tile->Object3 == TileObject::OBJECT_ENEMY)
		{
			Attack();
			((Weppon *)ITEMMANAGER->FindItem(m_inven[1].ItemKey))->WepponAttack(dir);
		}
		else if (Tile->Object == TileObject::OBJECT_WALL
			|| Tile->Object == TileObject::OBJECT_BROKENWALL ||
			Tile->Object == TileObject::OBJECT_TWOBROKENWALL ||
			Tile->Object == TileObject::OBJECT_SHOPWALL)
		{
			ITEMMANAGER->FindItem(m_inven[0].ItemKey)->Setm_IsEffect(true);
		}
		else if(Tile->Object2 == TileObject::OBJECT_SHOPITEM)
		{
			if (MAPMANAGER->Getm_Shop()->BuyItem(Tile->ImagKey2, this))
			{
				Tile->Object2 = TileObject::OBJECT_EMPTY;
				Tile->ImagKey2 = "";
				Tile->Property = PASS;
			}
		}
		else if (Tile->Object == TileObject::OBJECT_LOCKSTAIRS)
		{
			if (GAMEMANAGER->Getm_CurrentLevel() > -1)
			{
				if (m_inven[6].ItemKey != "")
				{
					Tile->Object = TileObject::OBJECT_STAIRS;
					Tile->ImageKey = "Stairs";
					Tile->Property = PASS;
				}
			}
		}
		else if (Tile->Object == TileObject::OBJECT_ITEMBOX)
		{
			TRAPMANAGER->FindTrap(Tile->ImageKey)->TrapDown(this);
		}
	}
}

bool Player::Die()
{
	if (m_Life <= 0 && !m_Mujeck)
	{
		m_Alive = false;
		SOUNDMANAGER->play("Death", GAMEMANAGER->GetsfxVolum());
	}
	return true;
}

Player::Player()
{
}


Player::~Player()
{
}

HRESULT Player::init()
{
	m_Head = IMGMANAGER->GetImage("PlayerHead");
	m_Body = IMGMANAGER->GetImage("PlayerBody");
	m_Shadow = IMGMANAGER->GetImage("PlayerShadow");
	m_DelayTime = 0.0f;
	m_Life = 6;
	m_Mujeck = false;
	m_BodyAniDelayTime = 0.0f;
	m_IsShove = true;
	m_MaxLife = 6;
	m_EarthquakeCnt = 0;
	m_IsEarthquake = false;

	m_FramX = 0;
	m_FramY = 0;
	m_EarthquakeDelay = 0.0f;
	Dir = MOVE_NO;
	m_Cnt = 0;
	ITEMMANAGER->CreateItem();
	m_inven = new Inventory[8];
	m_inven[0].Kind = INVEN_SHOVEL;
	m_inven[0].ItemKey = "ShovelBasic";
	m_inven[1].Kind = INVEN_WEPON;
	m_inven[1].ItemKey = "Dagger";
	m_inven[2].Kind = INVEN_ARMOR;
	m_inven[3].Kind = INVEN_HEAD;
	m_inven[4].Kind = INVEN_FEET;
	m_inven[5].Kind = INVEN_POSION;
	m_inven[6].Kind = INVEN_KEY;
	m_inven[7].Kind = INVEN_PACK;
	m_Alive = true;
	m_Coin = 2000;
	m_Diamond = 0;
	m_Jump = 0;
	m_PlayerSight = 2;
	m_PlayerKeyOn = false;
	return S_OK;
}

void Player::release()
{
}

void Player::update()
{
	if (m_Alive)
	{
		m_PlayerKeyOn = false;
		PlayerRayCast();
		string ImgKey = "";
		Earthquake();
		if (Dir != MOVE_NO)
		{
			Move(Dir);
		}
		ST_TILE * TileTemp = nullptr;
		ST_TILE * TileTemp2 = nullptr;

		if (KEYMANAGER->isKeyDown(VK_LEFT))
		{
			RatateY = PI;
			if (m_BeatTime || m_PerfectBeat)
			{
				if (m_PerfectBeat)
				{
					ImgKey = "PerfectImg";
				}
				TileTemp = MAPMANAGER->GetMap(static_cast<int>(m_Pos.x) - 1, static_cast<int>(m_Pos.y));

				Dir = MOVE_LEFT;
				MoveCheck(TileTemp, Dir);
				m_BeatTime = false;
				m_PerfectBeat = false;
			}
			m_PlayerKeyOn = true;
		}
		else if (KEYMANAGER->isKeyDown(VK_RIGHT))
		{
			if (KEYMANAGER->isKeyUp(VK_LEFT))
			{
				RPLUSL = true;
				return;
			}
			RatateY = 0.0f;
			if (m_BeatTime || m_PerfectBeat)
			{
				if (m_PerfectBeat)
				{
					ImgKey = "PerfectImg";
				}
				TileTemp = MAPMANAGER->GetMap(static_cast<int>(m_Pos.x) + 1, static_cast<int>(m_Pos.y));
				Dir = MOVE_RIGHT;
				MoveCheck(TileTemp, Dir);
				m_BeatTime = false;
				m_PerfectBeat = false;
			}
			m_PlayerKeyOn = true;
		}
		else if (KEYMANAGER->isKeyDown(VK_UP))
		{
			if (m_BeatTime || m_PerfectBeat)
			{
				TileTemp = MAPMANAGER->GetMap(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y) + 1);
				Dir = MOVE_UP;
				MoveCheck(TileTemp, Dir);
				m_BeatTime = false;
				m_PerfectBeat = false;
			}
			m_PlayerKeyOn = true;
		}
		else if (KEYMANAGER->isKeyDown(VK_DOWN))
		{
			if (m_BeatTime || m_PerfectBeat)
			{
				TileTemp = MAPMANAGER->GetMap(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y) - 1);
				Dir = MOVE_DOWN;
				MoveCheck(TileTemp, Dir);
				m_BeatTime = false;
				m_PerfectBeat = false;
			}
			m_PlayerKeyOn = true;
		}

		
		for (int i = 0; i < 5; i++)
		{
			if (m_inven[i].ItemKey != "")
			{
				ITEMMANAGER->FindItem(m_inven[i].ItemKey)->Update();
			}
		}
		CurrentTile = MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y));
		CurrentTile->Property = NONPASS;
		CurrentTile->Object3 = OBJECT_PLAYER;

		if (KEYMANAGER->isKeyDown(VK_F5))
		{
			m_Pos = MAPMANAGER->GetKeyPos();
			CurrentTile->Object3 = OBJECT_EMPTY;
			CurrentTile->ImageKey = "";
			CurrentTile->Property = PASS;
			m_ScreenPos.x = (m_Pos.x * 50);
			m_ScreenPos.y = (m_Pos.y * 50);
			g_ptCam.x = m_ScreenPos.x - (WINSIZEX / 2);
			g_ptCam.y = m_ScreenPos.y - (WINSIZEY / 2);
		}
		if (KEYMANAGER->isKeyDown(VK_F6))
		{
			m_Pos = MAPMANAGER->GetExitPos();
			CurrentTile->Object3 = OBJECT_EMPTY;
			CurrentTile->ImageKey = "";
			CurrentTile->Property = PASS;
			m_ScreenPos.x = (m_Pos.x * 50);
			m_ScreenPos.y = (m_Pos.y * 50);
			g_ptCam.x = m_ScreenPos.x - (WINSIZEX / 2);
			g_ptCam.y = m_ScreenPos.y - (WINSIZEY / 2);
		}
		if (KEYMANAGER->isKeyDown(VK_F7))
		{
			MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_Pos.x + 1), static_cast<int>(m_Pos.y), 0, 0, MODE_DRAW, "BorodSword", true, PASS, OBJECT_ITEM);
		}
		if (KEYMANAGER->isKeyDown(VK_F8))
		{
			m_Mujeck = true;
		}
		Die();
	}
}

void Player::render()
{
	if (m_Alive)
	{
		m_BodyAniDelayTime += g_ETime;

		if (m_BodyAniDelayTime > 0.2f)
		{
			m_BodyAniDelayTime = 0.0f;
			m_FramX++;
			if (m_FramX > 3)
			{
				m_FramX = 0;
			}
		}
		m_Shadow->Scale(48, 54);
		m_Shadow->Position(m_ScreenPos.x, m_ScreenPos.y + 15);
		m_Shadow->Color(0, 0, 0, 0.6f);
		m_Shadow->Render();
		if (GAMEMANAGER->Getm_SkinName() == "Jp3")
		{
			m_FramY = 0;
		}
		m_Body = IMGMANAGER->GetImage(GAMEMANAGER->Getm_SkinName() + "_Body");
		m_Body->Scale(50, 50);
		m_Body->Position(m_ScreenPos.x, m_ScreenPos.y + 15 + m_Jump);
		m_Body->Rotate(0, RatateY, 0);
		m_Body->GetRefCurrentFrameX() = m_FramX;
		m_Body->GetRefCurrentFrameY() = m_FramY;
		m_Body->Render();
		m_Head = IMGMANAGER->GetImage(GAMEMANAGER->Getm_SkinName() + "_Head");
		m_Head->Scale(50, 50);
		m_Head->Rotate(0, RatateY, 0);
		m_Head->Position(m_ScreenPos.x, m_ScreenPos.y + 15 + m_Jump);
		m_Head->GetRefCurrentFrameX() = m_FramX;
		m_Head->GetRefCurrentFrameY() = 0;
		m_Head->Render();
	}
}

void Player::Move(MOVEDIR dir)
{
	switch (dir)
	{
	case MOVE_LEFT:
		m_Cnt -= (300 * g_ETime);
		m_ScreenPos.x -= (300 * g_ETime);
		g_ptCam.x -= (300 * g_ETime);
		break;
	case MOVE_RIGHT:
		m_Cnt -= (300 * g_ETime);
		m_ScreenPos.x += (300 * g_ETime);
		g_ptCam.x += (300 * g_ETime);
		break;
	case MOVE_DOWN:
		m_Cnt -= (300 * g_ETime);
		m_ScreenPos.y -= (300 * g_ETime);
		g_ptCam.y -= (300 * g_ETime);
		break;
	case MOVE_UP:
		m_Cnt -= (300 * g_ETime);
		m_ScreenPos.y += (300 * g_ETime);
		g_ptCam.y += (300 * g_ETime);
		break;
	}


	if (m_Cnt)
	{
		if (m_Cnt > (50 / 2))
		{
			m_Jump += (200 * g_ETime);
		}
		else
		{
			m_Jump -= (200 * g_ETime);
		}
	}

	if (m_Cnt < 0)
	{
		switch (dir)
		{
		case MOVE_LEFT:
			if (MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x + 1), static_cast<int>(m_Pos.y))->Object3 == OBJECT_PLAYER 
				|| MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x + 1), static_cast<int>(m_Pos.y))->Object == OBJECT_TRAP)
			{
				MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x + 1), static_cast<int>(m_Pos.y), TileObject::OBJECT_EMPTY, TileProperty::PASS, "");
			}
			break;
		case MOVE_RIGHT:
			if (MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x - 1), static_cast<int>(m_Pos.y))->Object3 == OBJECT_PLAYER
				|| MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x - 1), static_cast<int>(m_Pos.y))->Object == OBJECT_TRAP)
			{
				MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x - 1), static_cast<int>(m_Pos.y), TileObject::OBJECT_EMPTY, TileProperty::PASS, "");
			}
			break;
		case MOVE_DOWN:
			if (MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y + 1))->Object3 == OBJECT_PLAYER
				|| MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y + 1))->Object == OBJECT_TRAP)
			{
				MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y + 1), TileObject::OBJECT_EMPTY, TileProperty::PASS, "");
			}
			break;
		case MOVE_UP:
			if (MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y - 1))->Object3 == OBJECT_PLAYER
				|| MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y - 1))->Object == OBJECT_TRAP)
			{
				MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y - 1), TileObject::OBJECT_EMPTY, TileProperty::PASS, "");
			}
			break;
		}

		TileCheck();
		m_ScreenPos.x = (m_Pos.x * 50);
		m_ScreenPos.y = (m_Pos.y * 50);
		g_ptCam.x = m_ScreenPos.x - (WINSIZEX / 2);
		g_ptCam.y = m_ScreenPos.y - (WINSIZEY / 2);
		m_Jump = 0;
		Dir = MOVE_NO;
	}
}

void Player::Attack()
{
	SOUNDMANAGER->play("Attack", GAMEMANAGER->GetsfxVolum());
	m_IsEarthquake = true;
	ITEMMANAGER->FindItem(m_inven[1].ItemKey)->Setm_IsEffect(true);
}

void Player::Damage(int Attack)
{
	Deffence(Attack);
	m_Life -= Attack;

	if (Attack > 0)
	{
		MAPMANAGER->SetFever(false);
	}
}

void Player::Deffence(int & Attack)
{
	int p = Attack;
	p = Attack - m_Deffence;
	if (p < 0)
	{
		p = 0;
	}
	Attack = p;
}

void Player::SetInvenTory(int Num, string str)
{
	m_inven[Num].ItemKey = str;
}

void Player::TileCheck()
{
	if (!m_TileCheck)
	{
		if (CurrentTile->Object2 == TileObject::OBJECT_ITEM || CurrentTile->Object2 == OBJECT_KEY)
		{
			if (ITEMMANAGER->FindItem(CurrentTile->ImagKey2)->GetItemKind() != INVEN_EMPTY)
			{
				ITEMMANAGER->FindItem(CurrentTile->ImagKey2)->GetItem(this);
				
			}
			else
			{
				ITEMMANAGER->FindItem(CurrentTile->ImagKey2)->GetItem(this);
				CurrentTile->Object2 = TileObject::OBJECT_EMPTY;
				CurrentTile->ImagKey2 = "";
			}
		}

		else if (CurrentTile->Object == TileObject::OBJECT_STAIRS)
		{
			if (GAMEMANAGER->Getm_CurrentLevel() > -1)
			{
				if (GAMEMANAGER->Getm_Mode() == MODE_STORY)
				{
					if (GAMEMANAGER->Getm_CurrentLevel() == 5)
					{
						SOUNDMANAGER->stop(BEATMANAGER->GetKey());
						GAMEMANAGER->Setm_CurrentLevel(-1);
						SCENEMANAGER->ChangeScene("Loading");
						m_TileCheck = true;
						return;
					}
				}
				ITEMMANAGER->FindItem("LockKey")->SetInventory(false);
				m_inven[ITEMMANAGER->FindItem("LockKey")->GetItemKind() - 1].ItemKey = "";
				SOUNDMANAGER->stop(BEATMANAGER->GetKey());

				if (GAMEMANAGER->Getm_Mode() == MODE_STORY && GAMEMANAGER->Getm_CurrentLevel() == 4)
				{
					GAMEMANAGER->GetLevel()[1].m_IsUse = true;
					SCENEMANAGER->ChangeScene("BossStage");
					GAMEMANAGER->Setm_CurrentLevel(5);
				}
				else if(GAMEMANAGER->Getm_Mode() == MODE_STORY && GAMEMANAGER->Getm_CurrentLevel() != 4)
				{
					SCENEMANAGER->ChangeScene("Loading");
					GAMEMANAGER->GetLevel()[GAMEMANAGER->Getm_CurrentLevel() + 4].m_IsUse = true;
					GAMEMANAGER->Setm_CurrentLevel(GAMEMANAGER->Getm_CurrentLevel() + 1);
				}
				else if (GAMEMANAGER->Getm_Mode() == MODE_INFINITY)
				{
					SCENEMANAGER->ChangeScene("Loading");
					GAMEMANAGER->Setm_CurrentLevel(GAMEMANAGER->Getm_CurrentLevel() + 1);
				}
			}
		}
		else if (CurrentTile->Object == TileObject::OBJECT_TRAP)
		{
			TRAPMANAGER->FindTrap(CurrentTile->ImageKey)->TrapDown(this);
		}
		m_TileCheck = true;
	}
}

void Player::PlayerInitPos()
{
	vector<Room *> Rooms = MAPMAKERMANAGER->GetRooms();
	int Index = GetInt(Rooms.size() - 1);
	RatateY = 0.0f;
	Room * SelectRoom = Rooms[Index];
	SelectRoom->m_Property = ROOM_PLAYER;
	m_Pos.x = static_cast<float>(SelectRoom->m_X + (SelectRoom->m_Width / 2));
	m_Pos.y = static_cast<float>(SelectRoom->m_Y + (SelectRoom->m_Height / 2));
	CurrentTile = MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y));
	CurrentTile->Object3 = OBJECT_PLAYER;
	CurrentTile->Property = NONPASS;
	m_ScreenPos.x = (m_Pos.x * 50);
	m_ScreenPos.y = (m_Pos.y * 50);
}

void Player::PlayerRayCast()
{
	// 광선을 발사
	// 광선은 오브젝트들의 밝기 값을 자신의 가중치 만큼 올린다
	// 가중치는 캐릭터가 보유한 밝기 값으로 한다.
	// 광선은 벽에 부딪히거나 가중치가 전부 소모되면 종료
	float lay = 0.0f;
	float max = static_cast<float>(m_PlayerSight) - 0.25f;

	float bright = 0.0f;  // 밝기
	float sightmax = 5.0f;  // 최대 시야 값
	float CurrentSight = 0.0f; // 현재 시야

	float r = 0.0f;
	float angle = (10.0f * PI) / 180.0f; // 광선 발사 각

	while (r < 2 * PI)
	{
		if (lay > max)
			bright = 0.0f;
		else
			bright = lay;

		if (CurrentSight > sightmax)
		{
			CurrentSight = 0.0f;
			bright = 0.0f;
			lay = 0.0f;
			r += angle;
		}

		float X = m_Pos.x + CurrentSight * cos(r) + 0.5f;   //각도 계산
		float y = m_Pos.y + CurrentSight * sin(r) + 0.5f;

		if (X < 0 || y < 0)          // x,y 좌표 둘중 하나라도 음수이면
		{
			CurrentSight = sightmax + 0.1f;
			continue;
		}
		if (X > MAPMANAGER->Getm_Width() - 1 || y > MAPMANAGER->Getm_Height() - 1) //광선이 맵을 벗어낫을 경우 예외처리
		{
			CurrentSight = sightmax + 0.1f;
			continue;
		}
		int a = static_cast<int>(X - m_Pos.x);
		if (a < 0)
		{
			a *= -1;
		}
		int b = static_cast<int>(y - m_Pos.y);
		if (b < 0)
		{
			b *= -1;
		}
		int c = a + b;
		// 맵 타일 이미지의 알파값을 업데이트 해준다.
		int val = MAPMANAGER->PlayerRayUpdate(static_cast<int>(X), static_cast<int>(y), c, m_PlayerSight);

		if (val == 1) // 광선이 벽과 같은 오브젝트에 부딪혔다면 초기화 해준다
		{
			lay = 0.0f;
			CurrentSight = 0.0f;
			r += angle;
		}
		else if (val == 2)  // 광선이 맵 타일에 부딪혔으면 시야와 광선값을 증가시킨다.
		{
			lay += 0.25f;
			CurrentSight += 0.25f;
		}
	}
}

void Player::Earthquake()
{
	if (m_IsEarthquake)
	{
		if (m_EarthquakeCnt > 15)
		{
			m_IsEarthquake = false;
			m_EarthquakeCnt = 0;
			g_ptCam.x = (m_Pos.x * 50) - (WINSIZEX / 2);
			g_ptCam.y = (m_Pos.y * 50) - (WINSIZEY / 2);
		}
		else
		{
			m_EarthquakeDelay += g_ETime;

			if (m_EarthquakeDelay > 0.05f)
			{
				int X = GetFromIntTo(6, 10);
				int Y = GetFromIntTo(6, 10);

				if (m_EarthquakeCnt % 2 == 0)
				{
					g_ptCam.x += X;
					g_ptCam.y += Y;
				}
				else
				{
					g_ptCam.x -= X;
					g_ptCam.y -= Y;
				}
				m_EarthquakeDelay = 0.0f;
				m_EarthquakeCnt++;
			}
		}
	}
}





