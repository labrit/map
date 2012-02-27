#!/bin/sh

CC=clang
CFLAGS="-Wall -pedantic -O2"

SRC_DIR=./src
BIN_DIR=./bin

LIB_SRC=( $SRC_DIR/map.c )
LIB_OBJ=( $BIN_DIR/map.o )
LIB_BIN=( $BIN_DIR/libmap.a )

SRC=( $SRC_DIR/main.c )
SRC_BIN=( $BIN_DIR/maptest )

if [ -d $BIN_DIR ]; then
  echo "Cleaning up last build..."
  rm -rf $BIN_DIR
fi

mkdir $BIN_DIR

echo "Compiling ${LIB_OBJ}..."
$CC $CFLAGS -o $LIB_OBJ -c $LIB_SRC

echo "Making ${LIB_BIN}..."
ar rcs $LIB_BIN $LIB_OBJ

echo "Compiling ${SRC_BIN}..."
$CC $CFLAGS -o $SRC_BIN $SRC $LIB_BIN
