
// RemindingDlg.h: 头文件
//

#pragma once
#include "timerManager.h"
#include <map>


// CRemindingDlg 对话框
class CRemindingDlg : public CDialogEx
{
// 构造
public:
	CRemindingDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMINDING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    CListCtrl m_wndList;
    CComboBox m_wndComb;
    CEdit m_remark;
    CDateTimeCtrl m_time;
    CComboBox m_weekDay;

    CTimerManager m_timerManager;

    CStatusBar m_StatusBar;
public:
    afx_msg void OnCbnSelchangeCombo1();
    afx_msg void OnCbnSelchangeCombo3();
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonDel();
    afx_msg void OnBnClickedButtonExit();

    void ChangeList(int type);
    void UpdateAlarmClock();
    enum {emtimer_1s = 1};
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
