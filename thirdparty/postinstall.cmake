message(STATUS "===== POST INSTALL =====")


file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../install/include")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../install/${CMAKE_INSTALL_CONFIG_NAME}/lib")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../install/${CMAKE_INSTALL_CONFIG_NAME}/bin")


set(SRC_VMA_INC "${CMAKE_CURRENT_SOURCE_DIR}/../VulkanMemoryAllocator/install/include")
set(SRC_INC "${CMAKE_CURRENT_SOURCE_DIR}/../build/include")
set(SRC_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../build/lib")
set(SRC_BIN "${CMAKE_CURRENT_SOURCE_DIR}/../build/bin")
set(SRC_SDL "${CMAKE_CURRENT_SOURCE_DIR}/SDL/${CMAKE_INSTALL_CONFIG_NAME}")

set(DST "${CMAKE_CURRENT_SOURCE_DIR}/../install/${CMAKE_INSTALL_CONFIG_NAME}")
set(DST_INC "${CMAKE_CURRENT_SOURCE_DIR}/../install/include")
set(DST_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../install/${CMAKE_INSTALL_CONFIG_NAME}/lib")
set(DST_BIN "${CMAKE_CURRENT_SOURCE_DIR}/../install/${CMAKE_INSTALL_CONFIG_NAME}/bin")



file(COPY
    "${SRC_VMA_INC}/"
    DESTINATION "${DST_INC}"
)

file(COPY
    "${SRC_INC}/"
    DESTINATION "${DST_INC}"
)

file(COPY
    "${SRC_LIB}/"
    DESTINATION "${DST_LIB}"
)

file(COPY
    "${SRC_BIN}/"
    DESTINATION "${DST_BIN}"
)


file(GLOB SDL_LIBS
    "${SRC_SDL}/*.lib"
)

file(COPY
    ${SDL_LIBS}
    DESTINATION "${DST_LIB}"
)

file(GLOB SDL_BINS
    "${SRC_SDL}/*.dll"
)

file(COPY
    ${SDL_BINS}
    DESTINATION "${DST_BIN}"
)

message(STATUS "===== COPY FINISHED =====")