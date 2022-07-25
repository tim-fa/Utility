// STL
#include <string>
#include <iomanip>
#include <utility>

// Library
#include "termcolor/termcolor.hpp"

// Local
#include "Log/Logger.h"

static Log::Logger::LogLevel maxLoglevel = Log::Logger::LogLevel::debug;

namespace Log
{
	void enableConsole()
	{
        AllocConsole();
        FILE* fDummy;
        freopen_s(&fDummy, "CONIN$", "r", stdin);
        freopen_s(&fDummy, "CONOUT$", "w", stderr);
        freopen_s(&fDummy, "CONOUT$", "w", stdout);
	}

	Logger::Logger(std::string name)
		: name(std::move(name))
	{
	}

	std::string Logger::getLogLevelStr(Logger::LogLevel level)
	{
		switch (level) {
			case info:
				return "Info";
			case warn:
				return "Warning";
			case error:
				return "Error";
			case fatal:
				return "Fatal";
			case debug:
				return "Debug";
			default:
				return "Unknown";
		}
	}

	void Logger::log(Logger::LogLevel level, const std::string& msg)
	{
		staticLog(level, msg, name);
	}

	void Logger::setName(const std::string& n)
	{
		this->name = n;
	}

	void Logger::setMaxLoglevel(Logger::LogLevel level)
	{
		maxLoglevel = level;
	}

    void Logger::staticLog(Logger::LogLevel level, const std::string &msg, const std::string& name) {
        if (level > maxLoglevel) {
            return;
        }

        switch (level) {
            case info:
                std::cout << termcolor::green;
                break;
            case warn:
                std::cout << termcolor::yellow;
                break;
            case error:
            case fatal:
                std::cout << termcolor::red;
                break;
            case debug:
                std::cout << termcolor::white;
                break;
        }
        std::cout << std::setw(20) << std::left << name;
        std::cout << std::setw(10) << std::left << fmt::format("[{}]", getLogLevelStr(level));
        std::cout << std::left << msg << std::endl;
        std::cout << termcolor::reset;

        if (level == LogLevel::fatal) {
            throw std::runtime_error(fmt::format("{}: {}", name, msg));
        }
    }
}