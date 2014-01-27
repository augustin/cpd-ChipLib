project(ChipLib)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/IO/Cif.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/Chip.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/Globals.cpp
         ../ChipDisplay/PainterLG.cpp)

add_library(ChipLib SHARED ${SRCS} ${MOC_SRCS})
target_link_libraries(ChipLib ${QT_LIBS})
