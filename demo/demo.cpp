// demo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "demo.h"
#include "string.h"
#include "MessageHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: ���Ĵ�������Է���������Ҫ
			_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
			nRetCode = 1;
		}
		else
		{
			string strPath = "d:\\test.db";
			//CMessageHelper::InitDb(strPath);
			//string strMsg = CMessageHelper::ModifyJiFen(542975091,2523644894,10,"ǩ������",int64_t(time(0)),"Ⱥ����");

			string strMsg = CMessageHelper::OppSinged(542975091,2523644892,int64_t(time(0)));

			_tprintf(strMsg.data());
			//int nSort = CMessageHelper::getSingedSort(542975091,2523644894,int64_t(time(0)),true);

			/*
			int64_t groupID = 257386369;
			int64_t fromQQ =2523644894;

			string strMsg = ""; 
			
			if(CMessageHelper::IsGroupID(groupID))
			{
				_tprintf("������Ⱥ\r\n");
			}
			else
			{
				_tprintf("�Ǳ�����Ⱥ\r\n");
			}
			if(CMessageHelper::IsMangerID(groupID,fromQQ))
			{
				_tprintf("����ԱQQ\r\n");
			}
			else
			{
				_tprintf("�ǹ���ԱQQ\r\n");
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
			string msg = "�����ˣ�[CQ:at,qq=278951765]";
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

			// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		}
	}
}
	
