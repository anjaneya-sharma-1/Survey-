#include "ui.h"
#include <iostream>
#include <iomanip>
#include <cstring>

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

// Copy all UI-related function implementations from the original file
// Including: title(), drawHistogram(), displayRatingVisual()
// Remove the using namespace std; and use std:: prefix
