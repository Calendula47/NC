# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\NC_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\NC_autogen.dir\\ParseCache.txt"
  "NC_autogen"
  )
endif()
