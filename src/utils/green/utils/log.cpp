#include <green/utils/log.hpp>

#include <cstdio>
#include <cassert>
#include <ctime>

namespace
{

    void log_print(const char* message)
    {
        fprintf(stderr, "%s", message);
    }

} // namespace

namespace green::utils
{

    void log_vprintf(log_level level, const char* tag, const char* format, va_list args)
    {
        assert(tag);
        assert(format);
        static const char* log_level_strings[] = {"debug", "info", "warning", "error", "critical"};
        const auto* level_text = log_level_strings[static_cast<int>(level)];
        char buffer[4096];

        // current time
        time_t rawtime{0};
        tm* timeinfo{nullptr};
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        auto n = 0;
        n += snprintf(buffer + n, sizeof(buffer) - n - 1, "[green][%s]", level_text);
        n += strftime(buffer + n, sizeof(buffer) - n - 1, "[%T]", timeinfo);
        n += snprintf(buffer + n, sizeof(buffer) - n - 1, "[%s] ", tag);
        n += vsnprintf(buffer + n, sizeof(buffer) - n - 1, format, args);
        n += snprintf(buffer + n, sizeof(buffer) - n - 1, "\n");
        buffer[n] = 0;

        log_print(buffer);
    }

    #define GREEN_LOG(level, tag, format)      \
        va_list args;                          \
        va_start(args, format);                \
        log_vprintf(level, tag, format, args); \
        va_end(args)

    void log_printf(log_level level, const char* tag, const char* format, ...)
    {
        GREEN_LOG(level, tag, format);
    }

    void log_debug(const char* tag, const char* format, ...)
    {
        GREEN_LOG(log_level::debug, tag, format);
    }

    void log_info(const char* tag, const char* format, ...)
    {
        GREEN_LOG(log_level::info, tag, format);
    }

    void log_warning(const char* tag, const char* format, ...)
    {
        GREEN_LOG(log_level::warning, tag, format);
    }

    void log_error(const char* tag, const char* format, ...)
    {
        GREEN_LOG(log_level::error, tag, format);
    }
    void log_critical(const char* tag, const char* format, ...)
    {
        GREEN_LOG(log_level::critical, tag, format);
    }


    /* logger::logger */
    logger::logger(const char* tag)
        : m_tag(tag)
    {}

    /* logger::~logger */
    logger::~logger() = default;

    /* logger::printf */
    void logger::printf(log_level level, const char* format, ...)
    {
        GREEN_LOG(level, m_tag, format);
    }

    /* logger::debug */
    void logger::debug(const char* format, ...)
    {
        GREEN_LOG(log_level::debug, m_tag, format);
    }

    /* logger::info */
    void logger::info(const char* format, ...)
    {
        GREEN_LOG(log_level::info, m_tag, format);
    }

    /* logger::warning */
    void logger::warning(const char* format, ...)
    {
        GREEN_LOG(log_level::warning, m_tag, format);
    }

    /* logger::error */
    void logger::error(const char* format, ...)
    {
        GREEN_LOG(log_level::error, m_tag, format);
    }

    /* logger::critical */
    void logger::critical(const char* format, ...)
    {
        GREEN_LOG(log_level::critical, m_tag, format);
    }

} // green::utils
