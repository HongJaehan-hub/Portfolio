#include <iostream>
#include <vector>
#include <math.h>
#include <queue>

using namespace std;

class Node {
public:
	Node(int _row, int _col) : row(_row), col(_col)
	{
		G = 0;
		H = 0;
		F = 0;
		parentIndex = -1;
	};

	int row, col; // ��ġ
	int G, H, F; // G : ����ġ, H : �޸���ƽ ����ġ, F : G + H
	int parentIndex; // vector<int> close list��  index���� ����
};

// �� ��  ��  �� �� �� �� ��
int row[8] = { 1, 0, -1, 0, 1, -1, -1, 1 };
int col[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
string arrow[9] = { " ", "��", "��", "��", "��", "��", "��", "��", "��" };

int GetArrowIndex(const vector<Node>& close, Node& curentNode)
{
	if (curentNode.parentIndex < 0)
		return 0;

	Node parentNode = close[curentNode.parentIndex];
	for (int i = 0; i < 8; ++i)
	{
		if ((row[i] == curentNode.row - parentNode.row) && (col[i] == curentNode.col - parentNode.col))
		{
			return i + 1;
		}
	}
	return 0;
}

void print_result(vector<vector<int>>& map, const vector<Node>& close, Node& node)
{
	map[node.row][node.col] = 99;
	int parentIndex = node.parentIndex;

	Node currentNode(node);
	while (parentIndex != -1)
	{
		Node parentNode = close[parentIndex];
		map[parentNode.row][parentNode.col] = GetArrowIndex(close, currentNode);
		parentIndex = parentNode.parentIndex;
		currentNode = parentNode;
	}

	for (int row = 0; row < map.size(); ++row)
	{
		for (int col = 0; col < map.size(); ++col)
		{
			if (map[row][col] == -1)
			{
				cout << "��" << " ";
			}
			else if (map[row][col] == 0)
			{
				cout << "��" << " ";
			}
			else if (map[row][col] == 99)
			{
				cout << "��" << " ";
			}
			else
			{
				cout << arrow[map[row][col]] << " ";
			}
		}
		cout << endl;
	}
}

void Astar(vector<vector<int>> map, pair<int, int> start, pair<int, int> end)
{
	vector<Node> open; // ���� �߰� ��带 �����ϴ� Open List
	vector<Node> close; // ���� ��� ��带 �����ϴ� Close List

	// ���� ��带 ���� �� Open�� �߰�
	Node node_start(start.first, start.second);
	open.push_back(node_start);

	while (!open.empty())
	{
		// F���� ���� ���� ��带 ������
		auto it = min_element(open.begin(), open.end(), [](Node a, Node b) {return a.F < b.F; });
		Node curNode = *it;
		open.erase(it);

		// close List�� �߰�
		close.push_back(curNode);
		int parentIndex = close.size() - 1;

		if (curNode.row == end.first && curNode.col == end.second)
		{
			print_result(map, close, curNode);
			break;
		}

		// ����� ���(�迭�� �����¿�)�� Open List�� �߰�
		for (int i = 0; i < 8; i++)
		{
			int x = curNode.row + row[i];
			int y = curNode.col + col[i];

			// -1 �� ���������� ���
			if (-1 < x && x < map.size() && -1 < y && y < map.size() && map[x][y] != -1)
			{
				// close�� �����ϴ��� üũ
				auto find = [x, y](Node node) {return node.row == x && node.col == y; };

				// close�� ���� ��쿡�� Open List�� �߰�
				bool bExistInClose = find_if(close.begin(), close.end(), find) != close.end();
				if (bExistInClose == false)
				{
					Node addNode(x, y);
					// �밢�� ����(i : 4 ~ 7)�� �޸���ƽ ����ġ�� 14(��2 * 10)������ ����
					addNode.G = curNode.G + (x - curNode.row == 0 || y - curNode.col == 0 ? 10 : 14);
					addNode.H = max(abs(end.first - x), abs(end.second - y)) * 10;
					addNode.F = addNode.G + addNode.H;

					auto it = find_if(open.begin(), open.end(), [x, y](Node n) {return n.row == x && n.col == y; });
					// open�� �ش� ��尡 �̹� ���ְ� addNode�� F���� ���� ��� �߰����� �ʰ� �ش� ���� ����
					if (it != open.end())
					{
						Node openNode = static_cast<Node>(*it);
						if (addNode.G < openNode.G)
						{
							openNode.F = addNode.F;
							openNode.G = addNode.G;
							openNode.H = addNode.H;
							openNode.parentIndex = parentIndex;
							continue;
						}
					}

					addNode.parentIndex = parentIndex;
					open.push_back(addNode);
				}
			}
		}
	}
}

int main()
{
	vector<vector<int>> map(10, vector<int>(10, 0));
	pair<int, int> start, goal;
	start = { 4, 2 };
	goal = { 9, 4 };
	map = { {0,0,0,0,0,0,0,0,0,0     },
			{0,0,0,0,0,0,0,0,0,0     },
			{0,0,-1,-1,-1,0,0,0,0,0  },
			{0,0,0,-1,0,0,0,0,0,0    },
			{0,0,0,-1,0,0,0,0,0,0    },
			{0,0,0,-1,0,0,0,0,0,0    },
			{0,0,0,-1,0,0,0,0,0,0    },
			{0,0,0,-1,-1,-1,-1,-1,0,0},
			{0,0,0,-1,0,0,0,0,0,0    },
			{0,0,0,-1,0,0,0,0,0,0    },
	};
	Astar(map, start, goal);
	return 0;
}