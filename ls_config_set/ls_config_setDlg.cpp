
// ls_config_setDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ls_config_set.h"
#include "ls_config_setDlg.h"
#include "afxdialogex.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cls_config_setDlg 对话框



Cls_config_setDlg::Cls_config_setDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cls_config_setDlg::IDD, pParent)
	, m_strAwardS(_T(""))
	, m_strAwardE(_T(""))
	, m_strInviteJiFen(_T(""))
	, m_strFromGroupAdd(_T(""))
	, m_strMangerQQ(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	TCHAR szFilePath[MAX_PATH + 1]; 
	GetModuleFileName(NULL, szFilePath, MAX_PATH); 
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;//删除文件名，只获得路径

	CString strDBPath =  szFilePath;
	strDBPath += _T("com.luosen200229.sq");
	
	_mkdir(strDBPath); 
	strDBPath += _T("\\com.luosen200229.sq.db");
	m_DBHelper.InitDb(strDBPath);

	

}

void Cls_config_setDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CommGroup);
	DDX_Control(pDX, IDC_LIST3, m_MangerListBox);
	DDX_Text(pDX, IDC_EDIT3, m_strAwardS);
	DDX_Text(pDX, IDC_EDIT4, m_strAwardE);
	DDX_Text(pDX, IDC_EDIT5, m_strInviteJiFen);
	DDX_Text(pDX, IDC_EDIT1, m_strFromGroupAdd);
	DDX_Text(pDX, IDC_EDIT2, m_strMangerQQ);
}

BEGIN_MESSAGE_MAP(Cls_config_setDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Cls_config_setDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Cls_config_setDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Cls_config_setDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, &Cls_config_setDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &Cls_config_setDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &Cls_config_setDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &Cls_config_setDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &Cls_config_setDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// Cls_config_setDlg 消息处理程序

BOOL Cls_config_setDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CStringArray fromgroupArr;
	m_DBHelper.GetGroupArray(fromgroupArr);

	for(int i = 0; i < fromgroupArr.GetCount(); i ++)
	{
		m_CommGroup.AddString(fromgroupArr[i]);
	}
	if(fromgroupArr.GetCount()>0)
	{
		m_CommGroup.SetCurSel(0);
		OnCbnSelchangeCombo1();
	}


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cls_config_setDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cls_config_setDlg::OnPaint()
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
HCURSOR Cls_config_setDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cls_config_setDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

//增加监控群
void Cls_config_setDlg::OnBnClickedButton1()
{

		
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE);
	if(!m_DBHelper.AddGroup(m_strFromGroupAdd))
	{
		AfxMessageBox(_T("你已添加过此群！"));
		return;
	}
	m_CommGroup.AddString(m_strFromGroupAdd);
	m_CommGroup.SetCurSel(m_CommGroup.GetCount()-1);
	OnCbnSelchangeCombo1();

}

//监控群切换
void Cls_config_setDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_CommGroup.GetCurSel();

	if(nSel == -1)
	{
		return;
	}


	//加载管理员
	m_MangerListBox.ResetContent();
	CString strfromGroup;
	m_CommGroup.GetLBText(nSel,strfromGroup);


	CStringArray fromMangerpArray;
	m_DBHelper.GetMangerArray(strfromGroup,fromMangerpArray);
	for(int i = 0; i < fromMangerpArray.GetCount(); i ++)
	{
		m_MangerListBox.AddString(fromMangerpArray[i]);
	}
	//获取签到积分
	int nMin;
	int nMax;
	m_DBHelper.GetSingedJiFen(strfromGroup, nMin, nMax);
	if(nMin !=0)
	{
		m_strAwardS.Format(_T("%d"),nMin);
	}
	else
	{
		m_strAwardS = "";
	}

	if(nMax !=0)
	{
		m_strAwardE.Format(_T("%d"),nMax);
	}
	else
	{
		m_strAwardE = "";
	}

	
	//获取邀请积分

	int nInviteJiFen = m_DBHelper.GetInviteJiFen(strfromGroup);

	if(nInviteJiFen !=0)
	{
		m_strInviteJiFen.Format(_T("%d"),nInviteJiFen);
	}
	else
	{
		m_strInviteJiFen = "";
	}
	

	this->UpdateData(FALSE);


	

}


void Cls_config_setDlg::OnBnClickedButton3()
{

	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_CommGroup.GetCurSel();

	if(nSel == -1)
	{
		AfxMessageBox(_T("请选择需要的管理的群！"));
		return;
	}

	if(m_strMangerQQ == _T(""))
	{
		AfxMessageBox(_T("请输入管理员QQ！"));
		return;
	}
	CString strfromGroup;
	m_CommGroup.GetLBText(nSel,strfromGroup);

	if(m_DBHelper.AddManger(strfromGroup,m_strMangerQQ))
	{
		AfxMessageBox(_T("增加管理员成功！"));
		OnCbnSelchangeCombo1();
		
	}
	else
	{
		AfxMessageBox(_T("增加管理员失败！"));
	}


}


void Cls_config_setDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_CommGroup.GetCurSel();

	if(nSel == -1)
	{
		return;
	}

	CString strfromGroup;
	m_CommGroup.GetLBText(nSel,strfromGroup);

	if(m_DBHelper.DeleteGroup(strfromGroup))
	{
		m_CommGroup.DeleteString(nSel);
		AfxMessageBox(_T("成功删除！"));
		if(m_CommGroup.GetCount()>0)
		{
			m_CommGroup.SetCurSel(0);
			OnCbnSelchangeCombo1();
		}
	}
	else
	{
		AfxMessageBox(_T("删除失败！"));
	}

	

}


void Cls_config_setDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_CommGroup.GetCurSel();

	if(nSel == -1)
	{
		AfxMessageBox(_T("请选择需要管理的群！"));
		return;
	}
	CString strfromGroup;
	m_CommGroup.GetLBText(nSel,strfromGroup);

	nSel = m_MangerListBox.GetCurSel();
	
	

	if(nSel == -1)
	{
		AfxMessageBox(_T("请选择需要删除的管理员的QQ！"));
		return;
	}

	CString strMangerQQ = "";
	m_MangerListBox.GetText(nSel,strMangerQQ);

	if(m_DBHelper.DeleteManger(strfromGroup,strMangerQQ))
	{
		AfxMessageBox(_T("删除管理员成功！"));
		OnCbnSelchangeCombo1();
		
	}
	else
	{
		AfxMessageBox(_T("删除管理员失败！"));
	}



}


void Cls_config_setDlg::OnBnClickedButton5()
{
	this->UpdateData(TRUE);
	int nSel = m_CommGroup.GetCurSel();

	if(nSel == -1)
	{
		AfxMessageBox(_T("请选择需要管理的群！"));
		return;
	}
	CString strfromGroup;
	m_CommGroup.GetLBText(nSel,strfromGroup);

	

	if(m_DBHelper.ModifySinged(strfromGroup,m_strAwardS,m_strAwardE))
	{
		AfxMessageBox(_T("修改签到积分成功！"));
		
	}
	else
	{
		AfxMessageBox(_T("修改签到积分失败！"));
	}

}


void Cls_config_setDlg::OnBnClickedButton6()
{
	this->UpdateData(TRUE);
	int nSel = m_CommGroup.GetCurSel();

	if(nSel == -1)
	{
		AfxMessageBox(_T("请选择需要管理的群！"));
		return;
	}
	CString strfromGroup;
	m_CommGroup.GetLBText(nSel,strfromGroup);

	

	if(m_DBHelper.ModifyInvite(strfromGroup,m_strInviteJiFen))
	{
		AfxMessageBox(_T("修改邀请积分成功！"));
		
	}
	else
	{
		AfxMessageBox(_T("修改邀请积分失败！"));
	}

}
