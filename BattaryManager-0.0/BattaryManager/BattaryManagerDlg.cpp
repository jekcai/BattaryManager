
// BattaryManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BattaryManager.h"
#include "BattaryManagerDlg.h"
#include "afxdialogex.h"

#include <iostream>
using namespace std;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CLOCK_TIMER 1
#define BATTARY_CHARGING 2
#define BATTARY_FULL 3

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBattaryManagerDlg 对话框



CBattaryManagerDlg::CBattaryManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBattaryManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	GdiplusStartupInput input;
	GdiplusStartup(&m_token, &input, NULL);

	m_move_flag = false; //初始化移动窗口变量
	m_battary_charging_flag = false; //初始化充电标识
	charge_block = 1; //充电块个数初始化为1
	m_battary_full_flag = false; //初始化电源充满标识
	m_battary_full_warning_falg = false; //充满电警示标识

	m_battary_percent = 0;//初始化电池容量
	m_energy_blocks = 0; //能量条个数初始化

	m_warning = NULL; //初始化窗口指针
	m_warning_dlg_create = false; //初始化窗口创建标识
	m_warning_dlg_show = false; //初始化窗口显示标识

	//RunBySystemOn(); //添加注册表启动项
}

CBattaryManagerDlg::~CBattaryManagerDlg()
{
	if (m_warning != NULL)  //回收窗口指针
		delete m_warning;
	m_warning = NULL;

	GdiplusShutdown(m_token);
}

void CBattaryManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBattaryManagerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CBattaryManagerDlg 消息处理程序

BOOL CBattaryManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// TODO:  在此添加额外的初始化代码
	SetTimer(CLOCK_TIMER, 1000, NULL);

	int sc_width = GetSystemMetrics(SM_CXSCREEN);
	int sc_height = GetSystemMetrics(SM_CYSCREEN);
	CRect main_crect;
	GetClientRect(&main_crect);
	SetWindowPos(NULL, sc_width - main_crect.Width(), 0, main_crect.Width(), main_crect.Height(), SWP_SHOWWINDOW);
	
	//::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	//::SetLayeredWindowAttributes(m_hWnd, 0, 120, LWA_ALPHA); // 120是透明度，范围是0～255 
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(RGB(0, 0, 0), 255, LWA_COLORKEY);
	
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW); //设置程序模式无任务栏图标
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBattaryManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBattaryManagerDlg::OnPaint()
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
		CDialog::OnPaint();
	}
	//获取电源信息：
	//SYSTEM_POWER_STATUS lp_pwr_state;
	//GetSystemPowerStatus(&lp_pwr_state);
	//
	//cout << "ACLineStatus: " << (int)lp_pwr_state.ACLineStatus << endl;
	//cout << "BatteryFlag: " << (int)lp_pwr_state.BatteryFlag << endl;
	//cout << "BatteryFullLifeTime: " << (int)lp_pwr_state.BatteryFullLifeTime << endl;
	//cout << "BatteryLifePercent: " << (int)lp_pwr_state.BatteryLifePercent << endl;
	//cout << "BatteryLifeTime: " << (int)lp_pwr_state.BatteryLifeTime << endl;
	//cout << "Reserved1: " << (int)lp_pwr_state.Reserved1 << endl;

	//获取系统时间：
	//m_system_time = GetCurrentTime();
	//CString str;
	//str = m_system_time.Format("%Y年%m月%d日 %X");
	//USES_CONVERSION;
	//char* system_time = T2A(str);
	//cout << system_time << endl;

	//获取当地时间：
	SYSTEMTIME st;
	CString strDate, strTime;
	GetLocalTime(&st);
	strDate.Format(L"%4d-%2d-%2d", st.wYear, st.wMonth, st.wDay);
	strTime.Format(L"%2d:%2d:%2d", st.wHour, st.wMinute, st.wSecond);
	USES_CONVERSION;
	char* str_data = T2A(strDate);
	char* str_time = T2A(strTime);
	//cout << str_data << "\t\t" << str_time << endl;
	
	//创建双缓存：
	CDC* pDC = GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CRect main_crect;
	GetClientRect(&main_crect);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, main_crect.Width(), main_crect.Height());
	CBitmap* old_bmp = dcMem.SelectObject(&bmp);
	//创建GDI+对象：
	Graphics graphics(dcMem);
	SolidBrush back_brush(Color(0, 0, 0, 0));
	//绘制背景：
	//Rect main_rect(main_crect.left, main_crect.top, main_crect.Width(), main_crect.Height());
	//graphics.FillRectangle(&back_brush, main_rect);
	//CBrush back_brush = CBrush(RGB(51, 51, 51));
	//dcMem.FillRect(&main_rect, &back_brush);
	//绘制圆弧：
	
	//Brush* p_g_brush = &SolidBrush(Color(255, 255, 255, 128));
	Pen g_pen(Color(128, 255, 255, 0), 5);
	//Rect eclips_are(10, 10 , 50, 50);
	//graphics.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	//graphics.DrawArc(&g_pen, eclips_are, 0, m_angle);
	//graphics.DrawLine(&g_pen, Point(0, 0), Point(50, 50));
	//绘制字体：
	FontFamily font_family(L"Bauhaus 93");
	Gdiplus::Font clock_font(&font_family, 32, FontStyleRegular, UnitPoint);
	//SolidBrush text_brush(Color(250, 101, 194, 0));

	LinearGradientBrush lgBrush2(PointF(0, 0), PointF(40, 40), Color(255, 51, 51, 51), Color(255, 255, 255, 0));//斜向渐变  
	//普通平铺  
	lgBrush2.SetWrapMode(WrapModeTile);//默认 
	graphics.DrawString(strDate, strDate.GetLength(), &clock_font, PointF(20, 20), &lgBrush2);
	graphics.DrawString(strTime, strTime.GetLength(), &clock_font, PointF(20, 60), &lgBrush2);

	//绘制曲线：
	//graphics.DrawBezier(&g_pen, Point(20, 100), Point(m_angle, 100), Point(60, 120), Point(80, m_angle));

	DrawBattary(graphics);

	//pDC->SetBkColor(TRANSPARENT);
	pDC->BitBlt(0, 0, main_crect.Width(), main_crect.Height(), &dcMem, 0, 0, SRCCOPY);
}

void CBattaryManagerDlg::DrawBattary(Graphics& graphics)
{
	Pen battary_bound_pen(Color(200, 255, 255, 255), 2);
	Rect battary_rect(20, 120, 200, 70);
	DrawRoundRectangle(graphics, battary_bound_pen, battary_rect, 20, 20);
	
	for (int i = 0; i < 6; i++) //绘制电条
	{
		graphics.DrawRectangle(&battary_bound_pen, Rect(35 + 30 * i, 130, 20, 50));
	}

	Rect energy_bar(35, 130, 20, 50);

	if (m_battary_charging_flag == true)//绘制充电状态
	{
		DrawBattaryCharging(graphics, energy_bar, charge_block);
	}
	else
	{
		DrawBattaryStatues(graphics, energy_bar, m_battary_percent);
	}
}

void CBattaryManagerDlg::DrawBattaryCharging(Graphics& graphics,  Rect& rect, int block)
{
	for (int i = 0; i < m_energy_blocks; i++) //绘制已有区间能量条
	{
		graphics.FillRectangle(&SolidBrush(Color(255, 0, 205, 0)), Rect(35 + i * 30, 130, 20, 50));
	}

	for (int i = m_energy_blocks; i < block; i++) //绘制待充满区间能量条
	{
		graphics.FillRectangle(&SolidBrush(Color(255, 0, 205, 0)), Rect(35 + i * 30, 130, 20, 50));
	}
}

void CBattaryManagerDlg::DrawBattaryStatues(Graphics& graphics, Rect& rect, int full_percent)
{
	SolidBrush solid_brush(Color(255, 255, 255));

	for (int i = 0; i < m_energy_blocks; i++)
	{
		if (m_energy_blocks == 1) //根据能量条数不同绘制不同的颜色,最后一个色块根据比例绘色
		{
			double contain_rate = m_battary_percent / 17.0;
			solid_brush.SetColor(Color(255 * contain_rate, 0, 0));
		}
		else if (m_energy_blocks > 1 && m_energy_blocks <= 3)
		{
			double contain_rate = 1.0;
			if (i == m_energy_blocks - 1)
			{
				if (m_energy_blocks == 2)
				{
					contain_rate = (m_battary_percent - 17) / 17.0;
				}
				else //m_energy_blocks 等于3
				{
					contain_rate = (m_battary_percent - 34) / 17.0;
				}
			}
			solid_brush.SetColor(Color(255 * contain_rate, 255 * contain_rate, 0));
		}
		else
		{
			double contain_rate = 1.0;
			if (i == m_energy_blocks - 1)
			{
				if (m_energy_blocks == 4)
				{
					contain_rate = (m_battary_percent - 51) / 17.0;
				}
				else if (m_energy_blocks == 5)
				{
					contain_rate = (m_battary_percent - 68) / 17.0;
				}
				else //m_energy_blocks 等于6
				{
					contain_rate = (m_battary_percent - 85) / 17.0;
				}
			}
			solid_brush.SetColor(Color(0, 205 * contain_rate, 0));
		}
		graphics.FillRectangle(&solid_brush, Rect(35 + i * 30, 130, 20, 50));
	}
}

void CBattaryManagerDlg::GetBlocksNum(int full_percent)
{ //获取能量条数
	if (full_percent < 17)
	{
		m_energy_blocks = 1;
	}
	else if (full_percent < 51)
	{
		if (full_percent >= 17 && full_percent < 34)
		{
			m_energy_blocks = 2;
		}
		else
		{
			m_energy_blocks = 3;
		}
	}
	else
	{
		if (full_percent >= 51 && full_percent < 68)
		{
			m_energy_blocks = 4;
		}
		else if (full_percent >= 68 && full_percent < 85)
		{
			m_energy_blocks = 5;
		}
		else
		{
			m_energy_blocks = 6;
		}
	}
}

void CBattaryManagerDlg::DrawRoundRectangle(Graphics& graphics, Pen& bound_pen, Rect& rect, int w_round, int h_round)
{
	graphics.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	graphics.DrawArc(&bound_pen, Rect(rect.GetLeft(), rect.GetTop(), w_round, h_round), 180, 90);
	graphics.DrawLine(&bound_pen, Point(rect.GetLeft() + (w_round / 2), rect.GetTop()), Point(rect.GetRight() - (w_round / 2), rect.GetTop()));
	graphics.DrawLine(&bound_pen, Point(rect.GetLeft(), rect.GetTop() + (h_round / 2)), Point(rect.GetLeft(), rect.GetBottom() - (h_round / 2)));
	graphics.DrawArc(&bound_pen, Rect(rect.GetLeft(), rect.GetBottom() - h_round, w_round, h_round), 90, 90);
	graphics.DrawLine(&bound_pen, Point(rect.GetLeft() + (w_round / 2), rect.GetBottom()), Point(rect.GetRight() - (w_round / 2), rect.GetBottom()));
	graphics.DrawArc(&bound_pen, Rect(rect.GetRight() - w_round, rect.GetBottom() - h_round, w_round, h_round), 0, 90);
	graphics.DrawLine(&bound_pen, Point(rect.GetRight(), rect.GetBottom() - (h_round / 2)), Point(rect.GetRight(), rect.GetTop() + (h_round / 2)));
	graphics.DrawArc(&bound_pen, Rect(rect.GetRight() - w_round, rect.GetTop(), w_round, h_round), 270, 90);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBattaryManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBattaryManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == CLOCK_TIMER)
	{
		//判断窗口状态
		/*CRect wnd_pos;
		GetWindowRect(&wnd_pos);
		cout << "left: " << wnd_pos.left << "\t\t top: " << wnd_pos.top << endl;

		if (IsWindowVisible()== false)
		{
			cout << "\t\t unvisible" << endl;
		}
		if (IsIconic() == true)
		{
			cout << "\t\t is iconic" << endl;
		}*/
		
		//获取电池状态
		GetSystemPowerStatus(&lp_pwr_state);

		m_battary_percent = (int)lp_pwr_state.BatteryLifePercent; //获取电池容量
		GetBlocksNum(m_battary_percent); //获取能量条个数

		//电源充电状态，插上电源线标识为1
		bool charging_flag = (bool)lp_pwr_state.ACLineStatus;

		if (m_battary_charging_flag == false && charging_flag == true)
		{ //充电中定时器
			m_battary_charging_flag = true;
			SetTimer(BATTARY_CHARGING, 1000, NULL);
		}

		if ((int)lp_pwr_state.BatteryLifePercent == 100)
		{ //电池充满状态
			m_battary_full_flag = true;
		}
		else
		{ //电池未满状态
			m_battary_full_flag = false;
		}

		if (m_battary_full_flag == true && charging_flag == true && m_battary_full_warning_falg == false) 
		{ //当电充满且没拔掉电源线 及未设置定时器时 设置定时器,
			cout << "in" << endl;
			SetTimer(BATTARY_FULL, 1000, NULL);
			m_battary_full_warning_falg = true; //设置了警示定时器；
		}

		if (m_battary_full_warning_falg == true && charging_flag == false)
		{ //当已设置警示定时器时，此时拔掉了电源线，应该取消警示定时器
			if (m_warning_dlg_create == true)
			{
				m_warning->DestroyWindow();
				m_warning = NULL;
				m_warning_dlg_create = false;
			}
			cout << "out" << endl;
			KillTimer(BATTARY_FULL);
			m_battary_full_warning_falg = false;
		}

		if (m_battary_full_flag == true || (charging_flag != true && m_battary_charging_flag == true))
		{ //充电结束取消定时器,取消充电显示
			KillTimer(BATTARY_CHARGING);
			m_battary_charging_flag = false;
		}

		Invalidate(true);
	}

	if (nIDEvent == BATTARY_CHARGING)
	{
		if (charge_block > 5)
			charge_block = 1;
		else
			charge_block++;
	}

	if (nIDEvent == BATTARY_FULL)
	{ //电充满后提醒
		cout << "im in full" << endl;
		if (m_warning_dlg_create == false)
		{
			cout << "create" << endl;
			m_warning = new CWarning();
			m_warning->Create(IDD_WARNING_DIALOG, this);
			m_warning_dlg_create = true;
		}
		
		if (m_warning_dlg_create == true && m_warning_dlg_show == false)
		{
			cout << "show" << endl;
			m_warning->ShowWindow(SW_SHOW);
			m_warning_dlg_show = true;
		}
		else
		{
			cout << "hind" << endl;
			m_warning->ShowWindow(SW_HIDE);
			m_warning_dlg_show = false;
		}

	}

	CDialog::OnTimer(nIDEvent);
}


BOOL CBattaryManagerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	
	return false;
	//return CDialog::OnEraseBkgnd(pDC);
}


BOOL CBattaryManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	

	return CDialog::PreTranslateMessage(pMsg);
}


void CBattaryManagerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_move_flag == true)
	{
		CRect main_crect;
		GetWindowRect(&main_crect);
		SetWindowPos(NULL, main_crect.left + point.x - m_start_pos.x, main_crect.top + point.y - m_start_pos.y, main_crect.Width(), main_crect.Height(), SWP_NOREDRAW);
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CBattaryManagerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_move_flag = true;
	
	m_start_pos = point; //窗口移动前的位置

	CDialog::OnLButtonDown(nFlags, point);
}


void CBattaryManagerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_move_flag = false;

	CDialog::OnLButtonUp(nFlags, point);
}


void CBattaryManagerDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}


void CBattaryManagerDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}

void CBattaryManagerDlg::RunBySystemOn()
{
	HKEY hKey;
	//找到系统的启动项   
	LPCTSTR lpRun = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	//打开启动项Key   
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		TCHAR pFileName[MAX_PATH] = L"C:\\AppSoft\\battarymanager\\BattaryManager.exe";
		//得到程序自身的全路径   
		DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
		//添加一个子Key,并设置值 // 下面的"BattaryManager"是应用程序名字（不加后缀.exe）  
		lRet = RegSetValueEx(hKey, _T("BattaryManager"), 0, REG_SZ, (BYTE *)pFileName, dwRet);
		//关闭注册表   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(L"系统参数错误,不能完成开机启动设置");
		}
		else
		{
			AfxMessageBox(L"打开开机启动成功");
		}
	}
}

void CBattaryManagerDlg::OutRunOnSystem()
{
	HKEY hKey;
	//找到系统的启动项   
	LPCTSTR lpRun = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	//打开启动项Key   
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		TCHAR pFileName[MAX_PATH] = L"C:\\AppSoft\\battarymanager\\BattaryManager.exe";
		//得到程序自身的全路径   
		DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
		//添加一个子Key,并设置值 // 下面的"test"是应用程序名字（不加后缀.exe）  
		lRet = RegDeleteValue(hKey, _T("BattaryManager"));
		//关闭注册表   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(L"系统参数错误,不能完成取消开机启动设置");
		}
		else{
			AfxMessageBox(L"关闭开机启动成功");
		}
	}
}