find_package(tesseract_common REQUIRED)

add_executable(${PROJECT_NAME}_parse_srdf_example parse_srdf_example.cpp)
target_link_libraries(
  ${PROJECT_NAME}_parse_srdf_example
  ${PROJECT_NAME}
  tesseract::tesseract_common
  console_bridge::console_bridge)
target_compile_options(${PROJECT_NAME}_parse_srdf_example PRIVATE ${TESSERACT_COMPILE_OPTIONS_PRIVATE}
                                                                  ${TESSERACT_COMPILE_OPTIONS_PUBLIC})
target_clang_tidy(${PROJECT_NAME}_parse_srdf_example ENABLE ${TESSERACT_ENABLE_CLANG_TIDY})
target_cxx_version(${PROJECT_NAME}_parse_srdf_example PRIVATE VERSION ${TESSERACT_CXX_VERSION})
add_dependencies(${PROJECT_NAME}_parse_srdf_example ${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME}_parse_srdf_example DESTINATION bin)
