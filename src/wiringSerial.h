#ifndef _WPI_WIRING_SERIAL_H_
#define _WPI_WIRING_SERIAL_H_

#include <node_api.h>
  
  namespace wiringserial {

    napi_value init            (napi_env env, napi_value exports);
    napi_value serialOpen      (napi_env env, napi_callback_info info);
    napi_value serialClose     (napi_env env, napi_callback_info info);
    napi_value serialFlush     (napi_env env, napi_callback_info info);
    napi_value serialPutchar   (napi_env env, napi_callback_info info);
    napi_value serialPuts      (napi_env env, napi_callback_info info);
    napi_value serialPrintf    (napi_env env, napi_callback_info info);
    napi_value serialDataAvail (napi_env env, napi_callback_info info);
    napi_value serialGetchar   (napi_env env, napi_callback_info info);

} // namespace wiringserial

#endif
