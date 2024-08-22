﻿# cmake -P innosetup.cmake
if(DEFINED ENV{REMOVE_TMP})
  set(REMOVE_TMP ON)
else()
  option(REMOVE_TMP "" OFF)
endif()

# innosetup 6.3.3
set(INNOSETUP_EXE "innosetup-6.3.3.exe")
set(INNOSETUP_URL "https://files.jrsoftware.org/is/6/${INNOSETUP_EXE}")
set(INNOSETUP_HASH "0bcb2a409dea17e305a27a6b09555cabe600e984f88570ab72575cd7e93c95e6")
set(CHECK_FILE innosetup6/ISCC.exe)
set(CHECK_HASH "bf65156e415096b4b524ea7a8646d1e5b4ff7817fe8ba5dfc142a637640ae7d3")

# check innosetup
if(EXISTS ${CHECK_FILE})
  file(SHA256 ${CHECK_FILE} HASH)
  if(${HASH} STREQUAL ${CHECK_HASH})
    return()
  endif()
  message("file ${CHECK_FILE}")
  message("actual HASH=${HASH}")
  message("expect HASH=${CHECK_HASH}")
endif()

# download
message("download ${INNOSETUP_EXE} from ${INNOSETUP_URL}")
file(MAKE_DIRECTORY "download/innosetup6")
file(DOWNLOAD
  ${INNOSETUP_URL}
  download/innosetup6/${INNOSETUP_EXE}
  EXPECTED_HASH SHA256=${INNOSETUP_HASH}
  SHOW_PROGRESS
  )

# setup
if(EXISTS "innosetup6")
  file(REMOVE_RECURSE "innosetup6")
endif()
file(MAKE_DIRECTORY "innosetup6")
execute_process(
  COMMAND ../download/innosetup6/${INNOSETUP_EXE} /dir=. /CURRENTUSER /PORTABLE=1 /VERYSILENT
  WORKING_DIRECTORY "innosetup6"
  )
if(REMOVE_TMP)
  file(REMOVE_RECURSE "download/innosetup6")
endif()
