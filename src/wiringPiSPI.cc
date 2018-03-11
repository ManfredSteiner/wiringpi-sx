#include <node_api.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "errno.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "wiringPiSPI.h"

#include <stdexcept>
#include <sstream>
#include "addon.h"

namespace wiringpispi {
    
    /**
     * @description Initialize the desired SPI channel with CPOL=0 and CPHA=0.
     * @param {number} channel use value 0 or 1 to select the SPI channel
     * @param {number} speed use values between 500000 and 32000000 for the SPI clock frequency in Hz
     * @returns {number} file-descriptor of the SPI device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */    
    napi_value setup (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 2;
            napi_value args[2];
            int32_t channel;
            int32_t speed;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (argc != 2) throw WpiLogicError(__LINE__, "invalid number of arguments");

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for channel");
            status = napi_get_value_int32(env, args[0], &channel);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[1], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for speed");
            status = napi_get_value_int32(env, args[1], &speed);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (channel != 0 && channel != 1) { throw WpiLogicError(__LINE__, "invalid channel value, use 0 or 1"); }
            if (speed < 500000 || speed > 32000000) {
                throw WpiLogicError(__LINE__, "invalid speed value, use a value between 500000 and 32000000");
            }

            ::wiringPiClearFailureString();
            int res = ::wiringPiSPISetup(channel, speed);
            if (res < 0) {
                std::ostringstream os;
                os << "Cannot get file descriptor for spi device";
                if (::wiringPiGetLastFailureString()[0] != 0) {
                    os << " (" << ::wiringPiGetLastFailureString() << ")";
                }
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
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_wiringPiSPISetup", "?"); }
       return nullptr;
    }

    /**
     * @description Initialize the desired SPI channel with the desired operation mode (CPOL/CPHA).
     * @param {number} channel use value 0 or 1 to select the SPI channel
     * @param {number} speed use values between 500000 and 32000000 for the SPI clock frequency in Hz
     * @param {number} mode use 0, 1, 2 or 3. Bit 0 is CPOL, bit 1 is CPHA.
     * @returns {number} file-descriptor of the SPI device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    napi_value setupMode (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 3;
            napi_value args[3];
            int32_t channel;
            int32_t speed;
            int32_t mode;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (argc != 3) throw WpiLogicError(__LINE__, "invalid number of arguments");

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for channel");
            status = napi_get_value_int32(env, args[0], &channel);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[1], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for speed");
            status = napi_get_value_int32(env, args[1], &speed);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[2], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for mode");
            status = napi_get_value_int32(env, args[2], &mode);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (channel != 0 && channel != 1) { throw WpiLogicError(__LINE__, "invalid channel value, use 0 or 1"); }
            if (speed < 500000 || speed > 32000000) {
                throw WpiLogicError(__LINE__, "invalid speed value, use a value between 500000 and 32000000");
            }
            if (mode < 0 || mode > 3) { throw WpiLogicError(__LINE__, "invalid mode value, use 0, 1, 2 or 3"); }
            
            ::wiringPiClearFailureString();
            int res = ::wiringPiSPISetupMode(channel, speed, mode);
            if (res < 0) {
                std::ostringstream os;
                os << "Cannot get file descriptor for spi device";
                if (::wiringPiGetLastFailureString()[0] != 0) {
                    os << " (" << ::wiringPiGetLastFailureString() << ")";
                }
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
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_wiringPiSPISetupMode", "?"); }
       return nullptr;
    }

    /**
     * @description Return the file-descriptor for the given channel
     * @param {number} channel use value 0 or 1 to select the SPI channel
     * @returns {number} file descriptor of the SPI device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    napi_value getFd (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 1;
            napi_value args[1];
            int32_t channel;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (argc != 1) throw WpiLogicError(__LINE__, "invalid number of arguments");

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for channel");
            status = napi_get_value_int32(env, args[0], &channel);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (channel != 0 && channel != 1) { throw WpiLogicError(__LINE__, "invalid channel value, use 0 or 1"); }
            int res = ::wiringPiSPIGetFd(channel);
            if (res < 0) {
                std::ostringstream os;
                os << "Error " << res;
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
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_wiringPiSPIGetFd", "?"); }
       return nullptr;
    }

    /**
     * @description Write and Read a block of data over the SPI bus.
     *     Note the data ia being read into the transmit buffer, so will overwrite it!
     * @param {number} channel use value 0 or 1 to select the SPI channel
     * @param {Buffer} data binary data stream to write and read data. 
     * @returns {number} number of transferred bytes
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    napi_value dataRW (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 2;
            napi_value args[2];
            int32_t channel;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;
            bool isBuffer;
            void *data;
            size_t length;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (argc != 2) throw WpiLogicError(__LINE__, "invalid number of arguments");

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type of argument channel");
            status = napi_get_value_int32(env, args[0], &channel);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_is_buffer(env, args[1], &isBuffer);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (!isBuffer) { throw WpiLogicError(__LINE__, "invalid type of argument data"); }
            status = napi_get_buffer_info(env, args[1], &data, &length);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (channel != 0 && channel != 1) { throw WpiLogicError(__LINE__, "invalid channel value, use 0 or 1"); }
            if (length <= 0) { throw WpiLogicError(__LINE__, "invalid length of data"); }
            int res = ::wiringPiSPIDataRW(channel, (unsigned char*)data, length);
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
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_wiringPiSPIDataRW", "?"); }
       return nullptr;
    }

    /**
     * @description This closes opened SPI file descriptor.
     * @param {number} fd file-descriptor returned either from wiringPiSPISetup or wiringPiSPISetupMode
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    napi_value close (napi_env env, napi_callback_info info) {
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
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type of argument fd");
            status = napi_get_value_int32(env, args[0], &fd);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (fd <= 0) { throw WpiLogicError(__LINE__, "invalid value for fd"); }
            int res = ::close(fd);
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
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_wiringPiSPIClose", "?"); }
       return nullptr;
    }

    napi_value init (napi_env env, napi_value exports) {
        try {
            napi_status status;
            napi_value fn;

            status = napi_create_function(env, nullptr, 0, setup, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "wiringPiSPISetup", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, setupMode, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "wiringPiSPISetupMode", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, getFd, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "wiringPiSPIGetFd", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, dataRW, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "wiringPiSPIDataRW", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, close, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "wiringPiSPIClose", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            return exports;
        }
        catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
        catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); }
        catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); }  
        catch ( ... )                       { napi_throw_error(env, "ERR_WPI_wiringPiSPIInit", "?"); }
        return nullptr;
    }

}  // namespace wiringpispi
