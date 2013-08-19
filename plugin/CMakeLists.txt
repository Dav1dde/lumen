find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include_directories(${KDE4_INCLUDES})

set(lumen_SRCS
    lumen.cpp
    dcd.cpp
    completion.cpp
)

kde4_add_plugin(lumen ${lumen_SRCS})

target_link_libraries(lumen
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
)

install(TARGETS lumen DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES lumen.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES lumen.rc  DESTINATION  ${DATA_INSTALL_DIR}/lumen )
