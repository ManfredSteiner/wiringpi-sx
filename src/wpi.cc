#include <node_api.h>
#include "addon.h"
#include <wiringPi.h>
#include "wiringPi.h"
#include "wiringPiSPI.h"
#include "wiringSerial.h"

namespace nodemodule {

    napi_value init (napi_env env, napi_value exports) {
        if (wiringpi::init(env, exports) == nullptr)    { return nullptr; }
        if (wiringpispi::init(env, exports) == nullptr) { return nullptr; }
        if (wiringserial::init(env, exports) == nullptr) { return nullptr; }
        return exports;            
    }

    NAPI_MODULE(NODE_GYP_MODULE_NAME, init)


}  // namespace nodemodule
