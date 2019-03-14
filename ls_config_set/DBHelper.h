#pragma once

#include "sqlite3.h"
#include "stdint.h"
#include "string.h"

class CDBHelper
{

public:
	bool openDB();
	void closeDB();
	void ConvertGBKToUtf8(CString &strGBK);
	//�õ�������Ҫ�����Ⱥ
	void GetGroupArray(CStringArray& fromGroupArray);
	//�õ�������Ҫ�����Ⱥ
	void  GetMangerArray(CString fromGroup,CStringArray& fromMangerpArray);
	//�õ�ǩ������
	void GetSingedJiFen(CString fromGroup,int& nMin,int& nMax);
	//��ȡǩ������
	int GetInviteJiFen(CString fromGroup);
	//���ӿؼ�Ⱥ
	bool AddGroup(CString strGroup);
	//ɾ�����Ⱥ
	bool DeleteGroup(CString strGroup);
	//���ӹ���Ա
	bool AddManger(CString strGroup,CString strMangerQQ);
	//ɾ������Ա
	bool DeleteManger(CString strGroup,CString strMangerQQ);
	//�޸�ǩ������ֵ
	bool ModifySinged(CString strfromGroup,CString strAwardS,CString strAwardE);
	//�޸��������ֵ
	bool ModifyInvite(CString strfromGroup,CString strAward);
	CString Int64ToString(int64_t num);
	//���ز�ѯ��ֵ
	int GetSearchInt(CString  strSQL);
	bool execSql(CString strSQL);
	//��ʼ�����ݿ�
	void InitDb(CString strPath);
public:
	CDBHelper(void);
	~CDBHelper(void);

	 sqlite3 * pDB;
	 CString m_strpathDB;
};

