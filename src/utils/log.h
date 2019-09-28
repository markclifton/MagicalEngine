#pragma once

#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

namespace ME {

    enum SEVERITY {
        DEBUG,
        INFO,
        WARN,
        FATAL
    };

    class Log {
        Log() {}
        friend class DLog;
        friend class ILog;
        friend class WLog;
        friend class FLog;
    public:
        ~Log() {
            if(!m_out) return;
            if(auto fs = dynamic_cast<std::ofstream*>(m_out)){
                fs->close();
            }
        }

        static Log& instance() {
            static Log log;
            return log;
        }

        void set_stream(std::ostream* stream) {
            m_out = stream;
        }

        void set_severity(SEVERITY severity) {
            m_severity = severity;
        }

        protected:
            void write(SEVERITY severity, const std::string& error) {
                auto currentDateTime = []() -> std::string {
                    time_t     now = time(0);
                    struct tm  tstruct;
                    char       buf[80];
                    tstruct = *localtime(&now);
                    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
                    return buf;
                };

                auto toString = [](SEVERITY severity) -> std::string {
                    switch (severity) {
                        case DEBUG: return "DEBUG";
                        case INFO: return "INFO";
                        case WARN: return "WARN";
                        case FATAL: return "FATAL";
                        default: return "";
                    }
                };

                if(m_out && severity >= m_severity) {
                    (*m_out) << currentDateTime() << ": " << toString(severity) << ": " << error << std::endl;
                }

                if(severity == SEVERITY::FATAL) {
                    exit(1);
                }
            }

    private:
        std::ostream* m_out {&std::cout};
        SEVERITY m_severity {SEVERITY::DEBUG};
    };

    class DLog {
        DLog() {}
    public:
        static DLog& instance() {
            static DLog log;
            return log;
        }

        DLog& operator << (const std::string& error)  {
            Log::instance().write(SEVERITY::DEBUG, error);
            return *this;
        }
    };

    class ILog {
        ILog() {}
    public:
        static ILog& instance() {
            static ILog log;
            return log;
        }

        ILog& operator << (const std::string& error)  {
            Log::instance().write(SEVERITY::INFO, error);
            return *this;
        }
    };    
    
    class WLog {
        WLog() {}
    public:
        static WLog& instance() {
            static WLog log;
            return log;
        }

        WLog& operator << (const std::string& error)  {
            Log::instance().write(SEVERITY::WARN, error);
            return *this;
        }
    };    
    
    class FLog {
        FLog() {}
    public:
        static FLog& instance() {
            static FLog log;
            return log;
        }

        FLog& operator << (const std::string& error)  {
            Log::instance().write(SEVERITY::FATAL, error);
            return *this;
        }
    };
}
