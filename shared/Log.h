#pragma once

#include <sstream>
#include "cpp-sdk/ICore.h"

class Log
{
    std::stringstream buf;

    typedef Log& (*LogFn)(Log&);
    static Log* _Instance;

    enum Type
    {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
        COLORED
    } type = INFO;

    Log() = default;

public:
    Log(const Log&) = delete;
    Log(Log&&) = delete;
    Log& operator=(const Log&) = delete;

    template<class T>
    Log& Put(const T& val)
    {
        buf << val;
        return *this;
    }
#if __cplusplus >= 202002L
    Log& Put(const char8_t* val)
    {
        buf << (const char*)val;
        return *this;
    }
#endif  // __cplusplus
    Log& Put(LogFn val)
    {
        return val(*this);
    }
    Log& SetType(Type _type)
    {
        type = _type;
        return *this;
    }
    template<class T>
    Log& operator<<(const T& val)
    {
        return Put(val);
    }

    static constexpr struct Log_Info
    {
        template<class T>
        Log& operator<<(const T& val) const
        {
            return Instance().SetType(INFO).Put(val);
        }
    } Info{};

    static constexpr struct Log_Debug
    {
        template<class T>
        Log& operator<<(const T& val) const
        {
            return Instance().SetType(DEBUG).Put(val);
        }
    } Debug{};

    static constexpr struct Log_Warning
    {
        template<class T>
        Log& operator<<(const T& val) const
        {
            return Instance().SetType(WARNING).Put(val);
        }
    } Warning{};

    static constexpr struct Log_Error
    {
        template<class T>
        Log& operator<<(const T& val) const
        {
            return Instance().SetType(ERROR).Put(val);
        }
    } Error{};

    static constexpr struct Log_Colored
    {
        template<class T>
        Log& operator<<(const T& val) const
        {
            return Instance().SetType(COLORED).Put(val);
        }
    } Colored{};

    static Log& Endl(Log& log)
    {
        switch(log.type)
        {
            case INFO: alt::ICore::Instance().LogInfo(log.buf.str()); break;
            case DEBUG: alt::ICore::Instance().LogDebug(log.buf.str().c_str()); break;
            case WARNING: alt::ICore::Instance().LogWarning(log.buf.str().c_str()); break;
            case ERROR: alt::ICore::Instance().LogError(log.buf.str().c_str()); break;
            case COLORED: alt::ICore::Instance().LogColored(log.buf.str().c_str()); break;
        }

        log.buf.str("");
        return log;
    }

    static Log& Instance()
    {
        static Log _Instance;
        return _Instance;
    }
};
