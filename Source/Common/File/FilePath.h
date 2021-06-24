#ifndef _COMMON_FILE_FILE_PATH_H_
#define _COMMON_FILE_FILE_PATH_H_

#include "Common/Common.h"
#include <string>

namespace ContAI {

    typedef enum {
        FileOk = 0,
        FileError = 1
    } FileStatus;

    std::string GetPath(
                            const std::string& _file_name,
                            const std::string& _file_type
                            );

} // namespace ContAI
#endif // _COMMON_FILE_FILE_PATH_H_
