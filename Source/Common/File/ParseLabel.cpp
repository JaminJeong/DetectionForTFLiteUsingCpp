#include "Common/File/ParseLabel.h"
#include "Common/Debug/DebugUtil.h"

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

namespace ContAI {
   
    
    FileStatus LoadClassificatonLabels(
                                       const std::string& _full_file_path,
                                       LabelListType& label_strings
                                       )
    {
        label_strings.clear();
        std::ifstream t;
        t.open(_full_file_path);
        std::string line;
        while (t) {
            std::getline(t, line);
            label_strings.push_back(line);
        }
        t.close();
        
#if defined(DEBUG_PRINT)
        {
            std::stringstream stream_res;
            stream_res << "LoadClassificatonLabels()" << "\n";
            for (LabelListType::const_iterator itr = label_strings.begin();
                 itr != label_strings.end();
                 ++itr) {
                stream_res << *itr << "\n";
            }
            PrintLog(stream_res);
        }
#endif // DEBUG_PRINT
        
        return FileStatus::FileOk;
    }
    
    FileStatus LoadDetectionLabels(
                                   const std::string& _full_file_path,
                                   LabelIndexMapType& label_map,
                                   LabelStringMapType& label_strings
                                   )
    {
        std::vector<int> indexList;
        label_map.clear();
        std::ifstream t;
        t.open(_full_file_path);

        if (t.is_open() == false)
        {
#if defined(DEBUG_PRINT)
            PrintLog("File Open Failed !!");
#endif // DEBUG_PRINT
            return FileStatus::FileError;
        }


        std::string line;
        while (t) {
            std::getline(t, line);
            if (line.find("id:") != std::string::npos)
            {
                std::string num(line.substr(line.find("id:") + 4, line.rfind("\n") - (line.find("id:") + 4)));
#if defined(DEBUG_PRINT)
                {
                    std::stringstream stream_res;
                    stream_res << "num.c_str() : " << num.c_str() << "\n";
                    PrintLog(stream_res);
                }
#endif // DEBUG_PRINT
                indexList.push_back(atoi(num.c_str()) - 1);
            }
            if (line.find("name:") != std::string::npos || line.find("Name:") != std::string::npos )
            {
                label_map[indexList[indexList.size() - 1]] = line.substr(line.find("'") + 1, line.rfind("'") - line.find("'") - 1);
#if defined(DEBUG_PRINT)
                {
                    std::stringstream stream_res;
                    stream_res << "label_map : " <<  label_map[indexList[indexList.size() - 1]].c_str() << "\n";
                    PrintLog(stream_res);
                }
#endif // DEBUG_PRINT
            }
        }
        t.close();
        
        label_strings.clear();
        for (LabelIndexMapType::iterator itr = label_map.begin();
             itr != label_map.end();
             ++itr)
        {
            label_strings[itr->second] = itr->first;
#if defined(DEBUG_PRINT)
            {
                std::stringstream stream_res;
                stream_res << "label_strings :" <<  itr->second.c_str() << " : " << itr->first << "\n";
                PrintLog(stream_res);
            }
#endif // DEBUG_PRINT
        }
        
        return FileStatus::FileOk;
    }

    FileStatus LoadClassificatonLabels(
                                       const std::string& _file_name,
                                       const std::string& _file_type,
                                       LabelListType& label_strings
                                       )
    {
        std::string label_path = GetPath(_file_name, _file_type);
        return LoadClassificatonLabels(label_path, label_strings);
    }

    FileStatus LoadDetectionLabels(
                                   const std::string& _file_name,
                                   const std::string& _file_type,
                                   LabelIndexMapType& label_map,
                                   LabelStringMapType& label_strings
                                   )
    {
        
        std::string label_path = GetPath(_file_name, _file_type);
        return LoadDetectionLabels(label_path, label_map, label_strings);
    }

} // namespace ContAI
