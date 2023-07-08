
// DrawDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Draw.h"
#include "DrawDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C绘图软件Dlg 对话框


CDrawDlg::CDrawDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAW_DIALOG, pParent)
{
	m_w = 1500;
	m_h = 1200;
	m_x1 = 150;
	m_y1 = 10;
	m_x2 = m_w - 50;
	m_y2 = m_h - 150;
	m_StateShow.m_r = 1.0; //放大倍率
	m_StateShow.m_dx = 0; //水平移动
	m_StateShow.m_dy = 0; //垂直移动
	m_StateShow.m_bViewPoint = true; //显示节点
	m_ColorLine = RGB(127, 255, 127); //亮绿色
	m_ColorPoint = RGB(255, 0, 255); //黄色
	m_ColorLineCur = RGB(255, 127, 255); //亮黄色
	m_ColorPointCur = RGB(255, 255, 255); //黄色
	m_bDraw = true;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mousemode = 0;
	dxfmode = 0;
}

void CDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDrawDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_OPEN, &CDrawDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_NEW, &CDrawDlg::OnBnClickedNew)
	ON_BN_CLICKED(IDC_SAVE, &CDrawDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_SAVE_AS, &CDrawDlg::OnBnClickedSaveAs)
	ON_BN_CLICKED(IDC_ZOOMIN, &CDrawDlg::OnBnClickedZoomin)
	ON_BN_CLICKED(IDC_ZOOMOUT, &CDrawDlg::OnBnClickedZoomout)
	ON_BN_CLICKED(IDC_ZOOMDEF, &CDrawDlg::OnBnClickedZoomdef)
	ON_BN_CLICKED(IDC_MUP, &CDrawDlg::OnBnClickedMup)
	ON_BN_CLICKED(IDC_MDOWN, &CDrawDlg::OnBnClickedMdown)
	ON_BN_CLICKED(IDC_ML, &CDrawDlg::OnBnClickedMl)
	ON_BN_CLICKED(IDC_MR, &CDrawDlg::OnBnClickedMr)
	ON_BN_CLICKED(IDC_VIEW_POINT, &CDrawDlg::OnBnClickedViewPoint)
	ON_BN_CLICKED(IDC_HIDE_POINT, &CDrawDlg::OnBnClickedHidePoint)
	ON_BN_CLICKED(IDC_DRAW, &CDrawDlg::OnBnClickedDraw)
	ON_BN_CLICKED(IDC_SEL, &CDrawDlg::OnBnClickedSel)
	ON_BN_CLICKED(IDC_DEL_POINT, &CDrawDlg::OnBnClickedDelPoint)
	ON_BN_CLICKED(IDC_DEL_LINE, &CDrawDlg::OnBnClickedDelLine)
	ON_WM_CLOSE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_OPENDXF, &CDrawDlg::OnOpenDxf)
END_MESSAGE_MAP()


// C绘图软件Dlg 消息处理程序

BOOL CDrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowPos(NULL, 0, 0, m_w, m_h, SWP_NOZORDER | SWP_NOMOVE);
	m_Font.CreatePointFont(105, "黑体", NULL);
	GetDlgItem(IDC_SAVE)->EnableWindow(false);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE 
}

void CDrawDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDrawDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		Draw(&dc);
		ShowStatus(&dc);
		CDialogEx::OnPaint();
	}
	if (dxfmode==1)
	{
		CWnd* pWnd = GetDlgItem(IDC_PANEL);
		CDC* pDC = pWnd->GetDC();
		CRect rectClient;
		pWnd->GetClientRect(&rectClient);
		m_DxfRead.InitView(rectClient.left, rectClient.top, rectClient.right - rectClient.left, rectClient.bottom - rectClient.top);
		m_DxfRead.Paint(pDC->m_hDC);
		ReleaseDC(pDC);
	}
}

void CDrawDlg::Draw(CDC* pDC) {
	CBrush* pOldBrush, BrushBk(RGB(0, 0, 0));
	pOldBrush = pDC->SelectObject(&BrushBk);
	pDC->Rectangle(m_x1, m_y1, m_x2, m_y2);
	pDC->SelectObject(pOldBrush);
	CPen* pOldPen;
	CPen NewPen(PS_SOLID, 1, m_ColorLine);
	pOldPen = pDC->SelectObject(&NewPen);
	m_Data.ShowLine(pDC, SPoint(0, m_x1, m_y2), m_StateShow);
	CPen NewPenCur(PS_SOLID, 1, m_ColorLineCur);
	pDC->SelectObject(&NewPenCur);
	m_Data.ShowCurLine(pDC, SPoint(0, m_x1, m_y2), m_StateShow);
	if (m_StateShow.m_bViewPoint) {
		CPen NewPen(PS_SOLID, 1, m_ColorPoint);
		pDC->SelectObject(&NewPen);
		m_Data.ShowPoint(pDC, SPoint(0, m_x1, m_y2), m_StateShow);
		CPen NewPenCur(PS_SOLID, 1, m_ColorPointCur);
		pDC->SelectObject(&NewPenCur);
		m_Data.ShowCurPoint(pDC, SPoint(0, m_x1, m_y2), m_StateShow);
	}
	pDC->SelectObject(pOldPen);
}

void CDrawDlg::OnLButtonUp(UINT nFlags, CPoint point) {
	if (point.x <= m_x1 || point.x >= m_x2 || point.y <= m_y1 || point.y
		>= m_y2)
		return;
	point.x -= m_StateShow.m_dx;
	point.y += m_StateShow.m_dy;
	SPoint tPoint = { 0,(double)point.x, (double)point.y };
	SPoint::XY2xy(tPoint, SPoint(0, m_x1, m_y2), m_StateShow);
	if (m_bDraw)
		m_Data.AddPoint(tPoint);
	else
		m_Data.SetCurrent(tPoint);
	CClientDC dc(this);
	Draw(&dc);
	ShowStatus(&dc);
	if (mousemode == 1)
	{
		mousemode = 0;
		SPoint sPoint = { 0,(double)point.x, (double)point.y };
		SPoint::XY2xy(sPoint, SPoint(0, m_x1, m_y2), m_StateShow);
		pt_End.x = sPoint.m_x;
		pt_End.y = sPoint.m_y;
		Movey = pt_End.y - pt_Begin.y;
		Movex = pt_End.x - pt_Begin.x;
		m_StateShow.m_dy += Movey / 2;
		m_StateShow.m_dx += Movex / 2;

		Invalidate();
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CDrawDlg::OnRButtonDown(UINT nFlags, CPoint point) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Data.EndLine();
	CDialogEx::OnRButtonDown(nFlags, point);
}
void CDrawDlg::OnBnClickedOpen() {
	char szFilter[] = "HPGL Plotter files (*.PLT)|*.plt|Text files(*.txt) | *.txt || ";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() == IDOK) {
		m_csFilePath = dlg.GetPathName();
		m_Data.ReadPlt(dlg.GetPathName());
		GetDlgItem(IDC_SAVE)->EnableWindow(true);
		Invalidate();
	}
}
void CDrawDlg::OnBnClickedSave() {
	m_Data.WritePlt(m_csFilePath);
	AfxMessageBox("文件保存成功!");
}
void CDrawDlg::OnBnClickedSaveAs() {
	char szFilter[] = "HPGL Plotter files(*.plt)|*.plt|Text files (*.txt) | *.txt || ";
	CFileDialog dlg(FALSE, "plt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() == IDOK) {
		m_csFilePath = dlg.GetPathName();
		m_Data.WritePlt(dlg.GetPathName());
		AfxMessageBox("文件保存成功!");
		CClientDC dc(this);
		ShowStatus(&dc);
	}
}
void CDrawDlg::OnMouseMove(UINT nFlags, CPoint point) {//输出光标坐标
	CString csString;
	SPoint tPoint(0, point.x, point.y);
	SPoint::XY2xy(tPoint, SPoint(0, 105, m_h - 70), m_StateShow);
	csString.Format("x=%04d,y=%04d", (int)tPoint.m_x - m_StateShow.
		m_dx, (int)tPoint.m_y - m_StateShow.m_dy);
	CClientDC dc(this);
	dc.TextOut(m_w - 140, m_h - 65, csString);

	CDialogEx::OnMouseMove(nFlags, point);

}
void CDrawDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x <= m_x1 || point.x >= m_x2 || point.y <= m_y1 || point.y
		>= m_y2)
		return;
	if (m_bDraw == false)
	{
		mousemode = 1;
		SPoint sPoint = { 0,(double)point.x, (double)point.y };
		SPoint::XY2xy(sPoint, SPoint(0, m_x1, m_y2), m_StateShow);
		pt_Begin.x = sPoint.m_x;
		pt_Begin.y = sPoint.m_y;

	}
	CDialogEx::OnLButtonDown(nFlags, point);
}
void CDrawDlg::OnBnClickedNew() {
	m_csFilePath = "";
	GetDlgItem(IDC_SAVE)->EnableWindow(false);
	m_Data.Clear();
	Invalidate();
	CClientDC dc(this);
	ShowStatus(&dc);
	dxfmode = 0;
}
void CDrawDlg::ShowStatus(CDC* pDC) {
	CString csString;
	csString.Format("图形总数:%04d;放大倍率:%0.2f;横向偏移:%04d; 纵向偏移: % 04d; 文件路径: % s", m_Data.GetNum(), m_StateShow.m_r, m_StateShow.m_dx, m_StateShow.m_dy, m_csFilePath);
	CFont* pOldFont = pDC->SelectObject(&m_Font);
	pDC->TextOut(10, m_h - 100, csString);
	pDC->SelectObject(pOldFont);
}
void CDrawDlg::OnBnClickedZoomin() {
	m_StateShow.m_r *= 1.2;
	Invalidate();
}
void CDrawDlg::OnBnClickedZoomout() {
	m_StateShow.m_r *= 0.8;
	Invalidate();
}
void CDrawDlg::OnBnClickedZoomdef() {
	m_StateShow.m_r = 1.0;
	Invalidate();
}
void CDrawDlg::OnBnClickedMup() {
	m_StateShow.m_dy += 100;
	Invalidate();
}
void CDrawDlg::OnBnClickedMdown() {
	m_StateShow.m_dy -= 100;
	Invalidate();
}
void CDrawDlg::OnBnClickedMl() {
	m_StateShow.m_dx -= 100;
	Invalidate();
}
void CDrawDlg::OnBnClickedMr() {
	m_StateShow.m_dx += 100;
	Invalidate();
}
void CDrawDlg::OnBnClickedViewPoint() {
	GetDlgItem(IDC_VIEW_POINT)->EnableWindow(false);
	GetDlgItem(IDC_HIDE_POINT)->EnableWindow(true);
	m_StateShow.m_bViewPoint = true;
	Invalidate();
}
void CDrawDlg::OnBnClickedHidePoint() {
	GetDlgItem(IDC_HIDE_POINT)->EnableWindow(false);
	GetDlgItem(IDC_VIEW_POINT)->EnableWindow(true);
	m_StateShow.m_bViewPoint = false;
	Invalidate();
}
void CDrawDlg::OnBnClickedDraw() {
	GetDlgItem(IDC_DRAW)->EnableWindow(false);
	GetDlgItem(IDC_SEL)->EnableWindow(true);
	m_bDraw = true;
}
void CDrawDlg::OnBnClickedSel() {
	GetDlgItem(IDC_SEL)->EnableWindow(false);
	GetDlgItem(IDC_DRAW)->EnableWindow(true);
	m_bDraw = false;
}
void CDrawDlg::OnBnClickedDelPoint() {
	m_Data.DelPoint();
	Invalidate();

}
void CDrawDlg::OnBnClickedDelLine() {
	m_Data.DelLine();
	Invalidate();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDrawDlg::OnClose()
{
	int iRet = AfxMessageBox("确定退出程序吗？退出前确定文件是否已经保存！",
		MB_OKCANCEL | MB_ICONQUESTION);
	if (iRet == IDOK)
		CDialogEx::OnClose();
}


BOOL CDrawDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0)
	{
		m_StateShow.m_r *= 1.2;
		Invalidate();
	}
	if (zDelta < 0)
	{
		m_StateShow.m_r *= 0.8;
		Invalidate();
	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CDrawDlg::OnOpenDxf()
{
	dxfmode = 1;
	CFileDialog fileDlg(TRUE);
	fileDlg.m_ofn.lpstrTitle = "选择DXF文件";
	fileDlg.m_ofn.lpstrFilter = "DXF Files (*.dxf)\0*.dxf\0";
	fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	if (IDOK == fileDlg.DoModal())
	{
		CString FilePath = fileDlg.GetPathName();
		::SetWindowText(this->m_hWnd, FilePath);
		if (!m_DxfRead.LoadDXFFile(FilePath, m_hWnd))
			MessageBox("打开失败");
	}
	Invalidate();
}


