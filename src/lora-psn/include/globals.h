#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <Arduino.h>

#define DATAID_MASK(req) (req & 0xf0)
#define BOARDID_MASK(req) (req & 0x0f)
#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef float f32;
typedef double f64;

/**
 * @brief Template struct for single value sensor data.
 * @tparam T Field variable type
 */
template <typename T>
struct SensorValues
{
    T temperature;
    T pressure;
    T humidity;
};

/**
 * @brief Template struct for sensor data that needs an array.
 * @tparam T Field variable type
 * @tparam N Array size
 */
template <typename T, size_t size>
struct SensorData
{
    T temperature[size];
    T pressure[size];
    T humidity[size];
};

#endif /* _GLOBALS_H */