# -----------------------------------------------------------------------------
# Различные функции для упрощения кода
# -----------------------------------------------------------------------------

# Экспортирует переменную VariableName, содержащую все .h, .c, .cpp, .tpp .hpp из директории Folder по шаблону File
function(get_list Folder File VariableName)
    file(GLOB List "${CMAKE_CURRENT_LIST_DIR}/${Folder}/${File}.h" "${CMAKE_CURRENT_LIST_DIR}/${Folder}/${File}.c" "${CMAKE_CURRENT_LIST_DIR}/${Folder}/${File}.cpp" "${CMAKE_CURRENT_LIST_DIR}/${Folder}/${File}.tpp" "${CMAKE_CURRENT_LIST_DIR}/${Folder}/${File}.hpp")
    set(${VariableName} ${${VariableName}} ${List} PARENT_SCOPE)
endfunction()

# Экспортирует переменную VariableName, содержащую все .h, .c, .cpp, .tpp .hpp из директории Folder по шаблону File (включая поддиректории)
function(get_list_recurse Folder File VariableName)
    file(GLOB_RECURSE List "${CMAKE_CURRENT_LIST_DIR}/${Folder}/${File}.h" "${CMAKE_CURRENT_LIST_DIR}/${Folder}/${File}.c" "${CMAKE_CURRENT_LIST_DIR}/${Folder}/${File}.cpp" "${CMAKE_CURRENT_LIST_DIR}/${Folder}/${File}.tpp" "${CMAKE_CURRENT_LIST_DIR}/${Folder}/${File}.hpp")
    set(${VariableName} ${${VariableName}} ${List} PARENT_SCOPE)
endfunction()

# Исключает из перечисления List все .h, .c, .cpp, .tpp .hpp файлы по шаблонам, перечисленным после List, содержащиеся в директории Folder
function(do_list_exclude Folder List)
    foreach (File ${ARGN})
        get_list_recurse(${Folder} ${File} Exclude_List)
    endforeach ()
    list(REMOVE_ITEM ${List} ${Exclude_List})
    set(${List} ${${List}} PARENT_SCOPE)
endfunction()

# Экспортирует переменную VariableName, содержащую все .h, .c, .cpp, .tpp .hpp из директории Folder (включая поддиректории).
#   Если после VariableName идет IN, то включаются только последующие шаблоны.
#   Если после VariableName идет EX, то из общего списка исключаются последующие шаблоны
function(list_sources Folder VariableName)
    if (ARGC GREATER 3 AND ARGV2 STREQUAL "IN")
        list(REMOVE_AT ARGN 0)
        foreach (File ${ARGN})
            get_list_recurse(${Folder} ${File} Sources_Local)
        endforeach ()
    else ()
        get_list_recurse(${Folder} "*" Sources_Local)
        if (ARGC GREATER 3 AND ARGV2 STREQUAL "EX")
            list(REMOVE_AT ARGN 0)
            do_list_exclude(${Folder} Sources_Local ${ARGN})
        endif ()
    endif ()
    set(${VariableName} ${${VariableName}} ${Sources_Local} PARENT_SCOPE)
endfunction()

# Экспортирует переменную VariableName, содержащую все .h, .c, .cpp, .tpp .hpp из директорий Common_Folder и Specific_Folder (включая поддиректории).
#   Если файл есть в обеих директориях, то предпочтение дается Specific_Folder.
#   Если после VariableName идет IN, то включаются только последующие шаблоны.
#   Если после VariableName идет EX, то из общего списка исключаются последующие шаблоны
function(list_sources_overload Common_Folder Specific_Folder VariableName)
    get_list(${Common_Folder} "*" Common_Sources)
    get_list(${Specific_Folder} "*" Specific_Sources)

    if (ARGC GREATER 4 AND ARGV3 STREQUAL "EX")
        list(REMOVE_AT ARGN 0)
        do_list_exclude(${Common_Folder} Common_Sources ${ARGN})
        do_list_exclude(${Specific_Folder} Specific_Sources ${ARGN})
    endif ()

    foreach (Specific_File ${Specific_Sources})
        get_filename_component(Specific_Name ${Specific_File} NAME)
        list(FILTER Common_Sources EXCLUDE REGEX "${Specific_Name}$")
    endforeach ()

    set(${VariableName} ${${VariableName}} ${Common_Sources} ${Specific_Sources} PARENT_SCOPE)
endfunction()

# Делает add_subdirectory() для каждого найденного в поддиректориях CMakeLists.txt
function(add_all_subdirectories Folder)
    file(GLOB_RECURSE CMakes_Local "${CMAKE_CURRENT_SOURCE_DIR}/${Folder}/*/CMakeLists.txt")

    foreach (SubCMake ${CMakes_Local})
        get_filename_component(SubCMakeFolder ${SubCMake} DIRECTORY)
        add_subdirectory(${SubCMakeFolder})
    endforeach ()
endfunction()

# Проверяем, поддерживаем ли мы данную ОС
function(check_os)
    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
        message(STATUS " *** Операционная система определена как Linux ***")
    elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        message(STATUS " *** Операционная система определена как Windows ***")
    else ()
        message(FATAL_ERROR " *** Операционная система не поддерживается ***")
    endif ()
endfunction()

# Ищет файлы, у которых забыли написать описание
function(check_headers_for_description)
    # Список каталогов для поиска файлов
    set(SEARCH_DIRS
            "${PROJECT_SOURCE_DIR}/Drivers"
            "${PROJECT_SOURCE_DIR}/HAL"
            "${PROJECT_SOURCE_DIR}/Logic"
            "${PROJECT_SOURCE_DIR}/Platforms"
            "${PROJECT_SOURCE_DIR}/Shared"
    )

    # Список расширений для поиска
    set(FILE_EXTENSIONS "*.h" "*.hpp" "*.c" "*.cpp" "*.ipp" "*.tpp" "*.hpp")

    # Инициализация списка файлов
    set(ALL_FILES "")

    # Рекурсивный поиск файлов в каждом каталоге
    foreach (DIR ${SEARCH_DIRS})
        foreach (EXT ${FILE_EXTENSIONS})
            file(GLOB_RECURSE DIR_FILES
                    LIST_DIRECTORIES false
                    ${DIR}/${EXT}
            )
            list(APPEND ALL_FILES ${DIR_FILES})
        endforeach ()
    endforeach ()

    # Игнорируем привнесённые файлы
    set(FILTERED_FILES "")
    foreach (file ${ALL_FILES})
        if (NOT file MATCHES ".*ThirdParty.*"
                AND NOT file MATCHES ".*Drivers.*Libraries.*")
            list(APPEND FILTERED_FILES ${file})
        endif ()
    endforeach ()

    # Группировка файлов по имени без расширения
    unset(FILES_MAP_KEYS)
    foreach (file ${FILTERED_FILES})
        file(RELATIVE_PATH REL_FILE ${PROJECT_SOURCE_DIR} ${file})
        get_filename_component(NAME_WE ${REL_FILE} NAME_WE)
        get_filename_component(REL_DIR ${REL_FILE} DIRECTORY)
        string(REPLACE "/" "_" SAFE_DIR ${REL_DIR})
        list(APPEND FILES_MAP_KEYS ${SAFE_DIR}_${NAME_WE})
        list(APPEND FILES_MAP_${SAFE_DIR}_${NAME_WE} ${file})
    endforeach ()
    list(REMOVE_DUPLICATES FILES_MAP_KEYS)

    # Проверка файлов на отсутствие строки \file \brief или отсутствие описания после нее
    set(BAD_FILES "")
    foreach (NAME_WE ${FILES_MAP_KEYS})
        set(SIGNED_FILE_FOUND FALSE)
        foreach (file ${FILES_MAP_${NAME_WE}})
            file(READ ${file} FILE_CONTENTS LIMIT 256)
            if (FILE_CONTENTS MATCHES " \\\\file.* \\\\brief +[^ \t\r\n]")
                set(SIGNED_FILE_FOUND TRUE)
                break()
            endif ()
        endforeach ()
        if (NOT SIGNED_FILE_FOUND)
            list(APPEND BAD_FILES ${FILES_MAP_${NAME_WE}})
            list(GET FILES_MAP_${NAME_WE} 0 FIRST_FILE)
            message("* ${FIRST_FILE}")
        endif ()
    endforeach ()

    if (BAD_FILES)
        message(WARNING "*** Not all files contain \"\\file \\brief\" description. Bad files list see above ***")
    endif ()
endfunction()
