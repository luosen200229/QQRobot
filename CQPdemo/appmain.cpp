/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����

#include "MessageHelper.h"

#include <time.h>

CMessageHelper  g_MessageHelper;
bool g_bStarSinged = true;

int64_t g_LastfromQQ = 0;
#ifdef USER_LS 
int64_t g_GroupQQ = 542975091;
int64_t g_MangerQQ = 2523644894;
#else
int64_t g_GroupQQ = 10150994;
int64_t g_MangerQQ = 150438860;
#endif




extern  string  g_strDBPath;
	



using namespace std;

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;

bool g_bSinge = false;





/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	


	


	return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {


	
	return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;

#ifndef _DEBUG

	char szMsg[1024] = {0};
	sprintf_s(szMsg,"%s",CQ_getAppDirectory(ac));

	g_strDBPath = szMsg;
	g_strDBPath += "com.luosen200229.sq.db";
	//CMessageHelper::ShowMsg(g_strDBPath.data());


#endif // !_DEBUG
	
	return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {

	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
	//string strMsg = msg;
	//CQ_sendPrivateMsg(ac,fromQQ,strMsg.data());
#ifdef USER_LS
	if(fromQQ == g_MangerQQ)
	{
		string strMsg="";
		if(strcmp("��ʼǩ��",msg) == 0)
		{
			g_bStarSinged = true;
			strMsg = "������Կ�ʼǩ������\r\n\r\n";
			strMsg += CMessageHelper::SearchTime();
			CQ_sendGroupMsg(ac,542975091,strMsg.data());
		}
		
		if(strcmp("ֹͣǩ��",msg) == 0)
		{
			g_bStarSinged = false;
			strMsg = "����ǩ�����˽��������������ɣ�\r\n\r\n";			
			CQ_sendGroupMsg(ac,g_GroupQQ,strMsg.data());
		}
		
	}
	else
	{
#ifdef USER_LS
		string strMsg = msg;
		if(strMsg.find("http://") != -1 || strMsg.find("https://") != -1)
		{
			g_LastfromQQ = fromQQ;
			strMsg = "��" + CMessageHelper::Int64ToString(fromQQ) + "������Ʒ������10%���ֽ���$\r\n" + strMsg;
			CQ_sendGroupMsg(ac,154213656,strMsg.data());
			CQ_sendPrivateMsg(ac,g_MangerQQ,strMsg.data());
		}
#endif
	}
#endif
	return EVENT_IGNORE;
	

}

int32_t GetCurrentTimeTotal()
{
	time_t t;    
    time (&t);//��ȡUnixʱ�����
    return t;
}
  
/*
* Type=2 Ⱥ��Ϣ
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {



	
	g_MessageHelper.InitData();

	//ֻҪ�ܽ��յ�Ⱥ��Ϣ��˵����Q�ѿ��������Կ�ʼǩ��
	if(!g_bSinge)
	{
		g_bSinge = true;
		string strMsg="";		
		g_bStarSinged = true;
		strMsg = "������Կ�ʼǩ������\r\n\r\n";
		strMsg += CMessageHelper::SearchTime();
		CQ_sendGroupMsg(ac,g_GroupQQ,strMsg.data());		
	}

	
	


	//CMessageHelper::ShowMsg(msg);
	if(!g_MessageHelper.IsGroupID(fromGroup))
	{
		return EVENT_IGNORE;
	}

#ifdef USER_LS
	{
		string strMsgTemp = msg;
		int nFirst = strMsgTemp.find("��");
		int nSecond = strMsgTemp.find("��");

		if(nFirst != -1 && nSecond != -1 && nFirst + 3 < nSecond)
		{
		
			strMsgTemp = strMsgTemp.substr(nFirst + 2,nSecond - nFirst - 2);
			
			string strLastQQ = CMessageHelper::Int64ToString(g_LastfromQQ);
			if(strMsgTemp.compare(strLastQQ) == 0)
			{
				strMsgTemp = msg;
				nSecond = strMsgTemp.find("$");
				if(nSecond >= 0)
				{
					strMsgTemp = strMsgTemp.substr(nSecond + 1);
					strMsgTemp = "��ϲ���������ҵ����Ż�ȯ\r\n�Ƿ��л��ֽ���������ϵȺ����Ա��2523644894 \r\n" + strMsgTemp;
					CQ_sendPrivateMsg(ac,g_LastfromQQ,strMsgTemp.data());
					CQ_sendPrivateMsg(ac,g_MangerQQ,msg);

				}
				
			}

		}
	}
#endif
	


	//CMessageHelper::ShowMsg("aaa");
	char tmp[1024]={0}; 
	
	

	sendTime = GetCurrentTimeTotal();
	string strMsg;
	
	//��ѯʱ��
	if(strcmp(SEARCH_CUR_TIME,msg) == 0)
	{
		strMsg += CMessageHelper::SearchTime();
		CQ_sendGroupMsg(ac,fromGroup,strMsg.data());
	}
	//ǩ��
	else if(strcmp(OPP_SINGE,msg) == 0)
	{
		if(g_bStarSinged)
		{
			strMsg += CMessageHelper::OppSinged(fromGroup,fromQQ,sendTime);
			CQ_sendGroupMsg(ac,fromGroup,strMsg.data());
			CQ_sendPrivateMsg(ac,fromQQ,strMsg.data());
		}
		else
		{
			strMsg = "����ǩ�����˽��������������ɣ�\r\n\r\n";
			CQ_sendGroupMsg(ac,fromGroup,strMsg.data());
		}
		
	}
	//��ѯ����
	else if(strcmp(SEARCH_TOTAL_JIFEN,msg)==0 || strcmp("����",msg)==0 )
	{
		strMsg += CMessageHelper::SearchJiFen(fromGroup,fromQQ,sendTime);
		CQ_sendGroupMsg(ac,fromGroup,strMsg.data());
		CQ_sendPrivateMsg(ac,fromQQ,strMsg.data());
	}
	else 
	{
		if(g_MessageHelper.IsMangerID(fromGroup,fromQQ))
		{
			int64_t atQQ = 0;
		     string strContent;
			 if(CMessageHelper::getAtMember(msg,atQQ,strContent))
			 {
				 //��
				 if(strcmp(strContent.data(),OPP_KICK)==0)
				{
					
					CQ_sendPrivateMsg(ac,atQQ,strContent.data());
					CQ_setGroupKick(ac,fromGroup,atQQ,false);
				}
				 else if(strcmp(strContent.data(),OPP_BAN)==0)
				{
					
					CQ_sendPrivateMsg(ac,atQQ,strContent.data());
					CQ_setGroupBan(ac,fromGroup,atQQ,10000);
				}
				 else 
				 {
					 string strJiFen = JI_FEN;
					 int nFlagLength =  strJiFen.length();
					 if(strContent.length()>=nFlagLength)
					 {
						 string strFlag = strContent.substr(0,nFlagLength);
						 if(strcmp(strFlag.data(),strJiFen.data())==0)
						 {

							 string strAdd = strContent.substr(nFlagLength);
							 int nAdd = atoi(strAdd.data());
							
							 if(nAdd > 0)
							 {
								 

								 memset(tmp,0,1024);
								 sprintf_s(tmp,"���λ������ӣ�%d\r\n",nAdd);

								 strMsg += tmp;

								 //�޸Ļ���
	
								 CMessageHelper::ModifyJiFen(fromGroup,atQQ,nAdd,"",sendTime,"Ⱥ����");
							 }
							 else if(nAdd < 0)
							 {
							
								  memset(tmp,0,1024);
								 sprintf_s(tmp,"���λ��ֿ۳���%d\r\n",-nAdd);
								 strMsg += tmp;

								 CMessageHelper::ModifyJiFen(fromGroup,atQQ,nAdd,"",sendTime,"Ⱥ����");
							 }

							 strMsg += CMessageHelper::SearchJiFen(fromGroup,atQQ,sendTime);
							 CQ_sendGroupMsg(ac,fromGroup,strMsg.data());
							 CQ_sendPrivateMsg(ac,atQQ,strMsg.data());
							
							 

						 }

					 }
				 }
				 
			
			}
		}
		
		
	}
	

	

	
	/*
	
	if(g_MessageHelper.IsMangerID(fromGroup,fromQQ))
	{
	
		CQ_sendPrivateMsg(ac,fromQQ,"����Ա");
		
	}
	else
	{
		CQ_sendPrivateMsg(ac,fromQQ,"Ⱥ��Ա");
	}
	*/
	
	
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=4 ��������Ϣ
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=101 Ⱥ�¼�-����Ա�䶯
* subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	sendTime = GetCurrentTimeTotal();
	string strMsg = CMessageHelper::GroupMemberDecrease(subType,sendTime,fromGroup,fromQQ,beingOperateQQ);

	CQ_sendGroupMsg(ac,fromGroup,strMsg.data());
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	sendTime = GetCurrentTimeTotal();
	if(CMessageHelper::IsGroupID(fromGroup))
	{
		string strMsg = CMessageHelper::GroupMemberIncrease(subType,sendTime,fromGroup,fromQQ,beingOperateQQ);


	
		CQ_sendGroupMsg(ac,fromGroup,strMsg.data());
	}
	
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=201 �����¼�-���������
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	sendTime = GetCurrentTimeTotal();

	if(CMessageHelper::IsGroupID(fromGroup))
	{
		string strMsg="";
		string strInfo=msg;
		int64_t InviteQQ=0;
		strMsg = CMessageHelper::RequestAddGroup(subType,sendTime,fromGroup,fromQQ,strInfo,InviteQQ);
		CQ_sendPrivateMsg(ac,InviteQQ,strMsg.data());
	}
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

/*
* �˵������� .json �ļ������ò˵���Ŀ��������
* �����ʹ�ò˵������� .json ���˴�ɾ�����ò˵�
*/
CQEVENT(int32_t, __ConifgSet, 0)() {
	//MessageBoxA(NULL, "����menuA�����������봰�ڣ����߽�������������", "" ,0);
	string strMsg = CQ_getAppDirectory(ac);

	MessageBoxA(NULL, strMsg.data(), "" ,0);
	
	return 0;
}


