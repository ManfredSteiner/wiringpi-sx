#include "addon.h"
#include <node_api.h>
#include <stdexcept>
#include <string>
#include <sstream>


void throwWpiRuntimeError (napi_env env, const char *filename, const WpiRuntimeError& re) {
    std::ostringstream os;
    os << "runtime error (" << (filename != NULL ? filename : "?") << ":" << re.line();
    if (re.what() && *re.what()) { os << ", " << re.what(); }
    os << ")";
    napi_throw_error(env, "ERR_WPI_RUNTIME", os.str().c_str());
}

void throwWpiLogicError (napi_env env, const char *filename, const WpiLogicError& ex) {
    std::ostringstream os;
    os << "logic error (" << (filename != NULL ? filename : "?") << ":" << ex.line();
    if (ex.what() && *ex.what()) { os << ", " << ex.what(); }
    os << ")";
    napi_throw_error(env, "ERR_WPI_LOGICERROR", os.str().c_str());
}

void throwWpiExecutionError (napi_env env, const char *filename, const WpiExecutionError& ex) {
    std::ostringstream os;
    os << "execution error (" << (filename != NULL ? filename : "?") << ":" << ex.line();
    if (ex.what() && *ex.what()) { os << ", " << ex.what(); }
    os << ")";
    napi_throw_error(env, "ERR_WPI_EXECUTIONERROR", os.str().c_str());
}

WpiRuntimeError::WpiRuntimeError (const int line) : runtime_error("") {
    srcLine = line;
}

WpiRuntimeError::WpiRuntimeError (const int line, const std::string& msg) : runtime_error(msg) {
    srcLine = line;
}

int WpiRuntimeError::line () const {
    return srcLine;
}


WpiLogicError::WpiLogicError (const int line) : logic_error("") {
    srcLine = line;
}

WpiLogicError::WpiLogicError (const int line, const char *msg) : logic_error(msg) {
    srcLine = line;
}

int WpiLogicError::line () const {
    return srcLine;
};


WpiExecutionError::WpiExecutionError (const int line) : logic_error("") {
    srcLine = line;
}

WpiExecutionError::WpiExecutionError (const int line, const char *msg) : logic_error(msg) {
    srcLine = line;
}

int WpiExecutionError::line () const {
    return srcLine;
};
