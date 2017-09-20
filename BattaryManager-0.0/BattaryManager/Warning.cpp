// Warning.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BattaryManager.h"
#include "Warning.h"
#include "afxdialogex.h"


// CWarning �Ի���

IMPLEMENT_DYNAMIC(CWarning, CDialogEx)

CWarning::CWarning(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWarning::IDD, pParent)
{

}

CWarning::~CWarning()
{
}

void CWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWarning, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CWarning ��Ϣ�������


BOOL CWarning::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	int sc_width = GetSystemMetrics(SM_CXSCREEN);
	int sc_height = GetSystemMetrics(SM_CYSCREEN);

	int m_width = 500;
	int m_height = 100;
	int main_left = sc_width / 2 - m_width / 2;
	int main_top = sc_height / 2 - m_height / 2;
	int main_right = main_left + m_width;
	int main_bottom = main_top + m_height;

	m_main_crect = CRect(main_left, main_top, main_right, main_bottom);
	SetWindowPos(&wndTopMost, m_main_crect.left, m_main_crect.top, m_main_crect.Width(), m_main_crect.Height(), SWP_SHOWWINDOW);
	SetActiveWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CWarning::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	Graphics graphics(dc);

	Pen pen(Color(255, 185, 186, 187), 2);
	graphics.DrawRectangle(&pen, Rect(0, 0, m_main_crect.Width(), m_main_crect.Height()));

	FontFamily font_family(L"���Ĳ���");
	Gdiplus::Font warning_font(&font_family, 32, FontStyleRegular, UnitPoint);
	SolidBrush text_brush(Color(250, 255, 186, 187));
	CString strWarning = _T("���������ε���Դ��");
	graphics.DrawString(strWarning, strWarning.GetLength(), &warning_font, PointF(40, 30), &text_brush);

}


BOOL CWarning::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SetBackgroundColor(RGB(51, 51, 51));

	//return false;
	return CDialogEx::OnEraseBkgnd(pDC);
}
