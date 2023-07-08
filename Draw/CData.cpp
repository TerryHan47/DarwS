#include "pch.h"
#include "CData.h"
CData::CData() {
	m_nLine = 0;
}
CData::~CData() {
	Clear();
}
int CData::AddLine(SPoint p0) { // ���һ�����߷��ظ������
	m_pLine[m_nLine] = new CLine(m_nLine, p0);
	m_pCurLine = m_pLine[m_nLine]; //����Ϊ��ǰ��
	m_nLine++;
	return m_nLine - 1;
}
int CData::AddPoint(SPoint p0, int sn) {//Ϊָ�������һ����,���ص����
	if (sn)
		return m_pLine[sn - 1]->AddPoint(p0);
	else if (m_pCurLine) {
		return m_pCurLine->AddPoint(p0);
	}
	else {
		AddLine(p0);
		return p0.m_sn;
	}
}
bool CData::SetCurrent(SPoint p0, int d) { //ѡ��ǰ��,��
	for (int i = 0; i < m_nLine; i++) {
		if (m_pLine[i]->SetCurrent(p0, d)) {
			m_pCurLine = m_pLine[i];
			return true;
		}
	}
	return false;

}
void CData::ShowLine(CDC* pDC, SPoint p0, SShowState state) {
	for (int i = 0; i < m_nLine; i++)
		m_pLine[i]->ShowLine(pDC, p0, state);
}
void CData::ShowCurLine(CDC* pDC, SPoint p0, SShowState state) {
	if (m_pCurLine)
		m_pCurLine->ShowLine(pDC, p0, state);
}
bool CData::DelLine() {
	if (!m_pCurLine)
		return false;
	for (int i = 0; i < m_nLine; i++)
		if (m_pCurLine == m_pLine[i]) {
			delete m_pCurLine;
			m_pCurLine = NULL;
			for (int j = i; j < m_nLine; j++) {
				m_pLine[j] = m_pLine[j + 1];
			}
			m_pLine[m_nLine - 1] = NULL;
			m_nLine--;
			return true;
		}
	return false;
}
void CData::ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d) {
	for (int i = 0; i < m_nLine; i++)
		m_pLine[i]->ShowPoint(pDC, p0, state, d);
}
void CData::ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d) {
	if (m_pCurLine)
		m_pCurLine->ShowCurPoint(pDC, p0, state, d);
}
bool CData::DelPoint() {
	if (!m_pCurLine)
		return false;
	return m_pCurLine->DelPoint();
}
bool CData::ReadPlt(const char* szFname) {
	FILE* plt;
	if (fopen_s(&plt, szFname, "rt"))
		return false;
	Clear();
	char cBuf[256]; //�����ļ���һ��
	int x, y; //����PLT������
	while (!feof(plt)) {
		fgets(cBuf, sizeof(cBuf), plt);
		if (strstr(cBuf, "PU")) { //��ʼ����
			EndLine();
			sscanf_s(cBuf, "PU%d%d", &x, &y);
		}
		else if (strstr(cBuf, "PD")) {
			sscanf_s(cBuf, "PD%d%d", &x, &y);
		}
		else
			continue;
		AddPoint(SPoint(0, x / 40.0, y / 40.0)); //תΪ����
	}
	EndLine();
	fclose(plt);
	return true;

}
bool CData::WritePlt(const char* szFname) {
	FILE* fp;
	if (fopen_s(&fp, szFname, "wt"))
		return false;
	fprintf(fp, "IN;\n");
	for (int i = 0; i < m_nLine; i++) {
		fprintf(fp, "SP1;\n");
		fprintf(fp, "PU%d %d;\n", (int)(m_pLine[i]->GetPoint(0).
			m_x * 40), (int)(m_pLine[i]->GetPoint(0).m_y * 40));
		for (int j = 1; j < m_pLine[i]->GetNum(); j++) {
			fprintf(fp, "PD%d %d;\n", (int)(m_pLine[i]->GetPoint(j).m_x * 40), (int)(m_pLine[i]->GetPoint(j).m_y * 40));
		}
	}
	fprintf(fp, "SP0;\n");
	fclose(fp);
	return true;
}
void CData::Clear() { // �������
	for (int i = 0; i < m_nLine; i++)
		delete m_pLine[i];
	m_nLine = 0;
	m_pCurLine = nullptr;
}