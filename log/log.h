#ifndef __LOG__
#define __LOG__

#include <string>
#include <fstream>
#include <fmt/core.h>
#include <fmt/color.h>
#include <mutex>

#define LOGD(...) Logger::Instance().log(LogLevel::DEBUG, __VA_ARGS__)
#define LOGI(...) Logger::Instance().log(LogLevel::INFO,  __VA_ARGS__)
#define LOGW(...) Logger::Instance().log(LogLevel::WARN,  __VA_ARGS__)
#define LOGE(...) Logger::Instance().log(LogLevel::ERROR, __VA_ARGS__)

enum class LogLevel {DEBUG, INFO, WARN, ERROR};

class Logger
{
public:
    static Logger& Instance();

    void   set_log_level(const LogLevel& level);
    void   enable_file_logging(const std::string& filename);

    template<typename... Args>
    void  log(LogLevel level, fmt::format_string<Args...> fmt_str, Args&&... args);

private:
    Logger();
    ~Logger();
    std::string current_time() const;
    std::string level_to_string(const LogLevel& level) const;
    fmt::color  color_for(const LogLevel& level) const;

    LogLevel      level_ = LogLevel::INFO;
    std::ofstream file_out_;
    std::mutex    mutex_;
};
#endif

template <typename... Args>
inline void Logger::log(LogLevel level, fmt::format_string<Args...> fmt_str, Args &&...args)
{
    if (level < level_) return;

    std::scoped_lock lock(mutex_);
    std::string time_str  =  current_time();
    std::string level_str = level_to_string(level);
    std::string message   = fmt::format(fmt_str, std::forward<Args>(args)...);

    auto color = color_for(level);
    fmt::print(fg(color), "[{}] [{}] {}\n", time_str, level_str, message);

    if (file_out_.is_open())
    {
        file_out_ << "[" << time_str << "] [" << level_str << "] " << message << std::endl;
    }
}
