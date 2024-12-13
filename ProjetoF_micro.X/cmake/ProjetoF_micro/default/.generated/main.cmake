include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")
set(ProjetoF_micro_default_library_list )
# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm) 
if(ProjetoF_micro_default_FILE_GROUP_assemble)
    add_library(ProjetoF_micro_default_assemble OBJECT ${ProjetoF_micro_default_FILE_GROUP_assemble})
    ProjetoF_micro_default_assemble_rule(ProjetoF_micro_default_assemble)
    list(APPEND ProjetoF_micro_default_library_list "$<TARGET_OBJECTS:ProjetoF_micro_default_assemble>")
endif()

# Handle files with suffix S 
if(ProjetoF_micro_default_FILE_GROUP_assemblePreprocess)
    add_library(ProjetoF_micro_default_assemblePreprocess OBJECT ${ProjetoF_micro_default_FILE_GROUP_assemblePreprocess})
    ProjetoF_micro_default_assemblePreprocess_rule(ProjetoF_micro_default_assemblePreprocess)
    list(APPEND ProjetoF_micro_default_library_list "$<TARGET_OBJECTS:ProjetoF_micro_default_assemblePreprocess>")
endif()

# Handle files with suffix [cC] 
if(ProjetoF_micro_default_FILE_GROUP_compile)
    add_library(ProjetoF_micro_default_compile OBJECT ${ProjetoF_micro_default_FILE_GROUP_compile})
    ProjetoF_micro_default_compile_rule(ProjetoF_micro_default_compile)
    list(APPEND ProjetoF_micro_default_library_list "$<TARGET_OBJECTS:ProjetoF_micro_default_compile>")
endif()

if (BUILD_LIBRARY)
        message(STATUS "Building LIBRARY")
        add_library(${ProjetoF_micro_default_image_name} ${ProjetoF_micro_default_library_list})
        foreach(lib ${ProjetoF_micro_default_FILE_GROUP_link})
        target_link_libraries(${ProjetoF_micro_default_image_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR} /${lib})
        endforeach()
        add_custom_command(
            TARGET ${ProjetoF_micro_default_image_name}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ProjetoF_micro_default_output_dir}
    COMMAND ${CMAKE_COMMAND} -E copy lib${ProjetoF_micro_default_image_name}.a ${ProjetoF_micro_default_output_dir}/${ProjetoF_micro_default_original_image_name})
else()
    message(STATUS "Building STANDARD")
    add_executable(${ProjetoF_micro_default_image_name} ${ProjetoF_micro_default_library_list})
    foreach(lib ${ProjetoF_micro_default_FILE_GROUP_link})
    target_link_libraries(${ProjetoF_micro_default_image_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/${lib})
endforeach()
    ProjetoF_micro_default_link_rule(${ProjetoF_micro_default_image_name})
    
add_custom_command(
    TARGET ${ProjetoF_micro_default_image_name}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ProjetoF_micro_default_output_dir}
    COMMAND ${CMAKE_COMMAND} -E copy ${ProjetoF_micro_default_image_name} ${ProjetoF_micro_default_output_dir}/${ProjetoF_micro_default_original_image_name})
endif()
