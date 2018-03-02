#ifndef _sort_h_
#define _sort_h_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "fileIO.h"

extern void mergeSort(B **headRef);
extern B* sortedMerge(B *a, B *b);
extern void split(B *root, B **frontRef, B **backRef);

#endif
