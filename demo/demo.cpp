// demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "demo.h"
#include "string.h"
#include "MessageHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			string strPath = "d:\\test.db";
			//CMessageHelper::InitDb(strPath);
			//string strMsg = CMessageHelper::ModifyJiFen(542975091,2523644894,10,"签到奖励",int64_t(time(0)),"群奖惩");

			string strMsg = CMessageHelper::OppSinged(542975091,2523644892,int64_t(time(0)));

			_tprintf(strMsg.data());
			//int nSort = CMessageHelper::getSingedSort(542975091,2523644894,int64_t(time(0)),true);

			/*
			int64_t groupID = 257386369;
			int64_t fromQQ =2523644894;

			string strMsg = ""; 
			
			if(CMessageHelper::IsGroupID(groupID))
			{
				_tprintf("被监视群\r\n");
			}
			else
			{
				_tprintf("非被监视群\r\n");
			}
			if(CMessageHelper::IsMangerID(groupID,fromQQ))
			{
				_tprintf("管理员QQ\r\n");
			}
			else
			{
				_tprintf("非管理员QQ\r\n");
			}
			
			_tprintf("\r\n");
			
			strMsg = CMessageHelper::OppSinged(groupID,fromQQ,int64_t(time(0)));
			*/
			
		/*
			int32_t subType=1;
			int32_t sendTime=time(0);
			int64_t fromGroup = 257386369;
			int64_t fromQQ = 3470665054;
			int64_t beingOperateQQ = 3470665054;
			string msg = "邀请人：[CQ:at,qq=278951765]";
			int64_t InviteQQ=0;

			string strMsg;

			strMsg = CMessageHelper::RequestAddGroup(subType,sendTime,fromGroup,fromQQ,msg.data(),InviteQQ);
			 _tprintf(strMsg.data());

			
			subType = 1;
			strMsg = CMessageHelper::GroupMemberIncrease(subType,sendTime,fromGroup,fromQQ,beingOperateQQ);
		    _tprintf(strMsg.data());

			strMsg = CMessageHelper::GroupMemberDecrease(subType,sendTime,fromGroup,fromQQ,beingOperateQQ);
			_tprintf(strMsg.data());
		*/
			getchar();

			// TODO: 在此处为应用程序的行为编写代码。
		}
	}
}
	
