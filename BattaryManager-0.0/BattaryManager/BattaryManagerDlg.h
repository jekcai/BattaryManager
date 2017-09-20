
// BattaryManagerDlg.h : ͷ�ļ�
//

#pragma once

#include <gdiplus.h>
using namespace Gdiplus;

#include "Warning.h"

// CBattaryManagerDlg �Ի���
class CBattaryManagerDlg : public CDialog
{
// ����
public:
	CBattaryManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CBattaryManagerDlg(); //��������

private:
	void DrawBattary(Graphics& graphics);
	GraphicsPath DrawRoundRectanglePath(Rect& rect, int w_round, int h_round);
	void DrawRoundRectangle(Graphics& graphics, Pen& bound_pen, Rect& rect, int w_round, int h_round);
	void DrawBattaryCharging(Graphics& graphics, Rect& rect, int block);
	void DrawBattaryStatues(Graphics& graphics,  Rect& rect, int full_percent);
	void GetBlocksNum(int full_percent);
	void RunBySystemOn();
	void OutRunOnSystem();

// �Ի�������
	enum { IDD = IDD_BATTARYMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	SYSTEM_POWER_STATUS lp_pwr_state; //�����Ϣ
	bool m_battary_charging_flag; //����ʶ��
	int charge_block; //�������

	bool m_battary_full_flag; //��س�����ʶ
	bool m_battary_full_warning_falg; //��س�����ʾ��ʶ
	int m_battary_percent; //���������
	int m_energy_blocks; //������������

	bool m_move_flag;
	CPoint m_start_pos;

	CWarning* m_warning; //��������ʾ�Ի���
	bool m_warning_dlg_create; //��ʾ�Ի��򴴽���ʶ
	bool m_warning_dlg_show; //��ʾ�Ի�����ʾ��ʶ



// ʵ��
protected:
	HICON m_hIcon;
	ULONG_PTR m_token;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnOK();
	virtual void OnCancel();
};
