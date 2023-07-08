
// DrawDlg.h: 头文件
//
#include ".\DXF\DxfLib.h"
#pragma once
#define MAX_LINE 1024 //一个图形最多1024个曲线
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
	bool SetCurrent(SPoint p0, int d = 3); //根据p0选择当前线､点,d为范围
	void ShowLine(CDC* pDC, SPoint p0, SShowState state);//显示线
	void ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d = 2);//显示点
	void ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d = 3);
	//显示当前点
};
class CData {
	CLine* m_pLine[MAX_LINE];
	int m_nLine;
	CLine* m_pCurLine; //当前线
public:
	CData();
	~CData();
	int GetNum() { return m_nLine; }
	int AddLine(SPoint p0); //添加一条新线返回该线序号
	int AddPoint(SPoint p0, int sn = 0);//为指定线添加一个点,sn=0默认当前线
	bool SetCurrent(SPoint p0, int d = 3); //选择当前线､点
	void EndLine() { m_pCurLine = nullptr; }
	bool DelLine();
	bool DelPoint();
	void ShowLine(CDC* pDC, SPoint p0, SShowState state);
	void ShowCurLine(CDC* pDC, SPoint p0, SShowState state);
	void ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d = 2);
	void ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d = 3);
	bool ReadPlt(const char* szFname); //从PLT文件读入图形
	bool WritePlt(const char* szFname); //将图形数据写入PLT文件
	void Clear(); //清空数据
};


// CDrawDlg 对话框
class CDrawDlg : public CDialogEx
{
	// 构造
public:
	CDrawDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:

	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	int m_w; //对话框窗口宽度
	int m_h; //对话框窗口高度
	int m_x1; //绘图区域左上角物理坐标x 
	int m_y1; //绘图区域左上角物理坐标y 
	int m_x2; //绘图区域右下角物理坐标x 
	int m_y2; //绘图区域右下角物理坐标y 
	int mousemode;
	int dxfmode;
	int Movex;
	int Movey;
	CPoint pt_Begin;
	CPoint pt_End;
	SShowState m_StateShow; //当前显示状态
	bool m_bDraw; //true绘图,false选择
	CData m_Data; //数据处理对象
	CString m_csFilePath; //当前文件路径
	CFont m_Font; //当前字体
	COLORREF m_ColorLine; //画线颜色
	COLORREF m_ColorPoint; //画点颜色
	COLORREF m_ColorLineCur; //当前线颜色
	COLORREF m_ColorPointCur; //当前点颜色
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);//鼠标左键弹起消息
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);//鼠标右键按键消息
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);//鼠标移动消息
	afx_msg void OnBnClickedNew(); //新建图形
	afx_msg void OnBnClickedOpen(); //打开文件
	afx_msg void OnBnClickedSave(); //保存文件
	afx_msg void OnBnClickedSaveAs(); //文件另存
	afx_msg void OnBnClickedZoomin(); //显示放大
	afx_msg void OnBnClickedZoomout(); //显示缩小
	afx_msg void OnBnClickedZoomdef(); //显示1∶1 
	afx_msg void OnBnClickedMup(); //显示上移
	afx_msg void OnBnClickedMdown(); //显示下移
	afx_msg void OnBnClickedMl(); //显示左移
	afx_msg void OnBnClickedMr(); //显示右移
	afx_msg void OnBnClickedViewPoint(); //显示节点
	afx_msg void OnBnClickedHidePoint(); //隐藏节点
	afx_msg void OnBnClickedDraw(); //绘图状态
	afx_msg void OnBnClickedSel(); //选择状态
	afx_msg void OnBnClickedDelPoint(); //删除节点
	afx_msg void OnBnClickedDelLine(); //删除线
	void ShowStatus(CDC* pDC); //显示状态信息
	void Draw(CDC* pDC); //绘制图形
	afx_msg void OnClose();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CDrawing m_DxfRead;
	afx_msg void OnOpenDxf();
};