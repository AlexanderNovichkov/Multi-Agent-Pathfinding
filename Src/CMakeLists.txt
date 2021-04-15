
set(HEADERS         config.h
                    environmentoptions.h
                    gl_const.h
                    ilogger.h
                    map.h
                    mission.h
                    basicstructures.h
                    search.h
        searchstructures.h
                    searchresult.h
                    tinyxml2.h
                    xmllogger.h
                    heuristics.h)

set(SOURCES         config.cpp
                    environmentoptions.cpp
                    map.cpp
                    mission.cpp
                    search.cpp
        searchstructures.cpp
                    tinyxml2.cpp
                    xmllogger.cpp
                    heuristics.cpp
                    basicstructures.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} main.cpp)

add_library(PPLIB ${HEADERS} ${SOURCES})

install(TARGETS ${PROJECT_NAME} 
        CONFIGURATIONS Debug
        DESTINATION Bin/Debug)

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        DESTINATION Bin/Release)
