#include "stdafx.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase()

{
}

EnemyBase::~EnemyBase()
{
}

HRESULT EnemyBase::init()
{
	m_BeatTime = false;
	PathSucced = false;
	m_isJumpMoving = false;
	m_DelayTime = 0.0f;
	m_Jump = 0;
	RatateY = 0;
	m_Alive = true;
	m_Dir = MOVE_NO;
	m_CurrentFramX = 0;
	m_CurrentFramY = 0;
	m_HpShowDelay = 0.0f;
	m_Alpha = 0.0f;
	m_IsAttackAnime = false;
	AttackDelay = 0.0f;
	return S_OK;
}

void EnemyBase::SettingEnemy(UINT maxHp, UINT currentHp, int Damage, int m_Deffence)
{
	m_Range = 5;
	m_MaxLife = maxHp;
	m_Life = currentHp;
	m_Damage = Damage;
	this->m_Deffence = m_Deffence;
}

void EnemyBase::SetPos(D3DXVECTOR2 Pos)
{
	m_Pos = Pos;
	m_ScreenPos = D3DXVECTOR2(m_Pos.x * 50, m_Pos.y * 50);
	CurrentTile = MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y));
}

void EnemyBase::AstarPath(bool isBeat)
{
	if (m_Alive)
	{
		CurrentTile->Object3 = OBJECT_ENEMY;
		CurrentTile->ImageKey3 = Key;
		CurrentTile->Property = NONPASS;
		m_BeatTime = isBeat;
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
				if (AttackDelay >0.2f)
				{
					Attack();
					AttackDelay = 0.0f;
				}
			}
		}
		else
		{
			if ((static_cast<int>(pos.x) >= m_MyRoom->m_X && static_cast<int>(pos.x) < m_MyRoom->getRight() + 1) &&
				(static_cast<int>(pos.y) >= m_MyRoom->m_Y && static_cast<int>(pos.y) < m_MyRoom->getBottom() + 1))
			{
				if (!m_isJumpMoving)
				{
					PathSucced = ASTARMANAGER->AstarFindPath(m_Path, MAPMANAGER->GetAllMap(), static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y), static_cast<int>(player->Getm_Pos().x), static_cast<int>(player->Getm_Pos().y), 10);
				}
			}
			else
			{
				m_Dir = MOVE_NO;
				PathSucced = false;
			}
		}
		PosUpdate();
	}
}

void EnemyBase::PosUpdate()
{
	if (!m_isJumpMoving)
	{
		if (!m_Path.empty())
		{
			if (m_BeatTime)
			{
				float Xpos = static_cast<float>(m_Path.top().x);
				float Ypos = static_cast<float>(m_Path.top().y);
				m_Path.pop();
				if (MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(Xpos), static_cast<int>(Ypos))->Property == PASS)
				{
					D3DXVECTOR2 Dir;
					Dir.x = Xpos - m_Pos.x;
					Dir.y = Ypos - m_Pos.y;
					if (Dir.x == -1)
					{
						m_Pos.x -= 1;
						m_Cnt = 50;
						m_Jump = 0;
						m_Dir = MOVE_LEFT;
						RatateY = 0;
						m_isJumpMoving = true;
					}
					else if (Dir.x == 1)
					{
						m_Pos.x += 1;
						m_Cnt = 50;
						m_Jump = 0;
						RatateY = PI;
						m_Dir = MOVE_RIGHT;
						m_isJumpMoving = true;
					}
					else if (Dir.y == 1)
					{
						m_Pos.y += 1;
						m_Cnt = 50;
						m_Jump = 1;
						m_Dir = MOVE_UP;
						m_isJumpMoving = true;
					}
					else if (Dir.y == -1)
					{
						m_Pos.y -= 1;
						m_Cnt = 50;
						m_Jump = 0;
						m_Dir = MOVE_DOWN;
						m_isJumpMoving = true;
					}
					CurrentTile = MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y));
					CurrentTile->Object3 = OBJECT_ENEMY;
					CurrentTile->ImageKey3 = Key;
					CurrentTile->Property = NONPASS;
				}
				m_BeatTime = false;
			}
		}
	}

	if (m_Dir != MOVE_NO)
	{
		Move(m_Dir);
	}
}

void EnemyBase::Move(MOVEDIR dir)
{
	switch (dir)
	{
	case MOVE_LEFT:
		m_Cnt -= (300 * g_ETime);
		m_ScreenPos.x -= (300 * g_ETime);
		break;
	case MOVE_RIGHT:
		m_Cnt -= (300 * g_ETime);
		m_ScreenPos.x += (300 * g_ETime);
		break;
	case MOVE_DOWN:
		m_Cnt -= (300 * g_ETime);
		m_ScreenPos.y -= (300 * g_ETime);
		break;
	case MOVE_UP:
		m_Cnt -= (300 * g_ETime);
		m_ScreenPos.y += (300 * g_ETime);
		break;
	}


	if (m_Cnt)
	{
		if (m_Cnt > (50 / 2))
		{
			m_Jump += (180 * g_ETime);
		}
		else
		{
			m_Jump -= (180 * g_ETime);
		}
	}

	if (m_Cnt <= 0)
	{
		switch (dir)
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
		TileCheck();
		m_ScreenPos.x = (m_Pos.x * 50);
		m_ScreenPos.y = (m_Pos.y * 50);
		m_Jump = 0;
		m_Cnt = 0;
		m_isJumpMoving = false;
		m_Dir = MOVE_NO;
	}
}

void EnemyBase::Attack()
{
	if (!m_IsAttackAnime)
	{
		m_IsAttackAnime = true;
		AttackVoice();
		Player * pl = CHARTERMANAGER->GetPlayer();
		pl->Setm_IsEarthquake(true);
		pl->Damage(m_Damage);
		D3DXVECTOR2 pos = pl->Getm_Pos();
		if ((m_Pos.x - 1 == pos.x && m_Pos.y == pos.y))
		{
			AttackRotateZ = PI;
			m_AttackPos = D3DXVECTOR2(pos.x, pos.y);
		}
		else if ((m_Pos.x + 1 == pos.x && m_Pos.y == pos.y))
		{
			AttackRotateZ = 0;
			m_AttackPos = D3DXVECTOR2(pos.x, pos.y);
		}
		else if ((m_Pos.x == pos.x && m_Pos.y + 1 == pos.y))
		{
			AttackRotateZ = PI / 2;
			m_AttackPos = D3DXVECTOR2(pos.x, pos.y);
		}
		else if ((m_Pos.x == pos.x && m_Pos.y - 1 == pos.y))
		{
			AttackRotateZ = 2 * PI;
			m_AttackPos = D3DXVECTOR2(pos.x, pos.y);
		}
	}
}

void EnemyBase::HpRender()
{
	if (m_HpRender && m_MaxLife != m_Life)
	{
		for (int i = 0; i < m_MaxLife; i++)
		{
			Image * img = nullptr;
			if (i >= m_Life)
			{
				img = IMGMANAGER->GetImage("EnemyHeartEmpty");
			}
			else
			{
				img = IMGMANAGER->GetImage("EnemyHeart");
			}
			img->Scale(15, 15);
			img->Position((m_ScreenPos.x + 26) - (m_MaxLife / 2) * 24 + 24 * i, m_ScreenPos.y + 50);
			img->Render();
		}
	}
}

void EnemyBase::Damage(int Attack)
{
	Deffence(Attack);
	m_Life -= Attack;

	if (m_Life <= 0)
	{
		MAPMANAGER->SetFever(true);
	}
}

void EnemyBase::Deffence(int & Attack)
{
	Attack -= m_Deffence;
	if (Attack <= 0)
	{
		Attack = 1;
	}
}

void EnemyBase::TileCheck()
{
}

bool EnemyBase::Die()
{
	if (m_Life <= 0 && m_Alive)
	{
		m_Alive = false;
		DeathVoice();
		MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y), TileObject::OBJECT_EMPTY, TileProperty::PASS, "");
		PutCoin();

		switch (m_Dir)
		{
		case MOVE_LEFT:
			MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x + 1), static_cast<int>(m_Pos.y), TileObject::OBJECT_EMPTY, TileProperty::PASS, "");
			break;
		case MOVE_RIGHT:
			MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x - 1), static_cast<int>(m_Pos.y), TileObject::OBJECT_EMPTY, TileProperty::PASS, "");
			break;
		case MOVE_DOWN:
			MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y + 1), TileObject::OBJECT_EMPTY, TileProperty::PASS, "");
			break;
		case MOVE_UP:
			MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y - 1), TileObject::OBJECT_EMPTY, TileProperty::PASS, "");
			break;
		}
		return false;
	}
	return true;
}
