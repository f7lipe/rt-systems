# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/f7lipe/esp/esp-idf/components/bootloader/subproject"
  "/Users/f7lipe/Desktop/cyclical/build/bootloader"
  "/Users/f7lipe/Desktop/cyclical/build/bootloader-prefix"
  "/Users/f7lipe/Desktop/cyclical/build/bootloader-prefix/tmp"
  "/Users/f7lipe/Desktop/cyclical/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/f7lipe/Desktop/cyclical/build/bootloader-prefix/src"
  "/Users/f7lipe/Desktop/cyclical/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/f7lipe/Desktop/cyclical/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/f7lipe/Desktop/cyclical/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
