#include "stdafx.h"
#include "Jombi.h"


Jombi::Jombi()
{
}


Jombi::~Jombi()
{
}

void Jombi::JombeInit()
{
	m_Next = nullptr;
	m_Before = nullptr;
	m_state = ZOMBE_IDLE;
	m_IsPosUpdate = false;
	m_JomSight = 2;
}

void Jombi::release()
{
}

void Jombi::update()
{
	RayCastUpdate();
	
	if (!m_IsPosUpdate)
	{
		if (!m_Before)
		{
			m_state = JOMBISTATE::ZOMBE_HEAD;
			if (!m_Next)
			{
				m_state = JOMBISTATE::ZOMBE_ALONE;
			}
		}
		else
		{
			if (!m_isJumpMoving)
			{
				POINT JomPos;
				JomPos.x = static_cast<LONG>(m_Before->Getm_Pos().x);
				JomPos.y = static_cast<LONG>(m_Before->Getm_Pos().y);
				while (!m_Path.empty())
				{
					m_Path.pop();
				}
				m_Path.push(JomPos);
			}
		}

		if (m_state == JOMBISTATE::ZOMBE_HEAD || m_state == JOMBISTATE::ZOMBE_ALONE)
		{
			if (m_BeatTime)
			{
				HeadUpdate();
			}
		}
		m_IsPosUpdate = true;
	}
	else
	{
		if (m_state == JOMBISTATE::ZOMBE_HEAD || m_state == JOMBISTATE::ZOMBE_ALONE)
		{
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
					if (AttackDelay > 0.3f)
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
		}
		PosUpdate();
		m_IsPosUpdate = false;
	}
}

void Jombi::render()
{
	Image * img = IMGMANAGER->GetImage("Jombie");
	img->Scale(50, 50);
	img->Position(m_ScreenPos.x, (m_ScreenPos.y) + 20 + m_Jump);
	img->Color(0, 0, 0, m_Alpha);
	img->Rotate(0, RatateY, 0);
	switch (m_state)
	{
	case ZOMBE_IDLE:
		img->GetRefCurrentFrameX() = 71 - m_CurrentFramY;
		break;
	case ZOMBE_HEAD:
		img->GetRefCurrentFrameX() = 71 - (m_CurrentFramY + 24);
		break;
	case ZOMBE_ALONE:
		img->GetRefCurrentFrameX() = 71 - (m_CurrentFramY + 48);
		break;
	}
	img->GetRefCurrentFrameY() = m_CurrentFramY;
	img->Render();
}

void Jombi::HitVoice()
{
}

void Jombi::AttackVoice()
{
	SOUNDMANAGER->play("JombieAttack", GAMEMANAGER->GetsfxVolum());
}

void Jombi::DeathVoice()
{
	SOUNDMANAGER->play("JombieDeath", GAMEMANAGER->GetsfxVolum());
}

void Jombi::AttackRender()
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

void Jombi::ZomUpdate()
{
	m_IsPosUpdate = true;
	update();
	m_IsPosUpdate = false;
}

void Jombi::HeadUpdate()
{
	Jombi * current = this;

	while (current)
	{
		current->ZomUpdate();
		current = current->m_Next;
	}
}

void Jombi::RayCastUpdate()
{
	float lay = 0.0f;
	float max = static_cast<float>(m_JomSight) - 0.25f;

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

		int val = MAPMANAGER->PlayerRayUpdate(static_cast<int>(X), static_cast<int>(y), c, m_JomSight);

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

void Jombi::AstarPath(bool isBeat)
{
	m_BeatTime = isBeat;
}

void Jombi::PosUpdate()
{
	if (!m_isJumpMoving)
	{
		if (!m_Path.empty())
		{
			if (m_BeatTime && m_Dir == MOVE_NO)
			{
				float Xpos = static_cast<float>(m_Path.top().x);
				float Ypos = static_cast<float>(m_Path.top().y);
				m_Path.pop();
				if (MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(Xpos), static_cast<UINT>(Ypos))->Property == TileProperty::PASS)
				{
					D3DXVECTOR2 Dir;
					Dir.x = Xpos - m_Pos.x;
					Dir.y = Ypos - m_Pos.y;
					MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y), TileObject::OBJECT_EMPTY, TileProperty::PASS, Key);
					if (Dir.x == -1)
					{
						m_Pos.x -= 1;
						m_Cnt = 50;
						m_Jump = 0;
						m_CurrentFramX = 8;
						m_Dir = MOVE_LEFT;
						RatateY = 0;
						m_isJumpMoving = true;
					}
					else if (Dir.x == 1)
					{
						m_Pos.x += 1;
						m_Cnt = 50;
						m_Jump = 0;
						m_CurrentFramX = 8;
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
						m_CurrentFramX = 16;
						m_Dir = MOVE_DOWN;
						m_isJumpMoving = true;
					}
					MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y), TileObject::OBJECT_ENEMY, TileProperty::NONPASS, Key);
					m_BeatTime = false;
				}
			}
		}
	}

	if (m_Dir != MOVE_NO)
	{
		Move(m_Dir);
	}
}

void Jombi::Move(MOVEDIR dir)
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
		m_ScreenPos.y += (300 * g_ETime);
		break;
	case MOVE_UP:
		m_Cnt -= (300 * g_ETime);
		m_ScreenPos.y -= (300 * g_ETime);
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

	if (m_Cnt < 0)
	{
		m_ScreenPos.x = (m_Pos.x * 50);
		m_ScreenPos.y = (m_Pos.y * 50);
		m_Jump = 0;
		m_Cnt = 0;
		m_isJumpMoving = false;
		m_Dir = MOVE_NO;
	}
}

void Jombi::PutCoin()
{
}

bool Jombi::Die()
{
	if (m_Life <= 0)
	{
		m_Alive = false;
		DeathVoice();
		MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y), TileObject::OBJECT_EMPTY, TileProperty::PASS, Key);
		if (m_Before)
		{
			m_Before->Setm_Next(nullptr);
		}
		if (m_Next)
		{
			m_Next->Setm_Before(nullptr);
		}
		PutCoin();
		return false;
	}
	return true;
}
