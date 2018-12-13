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
#ifndef RPI_WIRE_HPP_20181201_003445_PST
#define RPI_WIRE_HPP_20181201_003445_PST

#include "broadcom.hpp"
#include <cstdint>
#include <boost/lexical_cast.hpp>
#include <vector>

class TwoWire final
{
  using addr_t = Broadcom::i2c_addr_t;
  using size_t = std::size_t;
  using queue_t = std::vector<char>;
public:
  TwoWire();
  ~TwoWire();
  auto begin() -> void;
  auto end() -> void;
  auto requestFrom(addr_t, size_t) -> size_t;
  auto beginTransmission(addr_t) -> void;
  auto write(char v) -> size_t;
  auto write(const std::string& s) -> size_t;
  auto write(const char*, size_t) -> size_t;
  auto endTransmission() -> int;
  auto read() -> uint8_t;
private:
  std::shared_ptr<Broadcom>   broadcom_;
  addr_t                      slave_addr_ = 0;
  std::string                 transmit_buffer_;
  queue_t                     receive_buffer_;
  queue_t::const_iterator     read_position_ = receive_buffer_.begin();
  bool                        i2c_enabled_ = false;
};

extern TwoWire Wire;

#endif//RPI_WIRE_HPP_20181201_003445_PST
