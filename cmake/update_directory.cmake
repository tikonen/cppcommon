#
# Copy changed files from runtime/common to the binary folder after
# successful build
#
# add_custom_command(TARGET ${_target}
#   POST_BUILD
#   COMMAND ${CMAKE_COMMAND}
#     -Dsrc_dir="${_runtime_path}/common"
#     -Ddst_dir="$<TARGET_FILE_DIR:${_target}>"
#     -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/update_directory.cmake"
# )
#

file(GLOB_RECURSE _file_list RELATIVE "${src_dir}" "${src_dir}/*")

foreach( each_file ${_file_list} )
  set(destinationfile "${dst_dir}/${each_file}")
  set(sourcefile "${src_dir}/${each_file}")
  if(NOT EXISTS ${destinationfile} OR ${sourcefile} IS_NEWER_THAN ${destinationfile})
    get_filename_component(destinationdir ${destinationfile} DIRECTORY)
    file(COPY ${sourcefile} DESTINATION ${destinationdir})
  endif()
endforeach(each_file)
