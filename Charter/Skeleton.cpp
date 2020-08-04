#include "stdafx.h"
#include "Skeleton.h"


Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
}

void Skeleton::release()
{
}

void Skeleton::update()
{
	if (m_HpRender)
	{
		m_HpShowDelay += g_ETime;
		if (m_HpShowDelay > 2.0f)
		{
			m_HpShowDelay = 0.0f;
			m_HpRender = false;
		}
	}
}

void Skeleton::render()
{
	m_DelayTime += g_ETime;

	if (m_DelayTime > 0.2f)
	{
		m_DelayTime = 0.0f;
		m_CurrentFramX++;

		if (m_CurrentFramX > 3)
		{
			m_CurrentFramX = 0;
		}
	}
	Image * img = IMGMANAGER->GetImage("Skeleton");
    img->Scale(50, 50);
	img->Position(m_ScreenPos.x, m_ScreenPos.y + 20 + m_Jump);
	img->Color(0, 0, 0, m_Alpha);
	img->Rotate(0, RatateY, 0);
	img->GetRefCurrentFrameX() = m_CurrentFramX;
	img->GetRefCurrentFrameY() = m_CurrentFramY;
	img->Render();
}

void Skeleton::HitVoice()
{
	SOUNDMANAGER->play("SkeletonHit", GAMEMANAGER->GetsfxVolum());
}

void Skeleton::AttackVoice()
{
	SOUNDMANAGER->play("SkeletonAttack", GAMEMANAGER->GetsfxVolum());
}

void Skeleton::DeathVoice()
{
	SOUNDMANAGER->play("SkeletonDeath", GAMEMANAGER->GetsfxVolum());
}

void Skeleton::AttackRender()
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

void Skeleton::PutCoin()
{
	if (MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y))->Object2 == OBJECT_EMPTY)
	{
		char coinname[256];
		sprintf_s(coinname, "Coin%d", GetFromIntTo(1, 3));
		MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y), 0, 0, MODE_DRAW, coinname, true, PASS, OBJECT_ITEM);
	}
}
