#include "iot/thing.h"
#include "board.h"
#include "audio_codec.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/uart_select.h"
#include <string.h>

#include <esp_log.h>
#include <string>

#define TAG "gassensor"

namespace iot {

// 这里仅定义 gassensor 的属性和方法，不包含具体的实现
class gassensor : public Thing {
private:
    // 这里可以添加 gassensor 的私有成员变量和方法
    const uart_port_t uart_num = UART_NUM_1;
    const int uart_buffer_size = 1024; // UART 缓冲区大小
    const int uart_read_bytes_timeout = 20; // UART 读取超时时间
    char* data = (char*)malloc(128); // 数据缓冲区
    int length = 0;
    const char* test_str = "No data received";
    
    // 初始化串口配置
        void InitializeUart() {
        uart_config_t uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_APB,
        };
        ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, uart_buffer_size, 0, 0, NULL, 0));
        ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
        ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, GPIO_NUM_10, GPIO_NUM_11, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE)); //tx, rx, rts, cts
        ESP_ERROR_CHECK(uart_set_rx_timeout(UART_NUM_1, uart_read_bytes_timeout));
    }

public:
gassensor() : Thing("gassensor", "多通道气体传感器") {
        InitializeUart();
        memset(data, 0, 128); // 初始化数据缓冲区
        ESP_LOGI(TAG, "UART initialized successfully");
    
    // 定义设备的属性
        properties_.AddStringProperty("NO2", "当前二氧化氮浓度值", [this]() -> std::string {
            const char* test_str_1 = "1\n";
            uart_write_bytes(uart_num, (const char*)test_str_1, strlen(test_str_1));
            ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length));
            length = uart_read_bytes(uart_num, data, length, 100);
            if(length) {
                data[length] = '\0'; // 确保字符串以 null 结尾
                ESP_LOGI(TAG, "Received data: %.*s", length, data);
                return data;
            } else {
                ESP_LOGI(TAG, "No data received");
                return test_str; // Return a default string when no data is received
            }
        });

        properties_.AddStringProperty("C2H5CH", "当前酒精浓度值", [this]() -> std::string {
            const char* test_str_2 = "2\n";
            uart_write_bytes(uart_num, (const char*)test_str_2, strlen(test_str_2));
            ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length));
            length = uart_read_bytes(uart_num, data, length, 100);
            if(length) {
                data[length] = '\0'; // 确保字符串以 null 结尾
                ESP_LOGI(TAG, "Received data: %.*s", length, data);
                return data;
            } else {
                ESP_LOGI(TAG, "No data received");
                return test_str; // Return a default string when no data is received
            }
        });

        properties_.AddStringProperty("VOC", "当前VOC浓度值", [this]() -> std::string {
            const char* test_str_3 = "3\n";
            uart_write_bytes(uart_num, (const char*)test_str_3, strlen(test_str_3));
            ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length));
            length = uart_read_bytes(uart_num, data, length, 100);
            if(length) {
                data[length] = '\0'; // 确保字符串以 null 结尾
                ESP_LOGI(TAG, "Received data: %.*s", length, data);
                return data;
            } else {
                ESP_LOGI(TAG, "No data received");
                return test_str; // Return a default string when no data is received
            }
        });

        properties_.AddStringProperty("CO", "当前一氧化碳浓度值", [this]() -> std::string {
            const char* test_str_4 = "4\n";
            uart_write_bytes(uart_num, (const char*)test_str_4, strlen(test_str_4));
            ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length));
            length = uart_read_bytes(uart_num, data, length, 100);
            if(length) {
                data[length] = '\0'; // 确保字符串以 null 结尾
                ESP_LOGI(TAG, "Received data: %.*s", length, data);
                return data;
            } else {
                ESP_LOGI(TAG, "No data received");
                return test_str; // Return a default string when no data is received
            }
        });

        // // 定义设备可以被远程执行的指令
        // methods_.AddMethod("SetVolume", "设置音量", ParameterList({
        //     Parameter("volume", "0到100之间的整数", kValueTypeNumber, true)
        // }), [this](const ParameterList& parameters) {
        //     auto codec = Board::GetInstance().GetAudioCodec();
        //     codec->SetOutputVolume(static_cast<uint8_t>(parameters["volume"].number()));
        // });
    }
};

} // namespace iot

DECLARE_THING(gassensor);