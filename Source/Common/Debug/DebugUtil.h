#ifndef _COMMON_DEBUG_DEBUG_UTIL_H_
#define _COMMON_DEBUG_DEBUG_UTIL_H_

#include "Common/Common.h"
#include "Common/Platform.h"

#include <string>
#include <sstream>

#if defined(DEBUG_PRINT)
namespace tflite {
    class Interpreter;
}
#endif // DEBUG_PRINT

namespace ContAI {

    void PrintLog(const std::string& str);
    void PrintLog(const std::stringstream& sstream);

#if defined(DEBUG_PRINT)
    void PrintInterpreterInfo(const tflite::Interpreter& interpreter);
    void PrintParameterBagInfo();
#endif // DEBUG_PRINT

} // namespace ContAI
#endif // _COMMON_DEBUG_DEBUG_UTIL_H_
