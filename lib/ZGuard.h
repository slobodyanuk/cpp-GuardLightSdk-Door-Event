#pragma once
// ����������� ���� ���� ifdef - ��� ����������� ����� �������� ��������, ���������� ��������� 
// �������� �� ��������� DLL. ��� ����� ������ DLL �������������� � �������������� ������� ZGUARD_EXPORTS,
// � ��������� ������. ���� ������ �� ������ ���� ��������� � �����-���� �������
// ������������ ������ DLL. ��������� ����� ����� ������ ������, ��� �������� ����� �������� ������ ����, ����� 
// ������� ZGUARD_API ��� ��������������� �� DLL, ����� ��� ������ DLL ����� �������,
// ������������ ������ ��������, ��� ����������������.

// (rom) this is the switch to change between static and dynamic linking.
// (rom) it is enabled by default here.
// (rom) To disable simply change the '$' to a '.'.
#define ZGUARD_LINKONREQUEST


#ifndef ZGUARD_LINKONREQUEST

#ifdef ZGUARD_EXPORTS
#define ZGUARD_API(type) extern "C" __declspec(dllexport) type __stdcall 
#else
#ifdef ZGUARD_STATIC
#define ZGUARD_API(type) extern "C" type __stdcall 
#else
#define ZGUARD_API(type) extern "C" __declspec(dllimport) type __stdcall
#endif
#endif

#endif // !ZGUARD_LINKONREQUEST

// ����������� ������ SDK
#define ZG_SDK_VER_MAJOR	3
#define ZG_SDK_VER_MINOR	33

#include "ZPort.h"
#include "ZBase.h"
#include "ZGErrors.h"

#pragma pack(1)   // turn byte alignment on

#define ZG_IF_LOG						0x100	// ���������� ���

#define ZG_DEVTYPE_GUARD				1
#define ZG_DEVTYPE_Z397					2
#define ZG_DEVTYPE_COM					5
#define ZG_DEVTYPE_IPGUARD				(ZP_MAX_REG_DEV)
#define ZG_DEVTYPE_CVTS					0x26
#define ZG_IPDEVTYPE_CVTS				1

#define ZG_MAX_LICENSES					16	// ������������ ���������� ��������, ������� ����� ���������� � ���������
#define ZG_DEF_CVT_LICN					5	// ����� �������� ���������� �� ���������
#define ZG_MAX_TIMEZONES				7	// �������� ��������� ���
#define ZG_MODES_TZ0					-2	// ����� ������ ��.���� ��� ������������ ������ �����������, ����� 2 ����
#define ZG_DUAL_ZONE_TZ0				-9	// ����� ������ ��.���� �� ������ ������, ����� 7 ���

// �������� �� ���������
#define ZG_CVT_SCANCTRSPERIOD		5000	// ������ ������������ ������������ (� �������������)
#define ZG_CVT_SCANCTRSLASTADDR		31		// ��������� ����������� ����� ����������� (��� ��������������� ������ ������������)

#ifndef ZR_DEVTYPE_READERS
// ������ ����������
enum ZG_CVT_TYPE : INT
{
	ZG_CVT_UNDEF = 0,		// �� ����������
	ZG_CVT_Z397,			// Z-397
	ZG_CVT_Z397_GUARD,		// Z-397 Guard
	ZG_CVT_Z397_IP,			// Z-397 IP
	ZG_CVT_Z397_WEB,		// Z-397 Web
	ZG_CVT_Z5R_WEB			// Z5R Web
};

// ����� ���������� Z397 Guard
enum ZG_GUARD_MODE : INT
{
	ZG_GUARD_UNDEF = 0,		// �� ����������
	ZG_GUARD_NORMAL,		// ����� "Normal" (�������� �������� ���������� Z397)
	ZG_GUARD_ADVANCED,		// ����� "Advanced"
	ZG_GUARD_TEST,			// ����� "Test" (��� ������������)
	ZG_GUARD_ACCEPT			// ����� "Accept" (��� ������������)
};

// ���������� � ����������, ������������ �������� ZG_SearchDevices
typedef struct _ZG_ENUM_IPCVT_INFO : _ZP_DEVICE_INFO
{
	ZG_CVT_TYPE nType;		// ������ IP-����������
	ZG_GUARD_MODE nMode;	// ����� ������ ���������� Guard
	DWORD nFlags;			// �����: ��� 0 - "VCP", ��� 1 - "WEB", 0xFF - "All"
} *PZG_ENUM_IPCVT_INFO;
#endif // !ZR_DEVTYPE_READERS

// �������� ����������
enum ZG_CVT_SPEED : UINT
{
	ZG_SPEED_19200	= 19200,
	ZG_SPEED_57600	= 57600
};

// ���������� � ����������, ������������ �������� ZG_SearchDevices
typedef struct _ZG_ENUM_CVT_INFO : _ZP_DEVICE_INFO
{
	ZG_CVT_TYPE nType;		// ��� ����������
	ZG_GUARD_MODE nMode;	// ����� ������ ���������� Guard
} *PZG_ENUM_CVT_INFO;


// ���������� � ����������, ������������ ���������: ZG_Cvt_Open, ZG_Cvt_AttachPort � ZG_Cvt_GetInformation
typedef struct _ZG_CVT_INFO : _ZP_DEVICE_INFO
{
	ZG_CVT_TYPE nType;		// ��� ����������
	ZG_CVT_SPEED nSpeed;	// �������� ����������

	ZG_GUARD_MODE nMode;	// ����� ������ ���������� Guard

	LPWSTR pszLinesBuf;		// ����� ��� �������������� �����
	INT nLinesBufMax;		// ������ ������ � ��������, ������� ����������� '\0'
} *PZG_CVT_INFO;

// ����� ��� ZG_CVT_OPEN_PARAMS.nFlags
enum _ZG_CVT_OPEN_FLAGS
{
	ZG_OF_NOCHECKLIC	= 0x0001,	// �� ���������/��������� ��������
	ZG_OF_NOSCANCTRS	= 0x0002	// �� ����������� �����������
};
typedef UINT ZG_CVTO_FLAGS;
// ��������� �������� ���������� (��� ������� ZG_Cvt_Open)
typedef struct _ZG_CVT_OPEN_PARAMS
{
	ZP_PORT_TYPE nType;			// ��� �����
	LPCWSTR pszName;			// ��� �����. ���� =NULL, �� ������������ hPort
	HANDLE hPort;				// ���������� �����, ���������� �������� ZP_Open
	ZG_CVT_TYPE nCvtType;		// ��� ����������. ���� =ZG_CVT_UNDEF, �� ���������������
	ZG_CVT_SPEED nSpeed;		// �������� ����������
	PZP_WAIT_SETTINGS pWait;	// ��������� ��������. ����� ���� =NULL.
	BYTE nStopBits;				
	INT nLicN;					// ����� ��������. ���� =0, �� ������������ ZG_DEF_CVT_LICN
	LPCSTR pActCode;			// ��� ��������� ��� ������ "Proxy"
	int nSn;					// �/� ���������� ��� ������ "Proxy"
	ZG_CVTO_FLAGS nFlags;		// ����� ZG_OF_...
} *PZG_CVT_OPEN_PARAMS;

// ���������� � �������� ���������� Guard
typedef struct _ZG_CVT_LIC_INFO
{
	WORD nStatus;			// ������ ��������
	WORD Reserved;			// ��������������� ��� ������������ ���������
	INT nMaxCtrs;			// ������������ ���������� ������������
	INT nMaxKeys;			// ������������ ���������� ������
	WORD nMaxYear;			// ����: ��� (= 0xFFFF ���� ������������)
	WORD nMaxMon;			// ����: �����
	WORD nMaxDay;			// ����: ����
	WORD nDownCountTime;	// ���������� ����� ����� �������� � �������
} *PZG_CVT_LIC_INFO;

// ������� ���������� � �������� ���������� Guard
typedef struct _ZG_CVT_LIC_SINFO
{
	int nLicN;				// ����� ��������
	int nMaxCtrs;			// ������������ ���������� ������������
	INT nMaxKeys;			// ������������ ���������� ������
} *PZG_CVT_LIC_SINFO;

// ������ �����������
enum ZG_CTR_TYPE : INT
{
	ZG_CTR_UNDEF	= 0,	// �� ����������
	ZG_CTR_GATE2K,			// Gate 2000
	ZG_CTR_MATRIX2NET,		// Matrix II Net
	ZG_CTR_Z5RNET,			// Z5R Net
	ZG_CTR_Z5RNET8K,		// Z5R Net 8000
	ZG_CTR_GUARDNET,		// Guard Net
	ZG_CTR_Z9,				// Z-9 EHT Net
	ZG_CTR_EUROLOCK,		// EuroLock EHT net
	ZG_CTR_Z5RWEB			// Z5R Web
};

// ������ �����������
enum ZG_CTR_SUB_TYPE: INT
{
	ZG_CS_UNDEF	= 0,		// �� ����������
	ZG_CS_DOOR,				// �����
	ZG_CS_TURNSTILE,		// ��������
	ZG_CS_GATEWAY,			// ����
	ZG_CS_BARRIER			// ��������
};

// ����� �����������
enum _ZG_CTR_INFO_FLAGS
{
	ZG_CTR_F_2BANKS		= 0x0001,	// 2 ����� / 1 ����
	ZG_CTR_F_PROXIMITY	= 0x0002,	// ��� ������: Proximity (Wiegand-26) / TouchMemory (Dallas)
	ZG_CTR_F_JOIN		= 0x0004,	// ����������� ���� ������
	ZG_CTR_F_X2			= 0x0008,	// �������� ������
	ZG_CTR_F_ELECTRO	= 0x0010,	// ������� ElectroControl (��� Matrix II Net FW 3.X)
	ZG_CTR_F_MODES		= 0x0020,	// ��������� ������� �������
	ZG_CTR_F_DUAL_ZONE	= 0x0040	// ��������� ���� ������� ��������� ���
};
typedef UINT ZG_CTRI_FLAGS;

// ���������� � ��������� �����������, ������������ �������� ZG_Cvt_FindNextCtr
typedef struct _ZG_FIND_CTR_INFO
{
	ZG_CTR_TYPE nType;			// ��� �����������
	BYTE nTypeCode;				// ��� ���� �����������
	BYTE nAddr;					// ������� �����
	WORD nSn;					// �������� ����� (�/�)
	WORD nVersion;				// ������ ��������
	INT nMaxKeys;				// �������� ������
	INT nMaxEvents;				// �������� �������
	ZG_CTRI_FLAGS nFlags;		// ����� ����������� (ZG_CTR_F_...)
	ZG_CTR_SUB_TYPE nSubType;	// ������ �����������
} *PZG_FIND_CTR_INFO;

// ���������� � �����������, ������������ ���������: ZG_Ctr_Open � ZG_Ctr_GetInformation
typedef struct _ZG_CTR_INFO
{
	ZG_CTR_TYPE nType;			// ��� �����������
	BYTE nTypeCode;				// ��� ���� �����������
	BYTE nAddr;					// ������� �����
	WORD nSn;					// �������� ����� (�/�)
	WORD nVersion;				// ������ ��
	INT nInfoLineCount;			// ���������� ����� � �����������
	INT nMaxKeys;				// �������� ������
	INT nMaxEvents;				// �������� �������
	UINT nFlags;				// ����� ����������� (ZG_CTR_F_...)
	WORD Reserved;				// ��������������� ��� ������������ ������� ���������
	LPWSTR pszLinesBuf;			// ����� ��� �������������� �����
	INT nLinesBufMax;			// ������ ������ � ��������, ������� ����������� '\0'
	ZG_CTR_SUB_TYPE nSubType;	// ������ �����������
	INT nOptReadItems;			// ���������� ���������, ������� ����� ���� ������� ����� �������� ����������� 
	INT nOptWriteItems;			// ���������� ���������, ������� ����� ���� �������� ����� �������� �����������
} *PZG_CTR_INFO;

// ����� ������� �����������
enum ZG_CTR_MODE: INT
{
	ZG_MODE_UNDEF = 0,
	ZG_MODE_NORMAL,			// ������� ����� ������
	ZG_MODE_BLOCK,			// ���������� (��������� ����� ������ "�����������" �����)
	ZG_MODE_FREE,			// ��������� (����� ���������, ��� ���������� ����� ��������������)
	ZG_MODE_WAIT			// �������� (������� ����� ������, ��� ���������� ���������� ����� ������� � ����� "Free")
};

// ����� HOTEL
enum ZG_HOTEL_MODE
{
	ZG_HMODE_UNDEF	= 0,
	ZG_HMODE_NORMAL,		// �����
	ZG_HMODE_BLOCK,			// ���������� 
	ZG_HMODE_FREE,			// ��������� ������ 
	ZG_HMODE_RESERVED		// ������ 
};

// ��������� ���� �����������
typedef struct _ZG_CTR_TIMEZONE
{
	BYTE nDayOfWeeks;		// ��� ������
	BYTE nBegHour;			// ������: ���
	BYTE nBegMinute;		// ������: ������
	BYTE nEndHour;			// �����: ���
	BYTE nEndMinute;		// �����: ������
	BYTE Reserved[3];
	ZG_CTR_MODE nMode;		// ����� ����������� (������������ ������ ��� ��.��� ZG_MODES_TZ0..ZG_MODES_TZ0+1)
} *PZG_CTR_TIMEZONE;

// ��� ����� �����������
enum ZG_CTR_KEY_TYPE : INT
{
	ZG_KEY_UNDEF = 0,	// �� ����������
	ZG_KEY_NORMAL,		// �������
	ZG_KEY_BLOCKING,	// �����������
	ZG_KEY_MASTER		// ������
};

// ����� ��� �����
enum _ZG_CTR_KEY_FLAGS
{
	ZG_KF_FUNCTIONAL		= 0x0002,	// ��������������
	ZG_KF_DUAL				= 0x0004,	// ������� �����
	ZG_KF_SHORTNUM			= 0x0020	// �������� �����. ���� fProximity=False, �� ���������� ����� ��������� ������ ������ 3 ����� ������ �����.
};
typedef UINT ZG_KEY_FLAGS;

// ���� �����������
typedef struct _ZG_CTR_KEY
{
	BOOL fErased;				// TRUE, ���� ���� �����
	Z_KEYNUM rNum;				// ����� �����
	ZG_CTR_KEY_TYPE nType;		// ��� �����
	ZG_KEY_FLAGS nFlags;		// ����� ZG_KF_...
	UINT nAccess;				// ������ (����� ��������� ���)
	BYTE aData1[4];				// ������ ������ �����
} *PZG_CTR_KEY;

// ���� �����������
typedef struct _ZG_CTR_CLOCK
{
	// ���� "���� �����������"
	BOOL fStopped;				// TRUE, ���� ���� �����������
	// ���� � �����
	WORD nYear;					// ���
	WORD nMonth;				// �����
	WORD nDay;					// ����
	WORD nHour;					// ���
	WORD nMinute;				// ������
	WORD nSecond;				// �������
} *PZG_CTR_CLOCK;

// ��� ������� �����������
enum ZG_CTR_EV_TYPE : INT
{
	ZG_EV_UNKNOWN = 0,			// ����������
	ZG_EV_BUT_OPEN,				// ������� ������� �������
	ZG_EV_KEY_NOT_FOUND,		// ���� �� ������ � ����� ������
	ZG_EV_KEY_OPEN,				// ���� ������, ����� �������
	ZG_EV_KEY_ACCESS,			// ���� ������, ������ �� ��������
	ZG_EV_REMOTE_OPEN,			// ������� ���������� �� ����
	ZG_EV_KEY_DOOR_BLOCK,		// ���� ������, ����� �������������
	ZG_EV_BUT_DOOR_BLOCK,		// ������� ������� ��������������� ����� �������
	ZG_EV_NO_OPEN,				// ����� ��������
	ZG_EV_NO_CLOSE,				// ����� ��������� �������� (timeout)
	ZG_EV_PASSAGE,				// ������ ���������
	ZG_EV_SENSOR1,				// �������� ������ 1 (������)
	ZG_EV_SENSOR2,				// �������� ������ 2 (�����)
	ZG_EV_REBOOT,				// ������������ �����������
	ZG_EV_BUT_BLOCK,			// ������������� ������ ����������
	ZG_EV_DBL_PASSAGE,			// ������� �������� �������
	ZG_EV_OPEN,					// ����� ������� ������
	ZG_EV_CLOSE,				// ����� �������
	ZG_EV_POWEROFF,				// ������� (0 � �������, 1 � ���������)
	ZG_EV_ELECTRO_ON,			// ��������� ��������������
	ZG_EV_ELECTRO_OFF,			// ���������� ��������������
	ZG_EV_LOCK_CONNECT,			// ��������� ����� (�������)
	ZG_EV_LOCK_DISCONNECT,		// ���������� ����� (�������)
	ZG_EV_MODE_STATE,			// ������������ ������� ������ (c� �����)
	ZG_EV_FIRE_STATE,			// ��������� ��������� ������
	ZG_EV_SECUR_STATE,			// ��������� ��������� ������
	ZG_EV_UNKNOWN_KEY,			// ����������� ����
	ZG_EV_GATEWAY_PASS,			// �������� ���� � ����
	ZG_EV_GATEWAY_BLOCK,		// ������������ ���� � ���� (�����)
	ZG_EV_GATEWAY_ALLOWED,		// �������� ���� � ����
	ZG_EV_ANTIPASSBACK,			// ������������ ������ (�����������)
	ZG_EV_HOTEL40,				// Hotel (��������� ������ ������)
	ZG_EV_HOTEL41				// Hotel (��������� ����)
};

// ������� �����������
typedef struct _ZG_CTR_EVENT
{
	ZG_CTR_EV_TYPE nType;			// ��� �������
	union
	{
		struct
		{
			BYTE nCode;				// ��� ������� � �����������
			BYTE aParams[7];		// ��������� �������
		} ep;
		BYTE aData[8];				// ������ ������� (����������� ������� �������������, �������������� ���� �������, 
									//	ZG_Ctr_DecodePassEvent, ZG_Ctr_DecodeEcEvent, ZG_Ctr_DecodeUnkKeyEvent, ZG_Ctr_DecodeFireEvent, ZG_Ctr_DecodeSecurEvent)
	};
} *PZG_CTR_EVENT;

// ����������� ������� �����������
enum ZG_CTR_DIRECT : INT
{
	ZG_DIRECT_UNDEF = 0,	// �� ����������
	ZG_DIRECT_IN,			// ����
	ZG_DIRECT_OUT			// �����
};

// ���� � ����� �������
typedef struct _ZG_EV_TIME
{
	BYTE nMonth;				// �����
	BYTE nDay;					// ����
	BYTE nHour;					// ���
	BYTE nMinute;				// ������
	BYTE nSecond;				// �������
	BYTE Reserved[3];
} *PZG_EV_TIME;

// �������, ��������� ������� ElectroControl: ZG_EV_ELECTRO_ON, ZG_EV_ELECTRO_OFF
enum ZG_EC_SUB_EV : INT
{
	ZG_EC_EV_UNDEF = 0,		// �� ����������
	ZG_EC_EV_CARD_DELAY,	// ��������� �������� ����� � ������ ������� (��� �����) �������� ��������
	ZG_EC_EV_RESERVED1,		// (���������������)
	ZG_EC_EV_ON_NET,		// �������� �������� �� ����
	ZG_EC_EV_OFF_NET,		// ��������� �������� �� ����
	ZG_EC_EV_ON_SCHED,		// �������� �� ��������� ����
	ZG_EC_EV_OFF_SHED,		// ��������� �� ��������� ����
	ZG_EC_EV_CARD,			// ��������� �������� ����� � ������������ ����������
	ZG_EC_EV_RESERVED2,		// (���������������)
	ZG_EC_EV_OFF_TIMEOUT,	// ��������� ����� ��������� ��������
	ZG_EC_EV_OFF_EXIT		// ��������� �� ������������ ������� ������
};

// �������, ��������� ������� ZG_EV_FIRE_STATE
enum ZG_FIRE_SUB_EV: INT
{
	ZG_FR_EV_UNDEF = 0,		// �� ����������
	ZG_FR_EV_OFF_NET,		// ��������� �� ����
	ZG_FR_EV_ON_NET,		// �������� �� ����
	ZG_FR_EV_OFF_INPUT_F,	// ��������� �� ����� FIRE
	ZG_FR_EV_ON_INPUT_F,	// �������� �� ����� FIRE
	ZG_FR_EV_OFF_TEMP,		// ��������� �� ������� �����������
	ZG_FR_EV_ON_TEMP		// �������� �� ������� �����������
};

// �������, ��������� ������� ZG_EV_SECUR_STATE
enum ZG_SECUR_SUB_EV: INT
{
	ZG_SR_EV_UNDEF = 0,		// �� ����������
	ZG_SR_EV_OFF_NET,		// ��������� �� ����
	ZG_SR_EV_ON_NET,		// �������� �� ����
	ZG_SR_EV_OFF_INPUT_A,	// ��������� �� ����� ALARM
	ZG_SR_EV_ON_INPUT_A,	// �������� �� ����� ALARM
	ZG_FR_EV_OFF_TAMPERE,	// ��������� �� �������
	ZG_FR_EV_ON_TAMPERE,	// �������� �� �������
	ZG_FR_EV_OFF_DOOR,		// ��������� �� ������� �����
	ZG_FR_EV_ON_DOOR		// �������� �� ������� �����
};

// �������, ��������� ������� ZG_EV_MODE_STATE
enum ZG_MODE_SUB_EV: INT
{
	ZG_MD_EV_UNDEF = 0,
	ZG_MD_EV_RS485_ALLOW,		// ��������� �������� �� ���� 
	ZG_MD_EV_RS485_DENIED,		// �������� ��������� �� ����
	ZG_MD_EV_TZ_START,			// �������� ��������� ����
	ZG_MD_EV_TZ_FINISH,			// ���������� ��������� ����
	ZG_MD_EV_CARD_ALLOW,		// ��������� ������
	ZG_MD_EV_CARD_DENIED		// �������� ��������� ������
};

// ������ ������� ��������� ��� ������� ZG_Ctr_ControlDevices
#define ZG_DEV_RELE1		0	// ���� ����� 1
#define ZG_DEV_RELE2		1	// ���� ����� 2
#define ZG_DEV_SW3			2	// ������� ���� SW3 (��) ����.5 ������� �5
#define ZG_DEV_SW4			3	// ������� ���� SW4 (��) ����.5 ������� �6
#define ZG_DEV_SW0			4	// ������� ���� SW0 (��) ����.1 ������� �4
#define ZG_DEV_SW1			5	// ������� ���� SW1 (��) ����.3 ������� �4
#define ZG_DEV_K65			6	// ����������� ���� (��) ����.6 ������� �5
#define ZG_DEV_K66			7	// ����������� ���� (��) ����.6 ������� �6

// ����� ������������ ��������������
enum _ZG_CTR_ELECTROCONTROL_CONFIG_FLAGS
{
	ZG_EC_CF_ENABLED		= 0x0001,	// ������������� ���������� ��������
	ZG_EC_CF_SCHEDULE		= 0x0002,	// ������������ ��������� ���� 6 ��� ��������� �������
	ZG_EC_CF_EXT_READER		= 0x0004,	// ����������� �����������: �0� Matrix-II Net, �1� ������� �����������
	ZG_EC_CF_INVERT			= 0x0008,	// ������������� ����������� �����
	ZG_EC_CF_EXIT_OFF		= 0x0010,	// ������������� ������ �����
	ZG_EC_CF_CARD_OPEN		= 0x0020	// �� ����������� ������� ���������� ��� ������������ �����������
};
typedef UINT ZG_CTRECC_FLAGS;

// ������������ ���������� ���������������
typedef struct _ZG_CTR_ELECTRO_CONFIG
{
	ZG_CTRECC_FLAGS nPowerConfig;	// ������������ ���������� ��������
	DWORD nPowerDelay;				// ����� �������� � ��������
	_ZG_CTR_TIMEZONE rTz6;			// ��������� ���� �6 (������� �� 0)
} *PZG_CTR_ELECTRO_CONFIG;

// ����� ��������� ��������������
enum _ZG_CTR_ELECTROCONTROL_STATE_FLAGS
{
	ZG_EC_SF_ENABLED		= 0x0001,	// ��������� ������� � 1 ���/0 ����
	ZG_EC_SF_SCHEDULE		= 0x0002,	// ������� ��������� �� ��������� ����
	ZG_EC_SF_REMOTE			= 0x0004,	// �������� �� ������� �� ����
	ZG_EC_SF_DELAY			= 0x0008,	// ���� ��������� ��������
	ZG_EC_SF_CARD			= 0x0010	// ����� � ���� ������������ �����������
};
typedef UINT ZG_CTRECS_FLAGS;

// ��������� ��������������
typedef struct _ZG_CTR_ELECTRO_STATE
{
	ZG_CTRECS_FLAGS nPowerFlags;		// ����� ��������� ��������������
	ZG_CTRECC_FLAGS nPowerConfig;		// ������������ ���������� ��������
	DWORD nPowerDelay;					// ����� �������� � ��������
} *PZG_CTR_ELECTRO_STATE;

// ����� ��������� ������ �����
enum _ZG_CTR_FIRE_STATE_FLAGS
{
	ZG_FR_F_ENABLED			= 0x0001,	// ��������� ��������� ������ � 1 ���/0 ����
	ZG_FR_F_INPUT_F			= 0x0002,	// ������� �������� ����� �� ����� FIRE
	ZG_FR_F_TEMP			= 0x0004,	// ������� �������� ����� �� ���������� �����������
	ZG_FR_F_NET				= 0x0008	// ������� �������� ����� �� ������� �������
};
typedef UINT ZG_CTRFS_FLAGS;
// ����� ��� ����� ���������� ���������� ������ �����
enum _ZG_CTR_FIRE_SRC_FLAGS
{
	ZG_FR_SRCF_INPUT_F		= 0x0001,	// �������� �������� ����� �� ����� FIRE
	ZG_FR_SRCF_TEMP			= 0x0002	// �������� �������� ����� �� ���������� �����������
};
typedef UINT ZG_CTRFSR_FLAGS;

// ����� ������
enum ZG_SECUR_MODE: INT
{
	ZG_SR_M_UNDEF = 0,		// �� ����������
	ZG_SR_M_SECUR_OFF,		// ��������� ����� ������
	ZG_SR_M_SECUR_ON,		// �������� ����� ������
	ZG_SR_M_ALARM_OFF,		// ��������� �������
	ZG_SR_M_ALARM_ON		// �������� �������
};

// ����� ��������� ������ ������
enum _ZG_CTR_SECURE_STATE_FLAGS
{
	ZG_SR_F_ENABLED			= 0x0001,	// ��������� ��������� ������ � 1 ���/0 ����
	ZG_SR_F_ALARM			= 0x0002,	// ��������� �������
	ZG_SR_F_INPUT_A			= 0x0004,	// ������� �� ����� ALARM
	ZG_SR_F_TAMPERE			= 0x0008,	// ������� �� �������
	ZG_SR_F_DOOR			= 0x0010,	// ������� �� ������� �����
	ZG_SR_F_NET				= 0x0020	// ������� �������� �� ����
};
typedef UINT ZG_CTRSS_FLAGS;

// ����� ��� ����� ���������� ���������� ������ ������
enum _ZG_CTR_SECURE_SRC_FLAGS
{
	ZG_SR_SRCF_INPUT_F		= 0x0001,	// ��������� ������� �� ����� FIRE
	ZG_SR_SRCF_TAMPERE		= 0x0002,	// ��������� ������� �� �������
	ZG_SR_SRCF_DOOR			= 0x0004	// ��������� ������� �� ������� �����
};
typedef UINT ZG_CTRSSR_FLAGS;

// �������, ��������� ������� ZG_EV_HOTEL40, ZG_EV_HOTEL41
enum ZG_HOTEL_SUB_EV : INT
{
	ZG_H_EV_UNDEF = 0,		// �� ����������
	ZG_H_EV_FREECARD,		// ����� ��������
	ZG_H_EV_BLOCKCARD,		// ����� �����������
	ZG_H_EV_EXFUNC,			// �������������� �������
	ZG_H_EV_NEWRCARD,		// ������� ��������� �����
	ZG_H_EV_NETWORK,
	ZG_H_EV_TIMEZONE,
	ZG_H_EV_COUNTER,		// �������� �������
	ZG_H_EV_CRYPTOKEY,		// �������� ����������
	ZG_H_EV_PULSEZ,			// ����������� ������� � ������� 2� ������
	ZG_H_EV_STATE			// ��������� ������� -���� ������ ����� � ��������� ����� ��� ����� 2 ������� 
};

// ����� ��� ������� ZG_EV_HOTEL40, ZG_EV_HOTEL41
#define ZG_HF_LATCH			1	// �������
#define ZG_HF_LATCH2		2	// ��������
#define ZG_HF_KEY			4	// ����
#define ZG_HF_CARD			8	// �����

// ����� ��� ������� ZG_Cvt_SetNotification � ��������� _ZG_CVT_NOTIFY_SETTINGS
enum _ZG_CVTN_FLAGS
{
	ZG_NF_CVT_CTR_EXIST			= 0x0001,	// ZG_N_CVT_CTR_INSERT / ZG_N_CVT_CTR_REMOVE
	ZG_NF_CVT_CTR_CHANGE		= 0x0002,	// ��������� ���������� ����������� ZG_N_CVT_CTR_CHANGE
	ZG_NF_CVT_ERROR				= 0x0004,	// ZG_N_CVT_ERROR
	ZG_CVTNF_CONNECTION_CHANGE	= 0x0008,	// ZG_CVN_CONNECTIONSTATUS
	ZG_NF_CVT_CTR_DBL_CHECK		= 0x1000,	// ������ ��������� ���������� ������������
	ZG_NF_CVT_REASSIGN_ADDRS	= 0x2000,	// �������������� �������������� ������� ������������ (�������� ������ � ZG_NF_CVT_CTR_EXIST)
	ZG_NF_CVT_RESCAN_CTRS		= 0x10000,	// ������ ������ ������������ ������������ (��� Z-397 � Z-397 Guard � ������ Normal)
	ZG_NF_CVT_ALT_SCAN			= 0x20000,	// �������������� ����� ������������
	ZG_NF_CVT_NOGATE			= 0x40000,	// �� ����������� GATE-����������� (���, ����� Eurolock)
	ZG_NF_CVT_NOEUROLOCK		= 0x80000	// �� ����������� Eurolock EHT net
};
typedef UINT ZG_CVTN_FLAGS;

// ����������� ������� ZG_Cvt_SetNotification (wParam - MsgCode, lParam - MsgParam)
typedef enum _ZG_CVT_NOTIFICATIONS
{
	ZG_N_CVT_CTR_INSERT			= 1,	// ���������� ��������� PZG_FIND_CTR_INFO(MsgParam) - ���������� � �����������
	ZG_N_CVT_CTR_REMOVE			= 2,	// ���������� �������� PZG_FIND_CTR_INFO(MsgParam) - ���������� � �����������
	ZG_N_CVT_CTR_CHANGE			= 3,	// �������� ��������� ����������� PZG_N_CTR_CHANGE_INFO(MsgParam)
	ZG_N_CVT_ERROR				= 4,	// �������� ������ � ���� (HRESULT*)nMsgParam - ��� ������
	ZG_CVTN_CONNECTION_CHANGE	= 5		// ���������� ��������� �����������
} ZG_CVT_NOTIFICATIONS;

typedef struct _ZG_N_CTR_CHANGE_INFO
{
	UINT nChangeMask;			// ����� ��������� (���0 addr, ���1 version, ���2 flags)
	_ZG_FIND_CTR_INFO rCtrInfo;	// ���������� ���������� � �����������
	WORD nOldVersion;			// ������ �������� ������
	BYTE nOldAddr;				// ������ �������� ������
	BYTE Reserved;				// ��������������� ��� ������������ ���������
} *PZG_N_CTR_CHANGE_INFO;

// ��������� ����������� ����������
typedef struct _ZG_CVT_NOTIFY_SETTINGS
{
	ZG_CVTN_FLAGS nNMask;		// ����� ����� ����������� (ZG_NF_CVT_...)

	HANDLE hEvent;				// ������� (������ �������������)
	HWND hWindow;				// ����, ����������� ��������� nWndMsgId
	UINT nWndMsgId;				// ��������� ��� �������� ���� hWnd

	DWORD nScanCtrsPeriod;		// ������ ������������ ������ ������������ � ��. ���� =0, ������������ �������� �� ��������� ZG_CVT_SCANCTRSPERIOD. ���� =-1, �������
	INT nScanCtrsLastAddr;		// ��������� ����������� ����� �����������. ���� =0, ������������ �������� �� ��������� ZG_CVT_SCANCTRSLASTADDR
} *PZG_CVT_NOTIFY_SETTINGS;

// ����� ��� ������� ZG_Ctr_SetNotification � ��������� _ZG_CTR_NOTIFY_SETTINGS
enum _ZG_CTRN_FLAGS
{
	ZG_NF_CTR_NEW_EVENT			= 0x0001,	// ZG_N_CTR_NEW_EVENT
	ZG_NF_CTR_CLOCK				= 0x0002,	// ZG_N_CTR_CLOCK
	ZG_NF_CTR_KEY_TOP			= 0x0004,	// ZG_N_CTR_KEY_TOP
	ZG_NF_CTR_ADDR_CHANGE		= 0x0008,	// ZG_N_CTR_ADDR_CHANGE
	ZG_NF_CTR_ERROR				= 0x0010	// ZG_N_CTR_ERROR
};
typedef UINT ZG_CTRN_FLAGS;

// ����������� ������� ZG_Ctr_SetNotification
typedef enum _ZG_CTR_NOTIFICATIONS
{
	ZG_N_CTR_NEW_EVENT			= 1,	// ����� ������� PZG_N_NEW_EVENT_INFO(MsgParam) - ����������
	ZG_N_CTR_CLOCK				= 2,	// �������� ���������������� � �������� PINT64(MsgParam)
	ZG_N_CTR_KEY_TOP			= 3,	// ���������� ������� ������� ������ PZG_N_KEY_TOP_INFO(MsgParam) - ����������
	ZG_N_CTR_ADDR_CHANGE		= 4,	// ������� ������� ����� ����������� PBYTE(MsgParam) = ����� �����
	ZG_N_CTR_ERROR				= 5		// �������� ������ � ���� PHRESULT(MsgParam) - ��� ������
} ZG_CTR_NOTIFICATIONS;

// ���������� ��� ����������� ZG_N_CTR_NEW_EVENT
typedef struct _ZG_N_NEW_EVENT_INFO
{
	INT nNewCount;		// ���������� ����� �������
	INT nWriteIdx;		// ��������� ������
	INT nReadIdx;		// ��������� ������
	Z_KEYNUM rLastNum;	// ����� ���������� ������������ �����
} *PZG_N_NEW_EVENT_INFO;

// ���������� ��� ����������� ZG_N_CTR_KEY_TOP
typedef struct _ZG_N_KEY_TOP_INFO
{
	INT nBankN;			// ����� ����� ������
	INT nNewTopIdx;		// ����� �������� ������� ������� ������
	INT nOldTopIdx;		// ������ �������� ������� ������� ������
} *PZG_N_KEY_TOP_INFO;

// ��������� ����������� �����������
typedef struct _ZG_CTR_NOTIFY_SETTINGS
{
	ZG_CTRN_FLAGS nNMask;		// ����� ����� ����������� (ZG_NF_CTR_...)

	HANDLE hEvent;          	// ������� (������ �������������)
	HWND hWnd;              	// ����, ����������� ��������� nWndMsgId
	UINT nWndMsgId;         	// ��������� ��� �������� ���� hWnd

	INT nReadEvIdx;				// ��������� ������ �������
	DWORD nCheckStatePeriod;	// ������ �������� ��������� ����������� (����� �������, �����, ������� ������� ������) � �� (=0 ������������ �������� �� ���������, 1000)

	DWORD nClockOffs;			// �������� ����� ����������� �� ����� �� � ��������
} *PZG_CTR_NOTIFY_SETTINGS;

enum _ZG_CVT_SEARCH_FLAGS
{
	ZG_CVTSF_DETECTOR			= 0x0001,	// ������������ ������� ������ ��������� ������������ ��������� (ZG_Cvt_SetNotification)
	ZG_CVTSF_NO_GATE			= 0x0004,	// �� ������ GATE-�����������
	ZG_CVTSF_NO_EUROLOCK		= 0x0008	// �� ������ �����: Eurolock EHT net � Z-9 EHT net
};
typedef UINT ZG_CVTS_FLAGS;

// Callback-�������
typedef BOOL (CALLBACK* ZG_PROCESSCALLBACK) (INT nPos, INT nMax, PVOID pUserData);
typedef BOOL (CALLBACK* ZG_ENUMCTRSPROC) (PZG_FIND_CTR_INFO pInfo, INT nPos, INT nMax, PVOID pUserData);
typedef BOOL (CALLBACK* ZG_ENUMCTRTIMEZONESPROC) (INT nIdx, PZG_CTR_TIMEZONE pTz, PVOID pUserData);
typedef BOOL (CALLBACK* ZG_ENUMCTRKEYSPROC) (INT nIdx, PZG_CTR_KEY pKey, INT nPos, INT nMax, PVOID pUserData);
typedef BOOL (CALLBACK* ZG_ENUMCTREVENTSPROC) (INT nIdx, PZG_CTR_EVENT pEvent, INT nPos, INT nMax, PVOID pUserData);

// ���������� ����
typedef BOOL (CALLBACK* ZG_ENUMCVTSPROC) (PZG_ENUM_CVT_INFO pInfo, PZP_PORT_INFO pPort, PVOID pUserData);
typedef BOOL (CALLBACK* ZG_ENUMIPCVTSPROC) (PZG_ENUM_IPCVT_INFO pInfo, PZP_PORT_INFO pPort, PVOID pUserData);

#pragma pack() // turn byte alignment off

#ifndef ZGUARD_LINKONREQUEST

// ���������� ������ ���������� ZGuard.dll
ZGUARD_API(DWORD) ZG_GetVersion();

// �������������/����������� ����������
ZGUARD_API(HRESULT) ZG_Initialize(UINT nFlags);
ZGUARD_API(HRESULT) ZG_Finalyze();

//////////////////////////////////////////////////////////////////////////
// ������� ��� ������ � �����������
//

// ���������� ������ �/� �����������, ������������ � Proxy-�������
ZGUARD_API(HRESULT) ZG_GetProxyConverters(PWORD pSnBuf, int nBufSize, LPINT pRCount, LPCWSTR pIpAddr, LPCSTR pActCode, PZP_WAIT_SETTINGS pWait=NULL);

// ��������� ����� �������� � ���������
ZGUARD_API(HRESULT) ZG_UpdateCvtFirmware(PZG_CVT_OPEN_PARAMS pParams, 
	LPCVOID pData, INT nCount, ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);

// ���������/��������� ���������
ZGUARD_API(HRESULT) ZG_Cvt_Open(PHANDLE pHandle, PZG_CVT_OPEN_PARAMS pParams, PZG_CVT_INFO pInfo=NULL);
// ����������� �� ����������, �� �������� ����, ���������� ���������� �����, ���������� �������� ZP_Open
ZGUARD_API(HRESULT) ZG_Cvt_DettachPort(HANDLE hHandle, PHANDLE pPortHandle);
// ���������� ��������� �����������
ZGUARD_API(HRESULT) ZG_Cvt_GetConnectionStatus(HANDLE hHandle, ZP_CONNECTION_STATUS* pValue);

// ����������/������������� ��������� �������� ���������� �������
ZGUARD_API(HRESULT) ZG_Cvt_GetWaitSettings(HANDLE hHandle, PZP_WAIT_SETTINGS pSetting);
ZGUARD_API(HRESULT) ZG_Cvt_SetWaitSettings(HANDLE hHandle, PZP_WAIT_SETTINGS pSetting);

// ��������� ���� (���������������� ������� �������� � ������)
ZGUARD_API(HRESULT) ZG_Cvt_SetCapture(HANDLE hHandle);
// ��������� ���� (������������ ������� �������� � ������)
ZGUARD_API(HRESULT) ZG_Cvt_ReleaseCapture(HANDLE hHandle);
// ������� ���� ����������
ZGUARD_API(HRESULT) ZG_Cvt_Clear(HANDLE hHandle);
// ���������� ������ ����������
ZGUARD_API(HRESULT) ZG_Cvt_Send(HANDLE hHandle, LPCVOID pData, INT nCount);
// ���������� ����� �� ����������
ZGUARD_API(HRESULT) ZG_Cvt_Receive(HANDLE hHandle, LPVOID pBuf, INT nBufSize, INT nMinRead, LPINT pRCount);
// ���������� ������ � ���������� ����� �� ����������
ZGUARD_API(HRESULT) ZG_Cvt_Exec(HANDLE hHandle, LPCVOID pData, INT nCount, 
	LPVOID pBuf, INT nBufSize, INT nMinRead, LPINT pRCount);

// ����������� ��� ������������ ����������� (��� �� Guard-����������� ����� �������������� ��������� ����������� � �����/����. ������������)
#define ZG_F_UPDATE		1	// �������� ������ ������
#define ZG_F_REASSIGN	2	// ������������� ������������� ������
#define ZG_F_NOGATE		4	// �� ������ GATE-�����������
#define ZG_F_NOEUROLOCK	8	// �� ������ Eurolock EHT net
ZGUARD_API(HRESULT) ZG_Cvt_EnumControllers(HANDLE hHandle, ZG_ENUMCTRSPROC pEnumProc, PVOID pUserData, UINT nFlags=ZG_F_UPDATE);
// ����� ����������� �� �������� ������
ZGUARD_API(HRESULT) ZG_Cvt_FindController(HANDLE hHandle, BYTE nAddr, PZG_FIND_CTR_INFO pInfo, 
	UINT nFlags=ZG_F_UPDATE, PZP_WAIT_SETTINGS pWait=NULL);

ZGUARD_API(HRESULT) ZG_Cvt_SearchControllers(HANDLE hHandle, INT nMaxCtrs, ZG_CVTS_FLAGS nFlags=0);
ZGUARD_API(HRESULT) ZG_Cvt_FindNextController(HANDLE hHandle, PZG_FIND_CTR_INFO pInfo);

// ���������� ���������� � ����������
ZGUARD_API(HRESULT) ZG_Cvt_GetInformation(HANDLE hHandle, PZG_CVT_INFO pInfo);

// ����������� ����������� �� ����������
ZGUARD_API(HRESULT) ZG_Cvt_SetNotification(HANDLE hHandle, PZG_CVT_NOTIFY_SETTINGS pSettings);
ZGUARD_API(HRESULT) ZG_Cvt_GetNextMessage(HANDLE hHandle, PUINT pMsg, LPARAM* pMsgParam);
ZGUARD_API(HRESULT) ZG_Cvt_GetScanCtrsState(HANDLE hHandle, LPINT pNextAddr);

// ��������� � ��������� ����� ��������
ZGUARD_API(HRESULT) ZG_Cvt_UpdateFirmware(HANDLE hHandle, LPCVOID pData, INT nCount, 
	ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);

// ���������� ���������� � �������� ����������
ZGUARD_API(HRESULT) ZG_Cvt_GetLicense(HANDLE hHandle, BYTE nLicN, PZG_CVT_LIC_INFO pInfo);
// ������������� ����� ��������
ZGUARD_API(HRESULT) ZG_Cvt_SetLicenseData(HANDLE hHandle, BYTE nLicN, LPCVOID pData, INT nCount, LPWORD pLicStatus=NULL);
// ������� ��� �������� � ����������
ZGUARD_API(HRESULT) ZG_Cvt_ClearAllLicenses(HANDLE hHandle);
// ���������� ���������� � ���� ���������, ������������� � ���������
ZGUARD_API(HRESULT) ZG_Cvt_GetAllLicenses(HANDLE hHandle, PZG_CVT_LIC_SINFO pBuf, INT nBufSize, LPINT pCount);

// ���������� �/� ���������� � ����� (������ Guard)
ZGUARD_API(HRESULT) ZG_Cvt_GetShortInfo(HANDLE hHandle, LPWORD pSn, ZG_GUARD_MODE* pMode);
// ���������� �������������� ������ ���������� (������ Guard)
ZGUARD_API(HRESULT) ZG_Cvt_GetLongInfo(HANDLE hHandle, PWORD pSn, PDWORD pVersion, ZG_GUARD_MODE* pMode,
	LPSTR pBuf, INT nBufSize, LPINT pLen); // �������� 'i'

// ��������� � ���������� ����� ��������
ZGUARD_API(HRESULT) ZG_Cvt_UpdateCtrFirmware(HANDLE hHandle, WORD nCtrSn, LPCVOID pData, INT nCount, LPCSTR pszInfoStr, 
	ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, ZG_CTR_TYPE nModel=ZG_CTR_UNDEF);

// ���������� ����� ������� ����� ����������� �� �/�
ZGUARD_API(HRESULT) ZG_Cvt_SetCtrAddrBySn(HANDLE hHandle, WORD nSn, BYTE nNewAddr, ZG_CTR_TYPE nModel=ZG_CTR_UNDEF);
ZGUARD_API(HRESULT) ZG_Cvt_SetCtrAddr(HANDLE hHandle, BYTE nOldAddr, BYTE nNewAddr, ZG_CTR_TYPE nModel=ZG_CTR_UNDEF);

// ����������� ���������� � ����������� �� �������� ������
ZGUARD_API(HRESULT) ZG_Cvt_GetCtrInfoNorm(HANDLE hHandle, BYTE nAddr, PBYTE pTypeCode, LPWORD pSn, LPWORD pVersion, LPINT pInfoLines, PUINT pFlags, ZG_CTR_TYPE nModel=ZG_CTR_UNDEF);
ZGUARD_API(HRESULT) ZG_Cvt_GetCtrInfoAdv(HANDLE hHandle, BYTE nAddr, PBYTE pTypeCode, LPWORD pSn, LPWORD pVersion, PUINT pFlags, PINT pEvWrIdx, PINT pEvRdIdx);
// ����������� ���������� � ����������� �� ��������� ������
ZGUARD_API(HRESULT) ZG_Cvt_GetCtrInfoBySn(HANDLE hHandle, WORD nSn, PBYTE pTypeCode, LPBYTE pAddr, LPWORD pVersion, LPINT pInfoLines, PUINT pFlags, ZG_CTR_TYPE nModel=ZG_CTR_UNDEF);
// ���������� �������������� ������ �����������
ZGUARD_API(HRESULT) ZG_Cvt_GetCtrInfoLine(HANDLE hHandle, WORD nSn, INT nLineN, LPSTR pBuf, INT nBufSize, LPINT pLen=NULL, ZG_CTR_TYPE nModel=ZG_CTR_UNDEF);
// ���������� ���������� � ������ ����������� (����� ������������ ��� �������� ����������� �����������)
ZGUARD_API(HRESULT) ZG_Cvt_GetCtrVersion(HANDLE hHandle, BYTE nAddr, LPBYTE pVerData5, PZP_WAIT_SETTINGS pWS=NULL);

//////////////////////////////////////////////////////////////////////////
// ������� ��� ������ � ������������
//

// ���������/��������� ����������
ZGUARD_API(HRESULT) ZG_Ctr_Open(PHANDLE pHandle, HANDLE hCvtHandle, BYTE nAddr, WORD nSn, PZG_CTR_INFO pInfo=NULL, ZG_CTR_TYPE nModel=ZG_CTR_UNDEF);
// ���������� ���������� � �����������
ZGUARD_API(HRESULT) ZG_Ctr_GetInformation(HANDLE hHandle, PZG_CTR_INFO pInfo);
// ����������� ����������� �� �����������
ZGUARD_API(HRESULT) ZG_Ctr_SetNotification(HANDLE hHandle, PZG_CTR_NOTIFY_SETTINGS pSettings);
ZGUARD_API(HRESULT) ZG_Ctr_GetNextMessage(HANDLE hHandle, PUINT pMsg, LPARAM* pMsgParam);
// ������������� ������� ����� ����������� (Guard �� ������������)
ZGUARD_API(HRESULT) ZG_Ctr_SetNewAddr(HANDLE hHandle, BYTE nNewAddr);
// ��������� ���������� ����������� � ����� ������� ������� �����������
ZGUARD_API(HRESULT) ZG_Ctr_AssignAddr(HANDLE hHandle, BYTE nAddr);
// ��������� ����� �������� � ����������
ZGUARD_API(HRESULT) ZG_Ctr_UpdateFirmware(HANDLE hHandle, LPCVOID pData, INT nCount, LPCSTR pszInfoStr, 
	ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);

// ������� �����
ZGUARD_API(HRESULT) ZG_Ctr_OpenLock(HANDLE hHandle, INT nLockN=0);
// ���������� �����
ZGUARD_API(HRESULT) ZG_Ctr_CloseLock(HANDLE hHandle);
// �������� ����� ���������� ���������� ������
ZGUARD_API(HRESULT) ZG_Ctr_EnableEmergencyUnlocking(HANDLE hHandle, BOOL fEnable);
// ���������� ��������� ������ ���������� ���������� ������
ZGUARD_API(HRESULT) ZG_Ctr_IsEmergencyUnlockingEnabled(HANDLE hHandle, PBOOL pEnabled);

// ������� �������� �����������
ZGUARD_API(HRESULT) ZG_Ctr_ReadRegs(HANDLE hHandle, DWORD nAddr, INT nCount, LPVOID pBuf);
// ������� ��������� ������
ZGUARD_API(HRESULT) ZG_Ctr_ReadPorts(HANDLE hHandle, LPDWORD pData);
// ���������� �������� ������������ (nDevType - ����� ���������� ZG_DEV_..)
ZGUARD_API(HRESULT) ZG_Ctr_ControlDevices(HANDLE hHandle, DWORD nDevType, BOOL fActive, DWORD nTimeMs=0);

// ������ / ������ ������ ����������� (nBankN - ����� �� ����: =0 ����, =1 �����, =2 �������
ZGUARD_API(HRESULT) ZG_Ctr_ReadData(HANDLE hHandle, INT nBankN, DWORD nAddr, INT nCount, LPVOID pBuf, 
	LPINT pReaded, ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);
ZGUARD_API(HRESULT) ZG_Ctr_WriteData(HANDLE hHandle, INT nBankN, DWORD nAddr, LPCVOID pData, INT nCount, 
	LPINT pWritten, ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);

// ���������� ������� ��� ������ �����������
ZGUARD_API(HRESULT) ZG_Ctr_ReadLockTimes(HANDLE hHandle, LPDWORD pOpenMs, LPDWORD pLetMs, LPDWORD pMaxMs, INT nBankN=0);
// ������������� ������� ��� ������ �����������
ZGUARD_API(HRESULT) ZG_Ctr_WriteLockTimes(HANDLE hHandle, DWORD nMask, DWORD nOpenMs, DWORD nLetMs, DWORD nMaxMs, INT nBankN=0);

// ���������� ���� ��� ��������� ��������� ���
ZGUARD_API(HRESULT) ZG_Ctr_ReadTimeZones(HANDLE hHandle, INT nIdx, PZG_CTR_TIMEZONE pBuf, INT nCount, 
	ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, INT nBankN=0);
// ������������� ���� ��� ��������� ��������� ���
ZGUARD_API(HRESULT) ZG_Ctr_WriteTimeZones(HANDLE hHandle, INT nIdx, PZG_CTR_TIMEZONE pTzs, INT nCount,
	ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, INT nBankN=0);
// ����������� ��������� ���� � �����������
ZGUARD_API(HRESULT) ZG_Ctr_EnumTimeZones(HANDLE hHandle, INT nStart, ZG_ENUMCTRTIMEZONESPROC fnEnumProc, PVOID pUserData, INT nBankN=0);

// ���������� ���� ��� ��������� ������
ZGUARD_API(HRESULT) ZG_Ctr_ReadKeys(HANDLE hHandle, INT nIdx, PZG_CTR_KEY pBuf, INT nCount,
	ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, INT nBankN=0);
// ������������� ���� ��� ��������� ������
ZGUARD_API(HRESULT) ZG_Ctr_WriteKeys(HANDLE hHandle, INT nIdx, PZG_CTR_KEY pKeys, INT nCount,
	ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, INT nBankN=0, BOOL fUpdateTop=TRUE);
// ������� ���� ��� ��������� ������
ZGUARD_API(HRESULT) ZG_Ctr_ClearKeys(HANDLE hHandle, INT nIdx, INT nCount, 
	ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, INT nBankN=0, BOOL fUpdateTop=TRUE);
// ���������� ������ ������� ������� ������
ZGUARD_API(HRESULT) ZG_Ctr_GetKeyTopIndex(HANDLE hHandle, LPINT pIdx, INT nBankN=0);
// ����������� ����� � �����������
ZGUARD_API(HRESULT) ZG_Ctr_EnumKeys(HANDLE hHandle, INT nStart, ZG_ENUMCTRKEYSPROC fnEnumProc, PVOID pUserData, INT nBankN=0);

// ���������� ���� �����������
ZGUARD_API(HRESULT) ZG_Ctr_GetClock(HANDLE hHandle, PZG_CTR_CLOCK pClock);
// ������������� ���� �����������
ZGUARD_API(HRESULT) ZG_Ctr_SetClock(HANDLE hHandle, PZG_CTR_CLOCK pClock);

// ���������� ����� ���������� ������������ �����
ZGUARD_API(HRESULT) ZG_Ctr_ReadLastKeyNum(HANDLE hHandle, Z_KEYNUM* pNum);
// ���������� ��������� ������� � ����� ���������� ������������ ����� (���� *pWrIdx ��� *pRdIdx == -1, �� ��������� ������������)
ZGUARD_API(HRESULT) ZG_Ctr_ReadRTCState(HANDLE hHandle, PZG_CTR_CLOCK pClock, 
	LPINT pWrIdx, LPINT pRdIdx, Z_KEYNUM* pNum);
// ���������� ��������� �������
ZGUARD_API(HRESULT) ZG_Ctr_ReadEventIdxs(HANDLE hHandle, LPINT pWrIdx, LPINT pRdIdx);
// ������������� ��������� �������
ZGUARD_API(HRESULT) ZG_Ctr_WriteEventIdxs(HANDLE hHandle, UINT nMask, INT nWrIdx, INT nRdIdx);
// ���������� ���� ��� ��������� �������
ZGUARD_API(HRESULT) ZG_Ctr_ReadEvents(HANDLE hHandle, INT nIdx, PZG_CTR_EVENT pBuf, INT nCount,
	ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);
// ����������� ������� �����������
// nStart - ������ ������� �������, ���� =-1, ������������ ��������� ������,
// nCount - ���������� ������������� �������, ���� =-1, �� ������������ ���������� ������� � nStart �� ��������� ������,
//			���� =INT_MAX, �� ������������� ��� �������
ZGUARD_API(HRESULT) ZG_Ctr_EnumEvents(HANDLE hHandle, INT nStart, INT nCount,
	ZG_ENUMCTREVENTSPROC fnEnumProc, PVOID pUserData);
// ������������� ������� �������
ZGUARD_API(HRESULT) ZG_Ctr_DecodePassEvent(HANDLE hHandle, PBYTE pData8, 
	PZG_EV_TIME pTime, ZG_CTR_DIRECT* pDirect, PINT pKeyIdx, PINT pKeyBank);
// ������������� ������� ElectoControl: ZG_EV_ELECTRO_ON, ZG_EV_ELECTRO_OFF
ZGUARD_API(HRESULT) ZG_Ctr_DecodeEcEvent(HANDLE hHandle, PBYTE pData8, 
	PZG_EV_TIME pTime, ZG_EC_SUB_EV* pSubEvent, PDWORD pPowerFlags);
// ������������� ������� "����������� ����": ZG_EV_UNKNOWN_KEY
ZGUARD_API(HRESULT) ZG_Ctr_DecodeUnkKeyEvent(HANDLE hHandle, PBYTE pData8, PZ_KEYNUM pKeyNum);
// ������������� ������� "��������� ��������� ������": ZG_EV_FIRE_STATE
ZGUARD_API(HRESULT) ZG_Ctr_DecodeFireEvent(HANDLE hHandle, PBYTE pData8, 
	PZG_EV_TIME pTime, ZG_FIRE_SUB_EV* pSubEvent, PDWORD pFireFlags);
// ������������� ������� "��������� ��������� ������": ZG_EV_SECUR_STATE
ZGUARD_API(HRESULT) ZG_Ctr_DecodeSecurEvent(HANDLE hHandle, PBYTE pData8, 
	PZG_EV_TIME pTime, ZG_SECUR_SUB_EV* pSubEvent, PDWORD pSecurFlags);
// ������������� ������� "��������� ��������� �����": ZG_EV_MODE_STATE
ZGUARD_API(HRESULT) ZG_Ctr_DecodeModeEvent(HANDLE hHandle, PBYTE pData8,
	PZG_EV_TIME pTime, ZG_CTR_MODE* pCurrMode, ZG_MODE_SUB_EV* pSubEvent);
// ������������� ������� HOTEL: ZG_EV_HOTEL40 � ZG_EV_HOTEL41
ZGUARD_API(HRESULT) ZG_DecodeHotelEvent(PBYTE pData8, 
	PZG_EV_TIME pTime, ZG_HOTEL_MODE* pMode, ZG_HOTEL_SUB_EV* pSubEvent, LPDWORD pFlags);

// ���������� �������� ������� �� ����
ZGUARD_API(HRESULT) ZG_Ctr_SetFireMode(HANDLE hHandle, BOOL fOn);
// ������ ��������� ��������� ������
// pFireFlags - ����� ��������� ZG_FR_F_...
// pCurrTemp - ������� ����������� (� ��������)
// pSrcMask - ����� ����������� ���������� ZG_FR_SRCF_...
// pLimitTemp - ��������� ����������� (� ��������)
ZGUARD_API(HRESULT) ZG_Ctr_GetFireInfo(HANDLE hHandle, ZG_CTRFS_FLAGS* pFireFlags, 
	PDWORD pCurrTemp, ZG_CTRFSR_FLAGS* pSrcMask, PDWORD pLimitTemp);
// ��������� ���������� ��������� ������
ZGUARD_API(HRESULT) ZG_Ctr_SetFireConfig(HANDLE hHandle, ZG_CTRFSR_FLAGS nSrcMask, DWORD nLimitTemp, 
	ZG_CTRFS_FLAGS* pFireFlags, PDWORD pCurrTemp);

// ���������� ������� ������ �� ����
ZGUARD_API(HRESULT) ZG_Ctr_SetSecurMode(HANDLE hHandle, ZG_SECUR_MODE nMode);
// ������ ��������� ������ ������
// pSecurFlags - ����� ��������� ZG_SR_F_
// pSrcMask - ����� ����������� ���������� ZG_SR_SRCF_...
// pAlarmTime - ����� �������� ������ (� ��������)
ZGUARD_API(HRESULT) ZG_Ctr_GetSecurInfo(HANDLE hHandle, ZG_CTRSS_FLAGS* pSecurFlags, 
	ZG_CTRSSR_FLAGS* pSrcMask, PDWORD pAlarmTime);
// ��������� ���������� ������ ������
ZGUARD_API(HRESULT) ZG_Ctr_SetSecurConfig(HANDLE hHandle, ZG_CTRSSR_FLAGS nSrcMask, DWORD nAlarmTime, 
	ZG_CTRSS_FLAGS* pSecurFlags=NULL);

// ���������� �������� ������� �����������
ZGUARD_API(HRESULT) ZG_Ctr_SetCtrMode(HANDLE hHandle, ZG_CTR_MODE nMode);
ZGUARD_API(HRESULT) ZG_Ctr_GetCtrModeInfo(HANDLE hHandle, ZG_CTR_MODE* pCurrMode, PDWORD pFlags);

// ���������� "ElectroControl" ��� Matrix II Net FW v3.X
// ���������� ��������� �������������� (��.���� � �������� 6)
ZGUARD_API(HRESULT) ZG_Ctr_ReadElectroConfig(HANDLE hHandle, PZG_CTR_ELECTRO_CONFIG pConfig);
// ������������� ����� ��������� �������������� (��.���� � �������� 6)
ZGUARD_API(HRESULT) ZG_Ctr_WriteElectroConfig(HANDLE hHandle, PZG_CTR_ELECTRO_CONFIG pConfig, BOOL fSetTz=TRUE);
// ���������� ������ ��������������
ZGUARD_API(HRESULT) ZG_Ctr_GetElectroState(HANDLE hHandle, PZG_CTR_ELECTRO_STATE pState);
// ���./����. ��������������
ZGUARD_API(HRESULT) ZG_Ctr_SetElectroPower(HANDLE hHandle, BOOL fOn);

#else

#define ZG_DLLNAME       _T("ZGuard.dll")

extern "C"
{
	typedef DWORD (__stdcall* ZG_GETVERSION)();
	typedef HRESULT (__stdcall* ZG_INITIALIZE)(UINT nFlags);
	typedef HRESULT (__stdcall* ZG_FINALYZE)();
	typedef HRESULT (__stdcall* ZG_GETPROXYCONVERTERS)(PWORD pSnBuf, int nBufSize, LPINT pRCount, LPCWSTR pIpAddr, 
		LPCSTR pActCode, PZP_WAIT_SETTINGS pWait/*=NULL*/);
	typedef HRESULT (__stdcall* ZG_UPDATECVTFIRMWARE)(PZG_CVT_OPEN_PARAMS pParams, 
		LPCVOID pData, INT nCount, ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);
	typedef HRESULT (__stdcall* ZG_CVT_OPEN)(PHANDLE pHandle, PZG_CVT_OPEN_PARAMS pParams, PZG_CVT_INFO pInfo/*=NULL*/);
	typedef HRESULT (__stdcall* ZG_CVT_DETTACHPORT)(HANDLE hHandle, PHANDLE pPortHandle);
	typedef HRESULT (__stdcall* ZG_CVT_GETCONNECTIONSTATUS)(HANDLE hHandle, ZP_CONNECTION_STATUS* pValue);
	typedef HRESULT (__stdcall* ZG_CVT_GETWAITSETTINGS)(HANDLE hHandle, PZP_WAIT_SETTINGS pSetting);
	typedef HRESULT (__stdcall* ZG_CVT_SETWAITSETTINGS)(HANDLE hHandle, PZP_WAIT_SETTINGS pSetting);
	typedef HRESULT (__stdcall* ZG_CVT_SETCAPTURE)(HANDLE hHandle);
	typedef HRESULT (__stdcall* ZG_CVT_RELEASECAPTURE)(HANDLE hHandle);
	typedef HRESULT (__stdcall* ZG_CVT_CLEAR)(HANDLE hHandle);
	typedef HRESULT (__stdcall* ZG_CVT_SEND)(HANDLE hHandle, LPCVOID pData, INT nCount);
	typedef HRESULT (__stdcall* ZG_CVT_RECEIVE)(HANDLE hHandle, LPVOID pBuf, INT nBufSize, INT nMinRead, LPINT pRCount);
	typedef HRESULT (__stdcall* ZG_CVT_EXEC)(HANDLE hHandle, LPCVOID pData, INT nCount, 
		LPVOID pBuf, INT nBufSize, INT nMinRead, LPINT pRCount);
	typedef HRESULT (__stdcall* ZG_CVT_ENUMCONTROLLERS)(HANDLE hHandle, ZG_ENUMCTRSPROC pEnumProc, PVOID pUserData, UINT nFlags/*=ZG_F_UPDATE*/);
	typedef HRESULT (__stdcall* ZG_CVT_FINDCONTROLLER)(HANDLE hHandle, BYTE nAddr, PZG_FIND_CTR_INFO pInfo, 
		UINT nFlags/*=ZG_F_UPDATE*/, PZP_WAIT_SETTINGS pWait/*=NULL*/);
	typedef HRESULT (__stdcall* ZG_CVT_SEARCHCONTROLLERS)(HANDLE hHandle, INT nMaxCtrs, ZG_CVTS_FLAGS nFlags/*=0*/);
	typedef HRESULT (__stdcall* ZG_CVT_FINDNEXTCONTROLLER)(HANDLE hHandle, PZG_FIND_CTR_INFO pInfo);
	typedef HRESULT (__stdcall* ZG_CVT_GETINFORMATION)(HANDLE hHandle, PZG_CVT_INFO pInfo);
	typedef HRESULT (__stdcall* ZG_CVT_SETNOTIFICATION)(HANDLE hHandle, PZG_CVT_NOTIFY_SETTINGS pSettings);
	typedef HRESULT (__stdcall* ZG_CVT_GETNEXTMESSAGE)(HANDLE hHandle, PUINT pMsg, LPARAM* pMsgParam);
	typedef HRESULT (__stdcall* ZG_CVT_GETSCANCTRSSTATE)(HANDLE hHandle, LPINT pNextAddr);
	typedef HRESULT (__stdcall* ZG_CVT_UPDATEFIRMWARE)(HANDLE hHandle, LPCVOID pData, INT nCount, 
		ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);
	typedef HRESULT (__stdcall* ZG_CVT_GETLICENSE)(HANDLE hHandle, BYTE nLicN, PZG_CVT_LIC_INFO pInfo);
	typedef HRESULT (__stdcall* ZG_CVT_SETLICENSEDATA)(HANDLE hHandle, BYTE nLicN, LPCVOID pData, INT nCount, LPWORD pLicStatus/*=NULL*/);
	typedef HRESULT (__stdcall* ZG_CVT_CLEARALLLICENSES)(HANDLE hHandle);
	typedef HRESULT (__stdcall* ZG_CVT_GETALLLICENSES)(HANDLE hHandle, PZG_CVT_LIC_SINFO pBuf, INT nBufSize, LPINT pCount);
	typedef HRESULT (__stdcall* ZG_CVT_GETSHORTINFO)(HANDLE hHandle, LPWORD pSn, ZG_GUARD_MODE* pMode);
	typedef HRESULT (__stdcall* ZG_CVT_GETLONGINFO)(HANDLE hHandle, PWORD pSn, PDWORD pVersion, ZG_GUARD_MODE* pMode,
		LPSTR pBuf, INT nBufSize, LPINT pLen);
	typedef HRESULT (__stdcall* ZG_CVT_UPDATECTRFIRMWARE)(HANDLE hHandle, WORD nCtrSn, LPCVOID pData, INT nCount, LPCSTR pszInfoStr, 
		ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, ZG_CTR_TYPE nModel/*=ZG_CTR_UNDEF*/);
	typedef HRESULT (__stdcall* ZG_CVT_SETCTRADDRBYSN)(HANDLE hHandle, WORD nSn, BYTE nNewAddr, ZG_CTR_TYPE nModel/*=ZG_CTR_UNDEF*/);
	typedef HRESULT (__stdcall* ZG_CVT_SETCTRADDR)(HANDLE hHandle, BYTE nOldAddr, BYTE nNewAddr, ZG_CTR_TYPE nModel/*=ZG_CTR_UNDEF*/);
	typedef HRESULT (__stdcall* ZG_CVT_GETCTRINFONORM)(HANDLE hHandle, BYTE nAddr, PBYTE pTypeCode, LPWORD pSn, LPWORD pVersion, LPINT pInfoLines, PUINT pFlags, ZG_CTR_TYPE nModel/*=ZG_CTR_UNDEF*/);
	typedef HRESULT (__stdcall* ZG_CVT_GETCTRINFOADV)(HANDLE hHandle, BYTE nAddr, PBYTE pTypeCode, LPWORD pSn, LPWORD pVersion, PUINT pFlags, PINT pEvWrIdx, PINT pEvRdIdx);
	typedef HRESULT (__stdcall* ZG_CVT_GETCTRINFOBYSN)(HANDLE hHandle, WORD nSn, PBYTE pTypeCode, LPBYTE pAddr, LPWORD pVersion, LPINT pInfoLines, PUINT pFlags, ZG_CTR_TYPE nModel/*=ZG_CTR_UNDEF*/);
	typedef HRESULT (__stdcall* ZG_CVT_GETCTRINFOLINE)(HANDLE hHandle, WORD nSn, INT nLineN, LPSTR pBuf, INT nBufSize, LPINT pLen/*=NULL*/, ZG_CTR_TYPE nModel/*=ZG_CTR_UNDEF*/);
	typedef HRESULT (__stdcall* ZG_CVT_GETCTRVERSION)(HANDLE hHandle, BYTE nAddr, LPBYTE pVerData5, PZP_WAIT_SETTINGS pWS/*=NULL*/);
	typedef HRESULT (__stdcall* ZG_CTR_OPEN)(PHANDLE pHandle, HANDLE hCvtHandle, BYTE nAddr, WORD nSn, PZG_CTR_INFO pInfo/*=NULL*/, ZG_CTR_TYPE nModel/*=ZG_CTR_UNDEF*/);
	typedef HRESULT (__stdcall* ZG_CTR_GETINFORMATION)(HANDLE hHandle, PZG_CTR_INFO pInfo);
	typedef HRESULT (__stdcall* ZG_CTR_SETNOTIFICATION)(HANDLE hHandle, PZG_CTR_NOTIFY_SETTINGS pSettings);
	typedef HRESULT (__stdcall* ZG_CTR_GETNEXTMESSAGE)(HANDLE hHandle, PUINT pMsg, LPARAM* pMsgParam);
	typedef HRESULT (__stdcall* ZG_CTR_SETNEWADDR)(HANDLE hHandle, BYTE nNewAddr);
	typedef HRESULT (__stdcall* ZG_CTR_ASSIGNADDR)(HANDLE hHandle, BYTE nAddr);
	typedef HRESULT (__stdcall* ZG_CTR_UPDATEFIRMWARE)(HANDLE hHandle, LPCVOID pData, INT nCount, LPCSTR pszInfoStr, 
		ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);
	typedef HRESULT (__stdcall* ZG_CTR_OPENLOCK)(HANDLE hHandle, INT nLockN/*=0*/);
	typedef HRESULT (__stdcall* ZG_CTR_CLOSELOCK)(HANDLE hHandle);
	typedef HRESULT (__stdcall* ZG_CTR_ENABLEEMERGENCYUNLOCKING)(HANDLE hHandle, BOOL fEnable);
	typedef HRESULT (__stdcall* ZG_CTR_ISEMERGENCYUNLOCKINGENABLED)(HANDLE hHandle, PBOOL pEnabled);
	typedef HRESULT (__stdcall* ZG_CTR_READREGS)(HANDLE hHandle, DWORD nAddr, INT nCount, LPVOID pBuf);
	typedef HRESULT (__stdcall* ZG_CTR_READPORTS)(HANDLE hHandle, LPDWORD pData);
	typedef HRESULT (__stdcall* ZG_CTR_CONTROLDEVICES)(HANDLE hHandle, DWORD nDevType, BOOL fActive, DWORD nTimeMs/*=0*/);
	typedef HRESULT (__stdcall* ZG_CTR_READDATA)(HANDLE hHandle, INT nBankN, DWORD nAddr, INT nCount, LPVOID pBuf, 
		LPINT pReaded, ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);
	typedef HRESULT (__stdcall* ZG_CTR_WRITEDATA)(HANDLE hHandle, INT nBankN, DWORD nAddr, LPCVOID pData, INT nCount, 
		LPINT pWritten, ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);
	typedef HRESULT (__stdcall* ZG_CTR_READLOCKTIMES)(HANDLE hHandle, LPDWORD pOpenMs, LPDWORD pLetMs, LPDWORD pMaxMs, INT nBankN/*=0*/);
	typedef HRESULT (__stdcall* ZG_CTR_WRITELOCKTIMES)(HANDLE hHandle, DWORD nMask, DWORD nOpenMs, DWORD nLetMs, DWORD nMaxMs, INT nBankN/*=0*/);
	typedef HRESULT (__stdcall* ZG_CTR_READTIMEZONES)(HANDLE hHandle, INT nIdx, PZG_CTR_TIMEZONE pBuf, INT nCount, 
		ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, INT nBankN/*=0*/);
	typedef HRESULT (__stdcall* ZG_CTR_WRITETIMEZONES)(HANDLE hHandle, INT nIdx, PZG_CTR_TIMEZONE pTzs, INT nCount,
		ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, INT nBankN/*=0*/);
	typedef HRESULT (__stdcall* ZG_CTR_ENUMTIMEZONES)(HANDLE hHandle, INT nStart, ZG_ENUMCTRTIMEZONESPROC fnEnumProc, PVOID pUserData, INT nBankN/*=0*/);
	typedef HRESULT (__stdcall* ZG_CTR_READKEYS)(HANDLE hHandle, INT nIdx, PZG_CTR_KEY pBuf, INT nCount,
		ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, INT nBankN/*=0*/);
	typedef HRESULT (__stdcall* ZG_CTR_WRITEKEYS)(HANDLE hHandle, INT nIdx, PZG_CTR_KEY pKeys, INT nCount,
		ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, INT nBankN/*=0*/, BOOL fUpdateTop/*=TRUE*/);
	typedef HRESULT (__stdcall* ZG_CTR_CLEARKEYS)(HANDLE hHandle, INT nIdx, INT nCount, 
		ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData, INT nBankN/*=0*/, BOOL fUpdateTop/*=TRUE*/);
	typedef HRESULT (__stdcall* ZG_CTR_GETKEYTOPINDEX)(HANDLE hHandle, LPINT pIdx, INT nBankN/*=0*/);
	typedef HRESULT (__stdcall* ZG_CTR_ENUMKEYS)(HANDLE hHandle, INT nStart, ZG_ENUMCTRKEYSPROC fnEnumProc, PVOID pUserData, INT nBankN/*=0*/);
	typedef HRESULT (__stdcall* ZG_CTR_GETCLOCK)(HANDLE hHandle, PZG_CTR_CLOCK pClock);
	typedef HRESULT (__stdcall* ZG_CTR_SETCLOCK)(HANDLE hHandle, PZG_CTR_CLOCK pClock);
	typedef HRESULT (__stdcall* ZG_CTR_READLASTKEYNUM)(HANDLE hHandle, Z_KEYNUM* pNum);
	typedef HRESULT (__stdcall* ZG_CTR_READRTCSTATE)(HANDLE hHandle, PZG_CTR_CLOCK pClock, 
		LPINT pWrIdx, LPINT pRdIdx, Z_KEYNUM* pNum);
	typedef HRESULT (__stdcall* ZG_CTR_READEVENTIDXS)(HANDLE hHandle, LPINT pWrIdx, LPINT pRdIdx);
	typedef HRESULT (__stdcall* ZG_CTR_WRITEEVENTIDXS)(HANDLE hHandle, UINT nMask, INT nWrIdx, INT nRdIdx);
	typedef HRESULT (__stdcall* ZG_CTR_READEVENTS)(HANDLE hHandle, INT nIdx, PZG_CTR_EVENT pBuf, INT nCount,
		ZG_PROCESSCALLBACK pfnCallback, PVOID pUserData);
	typedef HRESULT (__stdcall* ZG_CTR_ENUMEVENTS)(HANDLE hHandle, INT nStart, INT nCount,
		ZG_ENUMCTREVENTSPROC fnEnumProc, PVOID pUserData);
	typedef HRESULT (__stdcall* ZG_CTR_DECODEPASSEVENT)(HANDLE hHandle, PBYTE pData8, 
		PZG_EV_TIME pTime, ZG_CTR_DIRECT* pDirect, PINT pKeyIdx, PINT pKeyBank);
	typedef HRESULT (__stdcall* ZG_Ctr_DECODEECEVENT)(HANDLE hHandle, PBYTE pData8, 
		PZG_EV_TIME pTime, ZG_EC_SUB_EV* pSubEvent, PDWORD pPowerFlags);
	typedef HRESULT (__stdcall* ZG_CTR_DECODEUNKKEYEVENT)(HANDLE hHandle, PBYTE pData8, PZ_KEYNUM pKeyNum);
	typedef HRESULT (__stdcall* ZG_CTR_DECODEFIREEVENT)(HANDLE hHandle, PBYTE pData8, 
		PZG_EV_TIME pTime, ZG_FIRE_SUB_EV* pSubEvent, PDWORD pFireFlags);
	typedef HRESULT (__stdcall* ZG_CTR_DECODESECUREVENT)(HANDLE hHandle, PBYTE pData8, 
		PZG_EV_TIME pTime, ZG_SECUR_SUB_EV* pSubEvent, PDWORD pSecurFlags);
	typedef HRESULT (__stdcall* ZG_CTR_DECODEMODEEVENT)(HANDLE hHandle, PBYTE pData8,
		PZG_EV_TIME pTime, ZG_CTR_MODE* pCurrMode, ZG_MODE_SUB_EV* pSubEvent);
	typedef HRESULT (__stdcall* ZG_DECODEHOTELEVENT)(PBYTE pData8, 
		PZG_EV_TIME pTime, ZG_HOTEL_MODE* pMode, ZG_HOTEL_SUB_EV* pSubEvent, LPDWORD pFlags);
	typedef HRESULT (__stdcall* ZG_CTR_SETFIREMODE)(HANDLE hHandle, BOOL fOn);
	typedef HRESULT (__stdcall* ZG_CTR_GETFIREINFO)(HANDLE hHandle, ZG_CTRFS_FLAGS* pFireFlags, 
		PDWORD pCurrTemp, ZG_CTRFSR_FLAGS* pSrcMask, PDWORD pLimitTemp);
	typedef HRESULT (__stdcall* ZG_CTR_SETFIRECONFIG)(HANDLE hHandle, ZG_CTRFSR_FLAGS nSrcMask, DWORD nLimitTemp, 
		ZG_CTRFS_FLAGS* pFireFlags, PDWORD pCurrTemp);
	typedef HRESULT (__stdcall* ZG_CTR_SETSECURMODE)(HANDLE hHandle, ZG_SECUR_MODE nMode);
	typedef HRESULT (__stdcall* ZG_CTR_GETSECURINFO)(HANDLE hHandle, ZG_CTRSS_FLAGS* pSecurFlags, 
		ZG_CTRSSR_FLAGS* pSrcMask, PDWORD pAlarmTime);
	typedef HRESULT (__stdcall* ZG_CTR_SETSECURCONFIG)(HANDLE hHandle, ZG_CTRSSR_FLAGS nSrcMask, DWORD nAlarmTime, 
		ZG_CTRSS_FLAGS* pSecurFlags/*=NULL*/);
	typedef HRESULT (__stdcall* ZG_CTR_SETCTRMODE)(HANDLE hHandle, ZG_CTR_MODE nMode);
	typedef HRESULT (__stdcall* ZG_CTR_GETCTRMODEINFO)(HANDLE hHandle, ZG_CTR_MODE* pCurrMode, PDWORD pFlags);
	typedef HRESULT (__stdcall* ZG_CTR_READELECTROCONFIG)(HANDLE hHandle, PZG_CTR_ELECTRO_CONFIG pConfig);
	typedef HRESULT (__stdcall* ZG_CTR_WRITEELECTROCONFIG)(HANDLE hHandle, PZG_CTR_ELECTRO_CONFIG pConfig, BOOL fSetTz/*=TRUE*/);
	typedef HRESULT (__stdcall* ZG_CTR_GETELECTROSTATE)(HANDLE hHandle, PZG_CTR_ELECTRO_STATE pState);
	typedef HRESULT (__stdcall* ZG_CTR_SETELECTROPOWER)(HANDLE hHandle, BOOL fOn);
} // extern "C"

extern ZG_GETVERSION ZG_GetVersion;
extern ZG_INITIALIZE ZG_Initialize;
extern ZG_FINALYZE ZG_Finalyze;
extern ZG_GETPROXYCONVERTERS ZG_GetProxyConverters;
extern ZG_UPDATECVTFIRMWARE ZG_UpdateCvtFirmware;
extern ZG_CVT_OPEN ZG_Cvt_Open;
extern ZG_CVT_DETTACHPORT ZG_Cvt_DettachPort;
extern ZG_CVT_GETCONNECTIONSTATUS ZG_Cvt_GetConnectionStatus;
extern ZG_CVT_GETWAITSETTINGS ZG_Cvt_GetWaitSettings;
extern ZG_CVT_SETWAITSETTINGS ZG_Cvt_SetWaitSettings;
extern ZG_CVT_SETCAPTURE ZG_Cvt_SetCapture;
extern ZG_CVT_RELEASECAPTURE ZG_Cvt_ReleaseCapture;
extern ZG_CVT_CLEAR ZG_Cvt_Clear;
extern ZG_CVT_SEND ZG_Cvt_Send;
extern ZG_CVT_RECEIVE ZG_Cvt_Receive;
extern ZG_CVT_EXEC ZG_Cvt_Exec;
extern ZG_CVT_ENUMCONTROLLERS ZG_Cvt_EnumControllers;
extern ZG_CVT_FINDCONTROLLER ZG_Cvt_FindController;
extern ZG_CVT_SEARCHCONTROLLERS ZG_Cvt_SearchControllers;
extern ZG_CVT_FINDNEXTCONTROLLER ZG_Cvt_FindNextController;
extern ZG_CVT_GETINFORMATION ZG_Cvt_GetInformation;
extern ZG_CVT_SETNOTIFICATION ZG_Cvt_SetNotification;
extern ZG_CVT_GETNEXTMESSAGE ZG_Cvt_GetNextMessage;
extern ZG_CVT_GETSCANCTRSSTATE ZG_Cvt_GetScanCtrsState;
extern ZG_CVT_UPDATEFIRMWARE ZG_Cvt_UpdateFirmware;
extern ZG_CVT_GETLICENSE ZG_Cvt_GetLicense;
extern ZG_CVT_SETLICENSEDATA ZG_Cvt_SetLicenseData;
extern ZG_CVT_CLEARALLLICENSES ZG_Cvt_ClearAllLicenses;
extern ZG_CVT_GETALLLICENSES ZG_Cvt_GetAllLicenses;
extern ZG_CVT_GETSHORTINFO ZG_Cvt_GetShortInfo;
extern ZG_CVT_GETLONGINFO ZG_Cvt_GetLongInfo;
extern ZG_CVT_UPDATECTRFIRMWARE ZG_Cvt_UpdateCtrFirmware;
extern ZG_CVT_SETCTRADDRBYSN ZG_Cvt_SetCtrAddrBySn;
extern ZG_CVT_SETCTRADDR ZG_Cvt_SetCtrAddr;
extern ZG_CVT_GETCTRINFONORM ZG_Cvt_GetCtrInfoNorm;
extern ZG_CVT_GETCTRINFOADV ZG_Cvt_GetCtrInfoAdv;
extern ZG_CVT_GETCTRINFOBYSN ZG_Cvt_GetCtrInfoBySn;
extern ZG_CVT_GETCTRINFOLINE ZG_Cvt_GetCtrInfoLine;
extern ZG_CVT_GETCTRVERSION ZG_Cvt_GetCtrVersion;
extern ZG_CTR_OPEN ZG_Ctr_Open;
extern ZG_CTR_GETINFORMATION ZG_Ctr_GetInformation;
extern ZG_CTR_SETNOTIFICATION ZG_Ctr_SetNotification;
extern ZG_CTR_GETNEXTMESSAGE ZG_Ctr_GetNextMessage;
extern ZG_CTR_SETNEWADDR ZG_Ctr_SetNewAddr;
extern ZG_CTR_ASSIGNADDR ZG_Ctr_AssignAddr;
extern ZG_CTR_UPDATEFIRMWARE ZG_Ctr_UpdateFirmware;
extern ZG_CTR_OPENLOCK ZG_Ctr_OpenLock;
extern ZG_CTR_CLOSELOCK ZG_Ctr_CloseLock;
extern ZG_CTR_ENABLEEMERGENCYUNLOCKING ZG_Ctr_EnableEmergencyUnlocking;
extern ZG_CTR_ISEMERGENCYUNLOCKINGENABLED ZG_Ctr_IsEmergencyUnlockingEnabled;
extern ZG_CTR_READREGS ZG_Ctr_ReadRegs;
extern ZG_CTR_READPORTS ZG_Ctr_ReadPorts;
extern ZG_CTR_CONTROLDEVICES ZG_Ctr_ControlDevices;
extern ZG_CTR_READDATA ZG_Ctr_ReadData;
extern ZG_CTR_WRITEDATA ZG_Ctr_WriteData;
extern ZG_CTR_READLOCKTIMES ZG_Ctr_ReadLockTimes;
extern ZG_CTR_WRITELOCKTIMES ZG_Ctr_WriteLockTimes;
extern ZG_CTR_READTIMEZONES ZG_Ctr_ReadTimeZones;
extern ZG_CTR_WRITETIMEZONES ZG_Ctr_WriteTimeZones;
extern ZG_CTR_ENUMTIMEZONES ZG_Ctr_EnumTimeZones;
extern ZG_CTR_READKEYS ZG_Ctr_ReadKeys;
extern ZG_CTR_WRITEKEYS ZG_Ctr_WriteKeys;
extern ZG_CTR_CLEARKEYS ZG_Ctr_ClearKeys;
extern ZG_CTR_GETKEYTOPINDEX ZG_Ctr_GetKeyTopIndex;
extern ZG_CTR_ENUMKEYS ZG_Ctr_EnumKeys;
extern ZG_CTR_GETCLOCK ZG_Ctr_GetClock;
extern ZG_CTR_SETCLOCK ZG_Ctr_SetClock;
extern ZG_CTR_READLASTKEYNUM ZG_Ctr_ReadLastKeyNum;
extern ZG_CTR_READRTCSTATE ZG_Ctr_ReadRTCState;
extern ZG_CTR_READEVENTIDXS ZG_Ctr_ReadEventIdxs;
extern ZG_CTR_WRITEEVENTIDXS ZG_Ctr_WriteEventIdxs;
extern ZG_CTR_READEVENTS ZG_Ctr_ReadEvents;
extern ZG_CTR_ENUMEVENTS ZG_Ctr_EnumEvents;
extern ZG_CTR_DECODEPASSEVENT ZG_Ctr_DecodePassEvent;
extern ZG_Ctr_DECODEECEVENT ZG_Ctr_DecodeEcEvent;
extern ZG_CTR_DECODEUNKKEYEVENT ZG_Ctr_DecodeUnkKeyEvent;
extern ZG_CTR_DECODEFIREEVENT ZG_Ctr_DecodeFireEvent;
extern ZG_CTR_DECODESECUREVENT ZG_Ctr_DecodeSecurEvent;
extern ZG_CTR_DECODEMODEEVENT ZG_Ctr_DecodeModeEvent;
extern ZG_DECODEHOTELEVENT ZG_DecodeHotelEvent;
extern ZG_CTR_SETFIREMODE ZG_Ctr_SetFireMode;
extern ZG_CTR_GETFIREINFO ZG_Ctr_GetFireInfo;
extern ZG_CTR_SETFIRECONFIG ZG_Ctr_SetFireConfig;
extern ZG_CTR_SETSECURMODE ZG_Ctr_SetSecurMode;
extern ZG_CTR_GETSECURINFO ZG_Ctr_GetSecurInfo;
extern ZG_CTR_SETSECURCONFIG ZG_Ctr_SetSecurConfig;
extern ZG_CTR_SETCTRMODE ZG_Ctr_SetCtrMode;
extern ZG_CTR_GETCTRMODEINFO ZG_Ctr_GetCtrModeInfo;
extern ZG_CTR_READELECTROCONFIG ZG_Ctr_ReadElectroConfig;
extern ZG_CTR_WRITEELECTROCONFIG ZG_Ctr_WriteElectroConfig;
extern ZG_CTR_GETELECTROSTATE ZG_Ctr_GetElectroState;
extern ZG_CTR_SETELECTROPOWER ZG_Ctr_SetElectroPower;

extern HMODULE	hZgLib;
extern INT nZgLoadCount;

#endif // !ZGUARD_LINKONREQUEST

#ifndef ZGUARD_EXPORTS
inline BOOL IsZGuardLoaded()
{
#ifdef ZGUARD_LINKONREQUEST
	return hZgLib != 0;
#else
	return TRUE;
#endif // ZGUARD_LINKONREQUEST
}

HRESULT LoadZGuard(LPCTSTR pszPath=NULL);
VOID UnloadZGuard();
#endif // !ZGUARD_EXPORTS

#ifndef ZGUARD_EXPORTS
#define ZG_GetPortInfoList(pHandle, pCount) (HRESULT)ZP_GetPortInfoList(pHandle, pCount, ZG_DEVTYPE_CVTS, 0)
#define ZG_GetPortInfo	ZP_GetPortInfo
// ����� ��� ������������ ����������
inline HRESULT ZG_SearchDevices(PHANDLE pHandle, PZP_SEARCH_PARAMS pParams, BOOL fSerial=TRUE, BOOL fIP=TRUE)
{
	if (fSerial)
		pParams->nDevMask |= ZG_DEVTYPE_CVTS;
	if (fIP)
		pParams->nIpDevMask |= ZG_IPDEVTYPE_CVTS;
	return ZP_SearchDevices(pHandle, pParams);
};
inline HRESULT ZG_FindNextDevice(HANDLE hHandle, PZG_ENUM_IPCVT_INFO pInfo, 
	PZP_PORT_INFO pPortArr, INT nArrLen, PINT pPortCount, UINT nTimeout=INFINITE)
{
	pInfo->cbSize = sizeof(_ZG_ENUM_IPCVT_INFO);
	return ZP_FindNextDevice(hHandle, pInfo, pPortArr, nArrLen, pPortCount, nTimeout);
}
// ����������� �����������
inline HRESULT ZG_SetNotification(PHANDLE pHandle, PZP_DD_NOTIFY_SETTINGS pSettings, BOOL fSerial, BOOL fIP)
{
	if (fSerial)
		pSettings->nSDevTypes |= ZG_DEVTYPE_CVTS; 
	if (fIP)
		pSettings->nIpDevTypes |= ZG_IPDEVTYPE_CVTS;
	return ZP_DD_SetNotification(pHandle, pSettings);
}
#define ZG_GetNextMessage	ZP_DD_GetNextMessage
#endif // !ZGUARD_EXPORTS

#define ZG_CloseHandle ZP_CloseHandle


//////////////////////////////////////////////////////////////////////////
// ���������� �������

#define ZP_SF_UPDATE	1	// �������� ������ ������
#define ZP_SF_USEVCOM	2	// �� ����������� ������������ Com-����
inline HRESULT ZG_EnumConverters(PZP_PORT_ADDR pPorts, INT nPCount,
	ZG_ENUMCVTSPROC pEnumProc, PVOID pUserData,
	PZP_WAIT_SETTINGS pWait=NULL, UINT nFlags=ZP_SF_UPDATE)
{
	HANDLE hSearch;
	_ZP_SEARCH_PARAMS rParams;
	_ZG_ENUM_CVT_INFO rDI;
	_ZP_PORT_INFO rPI;
	int nPortCount;
	ZeroMemory(&rParams, sizeof(rParams));
	rParams.nDevMask = ZG_DEVTYPE_CVTS;
	if (nFlags & ZP_SF_USEVCOM)
		rParams.nFlags |= ZP_SF_USECOM;
	rParams.pPorts	= pPorts;
	rParams.nPCount	= nPCount;
	rParams.pWait	= pWait;
	HRESULT hr = ZP_SearchDevices(&hSearch, &rParams);
	if (FAILED(hr))
		return hr;
	__try {
		rDI.cbSize = sizeof(rDI);
		while ((hr = ZP_FindNextDevice(hSearch, &rDI, &rPI, 1, &nPortCount, INFINITE)) == S_OK)
		{
			for (int i = 0; i < nPortCount; i++)
				if (!pEnumProc(&rDI, &rPI, pUserData))
					return ZP_S_CANCELLED;
			rDI.cbSize = sizeof(rDI);
		}
	}
	__finally
	{
		ZP_CloseHandle(hSearch);
	}
	return hr;
}
inline HRESULT ZG_EnumIpConverters(ZG_ENUMIPCVTSPROC pEnumProc, PVOID pUserData,
	PZP_WAIT_SETTINGS pWait=NULL, UINT nFlags=ZP_SF_UPDATE)
{
	HANDLE hSearch;
	_ZP_SEARCH_PARAMS rParams;
	_ZG_ENUM_IPCVT_INFO rDI;
	_ZP_PORT_INFO rPI;
	int nPortCount;
	ZeroMemory(&rParams, sizeof(rParams));
	rParams.nIpDevMask = ZG_IPDEVTYPE_CVTS;
	if (nFlags & ZP_SF_USEVCOM)
		rParams.nFlags |= ZP_SF_USECOM;
	rParams.pWait = pWait;
	HRESULT hr = ZP_SearchDevices(&hSearch, &rParams);
	if (FAILED(hr))
		return hr;
	__try
	{
		rDI.cbSize = sizeof(rDI);
		while ((hr = ZP_FindNextDevice(hSearch, &rDI, &rPI, 1, &nPortCount, INFINITE)) == S_OK)
		{
			for (int i = 0; i < nPortCount; i++)
				if (!pEnumProc(&rDI, &rPI, pUserData))
					return ZP_S_CANCELLED;
			rDI.cbSize = sizeof(rDI);
		}
	}
	__finally
	{
		ZP_CloseHandle(hSearch);
	}
	return hr;
}
inline HRESULT ZG_FindConverter(PZP_PORT_ADDR pPorts, INT nPCount,
	PZG_ENUM_CVT_INFO pInfo, PZP_PORT_INFO pPort,
	PZP_WAIT_SETTINGS pWait=NULL, UINT nFlags=ZP_SF_UPDATE)
{
	HANDLE hSearch;
	_ZP_SEARCH_PARAMS rParams;
	_ZG_ENUM_CVT_INFO rDI;
	_ZP_PORT_INFO aPIs[2];
	int nPortCount;
	ZeroMemory(&rParams, sizeof(rParams));
	rParams.nDevMask = ZG_DEVTYPE_CVTS;
	if (nFlags & ZP_SF_USEVCOM)
		rParams.nFlags |= ZP_SF_USECOM;
	rParams.pPorts	= pPorts;
	rParams.nPCount	= nPCount;
	rParams.pWait	= pWait;
	HRESULT hr = ZP_SearchDevices(&hSearch, &rParams);
	if (FAILED(hr))
		return hr;
	__try
	{
		rDI.cbSize = sizeof(rDI);
		ZeroMemory(aPIs, sizeof(aPIs));
		hr = ZP_FindNextDevice(hSearch, &rDI, aPIs, _countof(aPIs), &nPortCount, INFINITE);
		if (FAILED(hr))
			return hr;
		if (hr == S_OK)
		{
			*pInfo = rDI;
			*pPort = aPIs[0];
		}
	}
	__finally
	{
		ZP_CloseHandle(hSearch);
	}
	return hr;
}
inline HRESULT ZG_Cvt_EnumMessages(HANDLE hHandle, ZP_NOTIFYPROC pEnumProc, PVOID pUserData)
{
	HRESULT hr;
	UINT nMsg;
	LPARAM nMsgParam;
	while ((hr = ZG_Cvt_GetNextMessage(hHandle, &nMsg, &nMsgParam)) == S_OK)
		pEnumProc(nMsg, nMsgParam, pUserData);
	if (hr == ZP_S_NOTFOUND)
		hr = S_OK;
	return hr;
}
inline HRESULT ZG_Ctr_EnumMessages(HANDLE hHandle, ZP_NOTIFYPROC pEnumProc, PVOID pUserData)
{
	HRESULT hr;
	UINT nMsg;
	LPARAM nMsgParam;
	while ((hr = ZG_Ctr_GetNextMessage(hHandle, &nMsg, &nMsgParam)) == S_OK)
		pEnumProc(nMsg, nMsgParam, pUserData);
	if (hr == ZP_S_NOTFOUND)
		hr = S_OK;
	return hr;
}

//#pragma deprecated(ZG_STATUS)
typedef HRESULT ZG_STATUS;	// ������ ���������� ������� DLL

#pragma deprecated(ZG_SUCCESS)
#define ZG_SUCCESS						S_OK
#pragma deprecated(ZG_E_CANCELLED)
#define ZG_E_CANCELLED					ZP_S_CANCELLED
#pragma deprecated(ZG_E_NOT_FOUND)
#define ZG_E_NOT_FOUND					ZP_S_NOTFOUND
#pragma deprecated(ZG_E_INVALID_PARAM)
#define ZG_E_INVALID_PARAM				E_INVALIDARG
#pragma deprecated(ZG_E_OPEN_NOT_EXIST)
#define ZG_E_OPEN_NOT_EXIST				ZP_E_OPENNOTEXIST
#pragma deprecated(ZG_E_OPEN_ACCESS)
#define ZG_E_OPEN_ACCESS				ZP_E_OPENACCESS
#pragma deprecated(ZG_E_OPEN_PORT)
#define ZG_E_OPEN_PORT					ZP_E_OPENPORT
#pragma deprecated(ZG_E_PORT_IO_ERROR)
#define ZG_E_PORT_IO_ERROR				ZP_E_PORTIO
#pragma deprecated(ZG_E_PORT_SETUP)
#define ZG_E_PORT_SETUP					ZP_E_PORTSETUP
#pragma deprecated(ZG_E_LOAD_FTD2XX)
#define ZG_E_LOAD_FTD2XX				ZP_E_LOADFTD2XX
#pragma deprecated(ZG_E_INIT_SOCKET)
#define ZG_E_INIT_SOCKET				ZP_E_SOCKET
#pragma deprecated(ZG_E_SERVERCLOSE)
#define ZG_E_SERVERCLOSE				ZP_E_SERVERCLOSE
#pragma deprecated(ZG_E_NOT_ENOUGH_MEMORY)
#define ZG_E_NOT_ENOUGH_MEMORY			E_OUTOFMEMORY
#pragma deprecated(ZG_E_UNSUPPORT)
#define ZG_E_UNSUPPORT					E_NOINTERFACE
#pragma deprecated(ZG_E_NOT_INITALIZED)
#define ZG_E_NOT_INITALIZED				ZP_E_NOTINITALIZED
#pragma deprecated(ZG_E_CREATE_EVENT)
#define ZG_E_CREATE_EVENT				E_FAIL
#pragma deprecated(ZG_E_TOO_LARGE_MSG)
#define ZG_E_TOO_LARGE_MSG				ZG_E_TOOLARGEMSG
#pragma deprecated(ZG_E_INSUFFICIENT_BUFFER)
#define ZG_E_INSUFFICIENT_BUFFER		ZP_E_INSUFFICIENTBUFFER
#pragma deprecated(ZG_E_NO_ANSWER)
#define ZG_E_NO_ANSWER					ZG_E_NOANSWER
#pragma deprecated(ZG_E_BAD_ANSWER)
#define ZG_E_BAD_ANSWER					ZG_E_BADANSWER
#pragma deprecated(ZG_E_ONLY_GUARD)
#define ZG_E_ONLY_GUARD					E_NOINTERFACE
#pragma deprecated(ZG_E_WRONG_ZPORT_VERSION)
#define ZG_E_WRONG_ZPORT_VERSION		ZG_E_WRONGZPORT
#pragma deprecated(ZG_E_CVT_BUSY)
#define ZG_E_CVT_BUSY					ZG_E_CVTBUSY
#pragma deprecated(ZG_E_G_ONLY_ADVANCED)
#define ZG_E_G_ONLY_ADVANCED			E_NOINTERFACE
#pragma deprecated(ZG_E_G_OTHER)
#define ZG_E_G_OTHER					ZG_E_CVTERROR
#pragma deprecated(ZG_E_G_LIC_OTHER)
#define ZG_E_G_LIC_OTHER				ZG_E_CVTERROR
#pragma deprecated(ZG_E_G_LIC_NOT_FOUND)
#define ZG_E_G_LIC_NOT_FOUND			ZG_E_LICNOTFOUND
#pragma deprecated(ZG_E_G_LIC_EXPIRED)
#define ZG_E_G_LIC_EXPIRED				ZG_E_LICEXPIRED
#pragma deprecated(ZG_E_G_LIC_CTR_LIM)
#define ZG_E_G_LIC_CTR_LIM				ZG_E_LICONTROLLERS
#pragma deprecated(ZG_E_G_LIC_RKEY_LIM)
#define ZG_E_G_LIC_RKEY_LIM				ZG_E_LICREADKEYS
#pragma deprecated(ZG_E_G_LIC_WKEY_LIM)
#define ZG_E_G_LIC_WKEY_LIM				ZG_E_LICWRITEKEYS
#pragma deprecated(ZG_E_G_LIC_EXPIRED2)
#define ZG_E_G_LIC_EXPIRED2				ZG_E_LICEXPIRED2
#pragma deprecated(ZG_E_G_BAD_CS)
#define ZG_E_G_BAD_CS					ZG_E_CVTERROR
#pragma deprecated(ZG_E_G_CTR_NOT_FOUND)
#define ZG_E_G_CTR_NOT_FOUND			ZG_E_NOCONTROLLER
#pragma deprecated(ZG_E_G_CMD_UNSUPPORT)
#define ZG_E_G_CMD_UNSUPPORT			ZG_E_CVTERROR
#pragma deprecated(ZG_E_CTR_NACK)
#define ZG_E_CTR_NACK					ZG_E_CTRNACK
#pragma deprecated(ZG_E_CTR_TRANSFER)
#define ZG_E_CTR_TRANSFER				ZG_E_NOCONTROLLER
#pragma deprecated(ZG_E_BOOTLOADER_NOSTART)
#define ZG_E_BOOTLOADER_NOSTART			ZG_E_FWBOOTLOADERNOSTART
#pragma deprecated(ZG_E_FIRMWARE_FILESIZE)
#define ZG_E_FIRMWARE_FILESIZE			ZG_E_FWFILESIZE
#pragma deprecated(ZG_E_FIRMWARE_NOSTART)
#define ZG_E_FIRMWARE_NOSTART			ZG_E_FWNOSTART
#pragma deprecated(ZG_E_FW_NO_COMPATIBLE)
#define ZG_E_FW_NO_COMPATIBLE			ZG_E_FWNOCOMPATIBLE
#pragma deprecated(ZG_E_FW_INVALID_DEV_NUM)
#define ZG_E_FW_INVALID_DEV_NUM			ZG_E_FWINVALIDDEVNUM
#pragma deprecated(ZG_E_FW_TOOLARGE)
#define ZG_E_FW_TOOLARGE				ZG_E_FWTOOLARGE
#pragma deprecated(ZG_E_FW_SEQUENCE_DATA)
#define ZG_E_FW_SEQUENCE_DATA			ZG_E_FWSEQUENCEDATA
#pragma deprecated(ZG_E_FW_DATA_INTEGRITY)
#define ZG_E_FW_DATA_INTEGRITY			ZG_E_FWDATAINTEGRITY
#pragma deprecated(ZG_E_FW_OTHER)
#define ZG_E_FW_OTHER					E_FAIL
#pragma deprecated(ZG_E_OTHER)
#define ZG_E_OTHER						E_FAIL

#pragma deprecated(ZG_NF_CVT_WND_SYNC)
#define ZG_NF_CVT_WND_SYNC			0x4000
#pragma deprecated(ZG_NF_CVT_ONLY_NOTIFY)
#define ZG_NF_CVT_ONLY_NOTIFY		0x8000
#pragma deprecated(ZG_KF_SHORT_NUM)
#define ZG_KF_SHORT_NUM			ZG_KF_SHORTNUM
#pragma deprecated(ZG_KF_ANTIPASSBACK)
#define ZG_KF_ANTIPASSBACK		2
#pragma deprecated(ZG_IF_ERROR_LOG)
#define ZG_IF_ERROR_LOG			ZG_IF_LOG

#pragma deprecated(ZG_CloseNotification)
#define ZG_CloseNotification ZP_CloseHandle
#pragma deprecated(ZG_FindNotification)
#define ZG_FindNotification	ZG_SetNotification
#pragma deprecated(ZG_ProcessMessages)
#define ZG_ProcessMessages	ZP_EnumMessages
#pragma deprecated(ZG_EnumMessages)
#define ZG_EnumMessages	ZP_EnumMessages
#pragma deprecated(ZG_Cvt_Close)
#define ZG_Cvt_Close ZG_CloseHandle
#pragma deprecated(ZG_Ctr_Close)
#define ZG_Ctr_Close ZG_CloseHandle
#pragma deprecated(ZG_Cvt_FindNotification)
#define ZG_Cvt_FindNotification ZG_Cvt_SetNotification
#pragma deprecated(ZG_Cvt_ProcessMessages)
#define ZG_Cvt_ProcessMessages ZG_Cvt_EnumMessages
#pragma deprecated(ZG_Ctr_FindNotification)
#define ZG_Ctr_FindNotification ZG_Ctr_SetNotification
#pragma deprecated(ZG_Ctr_ProcessMessages)
#define ZG_Ctr_ProcessMessages ZG_Ctr_EnumMessages

