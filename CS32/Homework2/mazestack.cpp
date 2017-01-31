#include <stack>
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
	stack<Coord> c_stack;
	char searched = '&';

	c_stack.push(start);
	maze[sr][sc] = searched;

	while (!c_stack.empty())
	{
		Coord current = c_stack.top();
		c_stack.pop();

		if (current.r() == er && current.c() == ec)
			return true;
		else
		{

			if (maze[current.r() - 1][current.c()] == '.')
			{
				Coord NORTH(current.r() - 1, current.c());
				c_stack.push(NORTH);
				maze[current.r() - 1][current.c()] = searched;
			}

			if (maze[current.r()][current.c() + 1] == '.')
			{
				Coord EAST(current.r(), current.c() + 1);
				c_stack.push(EAST);
				maze[current.r()][current.c() + 1] = searched;
			}

			if (maze[current.r() + 1][current.c()] == '.')
			{
				Coord SOUTH(current.r() + 1, current.c());
				c_stack.push(SOUTH);
				maze[current.r() + 1][current.c()] = searched;
			}

			if (maze[current.r()][current.c() - 1] == '.')
			{
				Coord WEST(current.r(), current.c() - 1);
				c_stack.push(WEST);
				maze[current.r()][current.c() - 1] = searched;
			}
			
		}
	}
	return false;
}