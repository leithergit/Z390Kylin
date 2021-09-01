#pragma once

#ifdef _WIN32
#  include <Windows.h>
#else
	#define DECLARE_HANDLE(x)  typedef WORD x

	#ifndef CONST
	#define CONST               const
	#endif

	#ifndef NULL
	#define NULL 0
	#endif

	# if !defined(__LFSDBUS__) && !defined(__PISADBUS__)
	#define FALSE   0
	#define TRUE    1
	typedef unsigned int UINT;
	typedef unsigned short USHORT;
	typedef char CHAR;
	typedef wchar_t WCHAR; 
	typedef unsigned int   DWORD;
	typedef DWORD* LPDWORD,*PDWORD;
	typedef short SHORT;
	typedef unsigned short      WORD;
	typedef WORD* LPWORD;
	typedef unsigned long ULONG;
	typedef unsigned char UCHAR;
	typedef SHORT * LPSHORT;
	typedef void* LPVOID;
	typedef LPVOID * LPLPVOID;
	typedef ULONG * LPULONG;
	typedef USHORT * LPUSHORT;
	typedef void *HANDLE;
	typedef void VOID;
	typedef VOID* PVOID;
	typedef HANDLE HPROVIDER;
	typedef ULONG REQUESTID;
	typedef REQUESTID * LPREQUESTID;
	typedef HANDLE HAPP;
	typedef HAPP * LPHAPP;
	typedef USHORT HSERVICE;
	typedef HSERVICE * LPHSERVICE;
	typedef long LONG;
	typedef LONG HRESULT;
	typedef LONG LRESULT;
	typedef HRESULT * LPHRESULT;
	typedef int BOOL;
	typedef BOOL *LPBOOL,*PBOOL;

	typedef CHAR *NPSTR, *LPSTR,*PSTR;
	typedef unsigned char       BYTE;
	typedef BYTE *LPBYTE,*PBYTE;
	#ifdef __DBUSBUILD__
		typedef char* HWND;
	#else
		typedef VOID* HWND;
	#endif
	typedef  CONST CHAR *LPCSTR, *PCSTR;
	typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;
	typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;
	typedef  WCHAR *NWPSTR, *LPWSTR;
	typedef  CONST WCHAR *LPCWSTR, *PCWSTR;
	typedef unsigned int UINT32, *PUINT32;
	typedef HANDLE HPROVIDER;
	typedef ULONG REQUESTID;
	typedef REQUESTID * LPREQUESTID;
	typedef HANDLE HAPP;
	typedef HAPP * LPHAPP;
	typedef USHORT HSERVICE;
	typedef HSERVICE * LPHSERVICE;
	DECLARE_HANDLE(HINSTANCE);
	typedef HINSTANCE HMODULE;
	typedef UINT WPARAM;
	typedef LONG LPARAM;
    #endif
#endif

namespace Lithograph
{
	typedef struct _LITHOGRAPHMAGINFO 
	{
		int nTrack1Len;		//TRACK1����
		int nTrack2Len;		//TRACK2����
		int nTrack3Len;		//TRACK3����
		unsigned char track1_data[80];	//TRACK1����
		unsigned char track2_data[80];	//TRACK2����
		unsigned char track3_data[170];	//TRACK3����
		WORD	wTrack1Method;			//TRACK1д�ŷ�ʽ,�Ϳ�0x0002,�߿�0x0004���Զ�0x0008
		WORD	wTrack2Method;			//TRACK2д�ŷ�ʽ,�Ϳ�0x0002,�߿�0x0004���Զ�0x0008
		WORD	wTrack3Method;			//TRACK3д�ŷ�ʽ,�Ϳ�0x0002,�߿�0x0004���Զ�0x0008
	}LITHOGRAPHMAGINFO,*LPLITHOGRAPHMAGINFO;

	typedef struct _LITHOGRAPHBOXUNIT
	{
		BYTE byType;			//�������� 0:δ֪��1�������䣬2��������
		BYTE byBoxNumber;		//������
		BYTE byStatus;			//����״̬,0������; 1������; 2:�޿�; 3�����ɲ���; 4��������; 5����(����)��6������ 7��δ֪
		BOOL bHardwareSensor;	//�Ƿ�֧�ֹ���״̬������,TRUE:֧�֣�FALSE����֧��
	}LITHOGRAPHBOXUNIT,*LPLITHOGRAPHBOXUNIT;

	typedef struct _LITHOGRAPHBOXINFO
	{
		int nCount;				//�����ܸ���(���п���)
		LPLITHOGRAPHBOXUNIT lpplist;		//������Ϣ�ṹ��ָ��
	}LITHOGRAPHBOXINFO,*LPLITHOGRAPHBOXINFO;

	typedef struct _LITHOGRAPHSTATUS
	{
		WORD fwDevice;			//��ӡ��״̬, 0-���ߣ�1-æ��2-�����ߣ�3-����
		WORD fwMedia;			//����״̬��0-�޿���1-�����ſڣ�2-�����ڲ���3-�����ϵ�λ��4-����բ���⣻5-�¿���6-��Ƭδ֪������Ӳ�����Է���,����֧�����޿����⣩
		WORD fwToner;			//ƽӡɫ��״̬,0-FLLL;1-LOW;2-OUT;3-NOTSUPP;4-UNKNOW
	}LITHOGRAPHSTATUS,*LPLITHOGRAPHSTATUS;

	//��λ
	enum ResetAction
	{
		RESET_HOLD = 1,
		RESET_EJECT,
		RESET_RETAIN,
	};

	enum CheckMag
	{
		NONCHECK_MAG = 0,
		CHECK_MAG,
	};

	enum TrackId
	{
		TRACK1 = 0x0001,
		TRACK2 = 0x0002,
		TRACK3 = 0x0003,
	};
	
	enum WriteMethod
	{
		LOCO = 0x0002,
		HICO = 0x0004,
		AUTO = 0x0008,
	};
	
	enum DispensePos
	{
		DISP_TO_TRACK_POS = 1,
		DISP_TO_CONTACT_POS = 2,
		DISP_TO_CONTACTLESS_POS = 3,
		DISP_TO_PRINT_POS = 4,
	};

	enum BoxType
	{
		UNKNOW_BOX = 0,
		SUPPLY_BOX = 1,
		RETAIN_BOX = 2,
	};

	enum BoxStatus
	{
		LITH_BOX_OK = 0,
		LITH_BOX_LOW,
		LITH_BOX_EMPTY,
		LITH_BOX_INOP,
		LITH_BOX_MISS,
		LITH_BOX_HIGH,
		LITH_BOX_FULL,
		LITH_BOX_UNKNOW,
	};
	
	enum PrinterStatus
	{
		DEVICE_ONLINE = 0,
		DEVICE_BUSY,
		DEVICE_OFFLINE,
		DEVICE_HWERR,
	};
	
	enum MediaStatus
	{
		NO_CARD = 0,
		CARD_GATE,
		CARD_INNER,
		CARD_IC_POS,
		CARD_RF_POS,
		CARD_JAM,
		CARD_UNKNOW
	};
	
	enum TonnerStatus
	{
		TONNER_FULL = 0,
		TONNER_LOW,
		TONNER_OUT,
		TONNER_NOTSUPP,
		TONNER_UNKNOW
	};

	enum FontStyle
	{
		FONT_NORMAL = 1,
		FONT_BLOD = 2,
		FONT_ITALIC = 4,
		FONT_BLACK = 8
	};

	enum CardType
	{
		CARD_IC,
		CARD_PSAM
	};
}


class CLithographPrinter
{
public:
	CLithographPrinter(void){};
	virtual ~CLithographPrinter(void){};
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  �����豸
	  * @param[in] pPort �˿ں�,U���豸��USB��usb�������豸��COM+������������COM1�������豸: IP
	  * @param[in] pPortParam �˿ڲ�����U���豸:  USB�豸������VID\PID;�����豸����ʽΪ������+��ż��Уλ+��ʼλ+��ֹλ�����磺57600,n,8,1;�����豸��IP��ַ�� ���� 172.0.0.1
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_Open(char *pPort, char *pPortParam, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  �ر��豸
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_Close(char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  ��λ�豸
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[in] nResetAction 1-��λ���������豸�ڣ�2-��λ�˿�����ӡ��ȡ��λ�ã�3-��λ�̿�����ӡ��������(��Ӳ��֧����ʵ��)
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_Reset(long lTimeout, int nResetAction, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  ��ȡ�̼��汾�Լ���̬���汾��Ϣ
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[out] pVersionInfo �̼��汾��Ϣ
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_GetVersion(long lTimeout, char *pVersionInfo, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  ��������
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[in] nCheckMode �Ƿ�����,0-�����ţ�1-����(��֧����ʵ��,������������)
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_EnableCard (long lTimeout, int nCheckMode, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  ȡ������
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_DisableCard(long lTimeout,  char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  �˿����ſ�
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_Eject(long lTimeout, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  �̿�����ӡ��������
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[in] nBoxNo �̿�������
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  ������(��֧����ʵ��)
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[in] wTrackId ��Ҫ��ȡ�Ĵŵ�,0x0001-��1�ţ�0x0002-��2�ţ�0x0004-��3��,����Ҫ��������ʱ,���򼴿�,Ʃ����1��2��3����,������0x0007
	  * @param[out] pTrackInfo �����Ĵ�������,�������ṹ�嶨��
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_ReadTrack(long /*lTimeout*/, WORD /*wTrackId*/, Lithograph::LITHOGRAPHMAGINFO* /*pTrackInfo*/, char* /*pszRcCode*/) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  д����(��֧����ʵ��)
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[in] wTrackId ��Ҫд�ŵ�,0x0001-��1�ţ�0x0002-��2�ţ�0x0004-��3��,����Ҫд������ʱ,���򼴿�,Ʃ��д1��2��3����,������0x0007 
	  * @param[in] pTrackInfo ��Ҫд�Ĵ�������,�ṹ�嶨��ͬ�������ӿ�
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_WriteTrack(long /*lTimeout*/, WORD /*wTrackId*/, Lithograph::LITHOGRAPHMAGINFO* /*pTrackInfo*/, char* /*pszRcCode*/) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  IC���ϵ�(��֧����ʵ��)
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[out] byOutAtr ATR����,HEX
	  * @param[out] nAtrlen ATR���ݳ���
	  * @param[out] byUID ��ʹ�÷ǽ�ʱ�践��UID,���򷵻ؿ�""
	  * @param[out] nUidLen ��ʹ�÷ǽ�ʱ����UID����,���򳤶�Ϊ0
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_IcPowerOn(long /*lTimeout*/, BYTE* /*byOutAtr*/, int& /*nAtrlen*/, BYTE* /*byUID*/, int& /*nUidLen*/, char* /*pszRcCode*/) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  IC���µ�(��֧����ʵ��)
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_IcPowerOff(long /*lTimeout*/, char* /*pszRcCode*/) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  IC�����ݽ���(��֧����ʵ��)
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[in] byIndata ����ָ��HEX
	  * @param[in] nInDataLen ����ָ���
	  * @param[out] pOutData ��Ӧ����HEX
	  * @param[out] nOutLen ��Ӧ���ݳ���
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_IcExchange(long /*lTimeout*/, BYTE* /*byIndata*/, int /*nInDataLen*/, BYTE* /*pOutData*/, int& /*nOutLen*/, char* /*pszRcCode*/) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  �ӿ��䷢��(��֧����ʵ��)
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[in] nBox ������
	  * @param[in] nDispPos ����λ��:1-����������λ�ã�2-�������Ӵ�IC��λ��;3-�������ǽ�IC��λ��;4-��������ӡλ��
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_Dispense(long /*lTimeout*/, int /*nBox*/, int /*nDispPos*/, char* /*pszRcCode*/) {return 0;};
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  ��ȡ����״̬,���������䡢�����䡢�Ͽ����ȣ���Ӳ��֧����ʵ�֣�
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[out] lpBoxInfo ����״̬��Ϣ�ṹ��
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_GetBoxStatus(long /*lTimeout*/, Lithograph::LPLITHOGRAPHBOXINFO& /*lpBoxInfo*/, char* /*pszRcCode*/) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  ��ȡ��ӡ��״̬
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[out] lpStatus ��ӡ����Ϣ�ṹ��,������
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  ��ʼ����ӡ,�ڴ�ӡͼƬ�����֡�����ǰ����
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_InitPrint(long lTimeout, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  Ԥ��ӡͼƬ
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[in] pImage ͼƬȫ·��
	  * @param[in] nAngle ͼƬ˳ʱ����ת�Ƕ�
	  * @param[in] fxPos ��ӡX����
	  * @param[in] fyPos ��ӡY����
	  * @param[in] fHeight ��ӡ�߶�
	  * @param[in] fWidth ��ӡ����
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_PrintImage(long lTimeout, char *pImage, int nAngle, float fxPos,  float fyPos,  float fHeight,  float fWidth,  char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  Ԥ��ӡ����
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[in] pText ��ӡ������
	  * @param[in] nAngle ˳ʱ����ת�Ƕ�,���ڴ�ӡˮƽ����ֱ����
	  * @param[in] fxPos ��ӡX����
	  * @param[in] fyPos ��ӡY����
	  * @param[in] pFontName ��ӡ����,����"����"
	  * @param[in] nFontSize ������С
	  * @param[in] nFontStyle ����������1-���棻2-���壻4-б�壻8-����
	  * @param[in] nColor ��ӡ������ɫ,RGB��ɫֵ
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_PrintText(long lTimeout, char *pText, int nAngle, float fxPos,  float fyPos, char *pFontName, int nFontSize, int nFontStyle, int nColor, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  Ԥ��ӡ����
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[in] nAngle ������ת�Ƕ�,������ֱ��ˮƽ��ӡ����
	  * @param[in] pBarcodeType ��������,Ʃ��"c39"��"2/5"��"c128"��"c128c"��"ean13"��,��֧��������������,����˵���ĵ��н���˵��
	  * @param[in] fxPos ��ӡX����
	  * @param[in] fyPos ��ӡY����
	  * @param[in] nMultiplier ��Ƶϵ��
	  * @param[in] nHeight �����߶�
	  * @param[in] nIsActive ����ֵ����,0������,1����
	  * @param[in] pBarcodeText ��ӡ����������
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_PrintBarcode(long lTimeout, int nAngle, char *pBarcodeType, float fxPos,  float fyPos, int nMultiplier, int nHeight, int nIsActive, char *pBarcodeText, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  �ύ����,��ʼ��ӡ
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_StartPrint(long lTimeout, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  ��ת��Ƭ
	  * @param[in] lTimeout ��ʱʱ��,��λms
	  * @param[in] nAngle ��ת�Ƕ�,һ��ֻ֧��180�ȷ�ת��Ƭ
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_Rotate(long /*lTimeout*/, int /*nAngle*/, char* /*pszRcCode*/) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  �ر�����
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_CloseSleepMode(long /*lTimeout*/, char* /*pszRcCode*/){return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  ��չ����
	  * @param[in] pCommand �����ַ���
	  * @param[in] lpCmdIn ��������
	  * @param[in] lpCmdOut ��������
	  * @param[out] pszRcCode ʧ��ʱ����4λ�����룬�ɹ�ʱ����"0000"
	  * @return 0���ɹ���1��ʧ��
	 **/
	virtual int Print_ExtraCommand(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode) = 0;
};

