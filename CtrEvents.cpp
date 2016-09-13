// CtrEvents.cpp: ���������� ����� ����� ��� ����������� ����������.
//
#include "stdafx.h"
#include <string>
#include <locale.h>
#include "ZGuard.h"
#include "ZPort.h"
#include <iostream>
#include <Windows.h>
#include <WinInet.h>

#pragma comment (lib, "Wininet.lib")
#pragma comment (lib, "urlmon.lib")
#pragma warning(disable : 4996)

#define POST 1
#define GET 0
#define PORT 3000

using namespace std;

#define SERVER_ADDR "192.168.1.135"

const ZP_PORT_TYPE CvtPortType = ZP_PORT_COM;
LPCWSTR CvtPortName = L"COM10";
const INT CtrAddr = 2;

#if !defined(ZGUARD_LINKONREQUEST)
#pragma comment(lib, "../ZGuard.lib")
#elif !defined(ZPORT_LINKONREQUEST)
#pragma comment(lib, "../ZGuard.lib")
#endif // !ZGUARD_LINKONREQUEST
#include "Utils.h"


LPCTSTR EvTypeStrs[] = {
	TEXT(""),
	TEXT("Open button"),				// ������� ������� �������
	TEXT("Key not found"),				// ���� �� ������ � ����� ������
	TEXT("Open key"),					// ���� ������, ����� �������
	TEXT("Key unscheduled"),			// ���� ������, ������ �� ��������
	TEXT("Open RS485"),					// ������� ���������� �� ����
	TEXT("Door is blocked (key)"),		// ���� ������, ����� �������������
	TEXT("Door is blocked (button)"),	// ������� ������� ��������������� ����� �������
	TEXT("Door is broken"),				// ����� ��������
	TEXT("Door is left open"),			// ����� ��������� �������� (timeout)
	TEXT("Passage"),					// ������ ���������
	TEXT("Sensor 1"),					// �������� ������ 1
	TEXT("Sensor 2"),					// �������� ������ 2
	TEXT("Controller is rebooted"),		// ������������ �����������
	TEXT("Button is disabled"),			// ������������� ������ ����������
	TEXT("Double pass"),				// ������� �������� �������
	TEXT("Door opened"),				// ����� ������� ������
	TEXT("Door closed"),				// ����� �������
	TEXT("Power off"),					// ������� �������
	TEXT("Power is on (ElectroControl)"),	// ��������� ��������������
	TEXT("Power is off (ElectroControl)"),	// ��������� ��������������
	TEXT("Enable the lock (trigger)"),		// ��������� ����� (�������)
	TEXT("Disable the lock (trigger)"),	// ���������� ����� (�������)
	TEXT("Changed state Mode"),			// ��������� ��������� �����
	TEXT("Changed state Fire"),			// ��������� ��������� ������
	TEXT("Changed state Security"),		// ��������� ��������� ������
	TEXT("Unknown key"),				// ����������� ����
	TEXT("Passage in the gateway"),		// �������� ���� � ����
	TEXT("Blocked the entrance to the gateway (busy)"),		// ������������ ���� � ���� (�����)
	TEXT("Allowed to enter the gateway"),	// �������� ���� � ����
	TEXT("AntiPassBack"),				// ������������ ������ (�����������)
	TEXT("Hotel40"),
	TEXT("Hotel41")
};

LPCTSTR DirectStrs[] = {
	TEXT(""),
	TEXT("IN"),   // ����
	TEXT("OUT")   // �����
};

LPCTSTR EcSubEvStrs[] = {
	TEXT(""),
	TEXT("CARD_DELAY"),			// ��������� �������� ����� � ������ ������� (��� �����) �������� ��������
	TEXT("RESERVED1"),			// (���������������)
	TEXT("ON_NET"),				// �������� �������� �� ����
	TEXT("OFF_NET"),			// ��������� �������� �� ����
	TEXT("ON_SCHED"),			// �������� �� ��������� ����
	TEXT("OFF_SHED"),			// ��������� �� ��������� ����
	TEXT("CARD"),				// ��������� �������� ����� � ������������ ����������
	TEXT("RESERVED2"),			// (���������������)
	TEXT("OFF_TIMEOUT"),		// ��������� ����� ��������� ��������
	TEXT("OFF_EXIT")			// ��������� �� ������������ ������� ������
};

LPCTSTR FireSubEvStrs[] = {
	TEXT(""),
	TEXT("OFF_NET"),			// ��������� �� ����
	TEXT("ON_NET"),				// �������� �� ����
	TEXT("OFF_INPUT_F"),		// ��������� �� ����� FIRE
	TEXT("ON_INPUT_F"),			// �������� �� ����� FIRE
	TEXT("OFF_TEMP"),			// ��������� �� ������� �����������
	TEXT("ON_TEMP")				// �������� �� ������� �����������
};

LPCTSTR SecurSubEvStrs[] = {
	TEXT(""),
	TEXT("OFF_NET"),			// ��������� �� ����
	TEXT("ON_NET"),				// �������� �� ����
	TEXT("OFF_INPUT_A"),		// ��������� �� ����� ALARM
	TEXT("ON_INPUT_A"),			// �������� �� ����� ALARM
	TEXT("OFF_TAMPERE"),		// ��������� �� �������
	TEXT("ON_TAMPERE"),			// �������� �� �������
	TEXT("OFF_DOOR"),			// ��������� �� ������� �����
	TEXT("ON_DOOR")				// �������� �� ������� �����
};

LPCTSTR ModeSubEvStrs[] = {
	TEXT(""),
	TEXT("RS485 allow"),		// ��������� �������� �� ����
	TEXT("RS485 denied"),		// �������� ��������� �� ����
	TEXT("Timezone start"),			// �������� ��������� ����
	TEXT("Timezone finish"),			// ���������� ��������� ����
	TEXT("Card allow"),			// ��������� ������
	TEXT("Card denied")			// �������� ��������� ������
};

LPCTSTR ModeStrs[] = {
	TEXT(""),
	TEXT("Norm"),				// �����
	TEXT("Block"),				// ����������
	TEXT("Free"),				// ���������
	TEXT("Wait")				// ��������
};

LPCTSTR HModeStrs[] = {
	TEXT(""),
	TEXT("Norm"),				// �����
	TEXT("Block"),				// ����������
	TEXT("Free"),				// ���������
	TEXT("???")					// ���������������
};

LPCTSTR HotelSubEvStrs[] = {
	TEXT(""),
	TEXT("FreeCard"),			// ����� ��������
	TEXT("BlockCard"),			// ����� �����������
	TEXT("DopFunc"),			// �������������� �������
	TEXT("NewRCard"),			// ������� ��������� �����
	TEXT("Network"),
	TEXT("Timezone"),
	TEXT("Counter"),			// �������� �������
	TEXT("CryptoKey"),			// �������� ����������
	TEXT("Pulse Z"),			// ����������� ������� � ������� 2� ������
	TEXT("State Change")		// ��������� ������� -���� ������ ����� � ��������� ����� ��� ����� 2 ������� 
};



//const ZP_PORT_TYPE CvtPortType = ZP_PORT_IP;
//LPCWSTR CvtPortName = L"90.11.11.56:1000";
//const INT CtrAddr = 4;

HANDLE g_hCtr = NULL;
INT g_nCtrMaxEvents;
BOOL g_fProximity;
UINT g_nCtrFlags;
BOOL g_fCtrNotifyEnabled;
INT g_nAppReadEventIdx;
HANDLE g_hEvent = NULL;
BOOL g_fThreadActive;
HANDLE g_hThread = NULL;

void GenerateDataRequest(char *data, char *event, char *id);

void ShowEvents(int nStart, int nCount)
{
	_ZG_CTR_EVENT aEvents[6];
	PZG_CTR_EVENT pEv;
	int i, j, nIdx, nCnt;

	i = 0;
	while (i < nCount)
	{
		nIdx = (nStart + i) % g_nCtrMaxEvents; 
		nCnt = (nCount - i);
		if (nCnt > _countof(aEvents))
			nCnt = _countof(aEvents);

		if ((nIdx + nCnt) > g_nCtrMaxEvents)
			nCnt = (g_nCtrMaxEvents - nIdx);

		if (!CheckZGError(ZG_Ctr_ReadEvents(g_hCtr, nIdx, aEvents, nCnt, NULL, NULL), _T("ZG_Ctr_ReadEvents")))
			return;
		for (j = 0; j < nCnt; j++)
		{
			pEv = &aEvents[j];
			switch (pEv->nType)
			{
			case ZG_EV_ELECTRO_ON:
			case ZG_EV_ELECTRO_OFF:
				{
					_ZG_EV_TIME rTime;
					ZG_EC_SUB_EV nSubEvent;
					DWORD nPowerFlags;
					ZG_Ctr_DecodeEcEvent(g_hCtr, pEv->aData, &rTime, &nSubEvent, &nPowerFlags);
					_tprintf(TEXT("%.4d. %.2d.%.2d %.2d:%.2d:%.2d %s Sub_event: %s, Power flags: %d\n"),
						nIdx + j,
						rTime.nDay, rTime.nMonth,
						rTime.nHour, rTime.nMinute, rTime.nSecond,
						EvTypeStrs[pEv->nType],
						EcSubEvStrs[nSubEvent], nPowerFlags);
				}
				break;
			case ZG_EV_FIRE_STATE:
				{
					_ZG_EV_TIME rTime;
					ZG_FIRE_SUB_EV nSubEvent;
					DWORD nFireFlags;
					ZG_Ctr_DecodeFireEvent(g_hCtr, pEv->aData, &rTime, &nSubEvent, &nFireFlags);
					_tprintf(TEXT("%.4d. %.2d.%.2d %.2d:%.2d:%.2d %s Sub_event: %s, Fire flags: %d\n"),
						nIdx + j,
						rTime.nDay, rTime.nMonth,
						rTime.nHour, rTime.nMinute, rTime.nSecond,
						EvTypeStrs[pEv->nType],
						FireSubEvStrs[nSubEvent], nFireFlags);
				}
				break;
			case ZG_EV_SECUR_STATE:
				{
					_ZG_EV_TIME rTime;
					ZG_SECUR_SUB_EV nSubEvent;
					DWORD nSecurFlags;
					ZG_Ctr_DecodeSecurEvent(g_hCtr, pEv->aData, &rTime, &nSubEvent, &nSecurFlags);
					_tprintf(TEXT("%.4d. %.2d.%.2d %.2d:%.2d:%.2d %s Sub_event: %s, Security flags: %.2Xh\n"),
						nIdx + j,
						rTime.nDay, rTime.nMonth,
						rTime.nHour, rTime.nMinute, rTime.nSecond,
						EvTypeStrs[pEv->nType],
						SecurSubEvStrs[nSubEvent], nSecurFlags);
				}
				break;
			case ZG_EV_MODE_STATE:
				{
					_ZG_EV_TIME rTime;
					ZG_CTR_MODE nMode;
					ZG_MODE_SUB_EV nSubEvent;
					ZG_Ctr_DecodeModeEvent(g_hCtr, pEv->aData, &rTime, &nMode, &nSubEvent);
					_tprintf(TEXT("%.4d. %.2d.%.2d %.2d:%.2d:%.2d %s Mode: %s, Sub_event: %s\n"),
						nIdx + j,
						rTime.nDay, rTime.nMonth,
						rTime.nHour, rTime.nMinute, rTime.nSecond,
						EvTypeStrs[pEv->nType],
						ModeStrs[nMode],
						ModeSubEvStrs[nSubEvent]);
				}
				break;
			case ZG_EV_UNKNOWN_KEY:
				{
					Z_KEYNUM rNum;
					ZG_Ctr_DecodeUnkKeyEvent(g_hCtr, pEv->aData, &rNum);
					_tprintf(TEXT("%.4d. Key \"%s\"\n"),
						nIdx + j,
						ZKeyNumToStr(rNum, g_fProximity).c_str());
				}
				break;
			case ZG_EV_HOTEL40:
			case ZG_EV_HOTEL41:
				{
					_ZG_EV_TIME rTime;
					ZG_HOTEL_MODE nMode;
					ZG_HOTEL_SUB_EV nSubEvent;
					DWORD nFlags;
					ZG_DecodeHotelEvent(pEv->aData, &rTime, &nMode, &nSubEvent, &nFlags);
					_tprintf(TEXT("%.4d. %.2d.%.2d %.2d:%.2d:%.2d %s Mode: %s, Sub_event: %s, flags: %.2Xh\n"),
						nIdx + j,
						rTime.nDay, rTime.nMonth,
						rTime.nHour, rTime.nMinute, rTime.nSecond,
						EvTypeStrs[pEv->nType],
						HModeStrs[nMode],
						HotelSubEvStrs[nSubEvent],
						nFlags);
				}
				break;
			default:
				{
					_ZG_EV_TIME rTime;
					ZG_CTR_DIRECT nDirect;
					INT nKeyIdx, nKeyBank;
					ZG_Ctr_DecodePassEvent(g_hCtr, pEv->aData, &rTime, &nDirect, &nKeyIdx, &nKeyBank);
					char date[64];
					char event[64];
					char id[64]; 
					
					sprintf(date, "%.2d.%.2d %.2d:%.2d:%.2d", rTime.nDay, rTime.nMonth,
						rTime.nHour, rTime.nMinute, rTime.nSecond);
					
					sprintf(event, "%s %s", DirectStrs[nDirect],
						EvTypeStrs[pEv->nType]);

					sprintf(id, "%d", nKeyIdx);
					
					GenerateDataRequest(date, event, id);
					
					_tprintf(TEXT("%.4d. %.2d.%.2d %.2d:%.2d:%.2d %s %s (key_idx: %d, bank#: %d)\n"),
						nIdx + j,
						rTime.nDay, rTime.nMonth,
						rTime.nHour, rTime.nMinute, rTime.nSecond,
						DirectStrs[nDirect],
						EvTypeStrs[pEv->nType],
						nKeyIdx, nKeyBank);
				
				}
				break;
			}
		}
		i += nCnt;
	}
}

HRESULT CheckNotifyMsgs()
{
	HRESULT hr;
	UINT nMsg;
	LPARAM nMsgParam;
	while ((hr = ZG_Ctr_GetNextMessage(g_hCtr, &nMsg, &nMsgParam)) == S_OK)
	{
		switch (nMsg)
		{
		case ZG_N_CTR_NEW_EVENT:
			{
				PZG_N_NEW_EVENT_INFO pInfo = PZG_N_NEW_EVENT_INFO(nMsgParam);

				_tprintf(TEXT("==> New Events: %d\n"), pInfo->nNewCount);
				ShowEvents(pInfo->nReadIdx, pInfo->nNewCount);
				g_nAppReadEventIdx = pInfo->nWriteIdx;
			}
			break;
		}
	}
	if (hr == ZP_S_NOTFOUND)
		hr = S_OK;
	return hr;
}

DWORD WINAPI NotifyThreadProc(LPVOID lpParameter)
{
	while (g_fThreadActive)
	{
		if (WaitForSingleObject(g_hEvent, INFINITE) == WAIT_OBJECT_0)
		{
			ResetEvent(g_hEvent);
			if (g_hCtr != NULL)
				CheckNotifyMsgs();
		}
	}
	return 0;
}

void StartNotifyThread()
{
	if (g_hThread != NULL)
		return;
	DWORD nThreadId;
	g_fThreadActive = TRUE;
	g_hThread = CreateThread(NULL, 0, NotifyThreadProc, NULL, 0, &nThreadId);
}

void StopNotifyThread()
{
	if (g_hThread == NULL)
		return;
	g_fThreadActive = FALSE;
	SetEvent(g_hEvent);
	WaitForSingleObject(g_hThread, INFINITE);
	CloseHandle(g_hThread);
	g_hThread = NULL;
}

void DoShowNewEvents()
{
	INT nWrIdx;
	if (!CheckZGError(ZG_Ctr_ReadEventIdxs(g_hCtr, &nWrIdx, NULL), _T("ZG_Ctr_ReadEventIdxs")))
		return;
	INT nNewCount;
	if (nWrIdx >= g_nAppReadEventIdx)
		nNewCount = (nWrIdx - g_nAppReadEventIdx);
	else
		nNewCount = (g_nCtrMaxEvents - g_nAppReadEventIdx + nWrIdx);

	if (nNewCount == 0)
		_tprintf(TEXT("No new events\n"));
	else
		_tprintf(TEXT("Available %d new events:\n"), nNewCount);
	INT nShowCount;
	while (nNewCount > 0)
	{
		nShowCount = min(nNewCount, 25);
		ShowEvents(g_nAppReadEventIdx, nShowCount);
		nNewCount -= nShowCount;
		g_nAppReadEventIdx = (g_nAppReadEventIdx + nShowCount) % g_nCtrMaxEvents;
		_tprintf(TEXT("Press Enter to continue or 'x' to break...\n"));
		char ch = getchar();
		if (ch == 'x')
		{
			_tprintf(TEXT("Aborted.\n"));
			return;
		}
	}
	_tprintf(TEXT("Done.\n"));
}

void DoShowAllEvents()
{
	INT nWrIdx;
	if (!CheckZGError(ZG_Ctr_ReadEventIdxs(g_hCtr, &nWrIdx, NULL), _T("ZG_Ctr_ReadEventIdxs")))
		return;
	INT nIdx, nTotalCount, nShowCount;

	nIdx = nWrIdx;
	nTotalCount = g_nCtrMaxEvents;
	while (nTotalCount > 0)
	{
		nShowCount = min(nTotalCount, 25);
		ShowEvents(nIdx, nShowCount);
		nTotalCount -= nShowCount;
		nIdx = (nIdx + nShowCount) % g_nCtrMaxEvents;
		_tprintf(TEXT("Press Enter to continue or 'x' to break...\n"));
		char ch = getchar();
		if (ch == 'x')
		{
			_tprintf(TEXT("Aborted.\n"));
			return;
		}
	}
	_tprintf(TEXT("Done.\n"));
}

void EnableNotification(BOOL fEnable=TRUE, BOOL fReport=TRUE)
{
	if (fEnable)
	{
		if (g_hEvent == NULL)
			g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		_ZG_CTR_NOTIFY_SETTINGS rNS = {0};
		rNS.nNMask = ZG_NF_CTR_NEW_EVENT;
		rNS.hEvent = g_hEvent;
		rNS.nReadEvIdx = g_nAppReadEventIdx;
		rNS.nCheckStatePeriod = 300;
		if (!CheckZGError(ZG_Ctr_SetNotification(g_hCtr, &rNS), _T("ZG_Ctr_SetNotification")))
			return;
		StartNotifyThread();
	}
	else
	{
		StopNotifyThread();
		if (!CheckZGError(ZG_Ctr_SetNotification(g_hCtr, NULL), _T("ZG_Ctr_SetNotification")))
			return;
	}
	g_fCtrNotifyEnabled = fEnable;
	if (fReport)
		_tprintf(TEXT("Done.\n"));
}

void DoRestoreFactorySettings()
{
	_tprintf(TEXT("Writing (0, 0)...\n"));
	if (!CheckZGError(ZG_Ctr_WriteEventIdxs(g_hCtr, 0x3, 0, 0), _T("ZG_Ctr_WriteEventIdxs")))
		return;
	g_nAppReadEventIdx = 0;
	_tprintf(TEXT("Done.\n"));
}

void DoCtrEventsMenu()
{
	HANDLE hCvt = 0;

	if (!CheckZGError(ZG_Initialize(ZP_IF_NO_MSG_LOOP), _T("ZG_Initialize")))
		return;
	__try
	{
		_ZG_CVT_OPEN_PARAMS rOp;
		ZeroMemory(&rOp, sizeof(rOp));
		rOp.nType = CvtPortType;
		rOp.pszName = CvtPortName;
		if (!CheckZGError(ZG_Cvt_Open(&hCvt, &rOp, NULL), _T("ZG_Cvt_Open")))
			return;
		_ZG_CTR_INFO rCtrInfo;
		ZeroMemory(&rCtrInfo, sizeof(rCtrInfo));
		if (!CheckZGError(ZG_Ctr_Open(&g_hCtr, hCvt, CtrAddr, 0, &rCtrInfo, ZG_CTR_UNDEF), _T("ZG_Ctr_Open")))
			return;
		g_nCtrMaxEvents = rCtrInfo.nMaxEvents;
		g_fProximity = (rCtrInfo.nFlags & ZG_CTR_F_PROXIMITY) != 0;
		g_nCtrFlags = rCtrInfo.nFlags;
		_tprintf(TEXT("%s addr: %d, s/n: %d, v%d.%d, Max_Events: %d, Key_Type: %s\n"),
			CtrTypeStrs[rCtrInfo.nType],
			rCtrInfo.nAddr,
			rCtrInfo.nSn,
			LOBYTE(rCtrInfo.nVersion), HIBYTE(rCtrInfo.nVersion),
			rCtrInfo.nMaxEvents,
			KeyModeStrs[g_fProximity ? 1 : 0]);
		g_fCtrNotifyEnabled = FALSE;
		INT nWrIdx, nRdIdx;
		if (!CheckZGError(ZG_Ctr_ReadEventIdxs(g_hCtr, &nWrIdx, &nRdIdx), _T("ZG_Ctr_ReadEventIdxs")))
			return;
		g_nAppReadEventIdx = nWrIdx;
		_tprintf(TEXT("-----\n"));
		EnableNotification(TRUE, FALSE);
		TCHAR szBuf[128];
		while (1)
		{
			_tprintf(TEXT("Enter command number:\n"));
			_tprintf(TEXT("1 - Show new events\n"));
			_tprintf(TEXT("2 - Show all events (%d)\n"), g_nCtrMaxEvents);
			_tprintf(TEXT("3 - %s notifications for new events\n"), (g_fCtrNotifyEnabled ? TEXT("Disable") : TEXT("Enable")));
			_tprintf(TEXT("9 - Restore factory settings (reset indexs)\n"));
			_tprintf(TEXT("0 - quit\n"));
			if (_tscanf_s(TEXT("%s"), szBuf, _countof(szBuf)) == 1)
			{
				_tprintf(TEXT("\n"));
				switch (_ttoi(szBuf))
				{
				case 1:
					DoShowNewEvents();
					break;
				case 2:
					DoShowAllEvents();
					break;
				case 3:
					EnableNotification(!g_fCtrNotifyEnabled);
					break;
				case 9:
					DoRestoreFactorySettings();
					break;
				case 0:
					return;
				default:
					_tprintf(TEXT("Invalid command.\n"));
				}
				_tprintf(TEXT("-----\n"));
			}
		}
	}
	__finally
	{
		StopNotifyThread();
		if (g_hCtr != NULL)
			ZG_CloseHandle(g_hCtr);
		if (hCvt != NULL)
			ZG_CloseHandle(hCvt);
		if (g_hEvent != NULL)
			CloseHandle(g_hEvent);
		ZG_Finalyze();
	}
}

void Request(int Method, LPCSTR Host, LPCSTR url, LPCSTR header, LPSTR data)
{
try{
	//Retrieve default http user agent
	char httpUseragent[512];
	DWORD szhttpUserAgent = sizeof(httpUseragent);
	ObtainUserAgentString( 0, httpUseragent, &szhttpUserAgent );
 
	char m[5];
 
	if(Method == GET)
		strcpy_s(m, "GET");
	else 
		strcpy_s(m, "POST");
 
	HINTERNET internet = InternetOpenA(httpUseragent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(internet != NULL)
	{
		HINTERNET connect = InternetConnectA(internet, Host, PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		if(connect != NULL)
		{
			HINTERNET request = HttpOpenRequestA(connect, m, url, "HTTP/1.1", NULL, NULL, 
				INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
					INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
					INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP  |
					INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
					INTERNET_FLAG_NO_AUTH |
					INTERNET_FLAG_NO_CACHE_WRITE |
					INTERNET_FLAG_NO_UI |
					INTERNET_FLAG_PRAGMA_NOCACHE |
					INTERNET_FLAG_RELOAD, NULL);

 
			if(request != NULL)
			{
				int datalen = 0;
				if(data != NULL) datalen = strlen(data);
				int headerlen = 0;
				if(header != NULL) headerlen = strlen(header);
 
				BOOL sent = HttpSendRequestA(request, header, headerlen, data, datalen);

				if(!sent)
				{
					printf("Error: HttpSendRequest " + GetLastError());
				}

				InternetCloseHandle(request);
			}
		}
		InternetCloseHandle(connect);
	}
	InternetCloseHandle(internet);
}
catch(...) {
}
}

void GenerateDataRequest(char *date, char *event, char *id){
	char data[1024];
	char* posturi = "{\"date\":\"%s\", \"event\":\"%s\", \"id\":\"%s\"}";
	
	wsprintfA( data, posturi,  date, event, id);
	Request(POST, SERVER_ADDR, "/event", "Content-Type: application/json", data);
}

int _tmain(int argc, char **argv)
{
	setlocale(LC_ALL, "Russian");
	CZGuardLoader oZGL;

	if (!oZGL.IsLoaded())
	{
		_tprintf(TEXT("Load ZGuard error 0x%X.\n"), oZGL.m_nStatus);
		getchar();
		return 0;
	}

	DoCtrEventsMenu();

	return 0;
}

