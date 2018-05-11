#include "meshmap.h"
#include "gameplay.h"
#include "helperfunc.h"

#include <queue>

using namespace std;

MeshMapNode::MeshMapNode()
{
	id = -1;
	cost = 1;

	parentid = -1;
	g = 0;
	h = 0;
	f = 0;
}

bool MeshMapNode::operator < (const MeshMapNode & node) const
{
	return f > node.f;
}

MeshMap* MeshMap::m_instance = nullptr;

MeshMap::MeshMap()
{
	
}

MeshMap* MeshMap::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new MeshMap;
	}

	return m_instance;
}

void MeshMap::Init(int mapwidth, int mapheight, int meshwidth, int meshheight)
{
	m_meshwidth = meshwidth;
	m_meshheight = meshheight;

	m_meshrnum = mapheight / m_meshheight;
	if (mapheight % m_meshheight)
	{
		++m_meshrnum;
	}
	m_meshcnum = mapwidth / m_meshwidth;
	if (mapwidth % m_meshwidth)
	{
		++m_meshcnum;
	}

	int nodeid = 0;
	for (int i = 0; i < m_meshrnum; ++i)
	{
		vector<MeshMapNode> vecr;
		for (int j = 0; j < m_meshcnum; ++j)
		{
			MeshMapNode node;
			node.id = nodeid;
			++nodeid;
			vecr.push_back(node);
		}
		m_map.push_back(vecr);
	}
}

bool MeshMap::IsValidID(int id)
{
	if (id >= 0 && id <= m_meshrnum * m_meshcnum - 1)
	{
		return true;
	}

	return false;
}

int MeshMap::GetMaxIDInRow(int r)
{
	return (r + 1) * m_meshcnum - 1;
}

int MeshMap::GetMinIDInRow(int r)
{
	return (GetMaxIDInRow(r) - m_meshcnum + 1);
}

int MeshMap::GetMaxIDInCol(int c)
{
	return (m_meshrnum - 1) * m_meshcnum + c;
}

int MeshMap::GetMinIDInCol(int c)
{
	return c;
}

int MeshMap::CalRow(int id)
{
	return (id / m_meshcnum);
}

int MeshMap::CalCol(int id)
{
	return (id % m_meshcnum);
}

RECT MeshMap::CalRect(int id)
{
	RECT r;
	int row = CalRow(id);
	int col = CalCol(id);
	r.top = row * m_meshheight;
	r.left = col * m_meshwidth;
	r.bottom = r.top + m_meshheight;
	r.right = r.left + m_meshwidth;

	return r;
}

int MeshMap::GetCost(int id)
{
	return m_map[CalRow(id)][CalCol(id)].cost;
}

void MeshMap::GetAroundID(vector<int>& out, int id)
{
	out.clear();

	//¥”…œÕ˘œ¬ ¥”◊ÛÕ˘”“
	out.push_back(id - m_meshcnum - 1);
	out.push_back(id - m_meshcnum);
	out.push_back(id - m_meshcnum + 1);
	out.push_back(id - 1);
	out.push_back(id + 1);
	out.push_back(id + m_meshcnum - 1);
	out.push_back(id + m_meshcnum);
	out.push_back(id + m_meshcnum + 1);

	for (int i = 0; i < out.size(); ++i)
	{
		if (!IsValidID(out[i]))
		{
			out[i] = -1;
		}
	}
}

void MeshMap::SetLocationCost(GeoDesc loc, int cost)
{
	int ctopleft = loc.x / m_meshwidth;
	int rtopleft = loc.y / m_meshheight;
	int ctopright = (loc.x + loc.width) / m_meshwidth;
	int rbottomleft = (loc.y + loc.height) / m_meshheight;

	int rindex = rtopleft;
	int cindex = ctopleft;
	while (rindex <= rbottomleft)
	{
		while (cindex <= ctopright)
		{
			m_map[rindex][cindex].cost = cost;
			++cindex;
		}
		cindex = ctopleft;
		++rindex;
	}
}

void MeshMap::AddBlockLocation(GeoDesc block)
{
	SetLocationCost(block, COST_BLOCK);
}

void MeshMap::DeleteBlockLocation(GeoDesc del)
{
	SetLocationCost(del, 1);
}

void MeshMap::SetBlockLocation(vector<GeoDesc>& blocks)
{
	for (int i = 0; i < blocks.size(); ++i)
	{
		AddBlockLocation(blocks[i]);
	}
}

int MeshMap::Pos2ID(int x, int y)
{
	int c = x / m_meshwidth;
	int r = y / m_meshheight;

	return (r * m_meshcnum + c);
}

pair<int, int> MeshMap::ID2Pos(int id)
{
	int r = CalRow(id);
	int c = CalCol(id);
	pair<int, int> p;
	p.first = c * m_meshwidth + m_meshwidth / 2;
	p.second = r * m_meshheight + m_meshheight / 2;

	return p;
}

RECT MeshMap::GetUnBlockedRect_Row(int startid, int len)
{
	int logicendid;
	int logicstartid;
	int actendid;
	RECT rs;
	RECT re;
	if (len > 0) //Õ˘”“—”…Ï
	{
		logicstartid = startid;
		logicendid = min(GetMaxIDInRow(CalRow(logicstartid)), logicstartid + len - 1);

		actendid = logicendid;
		for (int i = logicstartid; i <= logicendid; ++i)
		{
			if (m_map[CalRow(i)][CalCol(i)].cost == COST_BLOCK)
			{
				actendid = i;

				break;
			}
		}
		rs = CalRect(logicstartid);
		re = CalRect(actendid);
	}
	else //Õ˘◊Û—”…Ï
	{
		logicendid = startid;
		logicstartid = max(GetMinIDInRow(CalRow(logicendid)), logicendid + len + 1);

		actendid = logicstartid;
		for (int i = logicendid; i >= logicstartid; --i)
		{
			if (m_map[CalRow(i)][CalCol(i)].cost == COST_BLOCK)
			{
				actendid = i;

				break;
			}
		}
		rs = CalRect(logicendid);
		re = CalRect(actendid);
	}

	return ExpandRECT(rs, re);
}

RECT MeshMap::GetUnBlockedRect_Col(int startid, int len)
{
	int logicendid;
	int logicstartid;
	int actendid;
	RECT rs;
	RECT re;
	if (len > 0) //Õ˘œ¬—”…Ï
	{
		logicstartid = startid;
		logicendid = min(GetMaxIDInCol(CalCol(logicstartid)), logicstartid + (len - 1) * m_meshcnum);

		actendid = logicendid;
		for (int i = logicstartid; i <= logicendid; i += m_meshcnum)
		{
			if (m_map[CalRow(i)][CalCol(i)].cost == COST_BLOCK)
			{
				actendid = i;

				break;
			}
		}
		rs = CalRect(logicstartid);
		re = CalRect(actendid);
	}
	else //Õ˘…œ—”…Ï
	{
		logicendid = startid;
		logicstartid = max(GetMinIDInCol(CalCol(logicendid)), logicendid + (len - 1) * m_meshcnum);

		actendid = logicstartid;
		for (int i = logicendid; i >= logicstartid; i -= m_meshcnum)
		{
			if (m_map[CalRow(i)][CalCol(i)].cost == COST_BLOCK)
			{
				actendid = i;

				break;
			}
		}
		rs = CalRect(logicendid);
		re = CalRect(actendid);
	}

	return ExpandRECT(rs, re);
}

void MeshMap::AStar(vector<int>& out, int startid, int endid)
{
	out.clear();

	for (int i = 0; i < m_map.size(); ++i)
	{
		for (int j = 0; j < m_map[i].size(); ++j)
		{
			m_map[i][j].parentid = -1;
			m_map[i][j].g = 0;
			m_map[i][j].h = 0;
			m_map[i][j].f = 0;
		}
	}

	priority_queue<MeshMapNode> queue;
	int startr = CalRow(startid);
	int startc = CalCol(startid);
	m_map[startr][startc].g = 0;
	m_map[startr][startc].h = AStar_CalH(startid, endid);
	m_map[startr][startc].f = m_map[startr][startc].g + m_map[startr][startc].h;
	queue.push(m_map[startr][startc]);

	MeshMapNode t;
	vector<bool> vecclose;
	vecclose.resize(m_meshrnum * m_meshcnum);
	for (int i = 0; i < vecclose.size(); ++i)
	{
		vecclose[i] = false;
	}
	while (!queue.empty())
	{
		t = queue.top();
		queue.pop();
		vecclose[t.id] = true;

		if (t.id == endid)
		{
			break;
		}

		vector<int> aroundid;
		GetAroundID(aroundid, t.id);
		for (int i = 0; i < aroundid.size(); ++i)
		{
			if (aroundid[i] != -1 && !vecclose[aroundid[i]] && GetCost(aroundid[i]) != COST_BLOCK)
			{
				int sr = CalRow(aroundid[i]);
				int sc = CalCol(aroundid[i]);
				m_map[sr][sc].g = t.g + m_map[sr][sc].cost;
				m_map[sr][sc].h = AStar_CalH(aroundid[i], endid);
				m_map[sr][sc].f = m_map[sr][sc].g + m_map[sr][sc].h;
				m_map[sr][sc].parentid = t.id;
				queue.push(m_map[sr][sc]);
			}
		}
	}

	out.push_back(endid);
	int curid = endid;
	while (true)
	{
		int pid = m_map[CalRow(curid)][CalCol(curid)].parentid;

		if (pid == -1)
		{
			break;
		}

		out.push_back(pid);
		curid = pid;
	}
}

int MeshMap::AStar_CalH(int id, int targetid)
{
	return (abs(CalRow(id) - CalRow(targetid)) + abs(CalCol(id) - CalCol(targetid)));
}