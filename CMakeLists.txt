<<<<<<< HEAD
cmake_minimum_required(VERSION 3.7)
=======
cmake_minimum_required(VERSION 3.10)
>>>>>>> 5bcb533bb9d9ed87b21db658a268da687f66efab

##############################################################################
# PROJECT: rpi_duino
project(rpi_duino C CXX)

add_library(rpi_duino
  rpi_duino.cpp
  broadcom.cpp
  )
target_link_libraries(rpi_duino bcm2835)
