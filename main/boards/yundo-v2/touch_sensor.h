#pragma once
#include "iot/thing.h"
#include "config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "touch_button.h"
#include "iot_button.h"
#include "touch_sensor_lowlevel.h"

namespace iot {

class TouchSensor : public Thing {
public:
    TouchSensor();
private:
    bool touch_detected_ = false;
    button_handle_t btn_handle_ = nullptr;
    int64_t last_touch_sent_time_ = 0;
    static void touch_event_cb(void* arg, void* data);
};

} // namespace iot 