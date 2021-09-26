#ifndef EVOLIS_EVO_PRINTERS_H
#define EVOLIS_EVO_PRINTERS_H

#define X(NAME, VALUE) EVOLIS_##NAME = VALUE,

#define EVOLIS_CFG_VALUES(X) \
    X(CFG_X01                      , 0x80000000) \
    X(CFG_X02                      , 0x40000000) \
    X(CFG_R02                      , 0x20000000) \
    X(CFG_X04                      , 0x10000000) \
    X(CFG_EXTENSION_1              , 0x08000000) \
    X(CFG_S01                      , 0x04000000) \
    X(CFG_X07                      , 0x02000000) \
    X(CFG_KC200                    , 0x01000000) \
    X(CFG_WIFI                     , 0x00800000) \
    X(CFG_ETHERNET                 , 0x00400000) \
    X(CFG_USB_OVER_IP              , 0x00200000) \
    X(CFG_FLIP                     , 0x00100000) \
    X(CFG_CONTACTLESS              , 0x00080000) \
    X(CFG_SMART                    , 0x00040000) \
    X(CFG_MAGNETIC                 , 0x00020000) \
    X(CFG_REWRITE                  , 0x00010000) \
    X(CFG_FEED_MANUALLY            , 0x00008000) \
    X(CFG_FEED_BY_CDE              , 0x00004000) \
    X(CFG_FEED_BY_FEEDER           , 0x00002000) \
    X(CFG_EJECT_REVERSE            , 0x00001000) \
    X(CFG_FEED_CDE_REVERSE         , 0x00000800) \
    X(CFG_EXTENDED_RESOLUTION      , 0x00000400) \
    X(CFG_LCD                      , 0x00000200) \
    X(CFG_LOCK                     , 0x00000100) \
    X(CFG_OEM                      , 0x00000080) \
    X(CFG_JIS_MAG_HEAD             , 0x00000040) \
    X(CFG_REJECT_SLOT              , 0x00000020) \
    X(CFG_IO_EXT                   , 0x00000010) \
    X(CFG_MONO_ONLY                , 0x00000008) \
    X(CFG_KC100                    , 0x00000004) \
    X(CFG_KINE                     , 0x00000002) \
    X(CFG_WIFI_ENA                 , 0x00000001) \

typedef enum evolis_cfg_flag_e {
    EVOLIS_CFG_VALUES(X)
} evolis_cfg_flag_t;

#define EVOLIS_INF_VALUES(X) \
    X(INF_CLAIM                    , 0x80000000) \
    X(INF_CARD_HOPPER              , 0x40000000) \
    X(INF_CARD_FEEDER              , 0x20000000) \
    X(INF_CARD_FLIP                , 0x10000000) \
    X(INF_CARD_CONTACTLESS         , 0x08000000) \
    X(INF_CARD_SMART               , 0x04000000) \
    X(INF_CARD_PRINT               , 0x02000000) \
    X(INF_CARD_EJECT               , 0x01000000) \
    X(INF_PRINTER_MASTER           , 0x00800000) \
    X(INF_PCSVC_LOCKED             , 0x00400000) \
    X(INF_SLEEP_MODE               , 0x00200000) \
    X(INF_UNKNOWN_RIBBON           , 0x00100000) \
    X(INF_RIBBON_LOW               , 0x00080000) \
    X(INF_CLEANING_MANDATORY       , 0x00040000) \
    X(INF_CLEANING                 , 0x00020000) \
    X(INF_RESET                    , 0x00010000) \
    X(INF_CLEAN_OUTWARRANTY        , 0x00008000) \
    X(INF_CLEAN_LAST_OUTWARRANTY   , 0x00004000) \
    X(INF_CLEAN_2ND_PASS           , 0x00002000) \
    X(INF_READY_FOR_CLEANING       , 0x00001000) \
    X(INF_CLEANING_ADVANCED        , 0x00000800) \
    X(INF_WRONG_ZONE_RIBBON        , 0x00000400) \
    X(INF_RIBBON_CHANGED           , 0x00000200) \
    X(INF_CLEANING_REQUIRED        , 0x00000100) \
    X(INF_PRINTING_RUNNING         , 0x00000080) \
    X(INF_ENCODING_RUNNING         , 0x00000040) \
    X(INF_CLEANING_RUNNING         , 0x00000020) \
    X(INF_WRONG_ZONE_ALERT         , 0x00000010) \
    X(INF_WRONG_ZONE_EXPIRED       , 0x00000008) \
    X(INF_SYNCH_PRINT_CENTER       , 0x00000004) \
    X(INF_UPDATING_FIRMWARE        , 0x00000002) \
    X(INF_BUSY                     , 0x00000001) \

typedef enum evolis_inf_flag_e {
    EVOLIS_INF_VALUES(X)
} evolis_inf_flag_t;

#define EVOLIS_WAR_VALUES(X) \
    X(WAR_POWER_SUPPLY             , 0x80000000) \
    X(WAR_REMOVE_RIBBON            , 0x40000000) \
    X(WAR_RECEPTACLE_OPEN          , 0x10000000) \
    X(WAR_REJECT_BOX_FULL          , 0x08000000) \
    X(WAR_CARD_ON_EJECT            , 0x04000000) \
    X(WAR_WAIT_CARD                , 0x02000000) \
    X(WAR_FEEDER_EMPTY             , 0x01000000) \
    X(WAR_COOLING                  , 0x00200000) \
    X(WAR_HOPPER_FULL              , 0x00100000) \
    X(WAR_RIBBON_ENDED             , 0x00080000) \
    X(WAR_PRINTER_LOCKED           , 0x00040000) \
    X(WAR_COVER_OPEN               , 0x00020000) \
    X(WAR_NO_RIBBON                , 0x00010000) \
    X(WAR_UNSUPPORTED_RIBBON       , 0x00008000) \
    X(WAR_RET_TEMP_NOT_READY       , 0x00004000) \
    X(WAR_NO_CLEAR                 , 0x00002000) \
    X(WAR_CLEAR_END                , 0x00001000) \
    X(WAR_CLEAR_UNSUPPORTED        , 0x00000800) \
    X(WAR_REJECT_BOX_COVER_OPEN    , 0x00000400) \

typedef enum evolis_war_flag_e {
    EVOLIS_WAR_VALUES(X)
} evolis_war_flag_t;

#define EVOLIS_ERR_VALUES(X) \
    X(ERR_HEAD_TEMP                , 0x20000000) \
    X(ERR_NO_OPTION                , 0x10000000) \
    X(ERR_FEEDER_ERROR             , 0x08000000) \
    X(ERR_RIBBON_ERROR             , 0x04000000) \
    X(ERR_COVER_OPEN               , 0x02000000) \
    X(ERR_MECHANICAL               , 0x01000000) \
    X(ERR_REJECT_BOX_FULL          , 0x00800000) \
    X(ERR_BAD_RIBBON               , 0x00400000) \
    X(ERR_RIBBON_ENDED             , 0x00200000) \
    X(ERR_HOPPER_FULL              , 0x00100000) \
    X(ERR_BLANK_TRACK              , 0x00080000) \
    X(ERR_MAGNETIC_DATA            , 0x00040000) \
    X(ERR_READ_MAGNETIC            , 0x00020000) \
    X(ERR_WRITE_MAGNETIC           , 0x00010000) \
    X(ERR_FEATURE                  , 0x00008000) \
    X(ERR_RET_TEMPERATURE          , 0x00004000) \
    X(ERR_CLEAR_ERROR              , 0x00002000) \
    X(ERR_CLEAR_ENDED              , 0x00001000) \

typedef enum evolis_err_flag_e {
    EVOLIS_ERR_VALUES(X)
} evolis_err_flag_t;

#define EVOLIS_EX1_VALUES(X) \
    X(EX1_CFG_EXTENSION_2          , 0x80000000) \
    X(EX1_CFG_KIOSK                , 0x40000000) \
    X(EX1_CFG_QUANTUM              , 0x20000000) \
    X(EX1_CFG_SECURION             , 0x10000000) \
    X(EX1_CFG_DUALYS               , 0x08000000) \
    X(EX1_CFG_PEBBLE               , 0x04000000) \
    X(EX1_CFG_MEM_LAMINATION_MODULE_2, 0x01000000) \
    X(EX1_INF_NO_LAMINATION_TO_DO  , 0x00800000) \
    X(EX1_CFG_SEICO_FEEDER         , 0x00400000) \
    X(EX1_CFG_KYTRONIC_FEEDER      , 0x00200000) \
    X(EX1_CFG_HOPPER               , 0x00100000) \
    X(EX1_CFG_LAMINATOR            , 0x00080000) \
    X(EX1_INF_LAMI_ALLOW_TO_INSERT , 0x00040000) \
    X(EX1_INF_LAMINATING_RUNNING   , 0x00020000) \
    X(EX1_INF_CLEAN_REMINDER       , 0x00010000) \
    X(EX1_INF_LAMI_TEMP_NOT_READY  , 0x00008000) \
    X(EX1_INF_SYNCHRONOUS_MODE     , 0x00004000) \
    X(EX1_INF_LCD_BUT_ACK          , 0x00002000) \
    X(EX1_INF_LCD_BUT_OK           , 0x00001000) \
    X(EX1_INF_LCD_BUT_RETRY        , 0x00000800) \
    X(EX1_INF_LCD_BUT_CANCEL       , 0x00000400) \
    X(EX1_CFG_BEZEL                , 0x00000200) \
    X(EX1_INF_FEEDER_NEAR_EMPTY    , 0x00000100) \
    X(EX1_INF_FEEDER1_EMPTY        , 0x00000080) \
    X(EX1_INF_FEEDER2_EMPTY        , 0x00000040) \
    X(EX1_INF_FEEDER3_EMPTY        , 0x00000020) \
    X(EX1_INF_FEEDER4_EMPTY        , 0x00000010) \
    X(EX1_INF_FEEDER1_NEAR_EMPTY   , 0x00000008) \
    X(EX1_INF_FEEDER2_NEAR_EMPTY   , 0x00000004) \
    X(EX1_INF_FEEDER3_NEAR_EMPTY   , 0x00000002) \
    X(EX1_INF_FEEDER4_NEAR_EMPTY   , 0x00000001) \

typedef enum evolis_ex1_flag_e {
    EVOLIS_EX1_VALUES(X)
} evolis_ex1_flag_t;

#define EVOLIS_EX2_VALUES(X) \
    X(EX2_CFG_EXTENSION_3          , 0x80000000) \
    X(EX2_INF_SA_PROCESSING        , 0x40000000) \
    X(EX2_INF_SCP_PROCESSING       , 0x20000000) \
    X(EX2_INF_OPT_PROCESSING       , 0x10000000) \
    X(EX2_INF_X08_PRINTER_UNLOCKED , 0x08000000) \
    X(EX2_INF_X08_FEEDER_OPEN      , 0x04000000) \
    X(EX2_INF_X08_EJECTBOX_FULL    , 0x02000000) \
    X(EX2_INF_X08_PRINT_UNLOCKED   , 0x01000000) \
    X(EX2_CFG_LAMINATION_MODULE_2  , 0x00800000) \
    X(EX2_INF_LAMINATE_UNKNOWN     , 0x00400000) \
    X(EX2_INF_LAMINATE_LOW         , 0x00200000) \
    X(EX2_INF_LAMI_CARD            , 0x00100000) \
    X(EX2_INF_LAMI_CLEANING_RUNNING, 0x00080000) \
    X(EX2_INF_LAMI_UPDATING_FIRMWARE, 0x00040000) \
    X(EX2_INF_LAMI_READY_FOR_CLEANING, 0x00020000) \
    X(EX2_INF_CARD_REAR            , 0x00010000) \
    X(EX2_DEF_NO_LAMINATE          , 0x00008000) \
    X(EX2_DEF_LAMI_COVER_OPEN      , 0x00004000) \
    X(EX2_DEF_LAMINATE_END         , 0x00002000) \
    X(EX2_DEF_LAMI_HOPPER_FULL     , 0x00001000) \
    X(EX2_DEF_LAMINATE_UNSUPPORTED , 0x00000800) \
    X(EX2_INF_CLEAR_UNKNOWN        , 0x00000400) \
    X(EX2_INF_CLEAR_LOW            , 0x00000200) \
    X(EX2_INF_WRONG_ZONE_CLEAR     , 0x00000100) \
    X(EX2_ERR_LAMI_TEMPERATURE     , 0x00000080) \
    X(EX2_ERR_LAMINATE             , 0x00000040) \
    X(EX2_ERR_LAMI_MECHANICAL      , 0x00000020) \
    X(EX2_ERR_LAMINATE_END         , 0x00000010) \
    X(EX2_ERR_LAMI_COVER_OPEN      , 0x00000008) \
    X(EX2_INF_CLEAR_CHANGED        , 0x00000004) \
    X(EX2_INF_WRONG_ZONE_CLEAR_ALERT, 0x00000002) \
    X(EX2_INF_WRONG_ZONE_CLEAR_EXPIRED, 0x00000001) \

typedef enum evolis_ex2_flag_e {
    EVOLIS_EX2_VALUES(X)
} evolis_ex2_flag_t;

#undef X

#define EVOLIS_STATUS_EX_COUNT 4

/**
 * Evolis status buffer. TODO
 */
typedef struct evolis_status_e {
    uint32_t config;
    uint32_t information;
    uint32_t warning;
    uint32_t error;
    uint32_t exts[EVOLIS_STATUS_EX_COUNT];
    uint16_t session;
} evolis_status_t;

/// Get status of the printer.
/// EVOLIS_LIB int evolis_status(evolis_t* printer, evolis_status_t* status);

#endif // EVOLIS_EVO_PRINTERS_H
