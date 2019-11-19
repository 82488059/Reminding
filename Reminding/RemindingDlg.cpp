
// RemindingDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Reminding.h"
#include "RemindingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

    m_wndList.InsertColumn(0, _T("类型"), LVCFMT_LEFT, 100);
    m_wndList.InsertColumn(1, _T("时间"), LVCFMT_LEFT, 200);
    m_wndList.InsertColumn(2, _T("备注"), LVCFMT_LEFT, 100);

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
    if (_T("每小时") == szType)
    {
        m_time.GetTime(t);
        int m = t.GetMinute();
        int s = t.GetSecond();
        szDescription.Format(_T("%d分%d秒"), m, s);
        nType = timer::em_hour;
    }
    else if (_T("每天") == szType)
    {
        m_time.GetTime(t);
        int h = t.GetHour();
        int m = t.GetMinute();
        int s = t.GetSecond();
        szDescription.Format(_T("%d时%d分%d秒"), h, m, s);
        nType = timer::em_day;

    }
    else if (_T("每周") == szType)
    {
        m_time.GetTime(t);
        int h = t.GetHour();
        int m = t.GetMinute();
        int s = t.GetSecond();
        szDescription.Format(_T("%s %d时%d分%d秒"), szDay, h, m, s);
        nType = timer::em_week;

    }
    else if (_T("每月") == szType)
    {
        m_time.GetTime(t);
        int h = t.GetHour();
        int m = t.GetMinute();
        int s = t.GetSecond();
        szDescription.Format(_T("%s号 %d时%d分%d秒"), szDay, h, m, s);
        nType = timer::em_day;
    }
    else
    {
        return;
    }

    m_wndList.InsertItem(0, _T(""));
    m_wndList.SetItemText(0, 0, szType);
    m_wndList.SetItemText(0, 1, szDescription);
    m_wndList.SetItemText(0, 2, szRemark);

    spTimer sp = std::make_shared<timer>();
    sp->type = nType;
    sp->time = t;
    sp->szRemark = szRemark;
    int index = m_timerManager.AddTimer(sp);
    m_wndList.SetItemData(0, (DWORD_PTR)index);

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
