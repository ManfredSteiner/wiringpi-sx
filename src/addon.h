#ifndef _ADDON_H_
#define _ADDON_H_

#include <stdexcept>
#include <node_api.h>

class WpiRuntimeError : public std::runtime_error
{
    private:
        int srcLine;

    public:
        WpiRuntimeError (const int line);
        WpiRuntimeError (const int line, const std::string& msg);
        int line () const;
};

class WpiLogicError : public std::logic_error
{
    private:
        int srcLine;

    public:
        WpiLogicError (const int line);
        WpiLogicError (const int line, const char *msg);
        int line () const;
};

class WpiExecutionError : public std::logic_error
{
    private:
        int srcLine;

    public:
        WpiExecutionError (const int line);
        WpiExecutionError (const int line, const char *msg);
        int line () const;
};

void throwWpiRuntimeError (napi_env env, const char *filename, const WpiRuntimeError& re);
void throwWpiLogicError (napi_env env, const char *filename, const WpiLogicError& ex);
void throwWpiExecutionError (napi_env env, const char *filename, const WpiExecutionError& ex);


#endif // _ADDON_H_