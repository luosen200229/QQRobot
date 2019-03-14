#include "stdafx.h"
#include "MessageHelper.h"
#include<ctime>
#include "sqlite3.h"
#include  <io.h>
#include "Resource.h"


sqlite3 * pDB = NULL;

string szWeek[7]={"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};

#ifdef _DEBUG
string  g_strDBPath = "D:\\cn.com.lerio.statistic.db";
#else
string  g_strDBPath = "D:\\云烟监控版\\CQA\\app\\cn.com.lerio.statistic\\cn.com.lerio.statistic.db";
#endif





CMessageHelper::CMessageHelper(void)
{
	//CString str;

}

bool CMessageHelper::InitData()
{
	m_nGroupID = 542975091; 
	m_nMangerID= 2523644894;
	m_nLoadID=994868843; 
	return true;
}


CMessageHelper::~CMessageHelper(void)
{
}
//是否是被监控群
bool CMessageHelper::IsGroupID(int64_t groupID)
{

	string strSQL="";
	std_string_format(strSQL,"select ID from Set_Invite where GroupQQ=%s",Int64ToString(groupID).data());
	int nCount = GetRecordCount(strSQL);
	if(nCount == 1)
	{
		return true;
	}
	return false;
}
//是否是指定群的管理员

bool CMessageHelper::IsMangerID(int64_t groupID,int64_t QQ)
{

	string strSQL="";
	std_string_format(strSQL,"select ID from Set_QG_QQ where GroupQQ=%s and FromQQ=%s",Int64ToString(groupID).data(),Int64ToString(QQ).data());
	int nCount = GetRecordCount(strSQL);
	if(nCount >= 1)
	{
		return true;
	}

	return false;
}
bool CMessageHelper::IsLoadID(int64_t QQ)
{
	return m_nLoadID == QQ;
}
void CMessageHelper::ShowMsg(const char* msg)
{
	MessageBoxA(NULL, msg, "" ,0);
}

//0 : 返回日期查询字符串
//1： 返回时分秒字符串
//2： 返回年月日字符串
string CMessageHelper:: GetTimeString(int32_t time,int nType/*=0*/)
{
	int day,month,year,wday,hour,min,second;
	time_t t = time;
	struct tm * timeinfo;

	timeinfo = localtime(&t);
	day = timeinfo->tm_mday;
	month = timeinfo->tm_mon + 1;
	year = timeinfo->tm_year + 1900;
	hour = timeinfo->tm_hour;
	min = timeinfo->tm_min;
	second = timeinfo->tm_sec;
	wday = timeinfo->tm_wday;

	char tmp[1024]={0};
	string strMsg;

	if(nType == 0)
	{
		sprintf(tmp,"%d年%d月%d日 %s %.02d:%.02d:%.02d \r\n",year,month,day,szWeek[wday].data(),hour,min,second);
		strMsg = tmp;
		memset(tmp,0,1024);
		strftime(tmp,sizeof(tmp),"今天是今年的第 %j 天了",localtime(&t)); 
		strMsg += tmp;
	}
	else if(nType == 1)
	{
		sprintf(tmp,"%.02d时%.02d分%.02d秒",hour,min,second);
		strMsg = tmp;
	}
	else if(nType == 2)
	{
		sprintf(tmp,"%.04d年%.02d月%.02d日",year,month,day);
		strMsg = tmp;

	}
	else if(nType == 3)
	{
		sprintf(tmp,"%.04d-%.02d-%.02d %.02d:%.02d:%.02d",year,month,day,hour,min,second);
		strMsg = tmp;

	}

	return strMsg;
}

//返回查询结果个数
int CMessageHelper::GetRecordCount(string strSQL)
{

	int nCount = 0;
	if(!openDB())
	{
		return nCount;
	}
	sqlite3_stmt * stmt = NULL; 
	const char *zTail;

	string strUTF8 = GBKToUTF8(strSQL);
	if (sqlite3_prepare_v2(pDB, strUTF8.data(), -1, &stmt, &zTail) ==SQLITE_OK)
	{       
		while( sqlite3_step(stmt) ==SQLITE_ROW )
		{ 
			nCount ++;
		}
	}
	sqlite3_finalize(stmt); 

	return nCount;
}


//
int CMessageHelper::getRandSingedJiFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime,int64_t &singedTime,
									   int &nMin,int &nMax)
{

	int nSingedJiFen = 0;
	int day,month,year,wday,hour,min,second;
	struct tm * timeinfoS;
	struct tm * timeinfoE;
	struct tm * timeinfoC;
	int64_t sendTime1=sendTime;
	int64_t sendTime2=sendTime;
	int64_t sendTime3=sendTime;

	timeinfoS = localtime(&sendTime1);
	timeinfoE = localtime(&sendTime2);
	timeinfoC = localtime(&sendTime3);

	timeinfoS->tm_hour = 0;
	timeinfoS->tm_min = 0;
	timeinfoS->tm_sec = 0;
	int64_t sTime = mktime(timeinfoS);

	timeinfoE->tm_hour = 23;
	timeinfoE->tm_min = 59;
	timeinfoE->tm_sec = 59;


	int64_t eTime = mktime(timeinfoE);

	int nRecordCount  = 0;
	string strSql ="";
	std_string_format(strSql,"SELECT SignInTime FROM Record_SignIn where GroupQQ=%s and FromQQ=%s and SignInTime>%s and SignInTime<%s;",(Int64ToString(fromGroup)).data(),(Int64ToString(fromQQ)).data(),(Int64ToString(sTime)).data(),(Int64ToString(eTime)).data());
	singedTime=0;
	int nJiFen = 0;
	if(!openDB())
	{
		return -1;
	}
	sqlite3_stmt * stmt = NULL; 
	const char *zTail;

	string strUTF8 = GBKToUTF8(strSql);
	if (sqlite3_prepare_v2(pDB, strUTF8.data(), -1, &stmt, &zTail) ==SQLITE_OK)
	{       
		while( sqlite3_step(stmt) ==SQLITE_ROW )
		{ 
			singedTime =sqlite3_column_int64( stmt, 0); 
			break;
		}
	}
	sqlite3_finalize(stmt); 
	if(singedTime == 0)
	{
		strSql ="";
		std_string_format(strSql,"SELECT Award,AwardType,AwardS,AwardE FROM Set_SignIn where GroupQQ=%s;",(Int64ToString(fromGroup)).data());
		strUTF8 = GBKToUTF8(strSql);
		if (sqlite3_prepare_v2(pDB, strUTF8.data(), -1, &stmt, &zTail) ==SQLITE_OK)
		{       
			while( sqlite3_step(stmt) ==SQLITE_ROW )
			{ 
				int AwardType = sqlite3_column_int64( stmt, 1);
				int Award = sqlite3_column_int64( stmt, 0);
				int AwardS = sqlite3_column_int64( stmt, 2);
				int AwardE = sqlite3_column_int64( stmt, 3);

				if(AwardType == 0)
				{
					nSingedJiFen  = Award;
					nMin = nSingedJiFen;
				}
				else if(AwardType==1)
				{
					nMin = AwardS;
					nMax = AwardE;
					srand(time(NULL));
					nSingedJiFen = AwardS + rand() % (AwardE - AwardS);
				}
				else
				{
					nMin = 0;
					nMax = 0;
					nSingedJiFen = 0;
				}
				break;
			}
		}


	}
	else
	{
		nSingedJiFen = 0;
	}

	return nSingedJiFen;
}
int CMessageHelper::getSingedSort(int64_t fromGroup, int64_t fromQQ,int64_t sendTime,bool bTotal)
{

	int nSort = 0;
	string strSql ="";

	int nSingedJiFen = 0;
	int day,month,year,wday,hour,min,second;
	struct tm * timeinfoS;
	struct tm * timeinfoE;
	int64_t sendTime1=sendTime;
	int64_t sendTime2=sendTime;



	timeinfoS = localtime(&sendTime1);

	timeinfoS->tm_hour = 0;
	timeinfoS->tm_min = 0;
	timeinfoS->tm_sec = 0;
	int64_t sTime = mktime(timeinfoS);


	timeinfoE = localtime(&sendTime2);
	if(bTotal)
	{
		timeinfoE->tm_hour = 23;
		timeinfoE->tm_min = 59;
		timeinfoE->tm_sec = 59;
	}

	int64_t eTime = mktime(timeinfoE);

	std_string_format(strSql,"SELECT count(ID) FROM Record_SignIn where GroupQQ=%s and SignInTime>=%s and SignInTime < %s;",(Int64ToString(fromGroup)).data(),(Int64ToString(sTime)).data(),(Int64ToString(eTime)).data());
	nSort =  GetJiFen(strSql);

	return nSort;
}

string CMessageHelper::OppSinged(int64_t fromGroup, int64_t fromQQ,int64_t sendTime)
{
	string strMsg;
	strMsg = "[CQ:at,qq=";
	char buffer[32];
	_i64toa(fromQQ,buffer,10);
	strMsg += buffer;
	strMsg += "]\r\n";


	int64_t singedTime;
	int nMin = 0;
	int nMax = 0;
	int nJiFen = getRandSingedJiFen(fromGroup,fromQQ,sendTime,singedTime,nMin,nMax);

	string strTemp="";
	if(nJiFen == 0)
	{
		int nSort = getSingedSort(fromGroup,fromQQ,singedTime) + 1;
		int nTotal = getSingedSort(fromGroup,fromQQ,sendTime,true);
		std_string_format(strTemp, "你今天%s来签到过了，排名第 %s / %s 名哦！\r\n明天再来吧！\r\n\r\n",GetTimeString(singedTime,1).data(),Int64ToString(nSort).data(),Int64ToString(nTotal).data());
	}
	else if(nJiFen >= 0)
	{
		int nTotal = getSingedSort(fromGroup,fromQQ,sendTime,true);
		//前五名取最大积分
		if(nTotal <= 4)
		{
			nJiFen = nMax;
		}
		//10名之后最小积分
		else if(nTotal >= 15)
		{
			nJiFen = nMin;
		}
		InsertSingedRecord(fromGroup,fromQQ,sendTime);
		ModifyJiFen(fromGroup,fromQQ,nJiFen,"",sendTime,"群签到");
		int nSort = nTotal + 1;
		std_string_format(strTemp,"恭喜你，你今天是第%s个来签到哦！\r\n本次签到增加%s积分\r\n\r\n",Int64ToString(nSort).data(),Int64ToString(nJiFen).data());


	}
	else
	{
		strTemp = "签到失败！\r\n";
	}

	char szMsg[1024]={0};

	string strSearJifen = SearchJiFen( fromGroup,  fromQQ);
	sprintf_s(szMsg,"%s%s",strTemp.data(),strSearJifen.data());
	strMsg = szMsg;
	return strMsg;
}
string CMessageHelper::SearchJiFen(int64_t fromGroup, int64_t fromQQ)
{

	int nQianDao = getSingedFen(fromGroup,fromQQ);
	int nYaoQing = getYaoQingFen(fromGroup,fromQQ);
	int nJiangFa = getJiangFaFen(fromGroup,fromQQ);
	int nTotal   = getTotalFen(fromGroup,fromQQ);

	int nAdd=0;
	int nExit=0;

	getYaoQingNum( fromGroup,  fromQQ, nAdd, nExit);
	string strMsg;
	char szMsg[1024]={0};
	sprintf_s(szMsg,"[CQ:at,qq=%s] 你当前积分情况\r\n总积分：%d\r\n邀请积分：%d\r\n签到积分：%d\r\n奖惩积分：%d\r\n邀请人数：%d\r\n退群人数：%d\r\n有效人数：%d\r\n",
		Int64ToString(fromQQ).data(),
		nTotal,nYaoQing,nQianDao,nJiangFa,
		nAdd,nExit,nAdd-nExit
		);
	strMsg = szMsg;
	return strMsg;
}
string CMessageHelper::SearchTime()
{

	string strMsg = "【现在是北京时间】\r\n";

	strMsg += CMessageHelper::GetTimeString((int32_t)time(0));
	strMsg += "\r\n\r\n";
	strMsg += MSG_TAIL;
	return strMsg;

}

bool CMessageHelper::getAtMember(string strMsg,int64_t& atQQ,string& strContent)
{
	string strleft = "[CQ:at,qq=";

	if(strMsg.length() < strleft.length())
	{
		return false;
	}

	string strTemp = strMsg.substr(0,strleft.length());


	if(strcmp(strTemp.data(),strleft.data())!=0)
	{
		return false;
	}

	int nFind = strMsg.find(']');
	if(nFind==-1)
	{
		return false;
	}
	string strAt = strMsg.substr(strleft.length(),nFind -strleft.length());

	atQQ = _atoi64(strAt.data());
	if(strMsg.length()>nFind + 2)
	{
		strContent = strMsg.substr(nFind + 2);
	}
	else
	{
		strContent = "";
	}

	return true;

}

string CMessageHelper::Int64ToString(int64_t num)
{
	string strMsg = "";
	char tmp[1024]={0};
	_i64toa(num,tmp,10);
	strMsg = tmp;
	return strMsg;

}

bool CMessageHelper::InsertSingedRecord(int64_t fromGroup, int64_t fromQQ,int64_t sendTime)
{
	char tmp[1024]={0};

	// string strModule = "群奖惩";
	memset(tmp,0,1024);
	sprintf_s(tmp,"insert into Record_SignIn (LoginQQ,GroupQQ,FromQQ,SignInTime,Msg) VALUES(%s,%s,%s,%s,'%s')",
		"994868843",
		Int64ToString(fromGroup).data(),
		Int64ToString(fromQQ).data(),
		Int64ToString(sendTime).data(),
		GetTimeString(sendTime,3).data()
		);
	string strSQL = tmp;
	string strMsg = "";
	if(execSql(strSQL))
	{

		return true;

	}
	else
	{

		return false;
	}

}

//修改积分
string CMessageHelper::ModifyJiFen(int64_t fromGroup, int64_t fromQQ,int nJifen,string strContent,int32_t sendTime,string strModule)
{
	char tmp[1024]={0};

	// string strModule = "群奖惩";
	memset(tmp,0,1024);
	sprintf_s(tmp,"insert into Record_Award (LoginQQ,GroupQQ,FromQQ,Award,Module,Info,Time) VALUES(%s,%s,%s,%s,'%s','%s',%s)",
		"994868843",
		Int64ToString(fromGroup).data(),
		Int64ToString(fromQQ).data(),
		Int64ToString(nJifen).data(),
		strModule.data(),
		strContent.data(),
		Int64ToString(sendTime).data());
	string strSQL = tmp;
	string strMsg = "";

	openDB();
	if(execSql(strSQL))
	{


		memset(tmp,0,1024);
		sprintf_s(tmp," \r\n积分变化：%d\r\n\r\n",nJifen);
		strMsg += tmp;

	}
	else
	{
		strMsg = "\r\n积分修改失败！\r\n";
	}


	strMsg += SearchJiFen(fromGroup,fromQQ);





	return strMsg;

}

bool CMessageHelper::AddUser(const string& sName, const string& sAge)  
{  
	string strSql = "";  
	strSql += "insert into user(name,age)";  
	strSql += "values('";  
	strSql += sName;  
	strSql += "',";  
	strSql += sAge;  
	strSql += ");";  

	char* cErrMsg;  
	int nRes = sqlite3_exec(pDB , strSql.c_str() ,0 ,0, &cErrMsg);  
	if (nRes != SQLITE_OK)    
	{  

		return false;  
	}  
	else  
	{  

	}
	return true;  
}  


bool CMessageHelper::openDB()
{
	if(pDB != NULL)
	{
		return true;
	}


	string strUTF8 = GBKToUTF8(g_strDBPath);
	int nRes = sqlite3_open(strUTF8.c_str(), &pDB);
	if (nRes != SQLITE_OK)  
	{  
		pDB = NULL;
		return false; 
	}  
	return true;
}
void CMessageHelper::closeDB()
{
	sqlite3_close(pDB);
	pDB = NULL;
}


string & CMessageHelper::std_string_format(string & _str, const char * _Format, ...) {
	string tmp;

	va_list marker = NULL;
	va_start(marker, _Format);

	size_t num_of_chars = _vscprintf(_Format, marker);

	if (num_of_chars > tmp.capacity()) {
		tmp.resize(num_of_chars + 1);
	}

	vsprintf_s((char *)tmp.data(), tmp.capacity(), _Format, marker);

	va_end(marker);

	_str = tmp.c_str();
	return _str;
}
int64_t CMessageHelper::GetIni64tVaule(string strSQL)
{
	int64_t nJiFen = 0;
	if(!openDB())
	{
		return nJiFen;
	}
	sqlite3_stmt * stmt = NULL; 
	const char *zTail;

	string strUTF8 = GBKToUTF8(strSQL);
	if (sqlite3_prepare_v2(pDB, strUTF8.data(), -1, &stmt, &zTail) ==SQLITE_OK)
	{       
		while( sqlite3_step(stmt) ==SQLITE_ROW )
		{ 
			nJiFen =sqlite3_column_int64( stmt, 0); 
			break;
		}
	}
	sqlite3_finalize(stmt); 

	return nJiFen;
}
int CMessageHelper::GetJiFen(string strSQL)
{
	int nJiFen = 0;
	if(!openDB())
	{
		return nJiFen;
	}
	sqlite3_stmt * stmt = NULL; 
	const char *zTail;

	string strUTF8 = GBKToUTF8(strSQL);
	if (sqlite3_prepare_v2(pDB, strUTF8.data(), -1, &stmt, &zTail) ==SQLITE_OK)
	{       
		while( sqlite3_step(stmt) ==SQLITE_ROW )
		{ 
			nJiFen =sqlite3_column_int( stmt, 0); 
			break;
		}
	}
	sqlite3_finalize(stmt); 

	return nJiFen;
}
bool CMessageHelper::execSql(string strSQL)
{

	int nJiFen = 0;
	if(!openDB())
	{
		return false;
	}

	string strUTF8 = GBKToUTF8(strSQL);

	char* cErrMsg;  
	int nRes = sqlite3_exec(pDB , strUTF8.c_str() ,0 ,0, &cErrMsg);  
	if (nRes != SQLITE_OK)    
	{  
		//closeDB();
		return false;  
	}  
	//closeDB();

	return true;
}


string CMessageHelper::GBKToUTF8(const std::string& strGBK)  
{  
	string strOutUTF8 = "";  
	WCHAR * str1;  
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);  
	str1 = new WCHAR[n];  
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);  
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);  
	char * str2 = new char[n];  
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);  
	strOutUTF8 = str2;  
	delete[]str1;  
	str1 = NULL;  
	delete[]str2;  
	str2 = NULL;  
	return strOutUTF8;  
}  
int CMessageHelper::getSingedFen(int64_t fromGroup, int64_t fromQQ)
{
	int nJiFen = 0;
	string strSql ="";
	string strMoudle = "群签到";
	std_string_format(strSql,"SELECT sum(Award) FROM Record_Award where GroupQQ=%s and FromQQ=%s and Module='%s';",(Int64ToString(fromGroup)).data(),(Int64ToString(fromQQ)).data(),strMoudle.data());
	nJiFen =  GetJiFen(strSql);
	return nJiFen;
}
//得到邀请积分
int CMessageHelper::getYaoQingFen(int64_t fromGroup, int64_t fromQQ)
{
	int nJiFen = 0;
	string strSql ="";
	string strMoudle = "群邀请";	
	std_string_format(strSql,"SELECT sum(Award) FROM Record_Award where GroupQQ=%s and FromQQ=%s and Module='%s';",(Int64ToString(fromGroup)).data(),(Int64ToString(fromQQ)).data(),strMoudle.data());
	nJiFen =  GetJiFen(strSql);
	return nJiFen;
}
//得到奖惩积分
int CMessageHelper::getJiangFaFen(int64_t fromGroup, int64_t fromQQ)
{
	int nJiFen = 0;
	string strSql ="";
	string strMoudle = "群奖惩";
	std_string_format(strSql,"SELECT sum(Award) FROM Record_Award where GroupQQ=%s and FromQQ=%s and Module='%s';",(Int64ToString(fromGroup)).data(),(Int64ToString(fromQQ)).data(),strMoudle.data());
	nJiFen =  GetJiFen(strSql);
	return nJiFen;
}
//得到总积分
int CMessageHelper::getTotalFen(int64_t fromGroup, int64_t fromQQ)
{
	int nJiFen = 0;
	string strSql ="";
	std_string_format(strSql,"SELECT sum(Award) FROM Record_Award where GroupQQ=%s and FromQQ=%s;",(Int64ToString(fromGroup)).data(),(Int64ToString(fromQQ)).data());
	nJiFen =  CMessageHelper::GetJiFen(strSql);
	return nJiFen;
}
//获取邀请人数
bool CMessageHelper::getYaoQingNum(int64_t fromGroup, int64_t fromQQ,int& nAdd,int& nExit)
{
	nAdd = 0;
	nExit = 0;
	string strSql ="";
	string strMoudle = "入群";	
	std_string_format(strSql,"SELECT count(ID) FROM Record_Invite where GroupQQ=%s and FromQQ=%s and OpTypeInfo='%s';",(Int64ToString(fromGroup)).data(),(Int64ToString(fromQQ)).data(),strMoudle.data());
	nAdd =  CMessageHelper::GetJiFen(strSql);
	strMoudle = "退群";	
	std_string_format(strSql,"SELECT count(ID) FROM Record_Invite where GroupQQ=%s and FromQQ=%s and OpTypeInfo='%s';",(Int64ToString(fromGroup)).data(),(Int64ToString(fromQQ)).data(),strMoudle.data());
	nExit =  CMessageHelper::GetJiFen(strSql);

	return true;
}

/*
* Type=102 群事件-群成员减少
* subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
* fromQQ 操作者QQ(仅subType为2、3时存在)
* beingOperateQQ 被操作QQ
*/
string CMessageHelper::GroupMemberDecrease(int32_t subType, int32_t sendTime,
										   int64_t fromGroup, int64_t fromQQ, 
										   int64_t beingOperateQQ)
{
	string strSQL = "";
	string strMsg = "";

	std_string_format(strSQL,"select Award from Record_Invite where GroupQQ=%s and beingOperateQQ=%s and OpTypeInfo='入群'",
		Int64ToString(fromGroup).data(),
		Int64ToString(beingOperateQQ).data()
		);

	int nJifen = GetJiFen(strSQL);


	std_string_format(strSQL,"select FromQQ from Record_Invite where beingOperateQQ=%s order by InviteInTime desc",Int64ToString(beingOperateQQ).data());
	fromQQ = GetIni64tVaule(strSQL);

	if(fromQQ == 0)
	{
		return "";
	}

	std_string_format(strSQL,"insert into Record_Invite (LoginQQ,GroupQQ,FromQQ,beingOperateQQ,InviteInTime,Award,OpType,OpTypeInfo,Info) VALUES(%s,%s,%s,%s,%s,%s,%s,'%s','%s')",
		"994868843",
		Int64ToString(fromGroup).data(),
		Int64ToString(fromQQ).data(),
		Int64ToString(beingOperateQQ).data(),
		Int64ToString(sendTime).data(),
		Int64ToString(nJifen).data(),
		Int64ToString(subType).data(),
		"退群",
		""
		);
	execSql(strSQL);

	std_string_format(strMsg,"您邀请的%s退出了本群，积分减少%s积分.\r\n",
		Int64ToString(beingOperateQQ).data(),
		Int64ToString(nJifen).data());





	ModifyJiFen(fromGroup,fromQQ,-nJifen,"",sendTime,"群邀请");


	strMsg += SearchJiFen(fromGroup,fromQQ);



	return strMsg; //


}
/*
* Type=103 群事件-群成员增加
* subType 子类型，1/管理员已同意 2/管理员邀请
* fromQQ 操作者QQ(即管理员QQ)
* beingOperateQQ 被操作QQ(即加群的QQ)
*/
string CMessageHelper::GroupMemberIncrease(int32_t subType, int32_t sendTime, 
										   int64_t fromGroup, int64_t fromQQ,
										   int64_t beingOperateQQ)
{
	string strSQL = "";
	string strMsg = "";
	std_string_format(strSQL,"select Award from Set_Invite where GroupQQ=%s",Int64ToString(fromGroup).data());

	int nJifen = GetJiFen(strSQL);
	if(subType == 1)
	{
		//群成员邀请
		std_string_format(strSQL,"select FromQQ from Record_Invite where beingOperateQQ=%s order by InviteInTime desc",
			Int64ToString(beingOperateQQ).data());
		int64_t InviteQQ = GetIni64tVaule(strSQL);
		if(InviteQQ != 0)
		{
			fromQQ = InviteQQ;
		}

	}
	std_string_format(strSQL,"insert into Record_Invite (LoginQQ,GroupQQ,FromQQ,beingOperateQQ,InviteInTime,Award,OpType,OpTypeInfo,Info) VALUES(%s,%s,%s,%s,%s,%s,%s,'%s','%s')",
		"994868843",
		Int64ToString(fromGroup).data(),
		Int64ToString(fromQQ).data(),
		Int64ToString(beingOperateQQ).data(),
		Int64ToString(sendTime).data(),
		Int64ToString(nJifen).data(),
		Int64ToString(subType).data(),
		"入群",
		Int64ToString(fromQQ).data()
		);
	execSql(strSQL);


	std_string_format(strMsg,"您邀请[CQ:at,qq=%s] 加入了本群，积分增加%s积分.\r\n ",
		Int64ToString(beingOperateQQ).data(),
		Int64ToString(nJifen).data(),
		SearchJiFen(fromGroup,fromQQ).data());



	ModifyJiFen(fromGroup,fromQQ,nJifen,"",sendTime,"群邀请");

	strMsg += SearchJiFen(fromGroup,fromQQ);



	return strMsg; //
}

/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
*/
string CMessageHelper::RequestAddGroup(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, string msg,int64_t& InviteQQ)
{

	InviteQQ  = 0;
	string strSQL = "";
	string strMsg = "";
	if(subType == 1)
	{
		int64_t beingOperateQQ = fromQQ;

		//msg="邀请人：[CQ:at,qq=994868843]"
		int nFind = msg.find("邀请人");
		if(nFind == -1)
		{
			return "";
		}

		nFind  = msg.find("[");
		if(nFind == -1)
		{
			return "";
		}
		msg = msg.substr(nFind);

		string strContent="";
		if(!CMessageHelper::getAtMember(msg,InviteQQ,strContent))
		{
			return "";
		}


		std_string_format(strSQL,"select Award from Set_Invite where GroupQQ=%s",Int64ToString(fromGroup).data());

		int nJifen = GetJiFen(strSQL);

		std_string_format(strSQL,"insert into Record_Invite (LoginQQ,GroupQQ,FromQQ,beingOperateQQ,InviteInTime,Award,OpType,OpTypeInfo,Info) VALUES(%s,%s,%s,%s,%s,%s,%s,'%s','%s')",
			"994868843",
			Int64ToString(fromGroup).data(),
			Int64ToString(InviteQQ).data(),
			Int64ToString(beingOperateQQ).data(),
			Int64ToString(sendTime).data(),
			Int64ToString(nJifen).data(),
			Int64ToString(subType).data(),
			"邀请加群",
			Int64ToString(fromQQ).data()
			);
		execSql(strSQL);

		std_string_format(strMsg,"%s 邀请 %s 加群,请耐心等待管理员审核\r\n",
			Int64ToString(fromQQ).data(),
			Int64ToString(beingOperateQQ).data());
		return strMsg;

	}
	return "";



}