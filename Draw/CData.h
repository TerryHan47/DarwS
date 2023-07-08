#pragma once
#define MAX_LINE 1024 //һ��ͼ�����1024������
#define MAX_POINT 8192 
struct SShowState //��ʾ״̬
{
	double m_r; //�Ŵ���
	int m_dx; //ˮƽ�ƶ�
	int m_dy; //��ֱ�ƶ�
	bool m_bViewPoint; //�Ƿ���ʾ�ڵ�
};
struct SPoint {
	int m_sn; //���
	double m_x; //x����
	double m_y; //y����
	SPoint() { //���캯�������յĽڵ�
		m_sn = 0;
		m_x = 0;
		m_y = 0;
	}
	SPoint(int sn, double x, double y) { //���캯�������ڵ㲢��ֵ
		m_sn = sn;
		m_x = x;
		m_y = y;
	}
	static void XY2xy(SPoint& point, SPoint p0, SShowState state) {
		//����ת��
		point.m_x /= state.m_r; //��������
		point.m_y /= state.m_r; //��������
		p0.m_y /= state.m_r; //��������
		p0.m_x /= state.m_r; //��������
		point.m_x -= p0.m_x; //�����ƶ�
		point.m_y = p0.m_y - point.m_y; //�����ƶ�
	}
	static void xy2XY(SPoint& point, SPoint p0, SShowState state) {
		//����ת��
		point.m_x *= state.m_r; //��������
		point.m_y *= state.m_r; //��������
		point.m_x = point.m_x + p0.m_x; //�����ƶ�
		point.m_y = p0.m_y - point.m_y; //�����ƶ�
	}
};
class CLine
{
	int m_sn; //�����,��1��ʼ
	SPoint* m_pPoint; //�ڵ�����
	int m_nPoint; //�ڵ�����
	SPoint* m_pCurPoint; //��ǰ�ڵ�
public:
	CLine(int sn, SPoint& p0); //���캯��
	~CLine();
	int GetNum() { return m_nPoint; } //ȡ�ýڵ�����
	SPoint GetPoint(int i) { return m_pPoint[i]; }//ȡ��ָ����
	int AddPoint(SPoint& p0); //���һ����,���ص����
	bool DelPoint(); //ɾ��һ����,�ɹ�����true 
	bool SetCurrent(SPoint p0, int d = 3); //����p0ѡ��ǰ��?��,dΪ��Χ
	void ShowLine(CDC* pDC, SPoint p0, SShowState state);//��ʾ��
	void ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d = 2);//��ʾ��
	void ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d = 3);
	//��ʾ��ǰ��
};
class CData {
	CLine* m_pLine[MAX_LINE];
	int m_nLine;
	CLine* m_pCurLine; //��ǰ��
public:
	CData();
	~CData();
	int GetNum() { return m_nLine; }
	int AddLine(SPoint p0); //���һ�����߷��ظ������
	int AddPoint(SPoint p0, int sn = 0);//Ϊָ�������һ����,sn=0Ĭ�ϵ�ǰ��
	bool SetCurrent(SPoint p0, int d = 3); //ѡ��ǰ��?��
	void EndLine() { m_pCurLine = nullptr; }
	bool DelLine();
	bool DelPoint();
	void ShowLine(CDC* pDC, SPoint p0, SShowState state);
	void ShowCurLine(CDC* pDC, SPoint p0, SShowState state);
	void ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d = 2);
	void ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d = 3);
	bool ReadPlt(const char* szFname); //��PLT�ļ�����ͼ��
	bool WritePlt(const char* szFname); //��ͼ������д��PLT�ļ�
	void Clear(); //�������
};