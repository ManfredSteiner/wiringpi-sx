#ifndef _WPI_WIRING_PI_SPI_H_
#define _WPI_WIRING_PI_SPI_H_

#include <node_api.h>

namespace wiringpispi {

    napi_value init      (napi_env env, napi_value exports);
    napi_value setup     (napi_env env, napi_callback_info info);
    napi_value setupMode (napi_env env, napi_callback_info info);
    napi_value getFd     (napi_env env, napi_callback_info info);
    napi_value dataRW    (napi_env env, napi_callback_info info);
    napi_value close     (napi_env env, napi_callback_info info);

} // namespace wiringpispi

#endif // _WPI_WIRING_PI_SPI_H_