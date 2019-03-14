
// ls_config_setDlg.h : 头文件
//

#pragma once

#include "DBhelper.h"
#include "afxwin.h"




// Cls_config_setDlg 对话框
class Cls_config_setDlg : public CDialogEx
{
// 构造
public:
	Cls_config_setDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LS_CONFIG_SET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

     CDBHelper m_DBHelper;


// 实现
protected:
	HICON m_hIcon;
   

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_CommGroup;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	CListBox m_MangerListBox;
	afx_msg void OnBnClickedButton3();
	CString m_strAwardS;
	CString m_strAwardE;
	CString m_strInviteJiFen;
	CString m_strFromGroupAdd;
	afx_msg void OnBnClickedButton2();
	CString m_strMangerQQ;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
