// Warning.cpp : 实现文件
//

#include "stdafx.h"
#include "BattaryManager.h"
#include "Warning.h"
#include "afxdialogex.h"


// CWarning 对话框

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


// CWarning 消息处理程序


BOOL CWarning::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	// 异常:  OCX 属性页应返回 FALSE
}


void CWarning::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	Graphics graphics(dc);

	Pen pen(Color(255, 185, 186, 187), 2);
	graphics.DrawRectangle(&pen, Rect(0, 0, m_main_crect.Width(), m_main_crect.Height()));

	FontFamily font_family(L"华文彩云");
	Gdiplus::Font warning_font(&font_family, 32, FontStyleRegular, UnitPoint);
	SolidBrush text_brush(Color(250, 255, 186, 187));
	CString strWarning = _T("电充满，请拔掉电源！");
	graphics.DrawString(strWarning, strWarning.GetLength(), &warning_font, PointF(40, 30), &text_brush);

}


BOOL CWarning::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	SetBackgroundColor(RGB(51, 51, 51));

	//return false;
	return CDialogEx::OnEraseBkgnd(pDC);
}
