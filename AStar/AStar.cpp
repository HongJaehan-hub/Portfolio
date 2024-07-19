#include <iostream>
#include <vector>
#include <math.h>
#include <queue>

using namespace std;

class Node {
public:
	Node(int _x, int _y) : x(_x), y(_y)
	{
		G = 0;
		H = 0;
		F = 0;
	};

	int x, y; // ��ġ
	int G, H, F; // G : ����ġ, H : �޸���ƽ ����ġ, F : G + H
	int parentIndex; // vector<int> close list��  index���� ����
};

int cx[4] = { 0, 1, 0, -1 };
int cy[4] = { -1, 0, 1, 0 };

void print_result(vector<vector<int>> map, const vector<Node> &close, Node &node)
{
	map[node.x][node.y] = 8;
	int parentIndex = node.parentIndex;

	while (parentIndex != -1)
	{
		Node parentNode = close[parentIndex];
		map[parentNode.x][parentNode.y] = 8;
		parentIndex = parentNode.parentIndex;
	}

	for (int i = 0; i < map.size(); ++i)
	{
		for (int j = 0; j < map.size(); ++j)
		{
			cout << map[i][j] << " ";
		}
		cout << endl;
	}
}

void Astar(vector<vector<int>> map, pair<int, int> start, pair<int, int> end)
{
	auto cmp = [](Node a, Node b) {
		return a.F > b.F;
		};

	priority_queue <Node, vector<Node>, decltype(cmp)> open(cmp);
	vector<Node> close;

	Node node_start(start.first, start.second);
	node_start.parentIndex = -1;
	open.push(node_start);

	while (!open.empty())
	{
		// �켱 ���� ť���� ��带 ������ (F�� �� ���� ���� ���)
		Node curNode = open.top();
		open.pop();

		// close List�� �߰�
		close.push_back(curNode);
		int parentIndex = close.size() - 1;

		if (curNode.x == end.first && curNode.y == end.second)
		{
			print_result(map, close, curNode);
			break;
		}

		// ����� ���(�迭�� �����¿�)�� Open List�� �߰�
		for (int i = 0; i < 4; i++)
		{
			int x = curNode.x + cx[i];
			int y = curNode.y + cy[i];

			// -1 �� ���������� ���
			if (-1 < x && x < map.size() && -1 < y && y < map.size() && map[x][y] != 1)
			{
				auto find = [x, y](Node node) {
					return node.x == x && node.y == y;
					};

				// close�� ���� ��쿡�� Open List�� �߰�
				bool bExistInClose = find_if(close.begin(), close.end(), find) != close.end();
				if (bExistInClose == false)
				{
					Node addNode(x, y);
					addNode.G = curNode.G + 1;
					addNode.H = (abs(end.first - x) + abs(end.second - y)) * 10;
					addNode.F = addNode.G + addNode.H;
					addNode.parentIndex = parentIndex;
					open.push(addNode);
				}
			}
		}
	}
}

int main() 
{
	vector<vector<int>> map(10, vector<int>(10, 0));
	pair<int, int> start, goal;
	start = { 0, 0 };
	goal = { 9, 9 };
	map = { {0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,1,1,1,0,0,0,0,0},
			{0,0,0,1,0,0,0,0,0,0},
			{0,0,0,1,0,0,0,0,0,0},
			{0,0,0,1,0,0,0,0,0,0},
			{0,0,0,1,0,0,0,0,0,0},
			{0,0,0,1,1,1,1,1,0,0},
			{0,0,0,1,0,0,0,0,0,0},
			{0,0,0,1,0,0,0,0,0,0},
	};
	Astar(map, start, goal);
	return 0;
}