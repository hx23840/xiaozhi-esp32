#include "touch_sensor.h"
#include <esp_log.h>
#include <assert.h>
#include "application.h"
#include <esp_timer.h>

#define TAG "TouchSensor"
#define TOUCH_CHANNEL 9 // GPIO9 (T8 on ESP32-S3)
#define TOUCH_THRESHOLD 0.15 // 调低阈值便于调试

namespace iot {

// 节流间隔（微秒）
static constexpr int64_t kTouchSendIntervalUs = 500 * 1000;

void TouchSensor::touch_event_cb(void* arg, void* data) {
    TouchSensor* self = static_cast<TouchSensor*>(arg);
    button_handle_t handle = (button_handle_t)data;
    button_event_t event = iot_button_get_event(handle);
    ESP_LOGI(TAG, "Touch event: %s", iot_button_get_event_str(event));
    if (event == BUTTON_PRESS_DOWN) {
        self->touch_detected_ = true;
        int64_t now = esp_timer_get_time();
        if (now - self->last_touch_sent_time_ > kTouchSendIntervalUs) {
            std::string payload = "{\"touch\":\"down\"}";
            Application::GetInstance().SendMcpMessage(payload);
            self->last_touch_sent_time_ = now;
            ESP_LOGI(TAG, "Touch event sent");
        }
    } else if (event == BUTTON_PRESS_UP) {
        self->touch_detected_ = false;
    }
}

TouchSensor::TouchSensor() : Thing("TouchSensor", "A simple touch sensor") {
    last_touch_sent_time_ = 0;
    // 初始化底层touch
    uint32_t touch_channel_list[] = {TOUCH_CHANNEL};
    touch_lowlevel_type_t channel_type[1] = {TOUCH_LOWLEVEL_TYPE_TOUCH};
    touch_lowlevel_config_t low_config = {
        .channel_num = 1,
        .channel_list = touch_channel_list,
        .channel_type = channel_type,
    };
    esp_err_t ret = touch_sensor_lowlevel_create(&low_config);
    assert(ret == ESP_OK);

    // 配置按钮，与官方例子保持一致（无短按/长按逻辑）
    const button_config_t btn_cfg = {0};
    button_touch_config_t touch_cfg = {
        .touch_channel = TOUCH_CHANNEL,
        .channel_threshold = TOUCH_THRESHOLD,
        .skip_lowlevel_init = true,
    };
    ret = iot_button_new_touch_button_device(&btn_cfg, &touch_cfg, &btn_handle_);
    assert(ret == ESP_OK);

    // 只注册PRESS_DOWN和PRESS_UP
    iot_button_register_cb(btn_handle_, BUTTON_PRESS_DOWN, nullptr, touch_event_cb, this);
    iot_button_register_cb(btn_handle_, BUTTON_PRESS_UP, nullptr, touch_event_cb, this);

    touch_sensor_lowlevel_start();

    properties_.AddBooleanProperty("touch_detected", "Whether the sensor is being touched", [this]() -> bool {
        return touch_detected_;
    });

    ESP_LOGI(TAG, "TouchSensor initialized on GPIO%d (channel %d)", TOUCH_CHANNEL, TOUCH_CHANNEL);
}

} // namespace iot 