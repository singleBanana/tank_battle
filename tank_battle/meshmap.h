#ifndef _MESHMAP_H
#define _MESHMAP_H

#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

#define COST_BLOCK 999

struct MeshMapNode
{
	int id;
	int cost;

	//A*
	int g; //��������Ų�����·���ƶ�����ǰ����ƶ��ķ�
	int h; //�ӵ�ǰ���ƶ����յ��Ԥ���ƶ��ķ�
	int f;
	int parentid;

	MeshMapNode();

	bool operator < (const MeshMapNode & node) const;
};

struct GeoDesc
{
	int x; //left
	int y; //top
	int width;
	int height;
};

class MeshMap
{
	static MeshMap* m_instance;

	vector<vector<MeshMapNode>> m_map;
	int m_meshrnum;
	int m_meshcnum;
	int m_meshwidth;
	int m_meshheight;

	MeshMap();

public:
	static MeshMap* GetInstance();

	void Init(int mapwidth, int mapheight, int meshwidth, int meshheight);

	bool IsValidID(int id);
	int GetMaxIDInRow(int r);
	int GetMinIDInRow(int r);
	int GetMaxIDInCol(int c);
	int GetMinIDInCol(int c);

	int CalRow(int id);
	int CalCol(int id);
	RECT CalRect(int id);
	int GetCost(int id);

	void GetAroundID(vector<int>& out, int id);

	void SetLocationCost(GeoDesc loc, int cost);
	void AddBlockLocation(GeoDesc block);
	void DeleteBlockLocation(GeoDesc del);
	void SetBlockLocation(vector<GeoDesc>& blocks);
	int Pos2ID(int x, int y);
	pair<int, int> ID2Pos(int id);
	RECT GetUnBlockedRect_Row(int startid, int len); //lenΪ�������� ��Ϊ�� startС end��
	RECT GetUnBlockedRect_Col(int startid, int len); //��Ϊ��

	void AStar(vector<int>& out, int startid, int endid);
	int AStar_CalH(int id, int targetid); //�����پ���
};

#endif