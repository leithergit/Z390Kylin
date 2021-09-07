// -*- mode:c++ -*-

/**
 * @file   dcrf32.h
 * @author WeiYang <604710872@qq.com>
 * @date   Wed Jan 20 10:00:00 2019
 * @brief  ��д�����ܽӿ������ļ���
 */

#ifndef DCRF32_H_
#define DCRF32_H_

#ifdef WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define USER_API __stdcall
#else
typedef int DEVHANDLE;
#define USER_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @brief  ����ڡ�
   * @par    ˵����
   * ���Ի�ȡ������һЩ����ز������˽ӿڿɲ����ã�������ñ���������������ӿ�֮ǰ���á�
   * @param[in] flag ��־�����ھ��� @a context �����ͺͺ��塣
   * @n 0 - ��ʾ��ȡ��汾�� @a context ����Ϊchar *�������ٷ���64���ֽڡ�
   * @n 1 - ��ʾ���ÿ�Ĺ���Ŀ¼�� @a context ����Ϊconst char *��
   * @n 2 - ��ʾ���ÿ�����ߵĹ���Ŀ¼�� @a context ����Ϊconst char *��
   * @n 3 - �����ڲ�ʹ�ã���ʾע��˿ڸ����ӿ������ģ� @a context ����Ϊ�Զ���ṹָ�롣
   * @n 4 - �����ڲ�ʹ�ã���ʾ��ȡ�ڲ���֤�ӿ������ģ� @a context ����Ϊ�Զ���ṹָ�롣
   * @n 5 - �����ڲ�ʹ�ã����ÿ�İ汾�� @a context ����Ϊconst char *��
   * @n 6 - �����ڲ�ʹ�ã���ʾע����־�����ӿ������ģ� @a context ����Ϊ�Զ���ṹָ�롣
   * @n 7 - �����ڲ�ʹ�ã�ʹ��ע��ӿڿ���Ի�ȡ���������ģ� @a context ����Ϊ�Զ���ṹָ�롣
   * @param[in,out] context ����ʵ�����ͺͺ����� @a flag ��ֵ��������
   */
  void USER_API LibMain(int flag, void *context);

  /**
   * @brief  ���ö˿����ơ�
   * @par    ˵����
   * �������ö˿ڶ�Ӧ���������ƣ��ڵ��� ::dc_init ֮ǰ����ʹ�ô˽ӿ����ı�˿ں��ڲ���Ӧ��Ĭ���������ƣ���Windowsƽ̨��0�˿ںŶ�Ӧ��Ĭ������Ϊ"COM1"��Linuxƽ̨��0�˿ںŶ�Ӧ��Ĭ������Ϊ"/dev/ttyS0"��
   * @param[in] port �˿ںţ�ͬ ::dc_init �� @a port ��
   * @param[in] name �������ơ�
   */
  void USER_API dc_config_port_name(short port, const char *name);

  /**
   * @brief  ���ö˿ڹ���ģʽ��
   * @par    ˵����
   * �������ö˿ڵĹ���ģʽ���ڵ��� ::dc_init ֮ǰ����ʹ�ô˽ӿ����ı�˿ڴ���ʹ�õĹ���ģʽ��ע�⣺����ģʽ��ʹ�úͲ���ϵͳƽ̨֧�ֵĶ˿ڲ���ģʽ������أ�ͨ�����������ʹ�á�
   * @param[in] port �˿ںţ�ͬ ::dc_init �� @a port ��
   * @param[in] mode ģʽ��0��ʾ��������Ĭ����Ϊ����1��ʾ������
   */
  void USER_API dc_config_port_share_mode(short port, int mode);

  /**
   * @brief  ���豸��
   * @par    ˵����
   * �����豸��ͨѶ���ҷ�����Ӧ����Դ���󲿷ֹ��ܽӿڶ���Ҫ�ڴ˹��̺���ܽ��У��ڲ���Ҫʹ���豸�󣬱���ʹ�� ::dc_exit ȥ�ر��豸��ͨѶ���ͷ���Դ��
   * @param[in] port �˿ںš�
   * @n 0~99 - ��ʾ����ģʽ�����������Ӧ�������0��ʾ��һ�����ںϷ��豸�����1��ʾ�ڶ������ںϷ��豸���Դ����ơ�
   * @n 100~199 - ��ʾUSBģʽ������߼���Ӧ�������100��ʾ��һ��USB�Ϸ��豸�����101��ʾ�ڶ���USB�Ϸ��豸���Դ����ơ�
   * @n 200~299 - ��ʾPCSCģʽ������߼���Ӧ�������200��ʾ��һ��PCSC�Ϸ��豸�����201��ʾ�ڶ���PCSC�Ϸ��豸���Դ����ơ�
   * @n 300~399 - ��ʾ����ģʽ������߼���Ӧ�������300��ʾ��һ�������Ϸ��豸�����301��ʾ�ڶ��������Ϸ��豸���Դ����ơ�
   * @n 400~499 - ��ʾ��������ģʽ������߼���Ӧ�����ڲ�ʹ�á�
   * @n 500~599 - ��ʾ����SPIģʽ������߼���Ӧ�����ڲ�ʹ�á�
   * @n 600~699 - ��ʾ����ģʽ������߼���Ӧ�����ڲ�ʹ�á�
   * @param[in] baud �����ʣ�ֻ��Դ���ģʽ��Ч��
   * @return <0��ʾʧ�ܣ�����Ϊ�豸��ʶ����
   */
  DEVHANDLE USER_API dc_init(short port, int baud);

  /**
   * @brief  ���豸��
   * @par    ˵����
   * �����豸��ͨѶ���ҷ�����Ӧ����Դ���󲿷ֹ��ܽӿڶ���Ҫ�ڴ˹��̺���ܽ��У��ڲ���Ҫʹ���豸�󣬱���ʹ�� ::dc_exit ȥ�ر��豸��ͨѶ���ͷ���Դ��
   * @param[in] port �˿ںš�
   * @n 0~99 - ��ʾ����ģʽ�����������Ӧ�������0��ʾ��һ�����ںϷ��豸�����1��ʾ�ڶ������ںϷ��豸���Դ����ơ�
   * @n 100~199 - ��ʾUSBģʽ������߼���Ӧ�������100��ʾ��һ��USB�Ϸ��豸�����101��ʾ�ڶ���USB�Ϸ��豸���Դ����ơ�
   * @n 200~299 - ��ʾPCSCģʽ������߼���Ӧ�������200��ʾ��һ��PCSC�Ϸ��豸�����201��ʾ�ڶ���PCSC�Ϸ��豸���Դ����ơ�
   * @n 300~399 - ��ʾ����ģʽ������߼���Ӧ�������300��ʾ��һ�������Ϸ��豸�����301��ʾ�ڶ��������Ϸ��豸���Դ����ơ�
   * @n 400~499 - ��ʾ��������ģʽ������߼���Ӧ�����ڲ�ʹ�á�
   * @n 500~599 - ��ʾ����SPIģʽ������߼���Ӧ�����ڲ�ʹ�á�
   * @param[in] baud �����ʣ�ֻ��Դ���ģʽ��Ч��
   * @param[in] name �豸�߼����ơ�
   * @return <0��ʾʧ�ܣ�����Ϊ�豸��ʶ����
   */
  DEVHANDLE USER_API dc_init_name(short port, int baud, const char *name);

  /**
   * @brief  �ر��豸��
   * @par    ˵����
   * �ر��豸��ͨѶ���ͷ���Դ��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_exit(DEVHANDLE icdev);

  /**
   * @brief  ��ȡ���ơ�
   * @par    ˵����
   * �˽ӿڽ���ȡһ������ʶ��ǰ���豸���������Ӧ���߼����ƣ���ȡ���߼����ƽ����ڲ�ʹ�á�
   * @param[in] icdev �豸��ʶ����
   * @param[out] name ���ص������ַ����������ٷ���64���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_getname(DEVHANDLE icdev, char *name);

  /**
   * @brief  ��ȡUSB ID��Ϣ��
   * @par    ˵����
   * �˽ӿڽ���ȡ��ǰ���豸��ʹ��USB�˿ڵ�VID��PID������򿪵��豸����USB�豸��������ʧ�ܡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] vid USB�˿ڵ�VID��
   * @param[out] pid USB�˿ڵ�PID��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_getusbid(DEVHANDLE icdev, unsigned short *vid, unsigned short *pid);

  /**
   * @brief  ������
   */
  short USER_API dc_config(DEVHANDLE icdev, unsigned char _Mode, unsigned char _Baud);

  /**
   * @brief  Ѱ������
   * @par    ˵����
   * ֧��ISO 14443 Type A���Ϳ�Ƭ��Ѱ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��0x00��ʾ�Կ��п����в�����0x01��ʾ�����п�������
   * @param[out] TagType ���ص�ATQAֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_request(DEVHANDLE icdev, unsigned char _Mode, unsigned short *TagType);

  /**
   * @brief  ������ͻ��
   * @par    ˵����
   * ֧��ISO 14443 Type A���Ϳ�Ƭ�ķ�����ͻ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Bcnt �������̶�Ϊ0x00��
   * @param[out] _Snr ���صĿ����кš�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_anticoll(DEVHANDLE icdev, unsigned char _Bcnt, unsigned int *_Snr);

  /**
   * @brief  ѡ��������
   * @par    ˵����
   * ͨ��ָ�����кţ�ѡȡ��Ӧ�Ŀ�Ƭ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Snr ��Ƭ���кš�
   * @param[out] _Size ���ص�SAKֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_select(DEVHANDLE icdev, unsigned int _Snr, unsigned char *_Size);

  /**
   * @brief  ��֤M1�����롣
   * @par    ˵����
   * ʹ���豸�ڲ�װ�ص���������֤M1�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��
   * @n 0x00 - ��ʾ���豸�ڲ�װ�صĵ�0��A��������֤��ǰѡȡ��Ƭ��A���롣
   * @n 0x01 - ��ʾ���豸�ڲ�װ�صĵ�1��A��������֤��ǰѡȡ��Ƭ��A���롣
   * @n 0x02 - ��ʾ���豸�ڲ�װ�صĵ�2��A��������֤��ǰѡȡ��Ƭ��A���롣
   * @n 0x04 - ��ʾ���豸�ڲ�װ�صĵ�0��B��������֤��ǰѡȡ��Ƭ��B���롣
   * @n 0x05 - ��ʾ���豸�ڲ�װ�صĵ�1��B��������֤��ǰѡȡ��Ƭ��B���롣
   * @n 0x06 - ��ʾ���豸�ڲ�װ�صĵ�2��B��������֤��ǰѡȡ��Ƭ��B���롣
   * @param[in] _SecNr Ҫ��֤����������š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_authentication(DEVHANDLE icdev, unsigned char _Mode, unsigned char _SecNr);

  /**
   * @brief  ��ֹ��������
   * @par    ˵����
   * ʹ��Ƭ������ֹ״̬����ʱ����ѿ��Ƴ���Ӧ�����ٴη����Ӧ������Ѱ�����ſ���
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_halt(DEVHANDLE icdev);

  /**
   * @brief  �������ݡ�
   * @par    ˵����
   * ��ȡ�������ݣ�����M1����һ�ζ�ȡһ��������ݣ�Ϊ16���ֽڣ�����ML����һ�ζ�ȡ��ͬ���Ե���ҳ��Ϊ8���ֽڡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr ��ַ��
   * @n M1�� - S50���ַ��0~63����S70���ַ��0~255����
   * @n ML�� - ҳ��ַ��0~11����
   * @param[out] _Data �̶�����16���ֽ����ݣ���ʵ���ݿ���С��16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read(DEVHANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  �������ݡ�
   * @par    ˵����
   * ::dc_read ��HEX��ʽ�ӿڣ����� @a _Data ΪHEX��ʽ��
   */
  short USER_API dc_read_hex(DEVHANDLE icdev, unsigned char _Adr, char *_Data);

  /**
   * @brief  д�����ݡ�
   * @par    ˵����
   * д�����ݵ���Ƭ�ڣ�����M1����һ�α���д��һ��������ݣ�Ϊ16���ֽڣ�����ML����һ�α���д��һ��ҳ�����ݣ�Ϊ4���ֽڡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr ��ַ��
   * @n M1�� - S50���ַ��1~63����S70���ַ��1~255����
   * @n ML�� - ҳ��ַ��2~11����
   * @param[in] _Data �̶�����16���ֽ����ݣ���ʵ���ݿ���С��16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write(DEVHANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  д�����ݡ�
   * @par    ˵����
   * ::dc_write ��HEX��ʽ�ӿڣ����� @a _Data ΪHEX��ʽ��
   */
  short USER_API dc_write_hex(DEVHANDLE icdev, unsigned char _Adr, char *_Data);

  /**
   * @brief  ������
   */
  short USER_API dc_write_TS(DEVHANDLE icdev);

  /**
   * @brief  װ���豸���롣
   * @par    ˵����
   * װ��M1�����뵽�豸�ڲ���װ�غ�������ض�ʱ���� ::dc_authentication ����֤M1�����롣ע�⣺���ڶ��װ���豸������ܻ���Ӳ���洢�������ƣ��˽ӿ�ֻ����������Թ̶���װ�ش����ٵĳ��ϡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��ͬ ::dc_authentication �� @a _Mode ��
   * @param[in] _SecNr ��Ӧװ�ص������š�
   * @param[in] _NKey д���豸�еĿ����룬�̶�Ϊ6���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_load_key(DEVHANDLE icdev, unsigned char _Mode, unsigned char _SecNr, unsigned char *_NKey);

  /**
   * @brief  װ���豸���롣
   * @par    ˵����
   * ::dc_load_key ��HEX��ʽ�ӿڣ����� @a _NKey ΪHEX��ʽ��
   */
  short USER_API dc_load_key_hex(DEVHANDLE icdev, unsigned char _Mode, unsigned char _SecNr, char *_NKey);

  /**
   * @brief  Ѱ�����󡢷�����ͻ��ѡ��������
   * @par    ˵����
   * �ڲ������� ::dc_request ::dc_anticoll ::dc_select �Ĺ��ܡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��ͬ ::dc_request �� @a _Mode ��
   * @param[out] _Snr ���صĿ����кš�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_card(DEVHANDLE icdev, unsigned char _Mode, unsigned int *_Snr);

  /**
   * @brief  Ѱ�����󡢷�����ͻ��ѡ��������
   * @par    ˵����
   * ::dc_card ��HEX��ʽ�ӿڣ����� @a snrstr ΪHEX��ʽ��
   */
  short USER_API dc_card_hex(DEVHANDLE icdev, unsigned char _Mode, unsigned char *snrstr);

  /**
   * @brief  M1���������ÿ������
   * @par    ˵����
   * �޸�M1���������ÿ����ݣ�M1���������ÿ�Ҳ����ÿ�����������һ�飬��������A�������ֽڡ�����B���ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _SecNr �����š�
   * @param[in] _KeyA ����A���̶�Ϊ6���ֽڡ�
   * @param[in] _B0 ��0�����֣���һ������16��ʱ����ӦΪ��0~4�Ŀ����֣�����3λ��D2D1D0����ӦC10��C20��C30��
   * @param[in] _B1 ��1�����֣���һ������16��ʱ����ӦΪ��5~9�Ŀ����֣�����3λ��D2D1D0����ӦC11��C21��C31��
   * @param[in] _B2 ��2�����֣���һ������16��ʱ����ӦΪ��10~14�Ŀ����֣�����3λ��D2D1D0����ӦC12��C22��C32��
   * @param[in] _B3 ��3�����֣���һ������16��ʱ����ӦΪ��15�Ŀ����֣�����3λ��D2D1D0����ӦC13��C23��C33��
   * @param[in] _Bk �������̶�Ϊ0x00��
   * @param[in] _KeyB ����B���̶�Ϊ6���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_changeb3(DEVHANDLE icdev, unsigned char _SecNr, unsigned char *_KeyA, unsigned char _B0, unsigned char _B1, unsigned char _B2, unsigned char _B3, unsigned char _Bk, unsigned char *_KeyB);

  /**
   * @brief  M1���������ÿ������
   * @par    ˵����
   * ::dc_changeb3 ��HEX��ʽ�ӿڣ����� @a _KeyA @a _KeyB ΪHEX��ʽ��
   */
  short USER_API dc_changeb3_hex(DEVHANDLE icdev, unsigned char _SecNr, const char *_KeyA, unsigned char _B0, unsigned char _B1, unsigned char _B2, unsigned char _B3, unsigned char _Bk, const char *_KeyB);

  /**
   * @brief  M1��ֵ��ش���
   * @par    ˵����
   * ���ڰ�ָ����ֵ�������ݴ棬��������ʹ�� ::dc_transfer ���ݴ�ֵ�����ݴ��ݵ���һ���У�ʵ�ֿ����֮����ֵ���͡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr Ҫ�ش��Ŀ��ַ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_restore(DEVHANDLE icdev, unsigned char _Adr);

  /**
   * @brief  M1��ֵ�鴫�ݡ�
   * @par    ˵����
   * ���ڰ� ::dc_restore �ݴ��ֵ�����ݴ��ݵ�ָ�����У�ʵ�ֿ����֮����ֵ���͡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr Ҫ���ݵĿ��ַ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_transfer(DEVHANDLE icdev, unsigned char _Adr);

  /**
   * @brief  M1��ֵ���ֵ��
   * @par    ˵����
   * ���ڲ���M1��ֵ�飬ʹ��ֵ����ָ����ֵ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr ���ַ��
   * @param[in] _Value Ҫ���ӵ���ֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_increment(DEVHANDLE icdev, unsigned char _Adr, unsigned int _Value);

  /**
   * @brief  M1��ֵ���ֵ��
   * @par    ˵����
   * ���ڲ���M1��ֵ�飬ʹ��ֵ����ָ����ֵ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr ���ַ��
   * @param[in] _Value Ҫ���ٵ���ֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_decrement(DEVHANDLE icdev, unsigned char _Adr, unsigned int _Value);

  /**
   * @brief  M1��ֵ���ʼ����
   * @par    ˵����
   * ���ڲ���M1����ʹ��ָ����Ϊֵ�飬���ҳ�ʼ����ֵ��Ϊָ����ֵ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr ���ַ��
   * @param[in] _Value ��ʼ����ֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_initval(DEVHANDLE icdev, unsigned char _Adr, unsigned int _Value);

  /**
   * @brief  M1��ֵ���ֵ��
   * @par    ˵����
   * ���ڶ�ȡM1��ֵ����ֵ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr Ҫ��ȡ�Ŀ��ַ��
   * @param[out] _Value ���ص���ֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readval(DEVHANDLE icdev, unsigned char _Adr, unsigned int *_Value);

  /**
   * @brief  ML��ֵ��ʼ����
   * @par    ˵����
   * ���ڲ���ML������ʼ��������Ϊָ����ֵ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Value ��ʼ����ֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_initval_ml(DEVHANDLE icdev, unsigned short _Value);

  /**
   * @brief  ML����ֵ��
   * @par    ˵����
   * ���ڶ�ȡML����ֵ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] _Value ���ص���ֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readval_ml(DEVHANDLE icdev, unsigned short *_Value);

  /**
   * @brief  ML����ֵ��
   * @par    ˵����
   * ���ڲ���ML��ֵ��ʹֵ����ָ����ֵ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Value Ҫ���ٵ���ֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_decrement_ml(DEVHANDLE icdev, unsigned short _Value);

  /**
   * @brief  ��֤M1�����롣
   * @par    ˵����
   * ʹ���豸�ڲ�װ�ص���������֤M1�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��ͬ ::dc_authentication �� @a _Mode ��
   * @param[in] KeyNr Ҫ��֤����������š�
   * @param[in] Adr Ҫ��֤���������еĿ�ţ��˿�ŷǿ�Ƭ���Կ�š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_authentication_2(DEVHANDLE icdev, unsigned char _Mode, unsigned char KeyNr, unsigned char Adr);

  /**
   * @brief  �ڶ���������ͻ��
   * @par    ˵����
   * ֧��ISO 14443 Type A���Ϳ�Ƭ�ĵڶ���������ͻ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Bcnt �������̶�Ϊ0x00��
   * @param[out] _Snr ���صĿ����кš�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_anticoll2(DEVHANDLE icdev, unsigned char _Bcnt, unsigned int *_Snr);

  /**
   * @brief  �ڶ���ѡ��������
   * @par    ˵����
   * ͨ��ָ�����кţ�ѡȡ��Ӧ�Ŀ�Ƭ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Snr ��Ƭ���кš�
   * @param[out] _Size ���ص�SAKֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_select2(DEVHANDLE icdev, unsigned int _Snr, unsigned char *_Size);

  /**
   * @brief  ������������ͻ��
   * @par    ˵����
   * ֧��ISO 14443 Type A���Ϳ�Ƭ�ĵ�����������ͻ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Bcnt �������̶�Ϊ0x00��
   * @param[out] _Snr ���صĿ����кš�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_anticoll3(DEVHANDLE icdev, unsigned char _Bcnt, unsigned int *_Snr);

  /**
   * @brief  ������ѡ��������
   * @par    ˵����
   * ͨ��ָ�����кţ�ѡȡ��Ӧ�Ŀ�Ƭ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Snr ��Ƭ���кš�
   * @param[out] _Size ���ص�SAKֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_select3(DEVHANDLE icdev, unsigned int _Snr, unsigned char *_Size);

  /**
   * @brief  ������
   */
  short USER_API dc_HL_write(DEVHANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int *_Snr, unsigned char *_Data);

  /**
   * @brief  ������
   */
  short USER_API dc_HL_writehex(DEVHANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int *_Snr, unsigned char *_Data);

  /**
   * @brief  ������
   */
  short USER_API dc_HL_read(DEVHANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int _Snr, unsigned char *_Data, unsigned int *_NSnr);

  /**
   * @brief  ������
   */
  short USER_API dc_HL_readhex(DEVHANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int _Snr, unsigned char *_Data, unsigned int *_NSnr);

  /**
   * @brief  �߲���֤M1�����롣
   * @par    ˵����
   * �ڲ������� ::dc_card ::dc_authentication �Ĺ��ܡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] reqmode Ѱ������ģʽ��ͬ ::dc_request �� @a _Mode ��
   * @param[in] snr �������̶�Ϊ0��
   * @param[in] authmode ��֤ģʽ��ͬ ::dc_authentication �� @a _Mode ��
   * @param[in] secnr Ҫ��֤����������š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_HL_authentication(DEVHANDLE icdev, unsigned char reqmode, unsigned int snr, unsigned char authmode, unsigned char secnr);

  /**
   * @brief  ���֮ǰд������ݡ�
   * @par    ˵����
   * �ڲ������� ::dc_card ::dc_authentication ::dc_read �Ĺ��ܣ�������֤����Ͷ�ȡ�������Ƿ�һ�¡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] Snr �����кţ������ڲ��˶ԡ�
   * @param[in] authmode ��֤ģʽ��ͬ ::dc_authentication �� @a _Mode ��
   * @param[in] Adr ���ַ��
   * @param[in] _data �����ݣ��̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_check_write(DEVHANDLE icdev, unsigned int Snr, unsigned char authmode, unsigned char Adr, unsigned char *_data);

  /**
   * @brief  ���֮ǰд������ݡ�
   * @par    ˵����
   * ::dc_check_write ��HEX��ʽ�ӿڣ����� @a _data ΪHEX��ʽ��
   */
  short USER_API dc_check_writehex(DEVHANDLE icdev, unsigned int Snr, unsigned char authmode, unsigned char Adr, unsigned char *_data);

  /**
   * @brief  ��ȡ�豸�汾��
   * @par    ˵����
   * ��ȡ�豸�ڲ��̼�����İ汾��
   * @param[in] icdev �豸��ʶ����
   * @param[out] sver ���صİ汾�ַ����������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_getver(DEVHANDLE icdev, unsigned char *sver);

  /**
   * @brief  ������
   */
  short USER_API dc_update(DEVHANDLE icdev);

  /**
   * @brief  ������
   */
  short USER_API dc_clr_control_bit(DEVHANDLE icdev, unsigned char _b);

  /**
   * @brief  ������
   */
  short USER_API dc_set_control_bit(DEVHANDLE icdev, unsigned char _b);

  /**
   * @brief  ��λ��Ƶ��
   * @par    ˵����
   * ��λ�豸����Ƶ�����Թرգ��رղ���������
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Msec Ϊ0��ʾ�ر���Ƶ������Ϊ��λʱ�䣬��λΪ10���룬һ����ý���ֵΪ10��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_reset(DEVHANDLE icdev, unsigned short _Msec);

  /**
   * @brief  �豸������
   * @par    ˵����
   * �豸����������ָ��ʱ��ķ�������
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Msec ����ʱ�䣬��λΪ10���롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_beep(DEVHANDLE icdev, unsigned short _Msec);

  /**
   * @brief  Һ����ʾ��
   * @par    ˵����
   * ��ʾָ���ַ�����Һ����Ļ�ϡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] dispstr Ҫ��ʾ���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_disp_str(DEVHANDLE icdev, char *dispstr);

  /**
   * @brief  ��EEPROM��
   * @par    ˵����
   * ��ȡ�豸�ڲ�EEPROM�е����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�Ƶ�ַ��
   * @param[in] length ��ȡ���ȡ�
   * @param[out] rec_buffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_srd_eeprom(DEVHANDLE icdev, short offset, short length, unsigned char *rec_buffer);

  /**
   * @brief  дEEPROM��
   * @par    ˵����
   * д�����ݵ��豸�ڲ�EEPROM�У������������ݱ���ȡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�Ƶ�ַ��
   * @param[in] length д�볤�ȡ�
   * @param[in] send_buffer �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_swr_eeprom(DEVHANDLE icdev, short offset, short length, unsigned char *send_buffer);

  /**
   * @brief  дEEPROM��
   * @par    ˵����
   * ͬ ::dc_swr_eeprom ��
   */
  short USER_API swr_alleeprom(DEVHANDLE icdev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  ��EEPROM��
   * @par    ˵����
   * ͬ ::dc_srd_eeprom ��
   */
  short USER_API srd_alleeprom(DEVHANDLE icdev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  ��EEPROM��
   * @par    ˵����
   * ::dc_srd_eeprom ��HEX��ʽ�ӿڣ����� @a rec_buffer ΪHEX��ʽ��
   */
  short USER_API dc_srd_eepromhex(DEVHANDLE icdev, short offset, short length, unsigned char *rec_buffer);

  /**
   * @brief  дEEPROM��
   * @par    ˵����
   * ::dc_swr_eeprom ��HEX��ʽ�ӿڣ����� @a send_buffer ΪHEX��ʽ��
   */
  short USER_API dc_swr_eepromhex(DEVHANDLE icdev, short offset, short length, unsigned char *send_buffer);

  /**
   * @brief  ��ȡ�豸ʱ�䡣
   * @par    ˵����
   * ��ȡ�豸��ǰ��ʱ�䡣
   * @param[in] icdev �豸��ʶ����
   * @param[out] time ���ص�ʱ�䣬����Ϊ7���ֽڣ���ʽΪ'��'��'����'��'��'��'��'��'ʱ'��'��'��'��'���磺0x16,0x01,0x12,0x08,0x16,0x20,0x10��ʾ2016������һ12��8��16ʱ20��10�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_gettime(DEVHANDLE icdev, unsigned char *time);

  /**
   * @brief  ��ȡ�豸ʱ�䡣
   * @par    ˵����
   * ::dc_gettime ��HEX��ʽ�ӿڣ����� @a time ΪHEX��ʽ��
   */
  short USER_API dc_gettimehex(DEVHANDLE icdev, char *time);

  /**
   * @brief  �����豸ʱ�䡣
   * @par    ˵����
   * �����豸�ĵ�ǰʱ�䡣
   * @param[in] icdev �豸��ʶ����
   * @param[in] time ����ʱ�䣬����Ϊ7���ֽڣ���ʽΪ'��'��'����'��'��'��'��'��'ʱ'��'��'��'��'���磺0x16,0x01,0x12,0x08,0x16,0x20,0x10��ʾ2016������һ12��8��16ʱ20��10�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_settime(DEVHANDLE icdev, unsigned char *time);

  /**
   * @brief  �����豸ʱ�䡣
   * @par    ˵����
   * ::dc_settime ��HEX��ʽ�ӿڣ����� @a time ΪHEX��ʽ��
   */
  short USER_API dc_settimehex(DEVHANDLE icdev, char *time);

  /**
   * @brief  ������
   */
  short USER_API dc_setbright(DEVHANDLE icdev, unsigned char bright);

  /**
   * @brief  ������
   */
  short USER_API dc_ctl_mode(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ������
   */
  short USER_API dc_disp_mode(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ��DES�ӽ��ܡ�
   * @par    ˵����
   * ::dcdes ��HEX��ʽ�ӿڣ����� @a key @a sour @a dest ΪHEX��ʽ��
   */
  short USER_API dcdeshex(unsigned char *key, unsigned char *sour, unsigned char *dest, short m);

  /**
   * @brief  ��DES�ӽ��ܡ�
   * @par    ˵����
   * ʹ�õ�DES�㷨�����ݽ��м���/���ܡ�
   * @param[in] key 8���ֽ���Կ��
   * @param[in] sour 8���ֽ�Ҫ������/���ܵ����ݡ�
   * @param[out] dest ����8���ֽ����������ݡ�
   * @param[in] m ģʽ��0-���ܣ�1-���ܡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dcdes(unsigned char *key, unsigned char *sour, unsigned char *dest, short m);

  /**
   * @brief  ָʾ�Ƶ���/Ϩ��
   * @par    ˵����
   * ����/Ϩ���豸��ָʾ�ơ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _OnOff 0-������1-Ϩ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_light(DEVHANDLE icdev, unsigned short _OnOff);

  /**
   * @brief  Һ����ʾ��
   * @par    ˵����
   * ��ʾָ�����ݵ�Һ����Ļ�ϡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ��Ļƫ�ơ�
   * @param[in] displen ��ʾ���ݵĳ��ȡ�
   * @param[in] dispstr ��ʾ���ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_high_disp(DEVHANDLE icdev, unsigned char offset, unsigned char displen, unsigned char *dispstr);

  /**
   * @brief  ���õ�ǰ�Ӵ�ʽ������
   * @par    ˵����
   * ���õ�ǰ�Ӵ�ʽ����Ϊָ�����������ڶ࿨���л���������
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Byte ������š�
   * @n 0x0C - ������/�Ӵ�ʽCPU1������
   * @n 0x0B - �Ӵ�ʽCPU2������
   * @n 0x0D - SAM1������
   * @n 0x0E - SAM2������
   * @n 0x0F - SAM3������
   * @n 0x11 - SAM4������
   * @n 0x12 - SAM5������
   * @n 0x13 - SAM6����/ESAMоƬ��
   * @n 0x14 - SAM7������
   * @n 0x15 - SAM8������
   * @n 0x16~0xFF - ����������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_setcpu(DEVHANDLE icdev, unsigned char _Byte);

  /**
   * @brief  �Ӵ�ʽCPU����λ��
   * @par    ˵����
   * �Ե�ǰ����CPU�����и�λ�������˸�λΪ�临λ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen ���ظ�λ��Ϣ�ĳ��ȡ�
   * @param[out] databuffer ���صĸ�λ��Ϣ�������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_cpureset(DEVHANDLE icdev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ե�ǰ����CPU������ָ���������ע��˽ӿڲ���װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_cpuapdusource(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ե�ǰ����CPU������ָ���������ע��˽ӿ��ѷ�װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_cpuapdu(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU����λ��
   * @par    ˵����
   * ::dc_cpureset ��HEX��ʽ�ӿڣ����� @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_cpureset_hex(DEVHANDLE icdev, unsigned char *rlen, char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_cpuapdusource ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_cpuapdusource_hex(DEVHANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_cpuapdu ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_cpuapdu_hex(DEVHANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ե�ǰ����CPU������ָ���������ע��˽ӿ��ѷ�װ��Э�鲿�֣��ڲ�������SW1Ϊ0x61��0x6C�������
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_cpuapdurespon(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_cpuapdurespon ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_cpuapdurespon_hex(DEVHANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU���µ硣
   * @par    ˵����
   * �Ե�ǰ����CPU�������µ������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_cpudown(DEVHANDLE icdev);

  /**
   * @brief  ����485��ַ�š�
   * @par    ˵����
   * ����485��ַ�ţ������ý�ʹ�ú����ӿڵ���ʹ���µ�485��ַ�š�
   * @param[in] saddr ��ַ�š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_set_addr(unsigned char saddr);

  /**
   * @brief  ��485�豸��
   * @par    ˵����
   * �����豸��ͨѶ���ҷ�����Ӧ����Դ���󲿷ֹ��ܽӿڶ���Ҫ�ڴ˹��̺���ܽ��У��ڲ���Ҫʹ���豸�󣬱���ʹ�� ::dc_exit ȥ�ر��豸��ͨѶ���ͷ���Դ��
   * @param[in] port �˿ںš�
   * @n 0~99 - ��ʾ����ģʽ�����������Ӧ�������0��ʾ��һ�����ںϷ��豸�����1��ʾ�ڶ������ںϷ��豸���Դ����ơ�
   * @param[in] baud �����ʡ�
   * @return <0��ʾʧ�ܣ�����Ϊ�豸��ʶ����
   */
  DEVHANDLE USER_API dc_init_485(short port, int baud);

  /**
   * @brief  ���ô���485�豸ͨѶ�����ʡ�
   * @par    ˵����
   * ���ô���485�豸ͨѶ�����ʡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] baud �����ʡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_changebaud_485(DEVHANDLE icdev, int baud);

  /**
   * @brief  �޸��豸485��ַ�š�
   * @par    ˵����
   * �޸��豸485��ַ�ţ��޸ĺ��豸�ڲ���ʹ���µĵ�ַ�š�
   * @param[in] icdev �豸��ʶ����
   * @param[in] saddr ��ַ�š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_change_addr(DEVHANDLE icdev, unsigned char saddr);

  /**
   * @brief  �ǽӴ�ʽCPU����λ��
   * @par    ˵����
   * �Ը�Ӧ��CPU�����и�λ������
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen ���ظ�λ��Ϣ�ĳ��ȡ�
   * @param[out] receive_data ���صĸ�λ��Ϣ�������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_pro_reset(DEVHANDLE icdev, unsigned char *rlen, unsigned char *receive_data);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ը�Ӧ��CPU������ָ���������ע��˽ӿ��ѷ�װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @param[in] timeout ��ʱֵ����ֵֻ�ڲ����豸�ĵײ�ʹ�ã���λΪ250���룬һ����ý���ֵΪ7��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_pro_command(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  �ǽӴ�ʽCPU����λ��
   * @par    ˵����
   * ::dc_pro_reset ��HEX��ʽ�ӿڣ����� @a receive_data ΪHEX��ʽ��
   */
  short USER_API dc_pro_resethex(DEVHANDLE icdev, unsigned char *rlen, char *receive_data);

  /**
   * @brief  �ǽӴ�ʽCPU����λ��
   * @par    ˵����
   * �Ը�Ӧ��CPU�����и�λ������
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen ���ظ�λ��Ϣ�ĳ��ȡ�
   * @param[out] receive_data ���صĸ�λ��Ϣ�������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_pro_resetInt(DEVHANDLE icdev, unsigned char *rlen, unsigned char *receive_data);

  /**
   * @brief  �ǽӴ�ʽCPU����λ��
   * @par    ˵����
   * ::dc_pro_resetInt ��HEX��ʽ�ӿڣ����� @a receive_data ΪHEX��ʽ��
   */
  short USER_API dc_pro_resetInt_hex(DEVHANDLE icdev, unsigned char *rlen, char *receive_data);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_pro_command ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_pro_commandhex(DEVHANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ը�Ӧ��CPU������ָ���������ע��˽ӿڲ���װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @param[in] timeout ��ʱֵ����ֵֻ�ڲ����豸�ĵײ�ʹ�ã���λΪ250���룬һ����ý���ֵΪ7��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_pro_commandsource(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_pro_commandsource ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_pro_commandsourcehex(DEVHANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ը�Ӧ��CPU������ָ���������ע��˽ӿڲ���װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @param[in] timeout ��ʱֵ����ֵֻ�ڲ����豸�ĵײ�ʹ�ã���λΪ250���룬һ����ý���ֵΪ7��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_pro_commandsource_int(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  ��ֹ�ǽӴ�ʽCPU��������
   * @par    ˵����
   * ʹ�ǽӴ�ʽCPU��������ֹ״̬����ʱ����ѿ��Ƴ���Ӧ�����ٴη����Ӧ������Ѱ�����ſ���
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_pro_halt(DEVHANDLE icdev);

  /**
   * @brief  SHC1102��Ѱ������
   * @par    ˵����
   * ֧��SHC1102����Ѱ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��0x00��ʾ�Կ��п����в�����0x01��ʾ�����п�������
   * @param[out] TagType ���ص�ATQAֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_request_shc1102(DEVHANDLE icdev, unsigned char _Mode, unsigned short *TagType);

  /**
   * @brief  ��֤SHC1102�����롣
   * @par    ˵����
   * ʹ�ô������������֤SHC1102�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Data ���룬�̶�Ϊ4���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_auth_shc1102(DEVHANDLE icdev, unsigned char *_Data);

  /**
   * @brief  ��SHC1102����
   * @par    ˵����
   * ��ȡSHC1102�������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr ���ַ��0~15����
   * @param[out] _Data ���ص����ݣ��̶�Ϊ4���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_shc1102(DEVHANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  дSHC1102����
   * @par    ˵����
   * д�����ݵ�SHC1102���С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr ���ַ��2~15����
   * @param[in] _Data �������ݣ��̶�Ϊ4���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_shc1102(DEVHANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  ��ֹSHC1102��������
   * @par    ˵����
   * ʹSHC1102��������ֹ״̬����ʱ����ѿ��Ƴ���Ӧ�����ٴη����Ӧ������Ѱ�����ſ���
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_halt_shc1102(DEVHANDLE icdev);

  /**
   * @brief  ����ת����
   * @par    ˵����
   * ��ͨ���ݻ���ʮ�������ַ�������ת������
   * @param[in] hex Ҫת�������ݡ�
   * @param[out] a ת������ַ�����
   * @param[in] length Ҫת�����ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API hex_a(unsigned char *hex, unsigned char *a, short length);

  /**
   * @brief  ����ת����
   * @par    ˵����
   * ʮ�������ַ�����ת��Ϊ��ͨ���ݣ���ת�̣���
   * @param[in] a Ҫת�������ݡ�
   * @param[out] hex ת��������ݡ�
   * @param[in] len Ҫת�����ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API a_hex(unsigned char *a, unsigned char *hex, short len);

  /**
   * @brief  ���÷ǽӴ������͡�
   * @par    ˵����
   * ������Ҫ����ʲô���͵ķǽӴ�ʽ�����豸�ϵ��Ĭ�ϲ���Type A��������ʹ�ô˽ӿ����ı����ͣ�һ����Ѱ��ǰ���ô˽ӿڡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] cardtype ���ͣ�'A'��ʾISO 14443 Type A����'B'��ʾISO 14443 Type B����'1'��ʾISO 15693����'2'��ʾISO 18092����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_config_card(DEVHANDLE icdev, unsigned char cardtype);

  /**
   * @brief  Ѱ������
   * @par    ˵����
   * ֧��ISO 14443 Type B���Ϳ�Ƭ��Ѱ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode �������̶�Ϊ0x00��
   * @param[in] AFI Ӧ�����ʶ����
   * @param[in] N ʱ��۱�š�
   * @param[out] ATQB ���ص�ATQBֵ�������ٷ���32���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_request_b(DEVHANDLE icdev, unsigned char _Mode, unsigned char AFI, unsigned char N, unsigned char *ATQB);

  /**
   * @brief  �ܵ���־��
   * @par    ˵����
   * ֧��ISO 14443 Type B���Ϳ�Ƭ�Ĺܵ���־��
   * @param[in] icdev �豸��ʶ����
   * @param[in] N ʱ��۱�š�
   * @param[out] ATQB ���ص�ATQBֵ�������ٷ���32���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_slotmarker(DEVHANDLE icdev, unsigned char N, unsigned char *ATQB);

  /**
   * @brief  ���Ƭ��
   * @par    ˵����
   * ֧��ISO 14443 Type B���Ϳ�Ƭ�ļ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] PUPI αΨһPICC��ʶ�����̶�Ϊ4���ֽڡ�
   * @param[in] CID �ŵ��š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_attrib(DEVHANDLE icdev, unsigned char *PUPI, unsigned char CID);

  /**
   * @brief  ������
   */
  short USER_API dc_open_door(DEVHANDLE icdev, unsigned char cflag);

  /**
   * @brief  ������
   */
  short USER_API dc_open_timedoor(DEVHANDLE icdev, unsigned short utime);

  /**
   * @brief  ������
   */
  short USER_API dc_read_random(DEVHANDLE icdev, unsigned char *data);

  /**
   * @brief  ������
   */
  short USER_API dc_write_random(DEVHANDLE icdev, short length, unsigned char *data);

  /**
   * @brief  ������
   */
  short USER_API dc_read_random_hex(DEVHANDLE icdev, unsigned char *data);

  /**
   * @brief  ������
   */
  short USER_API dc_write_random_hex(DEVHANDLE icdev, short length, unsigned char *data);

  /**
   * @brief  ������
   */
  short USER_API dc_erase_random(DEVHANDLE icdev, short length);

  /**
   * @brief  Mifare Desfire����֤��
   * @par    ˵����
   * ��Mifare Desfire��������֤��
   * @param[in] icdev �豸��ʶ����
   * @param[in] keyno ��Կ�š�
   * @param[in] keylen ��Կ���ݵĳ��ȡ�
   * @param[in] authkey ��Կ���ݡ�
   * @param[in] randAdata ���������A��8���ֽڡ�
   * @param[out] randBdata ���ص������B��8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_mfdes_auth(DEVHANDLE icdev, unsigned char keyno, unsigned char keylen, unsigned char *authkey, unsigned char *randAdata, unsigned char *randBdata);

  /**
   * @brief  ��֤M1�����롣
   * @par    ˵����
   * ʹ�ô������������֤M1�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��0x00��ʾ��֤A���룬0x04��ʾ��֤B���롣
   * @param[in] _Addr Ҫ��֤����������š�
   * @param[in] passbuff ���룬�̶�Ϊ6���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_authentication_pass(DEVHANDLE icdev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  �߲�Һ����ʾ��
   * @par    ˵����
   * ��ʾָ�����ݵ�Һ����Ļ�ϡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] dispstr ��ʾ�ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_disp_neg(DEVHANDLE icdev, char *dispstr);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ը�Ӧ��CPU������ָ���������ע��˽ӿ��ѷ�װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @param[in] timeout ��ʱֵ����ֵֻ�ڲ����豸�ĵײ�ʹ�ã���λΪ250���룬һ����ý���ֵΪ7��
   * @param[in] FG �ָ�ֵ����ֵֻ�ڲ����豸�ĵײ�ʹ�ã���λΪ�ֽڣ�һ����ý���ֵΪ64��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_pro_commandlink(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_pro_commandlink ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_pro_commandlink_hex(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  Ѱ�����󡢷�����ͻ��ѡ��������
   * @par    ˵����
   * �ڲ������� ::dc_request ::dc_anticoll ::dc_select ::dc_anticoll2 ::dc_select2 �Ĺ��ܣ��˽ӿ������ȱ�ݣ����ܷ��ؿ����к�ʵ�ʳ��ȣ�����ʹ�� ::dc_card_n �����
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��ͬ ::dc_request �� @a _Mode ��
   * @param[out] _Snr ���صĿ����кţ������ٷ���8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_card_double(DEVHANDLE icdev, unsigned char _Mode, unsigned char *_Snr);

  /**
   * @brief  Ѱ�����󡢷�����ͻ��ѡ��������
   * @par    ˵����
   * ::dc_card_double ��HEX��ʽ�ӿڣ����� @a _Snr ΪHEX��ʽ��
   */
  short USER_API dc_card_double_hex(DEVHANDLE icdev, unsigned char _Mode, unsigned char *_Snr);

  /**
   * @brief  ��ID����
   * @par    ˵����
   * ��ȡID�����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] times �豸��ʱֵ����λΪ�롣
   * @param[out] _Data ���ص����ݣ��̶�Ϊ5���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_idcard(DEVHANDLE icdev, unsigned char times, unsigned char *_Data);

  /**
   * @brief  ��ȡid����
   * @par    ˵����
   * ::dc_read_idcard ��HEX��ʽ�ӿڣ����� @a _Data ΪHEX��ʽ��
   */
  short USER_API dc_read_idcard_hex(DEVHANDLE icdev, unsigned char times, unsigned char *_Data);

  /**
   * @brief  ��֤M1�����롣
   * @par    ˵����
   * ::dc_authentication_pass ��HEX��ʽ�ӿڣ����� @a passbuff ΪHEX��ʽ��
   */
  short USER_API dc_authentication_pass_hex(DEVHANDLE icdev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  ���ýӴ�ʽCPU��������
   * @par    ˵����
   * �˽ӿڿɿ��ƽӴ�ʽCPU����������Ϊ����������ʱ���豸��ʹ��Ĭ�Ϸ�ʽ���нӴ�ʽCPU��������
   * @param[in] icdev �豸��ʶ����
   * @param[in] cputype ������ţ�ͬ ::dc_setcpu �� @a _Byte ��
   * @param[in] cpupro ��Э���ţ�0x00��ʾT0��0x01��ʾT1��Ĭ��Ϊ0x00��
   * @param[in] cpuetu ����λ�����ʱ�ţ�0x5C��ʾ9600��0x14��ʾ38400��0x0E��ʾ115200��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_setcpupara(DEVHANDLE icdev, unsigned char cputype, unsigned char cpupro, unsigned char cpuetu);

  /**
   * @brief  ������
   */
  short USER_API dc_command(DEVHANDLE icdev, unsigned char cmd, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_command_hex(DEVHANDLE icdev, unsigned char cmd, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_creat_mac(unsigned char KeyLen, unsigned char *Key, unsigned short DataLen, unsigned char *Data, unsigned char *InitData, unsigned char AutoFixFlag, unsigned char FixChar, unsigned char *MacData);

  /**
   * @brief  ������
   */
  short USER_API dc_creat_mac_hex(unsigned char KeyLen, unsigned char *Key, unsigned short DataLen, unsigned char *Data, unsigned char *InitData, unsigned char AutoFixFlag, unsigned char FixChar, unsigned char *MacData);

  /**
   * @brief  ������
   */
  short USER_API dc_creat_mac2(DEVHANDLE icdev, unsigned short DataLen, unsigned char *Data, unsigned char *MacData, unsigned char flag);

  /**
   * @brief  ������
   */
  short USER_API dc_creat_mac2_hex(DEVHANDLE icdev, unsigned short DataLen, unsigned char *Data, unsigned char *MacData, unsigned char flag);

  /**
   * @brief  ������
   */
  short USER_API dc_encrypt(char *key, char *ptrSource, unsigned short msgLen, char *ptrDest);

  /**
   * @brief  ������
   */
  short USER_API dc_decrypt(char *key, char *ptrSource, unsigned short msgLen, char *ptrDest);

  /**
   * @brief  ������
   */
  short USER_API dc_encrypt_hex(char *key, char *ptrSource, unsigned short msgLen, char *ptrDest);

  /**
   * @brief  ������
   */
  short USER_API dc_decrypt_hex(char *key, char *ptrSource, unsigned short msgLen, char *ptrDest);

  /**
   * @brief  ������
   */
  short USER_API dc_HL_write_hex(DEVHANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int *_Snr, unsigned char *_Data);

  /**
   * @brief  ������
   */
  short USER_API dc_HL_read_hex(DEVHANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int _Snr, unsigned char *_Data, unsigned int *_NSnr);

  /**
   * @brief  ���֮ǰд������ݡ�
   * @par    ˵����
   * ::dc_check_write ��HEX��ʽ�ӿڣ����� @a _data ΪHEX��ʽ��
   */
  short USER_API dc_check_write_hex(DEVHANDLE icdev, unsigned int Snr, unsigned char authmode, unsigned char Adr, unsigned char *_data);

  /**
   * @brief  ��EEPROM��
   * @par    ˵����
   * ::dc_srd_eeprom ��HEX��ʽ�ӿڣ����� @a rec_buffer ΪHEX��ʽ��
   */
  short USER_API dc_srd_eeprom_hex(DEVHANDLE icdev, short offset, short length, unsigned char *rec_buffer);

  /**
   * @brief  дEEPROM��
   * @par    ˵����
   * ::dc_swr_eeprom ��HEX��ʽ�ӿڣ����� @a send_buffer ΪHEX��ʽ��
   */
  short USER_API dc_swr_eeprom_hex(DEVHANDLE icdev, short offset, short length, unsigned char *send_buffer);

  /**
   * @brief  ��ȡ�豸ʱ�䡣
   * @par    ˵����
   * ::dc_gettime ��HEX��ʽ�ӿڣ����� @a time ΪHEX��ʽ��
   */
  short USER_API dc_gettime_hex(DEVHANDLE icdev, char *time);

  /**
   * @brief  �����豸ʱ�䡣
   * @par    ˵����
   * ::dc_settime ��HEX��ʽ�ӿڣ����� @a time ΪHEX��ʽ��
   */
  short USER_API dc_settime_hex(DEVHANDLE icdev, char *time);

  /**
   * @brief  ��DES�ӽ��ܡ�
   * @par    ˵����
   * ::dc_des ��HEX��ʽ�ӿڣ����� @a key @a sour @a dest ΪHEX��ʽ��
   */
  short USER_API dc_des_hex(unsigned char *key, unsigned char *sour, unsigned char *dest, short m);

  /**
   * @brief  ��DES�ӽ��ܡ�
   * @par    ˵����
   * ʹ�õ�DES�㷨�����ݽ��м���/���ܡ�
   * @param[in] key 8���ֽ���Կ��
   * @param[in] sour 8���ֽ�Ҫ������/���ܵ����ݡ�
   * @param[out] dest ����8���ֽ����������ݡ�
   * @param[in] m ģʽ��0-���ܣ�1-���ܡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_des(unsigned char *key, unsigned char *sour, unsigned char *dest, short m);

  /**
   * @brief  �ǽӴ�ʽCPU����λ��
   * @par    ˵����
   * ::dc_pro_reset ��HEX��ʽ�ӿڣ����� @a receive_data ΪHEX��ʽ��
   */
  short USER_API dc_pro_reset_hex(DEVHANDLE icdev, unsigned char *rlen, char *receive_data);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_pro_command ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_pro_command_hex(DEVHANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_pro_commandsource ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_pro_commandsource_hex(DEVHANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  ������
   */
  short USER_API dc_switch_unix(DEVHANDLE icdev, int baud);

  /**
   * @brief  ��֤M1�����롣
   * @par    ˵����
   * ʹ�ô������������֤M1�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��0x00��ʾ��֤A���룬0x04��ʾ��֤B���롣
   * @param[in] _Addr Ҫ��֤����Ŀ�š�
   * @param[in] passbuff ���룬�̶�Ϊ6���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_authentication_passaddr(DEVHANDLE icdev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  ��֤M1�����롣
   * @par    ˵����
   * ::dc_authentication_passaddr ��HEX��ʽ�ӿڣ����� @a passbuff ΪHEX��ʽ��
   */
  short USER_API dc_authentication_passaddr_hex(DEVHANDLE icdev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  ѰFM11RF005����
   * @par    ˵����
   * ��FM11RF005������Ѱ��������
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��ģʽ��ͬ ::dc_request �� @a _Mode ��
   * @param[out] _Snr ���صĿ����кš�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_card_fm11rf005(DEVHANDLE icdev, unsigned char _Mode, unsigned int *_Snr);

  /**
   * @brief  ������
   */
  short USER_API dc_setusbtimeout(unsigned char ntimes);

  /**
   * @brief  ������
   */
  short USER_API dc_mfdes_baud(DEVHANDLE icdev, unsigned char _Mode, unsigned char para);

  /**
   * @brief  ��DES�ӽ��ܡ�
   * @par    ˵����
   * ʹ����DES�㷨�����ݽ��м���/���ܡ�
   * @param[in] key 16���ֽ���Կ��
   * @param[in] src 8���ֽ�Ҫ������/���ܵ����ݡ�
   * @param[out] dest ����8���ֽ����������ݡ�
   * @param[in] m ģʽ��0-���ܣ�1-���ܡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_tripledes(unsigned char *key, unsigned char *src, unsigned char *dest, short m);

  /**
   * @brief  ��DES�ӽ��ܡ�
   * @par    ˵����
   * ::dc_tripledes ��HEX��ʽ�ӿڣ����� @a key @a src @a dest ΪHEX��ʽ��
   */
  short USER_API dc_tripledes_hex(unsigned char *key, unsigned char *src, unsigned char *dest, short m);

  /**
   * @brief  Mifare Desfire����֤��
   * @par    ˵����
   * ::dc_mfdes_auth ��HEX��ʽ�ӿڣ����� @a authkey @a randAdata @a randBdata ΪHEX��ʽ��
   */
  short USER_API dc_mfdes_auth_hex(DEVHANDLE icdev, unsigned char keyno, unsigned char keylen, unsigned char *authkey, unsigned char *randAdata, unsigned char *randBdata);

  /**
   * @brief  ������
   */
  short USER_API dc_pro_sendcommandsource(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char timeout);

  /**
   * @brief  ������
   */
  short USER_API dc_pro_receivecommandsource(DEVHANDLE icdev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_inventory(DEVHANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char masklen, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_inventory_hex(DEVHANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char masklen, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_stay_quiet(DEVHANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_stay_quiet_hex(DEVHANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_select_uid(DEVHANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_select_uid_hex(DEVHANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_reset_to_ready(DEVHANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_reset_to_ready_hex(DEVHANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_readblock(DEVHANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_readblock_hex(DEVHANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_writeblock(DEVHANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char wlen, unsigned char *wbuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_writeblock_hex(DEVHANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char wlen, unsigned char *wbuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_lock_block(DEVHANDLE icdev, unsigned char flags, unsigned char block, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_lock_block_hex(DEVHANDLE icdev, unsigned char flags, unsigned char block, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_write_afi(DEVHANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_write_afi_hex(DEVHANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_lock_afi(DEVHANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_lock_afi_hex(DEVHANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_write_dsfid(DEVHANDLE icdev, unsigned char flags, unsigned char DSFID, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_write_dsfid_hex(DEVHANDLE icdev, unsigned char flags, unsigned char DSFID, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_lock_dsfid(DEVHANDLE icdev, unsigned char flags, unsigned char DSFID, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_lock_dsfid_hex(DEVHANDLE icdev, unsigned char flags, unsigned char DSFID, unsigned char *UID);

  /**
   * @brief  ������
   */
  short USER_API dc_get_systeminfo(DEVHANDLE icdev, unsigned char flags, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_get_systeminfo_hex(DEVHANDLE icdev, unsigned char flags, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_get_securityinfo(DEVHANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_get_securityinfo_hex(DEVHANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  ��ȡFM11RF005�����кš�
   * @par    ˵����
   * ��ȡFM11RF005�����кš�
   * @param[in] icdev �豸��ʶ����
   * @param[out] _Snr ���صĿ����кš�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_getsnr_fm11rf005(DEVHANDLE icdev, unsigned int *_Snr);

  /**
   * @brief  ��ȡFM11RF005�����кš�
   * @par    ˵����
   * ::dc_getsnr_fm11rf005 ��HEX��ʽ�ӿڣ����� @a snrstr ΪHEX��ʽ��
   */
  short USER_API dc_getsnr_fm11rf005_hex(DEVHANDLE icdev, unsigned char *snrstr);

  /**
   * @brief  дFM11RF005����
   * @par    ˵����
   * д�����ݵ�FM11RF005���С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr ���ַ��2~15����
   * @param[in] _Data �������ݣ��̶�Ϊ4���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_fm11rf005(DEVHANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  ��FM11RF005����
   * @par    ˵����
   * ��ȡFM11RF005�������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Adr ���ַ��0~15����
   * @param[out] _Data ���ص����ݣ��̶�Ϊ4���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_fm11rf005(DEVHANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  ��FM11RF005����
   * @par    ˵����
   * ::dc_read_fm11rf005 ��HEX��ʽ�ӿڣ����� @a _Data ΪHEX��ʽ��
   */
  short USER_API dc_read_fm11rf005_hex(DEVHANDLE icdev, unsigned char _Adr, char *_Data);

  /**
   * @brief  дFM11RF005����
   * @par    ˵����
   * ::dc_write_fm11rf005 ��HEX��ʽ�ӿڣ����� @a _Data ΪHEX��ʽ��
   */
  short USER_API dc_write_fm11rf005_hex(DEVHANDLE icdev, unsigned char _Adr, char *_Data);

  /**
   * @brief  ������
   */
  short USER_API DCDEV_CommandMcu(DEVHANDLE icdev, unsigned char ctimeout, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  ������
   */
  short USER_API DCDEV_CommandMcu_Hex(DEVHANDLE icdev, unsigned char ctimeout, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_displcd(DEVHANDLE icdev, unsigned char flag);

  /**
   * @brief  �����������롣
   * @par    ˵����
   * ��ȡ���̰����������������룬��ͬ�ļ���װ�ÿ����в�ͬ��Ч�����粻ͬ����ʾ����ͬ����ʾ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[out] rlen ���ذ������ݵĳ��ȣ�����'\0'��
   * @param[out] cpass ���صİ������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==0xA1��ʾȡ�����룬==0xA2��ʾ���볬ʱ��
   */
  short USER_API dc_getinputpass(DEVHANDLE icdev, unsigned char ctime, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  �����������롣
   * @par    ˵����
   * ��ȡ���̰����������������룬��ͬ�ļ���װ�ÿ����в�ͬ��Ч�����粻ͬ����ʾ����ͬ����ʾ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[out] rlen ���ذ������ݵĳ��ȣ�����'\0'��
   * @param[out] cpass ���صİ������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==0xA1��ʾȡ�����룬==0xA2��ʾ���볬ʱ��
   */
  short USER_API dc_GetInputPass(DEVHANDLE icdev, unsigned char ctime, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  ����������
   * @par    ˵����
   * ��ȡ�����������ݣ�֧��2��3�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[out] pTrack2Data ���ص�2������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[out] pTrack2Len ����2������ݵĳ��ȣ�����'\0'��
   * @param[out] pTrack3Data ���ص�3������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[out] pTrack3Len ����3������ݵĳ��ȣ�����'\0'��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readmagcard(DEVHANDLE icdev, unsigned char ctime, unsigned char *pTrack2Data, unsigned int *pTrack2Len, unsigned char *pTrack3Data, unsigned int *pTrack3Len);

  /**
   * @brief  д��������
   * @par    ˵����
   * д�����ݵ��������С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[in] pTrack1Data ����1������ݡ�
   * @param[in] pTrack1Len 1������ݵĳ��ȡ�
   * @param[in] pTrack2Data ����2������ݡ�
   * @param[in] pTrack2Len 2������ݵĳ��ȡ�
   * @param[in] pTrack3Data ����3������ݡ�
   * @param[in] pTrack3Len 3������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_writemagcard(DEVHANDLE icdev, unsigned char ctime, unsigned char *pTrack1Data, unsigned char pTrack1Len, unsigned char *pTrack2Data, unsigned char pTrack2Len, unsigned char *pTrack3Data, unsigned char pTrack3Len);

  /**
   * @brief  �����豸ͨѶ��
   * @par    ˵����
   * ���Ժ��豸֮���Ƿ��������ͨѶ��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_testdevicecomm(DEVHANDLE icdev);

  /**
   * @brief  ��ʾ�����档
   * @par    ˵����
   * ʹ�豸��Ļ����ʾԤ�õ������棬��ͬ�豸�������治����ͬ��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_dispmainmenu(DEVHANDLE icdev);

  /**
   * @brief  �����豸ʱ�䡣
   * @par    ˵����
   * �����豸�ĵ�ǰʱ�䡣
   * @param[in] icdev �豸��ʶ����
   * @param[in] year �꣨0~99����
   * @param[in] month �£�1~12����
   * @param[in] date �գ�1~31����
   * @param[in] hour ʱ��0~23����
   * @param[in] minute �֣�0~59����
   * @param[in] second �루0~59����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_setdevicetime(DEVHANDLE icdev, unsigned char year, unsigned char month, unsigned char date, unsigned char hour, unsigned char minute, unsigned char second);

  /**
   * @brief  ��ȡ�豸ʱ�䡣
   * @par    ˵����
   * ��ȡ�豸��ǰ��ʱ�䡣
   * @param[in] icdev �豸��ʶ����
   * @param[out] year �꣨0~99����
   * @param[out] month �£�1~12����
   * @param[out] date �գ�1~31����
   * @param[out] hour ʱ��0~23����
   * @param[out] minute �֣�0~59����
   * @param[out] second �루0~59����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_getdevicetime(DEVHANDLE icdev, unsigned char *year, unsigned char *month, unsigned char *date, unsigned char *hour, unsigned char *minute, unsigned char *second);

  /**
   * @brief  Һ����ʾ��
   * @par    ˵����
   * ��ʾָ���ַ�����Һ����Ļ�ϡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] line �кš�
   * @param[in] offset ƫ�ơ�
   * @param[in] data Ҫ��ʾ���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_dispinfo(DEVHANDLE icdev, unsigned char line, unsigned char offset, char *data);

  /**
   * @brief  Һ����ʾ��
   * @par    ˵����
   * ��ʾָ���ַ�����Һ����Ļ�ϡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ��Ļƫ�ơ�
   * @param[in] data Ҫ��ʾ���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_dispmaininfo(DEVHANDLE icdev, unsigned char offset, char *data);

  /**
   * @brief  �豸������
   * @par    ˵����
   * �豸����������ָ��ʱ��ķ�������
   * @param[in] icdev �豸��ʶ����
   * @param[in] beeptime ����ʱ�䣬��λΪ50���롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_posbeep(DEVHANDLE icdev, unsigned char beeptime);

  /**
   * @brief  LCD������ơ�
   * @par    ˵����
   * �����豸��LCD���⡣
   * @param[in] icdev �豸��ʶ����
   * @param[in] cOpenFlag 0-�����⣬1-�ر��⡣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ctlbacklight(DEVHANDLE icdev, unsigned char cOpenFlag);

  /**
   * @brief  ָʾ�ƿ��ơ�
   * @par    ˵����
   * �����豸��ָʾ�ơ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] cLed ָʾ�Ʊ�ţ�0��ʾȫ��ָʾ�ƣ�1��ʾ��һ��ָʾ�ƣ�2��ʾ�ڶ���ָʾ�ƣ��Դ����ơ�
   * @param[in] cOpenFlag 0-������1-Ϩ��2-��˸��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ctlled(DEVHANDLE icdev, unsigned char cLed, unsigned char cOpenFlag);

  /**
   * @brief  LCD������
   * @par    ˵����
   * ���LCD��Ļ����ʾ���ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] cLine ��Ҫ������кţ���Ŵ�1��ʼ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_lcdclrscrn(DEVHANDLE icdev, unsigned char cLine);

  /**
   * @brief  ���������ģʽ��
   * @par    ˵����
   * ʹ�豸�����ȡ���̵�״̬���ӿڱ�����ֱ�ӷ��أ�������ȴ�������ɻ��豸��ʱ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_passin(DEVHANDLE icdev, unsigned char ctime);

  /**
   * @brief  ���������ģʽ��
   * @par    ˵����
   * ʹ�豸�����ȡ���̵�״̬���ӿڱ�����ֱ�ӷ��أ�������ȴ�������ɻ��豸��ʱ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_PassIn(DEVHANDLE icdev, unsigned char ctime);

  /**
   * @brief  ���������ģʽ��
   * @par    ˵����
   * ʹ�豸�����ȡ���̵�״̬���ӿڱ�����ֱ�ӷ��أ�������ȴ�������ɻ��豸��ʱ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[in] ucmode ģʽ��0-�����ţ�1-�������š�
   * @param[in] cardno �����ַ�����ֻ���ڴ����ŵ�ģʽ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_passin2(DEVHANDLE icdev, unsigned char ctime, unsigned char ucmode, unsigned char *cardno);

  /**
   * @brief  ���������ģʽ��
   * @par    ˵����
   * ʹ�豸�����ȡ���̵�״̬���ӿڱ�����ֱ�ӷ��أ�������ȴ�������ɻ��豸��ʱ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[in] ucmode ģʽ��0-�����ţ�1-�������š�
   * @param[in] cardno �����ַ�����ֻ���ڴ����ŵ�ģʽ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_PassIn2(DEVHANDLE icdev, unsigned char ctime, unsigned char ucmode, unsigned char *cardno);

  /**
   * @brief  ��ȡ�������ݡ�
   * @par    ˵����
   * ��ȡ�豸�Ѿ���ȡ���ļ������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen ���ذ������ݵĳ��ȣ�����'\0'��
   * @param[out] cpass ���صİ������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_passget(DEVHANDLE icdev, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  ��ȡ�������ݡ�
   * @par    ˵����
   * ��ȡ�豸�Ѿ���ȡ���ļ������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen ���ذ������ݵĳ��ȣ�����'\0'��
   * @param[out] cpass ���صİ������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_PassGet(DEVHANDLE icdev, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  �˳�������ģʽ��
   * @par    ˵����
   * ���豸���������ģʽ���豸��һֱ���ڽ��ռ��������״̬����ʱֻ�а�����ɡ��豸��ʱ����ô˽ӿڲ���ʹ���豸�˳�����״̬��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_passcancel(DEVHANDLE icdev);

  /**
   * @brief  �˳�������ģʽ��
   * @par    ˵����
   * ���豸���������ģʽ���豸��һֱ���ڽ��ռ��������״̬����ʱ��ֻ�а�����ɡ��豸��ʱ����ô˽ӿڲ���ʹ���豸�˳�����״̬��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_PassCancel(DEVHANDLE icdev);

  /**
   * @brief  ��ȡ�������롣
   * @par    ˵����
   * ��ȡ���̰�����ͬʱ��������ʱ��Ļ��ʾ����Ϣ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] disptype ��ʾ���ͣ����������豸������ʾ��Ŀ��ء�
   * @param[in] line �кţ���Ϣ������ʾ�ڴ��к�ָ�������С�
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[out] rlen ���ذ������ݵĳ��ȣ�����'\0'��
   * @param[out] ckeydata ���صİ������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_getinputkey(DEVHANDLE icdev, unsigned char disptype, unsigned char line, unsigned char ctime, unsigned char *rlen, unsigned char *ckeydata);

  /**
   * @brief  ������
   */
  short USER_API dc_displcd_ext(DEVHANDLE icdev, unsigned char flag, unsigned char row, unsigned char offset);

  /**
   * @brief  ����������
   * @par    ˵����
   * ��ȡ�����������ݣ�֧��1��2��3�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[out] pTrack1Data ���ص�1������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[out] pTrack1Len ����1������ݵĳ��ȣ�����'\0'��
   * @param[out] pTrack2Data ���ص�2������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[out] pTrack2Len ����2������ݵĳ��ȣ�����'\0'��
   * @param[out] pTrack3Data ���ص�3������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[out] pTrack3Len ����3������ݵĳ��ȣ�����'\0'��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readmagcardall(DEVHANDLE icdev, unsigned char ctime, unsigned char *pTrack1Data, unsigned int *pTrack1Len, unsigned char *pTrack2Data, unsigned int *pTrack2Len, unsigned char *pTrack3Data, unsigned int *pTrack3Len);

  /**
   * @brief  ��ȡ�豸���кš�
   * @par    ˵����
   * ��ȡ�豸�ڲ����Ƶ����кţ��豸Ĭ�����к�Ϊ�գ�ֻ��Ԥ�ȶ��Ƶ��豸�Ż���ڿ��õ����кš�
   * @param[in] icdev �豸��ʶ����
   * @param[out] snr ���ص����к��ַ����������ٷ���33���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readdevsnr(DEVHANDLE icdev, unsigned char *snr);

  /**
   * @brief  ��ȡ�豸���кš�
   * @par    ˵����
   * ��ȡ�豸�ڲ����Ƶ����кţ��豸Ĭ�����к�Ϊ�գ�ֻ��Ԥ�ȶ��Ƶ��豸�Ż���ڿ��õ����кš�
   * @param[in] icdev �豸��ʶ����
   * @param[out] snr ���ص����к��ַ����������ٷ���17���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readreadersnr(DEVHANDLE icdev, unsigned char *snr);

  /**
   * @brief  ��λ�豸��
   * @par    ˵����
   * ʹ�豸�����ϵ��ʼ״̬��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_resetdevice(DEVHANDLE icdev);

  /**
   * @brief  ���ô����豸ͨѶ�����ʡ�
   * @par    ˵����
   * ���ô����豸ͨѶ�����ʡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] baud �����ʡ�
   * @n 0x00 - 115200��
   * @n 0x01 - 57600��
   * @n 0x02 - 38400��
   * @n 0x03 - 19200��
   * @n 0x04 - 9600��
   * @n 0x05 - 4800��
   * @n 0x06 - 2400��
   * @n 0x07 - 1200��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_setdevicebaud(DEVHANDLE icdev, unsigned char baud);

  /**
   * @brief  ��4442����
   * @par    ˵����
   * ��ȡ4442�������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] data_buffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_4442(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  ��4442����
   * @par    ˵����
   * ::dc_read_4442 ��HEX��ʽ�ӿڣ����� @a data_buffer ΪHEX��ʽ��
   */
  short USER_API dc_read_4442_hex(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  д4442����
   * @par    ˵����
   * д�����ݵ�4442���С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length д�볤�ȡ�
   * @param[in] data_buffer �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_4442(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  д4442����
   * @par    ˵����
   * ::dc_write_4442 ��HEX��ʽ�ӿڣ����� @a data_buffer ΪHEX��ʽ��
   */
  short USER_API dc_write_4442_hex(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  ��֤4442�����롣
   * @par    ˵����
   * ʹ�ô������������֤4442�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] passwd ���룬�̶�Ϊ3���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_verifypin_4442(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  ��֤4442�����롣
   * @par    ˵����
   * ::dc_verifypin_4442 ��HEX��ʽ�ӿڣ����� @a passwd ΪHEX��ʽ��
   */
  short USER_API dc_verifypin_4442_hex(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  ��4442�����롣
   * @par    ˵����
   * ��ȡ4442�������롣
   * @param[in] icdev �豸��ʶ����
   * @param[out] passwd ���룬�̶�Ϊ3���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readpin_4442(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  ��4442�����롣
   * @par    ˵����
   * ::dc_readpin_4442 ��HEX��ʽ�ӿڣ����� @a passwd ΪHEX��ʽ��
   */
  short USER_API dc_readpin_4442_hex(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  ��4442�����������
   * @par    ˵����
   * ��ȡ4442��������������˼���ֵ��ʾ���Գ�����֤����Ĵ�����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�����Ϊ�������ֵ��
   */
  short USER_API dc_readpincount_4442(DEVHANDLE icdev);

  /**
   * @brief  �޸�4442�����롣
   * @par    ˵����
   * �޸�4442�������롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] passwd ���룬�̶�Ϊ3���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_changepin_4442(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  �޸�4442�����롣
   * @par    ˵����
   * ::dc_changepin_4442 ��HEX��ʽ�ӿڣ����� @a passwd ΪHEX��ʽ��
   */
  short USER_API dc_changepin_4442_hex(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  ��4442������λ��
   * @par    ˵����
   * ��ȡ4442���ı���������Щλ���Ѿ����ñ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] data_buffer ���ص����ݣ������к���0x00�ֽڵ�λ�ñ�ʾ�Ѿ����ñ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readwrotect_4442(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  ��4442������λ��
   * @par    ˵����
   * ::dc_readwrotect_4442 ��HEX��ʽ�ӿڣ����� @a data_buffer ΪHEX��ʽ��
   */
  short USER_API dc_readwrotect_4442_hex(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  ��4442������λ��
   * @par    ˵����
   * ��ȡ4442���ı���������Щλ���Ѿ����ñ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] data_buffer ���ص����ݣ������к���0x00�ֽڵ�λ�ñ�ʾ�Ѿ����ñ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readprotect_4442(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  ��4442������λ��
   * @par    ˵����
   * ::dc_readprotect_4442 ��HEX��ʽ�ӿڣ����� @a data_buffer ΪHEX��ʽ��
   */
  short USER_API dc_readprotect_4442_hex(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  д4442������λ��
   * @par    ˵����
   * ��4442���ı�������ָ��λ�ý����ñ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length д�볤�ȡ�
   * @param[in] data_buffer �������ݣ������кͿ���ԭ��������ͬ���ֽ�λ�ý����ñ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_writeprotect_4442(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  д4442������λ��
   * @par    ˵����
   * ::dc_writeprotect_4442 ��HEX��ʽ�ӿڣ����� @a data_buffer ΪHEX��ʽ��
   */
  short USER_API dc_writeprotect_4442_hex(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  д24Cϵ�п���
   * @par    ˵����
   * д�����ݵ�24Cϵ�п��У�֧��24C01��24C02��24C04��24C08��24C16��24C32����
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length д�볤�ȡ�
   * @param[in] snd_buffer �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_24c(DEVHANDLE icdev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  д24Cϵ�п���
   * @par    ˵����
   * ::dc_write_24c ��HEX��ʽ�ӿڣ����� @a snd_buffer ΪHEX��ʽ��
   */
  short USER_API dc_write_24c_hex(DEVHANDLE icdev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  д24C64ϵ�п���
   * @par    ˵����
   * д�����ݵ�24C64ϵ�п��У�֧��24C64��24C128��24C256��24C512��24C1024��24C2048��24C4096����
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length д�볤�ȡ�
   * @param[in] snd_buffer �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_24c64(DEVHANDLE icdev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  д24C64ϵ�п���
   * @par    ˵����
   * ::dc_write_24c64 ��HEX��ʽ�ӿڣ����� @a snd_buffer ΪHEX��ʽ��
   */
  short USER_API dc_write_24c64_hex(DEVHANDLE icdev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  ��24Cϵ�п���
   * @par    ˵����
   * ��ȡ24Cϵ�п������ݣ�֧��24C01��24C02��24C04��24C08��24C16��24C32����
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] receive_buffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_24c(DEVHANDLE icdev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  ��24Cϵ�п���
   * @par    ˵����
   * ::dc_read_24c ��HEX��ʽ�ӿڣ����� @a receive_buffer ΪHEX��ʽ��
   */
  short USER_API dc_read_24c_hex(DEVHANDLE icdev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  ��24C64ϵ�п���
   * @par    ˵����
   * ��ȡ24C64ϵ�п������ݣ�֧��24C64��24C128��24C256��24C512��24C1024��24C2048��24C4096����
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] receive_buffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_24c64(DEVHANDLE icdev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  ��24C64ϵ�п���
   * @par    ˵����
   * ::dc_read_24c64 ��HEX��ʽ�ӿڣ����� @a receive_buffer ΪHEX��ʽ��
   */
  short USER_API dc_read_24c64_hex(DEVHANDLE icdev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  ��4428����
   * @par    ˵����
   * ��ȡ4428�������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] data_buffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_4428(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  ��4428����
   * @par    ˵����
   * ::dc_read_4428 ��HEX��ʽ�ӿڣ����� @a data_buffer ΪHEX��ʽ��
   */
  short USER_API dc_read_4428_hex(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  д4428����
   * @par    ˵����
   * д�����ݵ�4428���С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length д�볤�ȡ�
   * @param[in] data_buffer �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_4428(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  д4428����
   * @par    ˵����
   * ::dc_write_4428 ��HEX��ʽ�ӿڣ����� @a data_buffer ΪHEX��ʽ��
   */
  short USER_API dc_write_4428_hex(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  ��֤4428�����롣
   * @par    ˵����
   * ʹ�ô������������֤4428�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] passwd ���룬�̶�Ϊ2���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_verifypin_4428(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  ��֤4428�����롣
   * @par    ˵����
   * ::dc_verifypin_4428 ��HEX��ʽ�ӿڣ����� @a passwd ΪHEX��ʽ��
   */
  short USER_API dc_verifypin_4428_hex(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  ��4428�����롣
   * @par    ˵����
   * ��ȡ4428�������롣
   * @param[in] icdev �豸��ʶ����
   * @param[out] passwd ���룬�̶�Ϊ2���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readpin_4428(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  ��4428�����롣
   * @par    ˵����
   * ::dc_readpin_4428 ��HEX��ʽ�ӿڣ����� @a passwd ΪHEX��ʽ��
   */
  short USER_API dc_readpin_4428_hex(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  ��4428�����������
   * @par    ˵����
   * ��ȡ4428��������������˼���ֵ��ʾ���Գ�����֤����Ĵ�����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�����Ϊ�������ֵ��
   */
  short USER_API dc_readpincount_4428(DEVHANDLE icdev);

  /**
   * @brief  �޸�4428�����롣
   * @par    ˵����
   * �޸�4428�������롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] passwd ���룬�̶�Ϊ2���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_changepin_4428(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  �޸�4428�����롣
   * @par    ˵����
   * ::dc_changepin_4428 ��HEX��ʽ�ӿڣ����� @a passwd ΪHEX��ʽ��
   */
  short USER_API dc_changepin_4428_hex(DEVHANDLE icdev, unsigned char *passwd);

  /**
   * @brief  ��4428������λ��
   * @par    ˵����
   * ��ȡ4428���ı���������Щλ���Ѿ����ñ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] data_buffer ���ص����ݣ������к���0x00�ֽڵ�λ�ñ�ʾ�Ѿ����ñ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readprotect_4428(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  ��4428������λ��
   * @par    ˵����
   * ::dc_readprotect_4428 ��HEX��ʽ�ӿڣ����� @a data_buffer ΪHEX��ʽ��
   */
  short USER_API dc_readprotect_4428_hex(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  д4428������λ��
   * @par    ˵����
   * ��4428���ı�������ָ��λ�ý����ñ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length д�볤�ȡ�
   * @param[in] data_buffer �������ݣ������кͿ���ԭ��������ͬ���ֽ�λ�ý����ñ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_writeprotect_4428(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  д4428������λ��
   * @par    ˵����
   * ::dc_writeprotect_4428 ��HEX��ʽ�ӿڣ����� @a data_buffer ΪHEX��ʽ��
   */
  short USER_API dc_writeprotect_4428_hex(DEVHANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  ���4442����
   * @par    ˵����
   * ��⵱ǰ�������Ƿ����4442����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_Check_4442(DEVHANDLE icdev);

  /**
   * @brief  ���4428����
   * @par    ˵����
   * ��⵱ǰ�������Ƿ����4428����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_Check_4428(DEVHANDLE icdev);

  /**
   * @brief  ���24C01����
   * @par    ˵����
   * ��⵱ǰ�������Ƿ����24C01����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_Check_24C01(DEVHANDLE icdev);

  /**
   * @brief  ���24C02����
   * @par    ˵����
   * ��⵱ǰ�������Ƿ����24C02����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_Check_24C02(DEVHANDLE icdev);

  /**
   * @brief  ���24C04����
   * @par    ˵����
   * ��⵱ǰ�������Ƿ����24C04����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_Check_24C04(DEVHANDLE icdev);

  /**
   * @brief  ���24C08����
   * @par    ˵����
   * ��⵱ǰ�������Ƿ����24C08����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_Check_24C08(DEVHANDLE icdev);

  /**
   * @brief  ���24C16����
   * @par    ˵����
   * ��⵱ǰ�������Ƿ����24C16����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_Check_24C16(DEVHANDLE icdev);

  /**
   * @brief  ���24C64����
   * @par    ˵����
   * ��⵱ǰ�������Ƿ����24C64����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_Check_24C64(DEVHANDLE icdev);

  /**
   * @brief  ���CPU����
   * @par    ˵����
   * ��⵱ǰ�������Ƿ����CPU����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ����T=0��CPU����==1��ʾ����T=1��CPU����
   */
  short USER_API dc_Check_CPU(DEVHANDLE icdev);

  /**
   * @brief  ��⿨��
   * @par    ˵����
   * ��⵱ǰ�����д��ڵĿ����͡�
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڡ�
   * @n 8 - ��ʾ����4442����
   * @n 9 - ��ʾ����4428����
   * @n 30 - ��ʾ����T=0��CPU����
   * @n 31 - ��ʾ����T=1��CPU����
   * @n 21 - ��ʾ����24C01����
   * @n 22 - ��ʾ����24C02����
   * @n 23 - ��ʾ����24C04����
   * @n 24 - ��ʾ����24C08����
   * @n 25 - ��ʾ����24C16����
   * @n 26 - ��ʾ����24C64����
   */
  short USER_API dc_CheckCard(DEVHANDLE icdev);

  /**
   * @brief  ��ȡ�豸���汾��
   * @par    ˵����
   * ��ȡ�豸�ڲ��̼�����ĳ��汾���˳��汾�����豸�ڲ��̼������ʵ�֣��п���ͬ�豸�汾һ�¡�
   * @param[in] icdev �豸��ʶ����
   * @param[out] sver ���صİ汾�ַ����������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_getlongver(DEVHANDLE icdev, unsigned char *sver);

  /**
   * @brief  Ѱ�����󡢷�����ͻ��ѡ��������
   * @par    ˵����
   * �ڲ������� ::dc_request ::dc_anticoll ::dc_select �Ĺ��ܡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��ͬ ::dc_request �� @a _Mode ��
   * @param[out] Strsnr ���صĿ����кţ���ʽΪ�����ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_cardstr(DEVHANDLE icdev, unsigned char _Mode, char *Strsnr);

  /**
   * @brief  ѰType A��Type B�������
   * @par    ˵����
   * ��Type A��Type B������Ѱ���ͼ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen ���ؼ�����Ϣ�ĳ��ȡ�
   * @param[out] rbuf ���صļ�����Ϣ�������ٷ���128���ֽڡ�
   * @param[out] type ���ͣ�'A'��ʾType A����'B'��ʾType B����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_cardAB(DEVHANDLE icdev, unsigned char *rlen, unsigned char *rbuf, unsigned char *type);

  /**
   * @brief  ѰType B�������
   * @par    ˵����
   * ��Type B������Ѱ���ͼ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] rbuf ���صļ�����Ϣ�������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_card_b(DEVHANDLE icdev, unsigned char *rbuf);

  /**
   * @brief  ѰType B�������
   * @par    ˵����
   * ::dc_card_b ��HEX��ʽ�ӿڣ����� @a rbuf ΪHEX��ʽ��
   */
  short USER_API dc_card_b_hex(DEVHANDLE icdev, char *rbuf);

  /**
   * @brief  Һ����ʾ��
   * @par    ˵����
   * ��ʾָ���ַ�����Һ����Ļ�ϡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] line �кš�
   * @param[in] offset ƫ�ơ�
   * @param[in] data Ҫ��ʾ���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_dispinfo_T8(DEVHANDLE icdev, unsigned char line, unsigned char offset, char *data);

  /**
   * @brief  Һ����ʾ��
   * @par    ˵����
   * ��ʾָ���ַ�����Һ����Ļ�ϡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ��Ļƫ�ơ�
   * @param[in] data Ҫ��ʾ���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_dispinfo_pro_T8(DEVHANDLE icdev, unsigned char offset, char *data);

  /**
   * @brief  LCD������
   * @par    ˵����
   * ���LCD��Ļ����ʾ���ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] line ��Ҫ������кţ���Ŵ�1��ʼ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_clearlcd_T8(DEVHANDLE icdev, unsigned char line);

  /**
   * @brief  ָʾ�Ƶ���/Ϩ��
   * @par    ˵����
   * ����/Ϩ���豸��ָʾ�ơ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] cled ָʾ�Ʊ�ţ�0��ʾȫ��ָʾ�ƣ�1��ʾ��һ��ָʾ�ƣ�2��ʾ�ڶ���ָʾ�ƣ��Դ����ơ�
   * @param[in] cflag 0-������1-Ϩ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_led_T8(DEVHANDLE icdev, unsigned char cled, unsigned char cflag);

  /**
   * @brief  ������
   */
  short USER_API dc_dispmap_T8(DEVHANDLE icdev, unsigned char *mapdata);

  /**
   * @brief  ������
   */
  short USER_API dc_displow_T8(DEVHANDLE icdev, unsigned char rs, unsigned char cdata);

  /**
   * @brief  ��/д��Ƶ�Ĵ�����
   * @par    ˵����
   * ����Ƶ�Ĵ�����ȡֵ��д��ֵ����Ƶ�Ĵ����С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��д��־��0x00��ʾ����0x01��ʾд��
   * @param[in] _Adr �Ĵ�����ַ��
   * @param[in,out] _Data �Ĵ���ֵ���̶�Ϊ1���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_rw_rfreg(DEVHANDLE icdev, unsigned char flag, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  ��/д��Ƶ�Ĵ�����
   * @par    ˵����
   * ::dc_rw_rfreg ��HEX��ʽ�ӿڣ����� @a _Data ΪHEX��ʽ��
   */
  short USER_API dc_rw_rfreg_hex(DEVHANDLE icdev, unsigned char flag, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  ������
   */
  short USER_API dc_mulrequest_b(DEVHANDLE icdev, unsigned char _Mode, unsigned char AFI, unsigned char *cardnum, unsigned char *mulATQB);

  /**
   * @brief  ������
   */
  short USER_API dc_hltb(DEVHANDLE icdev, unsigned char *PUPI);

  /**
   * @brief  ������
   */
  short USER_API dc_set_poweroff(DEVHANDLE icdev, unsigned int MsTimes, unsigned char TimerClock, unsigned char TimerReload);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ը�Ӧ��CPU������ָ���������ע��˽ӿڲ���װ��Э�鲿�֣���CRC��
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @param[in] timeout ��ʱֵ����ֵֻ�ڲ����豸�ĵײ�ʹ�ã���λΪ250���룬һ����ý���ֵΪ7��
   * @param[in] CRCSTU CRCֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_pro_commandsourceCRC(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char CRCSTU);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_pro_commandsourceCRC ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_pro_commandsourceCRChex(DEVHANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout, unsigned char CRCSTU);

  /**
   * @brief  Mifare Plus�����ø��˻����ݣ�0������
   * @par    ˵����
   * ����Mifare Plus���ĸ��˻����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] BNr Ҫд��ĸ��˻����ݿ�š�
   * @param[in] dataperso Ҫд������ݣ��̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL0_writeperso(DEVHANDLE icdev, unsigned int BNr, unsigned char *dataperso);

  /**
   * @brief  Mifare Plus�����ø��˻����ݣ�0������
   * @par    ˵����
   * ::dc_MFPL0_writeperso ��HEX��ʽ�ӿڣ����� @a dataperso ΪHEX��ʽ��
   */
  short USER_API dc_MFPL0_writeperso_hex(DEVHANDLE icdev, unsigned int BNr, unsigned char *dataperso);

  /**
   * @brief  Mifare Plus���ύ���˻����ݣ�0������
   * @par    ˵����
   * �ύMifare Plus���ĸ��˻����ݣ��ύ�ɹ���Ƭ����1��״̬��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL0_commitperso(DEVHANDLE icdev);

  /**
   * @brief  ��֤Mifare Plus��״̬���루1������
   * @par    ˵����
   * ��֤Mifare Plus��״̬���룬������1��״̬��ʵ���ϸ����֤��
   * @param[in] icdev �豸��ʶ����
   * @param[in] authkey ��֤���룬�̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL1_authl1key(DEVHANDLE icdev, unsigned char *authkey);

  /**
   * @brief  ��֤Mifare Plus��״̬���루1������
   * @par    ˵����
   * ::dc_MFPL1_authl1key ��HEX��ʽ�ӿڣ����� @a authkey ΪHEX��ʽ��
   */
  short USER_API dc_MFPL1_authl1key_hex(DEVHANDLE icdev, unsigned char *authkey);

  /**
   * @brief  ����Mifare Plus��״̬��2����1������
   * @par    ˵����
   * ״̬�л�������ִ�иò�����1��״̬�Ŀ�Ƭת����2����
   * @param[in] icdev �豸��ʶ����
   * @param[in] authkey �������룬�̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL1_switchtol2(DEVHANDLE icdev, unsigned char *authkey);

  /**
   * @brief  ����Mifare Plus��״̬��3����1������
   * @par    ˵����
   * ״̬�л�������ִ�иò�����1��״̬�Ŀ�Ƭת����3����
   * @param[in] icdev �豸��ʶ����
   * @param[in] authkey �������룬�̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL1_switchtol3(DEVHANDLE icdev, unsigned char *authkey);

  /**
   * @brief  ����Mifare Plus��״̬��3����2������
   * @par    ˵����
   * ״̬�л�������ִ�иò�����2��״̬�Ŀ�Ƭת����3����
   * @param[in] icdev �豸��ʶ����
   * @param[in] authkey �������룬�̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL2_switchtol3(DEVHANDLE icdev, unsigned char *authkey);

  /**
   * @brief  ��֤Mifare Plus��״̬���루3������
   * @par    ˵����
   * ִ��3��״̬��Ƭ��֤�����������ŵĲ�ͬ����֤��ͬ�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] keyBNr �����š�
   * @param[in] authkey ��֤���룬�̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL3_authl3key(DEVHANDLE icdev, unsigned int keyBNr, unsigned char *authkey);

  /**
   * @brief  ��֤Mifare Plus��״̬���루3������
   * @par    ˵����
   * ::dc_MFPL3_authl3key ��HEX��ʽ�ӿڣ����� @a authkey ΪHEX��ʽ��
   */
  short USER_API dc_MFPL3_authl3key_hex(DEVHANDLE icdev, unsigned int keyBNr, unsigned char *authkey);

  /**
   * @brief  ��֤Mifare Plus��״̬���루3������
   * @par    ˵����
   * ִ��3��״̬��Ƭ��֤���������������ŵĲ�ͬ����֤��ͬ�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0x00��ʾ��֤A���룬�ڲ���0x4000+sectorBNr*2��������飬0x04��ʾ��֤B���룬�ڲ���0x4000+sectorBNr*2+1��������顣
   * @param[in] sectorBNr ���������š�
   * @param[in] authkey ��֤���룬�̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL3_authl3sectorkey(DEVHANDLE icdev, unsigned char mode, unsigned int sectorBNr, unsigned char *authkey);

  /**
   * @brief  ��֤Mifare Plus��״̬���루3������
   * @par    ˵����
   * ::dc_MFPL3_authl3key ��HEX��ʽ�ӿڣ����� @a authkey ΪHEX��ʽ��
   */
  short USER_API dc_MFPL3_authl3sectorkey_hex(DEVHANDLE icdev, unsigned char mode, unsigned int sectorBNr, unsigned char *authkey);

  /**
   * @brief  ��Mifare Plus�����ݣ�3������
   * @par    ˵����
   * ��3��״̬�¶�ȡMifare Plus�����ݣ�������������飬ÿ��16�ֽڡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] BNr ��ʼ���ַ��
   * @param[in] Numblock ����Ŀ��һ�㲻����6�顣
   * @param[out] readdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL3_readinplain(DEVHANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata);

  /**
   * @brief  ��Mifare Plus�����ݣ�3������
   * @par    ˵����
   * ::dc_MFPL3_readinplain ��HEX��ʽ�ӿڣ����� @a readdata ΪHEX��ʽ��
   */
  short USER_API dc_MFPL3_readinplain_hex(DEVHANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata);

  /**
   * @brief  ����ģʽ��Mifare Plus�����ݣ�3������
   * @par    ˵����
   * ��3��״̬���ü���ģʽ��ȡMifare Plus�����ݣ�������������飬ÿ��16�ֽڡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] BNr ��ʼ���ַ��
   * @param[in] Numblock ����Ŀ��һ�㲻����6�顣
   * @param[out] readdata ���ص����ݡ�
   * @param[in] flag ��־��0x00��ʾ���������ڲ����ܺ��ٷ��أ�0x01��ʾ��������ֱ�ӷ��ء�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL3_readencrypted(DEVHANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata, unsigned char flag);

  /**
   * @brief  ����ģʽ��Mifare Plus�����ݣ�3������
   * @par    ˵����
   * ::dc_MFPL3_readencrypted ��HEX��ʽ�ӿڣ����� @a readdata ΪHEX��ʽ��
   */
  short USER_API dc_MFPL3_readencrypted_hex(DEVHANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata, unsigned char flag);

  /**
   * @brief  дMifare Plus�����ݣ�3������
   * @par    ˵����
   * ��3��״̬��д�����ݵ�Mifare Plus���У���������д��飬ÿ��16�ֽڡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] BNr ��ʼ���ַ��
   * @param[in] Numblock ����Ŀ��һ�㲻����6�顣
   * @param[in] writedata �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL3_writeinplain(DEVHANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata);

  /**
   * @brief  дMifare Plus�����ݣ�3������
   * @par    ˵����
   * ::dc_MFPL3_writeinplain ��HEX��ʽ�ӿڣ����� @a writedata ΪHEX��ʽ��
   */
  short USER_API dc_MFPL3_writeinplain_hex(DEVHANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata);

  /**
   * @brief  ����ģʽдMifare Plus�����ݣ�3������
   * @par    ˵����
   * ��3��״̬���ü���ģʽд�����ݵ�Mifare Plus���У���������д��飬ÿ��16�ֽڡ�ע�⣺д����ʱ������ʹ�ô˺������ҿ���Ŀֻ��Ϊ1��
   * @param[in] icdev �豸��ʶ����
   * @param[in] BNr ��ʼ���ַ��
   * @param[in] Numblock ����Ŀ��һ�㲻����6�顣
   * @param[in] writedata �������ݡ�
   * @param[in] flag ��־��0x00��ʾ�����������ڲ����ܺ���ʹ�ã�0x01��ʾ��������ֱ��ʹ�á�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MFPL3_writeencrypted(DEVHANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata, unsigned char flag);

  /**
   * @brief  ����ģʽдMifare Plus�����ݣ�3������
   * @par    ˵����
   * ::dc_MFPL3_writeencrypted ��HEX��ʽ�ӿڣ����� @a writedata ΪHEX��ʽ��
   */
  short USER_API dc_MFPL3_writeencrypted_hex(DEVHANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata, unsigned char flag);

  /**
   * @brief  ��֤Mifare Ultralight C�����롣
   * @par    ˵����
   * ʹ�ô������������֤Mifare Ultralight C�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] key ���룬�̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_auth_ulc(DEVHANDLE icdev, unsigned char *key);

  /**
   * @brief  ��֤Mifare Ultralight C�����롣
   * @par    ˵����
   * ::dc_auth_ulc ��HEX��ʽ�ӿڣ����� @a key ΪHEX��ʽ��
   */
  short USER_API dc_auth_ulc_hex(DEVHANDLE icdev, unsigned char *key);

  /**
   * @brief  �޸�Mifare Ultralight C�����롣
   * @par    ˵����
   * �޸�Mifare Ultralight C�������롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] newkey ���룬�̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_changekey_ulc(DEVHANDLE icdev, unsigned char *newkey);

  /**
   * @brief  �޸�Mifare Ultralight C�����롣
   * @par    ˵����
   * ::dc_changekey_ulc ��HEX��ʽ�ӿڣ����� @a newkey ΪHEX��ʽ��
   */
  short USER_API dc_changekey_ulc_hex(DEVHANDLE icdev, unsigned char *newkey);

  /**
   * @brief  ��ȡ�Ӵ�ʽCPU��������
   * @par    ˵����
   * �˽ӿڿɻ�ȡ�Ӵ�ʽCPU��������Ϊ�Ĳ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] cputype ������ţ�ͬ ::dc_setcpu �� @a _Byte ��
   * @param[out] cpupro ��Э���ţ�0x00��ʾT0��0x01��ʾT1��Ĭ��Ϊ0x00��
   * @param[out] cpuetu ����λ�����ʱ�ţ�0x5C��ʾ9600��0x14��ʾ38400��0x0E��ʾ115200��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_getcpupara(DEVHANDLE icdev, unsigned char cputype, unsigned char *cpupro, unsigned char *cpuetu);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ե�ǰ����CPU������ָ���������ע��˽ӿڲ���װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_cpuapdusourceEXT(DEVHANDLE icdev, short slen, unsigned char *sendbuffer, short *rlen, unsigned char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_cpuapdusourceEXT ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_cpuapdusourceEXT_hex(DEVHANDLE icdev, short slen, char *sendbuffer, short *rlen, char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ե�ǰ����CPU������ָ���������ע��˽ӿ��ѷ�װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_cpuapduEXT(DEVHANDLE icdev, short slen, unsigned char *sendbuffer, short *rlen, unsigned char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_cpuapduEXT ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_cpuapduEXT_hex(DEVHANDLE icdev, short slen, char *sendbuffer, short *rlen, char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ե�ǰ����CPU������ָ���������ע��˽ӿ��ѷ�װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_cpuapduInt(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_cpuapduInt ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_cpuapduInt_hex(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ը�Ӧ��CPU������ָ���������ע��˽ӿ��ѷ�װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @param[in] timeout ��ʱֵ����ֵֻ�ڲ����豸�ĵײ�ʹ�ã���λΪ250���룬һ����ý���ֵΪ7��
   * @param[in] FG �ָ�ֵ����ֵֻ�ڲ����豸�ĵײ�ʹ�ã���λΪ�ֽڣ�һ����ý���ֵΪ64��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_pro_commandlinkEXT(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_pro_commandlinkEXT ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_pro_commandlinkEXT_hex(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ը�Ӧ��CPU������ָ���������ע��˽ӿ��ѷ�װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @param[in] timeout ��ʱֵ����ֵֻ�ڲ����豸�ĵײ�ʹ�ã���λΪ250���룬һ����ý���ֵΪ7��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_pro_commandlinkInt(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_pro_commandlinkInt ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_pro_commandlinkInt_hex(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  ������
   */
  short USER_API dc_exchangeblock(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  ������
   */
  short USER_API dc_exchangeblock_hex(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  ������
   */
  short USER_API dc_write1024(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writebuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_write1024_hex(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writebuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_read1024(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *databuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_read1024_hex(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *databuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_switch_linux(DEVHANDLE icdev);

  /**
   * @brief  �Ӵ�ʽCPU����λ��
   * @par    ˵����
   * �Ե�ǰ����CPU�����и�λ�������˸�λΪ�ȸ�λ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen ���ظ�λ��Ϣ�ĳ��ȡ�
   * @param[out] databuffer ���صĸ�λ��Ϣ�������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_cpuhotreset(DEVHANDLE icdev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU����λ��
   * @par    ˵����
   * ::dc_cpuhotreset ��HEX��ʽ�ӿڣ����� @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_cpuhotreset_hex(DEVHANDLE icdev, unsigned char *rlen, char *databuffer);

  /**
   * @brief  ���࿨״̬��
   * @par    ˵����
   * ����豸�д���ʲô���͵Ŀ�Ƭ��֧�ִſ����ǽӡ��Ӵ�������֤��
   * @param[in] icdev �豸��ʶ����
   * @param[out] flag ״̬��־���̶�Ϊ2���ֽڣ����Խ���Ϊһ���޷���������Big-Endian����ֵΪ�����г���һ�ֻ����֮�͡�
   * @n 0x0001 - ��ʾ��ˢ�ſ���
   * @n 0x0002 - ��ʾ�������ڽӴ�ʽCPU����
   * @n 0x0004 - ��ʾ�������ڽӴ�ʽδ֪����
   * @n 0x0008 - ��ʾ��Ӧ����������֤��
   * @n 0x0010 - ��ʾ��Ӧ�����ڼ���ǰ��Type A CPU����Type B CPU����
   * @n 0x0020 - ��ʾ��Ӧ�����ڼ�����Type A CPU����Type B CPU����
   * @n 0x0040 - ��ʾ��Ӧ�����ڼ���ǰ��M1����
   * @n 0x0100 - ��ʾ��Ӧ�����ڼ���ǰ�Ķ���Type A CPU����
   * @n 0x0200 - ��ʾ��Ӧ�����ڼ���ǰ�Ķ���M1����
   * @n 0x0400 - ��ʾ��Ӧ��ͬʱ���ڼ���ǰ��Type A CPU����M1����
   * @n 0x0800 - ��ʾˢ�ſ�ʧ�ܡ�
   * @n 0x1000 - ��ʾ����ˢ�ſ�ģʽ���ڹر�״̬��
   * @n 0x2000 - ��ʾ����ˢ�ſ�ģʽ���ڿ���״̬��
   * @n 0x4000 - ��ʾ��Ӧ������֤����ǰ��״̬һ�¡�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_card_exist(DEVHANDLE icdev, unsigned char *flag);

  /**
   * @brief  ���Ӵ�ʽ�����ڡ�
   * @par    ˵����
   * ���Ӵ�ʽ��Ƭ�Ƿ�����ڵ�ǰ�����С�
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ���ڣ�==1��ʾ�����ڡ�
   */
  short USER_API dc_card_status(DEVHANDLE icdev);

  /**
   * @brief  ����֤��Type A CPU����Type B CPU����⡣
   * @par    ˵����
   * ����Ӧ���Ƿ��������֤��Type A CPU����Type B CPU����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�޿���==1��ʾ��������֤��==2��ʾ����Type A CPU����==3��ʾ����Type B CPU����
   */
  short USER_API dc_typeab_card_status(DEVHANDLE icdev);

  /**
   * @brief  Ѱ�����󡢷�����ͻ��ѡ��������
   * @par    ˵����
   * �ڲ������� ::dc_request ::dc_anticoll ::dc_select ::dc_anticoll2 ::dc_select2 ::dc_anticoll3 ::dc_select3 �Ĺ��ܡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] _Mode ģʽ��ͬ ::dc_request �� @a _Mode ��
   * @param[out] SnrLen ���ؿ����кŵĳ��ȡ�
   * @param[out] _Snr ���صĿ����кţ������ٷ���8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿����޷�Ѱ����Ƭ��
   */
  short USER_API dc_card_n(DEVHANDLE icdev, unsigned char _Mode, unsigned int *SnrLen, unsigned char *_Snr);

  /**
   * @brief  Ѱ�����󡢷�����ͻ��ѡ��������
   * @par    ˵����
   * ::dc_card_n ��HEX��ʽ�ӿڣ����� @a _Snr ΪHEX��ʽ��
   */
  short USER_API dc_card_n_hex(DEVHANDLE icdev, unsigned char _Mode, unsigned int *SnrLen, unsigned char *_Snr);

  /**
   * @brief  ������
   */
  short USER_API dc_card_n_number(DEVHANDLE icdev, unsigned char _Mode, char *_Snr);

  /**
   * @brief  ��ȡ��������ֵ��
   * @par    ˵����
   * ��ȡ�豸���̵İ���ֵ��ÿ�ε��ý��ܻ�ȡ��������ֵ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] timeout �豸��ʱֵ����λΪ�롣
   * @param[out] value ���صİ���ֵ�������ٷ���8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_pass_key_value(DEVHANDLE icdev, unsigned char timeout, char *value);

  /**
   * @brief  ����������
   * @par    ˵����
   * �����豸����������
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode �����κţ�1~12����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_play_voice(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ����������
   * @par    ˵����
   * �����豸����������
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode �����κţ�1~12����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_playvoice(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  װ��������
   * @par    ˵����
   * װ���������ݵ��豸�ڡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��0��ʾ��һ����1��ʾ�ڶ������Դ����ƣ�0xFFFF��ʾ��������
   * @param[in] slen �������ݵĳ��ȣ�����С�ڻ����1024������־Ϊ��������ʱ��Ϊ0��
   * @param[in] sdata �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_load_voice(DEVHANDLE icdev, int flag, int slen, const unsigned char *sdata);

  /**
   * @brief  �������������á�
   * @par    ˵����
   * ���ò����������Ĳ���������������Զ������ģʽ��Ч��
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0x00��ʾ���������0x01��ʾ�Զ������
   * @param[in] track1 1���������
   * @n bit0 - �������������ã�0��ʾ�������1��ʾ�����
   * @n bit1 - �������ͷ���ã�0��ʾ������ͷ��1��ʾ������ͷ��
   * @n bit2 - �������β���ã�0��ʾ������β��1��ʾ������β��
   * @n bit3 - ������ݻ��з����ã�0��ʾ�޻��з���1��ʾ�л��з���
   * @n bit4 - ������ݵȺ�������������ã�0��ʾ�������1��ʾ�����
   * @n bit5~bit7 - ������
   * @param[in] track2 2���������
   * @n bit0 - �������������ã�0��ʾ�������1��ʾ�����
   * @n bit1 - �������ͷ���ã�0��ʾ������ͷ��1��ʾ������ͷ��
   * @n bit2 - �������β���ã�0��ʾ������β��1��ʾ������β��
   * @n bit3 - ������ݻ��з����ã�0��ʾ�޻��з���1��ʾ�л��з���
   * @n bit4 - ������ݵȺ�������������ã�0��ʾ�������1��ʾ�����
   * @n bit5~bit7 - ������
   * @param[in] track3 3���������
   * @n bit0 - �������������ã�0��ʾ�������1��ʾ�����
   * @n bit1 - �������ͷ���ã�0��ʾ������ͷ��1��ʾ������ͷ��
   * @n bit2 - �������β���ã�0��ʾ������β��1��ʾ������β��
   * @n bit3 - ������ݻ��з����ã�0��ʾ�޻��з���1��ʾ�л��з���
   * @n bit4 - ������ݵȺ�������������ã�0��ʾ�������1��ʾ�����
   * @n bit5~bit7 - ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_setmag(DEVHANDLE icdev, unsigned char mode, unsigned char track1, unsigned char track2, unsigned char track3);

  /**
   * @brief  ���ô�ӡ��������
   * @par    ˵����
   * ���ò�����ӡ���Ĳ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] FontSize �������ã�0x00��ʾ�ַ���СΪ8*16��0x01��ʾ�ַ���СΪ12*24��0x02��ʾ�ַ���СΪ16*32��
   * @param[in] Alignment �������ã�0x00��ʾ����룬0x01��ʾ���У�0x02��ʾ�Ҷ��롣
   * @param[in] LeftMargin ��߾����ã��ַ���СΪ8*16ʱ<48���ַ���СΪ12*24ʱ<32���ַ���СΪ16*32ʱ<24��
   * @param[in] RightMargin �ұ߾����ã��ַ���СΪ8*16ʱ<48���ַ���СΪ12*24ʱ<32���ַ���СΪ16*32ʱ<24��
   * @param[in] RowPitch �м�����ã���λΪ�㡣
   * @param[in] PrintOutRate ��ӡ�ٶ����ã�0x00~0x03����0x00��죬0x03������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_setprint(DEVHANDLE icdev, unsigned char FontSize, unsigned char Alignment, unsigned char LeftMargin, unsigned char RightMargin, unsigned char RowPitch, unsigned char PrintOutRate);

  /**
   * @brief  ��ѯ��ӡ��������
   * @par    ˵����
   * ��ѯ������ӡ���Ĳ�����
   * @param[in] icdev �豸��ʶ����
   * @param[out] FontSize �������ã�0x00��ʾ�ַ���СΪ8*16��0x01��ʾ�ַ���СΪ12*24��0x02��ʾ�ַ���СΪ16*32��
   * @param[out] Alignment �������ã�0x00��ʾ����룬0x01��ʾ���У�0x02��ʾ�Ҷ��롣
   * @param[out] LeftMargin ��߾����ã��ַ���СΪ8*16ʱ<48���ַ���СΪ12*24ʱ<32���ַ���СΪ16*32ʱ<24��
   * @param[out] RightMargin �ұ߾����ã��ַ���СΪ8*16ʱ<48���ַ���СΪ12*24ʱ<32���ַ���СΪ16*32ʱ<24��
   * @param[out] RowPitch �м�����ã���λΪ�㡣
   * @param[out] PrintOutRate ��ӡ�ٶ����ã�0x00~0x03����0x00��죬0x03������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_checkprint(DEVHANDLE icdev, unsigned char *FontSize, unsigned char *Alignment, unsigned char *LeftMargin, unsigned char *RightMargin, unsigned char *RowPitch, unsigned char *PrintOutRate);

  /**
   * @brief  ��ӡ�ַ���
   * @par    ˵����
   * ��ӡ�ı��ַ���Ϣ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] length �ַ����ݵĳ��ȣ�����С�ڻ����480��
   * @param[in] character �ַ����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_printcharacter(DEVHANDLE icdev, unsigned short length, unsigned char *character);

  /**
   * @brief  ��ӡһά�롣
   * @par    ˵����
   * ��ӡһά����Ϣ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] hight һά��ĸ߶ȡ�
   * @param[in] displayflag ��ʾ���ͣ�0x00��ʾͼ�������ı����ϣ�0x01��ʾͼ�������ı����£�0x02��ʾֻ��ʾͼ�β���ʾ�ı���
   * @param[in] FontSize �������ã�0x00��ʾ�ַ���СΪ8*16��0x01��ʾ�ַ���СΪ12*24��0x02��ʾ�ַ���СΪ16*32��
   * @param[in] length �ı����ݵĳ��ȣ�����С�ڻ����15��
   * @param[in] Onedimensional �ı����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_printOnedimensional(DEVHANDLE icdev, unsigned char hight, unsigned char displayflag, unsigned char FontSize, unsigned short length, unsigned char *Onedimensional);

  /**
   * @brief  ��ӡͼƬ��
   * @par    ˵����
   * ��ӡͼƬ��Ϣ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] length ���ȣ���λΪ�ֽڣ�����С�ڻ����48������Ϊ0��
   * @param[in] height �߶ȣ���λΪ�㣬����С��100������Ϊ0��
   * @param[in] Alignment �������ã�0x00��ʾ����룬0x01��ʾ���У�0x02��ʾ�Ҷ��롣
   * @param[in] LeftMargin ��߾����ã�����С�ڻ����48��
   * @param[in] RightMargin �ұ߾����ã�����С�ڻ����48��
   * @param[in] size ͼƬ���ݵĴ�С��
   * @param[in] Picture ͼƬ���ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_printPicture(DEVHANDLE icdev, unsigned char length, unsigned char height, unsigned char Alignment, unsigned char LeftMargin, unsigned char RightMargin, unsigned short size, unsigned char *Picture);

  /**
   * @brief  ��ֽ��
   * @par    ˵����
   * ��ֽ���豸�����ڴ�ӡ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] RowPitch ��ֽ��������λΪ�㡣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_printenter(DEVHANDLE icdev, unsigned char RowPitch);

  /**
   * @brief  ��ȡ��ӡ��״̬��
   * @par    ˵����
   * ��ȡ��ӡ����״̬��
   * @param[in] icdev �豸��ʶ����
   * @param[out] paperstatus ֽ״̬��0x00��ʾ��ȱֽ��0x01��ʾȱֽ��
   * @param[out] tempturestatus �¶�״̬��0x00��ʾ�¶�������0x01��ʾ�¶ȹ��ߡ�
   * @param[out] zkstatus �ֿ�״̬��
   * @n 0x00 - �����ֿⶼ���ڡ�
   * @n 0x01 - �����ֿⶼ�����ڡ�
   * @n 0x02 - ����8*16�ֿ⡣
   * @n 0x03 - ����12*24�ֿ⡣
   * @n 0x04 - ����16*32�ֿ⡣
   * @n 0x05 - ����8*16�ֿ��12*24�ֿ⡣
   * @n 0x06 - ����8*16�ֿ��16*32�ֿ⡣
   * @n 0x07 - ����12*24�ֿ��16*32�ֿ⡣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_printstatus(DEVHANDLE icdev, unsigned char *paperstatus, unsigned char *tempturestatus, unsigned char *zkstatus);

  /**
   * @brief  ����2.4Gģ�鲨���ʡ�
   * @par    ˵����
   * ������2.4Gģ��֮��ͨѶ�Ĳ����ʡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] baund �����ʡ�
   * @n 0x00 - 2400��
   * @n 0x01 - 4800��
   * @n 0x02 - 9600��
   * @n 0x03 - 14400��
   * @n 0x04 - 38400��
   * @n 0x05 - 57600��
   * @n 0x06 - 115200��
   * @n 0x07 - 256000��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_set2dot4Gbaund(DEVHANDLE icdev, unsigned char baund);

  /**
   * @brief  ��ѯ2.4Gģ�鲨���ʡ�
   * @par    ˵����
   * ��ѯ��2.4Gģ��֮��ͨѶ�Ĳ����ʡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] baund �����ʡ�
   * @n 0x00 - 2400��
   * @n 0x01 - 4800��
   * @n 0x02 - 9600��
   * @n 0x03 - 14400��
   * @n 0x04 - 38400��
   * @n 0x05 - 57600��
   * @n 0x06 - 115200��
   * @n 0x07 - 256000��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_check2dot4Gbaund(DEVHANDLE icdev, unsigned char *baund);

  /**
   * @brief  2.4G��λ��
   * @par    ˵����
   * ��2.4G���и�λ������
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen ���ظ�λ��Ϣ�ĳ��ȡ�
   * @param[out] databuffer ���صĸ�λ��Ϣ�������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_2dot4_reset(DEVHANDLE icdev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  2.4G��λ��
   * @par    ˵����
   * ::dc_2dot4_reset ��HEX��ʽ�ӿڣ����� @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_2dot4_resethex(DEVHANDLE icdev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  2.4Gָ�����
   * @par    ˵����
   * ��2.4G����ָ���������
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] senddata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] recvbuffer ���ص����ݡ�
   * @param[in] timeout �豸��ʱֵ����λΪ�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_2dot4_command(DEVHANDLE icdev, unsigned int slen, unsigned char *senddata, unsigned int *rlen, unsigned char *recvbuffer, unsigned char timeout);

  /**
   * @brief  2.4Gָ�����
   * @par    ˵����
   * ::dc_2dot4_command ��HEX��ʽ�ӿڣ����� @a senddata @a recvbuffer ΪHEX��ʽ��
   */
  short USER_API dc_2dot4_command_hex(DEVHANDLE icdev, unsigned int slen, unsigned char *senddata, unsigned int *rlen, unsigned char *recvbuffer, unsigned char timeout);

  /**
   * @brief  ���102����
   * @par    ˵����
   * ����Ƿ����102����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_check_102(DEVHANDLE icdev);

  /**
   * @brief  102���µ硣
   * @par    ˵����
   * ��102�������µ������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_down_102(DEVHANDLE icdev);

  /**
   * @brief  ��102����
   * @par    ˵����
   * ��ȡ102�������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] readdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_102(DEVHANDLE icdev, unsigned char offset, unsigned char length, unsigned char *readdata);

  /**
   * @brief  ��102����
   * @par    ˵����
   * ::dc_read_102 ��HEX��ʽ�ӿڣ����� @a readdata ΪHEX��ʽ��
   */
  short USER_API dc_read_102_hex(DEVHANDLE icdev, unsigned char offset, unsigned char length, unsigned char *readdata);

  /**
   * @brief  д102����
   * @par    ˵����
   * д�����ݵ�102���С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length д�볤�ȡ�
   * @param[in] writedata �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_102(DEVHANDLE icdev, unsigned char offset, unsigned char length, unsigned char *writedata);

  /**
   * @brief  д102����
   * @par    ˵����
   * ::dc_write_102 ��HEX��ʽ�ӿڣ����� @a writedata ΪHEX��ʽ��
   */
  short USER_API dc_write_102_hex(DEVHANDLE icdev, unsigned char offset, unsigned char length, unsigned char *writedata);

  /**
   * @brief  ��֤102�����롣
   * @par    ˵����
   * ʹ�ô������������֤102�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0 - ��ʾ�����룬���볤��Ϊ2���ֽڡ�
   * @n 11 - ��ʾһ���������룬���볤��Ϊ6���ֽڡ�
   * @n 12 - ��ʾ�����������룬���볤��Ϊ4���ֽڡ�
   * @param[in] password ���롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_checkpass_102(DEVHANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  ��֤102�����롣
   * @par    ˵����
   * ::dc_checkpass_102 ��HEX��ʽ�ӿڣ����� @a password ΪHEX��ʽ��
   */
  short USER_API dc_checkpass_102_hex(DEVHANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  �޸�102�����롣
   * @par    ˵����
   * �޸�102�������롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0 - ��ʾ�����룬���볤��Ϊ2���ֽڡ�
   * @n 11 - ��ʾһ���������룬���볤��Ϊ6���ֽڡ�
   * @n 12 - ��ʾ�����������룬���볤��Ϊ4���ֽڡ�
   * @param[in] password ���롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_changepass_102(DEVHANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  �޸�102�����롣
   * @par    ˵����
   * ::dc_changepass_102 ��HEX��ʽ�ӿڣ����� @a password ΪHEX��ʽ��
   */
  short USER_API dc_changepass_102_hex(DEVHANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  ��102�����������
   * @par    ˵����
   * ��ȡ102��������������˼���ֵ��ʾ���Գ�����֤����Ĵ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0 - ��ʾ�����롣
   * @return <0��ʾʧ�ܣ�����Ϊ�������ֵ��
   */
  short USER_API dc_readcount_102(DEVHANDLE icdev, short zone);

  /**
   * @brief  102����˿��
   * @par    ˵����
   * ��102��������˿������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_fuse_102(DEVHANDLE icdev);

  /**
   * @brief  ���1604����
   * @par    ˵����
   * ����Ƿ����1604����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_check_1604(DEVHANDLE icdev);

  /**
   * @brief  1604���µ硣
   * @par    ˵����
   * ��1604�������µ������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_down_1604(DEVHANDLE icdev);

  /**
   * @brief  ��1604����
   * @par    ˵����
   * ��ȡ1604�������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] readdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_1604(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  ��1604����
   * @par    ˵����
   * ::dc_read_1604 ��HEX��ʽ�ӿڣ����� @a readdata ΪHEX��ʽ��
   */
  short USER_API dc_read_1604_hex(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  д1604����
   * @par    ˵����
   * д�����ݵ�1604���С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length д�볤�ȡ�
   * @param[in] writedata �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_1604(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  д1604����
   * @par    ˵����
   * ::dc_write_1604 ��HEX��ʽ�ӿڣ����� @a writedata ΪHEX��ʽ��
   */
  short USER_API dc_write_1604_hex(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  ��֤1604�����롣
   * @par    ˵����
   * ʹ�ô������������֤1604�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0 - ��ʾ�����롣
   * @n 1 - ��ʾһ�����롣
   * @n 2 - ��ʾ�������롣
   * @n 3 - ��ʾ�������롣
   * @n 4 - ��ʾ�������롣
   * @n 11 - ��ʾһ���������롣
   * @n 12 - ��ʾ�����������롣
   * @n 13 - ��ʾ�����������롣
   * @n 14 - ��ʾ�����������롣
   * @param[in] password ���룬�̶�Ϊ2���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_checkpass_1604(DEVHANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  ��֤1604�����롣
   * @par    ˵����
   * ::dc_checkpass_1604 ��HEX��ʽ�ӿڣ����� @a password ΪHEX��ʽ��
   */
  short USER_API dc_checkpass_1604_hex(DEVHANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  �޸�1604�����롣
   * @par    ˵����
   * �޸�1604�������롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0 - ��ʾ�����롣
   * @n 1 - ��ʾһ�����롣
   * @n 2 - ��ʾ�������롣
   * @n 3 - ��ʾ�������롣
   * @n 4 - ��ʾ�������롣
   * @n 11 - ��ʾһ���������롣
   * @n 12 - ��ʾ�����������롣
   * @n 13 - ��ʾ�����������롣
   * @n 14 - ��ʾ�����������롣
   * @param[in] password ���룬�̶�Ϊ2���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_changepass_1604(DEVHANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  �޸�1604�����롣
   * @par    ˵����
   * ::dc_changepass_1604 ��HEX��ʽ�ӿڣ����� @a password ΪHEX��ʽ��
   */
  short USER_API dc_changepass_1604_hex(DEVHANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  ��1604�����������
   * @par    ˵����
   * ��ȡ1604��������������˼���ֵ��ʾ���Գ�����֤����Ĵ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0 - ��ʾ�����롣
   * @n 1 - ��ʾһ�����롣
   * @n 2 - ��ʾ�������롣
   * @n 3 - ��ʾ�������롣
   * @n 4 - ��ʾ�������롣
   * @n 11 - ��ʾһ���������롣
   * @n 12 - ��ʾ�����������롣
   * @n 13 - ��ʾ�����������롣
   * @n 14 - ��ʾ�����������롣
   * @return <0��ʾʧ�ܣ�����Ϊ�������ֵ��
   */
  short USER_API dc_readcount_1604(DEVHANDLE icdev, short zone);

  /**
   * @brief  1604����˿��
   * @par    ˵����
   * ��1604��������˿������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_fuse_1604(DEVHANDLE icdev);

  /**
   * @brief  ���45DBϵ�п���
   * @par    ˵����
   * ����Ƿ����45DBϵ�п���
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_check_45db(DEVHANDLE icdev);

  /**
   * @brief  45DBϵ�п��µ硣
   * @par    ˵����
   * ��45DBϵ�п������µ������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_down_45db(DEVHANDLE icdev);

  /**
   * @brief  ��45DBϵ�п���
   * @par    ˵����
   * ��ȡ45DBϵ�п������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] readdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_45db(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  ��45DBϵ�п���
   * @par    ˵����
   * ::dc_read_45db ��HEX��ʽ�ӿڣ����� @a readdata ΪHEX��ʽ��
   */
  short USER_API dc_read_45db_hex(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  д45DBϵ�п���
   * @par    ˵����
   * д�����ݵ�45DBϵ�п��С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�ơ�
   * @param[in] length д�볤�ȡ�
   * @param[in] writedata �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_45db(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  д45DBϵ�п���
   * @par    ˵����
   * ::dc_write_45db ��HEX��ʽ�ӿڣ����� @a writedata ΪHEX��ʽ��
   */
  short USER_API dc_write_45db_hex(DEVHANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  4428���µ硣
   * @par    ˵����
   * ��4428�������µ������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_down_4428(DEVHANDLE icdev);

  /**
   * @brief  4442���µ硣
   * @par    ˵����
   * ��4442�������µ������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_down_4442(DEVHANDLE icdev);

  /**
   * @brief  24Cϵ�п��µ硣
   * @par    ˵����
   * ��24Cϵ�п������µ������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_down_24c(DEVHANDLE icdev);

  /**
   * @brief  ������
   */
  short USER_API dc_flash_card(DEVHANDLE icdev);

  /**
   * @brief  ������
   */
  short USER_API dc_read_walletbalance(DEVHANDLE icdev, unsigned char *walletvalue);

  /**
   * @brief  ������
   */
  short USER_API dc_consume_wallet(DEVHANDLE icdev, unsigned char *walletvalue, unsigned char *secret);

  /**
   * @brief  ������
   */
  short USER_API dc_transfer_wallet(DEVHANDLE icdev, unsigned char *walletvalue);

  /**
   * @brief  ������
   */
  short USER_API dc_authentication_key(DEVHANDLE icdev, unsigned char *data, unsigned char *desdata);

  /**
   * @brief  ������
   */
  short USER_API dc_set_terminalnumber(DEVHANDLE icdev, unsigned char length, unsigned char *serieldata);

  /**
   * @brief  ������
   */
  short USER_API dc_get_terminalnumber(DEVHANDLE icdev, unsigned char length, unsigned char *serieldata);

  /**
   * @brief  ������
   */
  short USER_API dc_set_trademerchantcode(DEVHANDLE icdev, unsigned char length, unsigned char *merchantcode);

  /**
   * @brief  ������
   */
  short USER_API dc_get_trademerchantcode(DEVHANDLE icdev, unsigned char length, unsigned char *merchantcode);

  /**
   * @brief  ������
   */
  short USER_API dc_set_transactiontype(DEVHANDLE icdev, unsigned char typecode);

  /**
   * @brief  ������
   */
  short USER_API dc_get_transactiontype(DEVHANDLE icdev, unsigned char *typecode);

  /**
   * @brief  ������
   */
  short USER_API dc_set_tradesecretkey(DEVHANDLE icdev, unsigned char *secretkey);

  /**
   * @brief  ������
   */
  short USER_API dc_get_tradesecretkey(DEVHANDLE icdev, unsigned char *secretkey);

  /**
   * @brief  ������
   */
  short USER_API dc_set_hardwareserialnumber(DEVHANDLE icdev, unsigned char length, unsigned char *hardwareserial);

  /**
   * @brief  ������
   */
  short USER_API dc_get_hardwareserialnumber(DEVHANDLE icdev, unsigned char length, unsigned char *hardwareserial);

  /**
   * @brief  ������
   */
  short USER_API dc_reset_factory(DEVHANDLE icdev);

  /**
   * @brief  ������
   */
  short USER_API dc_get_signmessageparameters(DEVHANDLE icdev, unsigned char length, unsigned char *parameters);

  /**
   * @brief  ������
   */
  short USER_API dc_get_record(DEVHANDLE icdev, unsigned char tablenumber, unsigned char recordnumber, unsigned char *recordquantity, unsigned char *recorddata);

  /**
   * @brief  ��������������
   * @par    ˵����
   * ʹ�豸������ˢ������״̬���ӿڱ�����ֱ�ӷ��أ�������ȴ�ˢ���������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_startreadmag(DEVHANDLE icdev);

  /**
   * @brief  ֹͣ����������
   * @par    ˵����
   * ���豸������ˢ������״̬��ֻ��ˢ�����������ô˽ӿڲ���ʹ���豸�˳�����״̬��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_stopreadmag(DEVHANDLE icdev);

  /**
   * @brief  ��ȡ���������ݡ�
   * @par    ˵����
   * ��ȡ�豸�Ѿ���ȡ���Ĵ��������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] pTrack1Data ���ص�1������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[out] pTrack1Len ����1������ݵĳ��ȣ�����'\0'��
   * @param[out] pTrack2Data ���ص�2������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[out] pTrack2Len ����2������ݵĳ��ȣ�����'\0'��
   * @param[out] pTrack3Data ���ص�3������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[out] pTrack3Len ����3������ݵĳ��ȣ�����'\0'��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�ȴ�ˢ����==2��ʾ����ֹͣˢ��״̬��
   */
  short USER_API dc_readmag(DEVHANDLE icdev, unsigned char *pTrack1Data, unsigned int *pTrack1Len, unsigned char *pTrack2Data, unsigned int *pTrack2Len, unsigned char *pTrack3Data, unsigned int *pTrack3Len);

  /**
   * @brief  �Զ���ȡ���������ݡ�
   * @par    ˵����
   * �ڲ������� ::dc_startreadmag ::dc_readmag ::dc_stopreadmag �Ĺ��ܡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �ӿڳ�ʱֵ����λΪ�롣
   * @param[out] pTrack1Data ���ص�1������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[out] pTrack1Len ����1������ݵĳ��ȣ�����'\0'��
   * @param[out] pTrack2Data ���ص�2������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[out] pTrack2Len ����2������ݵĳ��ȣ�����'\0'��
   * @param[out] pTrack3Data ���ص�3������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[out] pTrack3Len ����3������ݵĳ��ȣ�����'\0'��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�ȴ�ˢ����==2��ʾ����ֹͣˢ��״̬��
   */
  short USER_API dc_readmagcardallA(DEVHANDLE icdev, unsigned char ctime, unsigned char *pTrack1Data, unsigned int *pTrack1Len, unsigned char *pTrack2Data, unsigned int *pTrack2Len, unsigned char *pTrack3Data, unsigned int *pTrack3Len);

  /**
   * @brief  �������������Կ��
   * @par    ˵����
   * ����������ڵ���Կ�������ò�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��
   * @n 0x00 - ��ʾװ������Կ������װ�ء�
   * @n 0x01 - ��ʾװ��PIN��Կ������װ�أ�������Կ���ܣ���
   * @n 0x02 - ��ʾװ��MAC��Կ������װ�أ�������Կ���ܣ���
   * @n 0x03 - ��ʾװ��TDK��Կ������װ�أ�������Կ���ܣ���
   * @n 0xF0 - ��ʾ�������Կ��PIN��Կ��MAC��Կ��TDK��Կ��
   * @n 0xF1 - ��ʾ���PIN��Կ��
   * @n 0xF2 - ��ʾ���MAC��Կ��
   * @n 0xF3 - ��ʾ���TDK��Կ��
   * @param[in] keylen ��Կ���ݵĳ��ȡ�
   * @param[in] keydata ��Կ���ݡ�
   * @param[in] mode ģʽ��0x00��ʾ��DES��Կ��0x01��ʾ��DES��Կ��0x02��ʾSM4��Կ��0x03��ʾ24�ֽ���DES��Կ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_InstallKbKey(DEVHANDLE icdev, unsigned char flag, unsigned char keylen, unsigned char *keydata, unsigned char mode);

  /**
   * @brief  ���õ�ǰ��Կ�顣
   * @par    ˵����
   * ����������̵�ǰʹ�õ���Կ�顣
   * @param[in] icdev �豸��ʶ����
   * @param[in] keyID ��Կ��ţ���0x00��ʼ��š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SetKbKeyID(DEVHANDLE icdev, unsigned char keyID);

  /**
   * @brief  ��ȡ���ܰ������ݡ�
   * @par    ˵����
   * ��������̻�ȡANSI X9.8 PIN BLOCK���ܵİ������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[in] panlen ���ŵĳ��ȡ�
   * @param[in] pandata ���š�
   * @param[out] rlen ���ؼ������ݵĳ��ȡ�
   * @param[out] cpass ���صļ������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==0xA1��ʾȡ�����룬==0xA2��ʾ���볬ʱ��
   */
  short USER_API dc_GetEnPass(DEVHANDLE icdev, unsigned char ctime, unsigned char panlen, unsigned char *pandata, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  ������
   */
  short USER_API dc_get55data(DEVHANDLE icdev, unsigned int slen, unsigned char *sdata, unsigned char *PacketType, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  ������
   */
  short USER_API dc_CardIssuingScript(DEVHANDLE icdev, unsigned int slen, unsigned char *sdata, unsigned char *TransactionIsInterrupted, unsigned char *TradingResult);

  /**
   * @brief  ������
   */
  short USER_API dc_SetEMVPara(DEVHANDLE icdev, unsigned int slen, unsigned char *sdata);

  /**
   * @brief  ������
   */
  short USER_API dc_CalculateMACCheck(DEVHANDLE icdev, unsigned char keyindex, unsigned char macmode, unsigned int datalen, unsigned char *srcdata, unsigned char *macdata);

  /**
   * @brief  ������
   */
  short USER_API dc_CalculateDes(DEVHANDLE icdev, unsigned char keyindex, unsigned char enmode, unsigned short datalen, unsigned char *srcdata, unsigned char *desdata);

  /**
   * @brief  ������
   */
  short USER_API dc_CalculateSm4(DEVHANDLE icdev, unsigned char keyindex, unsigned char enmode, unsigned short datalen, unsigned char *srcdata, unsigned char *sm4data);

  /**
   * @brief  �ù�����Կ�ӽ��ܡ�
   * @par    ˵����
   * ʹ��������̵Ĺ�����Կ�������ݼӽ��ܲ�������֧�ֵ�DES����DES��24�ֽ���DES�㷨���㷨��������Կʱȷ������ο� ::dc_InstallKbKey ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] keytype ��Կ���ͣ�0x01��ʾPIN��Կ��0x02��ʾMAC��Կ��0x03��ʾTDK��Կ��
   * @param[in] enmode ����ģʽ��0x00��ʾ���ܣ�0x01��ʾ���ܡ�
   * @param[in] sourcedata �������ݣ��̶�Ϊ8���ֽڡ�
   * @param[out] retdata ���ص����ݣ��̶�Ϊ8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CreatDesData(DEVHANDLE icdev, unsigned char keytype, unsigned char enmode, unsigned char *sourcedata, unsigned char *retdata);

  /**
   * @brief  �ù�����Կ�ӽ��ܡ�
   * @par    ˵����
   * ʹ��������̵Ĺ�����Կ�������ݼӽ��ܲ�������֧��SM4�㷨���㷨��������Կʱȷ������ο� ::dc_InstallKbKey ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] keytype ��Կ���ͣ�0x01��ʾPIN��Կ��0x02��ʾMAC��Կ��0x03��ʾTDK��Կ��
   * @param[in] enmode ����ģʽ��0x00��ʾ���ܣ�0x01��ʾ���ܡ�
   * @param[in] sourcedata �������ݣ��̶�Ϊ16���ֽڡ�
   * @param[out] retdata ���ص����ݣ��̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CreatSm4Data(DEVHANDLE icdev, unsigned char keytype, unsigned char enmode, unsigned char *sourcedata, unsigned char *retdata);

  /**
   * @brief  �ù�����Կ�ӽ��ܡ�
   * @par    ˵����
   * ʹ��������̵Ĺ�����Կ�������ݼӽ��ܲ�������֧�ֵ�DES ECB����DES ECB�㷨���ڲ������ ::dc_CreatDesData ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] keytype ��Կ���ͣ�0x01��ʾPIN��Կ��0x02��ʾMAC��Կ��0x03��ʾTDK��Կ��
   * @param[in] enmode ����ģʽ��0x00��ʾ���ܣ�0x01��ʾ���ܡ�
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sourcedata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] retdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CreatDesDataECB(DEVHANDLE icdev, unsigned char keytype, unsigned char enmode, int slen, unsigned char *sourcedata, int *rlen, unsigned char *retdata);

  /**
   * @brief  �ù�����Կ�ӽ��ܡ�
   * @par    ˵����
   * ʹ��������̵Ĺ�����Կ�������ݼӽ��ܲ�������֧��SM4 ECB�㷨���ڲ������ ::dc_CreatSm4Data ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] keytype ��Կ���ͣ�0x01��ʾPIN��Կ��0x02��ʾMAC��Կ��0x03��ʾTDK��Կ��
   * @param[in] enmode ����ģʽ��0x00��ʾ���ܣ�0x01��ʾ���ܡ�
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sourcedata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] retdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CreatSm4DataECB(DEVHANDLE icdev, unsigned char keytype, unsigned char enmode, int slen, unsigned char *sourcedata, int *rlen, unsigned char *retdata);

  /**
   * @brief  �ù�����Կ��MAC��
   * @par    ˵����
   * ʹ��������̵Ĺ�����Կ����������MAC�������ڲ������ ::dc_CreatDesData ::dc_CreatSm4Data ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] keytype ��Կ���ͣ�0x01��ʾPIN��Կ��0x02��ʾMAC��Կ��0x03��ʾTDK��Կ��
   * @param[in] enmode ����ģʽ��0x00��0x01��ʾ��DES POS ECB��MAC��0x02��ʾ��SM4��MAC��0x03��ʾ��DES X9.9��MAC��0x04��ʾ����DES X9.19��MAC��0x05��ʾ����DES POS ECB��MAC��0x06��ʾ��24�ֽ���DES X9.19��MAC��0x07��ʾ��24�ֽ���DES POS ECB��MAC��
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sourcedata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] retdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CreatMacDataECB(DEVHANDLE icdev, unsigned char keytype, unsigned char enmode, int slen, unsigned char *sourcedata, int *rlen, unsigned char *retdata);

  /**
   * @brief  дSD�洢��
   * @par    ˵����
   * д�����ݵ�SD�洢�У������������ݱ���ȡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�Ƶ�ַ��
   * @param[in] length д�볤�ȡ�
   * @param[in] writebuffer �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_writeSD(DEVHANDLE icdev, int offset, int length, unsigned char *writebuffer);

  /**
   * @brief  ��SD�洢��
   * @par    ˵����
   * ��ȡSD�洢�е����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�Ƶ�ַ��
   * @param[in] length ��ȡ���ȡ�
   * @param[out] readbuffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readSD(DEVHANDLE icdev, int offset, int length, unsigned char *readbuffer);

  /**
   * @brief  ����USBģʽ��
   * @par    ˵����
   * ����USBͨѶģʽ���������¶��豸�����ϵ������Ч��
   * @param[in] icdev �豸��ʶ����
   * @param[in] ucmode ģʽ��
   * @n 0x00 - ��ʾ�жϷ��ͺ��жϽ��գ���ʱVID=257B��PID=3010��
   * @n 0x01 - ��ʾ���Ʒ��ͺͿ��ƽ��գ���ʱVID=257B��PID=3011��
   * @n 0x02 - ��ʾ���Ʒ��ͺ��жϽ��գ���ʱVID=0471��PID=7002��
   * @n 0x03 - ��ʾ���Ʒ��ͺ��жϽ��գ���ʱVID=0471��PID=7003��
   * @n 0x04 - ��ʾ���Ʒ��ͺ��жϽ��գ���ʱVID=0471��PID=7003��֧�ִ������Զ��ϴ����ܡ�
   * @n 0x05 - ���⴮�ڡ�
   * @n 0x06 - ���⴮�ڣ�������HID����ʱHID��VID=0471��PID=7003��
   * @n 0x07 - ��ʾ���Ʒ��ͺ��жϽ��գ���ʱVID=0471��PID=7002��֧�ִ������Զ��ϴ����ܡ�
   * @n 0x08 - ��ʾ���Ʒ��ͺ��жϽ��գ���ʱVID=257B��PID=3031��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SetUsbMode(DEVHANDLE icdev, unsigned char ucmode);

  /**
   * @brief  ����������ͨѶ��
   * @par    ˵����
   * ������������̽���ͨѶ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[in] senlen �������ݵĳ��ȡ�
   * @param[in] source �������ݡ�
   * @param[out] reclen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_InstallKey(DEVHANDLE icdev, unsigned char ctime, unsigned char senlen, unsigned char *source, unsigned char *reclen, unsigned char *rdata);

  /**
   * @brief  ��ȡ���������̵İ汾��
   * @par    ˵����
   * ��ȡ���������̵İ汾��
   * @param[in] icdev �豸��ʶ����
   * @param[out] version ���صİ汾�ַ����������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_GetVersion(DEVHANDLE icdev, char *version);

  /**
   * @brief  ��ȡ�����������豸���кš�
   * @par    ˵����
   * ��ȡ�����������豸�ڲ����Ƶ����кţ��豸Ĭ�����к�Ϊ�գ�ֻ��Ԥ�ȶ��Ƶ��豸�Ż���ڿ��õ����кš�
   * @param[in] icdev �豸��ʶ����
   * @param[out] serial_number ���ص����к��ַ����������ٷ���33���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_ReadSerialNumber(DEVHANDLE icdev, char *serial_number);

  /**
   * @brief  ���������̲���������
   * @par    ˵����
   * ������������������������
   * @param[in] icdev �豸��ʶ����
   * @param[in] _playmode �����κţ�0x01~0x08����
   * @n 0x01 - "����������"��
   * @n 0x02 - "������ԭ����"��
   * @n 0x03 - "��������һ��"��
   * @n 0x04 - "������������"��
   * @n 0x05 - "��ȷ��������"��
   * @n 0x06 - "�����޸ĳɹ�"��
   * @n 0x07 - "��忨"��
   * @n 0x08 - "��ˢ��"��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_PlayVoice(DEVHANDLE icdev, unsigned char _playmode);

  /**
   * @brief  ���������̳�ʼ����
   * @par    ˵����
   * �����������̽��г�ʼ��������ʹ�临λΪ����״̬������״̬�����е�����Կ�ڴ�ռ䶼Ϊ�ֽ�0x88��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_Init(DEVHANDLE icdev);

  /**
   * @brief  ������������������Կ��
   * @par    ˵����
   * �������������ڵ�����Կ�������ò�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] keyset ��Կ��ţ���0x00��ʼ��š�
   * @param[in] oldkey ������Կ���ݣ����ģ��̶�Ϊ16���ֽڡ�
   * @param[in] newkey ������Կ���ݣ����ģ��̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_SetMainKey(DEVHANDLE icdev, unsigned char keyset, unsigned char *oldkey, unsigned char *newkey);

  /**
   * @brief  �������������̹�����Կ��
   * @par    ˵����
   * �������������ڵĹ�����Կ�������ò�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] keysetmain ����Կ��ţ���0x00��ʼ��š�
   * @param[in] keysetwork ������Կ��ţ���0x00��ʼ��š�
   * @param[in] enkeywork ������Կ���ݣ����ģ�������Կ���ܣ����̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_SetWorkKey(DEVHANDLE icdev, unsigned char keysetmain, unsigned char keysetwork, unsigned char *enkeywork);

  /**
   * @brief  �������������̹�����Կ��
   * @par    ˵����
   * �������������̵�ǰʹ�õĹ�����Կ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] keysetmain ����Կ��ţ���0x00��ʼ��š�
   * @param[in] keysetwork ������Կ��ţ���0x00��ʼ��š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_ActWorkKey(DEVHANDLE icdev, unsigned char keysetmain, unsigned char keysetwork);

  /**
   * @brief  ��������������������볤�ȡ�
   * @par    ˵����
   * �������������������û������������󳤶ȡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] keylength ������󳤶ȣ�1~16����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_SetKeyLength(DEVHANDLE icdev, unsigned char keylength);

  /**
   * @brief  ��ȡ�����������������ݡ�
   * @par    ˵����
   * �����������̻�ȡ���İ������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] szPasswd ���صİ������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==0xA1��ʾȡ�����룬==0xA2��ʾ���볬ʱ��
   */
  short USER_API SD_IFD_GetPIN(DEVHANDLE icdev, char *szPasswd);

  /**
   * @brief  ��ȡ�����������������ݡ�
   * @par    ˵����
   * �����������̻�ȡ���İ������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] szPasswd ���صİ������ݣ�����Ϊ���ĵ�HEX��ʽ�ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==0xA1��ʾȡ�����룬==0xA2��ʾ���볬ʱ��
   */
  short USER_API SD_IFD_GetEnPIN(DEVHANDLE icdev, char *szPasswd);

  /**
   * @brief  ��ȡ�����������������ݡ�
   * @par    ˵����
   * �����������̻�ȡ���İ������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] szPasswd ���صİ������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[in] yyflag ��־��
   * @n 0x80 - �̵�����Һ����ʾ"��ӭʹ��"����������ʾ��
   * @n 0x81 - �̵�����Һ����ʾ"��������һ��"����������ʾ��
   * @n 0x82 - �̵�����Һ����ʾ"����������"����������ʾ��
   * @n 0x83 - �̵�����Һ������������������ʾ��
   * @n 0x84 - �̵�����Һ����ʾ"������ԭ����"����������ʾ��
   * @n 0x85 - �̵�����Һ����ʾ"������������"����������ʾ��
   * @n 0x86 - �̵�����Һ����ʾ"������������"����������ʾ��
   * @param[in] timeout �豸��ʱֵ����λΪ�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==0xA1��ʾȡ�����룬==0xA2��ʾ���볬ʱ��
   */
  short USER_API SD_IFD_GetPINPro(DEVHANDLE icdev, char *szPasswd, unsigned char yyflag, unsigned char timeout);

  /**
   * @brief  ��ȡ�����������������ݡ�
   * @par    ˵����
   * �����������̻�ȡ���İ������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] szPasswd ���صİ������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[in] yyflag ��־��
   * @n 0x30 - ���������ʾ"����������"��������ʾ������������ʾ'*'��
   * @n 0x31 - ���������ʾ"��������һ��"��������ʾ������������ʾ'*'��
   * @n 0x32 - ���������ʾ"����������"����������ʾ������������ʾ'*'��
   * @n 0x33 - ���������ʾ"��������һ��"����������ʾ������������ʾ'*'��
   * @n 0x34 - ���������ʾ"����������"��������ʾ����������������ʾ��
   * @n 0x35 - ���������ʾ"��������һ��"��������ʾ����������������ʾ��
   * @n 0x36 - ���������ʾ"����������"����������ʾ����������������ʾ��
   * @n 0x37 - ���������ʾ"��������һ��"����������ʾ����������������ʾ��
   * @param[in] modeflag ģʽ��
   * @n 0x30 - ��"ȷ��"���򵽴�ָ����PIN���ȡ�
   * @n 0x31 - ��"ȷ��"����
   * @n 0x32 - ����ָ����PIN���ȡ�
   * @n 0x33 - ����ָ����PIN���Ȳ���"ȷ��"����
   * @param[in] timeout �豸��ʱֵ����λΪ�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==0xA1��ʾȡ�����룬==0xA2��ʾ���볬ʱ��
   */
  short USER_API SD_IFD_GetEnPINPro(DEVHANDLE icdev, char *szPasswd, unsigned char yyflag, unsigned char modeflag, unsigned char timeout);

  /**
   * @brief  ��ȡ�����������������ݡ�
   * @par    ˵����
   * �����������̻�ȡANSI X9.8 PIN BLOCK���ܵİ������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] szPasswd ���صİ������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[in] yyflag ��־��
   * @n 0x30 - ���������ʾ"����������"��������ʾ������������ʾ'*'��
   * @n 0x31 - ���������ʾ"��������һ��"��������ʾ������������ʾ'*'��
   * @n 0x32 - ���������ʾ"����������"����������ʾ������������ʾ'*'��
   * @n 0x33 - ���������ʾ"��������һ��"����������ʾ������������ʾ'*'��
   * @n 0x34 - ���������ʾ"����������"��������ʾ����������������ʾ��
   * @n 0x35 - ���������ʾ"��������һ��"��������ʾ����������������ʾ��
   * @n 0x36 - ���������ʾ"����������"����������ʾ����������������ʾ��
   * @n 0x37 - ���������ʾ"��������һ��"����������ʾ����������������ʾ��
   * @param[in] modeflag ģʽ��
   * @n 0x30 - ��"ȷ��"���򵽴�ָ����PIN���ȡ�
   * @n 0x31 - ��"ȷ��"����
   * @n 0x32 - ����ָ����PIN���ȡ�
   * @n 0x33 - ����ָ����PIN���Ȳ���"ȷ��"����
   * @param[in] cardno ��ȡ��Ŀ��ţ��̶�Ϊ12���ֽڡ�
   * @param[in] timeout �豸��ʱֵ����λΪ�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==0xA1��ʾȡ�����룬==0xA2��ʾ���볬ʱ��
   */
  short USER_API SD_IFD_GetEnPINBlock(DEVHANDLE icdev, char *szPasswd, unsigned char yyflag, unsigned char modeflag, unsigned char *cardno, unsigned char timeout);

  /**
   * @brief  ���������������㷨��
   * @par    ˵����
   * ��������������ʹ�õ��㷨��
   * @param[in] icdev �豸��ʶ����
   * @param[in] desmode ģʽ��0x01��ʾ��DES��0x02��ʾ��DES��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_SelecetDes(DEVHANDLE icdev, unsigned char desmode);

  /**
   * @brief  ����������Һ����ʾ��
   * @par    ˵����
   * ��ʾָ���ַ��������������̵�Һ����Ļ�ϡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] line �кš�
   * @param[in] row ƫ�ơ�
   * @param[in] leddata Ҫ��ʾ���ַ�����
   * @param[in] timeout �豸��ʱֵ����λΪ�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_LEDDisplay(DEVHANDLE icdev, unsigned char line, unsigned char row, unsigned char *leddata, unsigned char timeout);

  /**
   * @brief  �����������̹�����Կ�ӽ��ܡ�
   * @par    ˵����
   * ʹ�����������̵Ĺ�����Կ�������ݼӽ��ܲ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ����ģʽ��0x00��ʾ���ܣ�0x01��ʾ���ܡ�
   * @param[in] srcdata �������ݣ�HEX��ʽ�ַ�����
   * @param[out] desdata ���ص����ݣ�HEX��ʽ�ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_DES(DEVHANDLE icdev, unsigned char mode, unsigned char *srcdata, unsigned char *desdata);

  /**
   * @brief  �����������̹�����Կ�ӽ��ܡ�
   * @par    ˵����
   * ʹ�����������̵Ĺ�����Կ�������ݼӽ��ܲ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] srclen �������ݵĳ��ȡ�
   * @param[in] srcdata �������ݡ�
   * @param[out] desdata ���ص����ݣ�HEX��ʽ�ַ�����
   * @param[in] flag ����ģʽ��0x00��ʾ���ܣ�0x01��ʾ���ܡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_DesCaculate(DEVHANDLE icdev, unsigned char srclen, unsigned char *srcdata, unsigned char *desdata, unsigned char flag);

  /**
   * @brief  �����������̹�����Կ��MAC��
   * @par    ˵����
   * ʹ�����������̵Ĺ�����Կ����������MAC������
   * @param[in] icdev �豸��ʶ����
   * @param[in] srclen �������ݵĳ��ȡ�
   * @param[in] srcdata �������ݡ�
   * @param[out] macdata ���ص�MAC�ַ����������ٷ���64���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_CreateMac(DEVHANDLE icdev, unsigned char srclen, unsigned char *srcdata, unsigned char *macdata);

  /**
   * @brief  ���������̿�ʼɨ���ά�롣
   * @par    ˵����
   * ���������̿�ʼɨ���ά�롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode �������̶�Ϊ0x00��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_Scan2DBarcodeStart(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ���������̻�ȡ��ά�����ݡ�
   * @par    ˵����
   * ���������̻�ȡ��ά�����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_Scan2DBarcodeGetData(DEVHANDLE icdev, int *rlen, unsigned char *rdata);

  /**
   * @brief  �����������˳�ɨ���ά�롣
   * @par    ˵����
   * �����������˳�ɨ���ά�롣
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_Scan2DBarcodeExit(DEVHANDLE icdev);

  /**
   * @brief  ���������̻�ȡ��Ļ���ԡ�
   * @par    ˵����
   * ���������̻�ȡ��Ļ���ԡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���0x00��ʾ��ȡ�豸�ڲ���ģ���ݻ����ֽڴ�С��0x01��ʾ��ȡ��ĻX�����ظ�����0x02��ʾ��ȡ��ĻY�����ظ�����0x03��ʾ��Ļ����������ռ��λ����
   * @param[out] attribute_len �����������ݵĳ��ȡ�
   * @param[out] attribute ���ص��������ݡ�
   * @n typeΪ0x00ʱ������4�ֽڻ����ֽڴ�С����λ��ǰ��
   * @n typeΪ0x01ʱ������4�ֽ���ĻX�����ظ�������λ��ǰ��
   * @n typeΪ0x02ʱ������4�ֽ���ĻY�����ظ�������λ��ǰ��
   * @n typeΪ0x03ʱ������1�ֽ���Ļ����������ռ��λ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_GetScreenAttribute(DEVHANDLE icdev, unsigned char type, int *attribute_len, unsigned char *attribute);

  /**
   * @brief  ���������̻�����
   * @par    ˵����
   * ���������̰�����ͼ������Ļ�ϣ�ֻ֧��һ���Ի�������Ļ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] pixel_len �������ݵĳ��ȡ�
   * @param[in] pixel �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API SD_IFD_DrawScreen(DEVHANDLE icdev, int pixel_len, const unsigned char *pixel);

  /**
   * @brief  ��������֤��
   * @par    ˵����
   * �Զ�ȡ���ݷ�ʽ�����Ƿ�������֤�����ڸ�Ӧ����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_find_i_d(DEVHANDLE icdev);

  /**
   * @brief  ��������֤��
   * @par    ˵����
   * �Զ�ȡ״̬��ʽ�����Ƿ�������֤�����ڸ�Ӧ����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_find_i_d_2(DEVHANDLE icdev);

  /**
   * @brief  ��������֤��
   * @par    ˵����
   * ��Ѱ����ʽ�����Ƿ�������֤�����ڸ�Ӧ����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_find_i_d_speed(DEVHANDLE icdev);

  /**
   * @brief  ������֤ԭʼ���ݡ�
   * @par    ˵����
   * ��ȡ����֤ģ�鷵�ص�ԭʼ���ݣ������������ֺ���Ƭ���ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] rdata ���ص����ݣ������ٷ���4096���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_get_i_d_raw_buffer(DEVHANDLE icdev, unsigned char *rdata);

  /**
   * @brief  ������֤��
   * @par    ˵����
   * ��ȡ����֤ģ�鷵�ص�ԭʼ���ݣ������������ֺ���Ƭ���ݣ��ڲ������������ҷ�����Դ�����棬�ͷ���Դ��ʹ�� ::dc_end_i_d ��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�����Ϊ����֤��ʶ����
   */
  DEVHANDLE USER_API dc_start_i_d(DEVHANDLE icdev);

  /**
   * @brief  ������֤��
   * @par    ˵����
   * ��ȡ����֤ģ�鷵�ص�ԭʼ���ݣ������������֡���Ƭ��ָ�����ݣ��ڲ������������ҷ�����Դ�����棬�ͷ���Դ��ʹ�� ::dc_end_i_d ��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�����Ϊ����֤��ʶ����
   */
  DEVHANDLE USER_API dc_start_i_d_2(DEVHANDLE icdev);

  /**
   * @brief  ��ȡ������
   * @par    ˵����
   * ��ȡ�ڲ������������
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���ص������ַ�����
   */
  char *USER_API dc_i_d_query_name(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ��ȡ������
   * @par    ˵����
   * ��ȡ�ڲ������������
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[out] name �����������ٷ���32���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_name_2(DEVHANDLE idDEVHANDLE, char *name);

  /**
   * @brief  ��ȡ�Ա�
   * @par    ˵����
   * ��ȡ�ڲ�������Ա�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���ص��Ա��ַ�����
   */
  char *USER_API dc_i_d_query_sex(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ��ȡ�Ա�
   * @par    ˵����
   * ��ȡ�ڲ�������Ա�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[out] sex �Ա������ٷ���8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_sex_2(DEVHANDLE idDEVHANDLE, char *sex);

  /**
   * @brief  ��ȡ���塣
   * @par    ˵����
   * ��ȡ�ڲ���������塣
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���ص������ַ�����
   */
  char *USER_API dc_i_d_query_nation(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ��ȡ���塣
   * @par    ˵����
   * ��ȡ�ڲ���������塣
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[out] nation ���壬�����ٷ���32���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_nation_2(DEVHANDLE idDEVHANDLE, char *nation);

  /**
   * @brief  ��ȡ���ա�
   * @par    ˵����
   * ��ȡ�ڲ���������ա�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���ص������ַ�����
   */
  char *USER_API dc_i_d_query_birth(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ��ȡ���ա�
   * @par    ˵����
   * ��ȡ�ڲ���������ա�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[out] birth ���գ������ٷ���16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_birth_2(DEVHANDLE idDEVHANDLE, char *birth);

  /**
   * @brief  ��ȡסַ��
   * @par    ˵����
   * ��ȡ�ڲ������סַ��
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���ص�סַ�ַ�����
   */
  char *USER_API dc_i_d_query_address(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ��ȡסַ��
   * @par    ˵����
   * ��ȡ�ڲ������סַ��
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[out] address סַ�������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_address_2(DEVHANDLE idDEVHANDLE, char *address);

  /**
   * @brief  ��ȡ�������ݺ��롣
   * @par    ˵����
   * ��ȡ�ڲ�����Ĺ������ݺ��롣
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���صĹ������ݺ����ַ�����
   */
  char *USER_API dc_i_d_query_id_number(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ��ȡ�������ݺ��롣
   * @par    ˵����
   * ��ȡ�ڲ�����Ĺ������ݺ��롣
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[out] id_number �������ݺ��룬�����ٷ���32���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_id_number_2(DEVHANDLE idDEVHANDLE, char *id_number);

  /**
   * @brief  ��ȡǩ�����ء�
   * @par    ˵����
   * ��ȡ�ڲ������ǩ�����ء�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���ص�ǩ�������ַ�����
   */
  char *USER_API dc_i_d_query_department(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ��ȡǩ�����ء�
   * @par    ˵����
   * ��ȡ�ڲ������ǩ�����ء�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[out] department ǩ�����أ������ٷ���32���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_department_2(DEVHANDLE idDEVHANDLE, char *department);

  /**
   * @brief  ��ȡ��Ч���ޡ�
   * @par    ˵����
   * ��ȡ�ڲ��������Ч���ޡ�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���ص���Ч�����ַ�����
   */
  char *USER_API dc_i_d_query_expire_day(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ��ȡ��Ч���ޡ�
   * @par    ˵����
   * ��ȡ�ڲ��������Ч���ޡ�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[out] expire_day ��Ч���ޣ������ٷ���32���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_expire_day_2(DEVHANDLE idDEVHANDLE, char *expire_day);

  /**
   * @brief  ��ȡ��Ƭԭʼ���ݡ�
   * @par    ˵����
   * ��ȡ�ڲ��������Ƭԭʼ���ݣ���������Ҫͨ����������Ƭ�����������������Ƭͼ��
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���ص���Ƭԭʼ���ݣ�����ʵ�ʳ���ͨ������ ::dc_i_d_query_photo_len ����ȡ��
   */
  unsigned char *USER_API dc_i_d_query_photo(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ��ȡ��Ƭԭʼ���ݡ�
   * @par    ˵����
   * ��ȡ�ڲ��������Ƭԭʼ���ݣ���������Ҫͨ����������Ƭ�����������������Ƭͼ��
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[out] photo ��Ƭԭʼ���ݣ������ٷ���4096���ֽڣ�����ʵ�ʳ���ͨ������ ::dc_i_d_query_photo_len ����ȡ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_photo_2(DEVHANDLE idDEVHANDLE, unsigned char *photo);

  /**
   * @brief  ��ȡ��Ƭԭʼ���ݳ��ȡ�
   * @par    ˵����
   * ��ȡ�ڲ��������Ƭԭʼ���ݵĳ��ȡ�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���ص���Ƭԭʼ���ݳ��ȡ�
   */
  unsigned int USER_API dc_i_d_query_photo_len(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ������Ƭͼ���ļ���
   * @par    ˵����
   * ʹ���ڲ��������Ƭԭʼ���ݣ�ͨ�����ù�������Ƭ��������������Ƭͼ���ļ���
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[in] FileName �ļ�������ȷ����д���Ȩ�ޡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_photo_file(DEVHANDLE idDEVHANDLE, const char *FileName);

  /**
   * @brief  ������Ƭͼ�����ݡ�
   * @par    ˵����
   * ʹ���ڲ��������Ƭԭʼ���ݣ�ͨ�����ù�������Ƭ��������������Ƭͼ�����ݡ�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[out] BmpBuffer ���ص���Ƭͼ�����ݣ������ٷ���65536���ֽڡ�
   * @param[in,out] BmpLength ���� @a BmpBuffer ������ֽ�����������Ƭͼ�����ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_photo_bmp_buffer(DEVHANDLE idDEVHANDLE, unsigned char *BmpBuffer, unsigned int *BmpLength);

  /**
   * @brief  ��ȡָ��ԭʼ���ݡ�
   * @par    ˵����
   * ��ȡ�ڲ������ָ��ԭʼ���ݡ�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���ص�ָ��ԭʼ���ݣ�����ʵ�ʳ���ͨ������ ::dc_i_d_query_finger_len ����ȡ��
   */
  unsigned char *USER_API dc_i_d_query_finger(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ��ȡָ��ԭʼ���ݡ�
   * @par    ˵����
   * ��ȡ�ڲ������ָ��ԭʼ���ݡ�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @param[out] finger ָ��ԭʼ���ݣ������ٷ���4096���ֽڣ�����ʵ�ʳ���ͨ������ ::dc_i_d_query_finger_len ����ȡ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_i_d_query_finger_2(DEVHANDLE idDEVHANDLE, unsigned char *finger);

  /**
   * @brief  ��ȡָ��ԭʼ���ݳ��ȡ�
   * @par    ˵����
   * ��ȡ�ڲ������ָ��ԭʼ���ݵĳ��ȡ�
   * @param[in] idDEVHANDLE ����֤��ʶ����
   * @return ���ص�ָ��ԭʼ���ݳ��ȡ�
   */
  unsigned int USER_API dc_i_d_query_finger_len(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ������
   */
  short USER_API dc_i_d_query_finger_file(DEVHANDLE idDEVHANDLE, const char *FileName);

  /**
   * @brief  ������
   */
  short USER_API dc_i_d_query_finger_bmp_buffer(DEVHANDLE idDEVHANDLE, unsigned char *BmpBuffer, unsigned int *BmpLength);

  /**
   * @brief  �ͷŶ�����֤��Դ��
   * @par    ˵����
   * �ͷ� ::dc_start_i_d ::dc_start_i_d_2 �������Դ��
   * @param[in] idDEVHANDLE ����֤��ʶ����
   */
  void USER_API dc_end_i_d(DEVHANDLE idDEVHANDLE);

  /**
   * @brief  ��ȡ����֤UID��
   * @par    ˵����
   * ��ȡ����֤��Ψһ��ʶ������Ϊ8���ֽڡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] uid ���ص�UID���̶�Ϊ8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_getuid_i_d(DEVHANDLE icdev, unsigned char *uid);

  /**
   * @brief  LCD��ʾ��ά�롣
   * @par    ˵����
   * ���豸LCD��ʾ������ʾ��ά�롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] point_x X��ƫ�Ƶ�����0~60����
   * @param[in] point_y Y��ƫ�Ƶ�����0~5����
   * @param[in] info �ı��ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_display_2dbarcode(DEVHANDLE icdev, int point_x, int point_y, const char *info);

  /**
   * @brief  ������ָ֤�ơ�
   * @par    ˵����
   * ��ȡ����֤�е�ָ�����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] length ����ָ�����ݵĳ��ȣ�����'\0'��
   * @param[out] data ���ص�ָ�����ݣ�HEX��ʽ�ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_getfingerdata(DEVHANDLE icdev, int *length, unsigned char *data);

  /**
   * @brief  ��⿨Ƭλ�á�
   * @par    ˵����
   * ��⿨Ƭ���豸�е�λ�á�
   * @param[in] icdev �豸��ʶ����
   * @param[out] pos λ�á�
   * @n 0x01 - ������ʼλ��
   * @n 0x02 - ����ĩλ��
   * @n 0x03 - ������ס��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿���
   */
  short USER_API dc_card_status2(DEVHANDLE icdev, unsigned char *pos);

  /**
   * @brief  ������������
   * @par    ˵����
   * ������������
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��0x00��ʾ������0x01��ʾ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿���
   */
  short USER_API dc_card_make_and_lock(DEVHANDLE icdev, unsigned char flag);

  /**
   * @brief  ���Ͷ��š�
   * @par    ˵����
   * ���Ͷ��Ÿ�ָ�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] phone_no ���롣
   * @param[in] sms_content �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_send_sms(DEVHANDLE icdev, const char *phone_no, const char *sms_content);

  /**
   * @brief  ָ��ģ��ͨѶ��
   * @par    ˵����
   * ��ָ��ģ�����ͨѶ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[in] request �������ݡ�
   * @param[in] request_len �������ݵĳ��ȡ�
   * @param[out] response ���ص����ݡ�
   * @param[out] response_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_fingerprint_io_control(DEVHANDLE icdev, unsigned char ctime, const unsigned char *request, int request_len, unsigned char *response, int *response_len);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_io_control(DEVHANDLE icdev, unsigned char ctime, const unsigned char *request, int request_len, unsigned char *response, int *response_len);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_ImportKey(DEVHANDLE icdev, unsigned char *check_value, unsigned char check_mode, int key_index, int use_key_index, unsigned char enable, const unsigned char *key, int key_len);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_DeleteKey(DEVHANDLE icdev, int key_index);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_SetButtonMark(DEVHANDLE icdev, unsigned int mark_value, unsigned char use);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_Start(DEVHANDLE icdev, unsigned char mode, int max_len, unsigned char auto_end, unsigned char enable_sound);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_End(DEVHANDLE icdev);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_GetPressed(DEVHANDLE icdev, int time_ms, unsigned char *value);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_GetPinBlock(DEVHANDLE icdev, unsigned char mode, int key_index, const unsigned char *customer_data, int customer_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_GetRandomValue(DEVHANDLE icdev, int length, unsigned char *value);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_GetEncryptDataOfCBC(DEVHANDLE icdev, unsigned char mode, int key_index, const unsigned char *in_data, int in_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_GetDecryptDataOfCBC(DEVHANDLE icdev, unsigned char mode, int key_index, const unsigned char *in_data, int in_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_GetEncryptDataOfECB(DEVHANDLE icdev, unsigned char mode, int key_index, const unsigned char *in_data, int in_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_GetDecryptDataOfECB(DEVHANDLE icdev, unsigned char mode, int key_index, const unsigned char *in_data, int in_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  ������
   */
  short USER_API dc_keypad_GetMACDataOfCBC(DEVHANDLE icdev, unsigned char mode, int key_index, const unsigned char *in_data, int in_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  ��ȡ�ǽӴ������͡�
   * @par    ˵����
   * ��ȡ��Ӧ���д��ڵķǽӴ������ͣ�ע�⣺�˽ӿ���Ҫ�ڿ��������ܷ�����ȷ�Ŀ����ͣ�����᷵��0x00��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܡ�
   * @n 0x00 - �޿���
   * @n 0x11 - Type A CPU Card��
   * @n 0x13 - Type A Mifare S50��
   * @n 0x14 - Type A Mifare S70��
   * @n 0x15 - Type A Mifare Ultralight��
   * @n 0x21 - Type B CPU Card��
   * @n 0x22 - Type B �洢����
   */
  short USER_API dc_RfGetCardType(DEVHANDLE icdev);

  /**
   * @brief  ������
   */
  short USER_API dc_RfWaveControl(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ������
   */
  short USER_API dc_RfReadChipRegister(DEVHANDLE icdev, unsigned char *register_data);

  /**
   * @brief  ������
   */
  short USER_API dc_RfSetChipRegister(DEVHANDLE icdev, unsigned char register_type, unsigned char register_value);

  /**
   * @brief  ��ȡ����֤UID��
   * @par    ˵����
   * ��ȡ����֤��Ψһ��ʶ������Ϊ8���ֽڡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] _Data ���ص�UID���̶�Ϊ8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_get_idsnr(DEVHANDLE icdev, unsigned char *_Data);

  /**
   * @brief  ��ȡ����֤UID��
   * @par    ˵����
   * ::dc_get_idsnr ��HEX��ʽ�ӿڣ����� @a _Data ΪHEX��ʽ��
   */
  short USER_API dc_get_idsnr_hex(DEVHANDLE icdev, char *_Data);

  /**
   * @brief  �������ģ��ͨѶ��
   * @par    ˵����
   * ���������ģ�����ͨѶ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] time_100ms �豸��ʱֵ����λΪ100���롣
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sdata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_PinpadCommunication(DEVHANDLE icdev, int time_100ms, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  ͸��ͨѶ��
   * @par    ˵����
   * ͸��ͨѶ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0x00��ʾP3���̣�0x01��ʾAndroid��
   * @param[in] time_100ms �豸��ʱֵ����λΪ100���롣
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sdata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_PassthroughCommunication(DEVHANDLE icdev, unsigned char type, int time_100ms, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  ��Flash��
   * @par    ˵����
   * ��ȡ�豸�ڲ�Flash�е����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�Ƶ�ַ��
   * @param[in] length ��ȡ���ȡ�
   * @param[out] data_buffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadFlash(DEVHANDLE icdev, int offset, int length, unsigned char *data_buffer);

  /**
   * @brief  дFlash��
   * @par    ˵����
   * д�����ݵ��豸�ڲ�Flash�У������������ݱ���ȡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�Ƶ�ַ��
   * @param[in] length д�볤�ȡ�
   * @param[in] data_buffer �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WriteFlash(DEVHANDLE icdev, int offset, int length, const unsigned char *data_buffer);

  /**
   * @brief  ��⿨λ��״̬��
   * @par    ˵����
   * ��⿨Ƭ��ǰ��λ��״̬��
   * @param[in] icdev �豸��ʶ����
   * @param[out] pos λ��״̬����Ϊ�������ֵ��λ����һ��
   * @n 0x00 - �޿���
   * @n 0x01 - �޿�������ǰ�ſڣ����п�λ�á�
   * @n 0x02 - �޿�������ǰ�ſڣ������п�λ�á�
   * @n 0x10 - �п������ɲ����κο���
   * @n 0x11 - �п����ɲ���������
   * @n 0x12 - �п����ɲ����Ӵ���
   * @n 0x14 - �п����ɲ����ǽӴ���
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceCardStatus(DEVHANDLE icdev, unsigned char *pos);

  /**
   * @brief  ���뿨Ƭ��
   * @par    ˵����
   * ���뿨Ƭ������ǰ�豸���п������
   * @param[in] icdev �豸��ʶ����
   * @param[in] time_s �豸��ʱֵ����λΪ�롣
   * @param[in] mode ģʽ��
   * @n 0x00 - ǰ�˽�������������������IC��
   * @n 0x01 - ǰ�˽�����������������IC��
   * @n 0x02 - ��˽���������������
   * @n 0x03 - ��˽�������������
   * @n 0x04 - ǰ�˽�����������������IC��
   * @n 0x05 - ǰ�˽���������������IC��
   * @return <0��ʾʧ�ܡ�
   * @n 0 - �����ɹ���
   * @n 1 - �豸�����п���
   * @n 2 - ���ճ�ʱ��
   * @n 3 - ���ſ�����
   * @n 4 - �������ó�����
   * @n 5 - �쳣������������
   * @n 6 - �쳣�������豸�ڡ�
   */
  short USER_API dc_SelfServiceDeviceCardInject(DEVHANDLE icdev, unsigned char time_s, unsigned char mode);

  /**
   * @brief  ������Ƭ��
   * @par    ˵����
   * ������Ƭ������ǰ�豸���޿������
   * @param[in] icdev �豸��ʶ����
   * @param[in] time_s �豸��ʱֵ����λΪ�롣
   * @param[in] mode ģʽ��
   * @n 0x00 - ����ǰ�˲��п���
   * @n 0x01 - ������ˡ�
   * @n 0x02 - ����ǰ�˲��п���
   * @return <0��ʾʧ�ܡ�
   * @n 0 - �����ɹ���
   * @n 1 - �豸��û�п���
   * @n 2 - ���ճ�ʱ��
   * @n 3 - �������ó�����
   * @n 4 - ��Ƭ�ѵ�ǰ�żп�λ�ã��ɲ�����Ƭ��
   * @n 5 - ��Ƭ�ѵ�ǰ�Ų��п�λ�ã����ɲ�����Ƭ��
   */
  short USER_API dc_SelfServiceDeviceCardEject(DEVHANDLE icdev, unsigned char time_s, unsigned char mode);

  /**
   * @brief  �ƶ���Ƭ��
   * @par    ˵����
   * �ƶ���Ƭ������ǰ�豸���޿������
   * @param[in] icdev �豸��ʶ����
   * @param[in] time_s �豸��ʱֵ����λΪ�롣
   * @param[in] mode ģʽ��
   * @n 0x00 - �ƶ�������������λ�á�
   * @n 0x01 - �ƶ����Ӵ�������λ�á�
   * @n 0x02 - �ƶ����ǽӴ�������λ�á�
   * @return <0��ʾʧ�ܡ�
   * @n 0 - �����ɹ���
   * @n 1 - �豸��û�п���
   * @n 2 - ���ճ�ʱ��
   * @n 3 - �������ó�����
   * @n 4 - ��Ƭ�ѵ�ǰ�Ų��п�λ�ã����ɲ�����Ƭ��
   */
  short USER_API dc_SelfServiceDeviceCardMove(DEVHANDLE icdev, unsigned char time_s, unsigned char mode);

  /**
   * @brief  ��ȡ������״̬��
   * @par    ˵����
   * ��ȡ�豸��������״̬��
   * @param[in] icdev �豸��ʶ����
   * @param[out] value ״ֵ̬��
   * @n bit0 - ��բ�ſ��ش�������0��ʾ�򿪣�1��ʾ�رա�
   * @n bit1 - ѹ����������0��ʾ���ѱ���ѹ��1��ʾ�޿���
   * @n bit2~bit7 - �ֱ��ʾ��ǰ�˵���˵Ĵ�������ÿ��������ֵ0��ʾ�п���ֵ1��ʾ�޿���
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�����޷�ʹ�á�
   */
  short USER_API dc_SelfServiceDeviceSensorStatus(DEVHANDLE icdev, unsigned char *value);

  /**
   * @brief  ���õ���ģʽ��
   * @par    ˵����
   * ���õ���ģʽ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��
   * @n 0x00 - ����ǰ�˲��п���
   * @n 0x01 - ������ˡ�
   * @n 0x02 - ����ǰ�˲��п���
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceConfig(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ����ǰ�˽���ģʽ��
   * @par    ˵����
   * ����ǰ�˽���ģʽ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��
   * @n 0x00 - ��ֹ�������޴��źţ��޿��أ���IC����
   * @n 0x01 - �������д��źţ��п��أ���IC����
   * @n 0x02 - �������޴��źţ��п��أ���IC����
   * @n 0x03 - �������д��źţ��޿��أ���IC����
   * @n 0x04 - �������޴��źţ��޿��أ���IC����
   * @n 0x05 - �������޴��źţ��п��أ���IC����
   * @n 0x06 - �������д��źţ��޿��أ���IC����
   * @n 0x07 - �������д��źţ��п��أ���IC����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceConfigFront(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ���ú�˽���ģʽ��
   * @par    ˵����
   * ���ú�˽���ģʽ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��
   * @n 0x00 - ��˽�ֹ������
   * @n 0x01 - ��˴ſ���ʽ������
   * @n 0x02 - ��˷Ǵſ���ʽ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceConfigBack(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ����ͣ��λ�á�
   * @par    ˵����
   * ����ͣ��λ�á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��
   * @n 0x00 - ǰ�˲��п���
   * @n 0x01 - ǰ�˼п���
   * @n 0x02 - �Ӵ�ʽIC��λ�á�
   * @n 0x03 - ��Ƶ��λ�á�
   * @n 0x04 - �ſ�λ�á�
   * @n 0x05 - ��������ˡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceConfigPlace(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ��⿨���͡�
   * @par    ˵����
   * �豸���Զ��ƶ���Ƭ���Ҽ�⿨Ƭ���͡�
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܡ�
   * @n 0x00 - ��ʾ�޷���⵽��Ӧ��Ƭ��
   * @n 0x01 - ��ʾ�豸���޿���
   * @n 0x11 - ��ʾType A CPU Card��
   * @n 0x12 - ��ʾType A �洢����
   * @n 0x13 - ��ʾType A Mifare S50��
   * @n 0x14 - ��ʾType A Mifare S70��
   * @n 0x15 - ��ʾType A Mifare Ultralight��
   * @n 0x16 - ��ʾType A Mifare MINI��
   * @n 0x18 - ��ʾType A Ntag 21x / Ultralight EV1��
   * @n 0x1F - ��ʾType A OTHERS MEM��
   * @n 0x21 - ��ʾType B CPU Card��
   * @n 0x22 - ��ʾType B �洢����
   * @n 0x31 - ��ʾ�Ӵ�T=0 CPU Card��
   * @n 0x32 - ��ʾ�Ӵ�T=1 CPU Card��
   * @n 0x41 - ��ʾ4442 Card��
   * @n 0x42 - ��ʾ4428 Card��
   */
  short USER_API dc_SelfServiceDeviceCheckCardType(DEVHANDLE icdev);

  /**
   * @brief  ��λ�����豸��
   * @par    ˵����
   * ʹ�����豸�����ϵ��ʼ״̬���������ò���Ϊȱʡ������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceReset(DEVHANDLE icdev);

  /**
   * @brief  д�������������á�
   * @par    ˵����
   * д�������������á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0��ʾд���칲�ƶ����Σ�1��ʾд���칲�ƶ�һ�Ρ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceWriteMagConfig(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ���������������á�
   * @par    ˵����
   * ���������������á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] track1 1���������
   * @n bit0 - �������������ã�0��ʾ�������1��ʾ�����
   * @n bit1 - �������ͷ���ã�0��ʾ������ͷ��1��ʾ������ͷ��
   * @n bit2 - �������β���ã�0��ʾ������β��1��ʾ������β��
   * @n bit3~bit7 - ������
   * @param[in] track2 2���������
   * @n bit0 - �������������ã�0��ʾ�������1��ʾ�����
   * @n bit1 - �������ͷ���ã�0��ʾ������ͷ��1��ʾ������ͷ��
   * @n bit2 - �������β���ã�0��ʾ������β��1��ʾ������β��
   * @n bit3~bit7 - ������
   * @param[in] track3 3���������
   * @n bit0 - �������������ã�0��ʾ�������1��ʾ�����
   * @n bit1 - �������ͷ���ã�0��ʾ������ͷ��1��ʾ������ͷ��
   * @n bit2 - �������β���ã�0��ʾ������β��1��ʾ������β��
   * @n bit3~bit7 - ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceReadMagConfig(DEVHANDLE icdev, unsigned char track1, unsigned char track2, unsigned char track3);

  /**
   * @brief  ����ģʽ���á�
   * @par    ˵����
   * ����ģʽ���á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0��ʾ����ģʽ��1��ʾ�Ƕ���ģʽ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceShakeConfig(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ǰ�ŵ�բ�����á�
   * @par    ˵����
   * ǰ�ŵ�բ�����á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0��ʾ����ǰ�ŵ�բ�ţ�1��ʾ������ǰ�ŵ�բ�š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceSetFrontSwitch(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ǰ�ŵ�բ�Ż�ȡ��
   * @par    ˵����
   * ǰ�ŵ�բ�Ż�ȡ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] mode ģʽ��0��ʾ����ǰ�ŵ�բ�ţ�1��ʾ������ǰ�ŵ�բ�š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceGetFrontSwitch(DEVHANDLE icdev, unsigned char *mode);

  /**
   * @brief  ����ָ���
   * @par    ˵����
   * ����ָ���
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0x00��ʾ�쳣��ǰ����������0x01��ʾ�쳣�������������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceRecoverError(DEVHANDLE icdev, unsigned char type);

  /**
   * @brief  ��ȡ������״̬��
   * @par    ˵����
   * ��ȡ�豸��������״̬��
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @n rdata[0] - ��բ�ſ��ش�������0x00��ʾ�򿪣�0x01��ʾ�رա�
   * @n rdata[1] - ѹ����������0x00��ʾ���ѱ���ѹ��0x01��ʾ�޿���
   * @n rdata[2] - �뿨��⣬0x00��ʾ�У�0x01��ʾ�ޡ�
   * @n rdata[3] - ��λ1��⣬0x00��ʾ�У�0x01��ʾ�ޡ�
   * @n rdata[4] - ��λ2��⣬0x00��ʾ�У�0x01��ʾ�ޡ�
   * @n rdata[5] - ��λ3��⣬0x00��ʾ�У�0x01��ʾ�ޡ�
   * @n rdata[6] - ��λ4��⣬0x00��ʾ�У�0x01��ʾ�ޡ�
   * @n rdata[7] - ��λ5��⣬0x00��ʾ�У�0x01��ʾ�ޡ�
   * @n rdata[8] - IC����⣬0x00��ʾ�У�0x01��ʾ�ޡ�
   * @n rdata[9] - ���������⣬0x00��ʾ��ѹ��0x01��ʾ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�����޷�ʹ�á�
   */
  short USER_API dc_SelfServiceDeviceSensorStatusEx(DEVHANDLE icdev, int *rlen, unsigned char *rdata);

  /**
   * @brief  ѹס���ͷſ�����
   * @par    ˵����
   * ѹס���ͷſ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0x00��ʾ�ſ���Ƭ��0x01��ʾѹס��Ƭ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceKeepUpDown(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  д��������
   * @par    ˵����
   * д�����ݵ��������С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] t1_data ����1������ݡ�
   * @param[in] t1_len 1������ݵĳ��ȣ�ע�⣺-1��ʾ��մ˹�����ݣ�0��ʾ����д�˹����
   * @param[in] t2_data ����2������ݡ�
   * @param[in] t2_len 2������ݵĳ��ȣ�ע�⣺-1��ʾ��մ˹�����ݣ�0��ʾ����д�˹����
   * @param[in] t3_data ����3������ݡ�
   * @param[in] t3_len 3������ݵĳ��ȣ�ע�⣺-1��ʾ��մ˹�����ݣ�0��ʾ����д�˹����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceWriteMag(DEVHANDLE icdev, const unsigned char *t1_data, int t1_len, const unsigned char *t2_data, int t2_len, const unsigned char *t3_data, int t3_len);

  /**
   * @brief  ���࿨״̬��
   * @par    ˵����
   * �ж��Ƿ�Ϊ���ſ����Ƿ�ΪType A��Type B����
   * @param[in] icdev �豸��ʶ����
   * @param[out] type �����ͣ�0x0A��ʾType A����0x0B��ʾType B����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�޿���==2��ʾ���ڶ��ſ���
   */
  short USER_API dc_MulticardStatus(DEVHANDLE icdev, unsigned char *type);

  /**
   * @brief  ��ȡ�������ֵ��
   * @par    ˵����
   * ��ȡ�豸���̵İ���ֵ��ÿ�ε��ÿ��Ի�ȡ�������ֵ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[out] rlen ���ذ������ݵĳ��ȣ�����'\0'��
   * @param[out] cpass ���صİ������ݣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==0xA1��ʾȡ�����룬==0xA2��ʾ���볬ʱ��
   */
  short USER_API dc_pass_key_value2(DEVHANDLE icdev, unsigned char ctime, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  ���ⲿEEPROM��
   * @par    ˵����
   * ��ȡ�豸�ⲿEEPROM�е����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�Ƶ�ַ��
   * @param[in] length ��ȡ���ȡ�
   * @param[out] data_buffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadOutEeprom(DEVHANDLE icdev, int offset, int length, unsigned char *data_buffer);

  /**
   * @brief  д�ⲿEEPROM��
   * @par    ˵����
   * д�����ݵ��豸�ⲿEEPROM�У������������ݱ���ȡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�Ƶ�ַ��
   * @param[in] length д�볤�ȡ�
   * @param[in] data_buffer �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WriteOutEeprom(DEVHANDLE icdev, int offset, int length, const unsigned char *data_buffer);

  /**
   * @brief  ��ID����
   * @par    ˵����
   * ��ȡID�����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] time_ms �豸��ʱֵ����λΪ���롣
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadIdCardInfo(DEVHANDLE icdev, int time_ms, int *rlen, unsigned char *rdata);

  /**
   * @brief  ������
   */
  short USER_API dc_cpy_getkey(DEVHANDLE icdev, unsigned char *respkey, int *resplen);

  /**
   * @brief  ������
   */
  short USER_API dc_cpy_setkey(DEVHANDLE icdev, unsigned char *genkey, int keylen);

  /**
   * @brief  ������
   */
  short USER_API dc_cpy_cpuapdu(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_cpy_cpuapdu_hex(DEVHANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_cpy_cpuapduInt(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_cpy_cpuapduInt_hex(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  ������
   */
  short USER_API dc_cpy_procommand(DEVHANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  ������
   */
  short USER_API dc_cpy_procommand_hex(DEVHANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  ������
   */
  short USER_API dc_cpy_procommandInt(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  ������
   */
  short USER_API dc_cpy_procommandInt_hex(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ը�Ӧ��CPU������ָ���������ע��˽ӿ��ѷ�װ��Э�鲿�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @param[in] timeout ��ʱֵ����ֵֻ�ڲ����豸�ĵײ�ʹ�ã���λΪ250���룬һ����ý���ֵΪ7��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_procommandInt(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  �ǽӴ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_procommandInt ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_procommandInt_hex(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  ������
   */
  short USER_API dc_ReadRfCfgReg(DEVHANDLE icdev, int type, int number, int *value);

  /**
   * @brief  ������
   */
  short USER_API dc_WriteRfCfgReg(DEVHANDLE icdev, int type, int number, int value);

  /**
   * @brief  ��ʼ��NFC��
   * @par    ˵����
   * �л���Ƶ��NFC��Ե�ģʽ������NFC��Ե�ģʽ֮ǰ����ʹ�ô˽ӿڽ��г�ʼ����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_NfcInit(DEVHANDLE icdev);

  /**
   * @brief  ����NFC��
   * @par    ˵����
   * ѰNFC��������NFC���Զ����ӣ�������ATR���ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] atr ���ص����ݡ�
   * @param[out] atr_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_NfcActivate(DEVHANDLE icdev, unsigned char *atr, int *atr_len);

  /**
   * @brief  �Ͽ�NFC��
   * @par    ˵����
   * �Ͽ����Ӳ��ر����ߣ����ô˽ӿڱ����ȼ���NFC��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_NfcDeactivate(DEVHANDLE icdev);

  /**
   * @brief  NFC��Ե�����͸�������ͷ���ˣ���
   * @par    ˵����
   * �������ݵ�NFC�ֻ������ô˽ӿڱ����ȼ���NFC��
   * @param[in] icdev �豸��ʶ����
   * @param[in] request �������ݡ�
   * @param[in] request_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_NfcPush(DEVHANDLE icdev, const unsigned char *request, int request_len);

  /**
   * @brief  NFC��Ե�����͸�����ӷ���˽��գ���
   * @par    ˵����
   * ���ý���ģʽ�������󽫽���׼������NFC�ֻ����ݵ�ģʽ�����ô˽ӿ����뼤��NFC��
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��0��ʾ�رգ�1��ʾ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_NfcService(DEVHANDLE icdev, int flag);

  /**
   * @brief  ����NFC�ֻ����ݡ�
   * @par    ˵����
   * �豸�������ģʽ�󣬿���ʹ�ô˽ӿ�������NFC�ֻ������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] response ���ص����ݡ�
   * @param[out] response_len �������ݵĳ��ȡ�
   * @param[in] time_ms �ӿڳ�ʱֵ����λΪ�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�ֻ����ڸ�Ӧ����==2��ʾ������Ҫ���´��ݡ�
   */
  short USER_API dc_NfcFetch(DEVHANDLE icdev, unsigned char *response, int *response_len, int time_ms);

  /**
   * @brief  ����NFC�ֻ����ݡ�
   * @par    ˵����
   * �豸�������ģʽ�󣬿���ʹ�ô˽ӿ�������NFC�ֻ������ݣ�CCID���豸ʹ�ã���
   * @param[in] icdev �豸��ʶ����
   * @param[in] time_ms �ӿڳ�ʱֵ����λΪ�롣
   * @param[out] flag ��־���̶�Ϊ1���ֽڣ�0x00��ʾ�м�֡��0x01��ʾ��ʼ֡��0x02��ʾ����֡��0x03��ʾ��ʼ����֡��
   * @param[out] response ���ص����ݡ�
   * @param[out] response_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==1��ʾ�ֻ����ڸ�Ӧ����==2��ʾ������Ҫ���´��ݡ�
   */
  short USER_API dc_NfcDataReceiver(DEVHANDLE icdev, int time_ms, unsigned char *flag, unsigned char *response, int *response_len);

  /**
   * @brief  NFC��Ե�����͸�����ӷ���˽��գ���
   * @par    ˵����
   * ���ý���ģʽ�������󽫽���׼������NFC�ֻ����ݵ�ģʽ�����ô˽ӿ����뼤��NFC��
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0��ʾ�رգ�1��ʾ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_NfcSimulateCardConfig(DEVHANDLE icdev, int mode);

  /**
   * @brief  �����������Զ��ϴ�����ģʽ���á�
   * @par    ˵����
   * �����������Զ��ϴ�����ģʽ���á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��
   * @n 0 - ����������������ĸ����ĸת��Ϊ��д��ĸ��
   * @n 1 - ���������ԭʼ���ݣ�Ĭ�ϣ���
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MagFormatConfig(DEVHANDLE icdev, int mode);

  /**
   * @brief  ��ȡͨѶģ��MAC��ַ��
   * @par    ˵����
   * ��ȡͨѶģ��MAC��ַ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ģ�����ͣ�1��ʾ����ģ�飬2��ʾWIFIģ�飬3��ʾGPRS/CDMA/ȫ��ͨ��ģ�顣
   * @param[out] value ���ص�MAC��ַ�����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadModuleMacAddress(DEVHANDLE icdev, int type, char *value);

  /**
   * @brief  ����ͨѶģ��MAC��ַ��
   * @par    ˵����
   * ����ͨѶģ��MAC��ַ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ģ�����ͣ�1��ʾ����ģ�飬2��ʾWIFIģ�飬3��ʾGPRS/CDMA/ȫ��ͨ��ģ�顣
   * @param[in] value ����MAC��ַ�����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WriteModuleMacAddress(DEVHANDLE icdev, int type, const char *value);

  /**
   * @brief  ��ȡATS���ݡ�
   * @par    ˵����
   * ��ȡType A��Type B����ATS���˽ӿڱ����ڿ�Ƭ�����ſ�ʹ�á�
   * @param[in] icdev �豸��ʶ����
   * @param[out] ats_len ����ATS���ݵĳ��ȡ�
   * @param[out] ats ���ص�ATS���ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadRfTypeABATS(DEVHANDLE icdev, int *ats_len, unsigned char *ats);

  /**
   * @brief  ��ȡATS���ݡ�
   * @par    ˵����
   * ::dc_ReadRfTypeABATS ��HEX��ʽ�ӿڣ����� @a ats ΪHEX��ʽ��
   */
  short USER_API dc_ReadRfTypeABATS_hex(DEVHANDLE icdev, int *ats_len, char *ats);

  /**
   * @brief  ������
   */
  short USER_API dc_RfFormatConfig(DEVHANDLE icdev, int mode, int block_number, int offset, int length);

  /**
   * @brief  Flash����������
   * @par    ˵����
   * ����Flash�����е����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] sector ������ַ��0xFFFF��ʾ����ȫ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_FlashErase(DEVHANDLE icdev, int sector);

  /**
   * @brief  �豸������
   * @par    ˵����
   * �豸����������ָ���������з������ӿڱ�����ֱ�ӷ��أ�������ȴ�������������ɡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] beep_time_ms ���η���ʱ�䣬��λΪ���롣
   * @param[in] interval_time_ms ���ʱ�䣬��λΪ���롣
   * @param[in] count ��������������0x0000��ʾֹͣ��0xFFFF��ʾ���޴Ρ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BuzzerBeep(DEVHANDLE icdev, unsigned short beep_time_ms, unsigned short interval_time_ms, unsigned short count);

  /**
   * @brief  ����ģʽ���á�
   * @par    ˵����
   * ��������ģ���ͨѶģʽ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0x00��ʾ����ģʽ��ATָ���EEPROM��д����0x01��ʾ����ģʽ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtModeControl(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ���������ʲ�ѯ/���á�
   * @par    ˵����
   * ��ѯ/��������ģ���ͨѶ�����ʣ��޸ĵ�������ģ��ˡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��0x00��ʾ��ѯ��0x01��ʾ���á�
   * @param[in,out] value �����ʡ�
   * @n 0x00 - 9600��
   * @n 0x01 - 19200��
   * @n 0x02 - 38400��
   * @n 0x03 - 57600��
   * @n 0x04 - 115200��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtBaudrateControl(DEVHANDLE icdev, unsigned char flag, unsigned char *value);

  /**
   * @brief  �������������ѯ/���á�
   * @par    ˵����
   * ��ѯ/��������ģ����������롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��0x00��ʾ��ѯ��0x01��ʾ���á�
   * @param[in,out] value �����ַ�������ѯʱ�����ٷ���64���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtPasswordControl(DEVHANDLE icdev, unsigned char flag, char *value);

  /**
   * @brief  ����ģ���ַ��ѯ/���á�
   * @par    ˵����
   * ��ѯ/��������ģ��ĵ�ַ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��0x00��ʾ��ѯ��0x01��ʾ���á�
   * @param[in,out] length ��ַ�ĳ��ȡ�
   * @param[in,out] value ��ַ����ѯʱ�����ٷ���64���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtAddressControl(DEVHANDLE icdev, unsigned char flag, unsigned char *length, unsigned char *value);

  /**
   * @brief  ����ģ�����Ʋ�ѯ/���á�
   * @par    ˵����
   * ��ѯ/��������ģ������ơ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��0x00��ʾ��ѯ��0x01��ʾ���á�
   * @param[in,out] value �����ַ�������ѯʱ�����ٷ���64���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtNameControl(DEVHANDLE icdev, unsigned char flag, char *value);

  /**
   * @brief  �����û��Զ�����������
   * @par    ˵����
   * ���ö�дEEPROM��ʽ��������ģ����Զ������á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��0x00��ʾ��EEPROM��0x01��ʾдEEPROM��ATָ�ģʽ��
   * @param[in] offset ƫ�Ƶ�ַ��
   * @param[in] length ���ݵĳ��ȡ�
   * @param[in,out] value ���ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtUserControl(DEVHANDLE icdev, unsigned char flag, int offset, int length, unsigned char *value);

  /**
   * @brief  ����������λ��
   * @par    ˵����
   * ������ģ�����������λ��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtReset(DEVHANDLE icdev);

  /**
   * @brief  ����ģ��ͨѶ���ԡ�
   * @par    ˵����
   * ����ģ��������ͨѶģʽ�£��ô˽ӿڿ��Բ�������ͨѶ�Ƿ�ͨ�������ݷ��ͳɹ��󣬼����ش����Ƿ��н��յ����������ж�ͨѶ�Ƿ�ɹ���
   * @param[in] icdev �豸��ʶ����
   * @param[in] length �������ݵĳ��ȡ�
   * @param[in] value �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtCommunicationTest(DEVHANDLE icdev, int length, const unsigned char *value);

  /**
   * @brief  ���������ʲ�ѯ/���á�
   * @par    ˵����
   * ��ѯ/����������ģ��֮���ͨѶ�����ʣ��޸ĵ����豸�ˡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��0x00��ʾ��ѯ��0x01��ʾ���á�
   * @param[in,out] value �����ʡ�
   * @n 0x00 - 2400��
   * @n 0x01 - 4800��
   * @n 0x02 - 9600��
   * @n 0x03 - 14400��
   * @n 0x04 - 38400��
   * @n 0x05 - 57600��
   * @n 0x06 - 115200��
   * @n 0x07 - 256000��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtWithReaderBaudrateControl(DEVHANDLE icdev, unsigned char flag, unsigned char *value);

  /**
   * @brief  ����ģ��ͨѶ��
   * @par    ˵����
   * ������ģ�����ͨѶ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sdata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtCommand(DEVHANDLE icdev, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  ��ȡ�����ź�ǿ�ȡ�
   * @par    ˵����
   * ��ȡ�����ź�ǿ�ȡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] state ״̬��0x00��ʾ����δ���ӣ�0x01��ʾ���������ӡ�
   * @param[out] value �ź�ֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtSignalStrength(DEVHANDLE icdev, unsigned char *state, short *value);

  /**
   * @brief  ��ȡ������Ӧ��
   * @par    ˵����
   * ��ȡ����ģ�鵱ǰִ���������Ӧ�����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] length �������ݵĳ��ȡ�
   * @param[out] value ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtDataResponse(DEVHANDLE icdev, int *length, unsigned char *value);

  /**
   * @brief  ��������͸����
   * @par    ˵����
   * �˽ӿ����ڽ����������յ�������͸���������У������ش�������յ����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sdata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_BtDataTransmit(DEVHANDLE icdev, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  SAM_Aģ��ָ�����
   * @par    ˵����
   * �˽ӿ����ڲ�����������֤��֤��ȫ����ģ�顣
   * @param[in] icdev �豸��ʶ����
   * @param[in] code ָ����롣
   * @n 0x10FF - ��λSAM_A���޷������ݣ��޷������ݡ�
   * @n 0x11FF - SAM_A״̬��⣬�޷������ݣ��޷������ݡ�
   * @n 0x12FF - ��SAM_A������Ϣ���޷������ݣ���������Ϊ16���ֽڵ�SAM_A��š�
   * @n 0x2001 - Ѱ�Ҿ�������֤���޷������ݣ���������Ϊ4���ֽڵ�0x00��
   * @n 0x2002 - ѡȡ��������֤���޷������ݣ���������Ϊ8���ֽڵ�0x00��
   * @n 0x3001 - ������������Ϣ����Ƭ��Ϣ���޷������ݣ��������ݸ�ʽΪ����2���ֽ�������Ϣ���ȣ�Big-Endian������2���ֽ���Ƭ��Ϣ���ȣ�Big-Endian������������Ϣ��������256���ֽڣ�������Ƭ��Ϣ��������1024���ֽڣ�����
   * @n 0x3010 - ������������Ϣ����Ƭ��Ϣ��ָ����Ϣ���޷������ݣ��������ݸ�ʽΪ����2���ֽ�������Ϣ���ȣ�Big-Endian������2���ֽ���Ƭ��Ϣ���ȣ�Big-Endian������2���ֽ�ָ����Ϣ���ȣ�Big-Endian������������Ϣ��������256���ֽڣ�������Ƭ��Ϣ��������1024���ֽڣ�����ָ����Ϣ��������1024���ֽڣ�����
   * @n 0x3003 - ��׷��סַ��Ϣ���޷������ݣ���������Ϊ70���ֽڵ�׷��סַ��Ϣ��
   * @n 0x6000 - ����UART�ӿ����ʣ�����Ϊ115200bps���޷������ݣ��޷������ݡ�
   * @n 0x6001 - ����UART�ӿ����ʣ�����Ϊ57600bps���޷������ݣ��޷������ݡ�
   * @n 0x6002 - ����UART�ӿ����ʣ�����Ϊ38400bps���޷������ݣ��޷������ݡ�
   * @n 0x6003 - ����UART�ӿ����ʣ�����Ϊ19200bps���޷������ݣ��޷������ݡ�
   * @n 0x6004 - ����UART�ӿ����ʣ�����Ϊ9600bps���޷������ݣ��޷������ݡ�
   * @n 0x61FF - ����SAM_A����Ƶģ��һ֡ͨ�����ݵ�����ֽ�������������Ϊ1���ֽڵ�����ֽ�����ȡֵ����Ϊ0x18~0xFF��Ĭ��ֵΪ0x58���޷������ݡ�
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sdata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݣ������ٷ���4096���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SamAControl(DEVHANDLE icdev, int code, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  ������֤��
   * @par    ˵����
   * ��ȡ����֤��ԭʼ��Ϣ���ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���͡�
   * @n 1 - ��ȡ������Ϣ����Ƭ��Ϣ��ָ����Ϣ��
   * @n 2 - ��ȡ׷��סַ��Ϣ��
   * @n 3 - ��ȡ������Ϣ����Ƭ��Ϣ��ָ����Ϣ��׷��סַ��Ϣ��
   * @param[out] text_len ����������Ϣ�ĳ��ȡ�
   * @param[out] text ���ص�������Ϣ�������ٷ���256���ֽڡ�
   * @param[out] photo_len ������Ƭ��Ϣ�ĳ��ȡ�
   * @param[out] photo ���ص���Ƭ��Ϣ�������ٷ���1024���ֽڡ�
   * @param[out] fingerprint_len ����ָ����Ϣ�ĳ��ȡ�
   * @param[out] fingerprint ���ص�ָ����Ϣ�������ٷ���1024���ֽڡ�
   * @param[out] extra_len ����׷��סַ��Ϣ�ĳ��ȡ�
   * @param[out] extra ���ص�׷��סַ��Ϣ�������ٷ���70���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SamAReadCardInfo(DEVHANDLE icdev, int type, int *text_len, unsigned char *text, int *photo_len, unsigned char *photo, int *fingerprint_len, unsigned char *fingerprint, int *extra_len, unsigned char *extra);

  /**
   * @brief  ����������Ϣ��
   * @par    ˵����
   * �����й��˾�������֤������Ϣ����ȡ��Ӧ����Ŀ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] charset ��ȡ��Ŀ�����õ��ַ�����0��ʾGBK��1��ʾUCS-2LE��2��ʾUTF-8��
   * @param[in] info_len ������Ϣ�ĳ��ȡ�
   * @param[in] info ������Ϣ��
   * @param[out] name �����������ٷ���64���ֽڡ�
   * @param[out] sex �Ա������ٷ���8���ֽڡ�
   * @param[out] nation ���壬�����ٷ���12���ֽڡ�
   * @param[out] birth_day �������ڣ������ٷ���36���ֽڡ�
   * @param[out] address סַ�������ٷ���144���ֽڡ�
   * @param[out] id_number �������ݺ��룬�����ٷ���76���ֽڡ�
   * @param[out] department ǩ�����أ������ٷ���64���ֽڡ�
   * @param[out] expire_start_day ֤��ǩ�����ڣ������ٷ���36���ֽڡ�
   * @param[out] expire_end_day ֤����ֹ���ڣ������ٷ���36���ֽڡ�
   * @param[out] reserved Ԥ��������ٷ���76���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ParseTextInfo(DEVHANDLE icdev, int charset, int info_len, const unsigned char *info, unsigned char *name, unsigned char *sex, unsigned char *nation, unsigned char *birth_day, unsigned char *address, unsigned char *id_number, unsigned char *department, unsigned char *expire_start_day, unsigned char *expire_end_day, unsigned char *reserved);

  /**
   * @brief  ����������Ϣ��
   * @par    ˵����
   * ������������þ���֤������Ϣ����ȡ��Ӧ����Ŀ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] charset ��ȡ��Ŀ�����õ��ַ�����0��ʾGBK��1��ʾUCS-2LE��2��ʾUTF-8��
   * @param[in] info_len ������Ϣ�ĳ��ȡ�
   * @param[in] info ������Ϣ��
   * @param[out] english_name Ӣ�������������ٷ���244���ֽڡ�
   * @param[out] sex �Ա������ٷ���8���ֽڡ�
   * @param[out] id_number ���þ���֤���룬�����ٷ���64���ֽڡ�
   * @param[out] citizenship ���������ڵ������룬�����ٷ���16���ֽڡ�
   * @param[out] chinese_name ���������������ٷ���64���ֽڡ�
   * @param[out] expire_start_day ֤��ǩ�����ڣ������ٷ���36���ֽڡ�
   * @param[out] expire_end_day ֤����ֹ���ڣ������ٷ���36���ֽڡ�
   * @param[out] birth_day �������ڣ������ٷ���36���ֽڡ�
   * @param[out] version_number ֤���汾�ţ������ٷ���12���ֽڡ�
   * @param[out] department_code ���������������ش��룬�����ٷ���20���ֽڡ�
   * @param[out] type_sign ֤�����ͱ�ʶ�������ٷ���8���ֽڡ�
   * @param[out] reserved Ԥ��������ٷ���16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ParseTextInfoForForeigner(DEVHANDLE icdev, int charset, int info_len, const unsigned char *info, unsigned char *english_name, unsigned char *sex, unsigned char *id_number, unsigned char *citizenship, unsigned char *chinese_name, unsigned char *expire_start_day, unsigned char *expire_end_day, unsigned char *birth_day, unsigned char *version_number, unsigned char *department_code, unsigned char *type_sign, unsigned char *reserved);

  /**
   * @brief  ����������Ϣ��
   * @par    ˵����
   * �����۰�̨�����ס֤������Ϣ����ȡ��Ӧ����Ŀ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] charset ��ȡ��Ŀ�����õ��ַ�����0��ʾGBK��1��ʾUCS-2LE��2��ʾUTF-8��
   * @param[in] info_len ������Ϣ�ĳ��ȡ�
   * @param[in] info ������Ϣ��
   * @param[out] name �����������ٷ���64���ֽڡ�
   * @param[out] sex �Ա������ٷ���8���ֽڡ�
   * @param[out] reserved Ԥ��������ٷ���12���ֽڡ�
   * @param[out] birth_day �������ڣ������ٷ���36���ֽڡ�
   * @param[out] address סַ�������ٷ���144���ֽڡ�
   * @param[out] id_number �������ݺ��룬�����ٷ���76���ֽڡ�
   * @param[out] department ǩ�����أ������ٷ���64���ֽڡ�
   * @param[out] expire_start_day ֤��ǩ�����ڣ������ٷ���36���ֽڡ�
   * @param[out] expire_end_day ֤����ֹ���ڣ������ٷ���36���ֽڡ�
   * @param[out] pass_number ͨ��֤���룬�����ٷ���40���ֽڡ�
   * @param[out] sign_count ǩ�������������ٷ���12���ֽڡ�
   * @param[out] reserved2 Ԥ��������ٷ���16���ֽڡ�
   * @param[out] type_sign ֤�����ͱ�ʶ�������ٷ���8���ֽڡ�
   * @param[out] reserved3 Ԥ��������ٷ���16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ParseTextInfoForHkMoTw(DEVHANDLE icdev, int charset, int info_len, const unsigned char *info, unsigned char *name, unsigned char *sex, unsigned char *reserved, unsigned char *birth_day, unsigned char *address, unsigned char *id_number, unsigned char *department, unsigned char *expire_start_day, unsigned char *expire_end_day, unsigned char *pass_number, unsigned char *sign_count, unsigned char *reserved2, unsigned char *type_sign, unsigned char *reserved3);

  /**
   * @brief  ������Ƭ��Ϣ��
   * @par    ˵����
   * ������Ƭ��Ϣ��ͨ����������Ƭ����⻹ԭ��Ƭͼ�����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ��Ƭͼ�����ݵĸ�ʽ��0��ʾBMP�ļ���1��ʾBMP���棬2��ʾBMP Base64�ַ�����
   * @param[in] info_len ��Ƭ��Ϣ�ĳ��ȡ�
   * @param[in] info ��Ƭ��Ϣ��
   * @param[in,out] photo_len ���ݵĳ��ȡ�
   * @n BMP�ļ� - ��Ч��
   * @n BMP���� - ���� @a photo ������ֽ�����������Ƭͼ�����ݵĳ��ȡ�
   * @n BMP Base64�ַ��� - ���� @a photo ������ֽ�����������Ƭͼ������Base64�ַ����ĳ��ȣ�����'\0'��
   * @param[in,out] photo ���ݡ�
   * @n BMP�ļ� - �����ļ�������ȷ����д���Ȩ�ޡ�
   * @n BMP���� - ���ص���Ƭͼ�����ݣ������ٷ���65536���ֽڡ�
   * @n BMP Base64�ַ��� - ���ص���Ƭͼ������Base64�ַ����������ٷ���65536���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ParsePhotoInfo(DEVHANDLE icdev, int type, int info_len, const unsigned char *info, int *photo_len, unsigned char *photo);

  /**
   * @brief  ����������Ϣ��
   * @par    ˵����
   * ����������Ϣ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��
   * @n ����Ϣ����GBK����ʱ��ʹ��0x00��ʾ�Ա�0x01��ʾ���壬0x02��ʾ������
   * @n ����Ϣ����UCS-2LE����ʱ��ʹ��0x10��ʾ�Ա�0x11��ʾ���壬0x12��ʾ������
   * @n ����Ϣ����UTF-8����ʱ��ʹ��0x20��ʾ�Ա�0x21��ʾ���壬0x22��ʾ������
   * @param[in] in_info ������Ϣ��
   * @param[out] out_info ���ص���Ϣ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ParseOtherInfo(DEVHANDLE icdev, int flag, const unsigned char *in_info, unsigned char *out_info);

  /**
   * @brief  ���Ӵ�ʽCPU����
   * @par    ˵����
   * ��⵱ǰ������CPU����״̬��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܡ�
   * @n 0 - ״̬δ֪��
   * @n 1 - �޿���
   * @n 2 - �п�����������δ֪��
   * @n 3 - �п�����δ�ϵ硣
   * @n 4 - �п��������ϵ硣
   */
  short USER_API dc_CpuCardStatus(DEVHANDLE icdev);

  /**
   * @brief  �����豸��
   * @par    ˵����
   * �����豸���˽ӿ��������豸�����������󣬳ɹ����ʾ�豸�Ѿ����ѡ�
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WakeDevice(DEVHANDLE icdev);

  /**
   * @brief  �����豸��
   * @par    ˵����
   * �����豸���˽ӿ��������豸������������ִ�гɹ�������ʾ�ɹ������������󣬲�����ʾ�豸�Ѿ���������״̬��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_RestDevice(DEVHANDLE icdev);

  /**
   * @brief  ��ȡ��ص�����
   * @par    ˵����
   * ��ȡ��ص�����
   * @param[in] icdev �豸��ʶ����
   * @param[out] value ��ذٷֱȣ�0x00~0x64����0x00��ʾ0%��0x64��ʾ100%��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetBatteryValue(DEVHANDLE icdev, unsigned char *value);

  /**
   * @brief  ������ͻ��
   * @par    ˵����
   * ֧��ISO 14443 Type A���Ϳ�Ƭ�ķ�����ͻ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0x93��ʾһ����0x95��ʾ������0x97��ʾ������
   * @param[in] _Bcnt �������̶�Ϊ0x00��
   * @param[out] _Snr ���صĿ����кš�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_anticoll_mode(DEVHANDLE icdev, unsigned char mode, unsigned char _Bcnt, unsigned int *_Snr);

  /**
   * @brief  ѡ��������
   * @par    ˵����
   * ͨ��ָ�����кţ�ѡȡ��Ӧ�Ŀ�Ƭ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0x93��ʾһ����0x95��ʾ������0x97��ʾ������
   * @param[in] _Snr ��Ƭ���кš�
   * @param[out] _Size ���ص�SAKֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_select_mode(DEVHANDLE icdev, unsigned char mode, unsigned int _Snr, unsigned char *_Size);

  /**
   * @brief  ���Ƭ��
   * @par    ˵����
   * ֧��ISO 14443 Type B���Ϳ�Ƭ�ļ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] PUPI αΨһPICC��ʶ����
   * @param[in] CID �ŵ��š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_attribInt(DEVHANDLE icdev, unsigned char *PUPI, unsigned char CID);

  /**
   * @brief  ���ϵ硣
   * @par    ˵����
   * ��ָ�������Ŀ�Ƭ�����ϵ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] icc_slot_no ������ţ��û���Ϊ0x0n��SAM��Ϊ0x1n�����С�n����ȡֵ��ΧΪ1~F��
   * @param[out] response ���صĸ�λ��Ϣ�������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�����Ϊ���ظ�λ��Ϣ�ĳ��ȡ�
   * @n -1 - ��Ƭ���Ͳ��ԡ�
   * @n -2 - �޿���
   * @n -3 - �п�δ�ϵ硣
   * @n -4 - ��Ƭ��Ӧ��
   */
  short USER_API dc_rf_poweron(DEVHANDLE icdev, unsigned char icc_slot_no, unsigned char *response);

  /**
   * @brief  ���µ硣
   * @par    ˵����
   * ��ָ�������Ŀ�Ƭ�����µ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] icc_slot_no ������ţ��û���Ϊ0x0n��SAM��Ϊ0x1n�����С�n����ȡֵ��ΧΪ1~F��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   * @n -1 - ��Ƭ���Ͳ��ԡ�
   * @n -2 - �޿���
   * @n -3 - �п�δ�ϵ硣
   * @n -4 - ��Ƭ��Ӧ��
   */
  short USER_API dc_rf_poweroff(DEVHANDLE icdev, unsigned char icc_slot_no);

  /**
   * @brief  ��ȡ��״̬��
   * @par    ˵����
   * ��ȡָ�������Ŀ�Ƭ״̬��
   * @param[in] icdev �豸��ʶ����
   * @param[in] icc_slot_no ������ţ��û���Ϊ0x0n��SAM��Ϊ0x1n�����С�n����ȡֵ��ΧΪ1~F��
   * @return <0��ʾʧ�ܣ�==0��ʾ�п������ϵ硣
   * @n -1 - ��Ƭ���Ͳ��ԡ�
   * @n -2 - �޿���
   * @n -3 - �п�δ�ϵ硣
   * @n -4 - ��Ƭ��Ӧ��
   */
  short USER_API dc_rf_getstatus(DEVHANDLE icdev, unsigned char icc_slot_no);

  /**
   * @brief  ��ָ�����
   * @par    ˵����
   * ��ָ�������Ŀ�Ƭ����ָ���������
   * @param[in] icdev �豸��ʶ����
   * @param[in] icc_slot_no ������ţ��û���Ϊ0x0n��SAM��Ϊ0x1n�����С�n����ȡֵ��ΧΪ1~F��
   * @param[in] length_of_command_apdu �������ݵĳ��ȡ�
   * @param[in] command_apdu �������ݡ�
   * @param[out] response_apdu ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�����Ϊ�������ݵĳ��ȡ�
   * @n -1 - ��Ƭ���Ͳ��ԡ�
   * @n -2 - �޿���
   * @n -3 - �п�δ�ϵ硣
   * @n -4 - ��Ƭ��Ӧ��
   */
  short USER_API dc_rf_application(DEVHANDLE icdev, unsigned char icc_slot_no, int length_of_command_apdu, unsigned char *command_apdu, unsigned char *response_apdu);

  /**
   * @brief  ��ȡ����Type A��UID��
   * @par    ˵����
   * ��ȡ����Type A��UID��
   * @param[in] icdev �豸��ʶ����
   * @param[out] uid_data ���ص����ݣ������ٷ���256���ֽڡ���ʽΪ����1�ֽ�UID��Ŀ��������1�ֽ�UID���ȡ���UID����1�ֽ�UID2���ȡ���UID2��...��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_RequestMultiCard(DEVHANDLE icdev, unsigned char *uid_data);

  /**
   * @brief  ѡ��һ��Type A�������
   * @par    ˵����
   * ѡ��һ��Type A�������
   * @param[in] icdev �豸��ʶ����
   * @param[in] uid_len ����UID�ĳ��ȡ�
   * @param[in] uid ����UID��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ActivateCard(DEVHANDLE icdev, unsigned char uid_len, const unsigned char *uid);

  /**
   * @brief  ��ȡ�����Ӧ�롣
   * @par    ˵����
   * ��ȡ���һ�κ��豸����ʱ�豸���ص���Ӧ״̬���룬�˽ӿڻ�ȡ����Ӧ�������豸�ڲ��̼������ʵ�֣���ͬ���豸�����в�һ�µ���Ӧ�롣
   * @param[in] icdev �豸��ʶ����
   * @param[out] code ���ص���Ӧ�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetLastResponseCode(DEVHANDLE icdev, unsigned int *code);

  /**
   * @brief  ���Flash����������
   * @par    ˵����
   * ����ָ��Flash�����е����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0x00��ʾ���İ��ϵ�Flash��0x01��ʾ��Spi��Flash��0x02��ʾ����Spi��Flash��0x03��ʾ�����洢��Flash��0x04��ʾ�ֿ�洢��Flash��
   * @param[in] sector ������ַ��0xFFFFFFFF��ʾ����ȫ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MultiFlashErase(DEVHANDLE icdev, unsigned char type, unsigned int sector);

  /**
   * @brief  ���Flash�����ݡ�
   * @par    ˵����
   * ��ȡָ��Flash�е����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0x00��ʾ���İ��ϵ�Flash��0x01��ʾ��Spi��Flash��0x02��ʾ����Spi��Flash��0x03��ʾ�����洢��Flash��0x04��ʾ�ֿ�洢��Flash��
   * @param[in] offset_h ƫ�Ƶ�ַ����λ����
   * @param[in] offset_l ƫ�Ƶ�ַ����λ����
   * @param[in] length_h ��ȡ���ȣ���λ����
   * @param[in] length_l ��ȡ���ȣ���λ����
   * @param[out] data_buffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MultiFlashRead(DEVHANDLE icdev, unsigned char type, unsigned int offset_h, unsigned int offset_l, unsigned int length_h, unsigned int length_l, unsigned char *data_buffer);

  /**
   * @brief  ���Flashд���ݡ�
   * @par    ˵����
   * д�����ݵ�ָ��Flash�С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0x00��ʾ���İ��ϵ�Flash��0x01��ʾ��Spi��Flash��0x02��ʾ����Spi��Flash��0x03��ʾ�����洢��Flash��0x04��ʾ�ֿ�洢��Flash��
   * @param[in] offset_h ƫ�Ƶ�ַ����λ����
   * @param[in] offset_l ƫ�Ƶ�ַ����λ����
   * @param[in] length_h ��ȡ���ȣ���λ����
   * @param[in] length_l ��ȡ���ȣ���λ����
   * @param[in] data_buffer �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MultiFlashWrite(DEVHANDLE icdev, unsigned char type, unsigned int offset_h, unsigned int offset_l, unsigned int length_h, unsigned int length_l, const unsigned char *data_buffer);

  /**
   * @brief  ���FLASH������С��ȡ��
   * @par    ˵����
   * ��ȡָ��Flash��������С��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0x00��ʾ���İ��ϵ�Flash��0x01��ʾ��Spi��Flash��0x02��ʾ����Spi��Flash��0x03��ʾ�����洢��Flash��0x04��ʾ�ֿ�洢��Flash��
   * @param[out] length_h ���صĳ��ȣ���λ����
   * @param[out] length_l ���صĳ��ȣ���λ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MultiFlashGetSize(DEVHANDLE icdev, unsigned char type, unsigned int *length_h, unsigned int *length_l);

  /**
   * @brief  ����Ѱ����
   * @par    ˵����
   * �п�Ѱ�����޿�Ѱ�����࿨����ͻ��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܡ�
   * @n 0 - �����ɹ���
   * @n 1 - δ��⵽��Ƶ����
   * @n 2 - ��⵽����֤��
   * @n 3 - �࿨������
   * @n 4 - ������Ƶ����
   * @n 5 - ��⵽Type A / Type B CPU����
   * @n 6 - ��⵽�п���Ŀ�Ƭ���ڸ�Ӧ����
   */
  short USER_API dc_RfMultiRequest(DEVHANDLE icdev);

  /**
   * @brief  TTS�������š�
   * @par    ˵����
   * TTS�������š�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0x00��ʾֹͣ���ţ�0x01��ʾ��������ΪUCS-2LE���룬0x02��ʾ��������ΪGBK���롣
   * @param[in] content �������ݡ�
   * @param[in] content_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_TtsVoicePlay(DEVHANDLE icdev, unsigned char type, const unsigned char *content, int content_len);

  /**
   * @brief  TTS�������á�
   * @par    ˵����
   * TTS�������á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0x01��ʾ�������٣�0x02��ʾ����������0x03��ʾ������ɫ��
   * @param[in] value ����ֵ��0x00~0xFF��������Ϊ0x80��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_TtsVoiceConfig(DEVHANDLE icdev, unsigned char type, unsigned char value);

  /**
   * @brief  ���ö�ά�빤��ģʽ��
   * @par    ˵����
   * ���ö�ά�빤��ģʽ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] content �������ݡ�
   * @param[in] content_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_Config2DBarcode(DEVHANDLE icdev, const unsigned char *content, int content_len);

  /**
   * @brief  ɨ���ά�롣
   * @par    ˵����
   * ɨ���ά�롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] time_ms �豸��ʱֵ����λΪ���롣
   * @param[out] message_data ��ά�룬���ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_Scan2DBarcode(DEVHANDLE icdev, unsigned short time_ms, char *message_data);

  /**
   * @brief  ��������̡�
   * @par    ˵����
   * ��������̲��ҷ��������Դ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ�0��ʾ����������̣�1��ʾ����������̡�
   * @param[out] version ���صİ汾�ַ����������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_KeypadOpen(DEVHANDLE icdev, int number, char *version);

  /**
   * @brief  �ر�������̡�
   * @par    ˵����
   * �ر�������̲����ͷ������Դ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ�0��ʾ����������̣�1��ʾ����������̡�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_KeypadClose(DEVHANDLE icdev, int number);

  /**
   * @brief  װ����Կ��
   * @par    ˵����
   * װ����Կ��������������ñ��档
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ�0��ʾ����������̣�1��ʾ����������̡�
   * @param[in] set_index ��Կ��������0~15����
   * @param[in] sub_index ��Կ������0~15��������0��ʾ����Կ��
   * @param[in] type ��Կ���ͣ�0��ʾ��DES��1��ʾ��DES��2��ʾ24�ֽ���DES��16��ʾSM4��
   * @param[in] mode װ��ģʽ��0��ʾ����װ�أ�1��ʾ����װ�ء�
   * @param[in] key_data ��Կ���ݡ�
   * @param[in] key_len ��Կ���ݵĳ��ȡ�
   * @param[in] flag ��־��0��ʾ����֤��1��ʾ��֤���1��2��ʾ��֤���2��
   * @param[in,out] ex_data �������ݣ������� @a flag ��ֵ��������
   * @n flagΪ1 - ex_dataΪout��ʽ�Ĳ��������ؼ���ȫ0x00������ݡ�
   * @n flagΪ2 - ex_dataΪIn��ʽ�Ĳ������������ȫ0x00������ݡ�
   * @param[in,out] ex_len �������ݵĳ��ȣ������� @a flag ��ֵ��������
   * @n flagΪ1 - ex_lenΪOut��ʽ�Ĳ��������� @a ex_data �ĳ��ȡ�
   * @n flagΪ2 - ex_lenΪIn��ʽ�Ĳ��������� @a ex_data �ĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_KeypadLoadKey(DEVHANDLE icdev, int number, int set_index, int sub_index, int type, int mode, const unsigned char *key_data, int key_len, int flag, unsigned char *ex_data, int *ex_len);

  /**
   * @brief  �����Կ��
   * @par    ˵����
   * ������������ָ������Կ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ�0��ʾ����������̣�1��ʾ����������̡�
   * @param[in] set_index ��Կ��������0~15����-1��ʾȫ����Կ�ס�
   * @param[in] sub_index ��Կ������0~15��������0��ʾ����Կ��-1��ʾȫ����Կ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_KeypadClearKey(DEVHANDLE icdev, int number, int set_index, int sub_index);

  /**
   * @brief  ��ȡ��Կ���͡�
   * @par    ˵����
   * ��ȡ���������ָ����Կ�����͡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ�0��ʾ����������̣�1��ʾ����������̡�
   * @param[in] set_index ��Կ��������0~15����
   * @param[in] sub_index ��Կ������0~15��������0��ʾ����Կ��
   * @param[out] type ��Կ���ͣ�0��ʾ��DES��1��ʾ��DES��2��ʾ24�ֽ���DES��16��ʾSM4��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_KeypadGetKeyType(DEVHANDLE icdev, int number, int set_index, int sub_index, int *type);

  /**
   * @brief  ���ݼӽ��ܡ�
   * @par    ˵����
   * ʹ�����������װ�ص���Կ�������ݼӽ��ܲ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ�0��ʾ����������̣�1��ʾ����������̡�
   * @param[in] set_index ��Կ��������0~15����
   * @param[in] sub_index ��Կ������0~15��������0��ʾ����Կ��
   * @param[in] flag ��־��0��ʾʹ����ԿĬ���㷨��1��ʾʹ����Կ0~7�ֽ��㵥DES��2��ʾʹ����Կ8~15�ֽ��㵥DES��3��ʾʹ����Կ16~23�ֽ��㵥DES��
   * @param[in] mode ģʽ��0��ʾ���ܣ�1��ʾ���ܡ�
   * @param[in] in_data �������ݡ�
   * @param[in] in_len �������ݵĳ��ȡ�
   * @param[out] out_data ������ݡ�
   * @param[out] out_len ������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_KeypadAlgorithm(DEVHANDLE icdev, int number, int set_index, int sub_index, int flag, int mode, const unsigned char *in_data, int in_len, unsigned char *out_data, int *out_len);

  /**
   * @brief  �����������롣
   * @par    ˵����
   * ����������̵İ������룬��ʱ�豸����ʼ�����û��İ������������ע�⣺���֮ǰ�Ѿ�������δ�˳�����ô�ٴε��ý���ʹ���µĲ����������ڲ�������ԡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ�0��ʾ����������̣�1��ʾ����������̡�
   * @param[in] mode ģʽ��0��ʾ��ȡ���ģ�1��ʾ��ȡANSI X9.8 PIN BLOCK���ܵİ������ݣ�2��ʾ��ȡIBM3624���ܵİ������ݣ�3��ʾ��ȡASCII���ܵİ������ݡ�
   * @param[in] set_index ��Կ��������0~15��������ģʽ�ڲ����������Ĵ��롣
   * @param[in] sub_index ��Կ������0~15��������0��ʾ����Կ������ģʽ�ڲ����������Ĵ��롣
   * @param[in] in_data �������ݣ�������ģʽ������ģʽ�ڲ����������Ĵ��룬����ģʽΪ���ţ��̶�Ϊ12���ֽڡ�
   * @param[in] in_len �������ݵĳ��ȡ�
   * @param[in] min_len ��С���ȡ�
   * @param[in] max_len ��󳤶ȡ�
   * @param[in] auto_end ����ʱ�������ƣ��˴��Ľ�����ʾ�Ľ������豸�Ƿ���Զ����롾ȷ�ϡ���ֵ����0��ʾ���Զ�������1��ʾ�Զ�������
   * @param[in] enable_beep ����ʱ���������ƣ�0��ʾ��ʹ�÷�����1��ʾʹ�÷�����
   * @param[in] time_s �ӿ�ʼ���������ΰ���֮���ʱ������ʱֵ����λΪ�룬����-1��ʾ��ʹ��ʱ������ʱֵ��0��ʾȱʡֵ�����豸ʵ�־�������
   * @param[out] out_data ������ݡ�
   * @param[out] out_len ������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_KeypadStartInput(DEVHANDLE icdev, int number, int mode, int set_index, int sub_index, const unsigned char *in_data, int in_len, int min_len, int max_len, int auto_end, int enable_beep, int time_s, unsigned char *out_data, int *out_len);

  /**
   * @brief  ��ȡ��ֵ��
   * @par    ˵����
   * ��ȡһ�����µİ���ֵ��ÿ��ֻ�ܻ�ȡһ����
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ�0��ʾ����������̣�1��ʾ����������̡�
   * @param[out] value ��������ģʽ����ֵ��Ϊ��ֵ��ASCIIֵ�������1Ϊ0x31����������ģʽ����ֵ��Ϊ0x2A���κ�����ģʽ�¿��Ƽ��ļ�ֵһ�£����˸�Ϊ0x08����ȷ�ϡ�Ϊ0x0D����ȡ����Ϊ0x1B���������Ϊ0x7F������û�а�������Ϊ0xFF�����볬ʱΪ0x80��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_KeypadGetKeyValue(DEVHANDLE icdev, int number, unsigned char *value);

  /**
   * @brief  �˳����һ�ȡ�������ݡ�
   * @par    ˵����
   * �˳����һ�ȡ������̵��������ݣ���ʱ�豸��ֹͣ�����û��İ������������
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ�0��ʾ����������̣�1��ʾ����������̡�
   * @param[in] mode ģʽ��0��ʾ���˳���1��ʾ�˳����һ�ȡ���ݡ�
   * @param[out] out_data ������ݡ�
   * @param[out] out_len ������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_KeypadExitAndGetInput(DEVHANDLE icdev, int number, int mode, unsigned char *out_data, int *out_len);

  /**
   * @brief  ���ü�ֵ��
   * @par    ˵����
   * ����һ�����µİ���ֵ��ÿ��ֻ������һ����
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ�0��ʾ����������̣�1��ʾ����������̡�
   * @param[in] value ��ֵ��Ϊ��ֵ��ASCIIֵ�������1Ϊ0x31�����Ƽ����˸�Ϊ0x08����ȷ�ϡ�Ϊ0x0D����ȡ����Ϊ0x1B���������Ϊ0x7F��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_KeypadSetKeyValue(DEVHANDLE icdev, int number, unsigned char value);

  /**
   * @brief  ������
   */
  short USER_API dc_KeypadCalculatePinBlock(DEVHANDLE icdev, int number, int mode, int set_index, int sub_index, const unsigned char *card_number_data, int card_number_len, const unsigned char *in_data, int in_len, unsigned char *out_data, int *out_len);

  /**
   * @brief  ��ȡ�豸Ψһʶ���롣
   * @par    ˵����
   * ��ȡ�豸�ڲ�д���Ψһʶ���롣
   * @param[in] icdev �豸��ʶ����
   * @param[out] uid ���ص�Ψһʶ�����ַ����������ٷ���33���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetDeviceUid(DEVHANDLE icdev, char *uid);

  /**
   * @brief  ��ȡ�豸�̻��š�
   * @par    ˵����
   * ��ȡ�豸�ڲ�д����̻��š�
   * @param[in] icdev �豸��ʶ����
   * @param[out] number ���ص��̻����ַ����������ٷ���33���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetDeviceTradeNumber(DEVHANDLE icdev, char *number);

  /**
   * @brief  ��ȡ�豸�ն˺š�
   * @par    ˵����
   * ��ȡ�豸�ڲ�д����ն˺š�
   * @param[in] icdev �豸��ʶ����
   * @param[out] number ���ص��ն˺��ַ����������ٷ���33���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetDeviceTerminalNumber(DEVHANDLE icdev, char *number);

  /**
   * @brief  ��ȡ�豸�Զ�����Ϣ��
   * @par    ˵����
   * ��ȡ�豸�Զ�����Ϣ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] info ���ص���Ϣ���̶�Ϊ8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadDeviceCustomInfo(DEVHANDLE icdev, unsigned char *info);

  /**
   * @brief  д���豸�Զ�����Ϣ��
   * @par    ˵����
   * д���豸�Զ�����Ϣ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] info ������Ϣ���̶�Ϊ8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WriteDeviceCustomInfo(DEVHANDLE icdev, const unsigned char *info);

  /**
   * @brief  ��ȡ�豸�ͺ����ơ�
   * @par    ˵����
   * ��ȡ�豸�ͺ����ơ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] name ���ص��ͺ������ַ����������ٷ���33���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadDeviceTypeName(DEVHANDLE icdev, char *name);

  /**
   * @brief  д���豸�ͺ����ơ�
   * @par    ˵����
   * д���豸�ͺ����ơ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] name �����ͺ����ƣ����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WriteDeviceTypeName(DEVHANDLE icdev, const char *name);

  /**
   * @brief  ����ģ���Դ���ơ�
   * @par    ˵����
   * ����ģ���Դ���ơ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] value ���أ�0x00��ʾ�رգ�0x01��ʾ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WirelessPowerControl(DEVHANDLE icdev, unsigned char value);

  /**
   * @brief  ��TCP���ӡ�
   * @par    ˵����
   * ��TCP���ӡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ����ģ�����ͣ�0x00��ʾCDMA��0x01��ʾGPRS��0x02��ʾWIFI��
   * @param[in] network_address IP��ַ�����������ݸ�ʽΪ'\0'��β���ַ�����
   * @param[in] network_port IP�˿ںţ����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WirelessTcpOpen(DEVHANDLE icdev, unsigned char type, const char *network_address, const char *network_port);

  /**
   * @brief  �ر�TCP���ӡ�
   * @par    ˵����
   * �ر�TCP���ӡ�
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WirelessTcpClose(DEVHANDLE icdev);

  /**
   * @brief  ��ѯ����״̬��
   * @par    ˵����
   * ��ѯ����״̬��
   * @param[in] icdev �豸��ʶ����
   * @param[out] value ״ֵ̬���̶�Ϊ1���ֽڡ�
   * @n 0x00 - ��ʾTCP��ʼ״̬��
   * @n 0x01 - ��ʾ����ģ���޷�ͨѶ��
   * @n 0x02 - ��ʾ����ģ���Դ�رա�
   * @n 0x03 - ��ʾ����ģ���Դ������
   * @n 0x04 - ��ʾSIM���쳣�����޿���
   * @n 0x05 - ��ʾ�����ź��쳣��
   * @n 0x06 - ��ʾ����ģ���Լ�ɹ���
   * @n 0x07 - ��ʾTCP����״̬��
   * @n 0x08 - ��ʾTCP�Ѿ����ӳɹ���
   * @n 0x09 - ��ʾTCP�Ѿ��Ͽ����ӡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WirelessGetStatus(DEVHANDLE icdev, unsigned char *value);

  /**
   * @brief  �������ݵ�����ˡ�
   * @par    ˵����
   * �������ݵ�����ˡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ����ģ�����ͣ�0x00��ʾCDMA��0x01��ʾGPRS��0x02��ʾWIFI��
   * @param[in] data_buffer �������ݡ�
   * @param[in] data_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WirelessSend(DEVHANDLE icdev, unsigned char type, const unsigned char *data_buffer, int data_len);

  /**
   * @brief  ������˽������ݡ�
   * @par    ˵����
   * ������˽������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ����ģ�����ͣ�0x00��ʾCDMA��0x01��ʾGPRS��0x02��ʾWIFI��
   * @param[in] mode ģʽ��0x00��ʾ���յ����ݸ�ʽ������ʽͷ���ţ�0x01��ʾ���յ����ݸ�ʽ����ʽͷ���ţ�WIFIģ��˲�����Ч��
   * @param[in] time_100ms �豸��ʱֵ����λΪ100���롣
   * @param[out] data_buffer ���ص����ݡ�
   * @param[out] data_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WirelessRecv(DEVHANDLE icdev, unsigned char type, unsigned char mode, unsigned char time_100ms, unsigned char *data_buffer, int *data_len);

  /**
   * @brief  ����WIFI������
   * @par    ˵����
   * ����WIFI������
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ����ģ�����ͣ�0x00��ʾCDMA��0x01��ʾGPRS��0x02��ʾWIFI��
   * @param[in] ssid WIFI SSID�����ݸ�ʽΪ'\0'��β���ַ�����
   * @param[in] password WIFI���룬���ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WirelessWifiConfig(DEVHANDLE icdev, unsigned char type, const char *ssid, const char *password);

  /**
   * @brief  ��ȡGPS��Ϣ��
   * @par    ˵����
   * ��ȡGPS��Ϣ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] time_ms �豸��ʱֵ����λΪ���롣
   * @param[out] data_buffer ���ص����ݡ�
   * @param[out] data_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GpsGetInfo(DEVHANDLE icdev, int time_ms, unsigned char *data_buffer, int *data_len);

  /**
   * @brief  ���ýӴ�ʽCPU��616C���á�
   * @par    ˵����
   * ���ýӴ�ʽCPU��616C���á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] value ����ֵ��0x00��ʾ�豸������616C��0x01��ʾ�豸����616C��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SetCpuCard616C(DEVHANDLE icdev, unsigned char value);

  /**
   * @brief  ��ȡ�Ӵ�ʽCPU��616C���á�
   * @par    ˵����
   * ��ȡ�Ӵ�ʽCPU��616C���á�
   * @param[in] icdev �豸��ʶ����
   * @param[out] value ����ֵ��0x00��ʾ�豸������616C��0x01��ʾ�豸����616C��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetCpuCard616C(DEVHANDLE icdev, unsigned char *value);

  /**
   * @brief  ��ȡоƬΨһID�š�
   * @par    ˵����
   * ��ȡоƬΨһID�š�
   * @param[in] icdev �豸��ʶ����
   * @param[out] number ���ص�ID���ַ����������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetChipIdNumber(DEVHANDLE icdev, char *number);

  /**
   * @brief  �������ʾ��
   * @par    ˵����
   * �������ʾ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ����ܱ�ţ�1~12����
   * @param[in] mode ģʽ��0x00��ʾ����������ʾ��0x01��ʾ������˸��ʾ��0x02��ʾ���ݲ���˸��ʾ��0x03��ʾ���ݴ�С������ʾ��
   * @param[in] value ��ʾ����ֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_LedDisplay(DEVHANDLE icdev, unsigned char number, unsigned char mode, unsigned char value);

  /**
   * @brief  �������ʾ���á�
   * @par    ˵����
   * �������ʾ���á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��
   * @n 0x00 - ���ơ�
   * @n 0x01 - ���ơ�
   * @n 0x02 - ��ѭ����
   * @n 0x03 - ��ѭ����
   * @n 0x04 - ��ʾ�����
   * @n 0x05 - ��ʾ��λ��
   * @n 0x06 - ��ʾ�������ӡ�
   * @n 0x07 - ��ʾ���Ƚ��͡�
   * @n 0x08 - ����ʾ��
   * @n 0x09 - ����ʾ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_LedControl(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ͸��CRCģʽ���á�
   * @par    ˵����
   * ͸��CRCģʽ���á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] tx_crc TXCRCģʽ��0x00��ʾ�ڲ�������TXCRC��0x01��ʾ�ڲ�����TXCRC��ֻ֧��ISO14443��׼������
   * @param[in] rx_crc RXCRCģʽ��0x00��ʾ�ڲ�������RXCRC��0x01��ʾ�ڲ�����RXCRC��ֻ֧��ISO14443��׼������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_RfCrcModeConfig(DEVHANDLE icdev, unsigned char tx_crc, unsigned char rx_crc);

  /**
   * @brief  ��ȡTCP��Ϣ��
   * @par    ˵����
   * ��ȡTCP��Ϣ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0x01��ʾ������ģ���ͺţ�0x02��ʾ�����õ�ǰ�ź�ǿ�ȡ�
   * @param[out] data_buffer ���ص����ݡ�
   * @param[out] data_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_TcpGetInfo(DEVHANDLE icdev, unsigned char type, unsigned char *data_buffer, int *data_len);

  /**
   * @brief  ��Ƶ���ߴ򿪡�
   * @par    ˵����
   * ��Ƶ���ߴ򿪡�
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_RfMarch(DEVHANDLE icdev);

  /**
   * @brief  ѡ�����ߡ�
   * @par    ˵����
   * ѡ�����ߡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ���߱�ţ���1��ʼ��š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelectAntenna(DEVHANDLE icdev, unsigned char number);

  /**
   * @brief  ������MAC��ַ��
   * @par    ˵����
   * ������MAC��ַ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] data_buffer ���ص�������ַ��
   * @param[out] data_len ����������ַ�ĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadNicMacAddress(DEVHANDLE icdev, unsigned char *data_buffer, int *data_len);

  /**
   * @brief  д����MAC��ַ��
   * @par    ˵����
   * д����MAC��ַ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] data_buffer ����������ַ��
   * @param[in] data_len ����������ַ�ĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WriteNicMacAddress(DEVHANDLE icdev, const unsigned char *data_buffer, int data_len);

  /**
   * @brief  ����������
   * @par    ˵����
   * ����������
   * @param[in] icdev �豸��ʶ����
   * @param[in] data_buffer ����IP��ַ��
   * @param[in] data_len ����IP��ַ�ĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_TestNic(DEVHANDLE icdev, const unsigned char *data_buffer, int data_len);

  /**
   * @brief  ����USB��
   * @par    ˵����
   * ����USB��
   * @param[in] icdev �豸��ʶ����
   * @param[in] data_buffer ������
   * @param[in] data_len �������̶�Ϊ0��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_TestUsb(DEVHANDLE icdev, const unsigned char *data_buffer, int data_len);

  /**
   * @brief  ���Դ��ڡ�
   * @par    ˵����
   * ���Դ��ڡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ���ڱ�ţ�1~3����
   * @param[in] name �����������ƣ��磺"COM1:115200"��
   * @param[in,out] data_buffer ͨѶ���ݡ�
   * @param[in,out] data_len ͨѶ���ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_TestCom(DEVHANDLE icdev, unsigned char number, const char *name, unsigned char *data_buffer, int *data_len);

  /**
   * @brief  �趨��ʱIP��ַ��
   * @par    ˵����
   * �趨��ʱIP��ַ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] data_buffer ����IP��ַ��
   * @param[in] data_len ����IP��ַ�ĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ConfigTempIpAddress(DEVHANDLE icdev, const unsigned char *data_buffer, int data_len);

  /**
   * @brief  �����������Զ��ϴ�����ģʽ��ȡ��
   * @par    ˵����
   * ��ȡ�����������Զ��ϴ�����ģʽ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] mode ģʽ��
   * @n 0 - ����������������ĸ����ĸת��Ϊ��д��ĸ��
   * @n 1 - ���������ԭʼ���ݣ�Ĭ�ϣ���
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetMagFormat(DEVHANDLE icdev, int *mode);

  /**
   * @brief  ������������ȡ��
   * @par    ˵����
   * ��ȡ�����������Ĳ���������������Զ������ģʽ��Ч��
   * @param[in] icdev �豸��ʶ����
   * @param[out] mode ģʽ��0x00��ʾ���������0x01��ʾ�Զ������
   * @param[out] track1 1���������
   * @n bit0 - �������������ã�0��ʾ�������1��ʾ�����
   * @n bit1 - �������ͷ���ã�0��ʾ������ͷ��1��ʾ������ͷ��
   * @n bit2 - �������β���ã�0��ʾ������β��1��ʾ������β��
   * @n bit3 - ������ݻ��з����ã�0��ʾ�޻��з���1��ʾ�л��з���
   * @n bit4 - ������ݵȺ�������������ã�0��ʾ�������1��ʾ�����
   * @n bit5~bit7 - ������
   * @param[out] track2 2���������
   * @n bit0 - �������������ã�0��ʾ�������1��ʾ�����
   * @n bit1 - �������ͷ���ã�0��ʾ������ͷ��1��ʾ������ͷ��
   * @n bit2 - �������β���ã�0��ʾ������β��1��ʾ������β��
   * @n bit3 - ������ݻ��з����ã�0��ʾ�޻��з���1��ʾ�л��з���
   * @n bit4 - ������ݵȺ�������������ã�0��ʾ�������1��ʾ�����
   * @n bit5~bit7 - ������
   * @param[out] track3 3���������
   * @n bit0 - �������������ã�0��ʾ�������1��ʾ�����
   * @n bit1 - �������ͷ���ã�0��ʾ������ͷ��1��ʾ������ͷ��
   * @n bit2 - �������β���ã�0��ʾ������β��1��ʾ������β��
   * @n bit3 - ������ݻ��з����ã�0��ʾ�޻��з���1��ʾ�л��з���
   * @n bit4 - ������ݵȺ�������������ã�0��ʾ�������1��ʾ�����
   * @n bit5~bit7 - ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetMag(DEVHANDLE icdev, unsigned char *mode, unsigned char *track1, unsigned char *track2, unsigned char *track3);

  /**
   * @brief  ��ȡ�����豸ͨѶ�����ʡ�
   * @par    ˵����
   * ��ȡ�����豸ͨѶ�����ʡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] baud �����ʡ�
   * @n 0x00 - 115200��
   * @n 0x01 - 57600��
   * @n 0x02 - 38400��
   * @n 0x03 - 19200��
   * @n 0x04 - 9600��
   * @n 0x05 - 4800��
   * @n 0x06 - 2400��
   * @n 0x07 - 1200��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetDeviceBaud(DEVHANDLE icdev, unsigned char *baud);

  /**
   * @brief  ��ȡUSBģʽ��
   * @par    ˵����
   * ��ȡUSBͨѶģʽ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] ucmode ģʽ��
   * @n 0x00 - ��ʾ�жϷ��ͺ��жϽ��գ���ʱVID=257B��PID=3010��
   * @n 0x01 - ��ʾ���Ʒ��ͺͿ��ƽ��գ���ʱVID=257B��PID=3011��
   * @n 0x02 - ��ʾ���Ʒ��ͺ��жϽ��գ���ʱVID=0471��PID=7002��
   * @n 0x03 - ��ʾ���Ʒ��ͺ��жϽ��գ���ʱVID=0471��PID=7003��
   * @n 0x04 - ��ʾ���Ʒ��ͺ��жϽ��գ���ʱVID=0471��PID=7003��֧�ִ������Զ��ϴ����ܡ�
   * @n 0x05 - ���⴮�ڡ�
   * @n 0x06 - ���⴮�ڣ�������HID����ʱHID��VID=0471��PID=7003��
   * @n 0x07 - ��ʾ���Ʒ��ͺ��жϽ��գ���ʱVID=0471��PID=7002��֧�ִ������Զ��ϴ����ܡ�
   * @n 0x08 - ��ʾ���Ʒ��ͺ��жϽ��գ���ʱVID=257B��PID=3031��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetUsbMode(DEVHANDLE icdev, unsigned char *ucmode);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ե�ǰ����CPU������ָ���������ע��˽ӿ��ѷ�װ��Э�鲿�֣��ڲ�������SW1Ϊ0x61��0x6C�������
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_cpuapduresponInt(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_cpuapduresponInt ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_cpuapduresponInt_hex(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  ��ʼɨ���ά�롣
   * @par    ˵����
   * ��ʼɨ���ά�롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode �������̶�Ϊ0x00��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_Scan2DBarcodeStart(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ��ȡ��ά�����ݡ�
   * @par    ˵����
   * ��ȡ��ά�����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_Scan2DBarcodeGetData(DEVHANDLE icdev, int *rlen, unsigned char *rdata);

  /**
   * @brief  �˳�ɨ���ά�롣
   * @par    ˵����
   * �˳�ɨ���ά�롣
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_Scan2DBarcodeExit(DEVHANDLE icdev);

  /**
   * @brief  �������ż���ʾ��
   * @par    ˵����
   * �����豸���������Լ��Ƿ���Ҫ��ʾ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode �����κţ�0~8����
   * @n 0 - ���������롣
   * @n 1 - ������ԭ���롣
   * @n 2 - ��������һ�Ρ�
   * @n 3 - �����������롣
   * @n 4 - ��ȷ�������롣
   * @n 5 - �����޸ĳɹ���
   * @n 6 - ��忨��
   * @n 7 - ��ˢ����
   * @n 8 - �������
   * @param[in] type ��ʾ���ͣ�0��ʾ��ʾ�����������ݣ�1��ʾ����ʾ�����������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_VoicePlayAndDisplay(DEVHANDLE icdev, unsigned char mode, unsigned char type);

  /**
   * @brief  ����豸���á�
   * @par    ˵����
   * ����豸���á�
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ClearDeviceConfig(DEVHANDLE icdev);

  /**
   * @brief  �Ӵ�ʽCPU��PPS�������á�
   * @par    ˵����
   * �Ӵ�ʽCPU��PPS�������á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0��ʾ�ײ㲻֧��PPS��1��ʾ�ײ�֧��PPS��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SetCpuCardPps(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  �Ӵ�ʽCPU��PPS���û�ȡ��
   * @par    ˵����
   * �Ӵ�ʽCPU��PPS���û�ȡ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] mode ģʽ��0��ʾ�ײ㲻֧��PPS��1��ʾ�ײ�֧��PPS��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetCpuCardPps(DEVHANDLE icdev, unsigned char *mode);

  /**
   * @brief  ��ά���Զ�����������á�
   * @par    ˵����
   * ��ά���Զ�����������á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0��ʾ�������1��ʾ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_Set2DBarcodeConfig(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ��ά���Զ�������û�ȡ��
   * @par    ˵����
   * ��ά���Զ�������û�ȡ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] mode ģʽ��0��ʾ�������1��ʾ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_Get2DBarcodeConfig(DEVHANDLE icdev, unsigned char *mode);

  /**
   * @brief  ������
   */
  short USER_API dc_checkcopy_m1(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ˢ�·��������档
   * @par    ˵����
   * ˢ�·��������档
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_RefreshAndDisplayMainUI(DEVHANDLE icdev);

  /**
   * @brief  ������
   */
  short USER_API dc_GlyProCommand(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  ������
   */
  short USER_API dc_GlyProCommand_hex(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  ��ȡSAM_Aģ�����кš�
   * @par    ˵����
   * ��ȡSAM_Aģ�����кš�
   * @param[in] icdev �豸��ʶ����
   * @param[out] number ���ص����к��ַ����������ٷ���64���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SamAReadSerialNumber(DEVHANDLE icdev, char *number);

  /**
   * @brief  2.4G APDUָ�����
   * @par    ˵����
   * 2.4G APDUָ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_2dot4Apdu(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  2.4G APDUָ�����
   * @par    ˵����
   * ::dc_2dot4Apdu ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_2dot4Apdu_hex(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  ��ʾͼ��
   * @par    ˵����
   * ����Ļ����ʾͼ�������ͼ��ָ����ǰ��ͼ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] x ͼ����ʾ����ʼX���ꡣ
   * @param[in] y ͼ����ʾ����ʼY���ꡣ
   * @param[in] w ͼ����ʾ�Ŀ��ȡ�
   * @param[in] h ͼ����ʾ�ĸ߶ȡ�
   * @param[in] time_s ��ʾʱ�䣬��λΪ�룬0��ʾһֱ��ʾ��
   * @param[in] image_data ͼ�����ݡ���ʽΪ����4���ֽ�ͼ����Ϣ���ȣ�Big-Endian������ͼ����Ϣ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ScreenDisplayImage(DEVHANDLE icdev, int x, int y, int w, int h, int time_s, const unsigned char *image_data);

  /**
   * @brief  �����ʾ��
   * @par    ˵����
   * �����Ļ�ϵ���ʾ�������������ʾΪǰ����
   * @param[in] icdev �豸��ʶ����
   * @param[in] x �����ʾ����ʼX���ꡣ
   * @param[in] y �����ʾ����ʼY���ꡣ
   * @param[in] w �����ʾ�Ŀ��ȡ�
   * @param[in] h �����ʾ�ĸ߶ȡ�
   * @param[in] time_s ��ʱ���ʱ�䣬��λΪ�룬0��ʾ���������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ScreenClearDisplay(DEVHANDLE icdev, int x, int y, int w, int h, int time_s);

  /**
   * @brief  װ�ر���ͼ��
   * @par    ˵����
   * װ�ر���ͼ���豸�ڲ���
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ͼ����ţ���1��ʼ��š�
   * @param[in] image_data ͼ�����ݡ���ʽΪ����4���ֽ�ͼ����Ϣ���ȣ�Big-Endian������ͼ����Ϣ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ScreenLoadBgImage(DEVHANDLE icdev, int number, const unsigned char *image_data);

  /**
   * @brief  ��ʾ����ͼ��
   * @par    ˵����
   * ��ʾ�豸�ڲ�װ�صı���ͼ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ͼ����ţ���1��ʼ��š�
   * @param[in] x ͼ����ʾ����ʼX���ꡣ
   * @param[in] y ͼ����ʾ����ʼY���ꡣ
   * @param[in] w ͼ����ʾ�Ŀ��ȡ�
   * @param[in] h ͼ����ʾ�ĸ߶ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ScreenDisplayBgImage(DEVHANDLE icdev, int number, int x, int y, int w, int h);

  /**
   * @brief  ��ȡ��ʾ���á�
   * @par    ˵����
   * ��ȡ��ʾ���á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] tag ��ǩ��
   * @n 0x01 - Ϩ��ʱ�䣬������Ϣ���ȹ̶�Ϊ4���ֽڣ���λΪ�루Big-Endian�����磺config_data = {0x04,0x00,0x00,0x00,0x10}��ʾϨ��ʱ��Ϊ16�롣
   * @n 0x02 - ��ʾ�ֱ��ʣ�������Ϣ���ȹ̶�Ϊ4���ֽڣ�ǰ2���ֽ�Ϊˮƽ�ֱ��ʣ�Big-Endian������2���ֽ�Ϊ��ֱ�ֱ��ʣ�Big-Endian�����磺config_data = {0x04,0x02,0x80,0x01,0xE0}��ʾ�ֱ���Ϊ640x480��
   * @param[out] config_data �������ݡ���ʽΪ����1�ֽ�������Ϣ���ȡ���������Ϣ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ScreenDisplayGetConfig(DEVHANDLE icdev, unsigned char tag, unsigned char *config_data);

  /**
   * @brief  ������ʾ���á�
   * @par    ˵����
   * ������ʾ���á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] tag ��ǩ��
   * @n 0x01 - Ϩ��ʱ�䣬������Ϣ���ȹ̶�Ϊ4���ֽڣ���λΪ�루Big-Endian�����磺config_data = {0x04,0x00,0x00,0x00,0x10}��ʾϨ��ʱ��Ϊ16�롣
   * @n 0x02 - ��ʾ�ֱ��ʣ�������Ϣ���ȹ̶�Ϊ4���ֽڣ�ǰ2���ֽ�Ϊˮƽ�ֱ��ʣ�Big-Endian������2���ֽ�Ϊ��ֱ�ֱ��ʣ�Big-Endian�����磺config_data = {0x04,0x02,0x80,0x01,0xE0}��ʾ�ֱ���Ϊ640x480��
   * @param[in] config_data �������ݡ���ʽΪ����1�ֽ�������Ϣ���ȡ���������Ϣ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ScreenDisplaySetConfig(DEVHANDLE icdev, unsigned char tag, const unsigned char *config_data);

  /**
   * @brief  ��ʾ���֡�
   * @par    ˵����
   * ����Ļ����ʾ���֣�ע������Ҳ����ǰ����
   * @param[in] icdev �豸��ʶ����
   * @param[in] font_size �����С��
   * @param[in] fg_rgb ����ǰ��ɫ��3���ֽ�RGBֵ����һ���ֽ�ΪR���ڶ����ֽ�ΪG���������ֽ�ΪB��
   * @param[in] bg_rgb ���ֱ���ɫ��3���ֽ�RGBֵ����һ���ֽ�ΪR���ڶ����ֽ�ΪG���������ֽ�ΪB��
   * @param[in] x ������ʾ����ʼX���ꡣ
   * @param[in] y ������ʾ����ʼY���ꡣ
   * @param[in] time_s ��ʾʱ�䣬��λΪ�룬0��ʾһֱ��ʾ��
   * @param[in] line_flag ���б�־��0��ʾ���Զ����У�1��ʾ�Զ����С�
   * @param[in] voice_flag ������־��0��ʾ������������1��ʾ����������
   * @param[in] text �ı��ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ScreenDisplayText(DEVHANDLE icdev, unsigned char font_size, const unsigned char *fg_rgb, const unsigned char *bg_rgb, int x, int y, int time_s, unsigned char line_flag, unsigned char voice_flag, const char *text);

  /**
   * @brief  ��䱳����
   * @par    ˵����
   * ʹ��ָ����ɫ��䱳����
   * @param[in] icdev �豸��ʶ����
   * @param[in] rgb ��ɫ��3���ֽ�RGBֵ����һ���ֽ�ΪR���ڶ����ֽ�ΪG���������ֽ�ΪB��
   * @param[in] x ��ʼX���ꡣ
   * @param[in] y ��ʼY���ꡣ
   * @param[in] w ���ȡ�
   * @param[in] h �߶ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ScreenDisplayBgImageByColor(DEVHANDLE icdev, const unsigned char *rgb, int x, int y, int w, int h);

  /**
   * @brief  ��ʾ��ά�롣
   * @par    ˵����
   * ��ʾ��ά�롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���0x01��ʾ�����ζ�ά�롣
   * @param[in] x ��ʼX���ꡣ
   * @param[in] y ��ʼY���ꡣ
   * @param[in] w ���ȡ�
   * @param[in] h �߶ȡ�
   * @param[in] content �������ݣ�һ������TLV��ʽ��T��L�ֱ�ռ��4���ֽڣ�Big-Endian����
   * @n TΪ1��ʾֵΪASCII�ı��ַ���
   * @param[in] content_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ScreenDisplay2DBarcode(DEVHANDLE icdev, unsigned char type, int x, int y, int w, int h, const unsigned char *content, int content_len);

  /**
   * @brief  NTAG��ǩ������
   * @par    ˵����
   * NTAG��ǩ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] type �������̶�Ϊ0x00��
   * @param[in] mode ģʽ��
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sdata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_NtagCommand(DEVHANDLE icdev, unsigned char type, unsigned char mode, unsigned int slen, const unsigned char *sdata, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  ���ýӴ�ʽCPU����ѹ��
   * @par    ˵����
   * ���ýӴ�ʽCPU����ѹ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] voltage ��ѹֵ��0x00��ʾ5V��0x01��ʾ3.3V��0x02��ʾ1.8V��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SetCpuVoltage(DEVHANDLE icdev, unsigned char voltage);

  /**
   * @brief  LCD��ʾ���֡�
   * @par    ˵����
   * ���豸LCD��ʾ������ʾ���֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] line �кš�
   * @param[in] offset ƫ�ơ�
   * @param[in] mode ģʽ��0��ʾ��ɫ��1��ʾ��ɫ��
   * @param[in] charset �ַ�����
   * @n 0 - ASCII��
   * @n 1 - GB2312��
   * @n 2 - GBK��
   * @n 3 - GB18030��
   * @n 4 - ANSI��
   * @n 5 - UNICODE��Little-Endian����
   * @n 6 - UNICODE��Big-Endian����
   * @param[in] text �ı��ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_LcdDisplayText(DEVHANDLE icdev, unsigned char line, unsigned char offset, unsigned char mode, unsigned char charset, const unsigned char *text);

  /**
   * @brief  LCD��ʾ���֡�
   * @par    ˵����
   * ͨ��ָ����ģ�������ݣ����豸LCD��ʾ������ʾ���֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] line �кš�
   * @param[in] offset ƫ�ơ�
   * @param[in] mode ģʽ��0��ʾ��ɫ��1��ʾ��ɫ��
   * @param[in] charset �ַ�����
   * @n 0 - ASCII��
   * @n 1 - GB2312��
   * @n 2 - GBK��
   * @n 3 - GB18030��
   * @n 4 - ANSI��
   * @n 5 - UNICODE��Little-Endian����
   * @n 6 - UNICODE��Big-Endian����
   * @param[in] text �ı��ַ�����
   * @param[in] data ��ģ�������ݡ�
   * @param[in] data_len ��ģ�������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_LcdDisplayTextFromData(DEVHANDLE icdev, unsigned char line, unsigned char offset, unsigned char mode, unsigned char charset, const unsigned char *text, const unsigned char *data, int data_len);

  /**
   * @brief  װ���ļ����ݡ�
   * @par    ˵����
   * װ���ļ����ݵ��豸�С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] file_type �ļ����͡�
   * @param[in] file_size �ļ���С��
   * @param[in] flag �����־��
   * @n 0 - ׼�����䡣
   * @n 1~0xFFFFFFFE - �ǽ�β���䡣
   * @n 0xFFFFFFFF - ��β���䡣
   * @param[in] data ���ݣ��� @a flag ��Ϊ0ʱ��Ч��
   * @param[in,out] data_size ���ݴ�С��
   * @n flagΪ0 - data_sizeΪOut��ʽ�Ĳ����������豸֧�ֵ����ְ����ȡ�
   * @n flag��Ϊ0 - data_sizeΪIn��ʽ�Ĳ��������� @a data �ĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_DownloadFileData(DEVHANDLE icdev, unsigned int file_type, unsigned int file_size, unsigned int flag, const unsigned char *data, unsigned int *data_size);

  /**
   * @brief  ���1608����
   * @par    ˵����
   * ����Ƿ����1608����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_check_1608(DEVHANDLE icdev);

  /**
   * @brief  1608���µ硣
   * @par    ˵����
   * ��1608�������µ������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_down_1608(DEVHANDLE icdev);

  /**
   * @brief  ��1608����
   * @par    ˵����
   * ��ȡ1608�������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0~7 - ��ʾ�û�����
   * @n 8 - ��ʾ��������
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] readdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_1608(DEVHANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  ��1608����
   * @par    ˵����
   * ::dc_read_1608 ��HEX��ʽ�ӿڣ����� @a readdata ΪHEX��ʽ��
   */
  short USER_API dc_read_1608_hex(DEVHANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  д1608����
   * @par    ˵����
   * д�����ݵ�1608���С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0~7 - ��ʾ�û�����
   * @n 8 - ��ʾ��������
   * @param[in] offset ƫ�ơ�
   * @param[in] length д�볤�ȡ�
   * @param[in] writedata �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_1608(DEVHANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  д1608����
   * @par    ˵����
   * ::dc_write_1608 ��HEX��ʽ�ӿڣ����� @a writedata ΪHEX��ʽ��
   */
  short USER_API dc_write_1608_hex(DEVHANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  ��֤1608�����롣
   * @par    ˵����
   * ʹ�ô������������֤1608�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0~7 - ��ʾ�û�����
   * @n 8 - ��ʾ��������
   * @param[in] type ���ͣ�0��ʾ��֤д��Կ��1��ʾ��֤����Կ��
   * @param[in] password ���룬�̶�Ϊ3���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_checkpass_1608(DEVHANDLE icdev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  ��֤1608�����롣
   * @par    ˵����
   * ::dc_checkpass_1608 ��HEX��ʽ�ӿڣ����� @a password ΪHEX��ʽ��
   */
  short USER_API dc_checkpass_1608_hex(DEVHANDLE icdev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  ��ʼ��1608����֤��
   * @par    ˵����
   * ʹ�ô������������ʼ��1608����֤��
   * @param[in] icdev �豸��ʶ����
   * @param[in] databuffer ���ݣ��̶�Ϊ8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_init_auth_1608(DEVHANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  ��ʼ��1608����֤��
   * @par    ˵����
   * ::dc_init_auth_1608 ��HEX��ʽ�ӿڣ����� @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_init_auth_1608_hex(DEVHANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  �˶�1608����֤��
   * @par    ˵����
   * ʹ�ô�����������˶�1608����֤��
   * @param[in] icdev �豸��ʶ����
   * @param[in] databuffer ���ݣ��̶�Ϊ8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_check_auth_1608(DEVHANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  �˶�1608����֤��
   * @par    ˵����
   * ::dc_check_auth_1608 ��HEX��ʽ�ӿڣ����� @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_check_auth_1608_hex(DEVHANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  д1608����˿��
   * @par    ˵����
   * д1608����˿��
   * @param[in] icdev �豸��ʶ����
   * @param[in] value �������̶�Ϊ0x00��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_fuse_1608(DEVHANDLE icdev, unsigned char value);

  /**
   * @brief  ��1608����˿��
   * @par    ˵����
   * ��1608����˿��
   * @param[in] icdev �豸��ʶ����
   * @param[out] value ״ֵ̬��
   * @n bit0 - FAB��Atmel��˾������˿��0��ʾ����˿��1��ʾδ��˿��
   * @n bit1 - CMA���������̷�����˿��0��ʾ����˿��1��ʾδ��˿��
   * @n bit2 - PER�����˻�������˿��0��ʾ����˿��1��ʾδ��˿��
   * @n bit3~bit7 - ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_fuse_1608(DEVHANDLE icdev, unsigned char *value);

  /**
   * @brief  ���153����
   * @par    ˵����
   * ����Ƿ����153����
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܻ򲻴��ڣ�==0��ʾ���ڡ�
   */
  short USER_API dc_check_153(DEVHANDLE icdev);

  /**
   * @brief  153���µ硣
   * @par    ˵����
   * ��153�������µ������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_down_153(DEVHANDLE icdev);

  /**
   * @brief  ��153����
   * @par    ˵����
   * ��ȡ153�������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0~2 - ��ʾ�û�����
   * @n 3 - ��ʾ��������
   * @param[in] offset ƫ�ơ�
   * @param[in] length ��ȡ���ȡ�
   * @param[out] readdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_153(DEVHANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  ��153����
   * @par    ˵����
   * ::dc_read_153 ��HEX��ʽ�ӿڣ����� @a readdata ΪHEX��ʽ��
   */
  short USER_API dc_read_153_hex(DEVHANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  д153����
   * @par    ˵����
   * д�����ݵ�153���С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0~2 - ��ʾ�û�����
   * @n 3 - ��ʾ��������
   * @param[in] offset ƫ�ơ�
   * @param[in] length д�볤�ȡ�
   * @param[in] writedata �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_153(DEVHANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  д153����
   * @par    ˵����
   * ::dc_write_153 ��HEX��ʽ�ӿڣ����� @a writedata ΪHEX��ʽ��
   */
  short USER_API dc_write_153_hex(DEVHANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  ��֤153�����롣
   * @par    ˵����
   * ʹ�ô������������֤153�����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] zone ��������
   * @n 0~2 - ��ʾ�û�����
   * @n 3 - ��ʾ��������
   * @param[in] type ���ͣ�0��ʾ��֤д��Կ��1��ʾ��֤����Կ��
   * @param[in] password ���룬�̶�Ϊ3���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_checkpass_153(DEVHANDLE icdev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  ��֤153�����롣
   * @par    ˵����
   * ::dc_checkpass_153 ��HEX��ʽ�ӿڣ����� @a password ΪHEX��ʽ��
   */
  short USER_API dc_checkpass_153_hex(DEVHANDLE icdev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  ��ʼ��153����֤��
   * @par    ˵����
   * ʹ�ô������������ʼ��153����֤��
   * @param[in] icdev �豸��ʶ����
   * @param[in] databuffer ���ݣ��̶�Ϊ8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_init_auth_153(DEVHANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  ��ʼ��153����֤��
   * @par    ˵����
   * ::dc_init_auth_153 ��HEX��ʽ�ӿڣ����� @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_init_auth_153_hex(DEVHANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  �˶�153����֤��
   * @par    ˵����
   * ʹ�ô�����������˶�153����֤��
   * @param[in] icdev �豸��ʶ����
   * @param[in] databuffer ���ݣ��̶�Ϊ8���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_check_auth_153(DEVHANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  �˶�153����֤��
   * @par    ˵����
   * ::dc_check_auth_153 ��HEX��ʽ�ӿڣ����� @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_check_auth_153_hex(DEVHANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  д153����˿��
   * @par    ˵����
   * д153����˿��
   * @param[in] icdev �豸��ʶ����
   * @param[in] value �������̶�Ϊ0x00��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_write_fuse_153(DEVHANDLE icdev, unsigned char value);

  /**
   * @brief  ��153����˿��
   * @par    ˵����
   * ��153����˿��
   * @param[in] icdev �豸��ʶ����
   * @param[out] value ״ֵ̬��
   * @n bit0 - FAB��Atmel��˾������˿��0��ʾ����˿��1��ʾδ��˿��
   * @n bit1 - CMA���������̷�����˿��0��ʾ����˿��1��ʾδ��˿��
   * @n bit2 - PER�����˻�������˿��0��ʾ����˿��1��ʾδ��˿��
   * @n bit3~bit7 - ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_read_fuse_153(DEVHANDLE icdev, unsigned char *value);

  /**
   * @brief  ������������ֹͣ��
   * @par    ˵����
   * ������������ֹͣ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] flag ��־��0��ʾֹͣ��1��ʾ������
   * @param[in,out] mask ����ֵ��32bit�ֱ��ʾ32·���ߣ���ӦbitλΪ1��ʾ������Ч������Ϊ�ڴ�ֵ������Ϊʵ��ֵ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MultiAntennaStartOrStop(DEVHANDLE icdev, unsigned char flag, unsigned int *mask);

  /**
   * @brief  �����߻�ȡ��״̬��
   * @par    ˵����
   * �����߻�ȡ��״̬��
   * @param[in] icdev �豸��ʶ����
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @n 0x00 - �޿���
   * @n 0x8A - �п���Type A CPU����
   * @n 0x8B - �п���Type B CPU����
   * @n 0x80 - �п���δ֪���͡�
   * @n 0x81 - �п���M1����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_MultiAntennaGetCardStatus(DEVHANDLE icdev, int *rlen, unsigned char *rdata);

  /**
   * @brief  ����reader��������̴��ڲ����ʡ�
   * @par    ˵����
   * ����reader��������̴��ڲ����ʡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] value �����ʡ�
   * @n 0x00 - 2400��
   * @n 0x01 - 4800��
   * @n 0x02 - 9600��
   * @n 0x03 - 14400��
   * @n 0x04 - 38400��
   * @n 0x05 - 57600��
   * @n 0x06 - 115200��
   * @n 0x07 - 256000��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SetPinpadWithReaderBaudrate(DEVHANDLE icdev, unsigned char value);

  /**
   * @brief  ��ѯreader��������̴��ڲ����ʡ�
   * @par    ˵����
   * ��ѯreader��������̴��ڲ����ʡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] value �����ʡ�
   * @n 0x00 - 2400��
   * @n 0x01 - 4800��
   * @n 0x02 - 9600��
   * @n 0x03 - 14400��
   * @n 0x04 - 38400��
   * @n 0x05 - 57600��
   * @n 0x06 - 115200��
   * @n 0x07 - 256000��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CheckPinpadWithReaderBaudrate(DEVHANDLE icdev, unsigned char *value);

  /**
   * @brief  ������֤��
   * @par    ˵����
   * ��ȡ����֤��ԭʼ��Ϣ���ݣ��˽ӿ�ʹ��Ӳ�����٣���̼�֧�֡�
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��Ϊ0x00��ʾ���ƿ����ڲ�ʹ�ã���Ϊ0x01��ʾģ������֤����Ϊ0x02��ʾ��������֤����
   * @param[in] type ���ͣ�Ϊ0x00��ʾ�ڲ�ʹ�ã�����ֵΪ�����г���һ�ֻ����֮�͡�
   * @n 0x01 - ��ȡ������Ϣ��
   * @n 0x02 - ��ȡ��Ƭ��Ϣ��
   * @n 0x04 - ��ȡָ����Ϣ��
   * @n 0x08 - ��ȡ׷��סַ��Ϣ��
   * @param[out] text_len ����������Ϣ�ĳ��ȡ�
   * @param[out] text ���ص�������Ϣ�������ٷ���256���ֽڡ�
   * @param[out] photo_len ������Ƭ��Ϣ�ĳ��ȡ�
   * @param[out] photo ���ص���Ƭ��Ϣ�������ٷ���1024���ֽڡ�
   * @param[out] fingerprint_len ����ָ����Ϣ�ĳ��ȡ�
   * @param[out] fingerprint ���ص�ָ����Ϣ�������ٷ���1024���ֽڡ�
   * @param[out] extra_len ����׷��סַ��Ϣ�ĳ��ȡ�
   * @param[out] extra ���ص�׷��סַ��Ϣ�������ٷ���70���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_IdCardReadCardInfo(DEVHANDLE icdev, unsigned char mode, unsigned char type, int *text_len, unsigned char *text, int *photo_len, unsigned char *photo, int *fingerprint_len, unsigned char *fingerprint, int *extra_len, unsigned char *extra);

  /**
   * @brief  Һ����ʾ��
   * @par    ˵����
   * ��ʾָ���ַ�����Һ����Ļ�ϡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] line �кš�
   * @param[in] offset ƫ�ơ�
   * @param[in] mode ģʽ��Ϊ0x00��ʾ���ԣ�Ϊ0x01��ʾ���ԡ�
   * @param[in] data Ҫ��ʾ���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_LcdDisplay(DEVHANDLE icdev, unsigned char line, unsigned char offset, unsigned char mode, const char *data);

  /**
   * @brief  ���豸�������кš�
   * @par    ˵����
   * ���豸�������кš�
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ���0��ʼ��š�
   * @param[out] data ���к��ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadDeviceCustomSerialNumber(DEVHANDLE icdev, unsigned char number, char *data);

  /**
   * @brief  д�豸�������кš�
   * @par    ˵����
   * д�豸�������кš�
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ��ţ���0��ʼ��š�
   * @param[in] data ���к��ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WriteDeviceCustomSerialNumber(DEVHANDLE icdev, unsigned char number, const char *data);

  /**
   * @brief  ��ά���Զ��ϴ�ģʽ����/�رգ��µ粻����״̬����
   * @par    ˵����
   * ��ά���Զ��ϴ�ģʽ����/�رգ��µ粻����״̬����
   * @param[in] icdev �豸��ʶ����
   * @param[in] value ����ֵ��0x00��ʾ�رգ�0x01��ʾ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_2DBarcodeAutoModeOnOff(DEVHANDLE icdev, unsigned char value);

  /**
   * @brief  ������������
   * @par    ˵����
   * ��ȡ���������е����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�Ƶ�ַ��
   * @param[in] length ��ȡ���ȡ�
   * @param[out] data_buffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadTamperDeviceArea(DEVHANDLE icdev, int offset, int length, unsigned char *data_buffer);

  /**
   * @brief  д����������
   * @par    ˵����
   * д�����ݵ����������У������������ݱ���ȡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] offset ƫ�Ƶ�ַ��
   * @param[in] length д�볤�ȡ�
   * @param[in] data_buffer �������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WriteTamperDeviceArea(DEVHANDLE icdev, int offset, int length, const unsigned char *data_buffer);

  /**
   * @brief  ͨ����ʾ��ȡ��Ļ���ԡ�
   * @par    ˵����
   * ͨ����ʾ��ȡ��Ļ������ԡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���0x01��ʾ��ȡ��Ļ֧�ֵķֱ��ʺ���ɫλ��
   * @param[out] attribute_len �����������ݵĳ��ȡ�
   * @param[out] attribute ���ص��������ݡ�
   * @n typeΪ0x01ʱ�����ء�1���ֽ�֧�ֱַ��ʺ���ɫλ����Ŀ���������ֱ��ʺ���ɫλ1�����ֱ��ʺ���ɫλ2��...��ÿ���ֱ��ʺ���ɫλ��Ŀ�̶�Ϊ9���ֽڣ�����λ��0~3�ֽڣ�Big-Endian����ʾ��Ļ���ȣ����أ���λ��4~7�ֽڣ�Big-Endian����ʾ��Ļ�߶ȣ����أ���λ��8�ֽڱ�ʾ��Ļ����������ռ��λ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_DisplayGetScreenAttribute(DEVHANDLE icdev, unsigned char type, int *attribute_len, unsigned char *attribute);

  /**
   * @brief  ͨ����ʾ������Ļ���ơ�
   * @par    ˵����
   * ͨ����ʾ������Ļ���ƣ��˽ӿ�����׼��������Ļ����Դ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] width �ֱ��ʿ��ȡ�
   * @param[in] height �ֱ��ʸ߶ȡ�
   * @param[in] bpp ��ɫλ��
   * @n typeΪ0x01ʱ�����ء�1���ֽ�֧�ֱַ��ʺ���ɫλ����Ŀ���������ֱ��ʺ���ɫλ1�����ֱ��ʺ���ɫλ2��...��ÿ���ֱ��ʺ���ɫλ��Ŀ�̶�Ϊ9���ֽڣ�����λ��0~3�ֽڣ�Big-Endian����ʾ��Ļ���ȣ����أ���λ��4~7�ֽڣ�Big-Endian����ʾ��Ļ�߶ȣ����أ���λ��8�ֽڱ�ʾ��Ļ����������ռ��λ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_DisplayStartScreenDraw(DEVHANDLE icdev, unsigned int width, unsigned int height, unsigned char bpp);

  /**
   * @brief  ͨ����ʾ������Ļ��
   * @par    ˵����
   * ͨ����ʾ������Ļ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] x ��ʼX���ꡣ
   * @param[in] y ��ʼY���ꡣ
   * @param[in] w ���ȡ�
   * @param[in] h �߶ȡ�
   * @param[in] pixel_len �������ݵĳ��ȡ�
   * @param[in] pixel �������ݡ�
   * @param[in] flag ��־��0x00��ʾ��ˢ����ʾ���ڲ�ʹ����ʾ���棩��0x01��ʾ����ˢ����ʾ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_DisplayDrawScreen(DEVHANDLE icdev, unsigned int x, unsigned int y, unsigned int w, unsigned int h, int pixel_len, const unsigned char *pixel, unsigned char flag);

  /**
   * @brief  ͨ����ʾ�˳����Ʋ���ˢ����Ļ��
   * @par    ˵����
   * ͨ����ʾ�˳����Ʋ���ˢ����Ļ��
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_DisplayExitDrawAndflushScreen(DEVHANDLE icdev);

  /**
   * @brief  ������
   */
  short USER_API dc_YiHuaCryptoGetRandom(DEVHANDLE icdev, unsigned char type, unsigned char length, unsigned char *random_data);

  /**
   * @brief  ������
   */
  short USER_API dc_YiHuaCryptoExternalAuthenticate(DEVHANDLE icdev, unsigned char type, unsigned char length, const unsigned char *crypto_random_data);

  /**
   * @brief  ������
   */
  short USER_API dc_YiHuaCryptoPublicKeyImport(DEVHANDLE icdev, unsigned char type, unsigned char length, const unsigned char *public_key_data);

  /**
   * @brief  ������
   */
  short USER_API dc_YiHuaCryptoSessionKeyNegotiationInit(DEVHANDLE icdev, unsigned char type, unsigned char *cr1_len, unsigned char *cr1_data, unsigned char *cr2_len, unsigned char *cr2_data, unsigned char *cdpk_len, unsigned char *cdpk_data);

  /**
   * @brief  ������
   */
  short USER_API dc_YiHuaCryptoSessionKeyGen(DEVHANDLE icdev, unsigned char type, unsigned char length, const unsigned char *crypto_random_data);

  /**
   * @brief  ������
   */
  short USER_API dc_YiHuaCryptoExternalAuthenticateKeyUpdate(DEVHANDLE icdev, unsigned char type, unsigned char length, const unsigned char *crypto_data);

  /**
   * @brief  ������
   */
  short USER_API dc_YiHuaCryptoReadMag(DEVHANDLE icdev, unsigned char type, unsigned char *t1_len, unsigned char *t1_data, unsigned char *t2_len, unsigned char *t2_data, unsigned char *t3_len, unsigned char *t3_data);

  /**
   * @brief  ������
   */
  short USER_API dc_YiHuaCryptoIcApdu(DEVHANDLE icdev, unsigned char type, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  ������
   */
  short USER_API dc_YiHuaCryptoRfApdu(DEVHANDLE icdev, unsigned char type, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  ������
   */
  short USER_API dc_YiHuaExportSessionKey(DEVHANDLE icdev, unsigned char type, int *sk_len, unsigned char *sk_data);

  /**
   * @brief  15693��͸��ָ�
   * @par    ˵����
   * 15693��͸��ָ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] time_ms �豸��ʱֵ����λΪ���롣
   * @param[in] in_data �������ݡ�
   * @param[in] in_len �������ݵĳ��ȡ�
   * @param[out] out_data ������ݡ�
   * @param[out] out_len ������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_Iso15693Communication(DEVHANDLE icdev, int time_ms, const unsigned char *in_data, int in_len, unsigned char *out_data, int *out_len);

  /**
   * @brief  ��Ƶ�������á�
   * @par    ˵����
   * ��Ƶ�������á�
   * @param[in] icdev �豸��ʶ����
   * @param[in,out] flags ���ͱ�־����Ϊ�������ֵ��λ����һ��
   * @n 0x00 - ȫ�������á�
   * @n 0x01 - ����rf_gain��Ч��
   * @n 0x02 - ����minimum_power��Ч��
   * @n 0x04 - ����couple_power��Ч��
   * @n 0x08 - ����modulation_depth��Ч��
   * @n 0x10 - ����waveform_adjustment��Ч��
   * @n 0x20 - ����rf_strength��Ч��
   * @param[in] rf_gain ��Ƶ�����źŷŴ�
   * @param[in] minimum_power ��С���ʡ�
   * @param[in] couple_power ��Ϲ��ʡ�
   * @param[in] modulation_depth ������ȡ�
   * @param[in] waveform_adjustment ���ε�����
   * @param[in] rf_strength ��Ƶ�����ǿ��С��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SetRfAttribute(DEVHANDLE icdev, unsigned short *flags, unsigned char rf_gain, unsigned char minimum_power, unsigned char couple_power, unsigned char modulation_depth, unsigned char waveform_adjustment, unsigned char rf_strength);

  /**
   * @brief  ��Ƶ������ȡ��
   * @par    ˵����
   * ��Ƶ������ȡ��
   * @param[in] icdev �豸��ʶ����
   * @param[in,out] flags ���ͱ�־����Ϊ�������ֵ��λ����һ��
   * @n 0x00 - ȫ�������á�
   * @n 0x01 - ����rf_gain��Ч��
   * @n 0x02 - ����minimum_power��Ч��
   * @n 0x04 - ����couple_power��Ч��
   * @n 0x08 - ����modulation_depth��Ч��
   * @n 0x10 - ����waveform_adjustment��Ч��
   * @n 0x20 - ����rf_strength��Ч��
   * @param[out] rf_gain ��Ƶ�����źŷŴ�
   * @param[out] minimum_power ��С���ʡ�
   * @param[out] couple_power ��Ϲ��ʡ�
   * @param[out] modulation_depth ������ȡ�
   * @param[out] waveform_adjustment ���ε�����
   * @param[out] rf_strength ��Ƶ�����ǿ��С��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetRfAttribute(DEVHANDLE icdev, unsigned short *flags, unsigned char *rf_gain, unsigned char *minimum_power, unsigned char *couple_power, unsigned char *modulation_depth, unsigned char *waveform_adjustment, unsigned char *rf_strength);

  /**
   * @brief  LCD��ʾ������
   * @par    ˵����
   * LCD��ʾ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ��ʽ���ͣ�0x00��ʾXML��ʽ��0x01��ʾTLV��ʽ��0x02��ʾJPEG��ʽ��
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sdata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_LcdDisplayInteraction(DEVHANDLE icdev, unsigned char type, unsigned int slen, const unsigned char *sdata, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  LCD��ʾ��������ѯ״̬����
   * @par    ˵����
   * LCD��ʾ��������ѯ״̬����
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ��ʽ���ͣ�0x00��ʾXML��ʽ��0x01��ʾTLV��ʽ��0x02��ʾJPEG��ʽ��
   * @param[in] id ����Ψһ��ʶһ���������ݵĴ��䡣
   * @param[in] size_of_processed_pc_data ��ʾPC�Ѵ������ݵ��ܴ�С����λ���ֽڣ���
   * @param[out] size_of_processed_device_data ��ʾDEVICE�Ѵ������ݵ��ܴ�С����λ���ֽڣ���
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_LcdDisplayInteractionQueryState(DEVHANDLE icdev, unsigned char type, unsigned int id, unsigned int size_of_processed_pc_data, unsigned int *size_of_processed_device_data);

  /**
   * @brief  LCD��ʾ����������PC��DEVICE���ݴ��䣩��
   * @par    ˵����
   * LCD��ʾ����������PC��DEVICE���ݴ��䣩��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ��ʽ���ͣ�0x00��ʾXML��ʽ��0x01��ʾTLV��ʽ��0x02��ʾJPEG��ʽ��
   * @param[in] id ����Ψһ��ʶһ���������ݵĴ��䡣
   * @param[in] pc_capacity_size ��ʾPCÿ֡�д������ݵ����ֵ����λ���ֽڣ���
   * @param[in] data_size Ҫ�������ݵ��ܴ�С����λ���ֽڣ���
   * @param[out] device_capacity_size ��ʾDEVICEÿ֡�д������ݵ����ֵ����λ���ֽڣ���
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_LcdDisplayInteractionPcToDeviceRequest(DEVHANDLE icdev, unsigned char type, unsigned int id, unsigned int pc_capacity_size, unsigned int data_size, unsigned int *device_capacity_size);

  /**
   * @brief  LCD��ʾ����������PC��DEVICE���ݴ��䣩��
   * @par    ˵����
   * LCD��ʾ����������PC��DEVICE���ݴ��䣩��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ��ʽ���ͣ�0x00��ʾXML��ʽ��0x01��ʾTLV��ʽ��0x02��ʾJPEG��ʽ��
   * @param[in] id ����Ψһ��ʶһ���������ݵĴ��䡣
   * @param[in,out] offset ����PCҪ�������ݵ�ƫ��������λ���ֽڣ����ҷ���DEVICEʵ�ʽ������ݵ�ƫ��������λ���ֽڣ���
   * @param[in,out] length ����PCҪ�������ݵĳ��ȣ���λ���ֽڣ����ҷ���DEVICEʵ�ʽ������ݵĳ��ȣ���λ���ֽڣ���
   * @param[in] data ����PCҪ���͵����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_LcdDisplayInteractionPcToDeviceRun(DEVHANDLE icdev, unsigned char type, unsigned int id, unsigned int *offset, unsigned int *length, const unsigned char *data);

  /**
   * @brief  LCD��ʾ����������DEVICE��PC���ݴ��䣩��
   * @par    ˵����
   * LCD��ʾ����������DEVICE��PC���ݴ��䣩��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ��ʽ���ͣ�0x00��ʾXML��ʽ��0x01��ʾTLV��ʽ��0x02��ʾJPEG��ʽ��
   * @param[in] id ����Ψһ��ʶһ���������ݵĴ��䡣
   * @param[in] pc_capacity_size ��ʾPCÿ֡�д������ݵ����ֵ����λ���ֽڣ���
   * @param[out] device_capacity_size ��ʾDEVICEÿ֡�д������ݵ����ֵ����λ���ֽڣ���
   * @param[out] data_size Ҫ�������ݵ��ܴ�С����λ���ֽڣ���
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_LcdDisplayInteractionDeviceToPcRequest(DEVHANDLE icdev, unsigned char type, unsigned int id, unsigned int pc_capacity_size, unsigned int *device_capacity_size, unsigned int *data_size);

  /**
   * @brief  LCD��ʾ����������DEVICE��PC���ݴ��䣩��
   * @par    ˵����
   * LCD��ʾ����������DEVICE��PC���ݴ��䣩��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ��ʽ���ͣ�0x00��ʾXML��ʽ��0x01��ʾTLV��ʽ��0x02��ʾJPEG��ʽ��
   * @param[in] id ����Ψһ��ʶһ���������ݵĴ��䡣
   * @param[in,out] offset ����PCҪ�������ݵ�ƫ��������λ���ֽڣ����ҷ���DEVICEʵ�ʷ������ݵ�ƫ��������λ���ֽڣ���
   * @param[in,out] length ����PCҪ�������ݵĳ��ȣ���λ���ֽڣ����ҷ���DEVICEʵ�ʷ������ݵĳ��ȣ���λ���ֽڣ���
   * @param[out] data ����DEVICEʵ�ʷ��͵����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_LcdDisplayInteractionDeviceToPcRun(DEVHANDLE icdev, unsigned char type, unsigned int id, unsigned int *offset, unsigned int *length, unsigned char *data);

  /**
   * @brief  FeliCa����λ��
   * @par    ˵����
   * FeliCa����λ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] uid_len ����UID�ĳ��ȡ�
   * @param[out] uid_data ���ص�UID�������ٷ���64���ֽڡ�
   * @param[out] ats_len ���ظ�λ��Ϣ�ĳ��ȡ�
   * @param[out] ats_data ���صĸ�λ��Ϣ�������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_FeliCaReset(DEVHANDLE icdev, unsigned char *uid_len, unsigned char *uid_data, unsigned char *ats_len, unsigned char *ats_data);

  /**
   * @brief  FeliCa��ָ�����
   * @par    ˵����
   * FeliCa��ָ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sdata �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_FeliCaApdu(DEVHANDLE icdev, unsigned int slen, const unsigned char *sdata, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  ��ȡ������ԭʼ���ݡ�
   * @par    ˵����
   * ��ȡ������ԭʼ���ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��0��ʾԭʼ���ݣ�1��ʾ��������������2��ʾISO7811Э��������ݡ�
   * @param[in] time_ms �豸��ʱֵ����λΪ���롣
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] rdata ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadRawMagData(DEVHANDLE icdev, int mode, int time_ms, int *rlen, unsigned char *rdata);

  /**
   * @brief  ��ȡ���ܰ������ݡ�
   * @par    ˵����
   * ��������̻�ȡANSI X9.8 PIN BLOCK���ܵİ������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] ctime �豸��ʱֵ����λΪ�롣
   * @param[in] panlen ���ŵĳ��ȡ�
   * @param[in] pandata ���š�
   * @param[in] keylen ��Կ�ĳ��ȡ�
   * @param[in] keydata ��Կ��
   * @param[out] rlen ���ؼ������ݵĳ��ȡ�
   * @param[out] rdata ���صļ������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���==0xA1��ʾȡ�����룬==0xA2��ʾ���볬ʱ��
   */
  short USER_API dc_GetEnPassAddr(DEVHANDLE icdev, unsigned char ctime, unsigned char panlen, unsigned char *pandata, unsigned char keylen, unsigned char *keydata, unsigned char *rlen, unsigned char *rdata);

  /**
   * @brief  ����״̬��ѯ��
   * @par    ˵����
   * ����״̬��ѯ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ������ƣ�Ŀǰ�̶�Ϊ0x01��
   * @param[in] in_data �������ݡ�
   * @param[in] in_len �������ݵĳ��ȡ�
   * @param[out] out_data ������ݡ�
   * @param[out] out_len ������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetTamperStatus(DEVHANDLE icdev, unsigned char mode, const unsigned char *in_data, int in_len, unsigned char *out_data, int *out_len);

  /**
   * @brief  ����������á�
   * @par    ˵����
   * ����������á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ������ƣ�Ŀǰ�̶�Ϊ0x01��
   * @param[in] item ������Ŀ��0x01��ʾ��������
   * @param[in] in_data �������ݡ�
   * @param[in] in_len �������ݵĳ��ȡ�
   * @param[out] out_data ������ݡ�
   * @param[out] out_len ������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SetTamperSense(DEVHANDLE icdev, unsigned char mode, unsigned char item, const unsigned char *in_data, int in_len, unsigned char *out_data, int *out_len);

  /**
   * @brief  ��ȡ�ⲿģ��汾�š�
   * @par    ˵����
   * ��ȡ�ⲿģ��汾�š�
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ģ����ţ���0��ʼ��š�
   * @param[out] version ���صİ汾�ַ����������ٷ���128���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetModuleVersion(DEVHANDLE icdev, int number, char *version);

  /**
   * @brief  SSL֤�������
   * @par    ˵����
   * ֱ�Ӷ�����ģ���ڲ��ļ����в�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ����ģʽ��
   * @n 0x00 - ֤�����ء�
   * @n 0x01 - ֤���ѯ��
   * @n 0x02 - ֤��ɾ����
   * @param[in] type ������ģʽΪ֤������ʱ��Ч��
   * @n 0x00 - ֤�����س�ʼ����
   * @n 0x01 - ֤�����ء�
   * @n 0x02 - ֤�����ؽ�����
   * @param[in] in_data �������ݡ�
   * @n ��mode==0x00ʱ�����type==0x00�����·�֤��洢·����type==0x01����֤��ʵ�����ݣ�type==0x02�����ڡ�
   * @n ��mode==0x01ʱ�������ڡ�
   * @n ��mode==0x02ʱ��֤��洢��·����
   * @param[in] in_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SslCertificateControl(DEVHANDLE icdev, unsigned char mode, unsigned char type, const unsigned char *in_data, int in_len);

  /**
   * @brief  ����������á�
   * @par    ˵����
   * �����������(TCP(S)/HTTP(S))��
   * @param[in] icdev �豸��ʶ����
   * @param[in] channel_number ����ͨ����
   * @n 0x00 - TCP(s)ͨ����
   * @n 0x01 - HTTP(s)ͨ����
   * @param[in] type ����Э�����͡�
   * @n 0x00 - TCP��
   * @n 0x01 - SSL��
   * @n 0x02 - HTTP��
   * @n 0x03 - HTTPS��
   * @param[in] mode ����ģʽ��
   * @n ��type==0x00��type==0x01ʱ��0x01��ʾ�����ӣ�0x02��ʾ�����ӡ�
   * @n ��type==0x02��type==0x03ʱ��0x00��ʾPOST��ʽ��0x01��ʾGET��ʽ��
   * @param[in] link_timeout_s ���ӷ�������ʱʱ�䣬��λΪ�롣
   * @param[in] response_timeout_s ������Ӧ��ʱʱ�䣬��λΪ�롣
   * @param[in] long_link_time_s �����ӱ���ʱ�䣬��λΪ�롣
   * @param[in] ip_name ��������IP��������ַ���磺"192.168.10.123"��"www.baidu.com"��
   * @param[in] ip_port IP�˿ںš�
   * @param[in] root_cert_path ��֤��·����Ϊ""��ʾ����Ҫ��
   * @param[in] client_cert_path �ͻ���֤��·����Ϊ""��ʾ����Ҫ��
   * @param[in] client_key_path �ͻ�����Կ·����Ϊ""��ʾ����Ҫ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_NetParameterConfig(DEVHANDLE icdev, unsigned char channel_number, unsigned char type, unsigned char mode, unsigned char link_timeout_s, unsigned char response_timeout_s, unsigned int long_link_time_s, const char *ip_name, unsigned short ip_port, const char *root_cert_path, const char *client_cert_path, const char *client_key_path);

  /**
   * @brief  ���������ӡ�
   * @par    ˵����
   * ���������ӡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] channel_number ����ͨ����
   * @n 0x00 - TCP(s)ͨ����
   * @n 0x01 - HTTP(s)ͨ����
   * @param[in] reserved �������̶�Ϊ0x00��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_OpenNetLink(DEVHANDLE icdev, unsigned char channel_number, unsigned char reserved);

  /**
   * @brief  �ر��������ӡ�
   * @par    ˵����
   * �ر��������ӡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] channel_number ����ͨ����
   * @n 0x00 - TCP(s)ͨ����
   * @n 0x01 - HTTP(s)ͨ����
   * @param[in] reserved �������̶�Ϊ0x00��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CloseNetLink(DEVHANDLE icdev, unsigned char channel_number, unsigned char reserved);

  /**
   * @brief  ��������������ݡ�
   * @par    ˵����
   * ��������������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] channel_number ����ͨ����
   * @n 0x00 - TCP(s)ͨ����
   * @n 0x01 - HTTP(s)ͨ����
   * @param[in] in_data �������ݡ�
   * @param[in] in_len �������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SendDataToServer(DEVHANDLE icdev, unsigned char channel_number, const unsigned char *in_data, int in_len);

  /**
   * @brief  �ӷ������������ݡ�
   * @par    ˵����
   * �ӷ������������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] channel_number ����ͨ����
   * @n 0x00 - TCP(s)ͨ����
   * @n 0x01 - HTTP(s)ͨ����
   * @param[in,out] state ����������ҷ���״̬������ʱ��0x00��ʾ������0x01��ʾ�����ݣ�0x02��ʾ�˳�������ʱ��0x00��ʾ��û�н��յ����������ݣ�0x01��ʾģ�黹�ж������ݣ�0x02ģ������ȡ���ˡ�
   * @param[out] out_data ������ݡ�
   * @param[out] out_len ������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_RecvDataFromServer(DEVHANDLE icdev, unsigned char channel_number, unsigned char *state, unsigned char *out_data, int *out_len);

  /**
   * @brief  ���ö�д���ͺš�
   * @par    ˵����
   * ���ö�д���ͺš�
   * @param[in] icdev �豸��ʶ����
   * @param[in] model �����ͺţ����ݸ�ʽΪ'\0'��β���ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SetReaderModel(DEVHANDLE icdev, const char *model);

  /**
   * @brief  ��ȡ��д���ͺš�
   * @par    ˵����
   * ��ȡ��д���ͺš�
   * @param[in] icdev �豸��ʶ����
   * @param[out] model ���ص��ͺ��ַ����������ٷ���33���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetReaderModel(DEVHANDLE icdev, char *model);

  /**
   * @brief  ��������������
   * @par    ˵����
   * ʹ�豸������ˢ������״̬���ӿڱ�����ֱ�ӷ��أ�������ȴ�ˢ���������
   * @param[in] icdev �豸��ʶ����
   * @param[in] mode ģʽ��
   * @n 0x01 - ��ʾ�ſ�ԭʼ��������������룬����ʹ�� ::dc_readmag2 ��ȡ���������ݡ�
   * @n 0x02 - ��ʾ�ſ�7811Э��������ݣ�����ʹ�� ::dc_readmag ��ȡ���������ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_startreadmag2(DEVHANDLE icdev, unsigned char mode);

  /**
   * @brief  ��ȡ���������ݡ�
   * @par    ˵����
   * ��ȡ�豸�Ѿ���ȡ���Ĵ��������ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] t1pp_data ���ص�1���ͬ���������ݡ�
   * @param[out] t1pp_len ����1���ͬ���������ݵĳ��ȡ�
   * @param[out] t1pn_data ���ص�1���ͬ�򷴽����ݡ�
   * @param[out] t1pn_len ����1���ͬ�򷴽����ݵĳ��ȡ�
   * @param[out] t1np_data ���ص�1��������������ݡ�
   * @param[out] t1np_len ����1���ͬ���������ݵĳ��ȡ�
   * @param[out] t1nn_data ���ص�1������򷴽����ݡ�
   * @param[out] t1nn_len ����1������򷴽����ݵĳ��ȡ�
   * @param[out] t2pp_data ���ص�2���ͬ���������ݡ�
   * @param[out] t2pp_len ����2���ͬ���������ݵĳ��ȡ�
   * @param[out] t2pn_data ���ص�2���ͬ�򷴽����ݡ�
   * @param[out] t2pn_len ����2���ͬ�򷴽����ݵĳ��ȡ�
   * @param[out] t2np_data ���ص�2��������������ݡ�
   * @param[out] t2np_len ����2���ͬ���������ݵĳ��ȡ�
   * @param[out] t2nn_data ���ص�2������򷴽����ݡ�
   * @param[out] t2nn_len ����2������򷴽����ݵĳ��ȡ�
   * @param[out] t3pp_data ���ص�3���ͬ���������ݡ�
   * @param[out] t3pp_len ����3���ͬ���������ݵĳ��ȡ�
   * @param[out] t3pn_data ���ص�3���ͬ�򷴽����ݡ�
   * @param[out] t3pn_len ����3���ͬ�򷴽����ݵĳ��ȡ�
   * @param[out] t3np_data ���ص�3��������������ݡ�
   * @param[out] t3np_len ����3���ͬ���������ݵĳ��ȡ�
   * @param[out] t3nn_data ���ص�3������򷴽����ݡ�
   * @param[out] t3nn_len ����3������򷴽����ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_readmag2(DEVHANDLE icdev, unsigned char *t1pp_data, unsigned int *t1pp_len, unsigned char *t1pn_data, unsigned int *t1pn_len, unsigned char *t1np_data, unsigned int *t1np_len, unsigned char *t1nn_data, unsigned int *t1nn_len, unsigned char *t2pp_data, unsigned int *t2pp_len, unsigned char *t2pn_data, unsigned int *t2pn_len, unsigned char *t2np_data, unsigned int *t2np_len, unsigned char *t2nn_data, unsigned int *t2nn_len, unsigned char *t3pp_data, unsigned int *t3pp_len, unsigned char *t3pn_data, unsigned int *t3pn_len, unsigned char *t3np_data, unsigned int *t3np_len, unsigned char *t3nn_data, unsigned int *t3nn_len);

  /**
   * @brief  �綯������EM3��ʼ����
   * @par    ˵����
   * �綯������EM3��ʼ����
   * @param[in] icdev �豸��ʶ����
   * @param[in] gate_check_mode ǰ�ŵ�բ�ż��ģʽ��
   * @n 0x00 - ��ʾ����⡣
   * @n 0x01 - ��ʾ��⡣
   * @param[in] card_move_mode ��ʼ�����ڲ��п�ʱ���ƶ�ģʽ��
   * @n 0x00 - ������ǰ�żп�״̬��
   * @n 0x01 - ������ǰ�Ų��п�״̬��
   * @n 0x02 - �̿���
   * @n 0x03 - ���ƶ��������ڲ���
   * @param[in] request_mode ��������ģ������ʶ���뷵�ؿ���ģʽ��
   * @n 0x00 - ��ʾ�����ؿ�������ģ������ʶ���롣
   * @n 0x01 - ��ʾ���ؿ�������ģ������ʶ���롣
   * @param[out] out_data ������ݣ�������ʱ�������¡�
   * @n byte0 - SAM1�����Ƿ��п���0x00��ʾ�޿���0x01��ʾ�п���
   * @n byte1 - SAM2�����Ƿ��п���0x00��ʾ�޿���0x01��ʾ�п���
   * @n byte2 - SAM3�����Ƿ��п���0x00��ʾ�޿���0x01��ʾ�п���
   * @n byte3 - SAM4�����Ƿ��п���0x00��ʾ�޿���0x01��ʾ�п���
   * @n byte4 - Ӳ���Ƿ���IC���װ�ã�0x00��ʾ�ޣ�0x01��ʾ�С�
   * @n byte5 - Ӳ���Ƿ��з�����װ�ã�0x00��ʾ�ޣ�0x01��ʾ�С�
   * @n byte6 - Ӳ���Ƿ��дſ�����װ�ã�0x00��ʾ�ޣ�0x01��ʾ�С�
   * @n byte7 - Ӳ���Ƿ�������֤ģ��װ�ã�0x00��ʾ�ޣ�0x01��ʾ�С�
   * @param[out] out_len ������ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceInit(DEVHANDLE icdev, unsigned char gate_check_mode, unsigned char card_move_mode, unsigned char request_mode, unsigned char *out_data, int *out_len);

  /**
   * @brief  �綯����������д���ٶȿ��ơ�
   * @par    ˵����
   * �綯����������д���ٶȿ��ơ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] value �ٶ�ֵ����Χ1~99��ֵԽ��Խ�졣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SelfServiceDeviceWriteMagSpeed(DEVHANDLE icdev, unsigned char value);

  /**
   * @brief  ���������ź������
   * @par    ˵����
   * ���������ź������
   * @param[in] icdev �豸��ʶ����
   * @param[in] on_off ���ƿ������߹رգ�0x00��ʾ�رգ�0x01��ʾ������
   * @param[in] occupation_ratio ռ�ձȣ��ٷֱȷ�Χ0~100��
   * @param[in] frequency ����Ƶ�ʣ���λΪHz��
   * @param[in] output_time_ms ���������ʱ�䣬��λΪ���룬0��ʾһֱ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_PulseSignalControl(DEVHANDLE icdev, unsigned char on_off, unsigned char occupation_ratio, unsigned int frequency, unsigned int output_time_ms);

  /**
   * @brief  ��ȡ�¶ȡ�
   * @par    ˵����
   * ��ȡ�¶ȡ�
   * @param[in] icdev �豸��ʶ����
   * @param[out] average_value ƽ���¶ȡ�
   * @param[out] maximum_value ����¶ȡ�
   * @param[out] minimum_value ����¶ȡ�
   * @param[out] ambient_value �����¶ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_TakeTemperatureValues(DEVHANDLE icdev, unsigned short *average_value, unsigned short *maximum_value, unsigned short *minimum_value, unsigned short *ambient_value);

  /**
   * @brief  �ⲿģ���Դ���ơ�
   * @par    ˵����
   * ���ڶ��ⲿģ��ĵ�Դ�����ϵ���µ���ơ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] module_number ģ���š�
   * @n 0x0001 - �������裬�磺SPI��FLASH����������ָʾ�Ƶȡ�
   * @n 0x0101 - Androidģ�顣
   * @param[in] value ֵ��0x00��ʾ�µ磬0x01��ʾ�ϵ硣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ModulePowerControl(DEVHANDLE icdev, unsigned short module_number, unsigned char value);

  /**
   * @brief  �ⲿģ�����������ơ�
   * @par    ˵����
   * ���ڿ�����رռ���ⲿģ�����������ܣ��Ա��豸֪������ģ���Ƿ������������С�
   * @param[in] icdev �豸��ʶ����
   * @param[in] module_number ģ���š�
   * @n 0x0001 - �������裬�磺SPI��FLASH����������ָʾ�Ƶȡ�
   * @n 0x0101 - Androidģ�顣
   * @param[in] value ֵ��0x00��ʾ�رգ�0x01��ʾ������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ModuleHeartbeatPacketControl(DEVHANDLE icdev, unsigned short module_number, unsigned char value);

  /**
   * @brief  ֪ͨ��ҵ��APP����״̬��
   * @par    ˵����
   * ��ҵ��APP�ô˽ӿ�����֪�豸��ǰ�Լ�������״̬��
   * @param[in] icdev �豸��ʶ����
   * @param[in] value ֵ��0x00��ʾAPP�˳���0x01��ʾAPP������
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_NotifyMainAppState(DEVHANDLE icdev, unsigned char value);

  /**
   * @brief  ������
   */
  short USER_API DC_GetCard_CsnCarId(DEVHANDLE icdev, char *cCardNo, char *cCardType);

  /**
   * @brief  ������
   */
  short USER_API DC_GetCard_Check(DEVHANDLE icdev, char *cIn, char *cOut);

  /**
   * @brief  �ַ���ת����
   * @par    ˵����
   * �ַ���ת����
   * @param[in] type ���͡�
   * @n 0 - GBK->UCS-2LE��
   * @n 1 - UCS-2LE->GBK��
   * @n 2 - GBK->UTF-8��
   * @n 3 - UTF-8->GBK��
   * @n 4 - UCS-2LE->UTF-8��
   * @n 5 - UTF-8->UCS-2LE��
   * @param[in] in_str �����ַ�����
   * @param[out] out_str ����ַ�����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_string_converter(int type, const unsigned char *in_str, unsigned char *out_str);

  /**
   * @brief  �������ͨѶ��
   * @par    ˵����
   * ͨ���˽ӿڵ��ã�������Ϣ��ʹ���豸������̽ӿڣ����ϼ����豸�ӿڣ������Ӧ�����ݡ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] param_data ����������ݣ��ɲ������ݵĳ��Ⱦ���������Щ�����������ڵĲ���ʹ���豸����ֵ������������ֵ��ʹ��ȱʡֵ�������г�����ʱ�Ķ��塣
   * @n param_data[0] - �Զ��ϴ��Ƿ���лس�����0��ʾ�����س�����ȱʡֵ����1��ʾ���س�����
   * @n param_data[1] - �Զ��ϴ�����ģʽ��0��ʾ�����ģʽ��ȱʡֵ����1��ʾ��д���ģʽ����ʮ�����ƴ�д�ַ����ݣ���2��ʾСд���ģʽ����ʮ������Сд�ַ����ݣ���
   * @param[in] param_len ����������ݵĳ��ȡ�
   * @param[in] key_data �����ֵ���ݡ�
   * @param[in] key_len �����ֵ���ݵĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_VirtualKeyOutputControl(DEVHANDLE icdev, const unsigned char *param_data, unsigned char param_len, const unsigned char *key_data, unsigned short key_len);

  /**
   * @brief  Android��Դ��ʱ���ơ�
   * @par    ˵����
   * Android��Դ��ʱ���ơ�
   * @param[in] icdev �豸��ʶ����
   * @param[in] power_switch ��Դ���أ�0x00��ʾ�µ磬0x01��ʾ�ϵ硣
   * @param[in] time_s ��ʱʱ�䣬��λΪ�롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_AndroidTimingControl(DEVHANDLE icdev, unsigned char power_switch, unsigned char time_s);

  /**
   * @brief  ��ȡ���ڲ����ʡ�
   * @par    ˵����
   * ��ȡ���ڲ����ʣ������ô洢ֵ��
   * @param[in] icdev �豸��ʶ����
   * @param[out] value �����ʡ�
   * @n 0x00 - 115200��
   * @n 0x01 - 57600��
   * @n 0x02 - 38400��
   * @n 0x03 - 19200��
   * @n 0x04 - 9600��
   * @n 0x05 - 4800��
   * @n 0x06 - 2400��
   * @n 0x07 - 1200��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetCommBaudrate(DEVHANDLE icdev, unsigned char *value);

  /**
   * @brief  ���ô��ڲ����ʡ�
   * @par    ˵����
   * ���ô��ڲ����ʣ������ô洢ֵ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] value �����ʡ�
   * @n 0x00 - 115200��
   * @n 0x01 - 57600��
   * @n 0x02 - 38400��
   * @n 0x03 - 19200��
   * @n 0x04 - 9600��
   * @n 0x05 - 4800��
   * @n 0x06 - 2400��
   * @n 0x07 - 1200��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SetCommBaudrate(DEVHANDLE icdev, unsigned char value);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * �Ե�ǰ����CPU������ָ���������ע��˽ӿ��ѷ�װ��Э�鲿�ֲ���ǿ��ʹ��ָ����Э����н�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ��ƬЭ�顣
   * @param[in] slen �������ݵĳ��ȡ�
   * @param[in] sendbuffer �������ݡ�
   * @param[out] rlen �������ݵĳ��ȡ�
   * @param[out] databuffer ���ص����ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_TransmitApdu(DEVHANDLE icdev, unsigned char type, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  �Ӵ�ʽCPU��ָ�����
   * @par    ˵����
   * ::dc_TransmitApdu ��HEX��ʽ�ӿڣ����� @a sendbuffer @a databuffer ΪHEX��ʽ��
   */
  short USER_API dc_TransmitApdu_hex(DEVHANDLE icdev, unsigned char type, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  дTLV�û����á�
   * @par    ˵����
   * дTLV�û����á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] tlv_data_len TLV���ݵĳ��ȡ�
   * @param[in] tlv_data TLV���ݡ�
   * @param[out] tag_count Tag������
   * @param[out] tag_info Tag��Ϣ��
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_WriteTlvUserConfig(DEVHANDLE icdev, unsigned int tlv_data_len, const unsigned char *tlv_data, int *tag_count, unsigned int *tag_info);

  /**
   * @brief  ��TLV�û����á�
   * @par    ˵����
   * ��TLV�û����á�
   * @param[in] icdev �豸��ʶ����
   * @param[in] tag_count Tag������
   * @param[in] tag_info Tag��Ϣ��
   * @param[out] tlv_data_len TLV���ݵĳ��ȡ�
   * @param[out] tlv_data TLV���ݡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ReadTlvUserConfig(DEVHANDLE icdev, int tag_count, const unsigned int *tag_info, unsigned int *tlv_data_len, unsigned char *tlv_data);

  /**
   * @brief  ��ȡ�籣��������Ϣ��
   * @par    ˵����
   * ��ȡ�籣��������Ϣ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0��ʾ�ǽӴ�ʽ��1��ʾ�Ӵ�ʽ��
   * @param[out] card_code ����ʶ���룬�����ٷ���33���ֽڡ�
   * @param[out] card_type ������������ٷ���2���ֽڡ�
   * @param[out] version �淶�汾�������ٷ���5���ֽڡ�
   * @param[out] init_org_number ��ʼ��������ţ������ٷ���25���ֽڡ�
   * @param[out] card_issue_date �������ڣ������ٷ���9���ֽڡ�
   * @param[out] card_expire_day ����Ч�ڣ������ٷ���9���ֽڡ�
   * @param[out] card_number ���ţ������ٷ���10���ֽڡ�
   * @param[out] social_security_number ��ᱣ�Ϻ��룬�����ٷ���19���ֽڡ�
   * @param[out] name �����������ٷ���31���ֽڡ�
   * @param[out] name_ex ������չ�������ٷ���21���ֽڡ�
   * @param[out] sex �Ա������ٷ���2���ֽڡ�
   * @param[out] nation ���壬�����ٷ���3���ֽڡ�
   * @param[out] birth_place �����أ������ٷ���7���ֽڡ�
   * @param[out] birth_day �������ڣ������ٷ���9���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetSocialSecurityCardBaseInfo(DEVHANDLE icdev, int type, char *card_code, char *card_type, char *version, char *init_org_number, char *card_issue_date, char *card_expire_day, char *card_number, char *social_security_number, char *name, char *name_ex, char *sex, char *nation, char *birth_place, char *birth_day);

  /**
   * @brief  �޸��籣���Ŀ����롣
   * @par    ˵����
   * �޸��籣���Ŀ����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0��ʾ�ǽӴ�ʽ��1��ʾ�Ӵ�ʽ��
   * @param[in] old_password �����롣
   * @param[in] new_password �����롣
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_ChangeSocialSecurityCardPassword(DEVHANDLE icdev, int type, const char *old_password, const char *new_password);

  /**
   * @brief  ��ȡ���п��ʻ����롣
   * @par    ˵����
   * ��ȡ���п��ʻ����롣
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0��ʾ�ǽӴ�ʽ��1��ʾ�Ӵ�ʽ��
   * @param[out] number ���ص����п��ʻ������ַ����������ٷ���64���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetBankAccountNumber(DEVHANDLE icdev, int type, char *number);

  /**
   * @brief  ��ȡ���п�������Ϣ��
   * @par    ˵����
   * ��ȡ���п�������Ϣ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0��ʾ�ǽӴ�ʽ��1��ʾ�Ӵ�ʽ��
   * @param[out] track1 ���ص����п��������1��Ϣ�ַ����������ٷ���512���ֽڡ�
   * @param[out] track2 ���ص����п��������2��Ϣ�ַ����������ٷ���512���ֽڡ�
   * @param[out] track3 ���ص����п��������3��Ϣ�ַ����������ٷ���512���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_GetBankMagInfo(DEVHANDLE icdev, int type, char *track1, char *track2, char *track3);

  /**
   * @brief  ��������֤����ͱ���ͼ��
   * @par    ˵����
   * ��������֤����ͱ���ͼ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] type ���ͣ�0��ʾ�й��˾�������֤��1��ʾ��������þ���֤��2��ʾ�۰�̨�����ס֤��
   * @param[in] text_len ������Ϣ�ĳ��ȡ�
   * @param[in] text ������Ϣ��
   * @param[in] photo_len ��Ƭ��Ϣ�ĳ��ȡ�
   * @param[in] photo ��Ƭ��Ϣ��
   * @param[in] front_file ��������ͼ���ļ�������ȷ����д���Ȩ�ޡ�
   * @param[in] back_file ���뱳��ͼ���ļ�������ȷ����д���Ȩ�ޡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_IdCardImageBuild(DEVHANDLE icdev, int type, int text_len, const unsigned char *text, int photo_len, const unsigned char *photo, const char *front_file, const char *back_file);

  /**
   * @brief  �����Ӿ�����ء�
   * @par    ˵����
   * �����Ӿ�����ء�
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CvLibLoad(DEVHANDLE icdev);

  /**
   * @brief  �����Ӿ���ж�ء�
   * @par    ˵����
   * �����Ӿ���ж�ء�
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CvLibUnload(DEVHANDLE icdev);

  /**
   * @brief  ��������ͷ��
   * @par    ˵����
   * ����ָ��������ͷ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ����ͷ��š�
   * @param[in] name ����ͷ�߼����ƣ���Ϊ���ַ�������ʾ����ȱʡ���Դ򿪵��׸�����ͷ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CvOpen(DEVHANDLE icdev, int number, const char *name);

  /**
   * @brief  �ر�����ͷ��
   * @par    ˵����
   * �ر�ָ��������ͷ��
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ����ͷ��š�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CvClose(DEVHANDLE icdev, int number);

  /**
   * @brief  ���ա�
   * @par    ˵����
   * ʹ��ָ��������ͷ�������ա�
   * @param[in] icdev �豸��ʶ����
   * @param[in] number ����ͷ��š�
   * @param[in] type ��Ƭͼ�����ݵĸ�ʽ��0��ʾBMP�ļ���1��ʾBMP���棬2��ʾBMP Base64�ַ�����
   * @param[in,out] photo_len ���ݵĳ��ȡ�
   * @n BMP�ļ� - ��Ч��
   * @n BMP���� - ���� @a photo ������ֽ�����������Ƭͼ�����ݵĳ��ȡ�
   * @n BMP Base64�ַ��� - ���� @a photo ������ֽ�����������Ƭͼ������Base64�ַ����ĳ��ȣ�����'\0'��
   * @param[in,out] photo ���ݡ�
   * @n BMP�ļ� - �����ļ�������ȷ����д���Ȩ�ޡ�
   * @n BMP���� - ���ص���Ƭͼ�����ݣ������ٷ���65536���ֽڡ�
   * @n BMP Base64�ַ��� - ���ص���Ƭͼ������Base64�ַ����������ٷ���65536���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_CvCapture(DEVHANDLE icdev, int number, int type, int *photo_len, unsigned char *photo);

  /**
   * @brief  �����ļ��ṹ������
   * @par    ˵����
   * �����ļ��ṹ������
   * @param[in] icdev �豸��ʶ����
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SAM_CreateFileSystem(DEVHANDLE icdev);

  /**
   * @brief  дSM4������Կ������
   * @par    ˵����
   * дSM4������Կ������
   * @param[in] icdev �豸��ʶ����
   * @param[in] pucKey ��Կֵ���̶�Ϊ16���ֽڡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SAM_WriteKeySM4(DEVHANDLE icdev, const unsigned char *pucKey);

  /**
   * @brief  SM4���ܲ�����
   * @par    ˵����
   * SM4���ܲ�����
   * @param[in] icdev �豸��ʶ����
   * @param[in] pucIV ��ʼֵ���̶�Ϊ16���ֽڡ�
   * @param[in] pucInput ����������ݡ�
   * @param[in] iInputLen ����������ݵĳ��ȡ�
   * @param[out] pucOutput ���ؼ��ܽ����
   * @param[out] piOutputLen ���ؼ��ܽ���ĳ��ȡ�
   * @return <0��ʾʧ�ܣ�==0��ʾ�ɹ���
   */
  short USER_API dc_SAM_AlgSM4EncryptECB(DEVHANDLE icdev, const unsigned char *pucIV, const unsigned char *pucInput, int iInputLen, unsigned char *pucOutput, int *piOutputLen);

#ifdef __cplusplus
}
#endif

#endif // DCRF32_H_