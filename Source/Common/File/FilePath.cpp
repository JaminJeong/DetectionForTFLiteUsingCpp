#include "Common/Platform.h"
#include "Common/File/FilePath.h"

namespace ContAI {

#if defined(_IOS_)

    namespace{
        NSString* FilePathForResourceName(NSString* name, NSString* extension) {
            NSString* file_path = [[NSBundle mainBundle] pathForResource:name ofType:extension];
            if (file_path == NULL) {
                NSLog(@"Couldn't find '%@.%@' in bundle.", name, extension);
                exit(-1);
            }
            return file_path;
        }
        
        NSString* FilePathForResourceName(const std::string& _name, const std::string& _extension)
        {
            NSString* name = [NSString stringWithFormat:@"%s", _name.c_str()];
            NSString* extension = [NSString stringWithFormat:@"%s", _extension.c_str()];
            
            return FilePathForResourceName(name, extension);
        }
    }

    std::string GetPath(
                            const std::string& _file_name,
                            const std::string& _file_type
                            )
    {
        NSString* labels_path = FilePathForResourceName(_file_name, _file_type);
         if (!labels_path) {
            NSLog(@"Failed to find model proto at %s %s", _file_name.c_str(), _file_type.c_str());
             return std::string("");
         }

         return [labels_path UTF8String];
    }

#elif defined(_ANDROID_) // _IOS_

    std::string GetPath(
                            const std::string& _file_name,
                            const std::string& _file_type
                            )
    {
        std::string path;
        //path.append(PLATFORM_RES_PATH);
        //path.append(FOLDER_SEPERATOR); // path ' / '
        path.append(_file_name);
        path.append(".");
        path.append(_file_type);
        return path;
    }
#endif // _ANDROID_

} // namespace ContAI
