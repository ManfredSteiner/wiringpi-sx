#include <node_api.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

#include <stdexcept>
#include <string>
#include <sstream>
#include "addon.h"

namespace wiringpi {

    /**
     * @description Returns the version of the used native wiringpi library (libwiringPi)
     * @returns {string} version as string major.minor 
     * @throws ERR_WPI_RUNTIME
     */
    napi_value libwiringPiVersion (napi_env env, napi_callback_info info) {
        try {
            napi_status status;
            int major, minor;

            ::wiringPiVersion(&major, &minor);
            char s[16];
            napi_value rv;
            snprintf(s, sizeof(s), "%d.%d", major, minor);
            status = napi_create_string_utf8(env, s, strlen(s), &rv);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            return rv;
        }
        catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
        catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); }
        catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); } 
        catch ( ... )                       { napi_throw_error(env, "ERR_WPI_libwiringPiVersion", "?"); }
        return nullptr;
    }


    /**
     * @description Initialises wiringPi and assumes that the calling program is going to be using the wiringPi pin numbering scheme.
     *    This is a simplified numbering scheme which provides a mapping from virtual pin numbers 0 through 63 to the real underlying Broadcom GPIO pin numbers.
     *    see the pins page (http://wiringpi.com/pins/) for a table
     *    which maps the wiringPi pin number to the Broadcom GPIO pin number to the physical location on the edge connector.
     *    This function needs to be called with root privileges.
     * @param {string} mode use 'wpi' to call the native library function wiringPiSetup()
     * @returns {number}  error code if v1 mode otherwise always returns 0
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    napi_value setup (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 1;
            napi_value args[1];
            char mode[10];
            
            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;
            size_t written;            

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_string) throw WpiLogicError(__LINE__, "invalid type for mode");
            status = napi_get_value_string_utf8(env, args[0], mode, sizeof(mode) - 1, &written);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            mode[written] = 0;

            ::wiringPiClearFailureString();
            int res;
            if (strcmp("wpi", mode) == 0) {
                res = ::wiringPiSetup();
            } else if (strcmp("gpio", mode) == 0) {
                res = ::wiringPiSetupGpio();
            } else if (strcmp("sys", mode) == 0) {
                res = ::wiringPiSetupSys();
            } else if (strcmp("phys", mode) == 0) {
                res = ::wiringPiSetupPhys();
            } else {
                throw WpiLogicError(__LINE__, "invalid value for mode");
            }
            if (res < 0) {
                std::ostringstream os;
                os << "setup fails";
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
        catch ( ... )                       { napi_throw_error(env, "ERR_WPI_setup", "?"); }
        return nullptr;
    }


    /**
     * @description Library function void pinMode (int pin, int mode)
     *     This sets the mode of a pin to either INPUT, OUTPUT, PWM_OUTPUT or GPIO_CLOCK.
     *     This function has no effect when in Sys mode. Note that usage of pin modes is restricted.
     *     If you need to change the pin mode, then you can do it with the gpio program in a script before you start your program.
     * @param {number} pin use the virtual pin number 0 to 63 (see http://wiringpi.com/pins/)
     * @param {number} mode use the constants INPUT, OUTPUT, PWM_OUTPUT or GPIO_CLOCK.
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR
     */
    napi_value pinMode (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 2;
            napi_value args[2];
            int32_t pin;
            int32_t mode;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for pin");
            status = napi_get_value_int32(env, args[0], &pin);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[1], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for mode");
            status = napi_get_value_int32(env, args[1], &mode);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (pin < 0 || pin > 63) { throw WpiLogicError(__LINE__, "invalid value for pin"); }
            if (mode != INPUT && mode != OUTPUT && mode != PWM_OUTPUT && mode != GPIO_CLOCK &&
                mode != SOFT_PWM_OUTPUT && mode != SOFT_TONE_OUTPUT && mode != PWM_TONE_OUTPUT) {
                throw WpiLogicError(__LINE__, "invalid value for mode");
            }
            ::pinMode(pin, mode);
            return nullptr;
        }
        catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
        catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); }
        catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); } 
        catch ( ... )                       { napi_throw_error(env, "ERR_WPI_pinMode", "?"); }
        return nullptr;
    }


    /**
     * @description This sets the pull-up or pull-down resistor mode on the given pin, which should be set as an input.
     *     Unlike the Arduino, the BCM2835 has both pull-up an down internal resistors. 
     *     This function has no effect when in Sys mode. If you need to activate a pull-up/pull-down, then you can do
     *     it with the gpio program in a script before you start your program.
     * @param {number} pin use the virtual pin number 0 to 63 (see http://wiringpi.com/pins/)
     * @param {number} pud use PUD_OFF (no pull up/down), PUD_DOWN (pull to ground) or PUD_UP (pull to 3.3v)
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR
     */

    napi_value pullUpDnControl (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 2;
            napi_value args[2];
            int32_t pin;
            int32_t pud;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for pin");
            status = napi_get_value_int32(env, args[0], &pin);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[1], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for pud");
            status = napi_get_value_int32(env, args[1], &pud);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (pin < 0 || pin > 63) { throw WpiLogicError(__LINE__, "invalid value for pin"); }
            if (pud != PUD_OFF && pud != PUD_DOWN && pud != PUD_UP) {
                throw WpiLogicError(__LINE__, "invalid value for pud");
            }
            ::pullUpDnControl(pin, pud);
            return nullptr;
        }
        catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
        catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); }
        catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); } 
        catch ( ... )                       { napi_throw_error(env, "ERR_WPI_pullUpDnControl", "?"); }
        return nullptr;
    }

    /**
     * @description Library function void digitalWrite (int pin, int value)
     *     Write the value HIGH or LOW (1 or 0) to the given pin which must have been previously set as an output.
     *     WiringPi treats any non-zero number as HIGH, however 0 is the only representation of LOW. 
     * @param {number} pin use the virtual pin number 0 to 63 (see http://wiringpi.com/pins/)
     * @param {number} value use the constants LOW or HIGH
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR
     */
    napi_value digitalWrite (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 2;
            napi_value args[2];
            int32_t pin;
            int32_t value;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for pin");
            status = napi_get_value_int32(env, args[0], &pin);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[1], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for value");
            status = napi_get_value_int32(env, args[1], &value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (pin < 0 || pin > 63) { throw WpiLogicError(__LINE__, "invalid value for pin"); }
            if (value != HIGH && value != LOW) {
                throw WpiLogicError(__LINE__, "invalid value for value");
            }
            ::digitalWrite(pin, value);
       }
       catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
       catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); }
       catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); }
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_digitalWrite", "?"); }
       return nullptr;
    }

    
    /**
     * @description Library function void digitalRead (int pin)
     *     Read the value of the given pin.
     * @param {number} pin use the virtual pin number 0 to 63 (see http://wiringpi.com/pins/)
     * @returns {number} value of the pin
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR
     */
    napi_value digitalRead (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 1;
            napi_value args[1];
            int32_t pin;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for pin");
            status = napi_get_value_int32(env, args[0], &pin);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (pin < 0 || pin > 63) { throw WpiLogicError(__LINE__, "invalid value for pin"); }
            int res = ::digitalRead(pin);
            napi_value rv;
            status = napi_create_int32(env, res, &rv);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            return rv;
       }
       catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
       catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); }
       catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); }
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_digitalWrite", "?"); }
       return nullptr;
    }
    

    /**
     * @description Set the freuency on a GPIO clock pin.
     *     Don't forget to set correct pin mode: pinMode(7, GPIO_CLOCK)
     * @param {number} pin use the virtual pin number 0 to 63 (see http://wiringpi.com/pins/)
     * @param {number} frequency value of frequency in Hz.
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR
     */
    napi_value gpioClockSet (napi_env env, napi_callback_info info) {
        try {
            size_t argc = 2;
            napi_value args[2];
            int32_t pin;
            int32_t frequency;

            napi_value _this;
            napi_status status;
            napi_valuetype valuetype;

            status = napi_get_cb_info(env, info, &argc, args, &_this, nullptr);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[0], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for pin");
            status = napi_get_value_int32(env, args[0], &pin);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_typeof(env, args[1], &valuetype);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            if (valuetype != napi_number) throw WpiLogicError(__LINE__, "invalid type for frequency");
            status = napi_get_value_int32(env, args[1], &frequency);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            if (pin < 0 || pin > 63) { throw WpiLogicError(__LINE__, "invalid value for pin"); }
            if (frequency <= 0) {
                throw WpiLogicError(__LINE__, "invalid value for frequency");
            }
            ::gpioClockSet(pin, frequency);
       }
       catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
       catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); }
       catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); }
       catch ( ... )                       { napi_throw_error(env, "ERR_WPI_digitalWrite", "?"); }
       return nullptr;
    }


    napi_value init (napi_env env, napi_value exports) {
        try {
            napi_status status;
            napi_value fn;
            napi_value value;

            status = napi_create_function(env, nullptr, 0, setup, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "setup", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, libwiringPiVersion, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "libwiringPiVersion", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, pinMode, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "pinMode", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, pullUpDnControl, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "pullUpDnControl", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, digitalWrite, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "digitalWrite", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, digitalRead, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "digitalRead", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_function(env, nullptr, 0, gpioClockSet, nullptr, &fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "gpioClockSet", fn);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_int32(env, INPUT, &value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "INPUT", value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_int32(env, OUTPUT, &value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "OUTPUT", value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_int32(env, PWM_OUTPUT, &value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "PWM_OUTPUT", value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_int32(env, GPIO_CLOCK, &value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "GPIO_CLOCK", value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            
            status = napi_create_int32(env, SOFT_PWM_OUTPUT, &value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "SOFT_PWM_OUTPUT", value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_int32(env, SOFT_TONE_OUTPUT, &value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "SOFT_TONE_OUTPUT", value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_int32(env, PWM_TONE_OUTPUT, &value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "PWM_TONE_OUTPUT", value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_int32(env, PUD_OFF, &value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "PUD_OFF", value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_int32(env, PUD_DOWN, &value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "PUD_DOWN", value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            status = napi_create_int32(env, PUD_UP, &value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);
            status = napi_set_named_property(env, exports, "PUD_UP", value);
            if (status != napi_ok) throw WpiRuntimeError(__LINE__);

            return exports;
        }
        catch (const WpiRuntimeError& re)   { throwWpiRuntimeError(env, __FILE__, re); }
        catch (const WpiLogicError& ex)     { throwWpiLogicError(env, __FILE__, ex); }
        catch (const WpiExecutionError& ex) { throwWpiExecutionError(env, __FILE__, ex); }
        catch ( ... )                       { napi_throw_error(env, "ERR_WPI_wiringPiInit", "?"); }
        return nullptr;
    }

}  // namespace wiringpi
