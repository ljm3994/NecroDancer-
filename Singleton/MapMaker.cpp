#include "stdafx.h"
#include "MapMaker.h"
// ³ëµå¸¦ ÂÉ°µ´Ù(ÂÉ°¶ ³ëµå, ·£´ý Seed)
void MapMaker::Division(queue<BspTreeNode*>& Node, RandomGet& Random)
{
	auto top = Node.front();
	Node.pop();

	top->Division(m_DivRange, Random);
	// ¿ÞÂÊ ÀÚ½ÄÀÌ Á¸ÀçÇÏ¸é
	if (top->GetLeftChile() != nullptr)
	{
		Node.push(top->GetLeftChile());
	}
	// ¿À¸¥ÂÊ ÀÚ½ÄÀÌ Á¸ÀçÇÏ¸é
	if (top->GetRightChile() != nullptr)
	{
		Node.push(top->GetRightChile());
	}
}

void MapMaker::RoomMaker()
{
	random_device rd;
	RandomGet Random(rd());

	m_Root.RoomMake(m_MidSize, m_SizeRange, Random);
}

void MapMaker::Coalescence()
{
	random_device rd;
	RandomGet Random(rd());

	m_Root.Coalescence(m_Com, Random);
}

void MapMaker::FillData()
{
	m_Root.FillData(m_width, m_height, m_Area, Rooms);
}

queue<BspTreeNode*> MapMaker::GetRootNode()
{

	queue<BspTreeNode*> Node;

	Node.push(&m_Root);

	return Node;
}

void MapMaker::SetWidth(int Width)
{
	m_width = Width;
	m_Root.Clear(0, 0, m_width, m_height);

	m_Area.resize(m_width * m_height, AREA_NOBROKENNWALL);
}

void MapMaker::SetHeight(int Height)
{
	m_height = Height;
	m_Root.Clear(0, 0,m_width, m_height);
	m_Area.resize(m_width * m_height, AREA_NOBROKENNWALL);
}

void MapMaker::MapCreatInit(int Width, int Height, int DivNum, float DivRange, float MidSize, float SizeRange, int Com)
{
	m_height = Height;
	m_width = Width;
	m_DivNum = DivNum;
	m_DivRange = DivRange;
	m_MidSize = MidSize;
	m_SizeRange = SizeRange;
	m_Com = Com;
	Rooms.clear();
	m_Root.Clear(0, 0, m_width, m_height);
	m_Area.resize(m_width * m_height, AREA_NOBROKENNWALL);
}

void MapMaker::CreateMap()
{
	{
		if (m_isCreated)
		{
			Rooms.clear();
			m_Root.Clear(0, 0, m_width, m_height);
		}

		m_isCreated = true;
	}
}

int MapMaker::ObjSize()
{
	return objdata.size();
}

MapMaker::MapMaker()
	:m_width(100), m_height(100), m_DivNum(6), m_Com(1), m_DivRange(0.2f), m_MidSize(0.6f), m_SizeRange(0.2f), m_isCreated(false)
{
	m_Root = BspTreeNode(0, 0, m_width, m_height);
	m_Area.resize(m_width * m_height, AREA_NOBROKENNWALL);
}

MapMaker::MapMaker(int Width, int Height, int DivNum, float DivRange, float MidSize, float SizeRange, int Com)
	:m_width(Width), m_height(Height), m_DivNum(DivNum), m_Com(Com), m_DivRange(DivRange), m_MidSize(MidSize), m_SizeRange(SizeRange), m_isCreated(false)
{
	m_Root = BspTreeNode(0, 0,m_width, m_height);
	m_Area.resize(m_width * m_height, AREA_NOBROKENNWALL);
}

MapMaker::~MapMaker()
{
}
