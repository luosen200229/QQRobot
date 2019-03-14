#include "stdafx.h"
#include "MessageHelper.h"
#include<ctime>
#include "sqlite3.h"
#include  <io.h>
#include "Resource.h"


sqlite3 * pDB = NULL;

string szWeek[7]={"������","����һ","���ڶ�","������","������","������","������"};

#ifdef _DEBUG
string  g_strDBPath = "D:\\cn.com.lerio.statistic.db";
#else
string  g_strDBPath = "";// "D:\\���̼�ذ�\\CQA\\app\\cn.com.lerio.statistic\\cn.com.lerio.statistic.db";
#endif





CMessageHelper::CMessageHelper(void)
{
	//CString str;

}

bool CMessageHelper::InitData()
{	
	return true;
}


CMessageHelper::~CMessageHelper(void)
{
}
//�Ƿ��Ǳ����Ⱥ
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
//�Ƿ���ָ��Ⱥ�Ĺ���Ա

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
	//return m_nLoadID == QQ;
	return true;
}
void CMessageHelper::ShowMsg(const char* msg)
{
	MessageBoxA(NULL, msg, "" ,0);
}

//0 : �������ڲ�ѯ�ַ���
//1�� ����ʱ�����ַ���
//2�� �����������ַ���
string CMessageHelper:: GetTimeString(int64_t time,int nType/*=0*/)
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
		sprintf(tmp,"%d��%d��%d�� %s %.02d:%.02d:%.02d \r\n",year,month,day,szWeek[wday].data(),hour,min,second);
		strMsg = tmp;
		memset(tmp,0,1024);
		strftime(tmp,sizeof(tmp),"�����ǽ���ĵ� %j ����",localtime(&t)); 
		strMsg += tmp;
	}
	else if(nType == 1)
	{
		sprintf(tmp,"%.02dʱ%.02d��%.02d��",hour,min,second);
		strMsg = tmp;
	}
	else if(nType == 2)
	{
		sprintf(tmp,"%.04d��%.02d��%.02d��",year,month,day);
		strMsg = tmp;

	}
	else if(nType == 3)
	{
		sprintf(tmp,"%.04d-%.02d-%.02d %.02d:%.02d:%.02d",year,month,day,hour,min,second);
		strMsg = tmp;

	}

	return strMsg;
}

//���ز�ѯ�������
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
					if(nMin != nMax)
					{
						nSingedJiFen = AwardS + rand() % (AwardE - AwardS);
					}
					else
					{
						nSingedJiFen = nMax;

					}
					
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

//��ȡ����ǩ������
int CMessageHelper::GetSingedDays(int64_t fromGroup, int64_t fromQQ,int64_t sendTime)
{
	string strSql ="";
	std_string_format(strSql,"SELECT SignIn_Count,Last_SignInTime FROM Record_SignIn_Total where GroupQQ=%s and  FromQQ=%s;",(Int64ToString(fromGroup)).data(),
		(Int64ToString(fromQQ)).data());

	

	if(!openDB())
	{
		return 0;
	}
	sqlite3_stmt * stmt = NULL; 
	const char *zTail;

	string strUTF8 = GBKToUTF8(strSql);
	int nSignInCount = 0;
	int nLastTime = 0; 

	struct tm * timeinfoSend;	
	timeinfoSend = localtime(&sendTime);
	timeinfoSend->tm_hour = 0;
	timeinfoSend->tm_min = 0;
	timeinfoSend->tm_sec = 0;
	sendTime = mktime(timeinfoSend);
	


	if (sqlite3_prepare_v2(pDB, strUTF8.data(), -1, &stmt, &zTail) ==SQLITE_OK)
	{       
		while( sqlite3_step(stmt) ==SQLITE_ROW )
		{ 
			nSignInCount = sqlite3_column_int( stmt, 0); 
			nLastTime = sqlite3_column_int( stmt,1);
			if(sendTime - nLastTime > 24 * 60 * 60)
			{
				nSignInCount = 0;
			}			
			break;
		}
	}
	sqlite3_finalize(stmt); 
	return nSignInCount;
}
//��������ǩ����
void CMessageHelper::SetSingedDays(int nDays,int64_t fromGroup, int64_t fromQQ,int64_t sendTime)
{
	string strSql ="";
	std_string_format(strSql,"SELECT ID FROM Record_SignIn_Total where GroupQQ=%s and  FromQQ=%s;",(Int64ToString(fromGroup)).data(),
		(Int64ToString(fromQQ)).data());
	
	int nCount = GetRecordCount(strSql);
	
	if(nCount > 0)
	{
		std_string_format(strSql,"update Record_SignIn_Total set SignIn_Count=%s,Last_SignInTime=%s where GroupQQ=%s and FromQQ=%s",
			(Int64ToString(nDays)).data(),
			(Int64ToString(sendTime)).data(),
			(Int64ToString(fromGroup)).data(),
			(Int64ToString(fromQQ)).data());

	}
	else
	{
		std_string_format(strSql,"insert into Record_SignIn_Total  (GroupQQ,FromQQ,SignIn_Count,Last_SignInTime)VALUES(%s,%s,%s,%s)",
			(Int64ToString(fromGroup)).data(),
			(Int64ToString(fromQQ)).data(),
			(Int64ToString(nDays)).data(),
			(Int64ToString(sendTime)).data()
			);
	}
	execSql(strSql);
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

		int nDays = GetSingedDays(fromGroup,fromQQ,sendTime);
		if(nDays == 0)
		{
			nDays ++;
		}
#ifdef USER_LS
		std_string_format(strTemp, "�����%s��ǩ�����ˣ������� %s / %s ��Ŷ��\r\n����ǩ�� %s ��\r\n\r\n���������ɣ�\r\n\r\n",
			GetTimeString(singedTime,1).data(),
			Int64ToString(nSort).data(),
			Int64ToString(nTotal).data(),
			Int64ToString(nDays).data());
#else
std_string_format(strTemp, "�����%s��ǩ������\r\n",
			GetTimeString(singedTime,1).data()			
			);
#endif

		
	}
	else if(nJiFen >= 0)
	{
		int nTotal = getSingedSort(fromGroup,fromQQ,sendTime,true);


		//ǰ����ȡ������
		if(nTotal <= 4)
		{
			nJiFen = nMax;
		}
		//15��֮����С����
		else if(nTotal >= 15)
		{
			nJiFen = nMin;
		}


		//��ȡ����ǩ������
		int nDays = GetSingedDays(fromGroup,fromQQ,sendTime);
		InsertSingedRecord(fromGroup,fromQQ,sendTime);
		nDays ++;
		SetSingedDays(nDays,fromGroup,fromQQ,sendTime);
		int nAddJiangli = 0;
		if(nDays >= 3 && nDays <= 10)
		{
			nAddJiangli = nDays;			
		}
		else if(nDays > 10)
		{
			nAddJiangli = 10;
		}	

#ifdef USER_LS
		ModifyJiFen(fromGroup,fromQQ,nJiFen+nAddJiangli,"",sendTime,"Ⱥǩ��");
		int nSort = nTotal + 1;
		std_string_format(strTemp,"��ϲ�㣬������ǵ� %s ����ǩ��Ŷ��\r\n����ǩ�� %s ��\r\n����ǩ������ %s ����\r\n���⽱�� %s ����\r\n\r\n",
			Int64ToString(nSort).data(),
			Int64ToString(nDays).data(),
			Int64ToString(nJiFen).data(),
			Int64ToString(nAddJiangli).data());
#else
		ModifyJiFen(fromGroup,fromQQ,nJiFen,"",sendTime,"Ⱥǩ��");
		int nSort = nTotal + 1;
		std_string_format(strTemp,"���� %s ����\r\n",
			Int64ToString(nJiFen).data()
			);
#endif


	}
	else
	{
		strTemp = "ǩ��ʧ�ܣ�\r\n";
	}

	char szMsg[1024]={0};

	string strSearJifen = SearchJiFen( fromGroup,  fromQQ,sendTime);
	sprintf_s(szMsg,"%s%s",strTemp.data(),strSearJifen.data());
	strMsg = szMsg;
	return strMsg;
}
string CMessageHelper::SearchJiFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime)
#ifdef USER_LS
{

	
	
	int64_t sendTimeC=sendTime;
	struct tm * timeinfo;
	timeinfo = localtime(&sendTimeC);
	

	int mon = timeinfo->tm_mon + 1;
	string strMsg = Int64ToString(mon).data();
	string strMon = "";

	if(mon <=3)
	{
		strMon = "��һ����";
		timeinfo->tm_mon = 0;
		
		
	}
	else  if(mon <=6)
	{
		strMon = "�ڶ�����";
		timeinfo->tm_mon = 3;
	}
	else if(mon <=9)
	{
		strMon = "��������";
		timeinfo->tm_mon = 6;
	}
	else
	{
		strMon = "���ļ���";
		timeinfo->tm_mon = 9;
	}
	timeinfo->tm_mday = 1;

	timeinfo->tm_hour = 0;
	timeinfo->tm_min = 0;
	timeinfo->tm_sec = 0;
	sendTime = mktime(timeinfo);
	

	int nQianDao = getSingedFen(fromGroup,fromQQ,sendTime);
	int nYaoQing = getYaoQingFen(fromGroup,fromQQ,sendTime);
	int nJiangFa = getJiangFaFen(fromGroup,fromQQ,sendTime);
	int nTotal   = getTotalFen(fromGroup,fromQQ,sendTime);

	int nAdd=0;
	int nExit=0;

	getYaoQingNum( fromGroup,  fromQQ, nAdd, nExit);

	char szMsg[1024]={0};
	sprintf_s(szMsg,"[CQ:at,qq=%s] \r\n��%s�����������:\r\n�ܻ��֣�%d\r\n������֣�%d\r\nǩ�����֣�%d\r\n���ͻ��֣�%d\r\n����������%d\r\n��Ⱥ������%d\r\n��Ч������%d\r\n",
		Int64ToString(fromQQ).data(),
		strMon.data(),
		nTotal,nYaoQing,nQianDao,nJiangFa,
		nAdd,nExit,nAdd-nExit
		);
	strMsg = szMsg;
	return strMsg;
}
#else

	{

	
	
	int64_t sendTimeC=sendTime;
	struct tm * timeinfo;
	timeinfo = localtime(&sendTimeC);
	

	int mon = timeinfo->tm_mon + 1;
	string strMsg = Int64ToString(mon).data();
	string strMon = "";

	
	timeinfo->tm_mon = 0;
	timeinfo->tm_mday = 1;
	timeinfo->tm_hour = 0;
	timeinfo->tm_min = 0;
	timeinfo->tm_sec = 0;
	sendTime = mktime(timeinfo);
	

	int nQianDao = getSingedFen(fromGroup,fromQQ,sendTime);
	int nYaoQing = getYaoQingFen(fromGroup,fromQQ,sendTime);
	int nJiangFa = getJiangFaFen(fromGroup,fromQQ,sendTime);
	int nTotal   = getTotalFen(fromGroup,fromQQ,sendTime);

	int nAdd=0;
	int nExit=0;

	getYaoQingNum( fromGroup,  fromQQ, nAdd, nExit);

	char szMsg[1024]={0};
	sprintf_s(szMsg,"[CQ:at,qq=%s] \r\n�ܻ���: %d\r\n100���ֶһ�1Ԫ��ֻ�����Ա�������Ʒ",
		Int64ToString(fromQQ).data(),		
		nTotal);
	strMsg = szMsg;
	return strMsg;
}
#endif
string CMessageHelper::SearchTime()
{

	string strMsg = "�������Ǳ���ʱ�䡿\r\n";

	strMsg += CMessageHelper::GetTimeString((int64_t)time(0));
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

	// string strModule = "Ⱥ����";
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

//�޸Ļ���
string CMessageHelper::ModifyJiFen(int64_t fromGroup, int64_t fromQQ,int nJifen,string strContent,int32_t sendTime,string strModule)
{
	char tmp[1024]={0};

	// string strModule = "Ⱥ����";
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
		sprintf_s(tmp," \r\n���ֱ仯��%d\r\n\r\n",nJifen);
		strMsg += tmp;

	}
	else
	{
		strMsg = "\r\n�����޸�ʧ�ܣ�\r\n";
	}


	strMsg += SearchJiFen(fromGroup,fromQQ,sendTime);





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


int CMessageHelper::getSingedFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime)
{
	int nJiFen = 0;
	string strSql ="";
	string strMoudle = "Ⱥǩ��";
	std_string_format(strSql,"SELECT sum(Award) FROM Record_Award where GroupQQ=%s and FromQQ=%s and Module='%s' and Time>%s;",
		(Int64ToString(fromGroup)).data(),
		(Int64ToString(fromQQ)).data(),
		strMoudle.data(),
		(Int64ToString(sendTime)).data());
	nJiFen =  GetJiFen(strSql);
	return nJiFen;
}
//�õ��������
int CMessageHelper::getYaoQingFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime)
{
	int nJiFen = 0;
	string strSql ="";
	string strMoudle = "Ⱥ����";	
	std_string_format(strSql,"SELECT sum(Award) FROM Record_Award where GroupQQ=%s and FromQQ=%s and Module='%s' and Time>%s;",
		(Int64ToString(fromGroup)).data(),
		(Int64ToString(fromQQ)).data(),
		strMoudle.data(),
		(Int64ToString(sendTime)).data());
	nJiFen =  GetJiFen(strSql);
	return nJiFen;
}
//�õ����ͻ���
int CMessageHelper::getJiangFaFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime)
{
	int nJiFen = 0;
	string strSql ="";
	string strMoudle = "Ⱥ����";
	std_string_format(strSql,"SELECT sum(Award) FROM Record_Award where GroupQQ=%s and FromQQ=%s and Module='%s' and Time>%s;",
		(Int64ToString(fromGroup)).data(),
		(Int64ToString(fromQQ)).data(),
		strMoudle.data(),
		(Int64ToString(sendTime)).data());
	nJiFen =  GetJiFen(strSql);
	return nJiFen;
}
//�õ��ܻ���
int CMessageHelper::getTotalFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime)
{
	int nJiFen = 0;
	string strSql ="";	

	std_string_format(strSql,"SELECT sum(Award) FROM Record_Award where GroupQQ=%s and FromQQ=%s and Time>%s;",(Int64ToString(fromGroup)).data(),(Int64ToString(fromQQ)).data(),
		(Int64ToString(sendTime)).data());
	nJiFen =  CMessageHelper::GetJiFen(strSql);
	return nJiFen;
}
//��ȡ��������
bool CMessageHelper::getYaoQingNum(int64_t fromGroup, int64_t fromQQ,int& nAdd,int& nExit)
{
	nAdd = 0;
	nExit = 0;
	string strSql ="";
	string strMoudle = "��Ⱥ";	
	std_string_format(strSql,"SELECT count(ID) FROM Record_Invite where GroupQQ=%s and FromQQ=%s and OpTypeInfo='%s';",(Int64ToString(fromGroup)).data(),(Int64ToString(fromQQ)).data(),strMoudle.data());
	nAdd =  CMessageHelper::GetJiFen(strSql);
	strMoudle = "��Ⱥ";	
	std_string_format(strSql,"SELECT count(ID) FROM Record_Invite where GroupQQ=%s and FromQQ=%s and OpTypeInfo='%s';",(Int64ToString(fromGroup)).data(),(Int64ToString(fromQQ)).data(),strMoudle.data());
	nExit =  CMessageHelper::GetJiFen(strSql);

	return true;
}

/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
string CMessageHelper::GroupMemberDecrease(int32_t subType, int32_t sendTime,
										   int64_t fromGroup, int64_t fromQQ, 
										   int64_t beingOperateQQ)
{
	string strSQL = "";
	string strMsg = "";

	std_string_format(strSQL,"select Award from Record_Invite where GroupQQ=%s and beingOperateQQ=%s and OpTypeInfo='��Ⱥ'",
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
		"��Ⱥ",
		""
		);
	execSql(strSQL);

	std_string_format(strMsg,"�������%s�˳��˱�Ⱥ�����ּ���%s����.\r\n",
		Int64ToString(beingOperateQQ).data(),
		Int64ToString(nJifen).data());





	ModifyJiFen(fromGroup,fromQQ,-nJifen,"",sendTime,"Ⱥ����");


	strMsg += SearchJiFen(fromGroup,fromQQ,sendTime);



	return strMsg; //


}
/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
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
		//Ⱥ��Ա����
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
		"��Ⱥ",
		Int64ToString(fromQQ).data()
		);
	execSql(strSQL);


	std_string_format(strMsg,"������[CQ:at,qq=%s] �����˱�Ⱥ����������%s����.\r\n ",
		Int64ToString(beingOperateQQ).data(),
		Int64ToString(nJifen).data(),
		SearchJiFen(fromGroup,fromQQ,sendTime).data());



	ModifyJiFen(fromGroup,fromQQ,nJifen,"",sendTime,"Ⱥ����");

	strMsg += SearchJiFen(fromGroup,fromQQ,sendTime);



	return strMsg; //
}

/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
*/
string CMessageHelper::RequestAddGroup(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, string msg,int64_t& InviteQQ)
{

	InviteQQ  = 0;
	string strSQL = "";
	string strMsg = "";
	if(subType == 1)
	{
		int64_t beingOperateQQ = fromQQ;

		//msg="�����ˣ�[CQ:at,qq=994868843]"
		int nFind = msg.find("������");
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
			"�����Ⱥ",
			Int64ToString(fromQQ).data()
			);
		execSql(strSQL);

		std_string_format(strMsg,"%s ���� %s ��Ⱥ,�����ĵȴ�����Ա���\r\n",
			Int64ToString(fromQQ).data(),
			Int64ToString(beingOperateQQ).data());
		return strMsg;

	}
	return "";



}