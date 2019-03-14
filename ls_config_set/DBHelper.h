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
	//得到所有需要管理的群
	void GetGroupArray(CStringArray& fromGroupArray);
	//得到所有需要管理的群
	void  GetMangerArray(CString fromGroup,CStringArray& fromMangerpArray);
	//得到签到积分
	void GetSingedJiFen(CString fromGroup,int& nMin,int& nMax);
	//获取签到积分
	int GetInviteJiFen(CString fromGroup);
	//增加控件群
	bool AddGroup(CString strGroup);
	//删除监控群
	bool DeleteGroup(CString strGroup);
	//增加管理员
	bool AddManger(CString strGroup,CString strMangerQQ);
	//删除管理员
	bool DeleteManger(CString strGroup,CString strMangerQQ);
	//修改签到积分值
	bool ModifySinged(CString strfromGroup,CString strAwardS,CString strAwardE);
	//修改邀请积分值
	bool ModifyInvite(CString strfromGroup,CString strAward);
	CString Int64ToString(int64_t num);
	//返回查询数值
	int GetSearchInt(CString  strSQL);
	bool execSql(CString strSQL);
	//初始化数据库
	void InitDb(CString strPath);
public:
	CDBHelper(void);
	~CDBHelper(void);

	 sqlite3 * pDB;
	 CString m_strpathDB;
};

