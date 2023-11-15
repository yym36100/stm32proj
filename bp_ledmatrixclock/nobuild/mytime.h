#pragma once

#include <time.h>

struct tm *mygmtime (const uint32_t *__restrict tim_p); //676byte local 64
time_t mymktime (struct tm *tim_p); //804byte local 104 (depth 2 total 144)
//static void myvalidate_structure (struct tm *tim_p) //812byte local 40
