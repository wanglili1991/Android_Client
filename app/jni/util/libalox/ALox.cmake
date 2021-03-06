# #################################################################################################
#  ALox.cmake - CMake file for ALox.
#
#  Copyright 2017 A-Worx GmbH, Germany
#  Published under 'Boost Software License' (a free software license, see LICENSE.txt)
#
#  Note: This cmake file includes ALib.cmake and expects this script in the very same folder.
# #################################################################################################

# -------------------------------------------------------------------------------------------------
# include ALib
# -------------------------------------------------------------------------------------------------

    include( "${CMAKE_CURRENT_LIST_DIR}/ALib.cmake" )


# -------------------------------------------------------------------------------------------------
# set cache variables
# -------------------------------------------------------------------------------------------------
    set( ALOX_VERSION                   "${ALIB_VERSION}"                               CACHE STRING
         "The ALox version. Not modifiable (will be overwritten on generation!)"        FORCE )

    set( ALOX_BASE_DIR                  ${ALIB_BASE_DIR}                                CACHE   PATH
         "The base path to ALox containing source code, project files, tools, docs, etc.")


    if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        set( tmp_default_val   "On" )
    else()
        set( tmp_default_val   "Off" )
    endif()
    set( ALOX_DBG_LOG                   ${tmp_default_val}                              CACHE   BOOL
         "Enable/disable debug log statements. Defaults to true in debug compilations, otherwise to false." )

    set( ALOX_DBG_LOG_CI                "On"                                            CACHE   BOOL
         "Defaults to true. If set, caller information is used with debug log statements." )

    set( ALOX_REL_LOG                   "On"                                            CACHE   BOOL
         "Enable/disable release log statements. Defaults to true." )

    set( ALOX_REL_LOG_CI                "Off"                                           CACHE   BOOL
         "Defaults to false. If set, caller information is used even with release log statements (and even in release builds!)" )

# -------------------------------------------------------------------------------------------------
# checks
# -------------------------------------------------------------------------------------------------
    if (tmp_alox_included_marker)
        message( "ALox.cmake: Already included (included twice!)" )
        return()
    endif()
    set(tmp_alox_included_marker "1")


# -------------------------------------------------------------------------------------------------
# source code definitions
# -------------------------------------------------------------------------------------------------

    set (              ALOX_SRC_DIR      ${ALOX_BASE_DIR}/alox/             )
    file( GLOB_RECURSE ALOX_INCS         ${ALOX_BASE_DIR}/alox/alox/*.h*    )
    file( GLOB_RECURSE ALOX_SRCS         ${ALOX_BASE_DIR}/alox/alox/*.cpp   )


# -------------------------------------------------------------------------------------------------
# Set ALox conditional compilation symbols
# -------------------------------------------------------------------------------------------------

    set( ALOX_COMPILER_SYMBOLS   "" )


    # write compilations symbols into ${ALOX_COMPILER_SYMBOLS}
    if ( ${ALOX_DBG_LOG} )
        set( ALOX_COMPILER_SYMBOLS  ${ALOX_COMPILER_SYMBOLS}    "ALOX_DBG_LOG_ON"          )
        if ( ${ALOX_DBG_LOG_CI} )
            set( ALOX_COMPILER_SYMBOLS  ${ALOX_COMPILER_SYMBOLS}    "ALOX_DBG_LOG_CI_ON"   )
        else()
            set( ALOX_COMPILER_SYMBOLS  ${ALOX_COMPILER_SYMBOLS}    "ALOX_DBG_LOG_CI_OFF"  )
        endif()
    else()
        set( ALOX_COMPILER_SYMBOLS  ${ALOX_COMPILER_SYMBOLS}    "ALOX_DBG_LOG_OFF"         )
    endif()

    if ( ${ALOX_REL_LOG} )
        set( ALOX_COMPILER_SYMBOLS  ${ALOX_COMPILER_SYMBOLS}    "ALOX_REL_LOG_ON"          )
        if ( ${ALOX_REL_LOG_CI} )
            set( ALOX_COMPILER_SYMBOLS  ${ALOX_COMPILER_SYMBOLS}    "ALOX_REL_LOG_CI_ON"   )
        else()
            set( ALOX_COMPILER_SYMBOLS  ${ALOX_COMPILER_SYMBOLS}    "ALOX_REL_LOG_CI_OFF"  )
        endif()
    else()
        set( ALOX_COMPILER_SYMBOLS  ${ALOX_COMPILER_SYMBOLS}    "ALOX_REL_LOG_OFF"         )
    endif()


# -------------------------------------------------------------------------------------------------
# Set filename of ALox library
# -------------------------------------------------------------------------------------------------

    # set name of ALox library
    set( ALOX_LIBRARY_FILENAME  "alox_${ALOX_VERSION}" )

    if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        set ( ALOX_LIBRARY_FILENAME         ${ALOX_LIBRARY_FILENAME}_DBG        )
        if ( NOT ${ALIB_DEBUG} )
            set ( ALOX_LIBRARY_FILENAME     ${ALOX_LIBRARY_FILENAME}_NOALIBDBG  )
        endif()
    else()
        if ( ${ALIB_DEBUG} )
            set ( ALOX_LIBRARY_FILENAME     ${ALOX_LIBRARY_FILENAME}_RELWITHALIBDBG   )
        endif()
    endif()

    if ( ${ALIB_DEBUG_STRINGS} )
        set ( ALOX_LIBRARY_FILENAME         ${ALOX_LIBRARY_FILENAME}_STRDBG     )
    endif()

    if ( NOT ${ALIB_FEAT_THREADS} )
        set ( ALOX_LIBRARY_FILENAME         ${ALOX_LIBRARY_FILENAME}_NOTHRD     )
    endif()

    if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        if ( NOT ${ALOX_DBG_LOG} )
            set ( ALOX_LIBRARY_FILENAME     ${ALOX_LIBRARY_FILENAME}_NODBGLOG   )
        endif()
    endif()

    if ( NOT ${ALOX_REL_LOG} )
        set ( ALOX_LIBRARY_FILENAME         ${ALOX_LIBRARY_FILENAME}_NORELLOG   )
    endif()


    #message("ALox library file name: ${ALOX_LIBRARY_FILENAME}")
    #ALibCMake_DumpStatus()

# -------------------------------------------------------------------------------------------------
# Targets
# -------------------------------------------------------------------------------------------------

# Static library
    # sources
    #add_library               ( ALox_StaticLib                               ${ALIB_INCS}    ${ALOX_INCS}
    #                                                                         ${ALIB_SRCS}    ${ALOX_SRCS}    )
    #target_include_directories( ALox_StaticLib    PUBLIC                     ${ALIB_SRC_DIR} ${ALOX_SRC_DIR} )

    # compiler/linker flags
    #set_target_properties     ( ALox_StaticLib    PROPERTIES  COMPILE_FLAGS  ${AWORX_COMPILE_FLAGS}          )

    #target_compile_definitions( ALox_StaticLib    PUBLIC                     ${ALIB_COMPILER_SYMBOLS}
    #                                                                         ${ALOX_COMPILER_SYMBOLS}        )
    #set_target_properties     ( ALox_StaticLib    PROPERTIES  LINK_FLAGS     ${AWORX_LINK_FLAGS}             )

    # library output path/name (for static libs this is 'ARCHIVE_...', for dynamic libs 'LIBRARY_...'
    #set_target_properties     ( ALox_StaticLib    PROPERTIES  ARCHIVE_OUTPUT_NAME  ${ALOX_LIBRARY_FILENAME}  )

# Shared library
    # sources
    add_library               ( alox    SHARED                     ${ALIB_INCS}    ${ALOX_INCS}
                                                                             ${ALIB_SRCS}    ${ALOX_SRCS}    )
    target_include_directories( alox    PUBLIC                     ${ALIB_SRC_DIR} ${ALOX_SRC_DIR} )

    # compiler/linker flags
    set_target_properties     ( alox    PROPERTIES  COMPILE_FLAGS  ${AWORX_COMPILE_FLAGS}          )

    target_compile_definitions( alox    PUBLIC                     ${ALIB_COMPILER_SYMBOLS}
                                                                             ${ALOX_COMPILER_SYMBOLS}        )
    set_target_properties     ( alox    PROPERTIES  LINK_FLAGS     ${AWORX_LINK_FLAGS}             )

    # library output path/name (for static libs this is 'ARCHIVE_...', for dynamic libs 'LIBRARY_...'
    set_target_properties     ( alox    PROPERTIES  LIBRARY_OUTPUT_NAME  ${ALOX_LIBRARY_FILENAME}  )

# -------------------------------------------------------------------------------------------------
# Precompiled headers with 'cotire' CMake script.
# For more Info see 'ALib.cmake' which downloads the script, the C++ setup instructions in the
# ALib/ALox documentation and the [cotire website](https://github.com/sakra/cotire).
# -------------------------------------------------------------------------------------------------

# set cotire (if download failed, this variable was set false)
if ( ${ALIB_CMAKE_COTIRE} )

    # use multiple processor cores
    #set_target_properties( ALox_StaticLib  PROPERTIES COTIRE_UNITY_SOURCE_MAXIMUM_NUMBER_OF_INCLUDES "-j" )
    set_target_properties( alox  PROPERTIES COTIRE_UNITY_SOURCE_MAXIMUM_NUMBER_OF_INCLUDES "-j" )

    # header for precomp is "alib/alib.hpp"
    #set_target_properties( ALox_StaticLib  PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT  "${ALIB_SRC_DIR}/alib/alib.hpp" )
    set_target_properties( alox  PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT  "${ALIB_SRC_DIR}/alib/alib.hpp" )

    # add cotire to projects
    #cotire               ( ALox_StaticLib ALox_SharedLib )
    cotire               ( alox )
	
    #--- dump cotire properties ---
    # set ( _dumppname "ALox_StaticLib" )
    # get_target_property( _unitySource        ${_dumppname}    COTIRE_CXX_UNITY_SOURCE      )
    # get_target_property( _prefixHeader       ${_dumppname}    COTIRE_CXX_PREFIX_HEADER     )
    # get_target_property( _precompiledHeader  ${_dumppname}    COTIRE_CXX_PRECOMPILED_HEADER)
    # get_target_property( _unityTargetName    ${_dumppname}    COTIRE_UNITY_TARGET_NAME     )
    #
    # message(STATUS "${_dumppname} unity source:       ${_unitySource}")
    # message(STATUS "${_dumppname} prefix header:      ${_prefixHeader}")
    # message(STATUS "${_dumppname} precompiled header: ${_precompiledHeader}")
    # message(STATUS "${_dumppname} unity target:       ${_unityTargetName}")

endif()
