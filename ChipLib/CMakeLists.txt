project(ChipLib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/IO/Cif.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/Chip.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/Globals.cpp)

set(HDRS LayerGraphics.h)

qt_moc(MOC_SRCS ${HDRS})

add_library(ChipLib SHARED ${SRCS} ${MOC_SRCS})
target_link_libraries(ChipLib ${QT_LIBS})
