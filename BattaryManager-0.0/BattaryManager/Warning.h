#pragma once
#include <gdiplus.h>
using namespace Gdiplus;


// CWarning 对话框

class CWarning : public CDialogEx
{
	DECLARE_DYNAMIC(CWarning)

public:
	CWarning(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWarning();

private:
	CRect m_main_crect; //设置主窗口
// 对话框数据
	enum { IDD = IDD_WARNING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
