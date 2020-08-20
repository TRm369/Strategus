#pragma once

//Uint types
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

/// Type for standard length IDs.
typedef uint32 ID_t;
/// Type for long IDs (combinations of two IDs).
typedef uint64 doubleID_t;
/// Returns the first ID from double
#define FIRST_ID(DBL_ID) ((uint32)DBL_ID)
/// Returns the first ID from double
#define SECOND_ID(DBL_ID) ((uint32)(DBL_ID >> 32))