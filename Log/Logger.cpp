//
// Created by Tim on 06.08.2020.
//

// STL
#include <string>
#include <iomanip>

// Library
#include "termcolor/termcolor.hpp"

// Local
#include "Logger.h"

static Yugen::Logger::LogLevel maxLoglevel = Yugen::Logger::LogLevel::debug;

namespace Yugen
{

	Logger::Logger(const std::string& name)
		: name(name)
	{
	}

	std::string Logger::getLogLevelStr(Yugen::Logger::LogLevel level)
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

	void Logger::log(Yugen::Logger::LogLevel level, const std::string& msg)
	{
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
	}

	void Logger::setName(const std::string& name)
	{
		this->name = name;
	}

	void Logger::setMaxLoglevel(Logger::LogLevel level)
	{
		maxLoglevel = level;
	}
}