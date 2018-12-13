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
#include "rpi_wire.hpp"
#include "broadcom.hpp"
//#include <cstring>

TwoWire Wire;

TwoWire::TwoWire()
  : broadcom_(Broadcom::instance())
{
}

TwoWire::~TwoWire()
{
  this->end();
}

auto TwoWire::begin() -> void
{
  if(broadcom_->i2c_begin() == 0)
  {
    throw std::runtime_error("could not initialize i2c bus");
  }
  i2c_enabled_ = true;
}

auto TwoWire::end() -> void
{
  if(i2c_enabled_)
  {
    broadcom_->i2c_end();
  }
}

auto TwoWire::write(const std::string& s) -> size_t
{
  transmit_buffer_ += s;
  return s.size();
}

// returns: Size of buffer, otherwise 0 for error
// if 0 is returned all subsequent calls to read() will return -1 (no data)
auto TwoWire::requestFrom(addr_t address, size_t sz) 
  -> size_t
{
  broadcom_->i2c_set_slave_address(address);
  receive_buffer_.clear();
  receive_buffer_.resize(sz, 0);
  auto read_successful = broadcom_->i2c_read(receive_buffer_.data(), sz) 
    == Broadcom::I2CResult::Ok;
  auto bytes_written = sz;
  if(read_successful == false)
  {
    receive_buffer_.clear();
    bytes_written = 0;
  }
  read_position_ = receive_buffer_.begin();
  return bytes_written;
}

auto TwoWire::beginTransmission(addr_t a) -> void
{
  slave_addr_ = a;
  transmit_buffer_.clear();
}

auto TwoWire::write(char c) -> size_t 
{
  transmit_buffer_ += c;
  return 1;
}

auto TwoWire::write(const char* buf, size_t sz) -> size_t
{
  transmit_buffer_.append(buf, sz);
  return sz;
}

auto TwoWire::endTransmission() -> int
{
  int retval = 0;
  enum { Success, DataTooLong, NACK, Other };
  broadcom_->i2c_set_slave_address(slave_addr_);
  auto result_code = broadcom_->i2c_write
    ( transmit_buffer_.c_str()
    , transmit_buffer_.size());
  switch(result_code)
  {
  case Broadcom::I2CResult::Ok:
    retval = Success;
    break;
  case Broadcom::I2CResult::TransmissionIncomplete:
    retval = DataTooLong;
    break;
  case Broadcom::I2CResult::Nack:
    retval = NACK;
    break;
  default:
    retval = Other;
    break;
  }
  return retval;
}

auto TwoWire::read() -> uint8_t
{
  char ch = 0;
  if(read_position_ == receive_buffer_.end())
  {
    ch = -1;
  }
  else
  {
    ch = *read_position_++;
  }
  return ch;
}
