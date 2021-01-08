#!/usr/bin/env bash

# GLOBAL

# COMPILING RELATED
CC=gcc
CCFLAGS="-Wall -c"
AR=ar
ARFLAGS="crs"
SOURCE_FILES=(nml.c nml_util.c)
OBJECT_FILES=(nml.o nml_util.o)
HEADER_FILES=(nml.h nml_util.h)
DIST_DIR="dist"
EXAMPLES="examples"
EXAMPLES_LIB="examples/lib"
LIB_NAME="libnml.a"
CMDS=($CC ar nm)

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

function sanity_checks {
  echo -e "${YELLOW}Sanity Checks:${NC}"
  for cmd in ${CMDS[@]};
  do
    echo -ne "\tChecking if $cmd is in PATH:"
    if ! command -v ${cmd} &> /dev/null
    then
      echo -e " ${RED}NOK${NC}"
      echo -e " ${RED}ABORTING${NC}"
      exit
    fi
    echo -e " ${GREEN}OK${NC}"
  done
}

function compile_objects {
  echo -e "${YELLOW}Compiling Objects:${NC}"
  for src_file in ${SOURCE_FILES[@]};
  do
    echo -e "\tCompiling ${src_file}"
    $CC ${CCFLAGS} ${src_file}
  done
}

function archive_lib {
  echo -e "${YELLOW}Building Library${NC} -> ${LIB_NAME} -> from:"
  for object in ${OBJECT_FILES[@]};
  do
    echo -e "\t${object}"
  done
  $AR ${ARFLAGS} $LIB_NAME ${OBJECT_FILES[@]}
}

function dist_lib {
  mkdir -p $DIST_DIR
  mv ${LIB_NAME} ${DIST_DIR}/
  for header in ${HEADER_FILES[@]};
  do
      cp $header ${DIST_DIR}
  done
}

function clean {
  echo -e "${YELLOW}Cleaning `pwd` for '*.o' files.${NC}"
  rm -f *.o
  echo -e "${YELLOW}Cleaning distribution folder `pwd`/${DIST_DIR}${NC}"
  rm -rf ${DIST_DIR}
  echo -e "${YELLOW}Cleaning the folder `pwd`/${EXAMPLES_LIB}${NC}"
  rm -rf ${EXAMPLES_LIB}
}

function examples {
  echo -e "${YELLOW}Preparing examples/ folder with the latest version:${NC}"
  echo -e "\tMoving ${DIST_DIR}/* to ${EXAMPLES_LIB}/"
  cp -r ${DIST_DIR} ${EXAMPLES_LIB}
}

function build_opt {
  sanity_checks
  compile_objects
  archive_lib
  dist_lib
  examples
}

### MAIN ###

case "$1" in
"build")
  sanity_checks
  compile_objects
  archive_lib
  dist_lib
  ;;
"examples")
  sanity_checks
  compile_objects
  archive_lib
  dist_lib
  examples
  ;;
"clean")
  clean
  ;;
*)
  echo -e "${RED}Unknown Option: '${1}'.${NC}"
  echo -e "Usage:"
  echo -e "\t ${YELLOW}./nml.sh build${NC}"
  echo -e "\t\t Builds the lib in: ${YELLOW}${DIST_DIR}/${NC}."
  echo -e "\t ${YELLOW}./nml.sh examples${NC}"
  echo -e "\t\t Builds ${YELLOW}${EXAMPLES}${NC}/ folder with the latest build."
  echo -e "\t ${YELLOW}./nml.sh clean${NC}"
  echo -e "\t\t Cleans the folder for *.o and *.a files. Deletes the ${YELLOW}${DIST_DIR}/${NC} folder."
  ;;
esac
