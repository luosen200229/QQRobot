#pragma once
#include "stdint.h"
#include "string.h"




using namespace std;


#define OPP_SINGE       "ǩ��"
#define OPP_KICK        "��"
#define OPP_BAN         "����"
#define SEARCH_CUR_TIME  "��ѯʱ��"
#define SEARCH_TOTAL_JIFEN "��ѯ����"

#define JI_FEN "����"

#define MSG_TAIL "��ӭ�������ǵı��豦�ֹ���Ⱥ��542975091"

class CMessageHelper
{
public:
	CMessageHelper(void);
	~CMessageHelper(void);
public:
	bool InitData(); //��ʼ������
	static bool IsGroupID(int64_t groupID);//�Ƿ��Ǳ����Ⱥ
	static bool IsMangerID(int64_t groupID,int64_t QQ);//�Ƿ���ָ��Ⱥ�Ĺ���Ա
	bool IsLoadID(int64_t QQ);//�Ƿ��ǿ�Q��

public:
	int64_t m_nGroupID;  //���ȺQQ��
	int64_t m_nMangerID; //����ԱQQ��
	int64_t m_nLoadID;  //��Q��


public:


	static void ShowMsg(const char* msg);
	static string GetTimeString(int32_t time,int nType=0);
	//��ѯ
	static string SearchTime();
	//��ѯ����
	static string SearchJiFen(int64_t fromGroup, int64_t fromQQ);
	//����
	static string OppSinged(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);
	//�޸Ļ���
	static string ModifyJiFen(int64_t fromGroup, int64_t fromQQ,int nJifen,string strMsg,int32_t sendTime,string strModule);
	//����ǩ����¼
	static bool InsertSingedRecord(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);

	static  bool getAtMember(string strMsg,int64_t& atQQ,string& strContent);

	//�õ�һ��ǩ������
	static int getRandSingedJiFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime,int64_t &singedTime,int &nMin,int &nMax);
	//�õ�ǩ������ bTotal=trueʱ����ȡ���ǵ�����ǩ������
	static int getSingedSort(int64_t fromGroup, int64_t fromQQ,int64_t sendTime,bool bTotal=false);
	//�õ�ǩ������
	static int getSingedFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);
	//�õ��������
	static int getYaoQingFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);
	//�õ����ͻ���
	static int getJiangFaFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);
	//�õ��ܻ���
	static int getTotalFen(int64_t fromGroup, int64_t fromQQ,int64_t sendTime);
	//��ȡ��������
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
	//���ز�ѯ�������
	static int GetRecordCount(string strSQL);


};

