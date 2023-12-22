#pragma once
#include "easylogging++/easylogging++.h"
#include <string>
#include <mutex>
#include <queue>


#define Warn(lgr, msg)    lgr->log(el::Level::Warning, __FILE__, __LINE__, __FUNCTION__, 1, msg);
#define Trace(lgr, msg)   lgr->log(el::Level::Trace,   __FILE__, __LINE__, __FUNCTION__, 1, msg);
#define Debug(lgr, msg)   lgr->log(el::Level::Debug,   __FILE__, __LINE__, __FUNCTION__, 1, msg);
#define Fatal(lgr, msg)   lgr->log(el::Level::Fatal,   __FILE__, __LINE__, __FUNCTION__, 1, msg);
#define Error(lgr, msg)   lgr->log(el::Level::Error,   __FILE__, __LINE__, __FUNCTION__, 1, msg);
#define Verbose(lgr, msg) lgr->log(el::Level::Verbose, __FILE__, __LINE__, __FUNCTION__, 1, msg);
#define Info(lgr, msg)    lgr->log(el::Level::Info,    __FILE__, __LINE__, __FUNCTION__, 1, msg);

namespace Eternal
{
    namespace Util
    {
        class Logger
        {
        private:
            static const std::string COLOR_FG_RED;
            static const std::string COLOR_FG_WHITE;
            static const std::string COLOR_FG_YELLOW;
            static const std::string COLOR_FG_CYAN;
            static const std::string COLOR_FG_GREEN; 
            static const std::string COLOR_FG_PURPLE;

            static std::string get_record_id(const el::LogMessage*);

        public:
            // TODO: implement is_threaded logic
            Logger(const std::string&, bool is_threaded = false);
            ~Logger();

        private:
            void _work();

        public:
            void log(el::Level level, const std::string& file, std::uint32_t line, const std::string& func, std::uint16_t verbose_level, const std::string& log_msg);


        private:
            bool _is_threaded;
            bool _should_stop;
            std::string _logger_name;
            el::Logger* _logger;
            std::mutex _mtx;
            std::queue<el::LogMessage> _Q;
            std::queue<el::LogMessage> _internalQ;
        };
    }
}
