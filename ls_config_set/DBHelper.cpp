#include "stdafx.h"
#include "DBHelper.h"
#include  <io.h>
#include "Resource.h"


int g_szSQLID[]={
	IDS_Record_Award,
	IDS_Record_GroupRequest,
	IDS_Record_Invite,
	IDS_Record_Invite,
	IDS_Record_SignIn,
	IDS_Set_AppSet,
	IDS_Set_AutoReply,
	IDS_Set_AutoSend,
	IDS_Set_Invite,
	IDS_Set_QG_auto,
	IDS_Set_QG_QQ,
	IDS_Set_SignIn

};


CDBHelper::CDBHelper(void)
{
	pDB = NULL;
	m_strpathDB = "";
	/*
#ifdef _DEBUG
	m_strpathDB = "D:\\cn.com.lerio.statistic.db";
#else
	m_strpathDB = "D:\\云烟监控版\\CQA\\app\\cn.com.lerio.statistic\\cn.com.lerio.statistic.db";
#endif
	InitDb(m_strpathDB);
	*/
}


CDBHelper::~CDBHelper(void)
{
}


//初始化数据库
void CDBHelper::InitDb(CString strPath)
{
	m_strpathDB = strPath;
	if( (_access(strPath, 0 )) != -1 )
	{
		return;
	}
	int nCount = sizeof(g_szSQLID)/sizeof(g_szSQLID[0]);
	
	openDB();
	for(int i = 0; i  < nCount; i ++)
	{
		
		CString strSQL  = MAKEINTRESOURCE(g_szSQLID[i]);
	

		execSql(strSQL);
	}
	closeDB();

}




bool CDBHelper::openDB()
  {
	  if(pDB != NULL)
	  {
		  return true;
	  }

	   CString  strUTF8 = m_strpathDB;
	  ConvertGBKToUtf8(strUTF8);

	  int nRes = sqlite3_open(strUTF8, &pDB);
	   if (nRes != SQLITE_OK)  
	   {  
		   pDB = NULL;
		   return false; 
	   }  
	   return true;
  }
 void CDBHelper::closeDB()
 {
	 sqlite3_close(pDB);
	 pDB = NULL;
 }


 


//得到所有需要管理的群
void  CDBHelper::GetGroupArray(CStringArray& fromGroupArray)
{
	CStringArray ;
	openDB();
	CString strSQL = _T("select GroupQQ from Set_Invite");
   CString  strUTF8 = strSQL;
	ConvertGBKToUtf8(strUTF8);

	 sqlite3_stmt * stmt = NULL; 
	 const char *zTail;
	

	 if (sqlite3_prepare_v2(pDB, strUTF8, -1, &stmt, &zTail) ==SQLITE_OK)
	 {       
		while( sqlite3_step(stmt) ==SQLITE_ROW )
		{ 
			int64_t fromgroup  =sqlite3_column_int64( stmt, 0); 
			fromGroupArray.Add(Int64ToString(fromgroup));
		 }
	 }
	sqlite3_finalize(stmt); 
}


//得到所有需要管理的群
void  CDBHelper::GetMangerArray(CString fromGroup,CStringArray& fromMangerpArray)
{
	CStringArray ;
	openDB();
	CString strSQL;
	strSQL.Format(_T("select FromQQ from Set_QG_QQ where GroupQQ=%s and OPType=1"),fromGroup);
   CString  strUTF8 = strSQL;
	ConvertGBKToUtf8(strUTF8);

	 sqlite3_stmt * stmt = NULL; 
	 const char *zTail;
	

	 if (sqlite3_prepare_v2(pDB, strUTF8, -1, &stmt, &zTail) ==SQLITE_OK)
	 {       
		while( sqlite3_step(stmt) ==SQLITE_ROW )
		{ 
			int64_t fromgroup  =sqlite3_column_int64( stmt, 0); 
			fromMangerpArray.Add(Int64ToString(fromgroup));
		 }
	 }
	sqlite3_finalize(stmt); 
}


void CDBHelper::ConvertGBKToUtf8(CString &strGBK)
 {
  int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0);
  wchar_t * wszUtf8 = new wchar_t [len];
  memset(wszUtf8, 0, len);
  MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, wszUtf8, len);
  len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);
  char *szUtf8=new char[len + 1];
  memset(szUtf8, 0, len + 1);
  WideCharToMultiByte (CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL,NULL);
  strGBK = szUtf8;
  delete[] szUtf8;
  delete[] wszUtf8;
 }

 CString CDBHelper::Int64ToString(int64_t num)
 {
	  CString strMsg = "";
	 char tmp[1024]={0};
	 _i64toa(num,tmp,10);
	 strMsg = tmp;
	 return strMsg;

 }
 //得到签到积分
void CDBHelper::GetSingedJiFen(CString fromGroup,int& nMin,int& nMax)
{
	nMin = 0;
	nMax = 0;
	int nAward = 0;
	int nType = 0;

	openDB();
	CString strSQL;
	strSQL.Format(_T("select AwardType,Award,AwardS,AwardE from Set_SignIn where GroupQQ=%s"),fromGroup);
    CString  strUTF8 = strSQL;
	ConvertGBKToUtf8(strUTF8);

	 sqlite3_stmt * stmt = NULL; 
	 const char *zTail;
	

	 if (sqlite3_prepare_v2(pDB, strUTF8, -1, &stmt, &zTail) ==SQLITE_OK)
	 {       
		while( sqlite3_step(stmt) ==SQLITE_ROW )
		{ 
			nType =sqlite3_column_int( stmt, 0); 
			if(nType == 0)
			{
				nMin  = sqlite3_column_int( stmt, 1);
				nMax = nMin;
			}
			else if(nType == 1)
			{
				nMin  = sqlite3_column_int( stmt, 2);
				nMax = sqlite3_column_int( stmt, 3);
			}
			break;
		 }
	 }
	sqlite3_finalize(stmt); 
}

//获取签到积分
int CDBHelper::GetInviteJiFen(CString fromGroup)
{
	int nAward = 0;
	openDB();
	CString strSQL;
	strSQL.Format(_T("select Award from Set_Invite where GroupQQ=%s"),fromGroup);
    CString  strUTF8 = strSQL;
	ConvertGBKToUtf8(strUTF8);

	 sqlite3_stmt * stmt = NULL; 
	 const char *zTail;
	

	 if (sqlite3_prepare_v2(pDB, strUTF8, -1, &stmt, &zTail) ==SQLITE_OK)
	 {       
		while( sqlite3_step(stmt) ==SQLITE_ROW )
		{ 
			nAward =sqlite3_column_int( stmt, 0); 
			break;
		 }
	 }
	sqlite3_finalize(stmt); 
	return nAward;
}

//返回查询数值
int CDBHelper::GetSearchInt(CString  strSQL)
{

	 int nValue = 0;
	 if(!openDB())
	 {
		 return nValue;
	 }
	 sqlite3_stmt * stmt = NULL; 
	 const char *zTail;
	
	 CString strUTF8  = strSQL;
	 ConvertGBKToUtf8(strUTF8);

	 if (sqlite3_prepare_v2(pDB, strUTF8, -1, &stmt, &zTail) ==SQLITE_OK)
	 {       
		while( sqlite3_step(stmt) ==SQLITE_ROW )
		{ 
			nValue = sqlite3_column_int( stmt, 0); 
			break;
		 }
	 }
	sqlite3_finalize(stmt); 

	 return nValue;
}

bool CDBHelper::execSql(CString strSQL)
{

	 int nJiFen = 0;
	 if(!openDB())
	 {
		 return false;
	 }
	
	 CString strUTF8  = strSQL;
	 ConvertGBKToUtf8(strUTF8);

    char* cErrMsg;  
    int nRes = sqlite3_exec(pDB , strUTF8 ,0 ,0, &cErrMsg);  
    if (nRes != SQLITE_OK)    
    {  
		
        return false;  
    }  


   return true;
}

//增加监控群
bool CDBHelper::AddGroup(CString strGroup)
{

	CString strSQL="";
	strSQL.Format("select ID from Set_Invite where GroupQQ=%s",strGroup);
	int nCount = GetSearchInt(strSQL);
	if(nCount > 0)
	{
		return false;
	}

	strSQL.Format(_T("insert into Set_Invite (LoginQQ,GroupQQ) VALUES(%s,%s)"),
		"994868843",strGroup);
	return execSql(strSQL);
}
//删除监控群
bool CDBHelper::DeleteGroup(CString strGroup)
{
	CString strSQL="";
	strSQL.Format("delete  from Set_Invite where GroupQQ=%s",strGroup);
	return execSql(strSQL);
}

//增加管理员
bool CDBHelper::AddManger(CString strGroup,CString strMangerQQ)
{
	CString strSQL="";
	strSQL.Format("select ID from Set_QG_QQ where GroupQQ='%s' and FromQQ='%s' and OPType=1",strGroup,strMangerQQ);
	int nCount = GetSearchInt(strSQL);
	if(nCount > 0)
	{
		return true;
	}
	
	strSQL.Format("insert into Set_QG_QQ (GroupQQ,FromQQ,OPType) VALUES(%s,%s,1)",strGroup,strMangerQQ);
	return execSql(strSQL);
}

//删除管理员
bool CDBHelper::DeleteManger(CString strGroup,CString strMangerQQ)
{
	CString strSQL="";
	strSQL.Format("delete from Set_QG_QQ where GroupQQ=%s and FromQQ=%s",strGroup,strMangerQQ);
	return execSql(strSQL);
}

//修改签到积分值
bool CDBHelper::ModifySinged(CString strfromGroup,CString strAwardS,CString strAwardE)
{
	CString strSQL="";

	strSQL.Format("select ID from Set_SignIn where GroupQQ='%s'",strfromGroup);
	int nCount = GetSearchInt(strSQL);
	if(nCount > 0)
	{
		strSQL.Format("update Set_SignIn set AwardS=%s,AwardE=%s,AwardType=1 where GroupQQ=%s",strAwardS,strAwardE,strfromGroup);

	}
	else
	{
		strSQL.Format("insert into Set_SignIn  (LoginQQ,AwardType,AwardS,AwardE,GroupQQ)VALUES(994868843,1,%s,%s,%s)",strAwardS,strAwardE,strfromGroup);
	}
	
	return execSql(strSQL);
}

//修改邀请积分值
bool CDBHelper::ModifyInvite(CString strfromGroup,CString strAward)
{
	CString strSQL="";

	strSQL.Format("select ID from Set_Invite where GroupQQ='%s'",strfromGroup);
	int nCount = GetSearchInt(strSQL);
	if(nCount > 0)
	{
		strSQL.Format("update Set_Invite set Award=%s where GroupQQ=%s",strAward,strfromGroup);

	}
	else
	{
		strSQL.Format("insert into Set_Invite  (LoginQQ,Award,GroupQQ)VALUES(994868843,%s,%s)",strAward,strAward,strfromGroup);
	}
	
	return execSql(strSQL);
}


