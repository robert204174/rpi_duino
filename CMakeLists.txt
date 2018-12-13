cmake_minimum_required(VERSION 3.13)

##############################################################################
# PROJECT: rpi_wire
project(rpi_wire C CXX)

add_library(rpi_wire
  rpi_wire.cpp
  broadcom.cpp
  )
target_link_libraries(rpi_wire bcm2835)
