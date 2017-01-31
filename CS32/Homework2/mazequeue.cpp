#include <queue>
#include <string>

using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	Coord start(sr, sc);
	queue<Coord> c_queue;
	char searched = '&';

	c_queue.push(start);
	maze[sr][sc] = searched;

	while (!c_queue.empty())
	{
		Coord current = c_queue.front();
		c_queue.pop();

		if (current.r() == er && current.c() == ec)
			return true;
		else
		{

			if (maze[current.r() - 1][current.c()] == '.')
			{
				Coord NORTH(current.r() - 1, current.c());
				c_queue.push(NORTH);
				maze[current.r() - 1][current.c()] = searched;
			}

			if (maze[current.r()][current.c() + 1] == '.')
			{
				Coord EAST(current.r(), current.c() + 1);
				c_queue.push(EAST);
				maze[current.r()][current.c() + 1] = searched;
			}

			if (maze[current.r() + 1][current.c()] == '.')
			{
				Coord SOUTH(current.r() + 1, current.c());
				c_queue.push(SOUTH);
				maze[current.r() + 1][current.c()] = searched;
			}

			if (maze[current.r()][current.c() - 1] == '.')
			{
				Coord WEST(current.r(), current.c() - 1);
				c_queue.push(WEST);
				maze[current.r()][current.c() - 1] = searched;
			}

		}
	}
	return false;
}