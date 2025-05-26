#include "log.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

Logger& Logger::Instance()
{
    static Logger logger;
    return logger;
}

void Logger::set_log_level(const LogLevel &level)
{
    level_ = level;
}

void Logger::enable_file_logging(const std::string &filename)
{
    file_out_.open(filename, std::ios::out | std::ios::app);
}

Logger::Logger()
{
}

Logger::~Logger()
{
    if (file_out_.is_open())
    {
        file_out_.close();
    }
}

std::string Logger::current_time() const
{
    auto now             = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buff;
#ifdef __WIN32
    localtime_s(&tm_buff, &now_time);
#else
    localtime_r(&now_time, &tm_buff);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_buff, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::level_to_string(const LogLevel& level) const
{
    switch (level)
    {
        case LogLevel::DEBUG : return "DBG";
        case LogLevel::INFO  : return "INF";
        case LogLevel::WARN  : return "WRN";
        case LogLevel::ERROR : return "ERR";
        default              : return "UNK";
    }
}

fmt::color Logger::color_for(const LogLevel &level) const
{
    switch (level)
    {
        case LogLevel::DEBUG : return fmt::color::cyan;
        case LogLevel::INFO  : return fmt::color::green;
        case LogLevel::WARN  : return fmt::color::yellow;
        case LogLevel::ERROR : return fmt::color::red;
        default              : return fmt::color::white;
    }
}
