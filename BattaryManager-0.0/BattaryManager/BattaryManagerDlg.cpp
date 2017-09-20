
// BattaryManagerDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CBattaryManagerDlg �Ի���



CBattaryManagerDlg::CBattaryManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBattaryManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	GdiplusStartupInput input;
	GdiplusStartup(&m_token, &input, NULL);

	m_move_flag = false; //��ʼ���ƶ����ڱ���
	m_battary_charging_flag = false; //��ʼ������ʶ
	charge_block = 1; //���������ʼ��Ϊ1
	m_battary_full_flag = false; //��ʼ����Դ������ʶ
	m_battary_full_warning_falg = false; //�����羯ʾ��ʶ

	m_battary_percent = 0;//��ʼ���������
	m_energy_blocks = 0; //������������ʼ��

	m_warning = NULL; //��ʼ������ָ��
	m_warning_dlg_create = false; //��ʼ�����ڴ�����ʶ
	m_warning_dlg_show = false; //��ʼ��������ʾ��ʶ

	//RunBySystemOn(); //���ע���������
}

CBattaryManagerDlg::~CBattaryManagerDlg()
{
	if (m_warning != NULL)  //���մ���ָ��
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


// CBattaryManagerDlg ��Ϣ�������

BOOL CBattaryManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	SetTimer(CLOCK_TIMER, 1000, NULL);

	int sc_width = GetSystemMetrics(SM_CXSCREEN);
	int sc_height = GetSystemMetrics(SM_CYSCREEN);
	CRect main_crect;
	GetClientRect(&main_crect);
	SetWindowPos(NULL, sc_width - main_crect.Width(), 0, main_crect.Width(), main_crect.Height(), SWP_SHOWWINDOW);
	
	//::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	//::SetLayeredWindowAttributes(m_hWnd, 0, 120, LWA_ALPHA); // 120��͸���ȣ���Χ��0��255 
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(RGB(0, 0, 0), 255, LWA_COLORKEY);
	
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW); //���ó���ģʽ��������ͼ��
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBattaryManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	//��ȡ��Դ��Ϣ��
	//SYSTEM_POWER_STATUS lp_pwr_state;
	//GetSystemPowerStatus(&lp_pwr_state);
	//
	//cout << "ACLineStatus: " << (int)lp_pwr_state.ACLineStatus << endl;
	//cout << "BatteryFlag: " << (int)lp_pwr_state.BatteryFlag << endl;
	//cout << "BatteryFullLifeTime: " << (int)lp_pwr_state.BatteryFullLifeTime << endl;
	//cout << "BatteryLifePercent: " << (int)lp_pwr_state.BatteryLifePercent << endl;
	//cout << "BatteryLifeTime: " << (int)lp_pwr_state.BatteryLifeTime << endl;
	//cout << "Reserved1: " << (int)lp_pwr_state.Reserved1 << endl;

	//��ȡϵͳʱ�䣺
	//m_system_time = GetCurrentTime();
	//CString str;
	//str = m_system_time.Format("%Y��%m��%d�� %X");
	//USES_CONVERSION;
	//char* system_time = T2A(str);
	//cout << system_time << endl;

	//��ȡ����ʱ�䣺
	SYSTEMTIME st;
	CString strDate, strTime;
	GetLocalTime(&st);
	strDate.Format(L"%4d-%2d-%2d", st.wYear, st.wMonth, st.wDay);
	strTime.Format(L"%2d:%2d:%2d", st.wHour, st.wMinute, st.wSecond);
	USES_CONVERSION;
	char* str_data = T2A(strDate);
	char* str_time = T2A(strTime);
	//cout << str_data << "\t\t" << str_time << endl;
	
	//����˫���棺
	CDC* pDC = GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CRect main_crect;
	GetClientRect(&main_crect);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, main_crect.Width(), main_crect.Height());
	CBitmap* old_bmp = dcMem.SelectObject(&bmp);
	//����GDI+����
	Graphics graphics(dcMem);
	SolidBrush back_brush(Color(0, 0, 0, 0));
	//���Ʊ�����
	//Rect main_rect(main_crect.left, main_crect.top, main_crect.Width(), main_crect.Height());
	//graphics.FillRectangle(&back_brush, main_rect);
	//CBrush back_brush = CBrush(RGB(51, 51, 51));
	//dcMem.FillRect(&main_rect, &back_brush);
	//����Բ����
	
	//Brush* p_g_brush = &SolidBrush(Color(255, 255, 255, 128));
	Pen g_pen(Color(128, 255, 255, 0), 5);
	//Rect eclips_are(10, 10 , 50, 50);
	//graphics.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	//graphics.DrawArc(&g_pen, eclips_are, 0, m_angle);
	//graphics.DrawLine(&g_pen, Point(0, 0), Point(50, 50));
	//�������壺
	FontFamily font_family(L"Bauhaus 93");
	Gdiplus::Font clock_font(&font_family, 32, FontStyleRegular, UnitPoint);
	//SolidBrush text_brush(Color(250, 101, 194, 0));

	LinearGradientBrush lgBrush2(PointF(0, 0), PointF(40, 40), Color(255, 51, 51, 51), Color(255, 255, 255, 0));//б�򽥱�  
	//��ͨƽ��  
	lgBrush2.SetWrapMode(WrapModeTile);//Ĭ�� 
	graphics.DrawString(strDate, strDate.GetLength(), &clock_font, PointF(20, 20), &lgBrush2);
	graphics.DrawString(strTime, strTime.GetLength(), &clock_font, PointF(20, 60), &lgBrush2);

	//�������ߣ�
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
	
	for (int i = 0; i < 6; i++) //���Ƶ���
	{
		graphics.DrawRectangle(&battary_bound_pen, Rect(35 + 30 * i, 130, 20, 50));
	}

	Rect energy_bar(35, 130, 20, 50);

	if (m_battary_charging_flag == true)//���Ƴ��״̬
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
	for (int i = 0; i < m_energy_blocks; i++) //������������������
	{
		graphics.FillRectangle(&SolidBrush(Color(255, 0, 205, 0)), Rect(35 + i * 30, 130, 20, 50));
	}

	for (int i = m_energy_blocks; i < block; i++) //���ƴ���������������
	{
		graphics.FillRectangle(&SolidBrush(Color(255, 0, 205, 0)), Rect(35 + i * 30, 130, 20, 50));
	}
}

void CBattaryManagerDlg::DrawBattaryStatues(Graphics& graphics, Rect& rect, int full_percent)
{
	SolidBrush solid_brush(Color(255, 255, 255));

	for (int i = 0; i < m_energy_blocks; i++)
	{
		if (m_energy_blocks == 1) //��������������ͬ���Ʋ�ͬ����ɫ,���һ��ɫ����ݱ�����ɫ
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
				else //m_energy_blocks ����3
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
				else //m_energy_blocks ����6
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
{ //��ȡ��������
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBattaryManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBattaryManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == CLOCK_TIMER)
	{
		//�жϴ���״̬
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
		
		//��ȡ���״̬
		GetSystemPowerStatus(&lp_pwr_state);

		m_battary_percent = (int)lp_pwr_state.BatteryLifePercent; //��ȡ�������
		GetBlocksNum(m_battary_percent); //��ȡ����������

		//��Դ���״̬�����ϵ�Դ�߱�ʶΪ1
		bool charging_flag = (bool)lp_pwr_state.ACLineStatus;

		if (m_battary_charging_flag == false && charging_flag == true)
		{ //����ж�ʱ��
			m_battary_charging_flag = true;
			SetTimer(BATTARY_CHARGING, 1000, NULL);
		}

		if ((int)lp_pwr_state.BatteryLifePercent == 100)
		{ //��س���״̬
			m_battary_full_flag = true;
		}
		else
		{ //���δ��״̬
			m_battary_full_flag = false;
		}

		if (m_battary_full_flag == true && charging_flag == true && m_battary_full_warning_falg == false) 
		{ //���������û�ε���Դ�� ��δ���ö�ʱ��ʱ ���ö�ʱ��,
			cout << "in" << endl;
			SetTimer(BATTARY_FULL, 1000, NULL);
			m_battary_full_warning_falg = true; //�����˾�ʾ��ʱ����
		}

		if (m_battary_full_warning_falg == true && charging_flag == false)
		{ //�������þ�ʾ��ʱ��ʱ����ʱ�ε��˵�Դ�ߣ�Ӧ��ȡ����ʾ��ʱ��
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
		{ //������ȡ����ʱ��,ȡ�������ʾ
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
	{ //�����������
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	return false;
	//return CDialog::OnEraseBkgnd(pDC);
}


BOOL CBattaryManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	

	return CDialog::PreTranslateMessage(pMsg);
}


void CBattaryManagerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_move_flag = true;
	
	m_start_pos = point; //�����ƶ�ǰ��λ��

	CDialog::OnLButtonDown(nFlags, point);
}


void CBattaryManagerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_move_flag = false;

	CDialog::OnLButtonUp(nFlags, point);
}


void CBattaryManagerDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}


void CBattaryManagerDlg::OnCancel()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialog::OnCancel();
}

void CBattaryManagerDlg::RunBySystemOn()
{
	HKEY hKey;
	//�ҵ�ϵͳ��������   
	LPCTSTR lpRun = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	//��������Key   
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		TCHAR pFileName[MAX_PATH] = L"C:\\AppSoft\\battarymanager\\BattaryManager.exe";
		//�õ����������ȫ·��   
		DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
		//���һ����Key,������ֵ // �����"BattaryManager"��Ӧ�ó������֣����Ӻ�׺.exe��  
		lRet = RegSetValueEx(hKey, _T("BattaryManager"), 0, REG_SZ, (BYTE *)pFileName, dwRet);
		//�ر�ע���   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(L"ϵͳ��������,������ɿ�����������");
		}
		else
		{
			AfxMessageBox(L"�򿪿��������ɹ�");
		}
	}
}

void CBattaryManagerDlg::OutRunOnSystem()
{
	HKEY hKey;
	//�ҵ�ϵͳ��������   
	LPCTSTR lpRun = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	//��������Key   
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		TCHAR pFileName[MAX_PATH] = L"C:\\AppSoft\\battarymanager\\BattaryManager.exe";
		//�õ����������ȫ·��   
		DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
		//���һ����Key,������ֵ // �����"test"��Ӧ�ó������֣����Ӻ�׺.exe��  
		lRet = RegDeleteValue(hKey, _T("BattaryManager"));
		//�ر�ע���   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(L"ϵͳ��������,�������ȡ��������������");
		}
		else{
			AfxMessageBox(L"�رտ��������ɹ�");
		}
	}
}