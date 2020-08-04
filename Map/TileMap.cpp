#include "stdafx.h"
#include "TileMap.h"
#include "Map/Shop.h"
\
TileMap::TileMap()
{
}


TileMap::~TileMap()
{

}

HRESULT TileMap::init()
{
	//임의의 초기값 만들어 주기
	Info.SetMaxTile(100, 100);
	Info.SetSizeTile(50.0f, 50.0f);
	Info.Setm_ZeroStartPos({ 0.0f, 0.0f });
	PassView = false;
	m_AllMapView = false;
	IndexView = false;
	ObjectView = false;
	TileView = false;
	//m_Tiles 는 더블포인터
	AniTime = 0.0f;
	int Index = 0;
	return S_OK;
}

void TileMap::release()
{
	if (m_Tiles)
	{
		/*for (UINT i = 0; i < Info.GetTileMaxY(); ++i)
		{
			if (m_Tiles[i])
			{
				delete[] m_Tiles[i];
			}
		}
		delete[] m_Tiles;*/
	}
}

void TileMap::update()
{
	if (KEYMANAGER->isKeyDown(VK_F2))
	{
		m_AllMapView = true;

	}
	if (KEYMANAGER->StayKeyDown('W'))
	{
		g_ptCam.y += 120.0f * g_ETime;

	}
	if (KEYMANAGER->StayKeyDown('S'))
	{
		g_ptCam.y -= 120.0f * g_ETime;

	}
	if (KEYMANAGER->StayKeyDown('D'))
	{
		g_ptCam.x += 120.0f * g_ETime;

	}
	if (KEYMANAGER->StayKeyDown('A'))
	{
		g_ptCam.x -= 120.0f * g_ETime;


	}
	TerianAlphaUpdate();	
}
// 타일 랜더 컬링 작업 적용
void TileMap::render()
{
	PtInTile(StartIndexX, StartIndexY, Info, D3DXVECTOR2(g_ptCam.x, g_ptCam.y)); // 카메가 좌표와 타일 충돌 계산(시작위치)
	PtInTile(EndIndexX, EndIndexY, Info, D3DXVECTOR2(WINSIZEX + g_ptCam.x, WINSIZEY + g_ptCam.y)); // 끝 위치

	StartIndexX = max(0, StartIndexX);
	StartIndexX = min(static_cast<int>(Info.GetTileMaxX()) - 1, StartIndexX);

	StartIndexY = max(0, StartIndexY);
	StartIndexY = min(static_cast<int>(Info.GetTileMaxY()) - 1, StartIndexY);

	EndIndexX = max(0, EndIndexX);
	EndIndexX = min(static_cast<int>(Info.GetTileMaxX()) - 1, EndIndexX);

	EndIndexY = max(0, EndIndexY);
	EndIndexY = min(static_cast<int>(Info.GetTileMaxY()) - 1, EndIndexY);

	Image * img = IMGMANAGER->GetImage(m_Tiles[0][0].ImageKey);
	Image * img2 = IMGMANAGER->GetImage(m_Tiles[0][0].ImagKey2);
	string prevImgKey = m_Tiles[0][0].ImageKey;
	string prevImgKey2 = m_Tiles[0][0].ImagKey2;
	m_TerianTex->Render();
	if (GAMEMANAGER->Getm_CurrentLevel() == -1)
	{
		vector<StageLevel> Level = GAMEMANAGER->GetLevel();
		Image * img = IMGMANAGER->GetImage("LobbyLocalcoop");
		img->Position(static_cast<float>(Level[0].XIndex * 50), static_cast<float>((Level[0].YIndex * 50) + 50));
		img->Scale(85, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyBoss");
		img->Position(static_cast<float>(Level[1].XIndex * 50), static_cast<float>((Level[1].YIndex * 50) + 40));
		img->Scale(40, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyLeveleditor");
		img->Position(static_cast<float>(Level[2].XIndex * 50), static_cast<float>((Level[2].YIndex * 50) + 50));
		img->Scale(85, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyZone1");
		img->Position(static_cast<float>(Level[3].XIndex * 50), static_cast<float>((Level[3].YIndex * 50) + 40));
		img->Scale(40, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyZone2");
		img->Position(static_cast<float>(Level[4].XIndex * 50), static_cast<float>((Level[4].YIndex * 50) + 40));
		img->Scale(40, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyZone3");
		img->Position(static_cast<float>(Level[5].XIndex * 50), static_cast<float>((Level[5].YIndex * 50) + 40));
		img->Scale(40, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyZone4");
		img->Position(static_cast<float>(Level[6].XIndex * 50), static_cast<float>((Level[6].YIndex * 50) + 40));
		img->Scale(40, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyAllZone");
		img->Position(static_cast<float>(Level[7].XIndex * 50), static_cast<float>((Level[7].YIndex * 50) + 60));
		img->Scale(70, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyMode");
		img->Position(static_cast<float>(Level[7].XIndex * 50), static_cast<float>((Level[7].YIndex * 50) + 40));
		img->Scale(30, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyCharter");
		img->Position(static_cast<float>(Level[8].XIndex * 50), static_cast<float>((Level[8].YIndex * 50) + 60));
		img->Scale(60, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbySelect");
		img->Position(static_cast<float>(Level[8].XIndex * 50), static_cast<float>((Level[8].YIndex * 50) + 40));
		img->Scale(40, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyTutorial");
		img->Position(static_cast<float>(Level[9].XIndex * 50), static_cast<float>((Level[9].YIndex * 50) + 40));
		img->Scale(60, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyExtra");
		img->Position(static_cast<float>(Level[10].XIndex * 50), static_cast<float>((Level[10].YIndex * 50) + 60));
		img->Scale(60, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyMode");
		img->Position(static_cast<float>(Level[10].XIndex * 50), static_cast<float>((Level[10].YIndex * 50) + 40));
		img->Scale(30, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyDailychallenge");
		img->Position(static_cast<float>(Level[11].XIndex * 50), static_cast<float>((Level[11].YIndex * 50) + 40));
		img->Scale(100, 20);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbyAllZoneMode");
		img->Position(static_cast<float>(Level[12].XIndex * 50), static_cast<float>((Level[12].YIndex * 50) + 55));
		img->Scale(100, 15);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
		img = IMGMANAGER->GetImage("LobbySeed");
		img->Position(static_cast<float>(Level[12].XIndex * 50), static_cast<float>((Level[12].YIndex * 50) + 35));
		img->Scale(80, 15);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
	}
	float m_Alpha = 0.0f;
	for (int i = EndIndexX; i >= StartIndexX; i--)        //끝 인덱스부터 시작 인덱스까지
	{
		for (int j = EndIndexY; j >= StartIndexY; j--)
		{
			if (m_Tiles[i][j].Property != TileProperty::EMPTY)       // 타일 속성이 존재한다면
			{
				if (m_Tiles[i][j].m_IsPlayerFirstLight || m_AllMapView) //플레이어가 아직 지나가지 않은 타일인지 여부 확인
				{
					if (m_Tiles[i][j].m_IsPlayerSight)   // 타일을 그리기 전 먼저 시야에 따른 알파값을 계산한다.
					{
						if (!m_Tiles[i][j].m_IsPlayerLight)
						{
							m_Alpha = 0.2f;
						}
						else
						{
							m_Alpha = 1.0f;
						}
					}
					else
					{
						m_Alpha = 0.2f;
					}
					if (m_Tiles[i][j].Object != TileObject::OBJECT_EMPTY && m_Tiles[i][j].Object != TileObject::OBJECT_FLOOR)
					{
						if (m_Tiles[i][j].Object == TileObject::OBJECT_TRAP || m_Tiles[i][j].Object == TileObject::OBJECT_ITEMBOX)
						{
							TRAPMANAGER->FindTrap(m_Tiles[i][j].ImageKey)->Render(m_Alpha);
						}
						else
						{ 
							if (prevImgKey != m_Tiles[i][j].ImageKey)
							{
								img = IMGMANAGER->GetImage(m_Tiles[i][j].ImageKey);
								prevImgKey = m_Tiles[i][j].ImageKey;
								
							}
							if (m_Tiles[i][j].Object == TileObject::OBJECT_WALL || m_Tiles[i][j].Object == TileObject::OBJECT_SHOPWALL || m_Tiles[i][j].Object == TileObject::OBJECT_BROKENWALL || m_Tiles[i][j].Object == TileObject::OBJECT_TWOBROKENWALL)
							{
								img->Scale(Info.GetTileWidth(), Info.GetTileHeight() + 25);
							}
							else if (m_Tiles[i][j].ImageKey == "DoorSide" || m_Tiles[i][j].ImageKey == "LockDoorSide")
							{
								img->Scale(Info.GetTileWidth(), Info.GetTileHeight() + 25);
							}
							else if (m_Tiles[i][j].Object == TileObject::OBJECT_ETC)
							{
								img->Scale(Info.GetTileWidth() + 50, Info.GetTileHeight() + 100);
							}
							else
							{
								img->Scale(Info.GetTileWidth(), Info.GetTileHeight());
							}

							img->Color(0, 0, 0, m_Alpha);
							img->Position(m_Tiles[i][j].Xpos, m_Tiles[i][j].Ypos);
							img->GetRefCurrentFrameX() = m_Tiles[i][j].CurrentFrameX;
							img->GetRefCurrentFrameY() = m_Tiles[i][j].CurrentFrameY;
							img->Render();
							
						}

					}

					if (m_Tiles[i][j].Object2 != TileObject::OBJECT_EMPTY)
					{
						if(m_Tiles[i][j].Object != TileObject::OBJECT_FLOOR)
						{
							if (m_Tiles[i][j].ImagKey2 == "Torch")
							{
								if (prevImgKey2 != m_Tiles[i][j].ImagKey2)
								{
									prevImgKey2 = m_Tiles[i][j].ImagKey2;
									img2 = IMGMANAGER->GetImage(m_Tiles[i][j].ImagKey2);
								}
								img2->Color(0, 0, 0, m_Alpha);
								img2->Scale(Info.GetTileWidth() - 25, Info.GetTileHeight() + 50);
								img2->Position(m_Tiles[i][j].Xpos, m_Tiles[i][j].Ypos + 50);
								img2->PlayAnimation(ANI_LOOP, true, 0.7f);
							}
							else if (m_Tiles[i][j].Object2 == OBJECT_ITEM || m_Tiles[i][j].Object2 == OBJECT_KEY)
							{
								ITEMMANAGER->FindItem(m_Tiles[i][j].ImagKey2)->TileItemRender(D3DXVECTOR2(m_Tiles[i][j].Xpos, m_Tiles[i][j].Ypos), m_Alpha);
							}
							else if (m_Tiles[i][j].Object2 == OBJECT_SHOPITEM)
							{
								ITEMMANAGER->FindItem(m_Tiles[i][j].ImagKey2)->TileItemRender(D3DXVECTOR2(m_Tiles[i][j].Xpos, m_Tiles[i][j].Ypos), m_Alpha);
								MAPMANAGER->Getm_Shop()->PriceRender(D3DXVECTOR2(static_cast<float>(i), static_cast<float>(j)), m_Tiles[i][j].ImagKey2, m_Alpha);
							}
						}
					}
					if (m_Tiles[i][j].Object3 != TileObject::OBJECT_EMPTY)
					{
						if (m_Tiles[i][j].Object3 == TileObject::OBJECT_ENEMY)
						{
							CHARTERMANAGER->EnemyRender(m_Tiles[i][j].ImageKey3, m_Alpha);
						}
						else if (m_Tiles[i][j].Object3 == TileObject::OBJECT_PLAYER)
						{
							CHARTERMANAGER->GetPlayer()->render();
						}
						else if(m_Tiles[i][j].Object3 == TileObject::OBJECT_SHOPKEEPER)
						{
							MAPMANAGER->Getm_Shop()->Render(m_Alpha);
						}
					}
					m_Tiles[i][j].m_IsPlayerSight = false;
					m_Tiles[i][j].m_IsPlayerLight = false;
				}
			}
		}
	}
}

void TileMap::BegineTileTexUpdate()
{
	m_TerianTex->MaxModel(0);
}

void TileMap::TexClear(UINT X, UINT Y, string TileName)
{
	m_TerianTex = InstanceImgManager->GetImage(TileName);
	m_TerianTex->Clear();
	m_TerianTex->SetOnlyMax(X * Y);
	m_TerianTex->MaxModel(0);
}

void TileMap::TexTileUpdate()
{
	m_TerianTex->Update();
}

void TileMap::SetMaxTile(UINT X, UINT Y, float W, float H, string TileName)
{
	Info.SetMaxTile(X, Y);
	Info.SetSizeTile(50.0f, 50.0f);
	Info.Setm_ZeroStartPos({ 0.0f, 0.0f });
	TexClear(X, Y, TileName);
	PassView = false;
	IndexView = false;
	ObjectView = false;
	TileView = false;
	m_IsBeate = false;
	m_IsFever = false;

	m_Tiles = new ST_TILE*[Info.GetTileMaxX()];
	for (int i = 0; i < static_cast<int>(Info.GetTileMaxX()); i++)
	{
		//m_Tiles[i] 는 포인터
		m_Tiles[i] = new ST_TILE[Info.GetTileMaxY()];
	}
	//m_Tiles 는 더블포인터
	AniTime = 0.0f;
	int Index = 0;
	DelayTime = 0.0f;
	for (UINT i = 0; i < Info.GetTileMaxX(); i++)
	{
		for (UINT j = 0; j < Info.GetTileMaxY(); j++)
		{
			m_Tiles[i][j].CurrentFrameX = 0;
			m_Tiles[i][j].CurrentFrameY = 0;
			m_Tiles[i][j].ImageKey = "TILE";
			m_Tiles[i][j].MaxFrameX = 20;
			m_Tiles[i][j].MaxFrameY = 8;
			m_Tiles[i][j].m_IsPlayerFirstLight = false;
			m_Tiles[i][j].m_IsPlayerLight = false;
			m_Tiles[i][j].m_IsPlayerSight = false;
			m_Tiles[i][j].Object = TileObject::OBJECT_EMPTY;
			m_Tiles[i][j].Object2 = TileObject::OBJECT_EMPTY;
			m_Tiles[i][j].Object3 = TileObject::OBJECT_EMPTY;
			m_Tiles[i][j].Property = TileProperty::EMPTY;
			m_Tiles[i][j].Xpos = Info.Getm_ZeroStartPos().x + i * Info.GetTileWidth();
			m_Tiles[i][j].Ypos = Info.Getm_ZeroStartPos().y + j * Info.GetTileHeight();
			Index = m_TerianTex->AddTexture();
			m_TerianTex->Scale(Index, Info.GetTileWidth(), Info.GetTileHeight());
			m_TerianTex->Position(Index, m_Tiles[i][j].Xpos, m_Tiles[i][j].Ypos);
			m_TerianTex->TextureUV(Index, static_cast<float>(m_Tiles[i][j].CurrentFrameX), static_cast<float>(m_Tiles[i][j].CurrentFrameY));
			m_TerianTex->SetTextureUpdate(Index);
		}
	}

	m_TerianTex->Update();

}

int TileMap::AlphaMap(int x, int y, int Ray, int Sight)
{
	if (m_Tiles[x][y].m_IsPlayerSight == false)
	{
		if (Ray <= Sight)
		{
			m_Tiles[x][y].m_IsPlayerLight = true;
			m_Tiles[x][y].m_IsPlayerFirstLight = true;
			m_Tiles[x][y].m_IsPlayerSight = true;
		}
	}

	if (m_Tiles[x][y].Object == TileObject::OBJECT_WALL || m_Tiles[x][y].Object == TileObject::OBJECT_SHOPWALL 
		|| m_Tiles[x][y].Object == TileObject::OBJECT_DOOR || m_Tiles[x][y].Object == TileObject::OBJECT_LOCKDOOR
		|| m_Tiles[x][y].Object == TileObject::OBJECT_BROKENWALL || m_Tiles[x][y].Object == OBJECT_TWOBROKENWALL)
	{
		return 1;
	}

	if (m_TerianTex->GetSight(y + x * Info.GetTileMaxX()) == false)
	{
		if (Ray <= Sight)
		{
			m_TerianTex->SetLight(y + x * Info.GetTileMaxX(), true);
			m_TerianTex->SetFirstLight(y + x * Info.GetTileMaxX(), true);
			m_TerianTex->SetSight(y + x * Info.GetTileMaxX(), true);
		}
	}

	return 2;
}

void TileMap::TeriansetMap(int Xtile, int Ytile, LONG m_SelectFrameX, LONG m_SelectFramey, Mode m_Mode, string imgKey1, TileProperty Property, float Alpha)
{
	if ((Xtile >= 0 && Xtile < static_cast<int>(Info.GetTileMaxX())) && (Ytile >= 0 && Ytile < static_cast<int>(Info.GetTileMaxY())))
	{
		POINT n;
		n = { m_SelectFrameX, m_SelectFramey };

		if (Property != TileProperty::EMPTY && m_Mode == MODE_DRAW)
		{
			//현재버튼이 지형이냐?
			m_TerianTex->TextureUV(Ytile + Xtile * Info.GetTileMaxY(), static_cast<float>(n.x), static_cast<float>(n.y));
			m_TerianTex->SetTextureUpdate(Ytile + Xtile * Info.GetTileMaxY());
		}
		else if (m_Mode == MODE_REMOVE)
		{
			m_Tiles[Xtile][Ytile].ObjFrameX = 0;
			m_Tiles[Xtile][Ytile].ObjFrameY = 0;
			m_Tiles[Xtile][Ytile].CurrentFrameX = 0;
			m_Tiles[Xtile][Ytile].CurrentFrameY = 0;
			m_Tiles[Xtile][Ytile].ImageKey = "";
			m_Tiles[Xtile][Ytile].ImagKey2 = "";
			m_Tiles[Xtile][Ytile].Object = TileObject::OBJECT_EMPTY;
			Property = TileProperty::EMPTY;
		}
		if (m_Tiles[Xtile][Ytile].Object == TileObject::OBJECT_EMPTY && m_Tiles[Xtile][Ytile].Object3 == OBJECT_EMPTY && m_Tiles[Xtile][Ytile].Object2 == OBJECT_EMPTY)
		{
			m_Tiles[Xtile][Ytile].Property = TileProperty::PASS;
		}	
	}
}

void TileMap::ObjectsetMap(int Xtile, int Ytile, LONG m_SelectFrameX, LONG m_SelectFramey, Mode m_Mode, string imgKey1, bool ObjectAdd, TileProperty Property, TileObject ObjectType)
{
	if ((Xtile >= 0 && Xtile < static_cast<int>(Info.GetTileMaxX())) && (Ytile >= 0 && Ytile < static_cast<int>(Info.GetTileMaxY())))
	{
		POINT n;
		n = { m_SelectFrameX, m_SelectFramey };

		if (Property != TileProperty::EMPTY && m_Mode == MODE_DRAW)
		{
			if (!ObjectAdd)
			{
				m_Tiles[Xtile][Ytile].CurrentFrameX = n.x;
				m_Tiles[Xtile][Ytile].CurrentFrameY = n.y;
				m_Tiles[Xtile][Ytile].ImageKey = imgKey1;
				m_Tiles[Xtile][Ytile].Object = ObjectType;
			}
			else
			{
				m_Tiles[Xtile][Ytile].ObjFrameX = n.x;
				m_Tiles[Xtile][Ytile].ObjFrameY = n.y;
				m_Tiles[Xtile][Ytile].ImagKey2 = imgKey1;
				m_Tiles[Xtile][Ytile].Object2 = ObjectType;
			}

		}
		else if (m_Mode == MODE_REMOVE)
		{
			m_Tiles[Xtile][Ytile].ObjFrameX = 0;
			m_Tiles[Xtile][Ytile].ObjFrameY = 0;
			m_Tiles[Xtile][Ytile].CurrentFrameX = 0;
			m_Tiles[Xtile][Ytile].CurrentFrameY = 0;
			m_Tiles[Xtile][Ytile].ImageKey = "";
			m_Tiles[Xtile][Ytile].ImagKey2 = "";
			m_Tiles[Xtile][Ytile].Object = TileObject::OBJECT_EMPTY;
			m_Tiles[Xtile][Ytile].Object2 = TileObject::OBJECT_EMPTY;
			Property = TileProperty::EMPTY;
		}
	}

	m_Tiles[Xtile][Ytile].Property = Property;
}


void TileMap::TerianAlphaUpdate()
{
	PtInTile(StartIndexX, StartIndexY, Info, D3DXVECTOR2(g_ptCam.x, g_ptCam.y));
	PtInTile(EndIndexX, EndIndexY, Info, D3DXVECTOR2(WINSIZEX + g_ptCam.x, WINSIZEY + g_ptCam.y));

	StartIndexX = max(0, StartIndexX);
	StartIndexX = min(static_cast<int>(Info.GetTileMaxX()) - 1, StartIndexX);

	StartIndexY = max(0, StartIndexY);
	StartIndexY = min(static_cast<int>(Info.GetTileMaxY()) - 1, StartIndexY);

	EndIndexX = max(0, EndIndexX);
	EndIndexX = min(static_cast<int>(Info.GetTileMaxX()) - 1, EndIndexX);

	EndIndexY = max(0, EndIndexY);
	EndIndexY = min(static_cast<int>(Info.GetTileMaxY()) - 1, EndIndexY);
	BegineTileTexUpdate();
	DelayTime += g_ETime;

	for (int i = StartIndexX; i <= EndIndexX; i++)
	{
		for (int j = StartIndexY; j <= EndIndexY; j++)
		{
			if (m_TerianTex->GetFirstLight(j + i * Info.GetTileMaxX()))
			{
				if (m_TerianTex->GetSight(j + i * Info.GetTileMaxX()))
				{
					if (m_TerianTex->GetLight(j + i * Info.GetTileMaxX()))
					{
						m_TerianTex->Color(j + i * Info.GetTileMaxX(), 0, 0, 0, 1);
					}
					else
					{
						m_TerianTex->Color(j + i * Info.GetTileMaxX(), 0, 0, 0, 0.3f);
					}
				}
				else
				{
					m_TerianTex->Color(j + i * Info.GetTileMaxX(), 0, 0, 0, 0.3f);
				}
			}
			TerranMapBeat(i, j, DelayTime);
			m_TerianTex->SetSight(j + i * Info.GetTileMaxX(), false);
			m_TerianTex->SetLight(j + i * Info.GetTileMaxX(), false);
		}
	}
	if (DelayTime > 0.2f)
	{
		DelayTime = 0.0f;
	}
	if (m_IsBeate)
	{
		m_IsBeate = false;
	}
	m_TerianTex->Update();
}

void TileMap::LightOffMap(int StartX, int StartY, int EndX, int EndY)
{
	BegineTileTexUpdate();
	for (int i = StartX; i <= EndX; i++)
	{
		for (int j = StartY; j <= EndY; j++)
		{
			m_Tiles[i][j].m_IsPlayerFirstLight = false;
			m_TerianTex->Color(j + i * Info.GetTileMaxX(), 0, 0, 0, 0.0f);
			m_TerianTex->SetFirstLight(j + i * Info.GetTileMaxX(), false);
			m_TerianTex->SetSight(j + i * Info.GetTileMaxX(), false);
			m_TerianTex->SetLight(j + i * Info.GetTileMaxX(), false);
		}
	}
	m_TerianTex->Update();
}

bool TileMap::LoadMap(LPCSTR SaveName, string FloorKey)
{
	HANDLE file = nullptr;
	bool ReturnValue = false;
	DWORD read = 0;

	file = CreateFile(SaveName, GENERIC_READ, 0, NULL, OPEN_EXISTING,
		0, NULL);
	ZeroMemory(&Info, sizeof(MapInfo));
	ReturnValue = ReadFile(file, &Info, sizeof(MapInfo), &read, NULL);
	release();
	SetMaxTile(Info.GetTileMaxX(), Info.GetTileMaxY(), Info.GetTileWidth(), Info.GetTileHeight(), FloorKey);

	for (UINT i = 0; i < Info.GetTileMaxX(); i++)
	{
		for (UINT j = 0; j < Info.GetTileMaxY(); j++)
		{
			ReturnValue = ReadFile(file, &m_Tiles[i][j], sizeof(ST_TILE), &read, NULL);
		}
	}

	CloseHandle(file);
	return ReturnValue;
}

bool TileMap::SaveMap(LPCSTR SaveName)
{
	HANDLE file = nullptr;
	bool ReturnValue = false;
	DWORD write;

	file = CreateFile(SaveName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	ReturnValue = WriteFile(file, &Info, sizeof(MapInfo), &write, NULL);

	for (UINT i = 0; i < Info.GetTileMaxX(); i++)
	{
		ReturnValue = WriteFile(file, m_Tiles[i], sizeof(ST_TILE) * Info.GetTileMaxY(), &write, NULL);
	}

	CloseHandle(file);

	return ReturnValue;
	return true;
}

void TileMap::TerranMapBeat(int x, int y, float DelayTime)
{

	if (m_IsBeate)
	{
		int pos = (y + x * Info.GetTileMaxY());
		int FramX = m_TerianTex->GetTexturUvX(pos);
		int FramY = m_TerianTex->GetTexturUvY(pos);
		bool Partern = m_TerianTex->GetPatern(pos);

		if (x % 2 == 0)
		{
			if (y % 2 == 0)
			{
				m_TerianTex->SetPatern(pos, false);
				if (!Partern)
				{
					if (FramX == 0)
					{
						TeriansetMap(x, y, 1, FramY);
					}
					else
					{
						TeriansetMap(x, y, 0, FramY);
					}
				}
			}
			else
			{
				m_TerianTex->SetPatern(pos, true);
				if (Partern)
				{
					if (FramX == 0)
					{
						TeriansetMap(x, y, 2, FramY);
					}
					else
					{
						TeriansetMap(x, y, 0, FramY);
					}
				}
			}
		}
		else
		{
			if (y % 2 == 0)
			{
				m_TerianTex->SetPatern(pos, true);
				if (Partern)
				{
					if (FramX == 0)
					{
						TeriansetMap(x, y, 2, FramY);
					}
					else
					{
						TeriansetMap(x, y, 0, FramY);
					}
				}
			}
			else
			{
				m_TerianTex->SetPatern(pos, false);
				if (!Partern)
				{
					if (FramX == 0)
					{
						TeriansetMap(x, y, 1, FramY);
					}
					else
					{
						TeriansetMap(x, y, 0, FramY);
					}
				}
			}
		}
	}
	int FramX = m_TerianTex->GetTexturUvX(y + x * Info.GetTileMaxY());
	int FramY = m_TerianTex->GetTexturUvY(y + x * Info.GetTileMaxY());
	if (m_IsFever)
	{
		TeriansetMap(x, y, FramX, 1);
	}
	else
	{
		TeriansetMap(x, y, FramX, 0);
	}
}

void TileMap::SetMonsterProperty(int Xtile, int Ytile, TileObject ObjectType, TileProperty Property, string ImageKey)
{
	if ((Xtile >= 0 && Xtile < static_cast<int>(Info.GetTileMaxX())) && (Ytile >= 0 && Ytile < static_cast<int>(Info.GetTileMaxY())))
	{
		//if (ObjectType == OBJECT_EMPTY && m_Tiles[Xtile][Ytile].ImageKey3 == ImageKey)
		//{
		//	m_Tiles[Xtile][Ytile].Property = Property;
		//	m_Tiles[Xtile][Ytile].Object3 = ObjectType;
		//	m_Tiles[Xtile][Ytile].ImageKey3 = "";
		//}
		//else
		//{
			m_Tiles[Xtile][Ytile].Property = Property;
			m_Tiles[Xtile][Ytile].Object3 = ObjectType;
			m_Tiles[Xtile][Ytile].ImageKey3 = ImageKey;
		//}
	}
}

ST_TILE * TileMap::GetTile(UINT IndexX, UINT IndexY)
{
	ST_TILE * temp = nullptr;

	if ((IndexX > -1 || IndexX < Info.GetTileMaxX()) && (IndexY > -1 || IndexY < Info.GetTileMaxY()))
	{
		temp = &m_Tiles[IndexX][IndexY];
	}

	return temp;
}

void TileMap::Sort()
{

}

bool TileMap::ColiisionTile(D3DXVECTOR2 Pos, D3DXVECTOR2 Scale, ColisionDir Dir)
{
	return false;
}

void TileMap::AllViewMap()
{
	m_AllMapView = true;
	BegineTileTexUpdate();
	for (UINT i = 0; i < Info.GetTileMaxX(); i++)
	{
		for (UINT j = 0; j < Info.GetTileMaxY(); j++)
		{
			m_TerianTex->Color(j + i * Info.GetTileMaxY(), 0, 0, 0, 1);
			m_TerianTex->SetTextureUpdate(j + i * Info.GetTileMaxY());
		}
	}
	m_TerianTex->Update();
}
