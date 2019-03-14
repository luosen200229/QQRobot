
// ls_config_setDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ls_config_set.h"
#include "ls_config_setDlg.h"
#include "afxdialogex.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cls_config_setDlg �Ի���



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
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;//ɾ���ļ�����ֻ���·��

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


// Cls_config_setDlg ��Ϣ�������

BOOL Cls_config_setDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

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


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cls_config_setDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cls_config_setDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cls_config_setDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}

//���Ӽ��Ⱥ
void Cls_config_setDlg::OnBnClickedButton1()
{

		
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE);
	if(!m_DBHelper.AddGroup(m_strFromGroupAdd))
	{
		AfxMessageBox(_T("������ӹ���Ⱥ��"));
		return;
	}
	m_CommGroup.AddString(m_strFromGroupAdd);
	m_CommGroup.SetCurSel(m_CommGroup.GetCount()-1);
	OnCbnSelchangeCombo1();

}

//���Ⱥ�л�
void Cls_config_setDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel = m_CommGroup.GetCurSel();

	if(nSel == -1)
	{
		return;
	}


	//���ع���Ա
	m_MangerListBox.ResetContent();
	CString strfromGroup;
	m_CommGroup.GetLBText(nSel,strfromGroup);


	CStringArray fromMangerpArray;
	m_DBHelper.GetMangerArray(strfromGroup,fromMangerpArray);
	for(int i = 0; i < fromMangerpArray.GetCount(); i ++)
	{
		m_MangerListBox.AddString(fromMangerpArray[i]);
	}
	//��ȡǩ������
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

	
	//��ȡ�������

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel = m_CommGroup.GetCurSel();

	if(nSel == -1)
	{
		AfxMessageBox(_T("��ѡ����Ҫ�Ĺ����Ⱥ��"));
		return;
	}

	if(m_strMangerQQ == _T(""))
	{
		AfxMessageBox(_T("���������ԱQQ��"));
		return;
	}
	CString strfromGroup;
	m_CommGroup.GetLBText(nSel,strfromGroup);

	if(m_DBHelper.AddManger(strfromGroup,m_strMangerQQ))
	{
		AfxMessageBox(_T("���ӹ���Ա�ɹ���"));
		OnCbnSelchangeCombo1();
		
	}
	else
	{
		AfxMessageBox(_T("���ӹ���Աʧ�ܣ�"));
	}


}


void Cls_config_setDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		AfxMessageBox(_T("�ɹ�ɾ����"));
		if(m_CommGroup.GetCount()>0)
		{
			m_CommGroup.SetCurSel(0);
			OnCbnSelchangeCombo1();
		}
	}
	else
	{
		AfxMessageBox(_T("ɾ��ʧ�ܣ�"));
	}

	

}


void Cls_config_setDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel = m_CommGroup.GetCurSel();

	if(nSel == -1)
	{
		AfxMessageBox(_T("��ѡ����Ҫ�����Ⱥ��"));
		return;
	}
	CString strfromGroup;
	m_CommGroup.GetLBText(nSel,strfromGroup);

	nSel = m_MangerListBox.GetCurSel();
	
	

	if(nSel == -1)
	{
		AfxMessageBox(_T("��ѡ����Ҫɾ���Ĺ���Ա��QQ��"));
		return;
	}

	CString strMangerQQ = "";
	m_MangerListBox.GetText(nSel,strMangerQQ);

	if(m_DBHelper.DeleteManger(strfromGroup,strMangerQQ))
	{
		AfxMessageBox(_T("ɾ������Ա�ɹ���"));
		OnCbnSelchangeCombo1();
		
	}
	else
	{
		AfxMessageBox(_T("ɾ������Աʧ�ܣ�"));
	}



}


void Cls_config_setDlg::OnBnClickedButton5()
{
	this->UpdateData(TRUE);
	int nSel = m_CommGroup.GetCurSel();

	if(nSel == -1)
	{
		AfxMessageBox(_T("��ѡ����Ҫ�����Ⱥ��"));
		return;
	}
	CString strfromGroup;
	m_CommGroup.GetLBText(nSel,strfromGroup);

	

	if(m_DBHelper.ModifySinged(strfromGroup,m_strAwardS,m_strAwardE))
	{
		AfxMessageBox(_T("�޸�ǩ�����ֳɹ���"));
		
	}
	else
	{
		AfxMessageBox(_T("�޸�ǩ������ʧ�ܣ�"));
	}

}


void Cls_config_setDlg::OnBnClickedButton6()
{
	this->UpdateData(TRUE);
	int nSel = m_CommGroup.GetCurSel();

	if(nSel == -1)
	{
		AfxMessageBox(_T("��ѡ����Ҫ�����Ⱥ��"));
		return;
	}
	CString strfromGroup;
	m_CommGroup.GetLBText(nSel,strfromGroup);

	

	if(m_DBHelper.ModifyInvite(strfromGroup,m_strInviteJiFen))
	{
		AfxMessageBox(_T("�޸�������ֳɹ���"));
		
	}
	else
	{
		AfxMessageBox(_T("�޸��������ʧ�ܣ�"));
	}

}
