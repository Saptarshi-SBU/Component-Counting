#ifndef CC_LOGGER_H_
#define CC_LOGGER_H_

#include <unistd.h>
#include <sys/types.h>

#include <ctime>
#include <mutex>
#include <chrono>
#include <cstring>
#include <cassert>
#include <fstream>
#include <iostream>
#include <type_traits>

using namespace std;

//for dev-site logging
#define CC_DEV

//log file name
#define CC_LOGFILE "ccfile.log"

#define CC_LOGFILE_SIZE  33554432L // 32 MB

#define CC_LOG_STATUS \
        X(0, CC_LOG_TRACE,  "TRACE") \
        X(1, CC_LOG_DEBUG,  "DEBUG") \
        X(2, CC_LOG_INFO,   "INFO ") \
        X(3, CC_LOG_WARN,   "WARN ") \
        X(4, CC_LOG_ERROR,  "ERROR") \
        X(5, CC_LOG_NOTICE, "NOTICE")

typedef enum CC_LOGLEVEL {
        #define X(code, name, string) name = code,
        CC_LOG_STATUS
        #undef X
}CC_LOGLEVEL;

static inline const char *CCLogLevelString(CC_LOGLEVEL level) {
        switch (level) {
        #define X(code, name, string) \
        case name: return string" ";
        CC_LOG_STATUS
        #undef X
        default:return "unknown loglevel";
        }
}

typedef std::chrono::time_point<std::chrono::system_clock> time_point_t;

// time formatting (call is synchronized by logger mutex)
static inline std::string SysTimeFormat(const time_point_t& tp) {
    char buffer[128];

    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm *timetm = std::localtime(&time);
    if (timetm != nullptr) {
        strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timetm);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>
            (tp.time_since_epoch()).count();
        return std::string(buffer) + ":" + std::to_string(ms % 1000);
    } else
        return std::string();
}

//CC Logger
class CCLog {
    public:

    // for logger instantiation
    static void Initialize(const char *logfile) {
        nanny_.lock();
        if (logfile && ostreamp_ == nullptr) {
            ostreamp_ = new std::ofstream();
            assert(ostreamp_);
            ostreamp_->open(logfile, std::ofstream::out | std::ofstream::trunc);
            if (!ostreamp_->is_open()) {
                printf("warn: error opening log file: %s, logging will be disabled\n",
                    strerror(errno));
                delete ostreamp_;
                ostreamp_ = nullptr;
            } else
                filename_.assign(logfile);
        }
        nanny_.unlock();
    }

    static void Shutdown(void) {
        std::lock_guard<std::mutex> lock(nanny_); // we may skip lock here
        if (ostreamp_ && ostreamp_->is_open()) {
            Flush();
            ostreamp_->close();
        }

        if (ostreamp_)
            delete ostreamp_;
        ostreamp_ = nullptr;
    }

    static bool GetLogStatus(void) {
        return (ostreamp_ != nullptr);
    }

    // change log level
    static void SetLogLevel(CC_LOGLEVEL level) {
        level_ = level;
    }

    template <typename... Args>
    static void Trace(Args... args) {
        Log(CC_LOG_TRACE, args...);
    }

    template <typename... Args>
    static void Debug(Args... args) {
        Log(CC_LOG_DEBUG, args...);
    }

    template <typename... Args>
    static void Info(Args... args) {
        Log(CC_LOG_INFO, args...);
    }

    template <typename... Args>
    static void Warn(Args... args) {
        Log(CC_LOG_WARN, args...);
        Flush();
    }

    template <typename... Args>
    static void Error(Args... args) {
        Log(CC_LOG_ERROR, args...);
        Flush();
    }

    template <typename... Args>
    static void Notice(Args... args) {
        Log(CC_LOG_NOTICE, args...);
        Flush();
    }

    static bool CheckCanLog(CC_LOGLEVEL level) {
        if ((ostreamp_ == nullptr) || (level < level_))
            return false;
        return true;
    }

    private:

    static std::string filename_; // hold file name

    static std::mutex nanny_; // to protect stream operations

    static std::ofstream *ostreamp_; // native fstream object

    static CC_LOGLEVEL level_; // logger log level

    CCLog() {} // prevent instantiation

    ~CCLog() {}

    static void Flush(void) {
        if (ostreamp_) {
            ostreamp_->flush();
            if (ostreamp_->bad()) {
                std::cerr << "error flushing log file" << std::endl;
                std::cerr << "error :" << errno << std::endl;
                assert(0);
            }
        }
    }

    static void Truncate(void) {
        if (!ostreamp_)
            return;

        if (ostreamp_->is_open())
            ostreamp_->close();

        assert(!filename_.empty());

        ostreamp_->open(filename_.c_str(), std::ofstream::out | std::ofstream::trunc);
        if (!ostreamp_->is_open()) {
            printf("warn: error opening log file in truncate mode: %s,"
                    "logging will be disabled\n", strerror(errno));
            filename_.clear();
            delete ostreamp_;
            ostreamp_ = nullptr;
        }
    }

    template<typename T>
    static void LogInternal(T value) {
        *ostreamp_ << " " << value;
        if (ostreamp_->bad()) {
            std::cerr << "error writing to log file" << std::endl;
            std::cerr << "error :" << errno << std::endl;
            assert(0);
        }
        if (ostreamp_->fail()) {
            std::cerr << "failed to parse entry" << std::endl;
            std::cerr << typeid(value).name() << std::endl;
            ostreamp_->clear(ios::failbit);
            assert(0);
        }
    }

    template <typename T, typename... Args>
    static void LogInternal(T value, Args... args) {
        LogInternal(value);
        LogInternal(args...);
    }

    // main writer method
    template <typename... Args>
    static void Log(CC_LOGLEVEL level, Args... args) {
        if (!CheckCanLog(level))
            return;

        std::lock_guard<std::mutex> lock(nanny_);

        if ((ostreamp_->tellp() - std::streampos(0)) > CC_LOGFILE_SIZE)
            Truncate();
        // logger header (includes timestamps and pid info)
        LogInternal(SysTimeFormat(std::chrono::system_clock::now()));
        LogInternal(std::string("[") + std::to_string(getpid()) + std::string("]"));
        LogInternal(CCLogLevelString(level), args...);
        *ostreamp_ << endl;
    }
};

// For set config property
static inline int SetLoggingLevel(std::string logLevel) {
    if (logLevel.compare("NOTICE") == 0)
        CCLog::SetLogLevel(CC_LOG_NOTICE);
    else if (logLevel.compare("ERROR") == 0)
        CCLog::SetLogLevel(CC_LOG_ERROR);
    else if (logLevel.compare("WARN") == 0)
        CCLog::SetLogLevel(CC_LOG_WARN);
    else if (logLevel.compare("INFO") == 0)
        CCLog::SetLogLevel(CC_LOG_INFO);
    else if (logLevel.compare("DEBUG") == 0)
        CCLog::SetLogLevel(CC_LOG_DEBUG);
    else if (logLevel.compare("TRACE") == 0)
        CCLog::SetLogLevel(CC_LOG_TRACE);
    else {
        CCLog::Error("invalid log level specified");
        return -EINVAL;
    }
    return 0;
}

class CCConsoleLog {
    public:

    static bool canLog;

    // change log level
    static void SetLogLevel(CC_LOGLEVEL level) {
        level_ = level;
    }

    template <typename... Args>
    static void Trace(Args... args) {
        Log(CC_LOG_TRACE, args...);
    }

    template <typename... Args>
    static void Debug(Args... args) {
        Log(CC_LOG_DEBUG, args...);
    }

    template <typename... Args>
    static void Info(Args... args) {
        Log(CC_LOG_INFO, args...);
    }

    template <typename... Args>
    static void Warn(Args... args) {
        Log(CC_LOG_WARN, args...);
        Flush();
    }

    template <typename... Args>
    static void Error(Args... args) {
        Log(CC_LOG_ERROR, args...);
        Flush();
    }

    template <typename... Args>
    static void Notice(Args... args) {
        Log(CC_LOG_NOTICE, args...);
        Flush();
    }

    static bool CheckCanLog(void) {
        return canLog;
    }

    private:

    static CC_LOGLEVEL level_; // logger log level

    CCConsoleLog() {} // prevent instantiation

    ~CCConsoleLog() {}

    static void Flush(void) {
        std::cout << std::flush;
    }

    template<typename T>
    static void LogInternal(T value) {
        std::cout << " " << value;
    }

    template <typename T, typename... Args>
    static void LogInternal(T value, Args... args) {
        LogInternal(value);
        LogInternal(args...);
    }

    template <typename... Args>
    static void Log(CC_LOGLEVEL level, Args... args) {
        if (!CheckCanLog())
            return;
        //LogInternal(CCLogLevelString(level), args...);
        LogInternal(args...);
        std::cout << endl;
    }
};

#define CHECK_LOGGER_START \
    do { \
        if (!CCLog::GetLogStatus()) \
            CCLog::Initialize(CC_LOGFILE); \
    } while (0)

#define CHECK_LOGGER_STOP \
    do { \
        CC_INFO("Logger Shutdown"); \
        CCLog::Shutdown(); \
    } while (0)

// macro substituition
#define LINESTR2(x) #x
#define LINESTR(x)  LINESTR2(x)

#ifdef CC_DEV
#define LOCSTR __FILE__ ":" LINESTR(__LINE__)
#else
#define LOCSTR LINESTR(CC_INDEX) ":" LINESTR(__LINE__)
#endif

#define CC_TRACE(Args,...)  \
    do { \
        if (CCLog::CheckCanLog(CC_LOG_TRACE)) \
            CCLog::Trace(LOCSTR, Args, ##__VA_ARGS__); \
    } while (0);

#define CC_DEBUG(Args,...) \
    do { \
        if (CCLog::CheckCanLog(CC_LOG_DEBUG)) \
            CCLog::Debug(LOCSTR, Args, ##__VA_ARGS__); \
    } while (0);

#define CC_INFO(Args,...) \
    do { \
        if (CCLog::CheckCanLog(CC_LOG_INFO)) \
            CCLog::Info(LOCSTR, Args, ##__VA_ARGS__); \
    } while (0);

#define CC_WARN(Args,...) \
    do { \
        if (CCLog::CheckCanLog(CC_LOG_WARN)) \
            CCLog::Warn(LOCSTR, Args, ##__VA_ARGS__); \
    } while (0);

#define CC_ERR(Args,...) \
    do { \
        if (CCLog::CheckCanLog(CC_LOG_ERROR)) \
            CCLog::Error(LOCSTR, Args, ##__VA_ARGS__); \
    } while (0);

#define CC_NOTICE(Args,...) \
    do { \
        if (CCLog::CheckCanLog(CC_LOG_NOTICE)) \
            CCLog::Notice(LOCSTR, Args, ##__VA_ARGS__); \
    } while (0);

// Console

#define CONSOLE_INFO(Args,...) \
    do { \
        if (CCConsoleLog::CheckCanLog()) \
            CCConsoleLog::Info(Args, ##__VA_ARGS__); \
    } while (0);

#define CONSOLE_WARN(Args,...) \
    do { \
        if (CCConsoleLog::CheckCanLog()) \
            CCConsoleLog::Warn(Args, ##__VA_ARGS__); \
    } while (0);

#define CONSOLE_ERR(Args,...) \
    do { \
        if (CCConsoleLog::CheckCanLog()) \
            CCConsoleLog::Error(Args, ##__VA_ARGS__); \
    } while (0);
#endif

