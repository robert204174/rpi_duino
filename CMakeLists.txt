##############################################################################
# PROJECT: rpi_duino
project(rpi_duino C CXX)

add_library(rpi_duino
  rpi_duino.cpp
  broadcom.cpp
  )
target_link_libraries(rpi_duino bcm2835)
