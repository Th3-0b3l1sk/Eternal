#include "Util/Logger.h"
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace Eternal
{
    namespace Util
    {
        const std::string Logger::COLOR_FG_RED    = "\x1b[38;2;255;0;0m";
        const std::string Logger::COLOR_FG_YELLOW = "\x1b[33m";
        const std::string Logger::COLOR_FG_CYAN   = "\x1b[36m";
        const std::string Logger::COLOR_FG_WHITE  = "\x1b[0m";
        const std::string Logger::COLOR_FG_GREEN  = "\x1b[38;2;0;255;0m";
        
        std::string Logger::get_record_id(const el::LogMessage*)
        {
            static std::uint32_t id = 1;

            std::stringstream strm;
            strm << std::setw(4) <<  id++;
            return strm.str();
        }

        Logger::Logger(const std::string& name, bool is_threaded)
            :_should_stop{ false }, _is_threaded{ is_threaded }, _logger_name{ name }, _logger{ nullptr }
        {
            _logger = el::Loggers::getLogger(name);
            // set configuration, hardcoded for now
            el::Configurations config{};
            config.setToDefault();

            // 2023-12-16 20:06:55 WARNING [default] Aborting application. Reason: Fatal log at [path] 
            config.set(el::Level::Global, el::ConfigurationType::Format, "[%id] [%datetime{%b %d, %Y %H:%m:%s}] [%logger:%level] %msg");

            config.set(el::Level::Trace,   el::ConfigurationType::Format, "[%id] [%datetime{%b %d, %Y %H:%m:%s}] [%logger:TRACE] %msg");
            config.set(el::Level::Info,    el::ConfigurationType::Format, "[%id] [%datetime{%b %d, %Y %H:%m:%s}] [%logger:" + COLOR_FG_GREEN  + "INFO " + COLOR_FG_WHITE + "] %msg");
            config.set(el::Level::Debug,   el::ConfigurationType::Format, "[%id] [%datetime{%b %d, %Y %H:%m:%s}] [%logger:" + COLOR_FG_CYAN   + "DEBUG" + COLOR_FG_WHITE + "] %msg => %loc");
            config.set(el::Level::Warning, el::ConfigurationType::Format, "[%id] [%datetime{%b %d, %Y %H:%m:%s}] [%logger:" + COLOR_FG_YELLOW + "WARN " + COLOR_FG_WHITE + "] %msg => %loc");
            config.set(el::Level::Error,   el::ConfigurationType::Format, "[%id] [%datetime{%b %d, %Y %H:%m:%s}] [%logger:" + COLOR_FG_RED    + "ERROR" + COLOR_FG_WHITE + "] %msg => %loc");
            config.set(el::Level::Fatal,   el::ConfigurationType::Format, "[%id] [%datetime{%b %d, %Y %H:%m:%s}] [%logger:" + COLOR_FG_RED    + "FATAL" + COLOR_FG_WHITE + "] %msg => %loc");

            // install record_id formatter
            el::Helpers::installCustomFormatSpecifier(el::CustomFormatSpecifier("%id", Logger::get_record_id));
            el::Loggers::reconfigureLogger(_logger->id(), config);

            // auto new line
            el::Loggers::addFlag(el::LoggingFlag::NewLineForContainer);

            if (_is_threaded) {
                std::thread(std::bind(&Logger::_work, this)).detach();
            }
        }

        Logger::~Logger()
        {
            if (_logger)
                el::Loggers::unregisterLogger(_logger_name.c_str());
        }
        void Logger::_work()
        {
            while (!_should_stop)
            {
                while (_Q.empty())
                    std::this_thread::sleep_for(std::chrono::milliseconds(15));

                _mtx.lock();
                _internalQ.swap(_Q);
                _mtx.unlock();

                while (!_internalQ.empty()) {
                    auto record{ std::move(_internalQ.front()) };
                    _internalQ.pop();

                    _logger->stream() << record.message();
                    // intentional anonymous variable as the lib seems to log the messagges 
                    // in the destructor of Writer. Why?
                    el::base::Writer(record.level(), record.file().c_str(), record.line(), record.func().c_str())
                        .construct(_logger, false);
                }

                _logger->flush();
            }
        }

        void Logger::log(el::Level level, const std::string& file, std::uint32_t line, const std::string& func, std::uint16_t verbose_level, const std::string& log_msg)
        {
            el::LogMessage msg(level, file, line, func, verbose_level, _logger);
            msg.m_message = log_msg;
            
            _mtx.lock();
            _Q.push(std::move(msg));
            _mtx.unlock();
        }
    }
}
