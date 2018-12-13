/* This file is part of the rpi_wire project.
 * Copyright (c) 2018 by Robert T. Adams                                  
 *                                                                        
 * This program is free software: you can redistribute it and/or modify   
 * it under the terms of the GNU General Public License, version 2, as    
 * published by the Free Software Foundation.                             
 *                                                                                                                                                
 * This program is distributed in the hope that it will be useful,        
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          
 * GNU General Public License for more details.                           
 *                                                                                                                                                                                                                       
 * You should have received a copy of the GNU General Public License      
 * along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *
 */
#include "broadcom.hpp"
//#include <stdexcept> 
#include <bcm2835.h>
//#include <cassert>

auto Broadcom::instance() -> std::shared_ptr<Broadcom>
{
  static std::shared_ptr<Broadcom> instance(new Broadcom());
  return instance;
}

#pragma push_macro("delay")
#ifdef delay
#undef delay
auto delay(unsigned d) -> void
#endif
{
  auto bc = Broadcom::instance();
  bc->delay(d);
}
#pragma pop_macro("delay")

Broadcom::Broadcom()
{
  if(bcm2835_init() == 0)
  {
    throw std::runtime_error("Could not initialize bcm2835 library");
  }
}

Broadcom::~Broadcom()
{
  bcm2835_close();
}

namespace
{
  auto impl_delay(unsigned d) -> void
  {
    ::delay(d);
  }
#if defined(delay)
#undef delay
#endif
}
auto Broadcom::delay(unsigned d) -> void
{
  impl_delay(d);
}

auto Broadcom::i2c_begin() -> int 
{
  return bcm2835_i2c_begin();
}
auto Broadcom::i2c_end() -> void
{
  bcm2835_i2c_end();
}

auto Broadcom::i2c_set_slave_address(i2c_addr_t address) -> void
{
  bcm2835_i2c_setSlaveAddress(address);
}

auto Broadcom::i2c_result(reason_code_t rc) -> I2CResult
{
  I2CResult result;
  switch(rc)
  {
  case BCM2835_I2C_REASON_OK:
    result = I2CResult::Ok;
    break;
  case BCM2835_I2C_REASON_ERROR_NACK:
    result = I2CResult::Nack;
    break;
  case BCM2835_I2C_REASON_ERROR_CLKT:
    result = I2CResult::ClockStrechTimeout;
    break;
  case BCM2835_I2C_REASON_ERROR_DATA:
    result = I2CResult::TransmissionIncomplete;
    break;
  default:
    result = I2CResult::Unknown;
  }
  return result;
}

auto Broadcom::i2c_write(const char* buf, size_t sz) -> I2CResult
{
  return i2c_result(bcm2835_i2c_write(buf, sz));
}

auto Broadcom::i2c_read(char* buf, size_t sz) -> I2CResult
{
  return i2c_result(bcm2835_i2c_read(buf, sz));
}
