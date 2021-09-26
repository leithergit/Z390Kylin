#ifndef EVOLISLIB_H
#define EVOLISLIB_H

#include "dynamiclib.h"
#include "../sdk/evolis/include/evolis_def.h"

using  evolis_log_set_path       = void (*)(const char* path);
using  evolis_log_set_level      = void (*)(evolis_log_t level);
using  evolis_open2              = evolis_t* (*)(const char* device, evolis_type_t type);
using  evolis_close              = void (*)(evolis_t* printer);
using  evolis_get_state          = int (*)(evolis_t* printer, evolis_major_state_t* major, evolis_minor_state_t* minor);
using  evolis_get_major_string   = const char* (*)(evolis_major_state_t major);
using  evolis_get_minor_string   = const char* (*)(evolis_minor_state_t minor);
using  evolis_get_infos          = int (*)(evolis_t* printer, evolis_infos_t* infos);
using  evolis_get_ribbon         = int (*)(evolis_t* printer, evolis_ribbon_t* ribbon);
using  evolis_get_cleaning       = int (*)(evolis_t* printer, evolis_cleaning_t* infos);
using  evolis_command            = ssize_t (*)(evolis_t* printer, const char* cmd, size_t cmdSize, char* reply, size_t replyMaxSize);
using  evolis_set_card_pos       = int (*)(evolis_t* printer, evolis_pos_t pos);
using  evolis_print_init         = int (*)(evolis_t* printer);
using  evolis_print_set_option   = int (*)(evolis_t* printer, const char* key, const char* value);
using  evolis_print_get_option   = const char* (*)(evolis_t* printer, const char* key);
using  evolis_print_set_imagep   = int (*)(evolis_t* printer, evolis_face_t face, const char* path);
using  evolis_status             = int (*)(evolis_t* printer, evolis_status_t* status);
using  evolis_print_exec         = int (*)(evolis_t* printer);

class evolislib
{
    void *pHandle = nullptr;
public:
    evolis_log_set_path     pevolis_log_set_path     = nullptr;
    evolis_log_set_level    pevolis_log_set_level    = nullptr;
    evolis_open2            pevolis_open2            = nullptr;
    evolis_close            pevolis_close            = nullptr;
    evolis_get_state        pevolis_get_state        = nullptr;
    evolis_get_major_string pevolis_get_major_string = nullptr;
    evolis_get_minor_string pevolis_get_minor_string = nullptr;
    evolis_get_infos        pevolis_get_infos        = nullptr;
    evolis_get_ribbon       pevolis_get_ribbon       = nullptr;
    evolis_get_cleaning     pevolis_get_cleaning     = nullptr;
    evolis_command          pevolis_command          = nullptr;
    evolis_set_card_pos     pevolis_set_card_pos     = nullptr;
    evolis_print_init       pevolis_print_init       = nullptr;
    evolis_print_set_option pevolis_print_set_option = nullptr;
    evolis_print_get_option pevolis_print_get_option = nullptr;
    evolis_print_set_imagep pevolis_print_set_imagep = nullptr;
    evolis_status           pevolis_status           = nullptr;
    evolis_print_exec       pevolis_print_exec       = nullptr;
    evolislib()
    {
        QString strLib = QDir::currentPath();
        strLib += "/libevolis.so";
        pHandle = dlopen(strLib.toUtf8().data(),RTLD_NOW|RTLD_GLOBAL);
        if (!pHandle)
        {
            qDebug()<<"Failed in load "<<strLib;
            qDebug()<<"Error:"<<strerror(errno);
            return ;
        }
        pevolis_log_set_path     = GetProcAddr(pHandle, evolis_log_set_path    );
        pevolis_log_set_level    = GetProcAddr(pHandle, evolis_log_set_level   );
        pevolis_open2            = GetProcAddr(pHandle, evolis_open2           );
        pevolis_close            = GetProcAddr(pHandle, evolis_close           );
        pevolis_get_state        = GetProcAddr(pHandle, evolis_get_state       );
        pevolis_get_major_string = GetProcAddr(pHandle, evolis_get_major_string);
        pevolis_get_minor_string = GetProcAddr(pHandle, evolis_get_minor_string);
        pevolis_get_infos        = GetProcAddr(pHandle, evolis_get_infos       );
        pevolis_get_ribbon       = GetProcAddr(pHandle, evolis_get_ribbon      );
        pevolis_get_cleaning     = GetProcAddr(pHandle, evolis_get_cleaning    );
        pevolis_command          = GetProcAddr(pHandle, evolis_command         );
        pevolis_set_card_pos     = GetProcAddr(pHandle, evolis_set_card_pos    );
        pevolis_print_init       = GetProcAddr(pHandle, evolis_print_init      );
        pevolis_print_set_option = GetProcAddr(pHandle, evolis_print_set_option);
        pevolis_print_get_option = GetProcAddr(pHandle, evolis_print_get_option);
        pevolis_print_set_imagep = GetProcAddr(pHandle, evolis_print_set_imagep);
        pevolis_status           = GetProcAddr(pHandle, evolis_status          );
        pevolis_print_exec       = GetProcAddr(pHandle, evolis_print_exec      );
    }

};

#endif // EVOLISLIB_H
