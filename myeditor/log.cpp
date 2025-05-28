/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/log.h"

namespace myeditor {

class Logger : public ::google::base::Logger {
 public:
  explicit Logger(::google::base::Logger* warpped);
  ~Logger() override;

  void Write(
    bool force_flush,
    time_t timestamp,
    const char* message,
    size_t message_len) override;

  void Flush() override;

  uint32_t LogSize() override;

  void RegisterWriter(std::function<void(const std::string&)> func);

  void UnRegisterWriteCallback();

 private:
  ::google::base::Logger* const warpped_;
  std::function<void(const std::string&)> write_cb_;
  std::function<void(const std::string&)> null_write_cb_;
};

Logger::Logger(::google::base::Logger* warpped) 
  : warpped_(warpped)
{}

Logger::~Logger() {}

void Logger::Write(
  bool force_flush,
  time_t timestamp,
  const char* message,
  size_t message_len) {
  warpped_->Write(force_flush, timestamp, message, message_len);
  if(write_cb_) {
    write_cb_(std::string(message, message_len));
  }
}

void Logger::Flush() {
  warpped_->Flush();
}

uint32_t Logger::LogSize() {
  return warpped_->LogSize();
}

void Logger::RegisterWriter(std::function<void(const std::string&)> func) {
  write_cb_ = func;
}

void Logger::UnRegisterWriteCallback() { write_cb_ = null_write_cb_; }

namespace {

Logger* g_logger = nullptr;
static void signal_handler(const char *data, size_t size) {
    std::string str = std::string(data, size);
    LOG(ERROR) << str;
}

};

void InitLog(
  const stdfs::path& log_dir,
  const std::string& bin_name,
  int max_size_mb) {
  google::InitGoogleLogging(bin_name.c_str());

  FLAGS_logbufsecs = 0;
  FLAGS_max_log_size = max_size_mb;
  FLAGS_stop_logging_if_full_disk = true;

  std::string dst_str = (log_dir / bin_name).string();
  google::SetLogDestination(google::GLOG_ERROR, "");
  google::SetLogDestination(google::GLOG_WARNING, "");
  google::SetLogDestination(google::GLOG_FATAL, "");
  google::SetLogDestination(google::GLOG_INFO, dst_str.c_str());

  google::InstallFailureSignalHandler();
  google::InstallFailureWriter(&signal_handler);

  g_logger = new Logger(google::base::GetLogger(FLAGS_minloglevel));
  google::base::SetLogger(FLAGS_minloglevel, g_logger);
}

void RegisterWriteCallback(
  std::function<void(const std::string&)> func) {
  if (g_logger) {
    g_logger->RegisterWriter(func);
  }
}

void UnRegisterWriteCallback() {
  if (g_logger) {
    g_logger->UnRegisterWriteCallback();
  }
}

void ShutdownLog() {
  google::ShutdownGoogleLogging();
}

}  // namespace myeditor
