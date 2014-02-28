project(ChipLib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS IO/Cif.cpp Chip.cpp Globals.cpp
         ../ChipDisplay/GDLG.cpp)

add_library(ChipLib SHARED ${SRCS} ${MOC_SRCS})
target_link_libraries(ChipLib ${QT_LIBS} gd)
