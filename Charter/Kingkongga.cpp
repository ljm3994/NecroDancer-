#include "stdafx.h"
#include "Kingkongga.h"


Kingkongga::Kingkongga()
{
}


Kingkongga::~Kingkongga()
{
}

void Kingkongga::KingkonggaInit()
{
	m_CurrentMosterNum = CHARTERMANAGER->EnemySize();
	m_IsDrum = false;
	m_DrumCount = 0;
	m_IsAttack = false;
	m_PlayerSight = 6;
	Count = 0;
}

void Kingkongga::release()
{
}

void Kingkongga::update()
{
	Count++;
	ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(m_Pos.x), static_cast<UINT>(m_Pos.y));
	tile->m_IsPlayerFirstLight = true;
	tile->m_IsPlayerSight = true;
	tile->m_IsPlayerLight = true;
	m_CurrentMosterNum = CHARTERMANAGER->EnemySize() - CHARTERMANAGER->GetDieMonster();
	KingKogaRayUpdate();

	if (m_CurrentMosterNum <= 1 && !m_IsAttack)
	{
		m_IsAttack = true;
		m_CurrentFramX = 5;
		MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(Getm_Pos().x), static_cast<int>(Getm_Pos().y), 0, 0, MODE_DRAW, "BossChair", false, TileProperty::NONPASS, TileObject::OBJECT_ETC);
		MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(Getm_Pos().x), static_cast<int>(Getm_Pos().y - 1), TileObject::OBJECT_ENEMY, TileProperty::NONPASS, Key);
		SOUNDMANAGER->play("BossCry", GAMEMANAGER->GetsfxVolum());
	}

	if (!m_IsAttack)
	{
		if (m_DrumCount > 6)
		{
			m_CurrentFramX = 4;
		}
		else
		{
			if (m_CurrentFramX == 0 || m_CurrentFramX == 2)
			{
				if (Count % 10 == 0)
				{
					Count = 0;
					m_CurrentFramX++;
				}
			}
			else
			{
				if (UIMANAGER->Getm_BeatUI()->Getm_isBeat())
				{
					m_CurrentFramX++;
				}

				if (m_CurrentFramX > 3)m_CurrentFramX = 0;
			}
		}
		if (UIMANAGER->Getm_BeatUI()->Getm_isBeat())
		{
			if (m_DrumCount > 6)
			{
				m_DrumCount = 0;
			}
		}
	}
	else
	{
		if (UIMANAGER->Getm_BeatUI()->Getm_isBeat())
		{
			if (m_CurrentFramX != 5)
			{
				m_CurrentFramX = 5;
			}
			else
			{
				m_CurrentFramX = 6;
			}
		}
	}
	if (m_HpRender)
	{
		if (!m_IsAttack)
		{
			m_IsAttack = true;
			m_CurrentFramX = 5;
			MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(Getm_Pos().x), static_cast<int>(Getm_Pos().y), 0, 0, MODE_DRAW, "BossChair", false, TileProperty::NONPASS, TileObject::OBJECT_ETC);
			MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(Getm_Pos().x), static_cast<int>(Getm_Pos().y - 1), TileObject::OBJECT_ENEMY, TileProperty::NONPASS, Key);
			SOUNDMANAGER->play("BossCry", GAMEMANAGER->GetsfxVolum());
		}
		m_HpShowDelay += g_ETime;
		if (m_HpShowDelay > 2.0f)
		{
			m_HpShowDelay = 0.0f;
			m_HpRender = false;
		}
	}
}

void Kingkongga::render()
{
	if (m_Alive)
	{
		Image *img = IMGMANAGER->GetImage("Boss");
		img->Scale(100, 150);
		img->Position(m_ScreenPos.x, m_ScreenPos.y + 20 + m_Jump);
		img->Rotate(0, RatateY, 0);
		img->GetRefCurrentFrameX() = m_CurrentFramX;
		img->GetRefCurrentFrameY() = m_CurrentFramY;
		img->Render();
	}
}

void Kingkongga::HitVoice()
{
	SOUNDMANAGER->play("BossHit", GAMEMANAGER->GetsfxVolum());
}

void Kingkongga::AttackVoice()
{
	SOUNDMANAGER->play("BossAttack", GAMEMANAGER->GetsfxVolum());
}

void Kingkongga::DeathVoice()
{
	SOUNDMANAGER->play("BossDeath", GAMEMANAGER->GetsfxVolum());
}

void Kingkongga::AttackRender()
{
	if (m_IsAttackAnime)
	{
		Image * img = IMGMANAGER->GetImage("EnemyEffect");
		img->Scale(60, 60);
		img->Position(m_AttackPos.x * 50, (m_AttackPos.y * 50) + 20);
		img->Rotate(0, 0, AttackRotateZ);
		img->PlayAnimation(PlayState::ANI_LOOP, true, 0.2f);
		if (img->GetRefCurrentFrameX() == 4)
		{
			img->GetRefCurrentFrameX() = 0;
			m_IsAttackAnime = false;
		}
	}
}

void Kingkongga::PutCoin()
{
	
}

void Kingkongga::AstarPath(bool isBeat)
{
	if (m_Alive && m_IsAttack)
	{
		m_BeatTime = isBeat;
		if (m_BeatTime)
		{
			m_DrumCount++;
		}
		Player * player = CHARTERMANAGER->GetPlayer();
		D3DXVECTOR2 pos = player->Getm_Pos();

		if ((m_Pos.x - 1 == pos.x && m_Pos.y == pos.y) ||
			(m_Pos.x + 1 == pos.x && m_Pos.y == pos.y) ||
			(m_Pos.x == pos.x && m_Pos.y + 1 == pos.y)
			|| (m_Pos.x == pos.x &&m_Pos.y - 1 == pos.y))
		{
			AttackDelay += g_ETime;
			if (m_BeatTime && !player->Getm_isJumpMoving())
			{
				if (AttackDelay > 2.0f)
				{
					Attack();
					AttackDelay = 0.0f;
				}
			}
		}
		else
		{
			if (!m_isJumpMoving)
			{
				PathSucced = ASTARMANAGER->AstarFindPath(m_Path, MAPMANAGER->GetAllMap(), static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y), static_cast<int>(player->Getm_Pos().x), static_cast<int>(player->Getm_Pos().y), 10);
			}
		}
		PosUpdate();
	}
}

void Kingkongga::KingKogaRayUpdate()
{
	float lay = 0.0f;
	float max = static_cast<float>(m_PlayerSight) - 0.25f;

	float bright = 0.0f;
	float sightmax = 5.0f;
	float CurrentSight = 0.0f;

	float r = 0.0f;
	float angle = (15.0f * PI) / 180.0f;

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

		float X = m_Pos.x + CurrentSight * cos(r) + 0.5f;
		float y = m_Pos.y + CurrentSight * -sin(r) + 0.5f;

		if (X < 0 || y < 0)
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

		int val = MAPMANAGER->PlayerRayUpdate(static_cast<int>(X), static_cast<int>(y), c, m_PlayerSight);

		if (val == 1)
		{
			lay = 0.0f;
			CurrentSight = 0.0f;
			r += angle;
		}
		else if (val == 2)
		{
			lay += 0.25f;
			CurrentSight += 0.25f;
		}
	}
}

bool Kingkongga::Die()
{
	if (m_Life <= 0 && m_Alive)
	{
		m_Alive = false;
		DeathVoice();
		GAMEMANAGER->GetLevel()[7].m_IsUse = true;
		MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y), TileObject::OBJECT_EMPTY, TileProperty::PASS, Key);
		ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y));
		tile->Object = OBJECT_STAIRS;
		tile->Property = PASS;
		tile->ImageKey = "Stairs";

		switch (m_Dir)
		{
		case MOVE_LEFT:
			MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x + 1), static_cast<int>(m_Pos.y), TileObject::OBJECT_EMPTY, TileProperty::PASS, Key);
			break;
		case MOVE_RIGHT:
			MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x - 1), static_cast<int>(m_Pos.y), TileObject::OBJECT_EMPTY, TileProperty::PASS, Key);
			break;
		case MOVE_DOWN:
			MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y + 1), TileObject::OBJECT_EMPTY, TileProperty::PASS, Key);
			break;
		case MOVE_UP:
			MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y - 1), TileObject::OBJECT_EMPTY, TileProperty::PASS, Key);
			break;
		}
		return false;
	}
	return true;
}
