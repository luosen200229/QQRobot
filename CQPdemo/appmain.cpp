/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载

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

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;

bool g_bSinge = false;





/* 
* 返回应用的ApiVer、Appid，打包后将不会调用
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* 接收应用AuthCode，酷Q读取应用信息后，如果接受该应用，将会调用这个函数并传递AuthCode。
* 不要在本函数处理其他任何代码，以免发生异常情况。如需执行初始化代码请在Startup事件中执行（Type=1001）。
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 酷Q启动
* 无论本应用是否被启用，本函数都会在酷Q启动后执行一次，请在这里执行应用初始化代码。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	


	


	return 0;
}


/*
* Type=1002 酷Q退出
* 无论本应用是否被启用，本函数都会在酷Q退出前执行一次，请在这里执行插件关闭代码。
* 本函数调用完毕后，酷Q将很快关闭，请不要再通过线程等方式执行其他代码。
*/
CQEVENT(int32_t, __eventExit, 0)() {


	
	return 0;
}

/*
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
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
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 私聊消息
* subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {

	//如果要回复消息，请调用酷Q方法发送，并且这里 return EVENT_BLOCK - 截断本条消息，不再继续处理  注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
	//如果不回复消息，交由之后的应用/过滤器处理，这里 return EVENT_IGNORE - 忽略本条消息
	//string strMsg = msg;
	//CQ_sendPrivateMsg(ac,fromQQ,strMsg.data());
#ifdef USER_LS
	if(fromQQ == g_MangerQQ)
	{
		string strMsg="";
		if(strcmp("开始签到",msg) == 0)
		{
			g_bStarSinged = true;
			strMsg = "今天可以开始签到啦！\r\n\r\n";
			strMsg += CMessageHelper::SearchTime();
			CQ_sendGroupMsg(ac,542975091,strMsg.data());
		}
		
		if(strcmp("停止签到",msg) == 0)
		{
			g_bStarSinged = false;
			strMsg = "今天签到至此结束，明天再来吧！\r\n\r\n";			
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
			strMsg = "【" + CMessageHelper::Int64ToString(fromQQ) + "】此商品不参与10%积分奖励$\r\n" + strMsg;
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
    time (&t);//获取Unix时间戳。
    return t;
}
  
/*
* Type=2 群消息
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {



	
	g_MessageHelper.InitData();

	//只要能接收到群消息，说明酷Q已开启，可以开始签到
	if(!g_bSinge)
	{
		g_bSinge = true;
		string strMsg="";		
		g_bStarSinged = true;
		strMsg = "今天可以开始签到啦！\r\n\r\n";
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
		int nFirst = strMsgTemp.find("【");
		int nSecond = strMsgTemp.find("】");

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
					strMsgTemp = "恭喜您，帮你找到了优惠券\r\n是否有积分奖励，请联系群管理员！2523644894 \r\n" + strMsgTemp;
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
	
	//查询时间
	if(strcmp(SEARCH_CUR_TIME,msg) == 0)
	{
		strMsg += CMessageHelper::SearchTime();
		CQ_sendGroupMsg(ac,fromGroup,strMsg.data());
	}
	//签到
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
			strMsg = "今天签到至此结束，明天再来吧！\r\n\r\n";
			CQ_sendGroupMsg(ac,fromGroup,strMsg.data());
		}
		
	}
	//查询积分
	else if(strcmp(SEARCH_TOTAL_JIFEN,msg)==0 || strcmp("积分",msg)==0 )
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
				 //踢
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
								 sprintf_s(tmp,"本次积分增加：%d\r\n",nAdd);

								 strMsg += tmp;

								 //修改积分
	
								 CMessageHelper::ModifyJiFen(fromGroup,atQQ,nAdd,"",sendTime,"群奖惩");
							 }
							 else if(nAdd < 0)
							 {
							
								  memset(tmp,0,1024);
								 sprintf_s(tmp,"本次积分扣除：%d\r\n",-nAdd);
								 strMsg += tmp;

								 CMessageHelper::ModifyJiFen(fromGroup,atQQ,nAdd,"",sendTime,"群奖惩");
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
	
		CQ_sendPrivateMsg(ac,fromQQ,"管理员");
		
	}
	else
	{
		CQ_sendPrivateMsg(ac,fromQQ,"群成员");
	}
	*/
	
	
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=4 讨论组消息
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=101 群事件-管理员变动
* subType 子类型，1/被取消管理员 2/被设置管理员
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=102 群事件-群成员减少
* subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
* fromQQ 操作者QQ(仅subType为2、3时存在)
* beingOperateQQ 被操作QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	sendTime = GetCurrentTimeTotal();
	string strMsg = CMessageHelper::GroupMemberDecrease(subType,sendTime,fromGroup,fromQQ,beingOperateQQ);

	CQ_sendGroupMsg(ac,fromGroup,strMsg.data());
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=103 群事件-群成员增加
* subType 子类型，1/管理员已同意 2/管理员邀请
* fromQQ 操作者QQ(即管理员QQ)
* beingOperateQQ 被操作QQ(即加群的QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	sendTime = GetCurrentTimeTotal();
	if(CMessageHelper::IsGroupID(fromGroup))
	{
		string strMsg = CMessageHelper::GroupMemberIncrease(subType,sendTime,fromGroup,fromQQ,beingOperateQQ);


	
		CQ_sendGroupMsg(ac,fromGroup,strMsg.data());
	}
	
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=201 好友事件-好友已添加
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=301 请求-好友添加
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
* responseFlag 反馈标识(处理请求用)
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
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

/*
* 菜单，可在 .json 文件中设置菜单数目、函数名
* 如果不使用菜单，请在 .json 及此处删除无用菜单
*/
CQEVENT(int32_t, __ConifgSet, 0)() {
	//MessageBoxA(NULL, "这是menuA，在这里载入窗口，或者进行其他工作。", "" ,0);
	string strMsg = CQ_getAppDirectory(ac);

	MessageBoxA(NULL, strMsg.data(), "" ,0);
	
	return 0;
}


