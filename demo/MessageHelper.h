#pragma once
#include "stdint.h"
#include "string.h"




using namespace std;


#define OPP_SINGE       "签到"
#define OPP_KICK        "踢"
#define OPP_BAN         "禁言"
#define SEARCH_CUR_TIME  "查询时间"
#define SEARCH_TOTAL_JIFEN "查询积分"

#define JI_FEN "积分"

#define MSG_TAIL "欢迎加入我们的宝妈宝爸购物群：542975091"

class CMessageHelper
{
public:
	CMessageHelper(void);
	~CMessageHelper(void);
public:
	bool InitData(); //初始化数据
	static bool IsGroupID(int64_t groupID);//是否是被监控群
	static bool IsMangerID(int64_t groupID,int64_t QQ);//是否是指定群的管理员
	bool IsLoadID(int64_t QQ);//是否是酷Q号

public:
	int64_t m_nGroupID;  //监控群QQ号
	int64_t m_nMangerID; //管理员QQ号
	int64_t m_nLoadID;  //酷Q号


public:


	static void ShowMsg(const char* msg);
	static string GetTimeString(int32_t time,int nType=0);
	//查询
	static string SearchTime();
	//查询积分
	static string SearchJiFen(int64_t fromGroup, int64_t fromQQ);
	//操作
	static string OppSinged(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);
	//修改积分
	static string ModifyJiFen(int64_t fromGroup, int64_t fromQQ,int nJifen,string strMsg,int32_t sendTime,string strModule);
	//插入签到记录
	static bool InsertSingedRecord(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);

	static  bool getAtMember(string strMsg,int64_t& atQQ,string& strContent);

	//得到一次签到积分
	static int getRandSingedJiFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime,int64_t &singedTime,int &nMin,int &nMax);
	//得到签到名次 bTotal=true时，获取的是当天总签到人数
	static int getSingedSort(int64_t fromGroup, int64_t fromQQ,int64_t sendTime,bool bTotal=false);
	//得到签到积分
	static int getSingedFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);
	//得到邀请积分
	static int getYaoQingFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);
	//得到奖惩积分
	static int getJiangFaFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);
	//得到总积分
	static int getTotalFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);
	//获取邀请人数
	static bool getYaoQingNum(int64_t fromGroup, int64_t fromQQ,int& nAdd,int& nExit);

	static int GetJiFen(string strSQL);

	static int64_t GetIni64tVaule(string strSQL);

	static string GroupMemberDecrease(int32_t subType, int32_t sendTime,
		int64_t fromGroup, int64_t fromQQ, 
		int64_t beingOperateQQ);
	static string GroupMemberIncrease(int32_t subType, int32_t sendTime, 
		int64_t fromGroup, int64_t fromQQ,
		int64_t beingOperateQQ);
	static string RequestAddGroup(int32_t subType, int32_t sendTime, int64_t fromGroup, 
		int64_t fromQQ, string strMsg,int64_t& InviteQQ);



	static bool AddUser(const string& sName, const string& sAge);


	static bool openDB();
	static void closeDB();
	static string Int64ToString(int64_t num);
	static string GBKToUTF8(const std::string& strGBK) ;
	static string & std_string_format(string & _str, const char * _Format, ...);
	static bool execSql(string strSQL);
	//返回查询结果个数
	static int GetRecordCount(string strSQL);


};

