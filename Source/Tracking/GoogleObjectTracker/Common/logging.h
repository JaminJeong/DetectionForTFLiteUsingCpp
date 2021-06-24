/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_EXAMPLES_ANDROID_JNI_OBJECT_TRACKING_LOGGING_H_
#define TENSORFLOW_EXAMPLES_ANDROID_JNI_OBJECT_TRACKING_LOGGING_H_

#if defined(__APPLE__) || defined(TARGET_OS_IPHONE)
#   define _IOS_
#elif defined(__ANDROID__)
#   define _ANDROID_
#endif

#if defined(_ANDROID_)
#   include <android/log.h>
#   define ANDROID_LOG_INFO 1
#   define ANDROID_LOG_ERROR 2
#elif defined(_IOS_)
#   define ANDROID_LOG_INFO 1
#   define ANDROID_LOG_ERROR 2
#endif

#include <string.h>
#include <ostream>
#include <sstream>
#include <string>

// A macro to disallow the copy constructor and operator= functions
// This is usually placed in the private: declarations for a class.
#define TF_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;         \
  void operator=(const TypeName&) = delete

#if defined(COMPILER_GCC3)
#define TF_PREDICT_FALSE(x) (__builtin_expect(x, 0))
#define TF_PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))
#else // COMPILER_GCC3
#define TF_PREDICT_FALSE(x) (x)
#define TF_PREDICT_TRUE(x) (x)
#endif // COMPILER_GCC3

// Log levels equivalent to those defined by
// third_party/tensorflow/core/platform/logging.h
const int INFO = 0;            // base_logging::INFO;
const int WARNING = 1;         // base_logging::WARNING;
const int ERROR = 2;           // base_logging::ERROR;
const int FATAL = 3;           // base_logging::FATAL;
const int NUM_SEVERITIES = 4;  // base_logging::NUM_SEVERITIES;

class LogMessage : public std::basic_ostringstream<char> {
 public:
  LogMessage(const char* fname, int line, int severity);
  ~LogMessage();

  // Returns the minimum log level for VLOG statements.
  // E.g., if MinVLogLevel() is 2, then VLOG(2) statements will produce output,
  // but VLOG(3) will not. Defaults to 0.
  static int64_t MinVLogLevel();

 protected:
  void GenerateLogMessage();

 private:
  const char* fname_;
  int line_;
  int severity_;
};

// LogMessageFatal ensures the process will exit in failure after
// logging this message.
class LogMessageFatal : public LogMessage {
 public:
  LogMessageFatal(const char* file, int line);
  ~LogMessageFatal();
};

#define _TF_LOG_INFO \
  ::tensorflow::internal::LogMessage(__FILE__, __LINE__, tensorflow::INFO)
#define _TF_LOG_WARNING \
  ::tensorflow::internal::LogMessage(__FILE__, __LINE__, tensorflow::WARNING)
#define _TF_LOG_ERROR \
  ::tensorflow::internal::LogMessage(__FILE__, __LINE__, tensorflow::ERROR)
#define _TF_LOG_FATAL \
  ::tensorflow::internal::LogMessageFatal(__FILE__, __LINE__)

#define _TF_LOG_QFATAL _TF_LOG_FATAL

#define LOG(severity) _TF_LOG_##severity

#define VLOG_IS_ON(lvl) ((lvl) <= LogMessage::MinVLogLevel())

#define VLOG(lvl)                        \
  if (TF_PREDICT_FALSE(VLOG_IS_ON(lvl))) \
  LogMessage(__FILE__, __LINE__, ANDROID_LOG_INFO)

void LogPrintF(const int severity, const char* format, ...);

// Support for printf style logging.
#define LOGV(...)
#define LOGD(...)
#define LOGI(...) LogPrintF(ANDROID_LOG_INFO, __VA_ARGS__);
#define LOGW(...) LogPrintF(ANDROID_LOG_INFO, __VA_ARGS__);
#define LOGE(...) LogPrintF(ANDROID_LOG_ERROR, __VA_ARGS__);
//#define LOGI(...) LogPrintF(1, __VA_ARGS__);
//#define LOGW(...) LogPrintF(1, __VA_ARGS__);
//#define LOGE(...) LogPrintF(2, __VA_ARGS__);


#endif  // TENSORFLOW_EXAMPLES_ANDROID_JNI_OBJECT_TRACKING_LOGGING_H_
