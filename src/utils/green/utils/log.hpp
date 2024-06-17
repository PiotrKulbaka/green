#pragma once

#include <cstdarg>

#ifdef __GNUC__
    #define GREEN_FORMAT_PRINTF(string_index, first_to_check) \
        __attribute__((format(printf, string_index, first_to_check)))
#else
    #define GREEN_FORMAT_PRINTF(string_index, first_to_check)
#endif

namespace green::utils
{

    enum log_level
    {
        debug,
        info,
        warning,
        error,
        critical
    };

    void log_vprintf(log_level level, const char* tag, const char* format, va_list args);

    void log_printf(log_level level, const char* tag, const char* format, ...) GREEN_FORMAT_PRINTF(3, 4);
    void log_debug(const char* tag, const char* format, ...) GREEN_FORMAT_PRINTF(2, 3);
    void log_info(const char* tag, const char* format, ...) GREEN_FORMAT_PRINTF(2, 3);
    void log_warning(const char* tag, const char* format, ...) GREEN_FORMAT_PRINTF(2, 3);
    void log_error(const char* tag, const char* format, ...) GREEN_FORMAT_PRINTF(2, 3);
    void log_critical(const char* tag, const char* format, ...) GREEN_FORMAT_PRINTF(2, 3);

    class logger
    {
    public:
        logger(const char* const tag);
        ~logger();

        void printf(log_level level, const char* format, ...) GREEN_FORMAT_PRINTF(3, 4);
        void debug(const char* format, ...) GREEN_FORMAT_PRINTF(2, 3);
        void info(const char* format, ...) GREEN_FORMAT_PRINTF(2, 3);
        void warning(const char* format, ...) GREEN_FORMAT_PRINTF(2, 3);
        void error(const char* format, ...) GREEN_FORMAT_PRINTF(2, 3);
        void critical(const char* format, ...) GREEN_FORMAT_PRINTF(2, 3);
    private:
        const char* const m_tag;
    }; // class logger

} // namespace green::utils
