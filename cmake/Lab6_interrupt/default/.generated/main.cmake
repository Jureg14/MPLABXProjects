include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")
set(Lab6_interrupt_default_library_list )
# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm) 
if(Lab6_interrupt_default_FILE_GROUP_assemble)
    add_library(Lab6_interrupt_default_assemble OBJECT ${Lab6_interrupt_default_FILE_GROUP_assemble})
    Lab6_interrupt_default_assemble_rule(Lab6_interrupt_default_assemble)
    list(APPEND Lab6_interrupt_default_library_list "$<TARGET_OBJECTS:Lab6_interrupt_default_assemble>")
endif()

# Handle files with suffix S 
if(Lab6_interrupt_default_FILE_GROUP_assemblePreprocess)
    add_library(Lab6_interrupt_default_assemblePreprocess OBJECT ${Lab6_interrupt_default_FILE_GROUP_assemblePreprocess})
    Lab6_interrupt_default_assemblePreprocess_rule(Lab6_interrupt_default_assemblePreprocess)
    list(APPEND Lab6_interrupt_default_library_list "$<TARGET_OBJECTS:Lab6_interrupt_default_assemblePreprocess>")
endif()

# Handle files with suffix [cC] 
if(Lab6_interrupt_default_FILE_GROUP_compile)
    add_library(Lab6_interrupt_default_compile OBJECT ${Lab6_interrupt_default_FILE_GROUP_compile})
    Lab6_interrupt_default_compile_rule(Lab6_interrupt_default_compile)
    list(APPEND Lab6_interrupt_default_library_list "$<TARGET_OBJECTS:Lab6_interrupt_default_compile>")
endif()

if (BUILD_LIBRARY)
        message(STATUS "Building LIBRARY")
        add_library(${Lab6_interrupt_default_image_name} ${Lab6_interrupt_default_library_list})
        foreach(lib ${Lab6_interrupt_default_FILE_GROUP_link})
        target_link_libraries(${Lab6_interrupt_default_image_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR} /${lib})
        endforeach()
        add_custom_command(
            TARGET ${Lab6_interrupt_default_image_name}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${Lab6_interrupt_default_output_dir}
    COMMAND ${CMAKE_COMMAND} -E copy lib${Lab6_interrupt_default_image_name}.a ${Lab6_interrupt_default_output_dir}/${Lab6_interrupt_default_original_image_name})
else()
    message(STATUS "Building STANDARD")
    add_executable(${Lab6_interrupt_default_image_name} ${Lab6_interrupt_default_library_list})
    foreach(lib ${Lab6_interrupt_default_FILE_GROUP_link})
    target_link_libraries(${Lab6_interrupt_default_image_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/${lib})
endforeach()
    Lab6_interrupt_default_link_rule(${Lab6_interrupt_default_image_name})
    
add_custom_command(
    TARGET ${Lab6_interrupt_default_image_name}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${Lab6_interrupt_default_output_dir}
    COMMAND ${CMAKE_COMMAND} -E copy ${Lab6_interrupt_default_image_name} ${Lab6_interrupt_default_output_dir}/${Lab6_interrupt_default_original_image_name})
endif()
