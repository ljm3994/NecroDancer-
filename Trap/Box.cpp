#include "stdafx.h"
#include "Box.h"


Box::Box()
{
}


Box::~Box()
{
}

void Box::Update()
{
}

void Box::Render(float Alpha)
{
	Image * img = IMGMANAGER->GetImage("ObjectBox");
	img->Scale(50, 50);
	img->Position(m_pos.x * 50, m_pos.y * 50);
	img->Color(0, 0, 0, Alpha);
	img->Render();
}

void Box::TrapDown(Charter * m_chr)
{
	SOUNDMANAGER->play("CheastOpen", GAMEMANAGER->GetsfxVolum());
	double Per[4] = { 30, 30, 30, 10 };
	int Num = GetRandom(Per, 4);
	MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 0, 0, MODE_DRAW, "", false, PASS, OBJECT_EMPTY);
	switch (Num)
	{
	case 1:
		MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 0, 0, MODE_DRAW, "Dagger", true, PASS, OBJECT_ITEM);
		break;
	case 2:
		MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 0, 0, MODE_DRAW, "BorodSword", true, PASS, OBJECT_ITEM);
		break;
	case 3:
		MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 0, 0, MODE_DRAW, "LeatherArmor", true, PASS, OBJECT_ITEM);
		break;
	case 4:
		MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 0, 0, MODE_DRAW, "Chainmail", true, PASS, OBJECT_ITEM);
		break;
	}
}
