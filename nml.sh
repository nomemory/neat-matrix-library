#!/usr/bin/env bash

# GLOBAL

# COMPILING RELATED
CC=gcc
CCFLAGS="-Wall -c"
CCFLAGS_EXAMPLES="-Wall"
AR=ar
ARFLAGS="crs"
SOURCE_FILES=(nml.c nml_util.c)
OBJECT_FILES=(nml.o nml_util.o)
HEADER_FILES=(nml.h nml_util.h)
LIB_NAME="libnml.a"
LIB_NAME_SIMPLE="nml"
DIST_DIR="dist"
EXAMPLES="examples"
EXAMPLES_LIB="examples/lib"
TESTS="tests"
TESTS_LIB="tests/lib"

CMDS=($CC ar nm)

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# exit when any command fails
set -e

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

  for src in ${SOURCE_FILES[@]};
  do
    echo -ne "\tChecking if ${src} exists in folder:"
    if [ ! -f "${src}" ]; then
      echo -e " ${RED}NOK${NC}"
      echo -e " ${RED}ABORTING${NC}"
      exit
    fi
    echo -e " ${GREEN}OK${NC}"
  done

  for hfile in ${HEADER_FILES[@]};
  do
    echo -ne "\tChecking if ${hfile} exists in folder:"
    if [ ! -f "${hfile}" ]; then
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
  echo -e "${YELLOW}Deleting:${NC}"
  echo -e "\tAll files (*.o) in `pwd`files."
  rm -f *.o
  echo -e "\tAll files (*.ex) in `pwd`/${EXAMPLES}/*.ex"
  rm -rf ${EXAMPLES}/*.ex
  echo -e "\tAll files (*.ex) in `pwd`/${TESTS}/*.ex"
  rm -rf ${TESTS}/*.ex
  echo -e "\tFolder `pwd`/${DIST_DIR}${NC}"
  rm -rf ${DIST_DIR}
  echo -e "\tFolder `pwd`/${EXAMPLES_LIB}${NC}"
  rm -rf ${EXAMPLES_LIB}
  echo -e "\tFolder `pwd`/${TESTS_LIB}${NC}"
  rm -rf ${TESTS_LIB}
}

function examples {
  echo -e "${YELLOW}Preparing examples/ folder with the latest version:${NC}"
  echo -e "\tMoving ${DIST_DIR}/* to ${EXAMPLES_LIB}/*"
  cp -r ${DIST_DIR} ${EXAMPLES_LIB}
  echo -e "${YELLOW}Compiling Examples:${NC}"
  # ${CC} ${CCFLAGS_EXAMPLES} ./examples/playground.c -L ./${EXAMPLES}/lib -l${LIB_NAME_SIMPLE} -o playground.ex
  ls ${EXAMPLES}/*.c | while read file ;
    do
      echo -e "\t $file -> ${GREEN}${file%%.*}.ex${NC}"
      ${CC} ${CCFLAGS_EXAMPLES} ${file} -L ./${EXAMPLES}/lib -l${LIB_NAME_SIMPLE} -o ${file%%.*}.ex
    done
}

function tests {
  echo -e "${YELLOW}Preparing tests/ folder with the latest versions: ${NC}"
  echo -e "\tMoving ${DIST_DIR}/* to ${TESTS_LIB}/*"
  cp -r ${DIST_DIR} ${TESTS_LIB}
  echo -e "${YELLOW}Compiling Tests:${NC}"
  ls ${TESTS}/*.c | while read file ;
    do 
      echo -e "\t $file -> ${file%%.*}.ex${NC}"
      ${CC} ${CCFLAGS_EXAMPLES} ${file} -L ./${TESTS}/lib -l${LIB_NAME_SIMPLE} -o ${file%%.*}.ex
    done
  echo -e "${YELLOW}Running Tests:${NC}"
  ls ${TESTS}/*ex | while read file ;
    do
      ./${file} `pwd`/${file%%.*}.data
    done   
} 

### MAIN ###

echo -e " "
echo -e "${GREEN}N${NC}eat ${GREEN}M${NC}atrix ${GREEN}L${NC}ibrary (libnml)"
echo -e " "

for token in "${@}" ;
do
  case $token in
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
  "test")
    sanity_checks
    compile_objects
    archive_lib
    dist_lib
    tests
    ;;
  "clean")
    clean
    ;;
  *)
    echo -e "${RED}Unknown Option: '${1}'.${NC}"
    echo -e "Usage:"
    echo -e "\t ${YELLOW}./nml.sh build${NC}"
    echo -e "\t\t Builds the lib in: ${YELLOW}${DIST_DIR}/${NC}."
    echo -e "\t ${YELLOW}./nml.sh tests${NC}"
    echo -e "\t\t Runs lib tests."
    echo -e "\t ${YELLOW}./nml.sh examples${NC}"
    echo -e "\t\t Builds ${YELLOW}${EXAMPLES}${NC}/ folder with the latest build."
    echo -e "\t ${YELLOW}./nml.sh clean${NC}"
    echo -e "\t\t Cleans the folder for *.o and *.a files. Deletes the ${YELLOW}${DIST_DIR}/${NC} folder."
    ;;
  esac
done
