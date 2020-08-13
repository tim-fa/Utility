//
// Created by Tim on 06.08.2020.
//
#ifndef LOGGER_H
#define LOGGER_H

// STL
#include <string>

// Libraries
#include <fmt/format.h>

#define ASSERT(condition, message) \
if (!(condition)) {                  \
	Yugen::Logger logger("Yugen");\
	logger.f("********** Assertion failed! **********\nin file '{}':{}\nin function '{}'\n{}\n{}", \
	__FILE__, __LINE__, __FUNCTION__, #condition, message);                                             \
}

namespace Log
{
	class Logger
	{
		public:
			enum LogLevel
			{
				fatal,
				error,
				info,
				warn,
				debug
			};

			explicit Logger(const std::string& name);

			template<class ... Args>
			void i(const std::string& msg, Args ... args)
			{
				log(LogLevel::info, fmt::format(msg, args...));
			}

			template<class ... Args>
			void w(const std::string& msg, Args ... args)
			{
				log(LogLevel::warn, fmt::format(msg, args...));
			}

			template<class ... Args>
			void e(const std::string& msg, Args ... args)
			{
				log(LogLevel::error, fmt::format(msg, args...));
			}

			template<class ... Args>
			void f(const std::string& msg, Args ... args)
			{
				log(LogLevel::fatal, fmt::format(msg, args...));
			}

			template<class ... Args>
			void d(const std::string& msg, Args ... args)
			{
				log(LogLevel::debug, fmt::format(msg, args...));
			}

			void setName(const std::string& name);
			static void setMaxLoglevel(LogLevel level);

		private:
			static std::string getLogLevelStr(LogLevel level);
			void log(LogLevel level, const std::string& msg);
			std::string name;
	};
}

#endif