#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef WIN32
#include <wchar.h>
#else
#include <sys/time.h>
#endif
#ifdef __APPLE__
#include <TargetConditionals.h>
#include "asset.h"
#endif
#include "dcrf32.h"

#ifdef WIN32
typedef __int64 BigInt;
static BigInt GetCurrentTimeTick() {
  return GetTickCount();
}
#else
typedef long long BigInt;
static BigInt GetCurrentTimeTick() {
  timeval tv;
  gettimeofday(&tv, NULL);
  return ((BigInt)tv.tv_sec) * 1000 + tv.tv_usec / 1000;
}
#endif

static void Random(int len, unsigned char *data) {
  int i;

  srand((unsigned int)GetCurrentTimeTick());
  for (i = 0; i < len; ++i) {
    data[i] = rand();
  }
}

static const unsigned char *ConvertString(const unsigned char *in_str, unsigned char *out_str) {
#ifdef WIN32
  return in_str;
#else
  dc_string_converter(2, in_str, out_str);
  return out_str;
#endif
}

static const unsigned char *ConvertString(int type, const unsigned char *in_str, unsigned char *out_str) {
  dc_string_converter(type, in_str, out_str);
  return out_str;
}

static int IdCardTest(HANDLE handle) {
  int result;
  unsigned char info_buffer[64];
  int text_len;
  unsigned char text[256];
  int photo_len;
  unsigned char photo[1024];
  int fingerprint_len;
  unsigned char fingerprint[1024];
  int extra_len;
  unsigned char extra[70];
  int type;
#ifndef WIN32
  unsigned char title[128];
#endif

  printf("\n-------- ID Card Test --------\n");

  printf("dc_SamAReadCardInfo ... ");
  result = dc_SamAReadCardInfo(handle, 3, &text_len, text, &photo_len, photo, &fingerprint_len, fingerprint, &extra_len, extra);
  if (result != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  type = 0;
  if ((text[248] == 'I') && (text[249] == 0)) {
    type = 1;
  } else if ((text[248] == 'J') && (text[249] == 0)) {
    type = 2;
  }

  if (type == 0) {
    unsigned char name[64];
    unsigned char sex[8];
    unsigned char nation[12];
    unsigned char birth_day[36];
    unsigned char address[144];
    unsigned char id_number[76];
    unsigned char department[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char reserved[76];

    printf("dc_ParseTextInfo ... ");
#ifdef WIN32
    result = dc_ParseTextInfo(handle, 1, text_len, text, name, sex, nation, birth_day, address, id_number, department, expire_start_day, expire_end_day, reserved);
#else
    result = dc_ParseTextInfo(handle, 2, text_len, text, name, sex, nation, birth_day, address, id_number, department, expire_start_day, expire_end_day, reserved);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"姓名", (wchar_t *)name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"姓名", title), name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"性别", (wchar_t *)info_buffer);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"性别", title), info_buffer);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x11, nation, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x21, nation, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"民族", (wchar_t *)info_buffer);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"住址", (wchar_t *)address);
    wprintf(L"%s: %s\n", L"公民身份号码", (wchar_t *)id_number);
    wprintf(L"%s: %s\n", L"签发机关", (wchar_t *)department);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"民族", title), info_buffer);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"住址", title), address);
    printf("%s: %s\n", ConvertString((unsigned char *)"公民身份号码", title), id_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发机关", title), department);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
#endif
  } else if (type == 1) {
    unsigned char english_name[244];
    unsigned char sex[8];
    unsigned char id_number[64];
    unsigned char citizenship[16];
    unsigned char chinese_name[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char birth_day[36];
    unsigned char version_number[12];
    unsigned char department_code[20];
    unsigned char type_sign[8];
    unsigned char reserved[16];

    printf("dc_ParseTextInfoForForeigner ... ");
#ifdef WIN32
    result = dc_ParseTextInfoForForeigner(handle, 1, text_len, text, english_name, sex, id_number, citizenship, chinese_name, expire_start_day, expire_end_day, birth_day, version_number, department_code, type_sign, reserved);
#else
    result = dc_ParseTextInfoForForeigner(handle, 2, text_len, text, english_name, sex, id_number, citizenship, chinese_name, expire_start_day, expire_end_day, birth_day, version_number, department_code, type_sign, reserved);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"英文姓名", (wchar_t *)english_name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"英文姓名", title), english_name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"性别", (wchar_t *)info_buffer, (wcscmp((wchar_t *)sex, L"1") == 0) ? L"M" : L"F");
    wprintf(L"%s: %s\n", L"永久居留证号码", (wchar_t *)id_number);
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"性别", title), info_buffer, (strcmp((char *)sex, "1") == 0) ? "M" : "F");
    printf("%s: %s\n", ConvertString((unsigned char *)"永久居留证号码", title), id_number);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x12, citizenship, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x22, citizenship, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"国籍或所在地区代码", (wchar_t *)info_buffer, (wchar_t *)citizenship);
    wprintf(L"%s: %s\n", L"中文姓名", (wchar_t *)chinese_name);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"证件版本号", (wchar_t *)version_number);
    wprintf(L"%s: %s\n", L"当次申请受理机关代码", (wchar_t *)department_code);
    wprintf(L"%s: %s\n", L"证件类型标识", (wchar_t *)type_sign);
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"国籍或所在地区代码", title), info_buffer, citizenship);
    printf("%s: %s\n", ConvertString((unsigned char *)"中文姓名", title), chinese_name);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件版本号", title), version_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"当次申请受理机关代码", title), department_code);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件类型标识", title), type_sign);
#endif
  } else if (type == 2) {
    unsigned char name[64];
    unsigned char sex[8];
    unsigned char reserved[12];
    unsigned char birth_day[36];
    unsigned char address[144];
    unsigned char id_number[76];
    unsigned char department[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char pass_number[40];
    unsigned char sign_count[12];
    unsigned char reserved2[16];
    unsigned char type_sign[8];
    unsigned char reserved3[16];

    printf("dc_ParseTextInfoForHkMoTw ... ");
#ifdef WIN32
    result = dc_ParseTextInfoForHkMoTw(handle, 1, text_len, text, name, sex, reserved, birth_day, address, id_number, department, expire_start_day, expire_end_day, pass_number, sign_count, reserved2, type_sign, reserved3);
#else
    result = dc_ParseTextInfoForHkMoTw(handle, 2, text_len, text, name, sex, reserved, birth_day, address, id_number, department, expire_start_day, expire_end_day, pass_number, sign_count, reserved2, type_sign, reserved3);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"姓名", (wchar_t *)name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"姓名", title), name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"性别", (wchar_t *)info_buffer);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"住址", (wchar_t *)address);
    wprintf(L"%s: %s\n", L"公民身份号码", (wchar_t *)id_number);
    wprintf(L"%s: %s\n", L"签发机关", (wchar_t *)department);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
    wprintf(L"%s: %s\n", L"通行证号码", (wchar_t *)pass_number);
    wprintf(L"%s: %s\n", L"签发次数", (wchar_t *)sign_count);
    wprintf(L"%s: %s\n", L"证件类型标识", (wchar_t *)type_sign);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"性别", title), info_buffer);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"住址", title), address);
    printf("%s: %s\n", ConvertString((unsigned char *)"公民身份号码", title), id_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发机关", title), department);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"通行证号码", title), pass_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发次数", title), sign_count);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件类型标识", title), type_sign);
#endif
  }

  printf("dc_ParsePhotoInfo ... ");
  result = dc_ParsePhotoInfo(handle, 0, photo_len, photo, 0, (unsigned char *)"me.bmp");
  if (result != 0) {
    if (result == -2) {
      printf("failed to call photo decoding library!\n");
    } else {
      printf("error!\n");
    }
    return -1;
  }
  printf("ok!\n");

  return 0;
}

static int IdCardTest2(HANDLE handle) {
  int result;
  unsigned char info_buffer[64];
  int text_len;
  unsigned char text[256];
  int photo_len;
  unsigned char photo[1024];
  int fingerprint_len;
  unsigned char fingerprint[1024];
  int extra_len;
  unsigned char extra[70];
  int type;
#ifndef WIN32
  unsigned char title[128];
#endif

  printf("\n-------- ID Card Test (Hardware Acceleration) --------\n");

  printf("dc_IdCardReadCardInfo ... ");
  result = dc_IdCardReadCardInfo(handle, 0x02, 0x0F, &text_len, text, &photo_len, photo, &fingerprint_len, fingerprint, &extra_len, extra);
  if (result != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  type = 0;
  if ((text[248] == 'I') && (text[249] == 0)) {
    type = 1;
  } else if ((text[248] == 'J') && (text[249] == 0)) {
    type = 2;
  }

  if (type == 0) {
    unsigned char name[64];
    unsigned char sex[8];
    unsigned char nation[12];
    unsigned char birth_day[36];
    unsigned char address[144];
    unsigned char id_number[76];
    unsigned char department[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char reserved[76];

    printf("dc_ParseTextInfo ... ");
#ifdef WIN32
    result = dc_ParseTextInfo(handle, 1, text_len, text, name, sex, nation, birth_day, address, id_number, department, expire_start_day, expire_end_day, reserved);
#else
    result = dc_ParseTextInfo(handle, 2, text_len, text, name, sex, nation, birth_day, address, id_number, department, expire_start_day, expire_end_day, reserved);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"姓名", (wchar_t *)name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"姓名", title), name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"性别", (wchar_t *)info_buffer);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"性别", title), info_buffer);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x11, nation, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x21, nation, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"民族", (wchar_t *)info_buffer);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"住址", (wchar_t *)address);
    wprintf(L"%s: %s\n", L"公民身份号码", (wchar_t *)id_number);
    wprintf(L"%s: %s\n", L"签发机关", (wchar_t *)department);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"民族", title), info_buffer);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"住址", title), address);
    printf("%s: %s\n", ConvertString((unsigned char *)"公民身份号码", title), id_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发机关", title), department);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
#endif
  } else if (type == 1) {
    unsigned char english_name[244];
    unsigned char sex[8];
    unsigned char id_number[64];
    unsigned char citizenship[16];
    unsigned char chinese_name[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char birth_day[36];
    unsigned char version_number[12];
    unsigned char department_code[20];
    unsigned char type_sign[8];
    unsigned char reserved[16];

    printf("dc_ParseTextInfoForForeigner ... ");
#ifdef WIN32
    result = dc_ParseTextInfoForForeigner(handle, 1, text_len, text, english_name, sex, id_number, citizenship, chinese_name, expire_start_day, expire_end_day, birth_day, version_number, department_code, type_sign, reserved);
#else
    result = dc_ParseTextInfoForForeigner(handle, 2, text_len, text, english_name, sex, id_number, citizenship, chinese_name, expire_start_day, expire_end_day, birth_day, version_number, department_code, type_sign, reserved);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"英文姓名", (wchar_t *)english_name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"英文姓名", title), english_name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"性别", (wchar_t *)info_buffer, (wcscmp((wchar_t *)sex, L"1") == 0) ? L"M" : L"F");
    wprintf(L"%s: %s\n", L"永久居留证号码", (wchar_t *)id_number);
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"性别", title), info_buffer, (strcmp((char *)sex, "1") == 0) ? "M" : "F");
    printf("%s: %s\n", ConvertString((unsigned char *)"永久居留证号码", title), id_number);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x12, citizenship, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x22, citizenship, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"国籍或所在地区代码", (wchar_t *)info_buffer, (wchar_t *)citizenship);
    wprintf(L"%s: %s\n", L"中文姓名", (wchar_t *)chinese_name);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"证件版本号", (wchar_t *)version_number);
    wprintf(L"%s: %s\n", L"当次申请受理机关代码", (wchar_t *)department_code);
    wprintf(L"%s: %s\n", L"证件类型标识", (wchar_t *)type_sign);
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"国籍或所在地区代码", title), info_buffer, citizenship);
    printf("%s: %s\n", ConvertString((unsigned char *)"中文姓名", title), chinese_name);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件版本号", title), version_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"当次申请受理机关代码", title), department_code);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件类型标识", title), type_sign);
#endif
  } else if (type == 2) {
    unsigned char name[64];
    unsigned char sex[8];
    unsigned char reserved[12];
    unsigned char birth_day[36];
    unsigned char address[144];
    unsigned char id_number[76];
    unsigned char department[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char pass_number[40];
    unsigned char sign_count[12];
    unsigned char reserved2[16];
    unsigned char type_sign[8];
    unsigned char reserved3[16];

    printf("dc_ParseTextInfoForHkMoTw ... ");
#ifdef WIN32
    result = dc_ParseTextInfoForHkMoTw(handle, 1, text_len, text, name, sex, reserved, birth_day, address, id_number, department, expire_start_day, expire_end_day, pass_number, sign_count, reserved2, type_sign, reserved3);
#else
    result = dc_ParseTextInfoForHkMoTw(handle, 2, text_len, text, name, sex, reserved, birth_day, address, id_number, department, expire_start_day, expire_end_day, pass_number, sign_count, reserved2, type_sign, reserved3);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"姓名", (wchar_t *)name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"姓名", title), name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"性别", (wchar_t *)info_buffer);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"住址", (wchar_t *)address);
    wprintf(L"%s: %s\n", L"公民身份号码", (wchar_t *)id_number);
    wprintf(L"%s: %s\n", L"签发机关", (wchar_t *)department);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
    wprintf(L"%s: %s\n", L"通行证号码", (wchar_t *)pass_number);
    wprintf(L"%s: %s\n", L"签发次数", (wchar_t *)sign_count);
    wprintf(L"%s: %s\n", L"证件类型标识", (wchar_t *)type_sign);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"性别", title), info_buffer);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"住址", title), address);
    printf("%s: %s\n", ConvertString((unsigned char *)"公民身份号码", title), id_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发机关", title), department);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"通行证号码", title), pass_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发次数", title), sign_count);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件类型标识", title), type_sign);
#endif
  }

  printf("dc_ParsePhotoInfo ... ");
  result = dc_ParsePhotoInfo(handle, 0, photo_len, photo, 0, (unsigned char *)"me.bmp");
  if (result != 0) {
    if (result == -2) {
      printf("failed to call photo decoding library!\n");
    } else {
      printf("error!\n");
    }
    return -1;
  }
  printf("ok!\n");

  return 0;
}

static int SsCardTest(HANDLE handle) {
  char card_code[33];
  char card_type[2];
  char version[5];
  char init_org_number[25];
  char card_issue_date[9];
  char card_expire_day[9];
  char card_number[10];
  char social_security_number[19];
  char name[31];
  char name_ex[21];
  char sex[2];
  char nation[3];
  char birth_place[7];
  char birth_day[9];
  unsigned char title[128], message[512];

  printf("\n-------- SS Card Test --------\n");

  printf("dc_GetSocialSecurityCardBaseInfo ... ");
  if (dc_GetSocialSecurityCardBaseInfo(handle, 1, card_code, card_type, version, init_org_number, card_issue_date, card_expire_day, card_number, social_security_number, name, name_ex, sex, nation, birth_place, birth_day) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("%s: %s\n", ConvertString((unsigned char *)"卡的识别码", title), ConvertString((unsigned char *)card_code, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"卡的类别", title), ConvertString((unsigned char *)card_type, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"规范版本", title), ConvertString((unsigned char *)version, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"初始化机构编号", title), ConvertString((unsigned char *)init_org_number, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"发卡日期", title), ConvertString((unsigned char *)card_issue_date, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"卡有效期", title), ConvertString((unsigned char *)card_expire_day, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"卡号", title), ConvertString((unsigned char *)card_number, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"社会保障号码", title), ConvertString((unsigned char *)social_security_number, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"姓名", title), ConvertString((unsigned char *)name, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"姓名扩展", title), ConvertString((unsigned char *)name_ex, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"性别", title), ConvertString((unsigned char *)sex, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"民族", title), ConvertString((unsigned char *)nation, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"出生地", title), ConvertString((unsigned char *)birth_place, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), ConvertString((unsigned char *)birth_day, message));

  return 0;
}

static int SsCardPinTestStep1(HANDLE handle) {
  printf("\n-------- SS Card PIN Test (Step1) --------\n");

  printf("dc_ChangeSocialSecurityCardPassword ... ");
  if (dc_ChangeSocialSecurityCardPassword(handle, 1, "123456", "111111") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  return 0;
}

static int SsCardPinTestStep2(HANDLE handle) {
  printf("\n-------- SS Card PIN Test (Step2) --------\n");

  printf("dc_ChangeSocialSecurityCardPassword ... ");
  if (dc_ChangeSocialSecurityCardPassword(handle, 1, "111111", "123456") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  return 0;
}

static int BankCardTest(HANDLE handle, int type) {
  char number[128];

  printf("\n-------- Bank Card Test --------\n");

  printf("dc_GetBankAccountNumber ... ");
  if (dc_GetBankAccountNumber(handle, type, number) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Account Number: %s\n", number);

  return 0;
}

static int M1CardTest(HANDLE handle) {
  unsigned char buffer[64];
  unsigned int len;

  printf("\n-------- M1 Card Test --------\n");

  printf("dc_reset ... ");
  if (dc_reset(handle, 10) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_config_card ... ");
  if (dc_config_card(handle, 'A') != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_card_n_hex ... ");
  if (dc_card_n_hex(handle, 0x00, &len, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("UID: %s\n", (char *)buffer);

  printf("dc_authentication_pass_hex (FFFFFFFFFFFF) ... ");
  if (dc_authentication_pass_hex(handle, 0x00, 0x00, (unsigned char *)"FFFFFFFFFFFF") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_write_hex (11223344556677889900AABBCCDDEEFF) ... ");
  if (dc_write_hex(handle, 0x01, (char *)"11223344556677889900AABBCCDDEEFF") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read_hex ... ");
  if (dc_read_hex(handle, 0x00, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Data: %s\n", (char *)buffer);

  return 0;
}

static int ContactlessCpuCardTest(HANDLE handle, unsigned char type) {
  unsigned char buffer[2048], buffer2[2048];
  unsigned char byte_len;
  unsigned int len, len2;

  printf("\n-------- Contactless CPU Card Test (Type %c) --------\n", type);

  printf("dc_reset ... ");
  if (dc_reset(handle, 10) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_config_card ... ");
  if (dc_config_card(handle, type) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  if (type == 'A') {
    printf("dc_card_n_hex ... ");
    if (dc_card_n_hex(handle, 0x00, &len, buffer) != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");

    printf("UID: %s\n", (char *)buffer);

    printf("dc_pro_resetInt_hex ... ");
    if (dc_pro_resetInt_hex(handle, &byte_len, (char *)buffer) != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");

    printf("ATS: %s\n", (char *)buffer);
  } else {
    printf("dc_card_b_hex ... ");
    if (dc_card_b_hex(handle, (char *)buffer) != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");

    printf("Info: %s\n", (char *)buffer);
  }

  printf("dc_pro_commandlinkInt_hex (APDU Request: 0084000008) ... ");
  strcpy((char *)buffer, "0084000008");
  len = 5;
  if (dc_pro_commandlinkInt_hex(handle, len, (char *)buffer, &len2, (char *)buffer2, 7) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("APDU Response: %s\n", (char *)buffer2);

  return 0;
}

static int ContactCpuCardTest(HANDLE handle, int number) {
  unsigned char buffer[2048], buffer2[2048];
  unsigned char byte_len;
  unsigned int len, len2;

  printf("\n-------- Contact CPU Card Test (Slot %d) --------\n", number);

  printf("dc_setcpu ... ");
  if (dc_setcpu(handle, 0x0C + number) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_cpureset_hex ... ");
  if (dc_cpureset_hex(handle, &byte_len, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("ATR: %s\n", (char *)buffer);

  printf("dc_cpuapduInt_hex (APDU Request: 0084000008) ... ");
  strcpy((char *)buffer, "0084000008");
  len = 5;
  if (dc_cpuapduInt_hex(handle, len, (char *)buffer, &len2, (char *)buffer2) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("APDU Response: %s\n", (char *)buffer2);

  return 0;
}

static int Sle4442CardTest(HANDLE handle) {
  unsigned char buffer[2048];

  printf("\n-------- SLE4442 Card Test --------\n");

  printf("dc_Check_4442 ... ");
  if (dc_Check_4442(handle) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read_4442_hex ... ");
  if (dc_read_4442_hex(handle, 0, 32, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Data: %s\n", (char *)buffer);

  return 0;
}

static int Sle4428CardTest(HANDLE handle) {
  unsigned char buffer[2048];

  printf("\n-------- SLE4428 Card Test --------\n");

  printf("dc_Check_4428 ... ");
  if (dc_Check_4428(handle) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read_4428_hex ... ");
  if (dc_read_4428_hex(handle, 0, 32, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Data: %s\n", (char *)buffer);

  return 0;
}

static int EepromTest(HANDLE handle) {
  unsigned char buffer[2048];

  printf("\n-------- EEPROM Test --------\n");

  Random(64, buffer);

  printf("dc_swr_eeprom ... ");
  if (dc_swr_eeprom(handle, 0, 64, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_srd_eeprom ... ");
  if (dc_srd_eeprom(handle, 0, 64, &buffer[64]) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  if (memcmp(buffer, &buffer[64], 64) != 0) {
    printf("check error!\n");
    return -1;
  }

  printf("check ok!\n");

  return 0;
}

static int EmidCardTest(HANDLE handle) {
  unsigned char buffer[2048];

  printf("\n-------- EMID Card Test --------\n");

  printf("dc_read_idcard_hex ... ");
  if (dc_read_idcard_hex(handle, 1, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Data: %s\n", (char *)buffer);

  return 0;
}

static int LcdDisplayTest(HANDLE handle) {
  unsigned char buffer[128];

  printf("\n-------- LCD Display Test --------\n");

  printf("dc_LcdDisplayText (First Line) ... ");
  if (dc_LcdDisplayText(handle, 0, 0, 0, 5, ConvertString(0, (unsigned char *)"  欢迎使用  ", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplayText (Second Line Step1) ... ");
  if (dc_LcdDisplayText(handle, 1, 0, 0, 5, ConvertString(0, (unsigned char *)"编号：", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplay (Second Line Step2) ... ");
  if (dc_LcdDisplay(handle, 1, 3, 0, "ABCxyz1234") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplayText (Third Line Step1) ... ");
  if (dc_LcdDisplayText(handle, 2, 0, 0, 5, ConvertString(0, (unsigned char *)"姓名：", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplay (Third Line Step2) ... ");
  if (dc_LcdDisplayText(handle, 2, 3, 0, 5, ConvertString(0, (unsigned char *)" 李小明 ", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplayText (Fourth Line Step1) ... ");
  if (dc_LcdDisplayText(handle, 3, 0, 0, 5, ConvertString(0, (unsigned char *)"金额：", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplay (Fourth Line Step2) ... ");
  if (dc_LcdDisplay(handle, 3, 3, 0, "88888.88") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplayText (Fourth Line Step3) ... ");
  if (dc_LcdDisplayText(handle, 3, 7, 0, 5, ConvertString(0, (unsigned char *)"元", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  return 0;
}

static int MultiInstanceTest(int port) {
  int result;
  HANDLE handle;
  unsigned char buffer[1024];

  printf("\n-------- Multi Instance Test --------\n");

  if (port == 100) {
    ++port;
  }

  printf("dc_init %d %d ... ", port, 115200);
  result = (int)dc_init(port, 115200);
  if (result < 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  handle = (HANDLE)result;

  printf("dc_getver ... ");
  result = dc_getver(handle, buffer);
  if (result != 0) {
    printf("error!\n");
    dc_exit(handle);
    return -1;
  }
  printf("ok!\n");

  printf("Firmware version: %s\n", (char *)buffer);

  printf("dc_readdevsnr ... ");
  result = dc_readdevsnr(handle, buffer);
  if (result != 0) {
    printf("error!\n");
    dc_exit(handle);
    return -1;
  }
  printf("ok!\n");

  printf("Device serial number: %s\n", (char *)buffer);

  dc_exit(handle);

  return 0;
}

class AutoPause {
 public:
  AutoPause() {}
  ~AutoPause() {
    printf("\nPress ENTER key to exit ...\n");
    getchar();
  }
};

int main(int argc, char *argv[]) {
#ifdef WIN32
  AutoPause auto_pause;
#endif
  char port_name[256];
  int port, baud, type;
  int result;
  HANDLE handle;
  unsigned char buffer[1024];
  char *p;

  setlocale(LC_ALL, "");

  printf("Program path: %s\n", argv[0]);

  LibMain(0, buffer);
  printf("Library version: %s\n", (char *)buffer);

  strcpy((char *)buffer, argv[0]);
#ifdef WIN32
  p = strrchr((char *)buffer, '\\');
#else
  p = strrchr((char *)buffer, '/');
#endif
  if (p) {
    p[1] = '\0';
    LibMain(1, buffer);
    LibMain(2, buffer);
  }

#if TARGET_OS_MAC
  AdapterContext ac;
  ac.Create = (void *)CreateAdapter;
  ac.Release = (void *)ReleaseAdapter;
  ac.context = (void *)0;
  LibMain(3, &ac);
  LogContext lc;
  lc.Create = (void *)CreateLog;
  lc.Release = (void *)ReleaseLog;
  LibMain(6, &lc);
  LibMain(7, (void *)QueryControl);
#endif

  if (argc < 4) {
    printf("Usage: dcrf32test port[|port_name] baud type\n");
    printf("  port:\n");
    printf("    0~99        - COM Port\n");
    printf("    100~199     - USB Port\n");
    printf("    200~299     - PCSC Port\n");
    printf("    300~399     - Bluetooth Port\n");
    printf("    400~499     - VNet Port\n");
    printf("    500~599     - SPI Port\n");
    printf("  baud:\n");
    printf("    9600~115200 - Only for COM Port\n");
    printf("  type:\n");
    printf("    0           - Device Connectivity Test\n");
    printf("    1           - ID Card Test\n");
    printf("    2           - ID Card Test (Hardware Acceleration)\n");
    printf("    3           - SS Card Test\n");
    printf("    4           - SS Card PIN Test (Step1)\n");
    printf("    5           - SS Card PIN Test (Step2)\n");
    printf("    6           - Bank Card Test (Contactless)\n");
    printf("    7           - Bank Card Test (Contact)\n");
    printf("    8           - M1 Card Test\n");
    printf("    9           - Contactless CPU Card Test (Type A)\n");
    printf("    10          - Contactless CPU Card Test (Type B)\n");
    printf("    11          - Contact CPU Card Test (Master Slot)\n");
    printf("    12          - Contact CPU Card Test (Sam1 Slot)\n");
    printf("    13          - Contact CPU Card Test (Sam2 Slot)\n");
    printf("    14          - Contact CPU Card Test (Sam3 Slot)\n");
    printf("    15          - SLE4442 Card Test\n");
    printf("    16          - SLE4428 Card Test\n");
    printf("    17          - EEPROM Test\n");
    printf("    18          - EMID Card Test\n");
    printf("    19          - LCD Display Test\n");
    printf("    20          - Multi Instance Test\n");
    return 0;
  }

  if (sscanf(argv[1], "%d|%s", &port, port_name) != 2) {
    if (sscanf(argv[1], "%d", &port) != 1) {
      printf("Error parsing parameter!");
      return -1;
    }
  } else {
    dc_config_port_name(port, port_name);
  }

  if (sscanf(argv[2], "%d", &baud) != 1) {
    printf("Error parsing parameter!");
    return -1;
  }

  if (sscanf(argv[3], "%d", &type) != 1) {
    printf("Error parsing parameter!");
    return -1;
  }

  printf("dc_init %d %d ... ", port, baud);
  result = (int)dc_init(port, baud);
  if (result < 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  handle = (HANDLE)result;

  printf("dc_getver ... ");
  result = dc_getver(handle, buffer);
  if (result != 0) {
    printf("error!\n");
    dc_exit(handle);
    return -1;
  }
  printf("ok!\n");

  printf("Firmware version: %s\n", (char *)buffer);

  printf("dc_readdevsnr ... ");
  result = dc_readdevsnr(handle, buffer);
  if (result != 0) {
    printf("error!\n");
    dc_exit(handle);
    return -1;
  }
  printf("ok!\n");

  printf("Device serial number: %s\n", (char *)buffer);

  switch (type) {
    case 1:
      result = IdCardTest(handle);
      break;
    case 2:
      result = IdCardTest2(handle);
      break;
    case 3:
      result = SsCardTest(handle);
      break;
    case 4:
      result = SsCardPinTestStep1(handle);
      break;
    case 5:
      result = SsCardPinTestStep2(handle);
      break;
    case 6:
      result = BankCardTest(handle, 0);
      break;
    case 7:
      result = BankCardTest(handle, 1);
      break;
    case 8:
      result = M1CardTest(handle);
      break;
    case 9:
      result = ContactlessCpuCardTest(handle, 'A');
      break;
    case 10:
      result = ContactlessCpuCardTest(handle, 'B');
      break;
    case 11:
      result = ContactCpuCardTest(handle, 0);
      break;
    case 12:
      result = ContactCpuCardTest(handle, 1);
      break;
    case 13:
      result = ContactCpuCardTest(handle, 2);
      break;
    case 14:
      result = ContactCpuCardTest(handle, 3);
      break;
    case 15:
      result = Sle4442CardTest(handle);
      break;
    case 16:
      result = Sle4428CardTest(handle);
      break;
    case 17:
      result = EepromTest(handle);
      break;
    case 18:
      result = EmidCardTest(handle);
      break;
    case 19:
      result = LcdDisplayTest(handle);
      break;
    case 20:
      result = MultiInstanceTest(port);
      break;
  }

  if (result != 0) {
    dc_exit(handle);
    return -1;
  }

  dc_beep(handle, 20);

  dc_exit(handle);

  return 0;
}
