#! /bin/sh

cat <<!
/* File created from $1 via mkskel.sh */

#include "flexdef.h"

const char *skel[] = {
!

sed 's/\\/&&/g' $* | sed 's/"/\\"/g' | sed 's/.*/  "&",/'

cat <<!
  0
};
!
