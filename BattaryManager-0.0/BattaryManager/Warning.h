#pragma once
#include <gdiplus.h>
using namespace Gdiplus;


// CWarning �Ի���

class CWarning : public CDialogEx
{
	DECLARE_DYNAMIC(CWarning)

public:
	CWarning(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWarning();

private:
	CRect m_main_crect; //����������
// �Ի�������
	enum { IDD = IDD_WARNING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
