// Fill out your copyright notice in the Description page of Project Settings.


#include "AStar.h"
#include <queue>

struct Node {
public:
	Node(int _x, int _y) : x(_x), y(_y)
	{
		G = 0;
		H = 0;
		F = 0;
	};

	int x, y; // 위치
	int G, H, F; // G : 가중치, H : 휴리스틱 추정치, F : G + H
	int parentIndex; // vector<int> close list의  index값을 저장

    bool operator<(const Node& other) const
    {
        return F > other.F; // F 값이 작은 것이 우선순위가 높음
    }
};

int cx[4] = { 0, 1, 0, -1 };
int cy[4] = { -1, 0, 1, 0 };

AStar::AStar()
{
}

AStar::~AStar()
{
}

TArray<FVector2D> AStar::FindPath(TArray<TArray<int>> Map, FVector2D Start, FVector2D End)
{
    TArray<FVector2D> Path;
	auto CompareFunc = [](const Node& a, const Node& b) {
		return a.F < b.F;
		};

    TArray<Node> OpenList;
	TArray<Node> CloseList;

    // 시작점 노드 생성 후 Open List에 추가
	Node StartNode(Start.X, Start.Y);
    StartNode.parentIndex = -1;
    OpenList.HeapPush(StartNode, CompareFunc);

    while (!OpenList.IsEmpty())
	{ 
		// 우선 순위 큐에서 노드를 가져옴 (F값 이 가장 작은 노드)
		Node CurrentNode = OpenList[0];
        OpenList.RemoveAt(0);

		// Close List에 꺼내온 노드를 추가
        CloseList.Push(CurrentNode);
		UE_LOG(LogTemp, Warning, TEXT("Push Close : : %d, %d, %d"), CurrentNode.x, CurrentNode.y, CurrentNode.parentIndex);

		// 현재 노드가 도착지점이라면 경로를 Close List에서 찾아서 반환
        if(CurrentNode.x == End.X && CurrentNode.y == End.Y)
        {
			Path.Push(FVector2D(CurrentNode.x, CurrentNode.y));
			int parentIndex = CurrentNode.parentIndex;
            
			while (0 < parentIndex)
            {
				if(parentIndex < CloseList.Num())
				{
				    Node ParentNode = CloseList[parentIndex];
                	Path.Push(FVector2D(ParentNode.x, ParentNode.y));
                	parentIndex = ParentNode.parentIndex;
				}
            }
            return Path;
        }

		int parentIndex = CloseList.Num() - 1;
		// 연결된 노드(배열의 상하좌우)를 Open List에 추가
		for (int i = 0; i < 4; i++)
		{
			int x = CurrentNode.x + cx[i];
			int y = CurrentNode.y + cy[i];

			// -1 은 막힌것으로 취급
			if (-1 < x && x < Map.Num() && -1 < y && y < Map.Num() && Map[x][y] != 1)
			{

				// CloseList에 없는 경우에만 Open List에 추가
				auto* ptr = CloseList.FindByPredicate([x, y](const Node& node){
					return node.x == x && node.y == y;
				});

				bool bAlreadyContain = ptr != nullptr;
                if(!bAlreadyContain)
                {
                    Node AddNode(x, y);
					AddNode.G = CurrentNode.G + 1;
					AddNode.H = (abs(End.X - x) + abs(End.Y - y)) * 10;
					AddNode.F = AddNode.G + AddNode.H;
					AddNode.parentIndex = parentIndex;
					OpenList.HeapPush(AddNode, CompareFunc);
                }
			}
		}
	}
    return Path;
}
