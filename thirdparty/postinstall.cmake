message(STATUS "===== POST INSTALL =====")

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/../build")
set(DST "${CMAKE_CURRENT_SOURCE_DIR}/../install/${CMAKE_INSTALL_CONFIG_NAME}")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../install/${CMAKE_INSTALL_CONFIG_NAME}")
message(STATUS "Source      : ${SRC}")
message(STATUS "Destination : ${DST}")

file(MAKE_DIRECTORY "${DST}")

file(COPY
    "${SRC}/"
    DESTINATION "${DST}"
)

message(STATUS "===== COPY FINISHED =====")