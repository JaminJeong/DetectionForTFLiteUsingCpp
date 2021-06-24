#ifndef _COMMON_FILE_PARSE_LABEL_H_
#define _COMMON_FILE_PARSE_LABEL_H_

#include "Common/Common.h"

#include "Common/File/FilePath.h"
#include <string>
#include <map>

namespace ContAI {

    
    FileStatus LoadDetectionLabels(
                                   const std::string& _file_name,
                                   const std::string& _file_type,
                                   LabelIndexMapType& label_map,
                                   LabelStringMapType& label_strings
                                   );

    FileStatus LoadClassificatonLabels(
                                       const std::string& _file_name,
                                       const std::string& _file_type,
                                       LabelListType& label_strings
                                       );

    
    FileStatus LoadDetectionLabels(
                                   const std::string& _full_file_path,
                                   LabelIndexMapType& label_map,
                                   LabelStringMapType& label_strings
                                   );
    
    FileStatus LoadClassificatonLabels(
                                       const std::string& _full_file_path,
                                       LabelListType& label_strings
                                       );

} // namespace ContAI
    
#endif // _COMMON_FILE_PARSE_LABEL_H_
