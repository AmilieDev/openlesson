include(GNUInstallDirs)

set(BASE_DIR "${PROJECT_SOURCE_DIR}/openlesson-base")

if(APPLE)
    set_target_properties(openlesson-base PROPERTIES
        MACOSX_BUNDLE TRUE
        OUTPUT_NAME "OpenLesson"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.openlesson.base"
    )
    set(OPENLESSON_DATA_DIR "OpenLesson.app/Contents/Resources")
else()
    set(OPENLESSON_DATA_DIR "${CMAKE_INSTALL_DATADIR}/openlesson")
endif()

install(TARGETS openlesson-base
    BUNDLE DESTINATION .
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(DIRECTORY "${BASE_DIR}/docs" "${BASE_DIR}/tools"
    DESTINATION "${OPENLESSON_DATA_DIR}"
    PATTERN ".DS_Store" EXCLUDE
    PATTERN "__pycache__" EXCLUDE
)

install(FILES "${BASE_DIR}/src/gui/everforest.qss"
    DESTINATION "${OPENLESSON_DATA_DIR}"
)

qt_generate_deploy_app_script(
    TARGET openlesson-base
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT "${deploy_script}")