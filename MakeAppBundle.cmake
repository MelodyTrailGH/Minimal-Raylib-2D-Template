file(
  GLOB_RECURSE
  RESOURCES
  "${CMAKE_CURRENT_LIST_DIR}/Resources/*.png"
  "${CMAKE_CURRENT_LIST_DIR}/Resources/.*jpg"
  "${CMAKE_CURRENT_LIST_DIR}/Resources/*.bmp"
  "${CMAKE_CURRENT_LIST_DIR}/Resources/*.wav"
  "${CMAKE_CURRENT_LIST_DIR}/Resources/*.ogg"
  "${CMAKE_CURRENT_LIST_DIR}/Resources/*.fnt"
  "${CMAKE_CURRENT_LIST_DIR}/Resources/*.ttf")

if(APPLE)
  set(APPBUNDLE_DIR "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app")
  set(APPBUNDLE_BIN_DIR "${APPBUNDLE_DIR}/Contents/MacOS")
  set(APPBUNDLE_RESOURCE_DIR "${APPBUNDLE_DIR}/Contents/Resources")
  set(APPBUNDLE_LIBRARY_DIR "${APPBUNDLE_DIR}/Contents/Frameworks")

  file(GLOB_RECURSE RAYLIB_LIBS "${raylib_BINARY_DIR}/*.dylib")

  add_custom_command(
    TARGET ${TARGET}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APPBUNDLE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APPBUNDLE_DIR}/Contents"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APPBUNDLE_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APPBUNDLE_RESOURCE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APPBUNDLE_LIBRARY_DIR}")

  add_custom_command(
    TARGET ${TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET}>
            "${APPBUNDLE_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCES} "${APPBUNDLE_RESOURCE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:raylib>
            "${APPBUNDLE_LIBRARY_DIR}")
endif()
