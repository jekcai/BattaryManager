
// BattaryManagerDlg.h : 头文件
//

#pragma once

#include <gdiplus.h>
using namespace Gdiplus;

#include "Warning.h"

// CBattaryManagerDlg 对话框
class CBattaryManagerDlg : public CDialog
{
// 构造
public:
	CBattaryManagerDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CBattaryManagerDlg(); //析构函数

private:
	void DrawBattary(Graphics& graphics);
	GraphicsPath DrawRoundRectanglePath(Rect& rect, int w_round, int h_round);
	void DrawRoundRectangle(Graphics& graphics, Pen& bound_pen, Rect& rect, int w_round, int h_round);
	void DrawBattaryCharging(Graphics& graphics, Rect& rect, int block);
	void DrawBattaryStatues(Graphics& graphics,  Rect& rect, int full_percent);
	void GetBlocksNum(int full_percent);
	void RunBySystemOn();
	void OutRunOnSystem();

// 对话框数据
	enum { IDD = IDD_BATTARYMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	SYSTEM_POWER_STATUS lp_pwr_state; //电池信息
	bool m_battary_charging_flag; //充电标识；
	int charge_block; //充电块个数

	bool m_battary_full_flag; //电池充满标识
	bool m_battary_full_warning_falg; //电池充满警示标识
	int m_battary_percent; //电池容量；
	int m_energy_blocks; //能量条个数；

	bool m_move_flag;
	CPoint m_start_pos;

	CWarning* m_warning; //充满电提示对话框
	bool m_warning_dlg_create; //提示对话框创建标识
	bool m_warning_dlg_show; //提示对话框显示标识



// 实现
protected:
	HICON m_hIcon;
	ULONG_PTR m_token;

	// 生成的消息映射函数
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
