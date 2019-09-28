#pragma once

#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>

namespace ME {

    enum SEVERITY {
        DEBUG,
        INFO,
        WARN,
        FATAL
    };

    template <typename EnumClass, EnumClass EnumVal>
    class LogImpl;

    template<auto EnumVal>
    auto Log() {
        return LogImpl<decltype(EnumVal), EnumVal>::instance();
    }

    class LogBase {
        template <typename EnumClass, EnumClass EnumVal>
        friend class LogImpl;
    public:
        ~LogBase() {
            if(!m_out) return;
            if(auto fs = dynamic_cast<std::ofstream*>(m_out)) fs->close();
        }

    protected:
        static LogBase& instance() {
            static LogBase log;
            return log;
        }

        void set_stream(std::ostream* stream) { m_out = stream; }
        void set_severity(SEVERITY severity) { m_severity = severity; }
        
        void write(SEVERITY severity, const std::string& error) {
            auto currentDateTime = []() -> std::string {
                time_t     now = time(0);
                char       buf[80];
                strftime(buf, sizeof(buf), "%Y-%m-%d.%X", localtime(&now));
                return buf;
            };

            auto toString = [](SEVERITY severity) -> std::string {
                switch (severity) {
                case DEBUG: return "DEBUG";
                case INFO: return "INFO";
                case WARN: return "WARN";
                case FATAL: return "FATAL";
                default: return "UNKNOWN";
                }
            };

            if(m_out && severity >= m_severity) (*m_out) << currentDateTime() << ": " << toString(severity) << ": " << error << std::endl;
            if(severity == SEVERITY::FATAL) system("pause"), exit(1);
        }

    private:
        LogBase() {}
        
        std::ostream* m_out {&std::cout};
        SEVERITY m_severity {SEVERITY::DEBUG};
    };

    template <typename EnumClass, EnumClass EnumVal>
    class LogImpl {
    public:
        static LogImpl& instance() {
            static LogImpl log;
            return log;
        }

        auto& operator << (const std::string& error)  {
            LogBase::instance().write(EnumVal, error);
            return *this;
        }

        void set_stream(std::ostream* stream) { LogBase::instance().set_stream(stream); }
        void set_severity(SEVERITY severity) { LogBase::instance().set_severity(severity); }
    };
}
