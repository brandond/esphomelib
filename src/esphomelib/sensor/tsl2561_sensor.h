//
//  tsl2561_sensor.h
//  esphomelib
//
//  Created by Otto Winter on 09.05.18.
//  Copyright © 2018 Otto Winter. All rights reserved.
//

#ifndef ESPHOMELIB_SENSOR_TSL2561_SENSOR_H
#define ESPHOMELIB_SENSOR_TSL2561_SENSOR_H

#include "esphomelib/i2c_component.h"
#include "esphomelib/sensor/sensor.h"
#include "esphomelib/defines.h"

#ifdef USE_TSL2561

ESPHOMELIB_NAMESPACE_BEGIN

namespace sensor {

/** Enum listing all conversion/integration time settings for the TSL2561
 *
 * Higher values mean more accurate results, but will take more energy/more time.
 */
enum TSL2561IntegrationTime {
  TSL2561_INTEGRATION_14MS = 0b00,
  TSL2561_INTEGRATION_101MS = 0b01,
  TSL2561_INTEGRATION_402MS = 0b10,
};

/** Enum listing all gain settings for the TSL2561.
 *
 * Higher values are better for low light situations, but can increase noise.
 */
enum TSL2561Gain {
  TSL2561_GAIN_1X = 0,
  TSL2561_GAIN_16X = 1,
};

/// This class includes support for the TSL2561 i2c ambient light sensor.
class TSL2561Sensor : public PollingSensorComponent, public I2CDevice {
 public:
  TSL2561Sensor(I2CComponent *parent, const std::string &name, uint8_t address = 0x39,
                uint32_t update_interval = 15000);

  /** Set the time that sensor values should be accumulated for.
   *
   * Longer means more accurate, but also mean more power consumption.
   *
   * Possible values are:
   *
   *  - `sensor::TSL2561_INTEGRATION_14MS`
   *  - `sensor::TSL2561_INTEGRATION_101MS`
   *  - `sensor::TSL2561_INTEGRATION_402MS` (default)
   *
   * @param integration_time The new integration time.
   */
  void set_integration_time(TSL2561IntegrationTime integration_time);

  /** Set the internal gain of the sensor. Can be useful for low-light conditions
   *
   * Possible values are:
   *
   *  - `sensor::TSL2561_GAIN_1X` (default)
   *  - `sensor::TSL2561_GAIN_16X`
   *
   * @param gain The new gain.
   */
  void set_gain(TSL2561Gain gain);

  /** The "CS" package of this sensor has a slightly different formula for
   * converting the raw values. Use this setting to indicate that this is a CS
   * package. Defaults to false (not a CS package)
   *
   * @param package_cs Is this a CS package.
   */
  void set_is_cs_package(bool package_cs);

  // ========== INTERNAL METHODS ==========
  // (In most use cases you won't need these)
  void setup() override;
  void update() override;
  std::string unit_of_measurement() override;
  std::string icon() override;
  int8_t accuracy_decimals() override;
  float get_setup_priority() const override;

  bool tsl2561_read_byte(uint8_t register_, uint8_t *value);
  bool tsl2561_read_uint(uint8_t register_, uint16_t *value);
  bool tsl2561_write_byte(uint8_t register_, uint8_t value);

 protected:
  float get_integration_time_ms_();
  void read_data_();
  float calculate_lx_(uint16_t ch0, uint16_t ch1);

  TSL2561IntegrationTime integration_time_{TSL2561_INTEGRATION_402MS};
  TSL2561Gain gain_{TSL2561_GAIN_1X};
  bool package_cs_{false};
};

} // namespace sensor

ESPHOMELIB_NAMESPACE_END

#endif //USE_TSL2561

#endif //ESPHOMELIB_SENSOR_TSL2561_SENSOR_H
