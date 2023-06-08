#include "lora.h"
#include "bme280.h"
#include "debug.h"

#define UPPER_BITMASK 0xff00
#define LOWER_BITMASK 0x00ff

HardwareSerial SerialLora(UART1_RX, UART1_TX);

namespace lora
{

    void shieldInit(BoardType_t type, u8 _sf, u8 boardId)
    {
        switch (type)
        {
        case SLAVE:
            debug::println(debug::INFO, "SLAVE module 0x0" + String(boardId, HEX));
            break;

        case MASTER:
            debug::println(debug::INFO, "MASTER module 0x0" + String(boardId, HEX));
            break;
        }

        while (!loraRadio.begin(&SerialLora))
        {
            debug::println(debug::INFO, "Shield not ready...");
            delay(1000); // Give the module 1s to init
        }

        loraRadio.setSpreadingFactor(_sf);

        debug::println(debug::INFO, "Shield ready!");
    }

    void sendRequest(u8 message)
    {
        u8 msg[] = {message};
        debug::println(debug::INFO, "Sending new requst with value 0x" + String(message, HEX));

        loraRadio.write(msg, sizeof(msg));
    }

    // Only for SLAVE modules
    void sendResponse(sensor::BufferData *buffer, u8 reqMsg)
    {
        /**
         * @brief Payload of the response. First byte is request code echo,
         * then 2 bytes of response, MSB-order.
         */
        u8 message[3];
        u16 bufferValue;

        switch (DATAID_MASK(reqMsg))
        {
        case TEMPERATURE:
            bufferValue = (u16)buffer->temperature;
            break;

        case PRESSURE:
            bufferValue = (u16)buffer->pressure;
            break;

        case HUMIDITY:
            bufferValue = (u16)buffer->humidity;
            break;
        }

        message[0] = reqMsg;
        message[1] = (bufferValue & UPPER_BITMASK) >> 8;
        message[2] = (bufferValue & LOWER_BITMASK);
        debug::println(debug::INFO, "Sending response: 0x" + String(message[0], HEX) + "\t" + String(message[1]) + "\t" + String(message[2]));

        loraRadio.write(message, sizeof(message));
    }

    // Only for MASTER module
    void readResponse(ReceivedData *data, u8 message[])
    {
        u8 boardId = BOARDID_MASK(message[0]) - 1;
        // Merge each 2x 8-bit fields into 1x 16-bit one, fix magnitudes
        switch (DATAID_MASK(message[0]))
        {
        case TEMPERATURE:
            data->temperature[boardId] = (f32)((message[1] << 8) + message[2]) / 100;
            break;

        case PRESSURE:
            data->pressure[boardId] = (f32)((message[1] << 8) + message[2]);
            break;

        case HUMIDITY:
            data->humidity[boardId] = (f32)((message[1] << 8) + message[2]) / 100;
            break;
        }
    }
}
