#ifndef _WPI_WIRING_PI_H_
#define _WPI_WIRING_PI_H_

#include <node_api.h>

namespace wiringpi {

    napi_value init               (napi_env env, napi_value exports);
    napi_value setup              (napi_env env, napi_callback_info info);
    napi_value libwiringPiVersion (napi_env env, napi_callback_info info);
    napi_value pinMode            (napi_env env, napi_callback_info info);
    napi_value digitalWrite       (napi_env env, napi_callback_info info);

} // namespace wiringpi

#endif // _WPI_WIRING_PI_H_
