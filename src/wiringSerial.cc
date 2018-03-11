#include <node_api.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "errno.h"
#include <wiringPi.h>
#include <wiringSerial.h>
#include "wiringSerial.h"

#include <stdexcept>
#include <sstream>
#include "addon.h"

namespace wiringserial {

    /**
     * @description This opens and initialises the serial device and sets the baud rate.
     *     It sets the port into “raw” mode (character at a time and no translations), and sets the read timeout to 10 seconds.
     *     You can use the standard read(), write(), etc. system calls on this file descriptor as required.
     *     E.g. you may wish to write a larger block of binary data where the serialPutchar() or serialPuts() function
     *     may not be the most appropriate function to use, in which case, you can use write() to send the data.
     *     Example: serialOpen('/dev/ttyAMA0', 9600);
     * @param {string} device name of serial device
     * @param {number} baudrate baudrate used by this serial device
     * @returns {number} file-descriptor of serial device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    napi_value serialOpen (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 2;
            napi_value args[2];
            char device[128];
            int32_t baudrate;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;
            size_t written;            

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (argc != 2) throw WpiLogicError(__LINE__, "invalid number of arguments");

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_string) throw WpiLogicError(__LINE__, "invalid type of argument device");
            status = napi_get_value_string_utf8(env, args[0], device, sizeof(device) - 1, &written);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            device[written] = 0;
            
            status = napi_typeof(env, args[1], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for baudrate");
            status = napi_get_value_int32(env, args[1], &baudrate);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (device[0] == 0) { throw WpiLogicError(__LINE__, "invalid device value"); }
            if (baudrate <= 0) { throw WpiLogicError(__LINE__, "invalid baudrate value"); }
            int res = ::serialOpen(device, baudrate);
            if (res == -2) { throw WpiLogicError(__LINE__, "unsupported baudrate value"); }
            if (res == -1) {
                std::ostringstream os;
                os << "IOError " << errno << " (" << strerror(errno) << ")";
                throw WpiExecutionError(__LINE__, os.str().c_str());
            }
            napi_value rv;
            status = napi_create_int32(env, res, &rv);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            return rv;
       }
       catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
       catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); } 
       catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); } 
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_serialOpen", "?"); }
       return nullptr;
    }

    /**
     * @description Closes the device identified by the file descriptor given.
     * @param {number} fd file-descriptor of serial device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    napi_value serialClose (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 1;
            napi_value args[1];
            int32_t fd;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (argc != 1) throw WpiLogicError(__LINE__, "invalid number of arguments");

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for fd");
            status = napi_get_value_int32(env, args[0], &fd);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (fd < 0) { throw WpiLogicError(__LINE__, "invalid fd value"); }
            int res = ::serialClose(fd);
            if (res != 0) {
                std::ostringstream os;
                os << "IOError " << errno << " (" << strerror(errno) << ")";
                throw WpiExecutionError(__LINE__, os.str().c_str());
            }
            return nullptr;
       }
       catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
       catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); } 
       catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); } 
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_serialClose", "?"); }
       return nullptr;
    }

    /**
     * @description This discards all data received, or waiting to be send down the given device.
     * @param {number} fd file-descriptor of serial device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR
     */
    napi_value serialFlush (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 1;
            napi_value args[1];
            int32_t fd;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (argc != 1) throw WpiLogicError(__LINE__, "invalid number of arguments");

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for fd");
            status = napi_get_value_int32(env, args[0], &fd);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (fd < 0) { throw WpiLogicError(__LINE__, "invalid fd value"); }
            int res = ::serialFlush(fd);
            if (res != 0) {
                std::ostringstream os;
                os << "IOError " << errno << " (" << strerror(errno) << ")";
                throw WpiExecutionError(__LINE__, os.str().c_str());
            }
            return nullptr;
       }
       catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
       catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); } 
       catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); } 
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_serialFlush", "?"); }
       return nullptr;
    }

    /**
     * @description Sends the single byte to the serial device identified by the given file descriptor.
     * @param {number} fd file-descriptor of serial device
     * @param {number} character the character value (0 to 255) send via the serial device.
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    napi_value serialPutchar (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 2;
            napi_value args[2];
            int32_t fd;
            int32_t character;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for fd");
            status = napi_get_value_int32(env, args[0], &fd);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[1], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for character");
            status = napi_get_value_int32(env, args[1], &character);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (fd < 0) { throw WpiLogicError(__LINE__, "invalid value for fd"); }
            if (character < 0 || character > 255) { throw WpiLogicError(__LINE__, "invalid value for character"); }
            int res = ::serialPutchar(fd, character);
            if (res == -1) {
                std::ostringstream os;
                os << "IOError " << errno << " (" << strerror(errno) << ")";
                throw WpiExecutionError(__LINE__, os.str().c_str());
            }
            if (res != 1) {
                std::ostringstream os;
                os << "IOError linux write() returns " << res << " (expect 1)";
                throw WpiExecutionError(__LINE__, os.str().c_str());
            }
            return nullptr;
        }
        catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
        catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); }
        catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); } 
        catch ( ... )                       { napi_throw_error(env, "ERR_WPI_serialPutchar", "?"); }
        return nullptr;
    }

    /**
     * @description Sends the nul-terminated string to the serial device identified by the given file descriptor.
     * @param {number} fd file-descriptor of serial device
     * @param {number} data the string send via the serial device.
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    napi_value serialPuts (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 2;
            napi_value args[2];
            int32_t fd;
            char* data = NULL;
            size_t length, written;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for fd");
            status = napi_get_value_int32(env, args[0], &fd);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[1], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_string) throw WpiLogicError(__LINE__, "invalid type for data");
            status = napi_get_value_string_utf8(env, args[1], NULL, 0, &length);
            data = (char *)malloc(length + 2);
            if (data == NULL) throw WpiRuntimeError(__LINE__, "out of memory");
            status = napi_get_value_string_utf8(env, args[1], data, length + 1, &written);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            data[written] = 0;

            if (fd < 0) { throw WpiLogicError(__LINE__, "invalid value for fd"); }
            int res = ::serialPuts(fd, data);
            free(data);
            if (res == -1) {
                std::ostringstream os;
                os << "IOError " << errno << " (" << strerror(errno) << ")";
                throw WpiExecutionError(__LINE__, os.str().c_str());
            }
            napi_value rv;
            status = napi_create_int32(env, res, &rv);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            return rv;
        }
        catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
        catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); }
        catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); } 
        catch ( ... )                       { napi_throw_error(env, "ERR_WPI_serialPuts", "?"); }
        return nullptr;
    }

    napi_value serialPrintf (napi_env env, napi_callback_info info) {
        return serialPuts(env, info);
    }

    /**
    * @description Returns the number of characters available for reading.
    * @param {number} fd file-descriptor of serial device
    * @returns {number} number of bytes available
    * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
    */
    napi_value serialDataAvail (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 1;
            napi_value args[1];
            int32_t fd;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (argc != 1) throw WpiLogicError(__LINE__, "invalid number of arguments");

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for fd");
            status = napi_get_value_int32(env, args[0], &fd);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (fd < 0) { throw WpiLogicError(__LINE__, "invalid fd value"); }
            int res = ::serialDataAvail(fd);
            if (res != 0) {
                std::ostringstream os;
                os << "IOError " << errno << " (" << strerror(errno) << ")";
                throw WpiExecutionError(__LINE__, os.str().c_str());
            }
            napi_value rv;
            status = napi_create_int32(env, res, &rv);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            return rv;
       }
       catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
       catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); } 
       catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); } 
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_serialDataAvail", "?"); }
       return nullptr;
    }

    /**
    * @description Returns the next character available on the serial device.
    *     This call will block for up to 10 seconds if no data is available.
    * @param {number} fd file-descriptor of serial device
    * @returns {number} received character value 0 to 255, or -1 if no value is received.
    * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
    */
    napi_value serialGetchar (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 1;
            napi_value args[1];
            int32_t fd;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (argc != 1) throw WpiLogicError(__LINE__, "invalid number of arguments");

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for fd");
            status = napi_get_value_int32(env, args[0], &fd);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (fd < 0) { throw WpiLogicError(__LINE__, "invalid fd value"); }
            int res = ::serialGetchar(fd);
            if (res != 0) {
                std::ostringstream os;
                os << "IOError " << errno << " (" << strerror(errno) << ")";
                throw WpiExecutionError(__LINE__, os.str().c_str());
            }
            napi_value rv;
            status = napi_create_int32(env, res, &rv);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            return rv;
       }
       catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
       catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); } 
       catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); } 
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_serialGetchar", "?"); }
       return nullptr;
    }


    napi_value init (napi_env env, napi_value exports) {
        try {
            napi_status status;
            napi_value fn;

            status = napi_create_function(env, nullptr, 0, serialOpen, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "serialOpen", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, serialClose, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "serialClose", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, serialFlush, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "serialFlush", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, serialPutchar, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "serialPutchar", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, serialPuts, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "serialPuts", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, serialPrintf, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "serialPrintf", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, serialDataAvail, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "serialDataAvail", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, serialGetchar, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "serialGetchar", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            return exports;
        }
        catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
        catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); }
        catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); }  
        catch ( ... )                       { napi_throw_error(env, "ERR_WPI_wiringSerialInit", "?"); }
        return nullptr;
    }

} // wiringserial
