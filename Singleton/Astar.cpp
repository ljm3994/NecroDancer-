#include "stdafx.h"
#include "Astar.h"

Astar::Astar()
{

}

Astar::~Astar()
{
}
// 경로 찾기 알고리즘 경로를 찾으면 True, 못찾으면 False를 반환
// (경로를 입력받을 POINT객체를 담고 있는 STACK, 현재 맵 정보가 담긴 객체, 시작위치 X, 시작 위치 Y, 목적지 X, 목적지 Y, 탐색 범위)
bool Astar::AstarFindPath(stack<POINT> &PathNode, TileMap *Map, int StartIndexX, int StartIndexY, int EndIndexX, int EndIndexY, int Range)
{
	while (!PathNode.empty())
	{
		PathNode.pop();
	}

	vector<AstarNode *> m_OpenNode; // 움직일 수 있는 타일
	vector<AstarNode *>::iterator m_OpenNodeit;
	vector <AstarNode *> m_CloseNode; // 닫힌 노드(이미 검사가 완료된 타일)
	vector<AstarNode *>::iterator m_Closeit;
	int Count = 1;    // 검사 횟수 확인용 카운트 변수
	int MaxTileX = min(static_cast<int>(Map->GetInfo().GetTileMaxX()), StartIndexX + Range);
	int MaxTileY = min(static_cast<int>(Map->GetInfo().GetTileMaxY()), StartIndexY + Range);
	AstarNode * Node = new AstarNode();           // Start노드를 생성한다.
	Node->TileIndexX = StartIndexX;
	Node->TileIndexY = StartIndexY;
	Node->m_Dept = 0;
	Node->m_Dist = SetFPos(StartIndexX, StartIndexY, EndIndexX, EndIndexY);
	Node->m_H = Node->m_Dept + Node->m_Dist;
	m_OpenNode.emplace_back(Node);
	
	while (true)
	{
		if (Count <= Range)
		{
			// 반복문 탈출 조건은 열린 노드에 어떠한 값도 입력되지 않고 모든 타일을 전부 검사했을 경우에는 탈출한다.
			if (!m_OpenNode.empty())
			{
				int ParentXPos;
				int ParentYPos;
				float ParentGcost;
				sort(m_OpenNode.begin(), m_OpenNode.end(), Cmp());
				AstarNode * ParentNode = m_OpenNode[0];
				m_OpenNode.erase(m_OpenNode.begin());
				ParentXPos = ParentNode->TileIndexX;
				ParentYPos = ParentNode->TileIndexY;
				m_CloseNode.emplace_back(ParentNode);
				ParentGcost = ParentNode->m_Dept;
				
				// 목적지가 열린 노드에 추가되어 있다면 다 찾았으므로 빠져나간다.
				if (ParentNode->TileIndexX == EndIndexX && ParentNode->TileIndexY == EndIndexY)
				{
					break;
				}

				// 3. 반복문을 돌면서 현재 위치에서 8방향의 타일을 전부 검사한다.
				for (int i = ParentXPos - 1; i <= ParentXPos + 1; i++)
				{
					for (int j = ParentYPos - 1; j <= ParentYPos + 1; j++)
					{
						// 맵 범위를 벗어났는지에 대한 예외처리
						if (i != -1 && j != -1 && i != MaxTileX && j != MaxTileY)
						{
							// 벽타일인지 검사한다.
							if (Map->GetTile(i, j)->Property != TileProperty::NONPASS)
							{
								// 닫힌 노드에 현재 값이 저장되어 있는지 검사한다.(닫힌 노드에 현재 값이 없을 경우에만 진행)
								m_Closeit = find_if(m_CloseNode.begin(), m_CloseNode.end(),
									[i, j](AstarNode * Node) -> bool { return((Node->TileIndexX == i) && (Node->TileIndexY == j)); });
								if (m_Closeit == m_CloseNode.end())
								{
									//코너를 돌수 있는지 여부를 검사한다.
									if (isConner(Map, i, j, ParentXPos, ParentYPos))
									{
										// 열린 노드에 현재 값이 저장되어있는지 여부 확인
										m_OpenNodeit = find_if(m_OpenNode.begin(), m_OpenNode.end(),
											[i, j](AstarNode * Node) -> bool { return((Node->TileIndexX == i) && (Node->TileIndexY == j)); });
										if (m_OpenNodeit == m_OpenNode.end())     // 이미 열린 노드에 추가되어있는 노드인지 검사한다.
										{
											AstarNode *Node = new AstarNode();
											Node->TileIndexX = i;
											Node->TileIndexY = j;
											Node->m_Dist = SetFPos(i, j, EndIndexX, EndIndexY);
											Node->m_Dept = SetG(ParentGcost, i, j, ParentXPos, ParentYPos);
											Node->m_H = Node->m_Dist + Node->m_Dept;
											Node->m_ParentNode = ParentNode;
											m_OpenNode.emplace_back(Node);    //열린 노드에 인덱스 번호를 저장한다.
											Count++;
										}
										// 인접한 타일이 이미 열린 목록에 있다면 추가 검사를 시행한다.
										// 만약 해당 타일의 경로 비용이 더 저렴하다면 부모의 값과 변경한다.
										else                      //열린 노드에 추가되어 있다면
										{
											AstarNode *Node = new AstarNode();
											Node->TileIndexX = i;
											Node->TileIndexY = j;
											Node->m_Dist = SetFPos(i, j, EndIndexX, EndIndexY);
											Node->m_Dept = SetG(ParentGcost, i, j, ParentXPos, ParentYPos);
											Node->m_H = Node->m_Dist + Node->m_Dept;
											Node->m_ParentNode = ParentNode;

											if ((*m_OpenNodeit)->m_Dept > Node->m_Dept)
											{
												(*m_OpenNodeit)->m_Dept = Node->m_Dept;
												(*m_OpenNodeit)->m_H = (*m_OpenNodeit)->m_Dept + (*m_OpenNodeit)->m_Dist;
												(*m_OpenNodeit)->m_ParentNode = Node->m_ParentNode;
											}
										}
									}
								}
							}
						}
					}
				}
			}
			// 만약 Count가 0 이라면 경로를 찾지 못한 것이므로 false를 반환한다.
			else
			{
				FreeNode(m_OpenNode);
				FreeNode(m_CloseNode);
				return false;
			}
		}
		else
		{
			break;
		}
	}

	// 실제 경로를 반환한다.
	AstarNode * Temp = m_CloseNode[m_CloseNode.size() - 1];

	while(true)
	{
		if (Temp->m_ParentNode)
		{
			Temp = Temp->m_ParentNode;
			if (Temp->TileIndexX == StartIndexX && Temp->TileIndexY == StartIndexY)
			{
				break;
			}
			PathNode.push({ Temp->TileIndexX, Temp->TileIndexY });
		}
		else
		{
			break;
		}
	}

	FreeNode(m_OpenNode);
	FreeNode(m_CloseNode);
	return true;
}


float Astar::SetFPos(int StartIndexX, int StartIndexY, int EndIndexX, int EndIndexY)
{
	// 거리구할시 맨해튼 거리 측정법을 사용한다(유클리디안 거리 측정법도 고려해 보면 좋을거 같긴하지만 일단 맨해튼 거리 측정법으로 계산)
	// 해당 거리는 장애물을 무시한 거리 측정 비용이다
	// (x1 - x2) + (y2- y1)
	// 현재 위치 부터 끝점 까지의 경로 비용
	return static_cast<float>((abs(StartIndexX - EndIndexX) + abs(EndIndexY - StartIndexY)) * 10);
}

float Astar::SetG(float ParentGcost, int StartIndexX, int StartIndexY, int EndIndexX, int EndIndexY)
{
	return ParentGcost + ((abs(StartIndexX - EndIndexX) == 1 && abs(StartIndexY - EndIndexY) == 1) ? 14 : 10);
}


// 코너 돌수 있는지 여부 검사
bool Astar::isConner(TileMap * Map, int idx, int idy, int ParentIdx, int ParentIdy)
{
	if (idx == ParentIdx - 1)
	{
		if (idy == ParentIdy - 1)
		{
			return false;
		}
		else if (idy == ParentIdy + 1)
		{
			return false;
		}
	}
	else if (idx == ParentIdx + 1)
	{
		if (idy == ParentIdy - 1)
		{
			return false;
		}
		else if (idy == ParentIdy + 1)
		{
			return false;
		}
	}

	return true;
}

void Astar::FreeNode(vector<PAstarNode>& OpenNode)
{
	for (UINT i = 0; i < OpenNode.size(); i++)
	{
		SAFE_DELETE(OpenNode[i]);
	}

	OpenNode.shrink_to_fit();
	OpenNode.clear();
}


