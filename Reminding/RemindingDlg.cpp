
// RemindingDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Reminding.h"
#include "RemindingDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment(lib, "winmm.lib")

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRemindingDlg 对话框



CRemindingDlg::CRemindingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REMINDING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemindingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_MAIN, m_wndList);
    DDX_Control(pDX, IDC_COMBO1, m_wndComb);
    DDX_Control(pDX, IDC_EDIT_REMARK, m_remark);
    DDX_Control(pDX, IDC_DATETIMEPICKER3, m_time);
    DDX_Control(pDX, IDC_COMBO3, m_weekDay);
}

BEGIN_MESSAGE_MAP(CRemindingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_CBN_SELCHANGE(IDC_COMBO1, &CRemindingDlg::OnCbnSelchangeCombo1)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CRemindingDlg::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_DEL, &CRemindingDlg::OnBnClickedButtonDel)
    ON_BN_CLICKED(IDC_BUTTON_EXIT, &CRemindingDlg::OnBnClickedButtonExit)
    ON_CBN_SELCHANGE(IDC_COMBO3, &CRemindingDlg::OnCbnSelchangeCombo3)
    ON_WM_TIMER()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_NOTIFY_MESSAGE, NotifyIconCallBack)
    ON_MESSAGE(WM_NOTIFY_MESGRESTORE, NotifyIconMesgRestore)
END_MESSAGE_MAP()


// CRemindingDlg 消息处理程序

BOOL CRemindingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码
    m_wndComb.SetCurSel(0);

    m_time.ModifyStyle(DTS_TIMEFORMAT, DTS_TIMEFORMAT);
    m_time.SetFormat(_T("mm:ss"));

    ChangeList(0);

    m_wndList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_ALIGNLEFT 
        | LVS_AUTOARRANGE | LVS_EX_FULLROWSELECT 
        | LVS_SHOWSELALWAYS);

    m_wndList.InsertColumn(0, _T("类型"), LVCFMT_LEFT, 80);
    m_wndList.InsertColumn(1, _T("时间"), LVCFMT_LEFT, 140);
    m_wndList.InsertColumn(2, _T("备注"), LVCFMT_LEFT, 100);
    m_wndList.InsertColumn(3, _T("NextTime"), LVCFMT_LEFT, 140);

    SetWindowText(_T("闹钟"));

    SetTimer(emtimer_1s, 1000, NULL);
    // status bar 
    if (!m_StatusBar.Create(this))
    {
        return -1;
    }
    const int StatusBar = 2;
    UINT array[StatusBar];
    for (int i = 0; i < StatusBar; i++)
    {
        array[i] = WM_USER + 1001 + i;
    }
    m_StatusBar.SetIndicators(array, sizeof(array) / sizeof(UINT)); //添加面板
    m_StatusBar.SetPaneInfo(0, array[0], 0, 50); //设置面板宽度
    m_StatusBar.SetPaneInfo(1, array[1], 0, 999); //设置面板宽度
    m_StatusBar.SetPaneText(0, _T("时间"));
    CString szTime;
    szTime = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
    m_StatusBar.SetPaneText(1, szTime);

    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);//显示状态栏

    InitNotifyIcon();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRemindingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRemindingDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRemindingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRemindingDlg::OnCbnSelchangeCombo1()
{
    // TODO: 在此添加控件通知处理程序代码
    CString szType;
    m_wndComb.GetWindowText(szType);
    if (_T("每小时") == szType)
    {
        ChangeList(0);
        m_time.SetFormat(_T("mm:ss"));
    }
    else if (_T("每天") == szType)
    {
        ChangeList(0);
        m_time.SetFormat(_T("HH:mm:ss"));
    }
    else if (_T("每周") == szType)
    {
        //m_data.SetFormat(_T("dddd"));
        //m_data.SetMonthCalStyle(MCS_WEEKNUMBERS);
        ChangeList(1);
        m_time.SetFormat(_T("HH:mm:ss"));
    }
    else if (_T("每月") == szType)
    {
        ChangeList(2);
        m_time.SetFormat(_T("HH:mm:ss"));
    }
}


void CRemindingDlg::OnBnClickedButtonAdd()
{
    // TODO: 在此添加控件通知处理程序代码
    CString szType, szTime, szRemark, szDay, szDescription;
    m_wndComb.GetWindowText(szType);
    m_time.GetWindowText(szTime);
    m_remark.GetWindowText(szRemark);
    m_weekDay.GetWindowText(szDay);

    int nType = 0;
    CTime t;
    SYSTEMTIME stLocal;
    memset(&stLocal, 0, sizeof(stLocal));

    if (_T("每小时") == szType)
    {
        m_time.GetTime(t);
        int m = t.GetMinute();
        int s = t.GetSecond();
        stLocal.wMinute = m;
        stLocal.wSecond = s;
        szDescription.Format(_T("%d分%d秒"), m, s);
        nType = CTimer::em_hour;
    }
    else if (_T("每天") == szType)
    {
        m_time.GetTime(t);
        int h = t.GetHour();
        int m = t.GetMinute();
        int s = t.GetSecond();
        stLocal.wHour = h;
        stLocal.wMinute = m;
        stLocal.wSecond = s;
        szDescription.Format(_T("%d时%d分%d秒"), h, m, s);
        nType = CTimer::em_day;
    }
    else if (_T("每周") == szType)
    {
        m_time.GetTime(t);
        int h = t.GetHour();
        int m = t.GetMinute();
        int s = t.GetSecond();
        stLocal.wHour = h;
        stLocal.wMinute = m;
        stLocal.wSecond = s;
        if (szDay == _T("星期一"))
        {
            stLocal.wDayOfWeek = 1;
        }
        else if (szDay == _T("星期二"))
        {
            stLocal.wDayOfWeek = 2;
        }
        else if (szDay == _T("星期三"))
        {
            stLocal.wDayOfWeek = 3;
        }
        else if (szDay == _T("星期四"))
        {
            stLocal.wDayOfWeek = 4;
        }
        else if (szDay == _T("星期五"))
        {
            stLocal.wDayOfWeek = 5;
        }
        else if (szDay == _T("星期六"))
        {
            stLocal.wDayOfWeek = 6;
        }
        else if (szDay == _T("星期日"))
        {
            stLocal.wDayOfWeek = 0;
        }

        szDescription.Format(_T("%s %d时%d分%d秒"), szDay, h, m, s);
        nType = CTimer::em_week;

    }
    else if (_T("每月") == szType)
    {
        m_time.GetTime(t);
        int h = t.GetHour();
        int m = t.GetMinute();
        int s = t.GetSecond();
        stLocal.wHour = h;
        stLocal.wMinute = m;
        stLocal.wSecond = s;
        stLocal.wDay = (WORD)_tstol(szDay);
        szDescription.Format(_T("%s号 %d时%d分%d秒"), szDay, h, m, s);
        nType = CTimer::em_month;
    }
    else
    {
        return;
    }

    if (m_timerManager.exist(nType, stLocal))
    {
        return;
    }
    if (szRemark.IsEmpty())
    {
        szRemark = _T("定时提醒：") + szDescription;
    }
    m_wndList.InsertItem(0, _T(""));
    m_wndList.SetItemText(0, 0, szType);
    m_wndList.SetItemText(0, 1, szDescription);
    m_wndList.SetItemText(0, 2, szRemark);

    spTimer sp = std::make_shared<CTimer>();
    
    sp->Set(nType, stLocal, szRemark);
    
    __time64_t next = sp->WillRing();
    CTime nexttime(next);
    m_wndList.SetItemText(0, 3, nexttime.Format(_T("%Y-%m-%d %H:%M:%S")));

    int index = m_timerManager.AddTimer(sp);
    m_wndList.SetItemData(0, (DWORD_PTR)index);

    m_timerManager.AnalysisTimer();
}


void CRemindingDlg::OnBnClickedButtonDel()
{
    // TODO: 在此添加控件通知处理程序代码
    POSITION pos = m_wndList.GetFirstSelectedItemPosition();
    if (pos)
    {
        int nItem = m_wndList.GetNextSelectedItem(pos);
        int nIndex = m_wndList.GetItemData(nItem);
        m_timerManager.RemoveTimer(nIndex);
        m_wndList.DeleteItem(nItem);
    }
}


void CRemindingDlg::OnBnClickedButtonExit()
{
    // TODO: 在此添加控件通知处理程序代码
    OnOK();
}


void CRemindingDlg::OnCbnSelchangeCombo3()
{
    // TODO: 在此添加控件通知处理程序代码
}

void CRemindingDlg::ChangeList(int type)
{
    if (1 == type)
    {
        m_weekDay.EnableWindow();
        m_weekDay.ResetContent();
        m_weekDay.AddString(_T("星期一"));
        m_weekDay.AddString(_T("星期二"));
        m_weekDay.AddString(_T("星期三"));
        m_weekDay.AddString(_T("星期四"));
        m_weekDay.AddString(_T("星期五"));
        m_weekDay.AddString(_T("星期六"));
        m_weekDay.AddString(_T("星期日"));
        m_weekDay.SetCurSel(0);
    }
    else if (2== type)
    {
        m_weekDay.EnableWindow();
        m_weekDay.ResetContent();
        CString day;
        for (int i = 1; i < 32; ++i)
        {
            day.Format(_T("%d"), i);
            m_weekDay.AddString(day);
        }
        m_weekDay.SetCurSel(0);
    }
    else
    {
        m_weekDay.EnableWindow(FALSE);
        m_weekDay.ResetContent();
    }
}

void CRemindingDlg::UpdateAlarmClock()
{
    m_timerManager.AnalysisTimer();
    m_timerManager.Sort();
}


void CRemindingDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    switch (nIDEvent)
    {
    case emtimer_1s:
    {
        CString szTime;
        szTime = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
        m_StatusBar.SetPaneText(1, szTime);
        spTimer timer = m_timerManager.NextTimer();
        if (timer)
        {
            long dt = CTime::GetCurrentTime().GetTime() - timer->WillRing();
            if (dt >= 0)
            {
                // 置顶
                ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                timer->Update();
                timer->WillRing();
                m_timerManager.AnalysisTimer();
                CString szText;
                szText.Format(_T("%s"), timer->Remark());
                PlaySound(_T("SystemStart"), NULL, SND_ALIAS | SND_ASYNC);
                MessageBox(szText, _T("闹钟"), MB_OK);
                PlaySound(NULL, NULL, SND_FILENAME);
                // 取消置顶
                ::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            }
        }
    }
        break;
    default:
        break;
    }

    CDialogEx::OnTimer(nIDEvent);
}


void CRemindingDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码

    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

    if (SIZE_MINIMIZED == nType)
    {
        // 最小华 
        ShowNotifyIcon(TRUE);
        ShowWindow(SW_HIDE);
    }
}

BOOL CRemindingDlg::InitNotifyIcon()
{
    // Add a Shell_NotifyIcon notificaion
    m_notifyIconData.cbSize = sizeof(m_notifyIconData);
    m_notifyIconData.uID = IDR_MAINFRAME;      // Per Windows Embedded CE docs, values from 0 to 12 are reserved and should not be used.
    m_notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_notifyIconData.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
    m_notifyIconData.uCallbackMessage = WM_NOTIFY_MESSAGE;
    lstrcpy(m_notifyIconData.szTip, _T("Reminding"));
    m_notifyIconData.hWnd = m_hWnd;
    // Add the notification to the tray.
    Shell_NotifyIcon(NIM_ADD, &m_notifyIconData);
    // Update the notification icon.
    // m_notifyIconData.uFlags = NIF_ICON;
    // m_notifyIconData.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
    return 0;
}

BOOL CRemindingDlg::ShowNotifyIcon(BOOL bShow)
{
    BOOL bResult = FALSE;
    if (bShow)
    {
        //bResult = Shell_NotifyIcon(NIM_MODIFY, &m_notifyIconData);
        bResult = Shell_NotifyIcon(NIM_ADD, &m_notifyIconData);
    }
    else
    {
        // Remove the notification from the tray.
        bResult = Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData);
    }
    return bResult;
}

LRESULT CRemindingDlg::NotifyIconCallBack(WPARAM wParam, LPARAM lParam)
{
    UINT uID{ wParam };
    UINT uMouseMsg{ (UINT)lParam };

    switch (uMouseMsg)
    {
    case WM_RBUTTONUP:
    {
        CMenu popMenu;
        popMenu.CreatePopupMenu();
        popMenu.AppendMenu(MF_STRING, IDC_BUTTON_EXIT, _T("退出"));
        POINT ptMouse;
        ::GetCursorPos(&ptMouse);
        ::SetForegroundWindow(m_notifyIconData.hWnd);
        ::TrackPopupMenu(popMenu.m_hMenu, 0, ptMouse.x, ptMouse.y, 0, m_notifyIconData.hWnd, NULL);
    }
    break;
    case WM_LBUTTONUP:
    {
        NotifyIconMesgRestore(0, 0);
    }
    break;
    default:
    {
    }
    break;
    }
    return LRESULT();
}

LRESULT CRemindingDlg::NotifyIconMesgRestore(WPARAM wParam, LPARAM lParam)
{
    // 还原到其原始大小并显示窗口
    ShowWindow(SW_SHOWNORMAL);
    // 置顶窗口
    ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    // 删除托盘图标
    ShowNotifyIcon(FALSE);
    // SendMessage(WM_SIZE, SIZE_RESTORED, SIZE_RESTORED);
    return LRESULT();
}


void CRemindingDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
    ShowNotifyIcon(FALSE);
}
