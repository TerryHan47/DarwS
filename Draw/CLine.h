#pragma once
#define MAX_POINT 8192 
struct SShowState //显示状态
{
	double m_r; //放大倍率
	int m_dx; //水平移动
	int m_dy; //垂直移动
	bool m_bViewPoint; //是否显示节点
};
struct SPoint {
	int m_sn; //序号
	double m_x; //x坐标
	double m_y; //y坐标
	SPoint() { //构造函数创建空的节点
		m_sn = 0;
		m_x = 0;
		m_y = 0;
	}
	SPoint(int sn, double x, double y) { //构造函数创建节点并赋值
		m_sn = sn;
		m_x = x;
		m_y = y;
	}
	static void XY2xy(SPoint& point, SPoint p0, SShowState state) {
		//坐标转换
		point.m_x /= state.m_r; //计算缩放
		point.m_y /= state.m_r; //计算缩放
		p0.m_y /= state.m_r; //计算缩放
		p0.m_x /= state.m_r; //计算缩放
		point.m_x -= p0.m_x; //计算移动
		point.m_y = p0.m_y - point.m_y; //计算移动
	}
	static void xy2XY(SPoint& point, SPoint p0, SShowState state) {
		//坐标转换
		point.m_x *= state.m_r; //计算缩放
		point.m_y *= state.m_r; //计算缩放
		point.m_x = point.m_x + p0.m_x; //计算移动
		point.m_y = p0.m_y - point.m_y; //计算移动
	}
};
class CLine
{
	int m_sn; //线序号,从1开始
	SPoint* m_pPoint; //节点数组
	int m_nPoint; //节点数量
	SPoint* m_pCurPoint; //当前节点
public:
	CLine(int sn, SPoint& p0); //构造函数
	~CLine();
	int GetNum() { return m_nPoint; } //取得节点数量
	SPoint GetPoint(int i) { return m_pPoint[i]; }//取得指定点
	int AddPoint(SPoint& p0); //添加一个点,返回点序号
	bool DelPoint(); //删除一个点,成功返回true 
	bool SetCurrent(SPoint p0, int d = 3); //根据p0选择当前线?点,d为范围
	void ShowLine(CDC* pDC, SPoint p0, SShowState state);//显示线
	void ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d = 2);//显示点
	void ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d = 3);
	//显示当前点
};