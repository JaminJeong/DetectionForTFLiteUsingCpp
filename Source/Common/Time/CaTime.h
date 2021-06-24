#ifndef _COMMON_TIME_CATIME_H_
#define _COMMON_TIME_CATIME_H_

#include "Common/Common.h"

#if defined(DEBUG_TIME_PRINT)

#include <string>
#include <sstream>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

namespace ContAI {

    class CaTime
    {
    public:
        CaTime()
        {
          clear();
        }

        void start()
        {
          clear();
          startTime = std::chrono::system_clock::now();
        }

        void stop()
        {
          endTime = std::chrono::system_clock::now();
          accumulationTime += (endTime - startTime);
          startTime = std::chrono::system_clock::now();
        }

        void resume()
        {
          startTime = std::chrono::system_clock::now();
        }

        void end()
        {
          endTime = std::chrono::system_clock::now();
          accumulationTime += (endTime - startTime);
        }

        void clear()
        {
          startTime = std::chrono::time_point<std::chrono::system_clock>();
          endTime = std::chrono::time_point<std::chrono::system_clock>();
          accumulationTime = std::chrono::duration<double>();
        }

        double GetTime()
        {
          return accumulationTime.count();
        }

        std::string GetStrTime()
        {
          return NumToString(accumulationTime.count());
        }

        static std::string TimeToString(double _time)
        {
          std::stringstream ss;
          ss  << _time * 1000.0f << "ms";
          return ss.str();
        }

        friend std::ostream& operator <<(std::ostream& stream, CaTime& _time)
        {
            stream << "Inference Time :" << " " << _time.GetTime() * 1000.0f << "ms";
            stream << "\n";
            
            return stream;
        }

    private:
        std::string NumToString(double _time)
        {
          std::stringstream ss;
          ss  << _time * 1000.0f << "ms";
          return ss.str();
        }


    private:
        std::chrono::time_point<std::chrono::system_clock> startTime;
        std::chrono::time_point<std::chrono::system_clock> endTime;
        std::chrono::duration<double> accumulationTime;
    };

#endif // DEBUG_TIME_PRINT

} // namespace ContAI
    
#endif // _COMMON_TIME_CATIME_H_
