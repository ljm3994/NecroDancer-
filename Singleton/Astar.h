#pragma once
#include <queue>
#include <stack>

typedef struct AstarNode
{
public:
	int TileIndexX;
	int TileIndexY;
	D3DXVECTOR2 m_scale;
	float m_H;
	float m_Dist;            // 현재 위치에서 도착지까지의 거리
	float m_Dept;          // 출발점에서 현재위치까지의 거리
	AstarNode * m_ParentNode;               //부모 노드
} *PAstarNode;

struct Cmp
{
	bool operator()(AstarNode *a, AstarNode *b)
	{
		return (a->m_H) < (b->m_H);
	}
};

class Astar : public SingletonBase<Astar>
{
public:
	Astar();
	~Astar();
	bool AstarFindPath(stack<POINT> &PathNode, TileMap *Map, int StartIndexX, int StartIndexY, int EndIndexX, int EndIndexY, int Range);
private:
	float SetFPos(int StartIndexX, int StartIndexY, int EndIndexX, int EndIndexY);
	float SetG(float ParentGcost, int StartIndexX, int StartIndexY, int EndIndexX, int EndIndexY);
	bool isConner(TileMap * Map, int idx, int idy, int ParentIdx, int ParentIdy);
	void FreeNode(vector<PAstarNode> &OpenNode);
};

