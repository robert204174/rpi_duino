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
#ifndef BROADCOM_HPP_20181201_021428_PST
#define BROADCOM_HPP_20181201_021428_PST

#include <cstdint>
#include <memory>

auto delay(unsigned) -> void;

class Broadcom final
{
private:
  using reason_code_t = uint8_t;
  using size_t = uint32_t;
  Broadcom();
public:
  ~Broadcom();
  friend void delay(unsigned);
  enum class I2CResult 
    { Ok
    , Nack
    , ClockStrechTimeout
    , TransmissionIncomplete
    , Unknown
    };
  using i2c_addr_t = uint8_t;

  static auto instance() -> std::shared_ptr<Broadcom>;

  auto delay(unsigned) -> void;

  auto i2c_begin() -> int;
  auto i2c_end() -> void;
  auto i2c_set_slave_address(i2c_addr_t address) -> void;
  auto i2c_read(char*, size_t) -> I2CResult;
  auto i2c_write(const char*, size_t) -> I2CResult;

  auto i2c_result(reason_code_t) -> I2CResult;

};


#endif//BROADCOM_HPP_20181201_021428_PST
