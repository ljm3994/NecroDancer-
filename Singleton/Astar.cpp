#include "stdafx.h"
#include "Astar.h"

Astar::Astar()
{

}

Astar::~Astar()
{
}
// ��� ã�� �˰��� ��θ� ã���� True, ��ã���� False�� ��ȯ
// (��θ� �Է¹��� POINT��ü�� ��� �ִ� STACK, ���� �� ������ ��� ��ü, ������ġ X, ���� ��ġ Y, ������ X, ������ Y, Ž�� ����)
bool Astar::AstarFindPath(stack<POINT> &PathNode, TileMap *Map, int StartIndexX, int StartIndexY, int EndIndexX, int EndIndexY, int Range)
{
	while (!PathNode.empty())
	{
		PathNode.pop();
	}

	vector<AstarNode *> m_OpenNode; // ������ �� �ִ� Ÿ��
	vector<AstarNode *>::iterator m_OpenNodeit;
	vector <AstarNode *> m_CloseNode; // ���� ���(�̹� �˻簡 �Ϸ�� Ÿ��)
	vector<AstarNode *>::iterator m_Closeit;
	int Count = 1;    // �˻� Ƚ�� Ȯ�ο� ī��Ʈ ����
	int MaxTileX = min(static_cast<int>(Map->GetInfo().GetTileMaxX()), StartIndexX + Range);
	int MaxTileY = min(static_cast<int>(Map->GetInfo().GetTileMaxY()), StartIndexY + Range);
	AstarNode * Node = new AstarNode();           // Start��带 �����Ѵ�.
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
			// �ݺ��� Ż�� ������ ���� ��忡 ��� ���� �Էµ��� �ʰ� ��� Ÿ���� ���� �˻����� ��쿡�� Ż���Ѵ�.
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
				
				// �������� ���� ��忡 �߰��Ǿ� �ִٸ� �� ã�����Ƿ� ����������.
				if (ParentNode->TileIndexX == EndIndexX && ParentNode->TileIndexY == EndIndexY)
				{
					break;
				}

				// 3. �ݺ����� ���鼭 ���� ��ġ���� 8������ Ÿ���� ���� �˻��Ѵ�.
				for (int i = ParentXPos - 1; i <= ParentXPos + 1; i++)
				{
					for (int j = ParentYPos - 1; j <= ParentYPos + 1; j++)
					{
						// �� ������ ��������� ���� ����ó��
						if (i != -1 && j != -1 && i != MaxTileX && j != MaxTileY)
						{
							// ��Ÿ������ �˻��Ѵ�.
							if (Map->GetTile(i, j)->Property != TileProperty::NONPASS)
							{
								// ���� ��忡 ���� ���� ����Ǿ� �ִ��� �˻��Ѵ�.(���� ��忡 ���� ���� ���� ��쿡�� ����)
								m_Closeit = find_if(m_CloseNode.begin(), m_CloseNode.end(),
									[i, j](AstarNode * Node) -> bool { return((Node->TileIndexX == i) && (Node->TileIndexY == j)); });
								if (m_Closeit == m_CloseNode.end())
								{
									//�ڳʸ� ���� �ִ��� ���θ� �˻��Ѵ�.
									if (isConner(Map, i, j, ParentXPos, ParentYPos))
									{
										// ���� ��忡 ���� ���� ����Ǿ��ִ��� ���� Ȯ��
										m_OpenNodeit = find_if(m_OpenNode.begin(), m_OpenNode.end(),
											[i, j](AstarNode * Node) -> bool { return((Node->TileIndexX == i) && (Node->TileIndexY == j)); });
										if (m_OpenNodeit == m_OpenNode.end())     // �̹� ���� ��忡 �߰��Ǿ��ִ� ������� �˻��Ѵ�.
										{
											AstarNode *Node = new AstarNode();
											Node->TileIndexX = i;
											Node->TileIndexY = j;
											Node->m_Dist = SetFPos(i, j, EndIndexX, EndIndexY);
											Node->m_Dept = SetG(ParentGcost, i, j, ParentXPos, ParentYPos);
											Node->m_H = Node->m_Dist + Node->m_Dept;
											Node->m_ParentNode = ParentNode;
											m_OpenNode.emplace_back(Node);    //���� ��忡 �ε��� ��ȣ�� �����Ѵ�.
											Count++;
										}
										// ������ Ÿ���� �̹� ���� ��Ͽ� �ִٸ� �߰� �˻縦 �����Ѵ�.
										// ���� �ش� Ÿ���� ��� ����� �� �����ϴٸ� �θ��� ���� �����Ѵ�.
										else                      //���� ��忡 �߰��Ǿ� �ִٸ�
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
			// ���� Count�� 0 �̶�� ��θ� ã�� ���� ���̹Ƿ� false�� ��ȯ�Ѵ�.
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

	// ���� ��θ� ��ȯ�Ѵ�.
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
	// �Ÿ����ҽ� ����ư �Ÿ� �������� ����Ѵ�(��Ŭ����� �Ÿ� �������� ����� ���� ������ ���������� �ϴ� ����ư �Ÿ� ���������� ���)
	// �ش� �Ÿ��� ��ֹ��� ������ �Ÿ� ���� ����̴�
	// (x1 - x2) + (y2- y1)
	// ���� ��ġ ���� ���� ������ ��� ���
	return static_cast<float>((abs(StartIndexX - EndIndexX) + abs(EndIndexY - StartIndexY)) * 10);
}

float Astar::SetG(float ParentGcost, int StartIndexX, int StartIndexY, int EndIndexX, int EndIndexY)
{
	return ParentGcost + ((abs(StartIndexX - EndIndexX) == 1 && abs(StartIndexY - EndIndexY) == 1) ? 14 : 10);
}


// �ڳ� ���� �ִ��� ���� �˻�
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


