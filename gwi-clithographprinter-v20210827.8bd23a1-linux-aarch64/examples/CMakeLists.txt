set(EXAMPLES
  card
  duplex
  mag
  simplex
  status
  version
  text
  barcode
  nonPrintingAreas
  gwiCardFlow
  invertFace
  printingMode
  pageRotation
  cardOrientation
)

include_directories(${PROJECT_SOURCE_DIR}/src)

if(STATIC_EXAMPLES)
  add_definitions(-DSTATIC_EXAMPLES -Wno-unused-parameter)
  link_libraries(
    Evolis_Avansia_LithographPrinter
    Evolis_Elypso_LithographPrinter
    Evolis_Zenius_LithographPrinter
  )
endif()

set(SOURCES main.cpp)
if(MSVC OR MINGW)
  list(APPEND SOURCES win32/dlfcn.c)
else()
  link_libraries(dl)
endif()

foreach(ITEM ${EXAMPLES})
  add_executable(${ITEM} ${ITEM}.cpp ${SOURCES})
  install(TARGETS ${ITEM} DESTINATION bin)
endforeach()
