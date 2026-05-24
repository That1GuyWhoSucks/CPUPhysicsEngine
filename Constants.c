#include <windows.h>

#include "Constants.h"

u_int RNG_STATE;
HANDLE fptr;

void Log(const char *msg, const char size) {
#if DO_LOG == 1
    WriteFile(fptr, msg, size, NULL, NULL);
#endif
}

void LogInit() {
    SUICIDE_FLAG = 0;
#if DO_LOG == 1
    fptr = CreateFile(".\\output.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#endif
    LARGE_INTEGER li; // get semi random value to seed LFSR
    QueryPerformanceCounter(&li);
    RNG_STATE = li.QuadPart;
    // uninitialized memory is 0 which doesn't work with a LFSR :(
}

void LogDestroy() {
#if DO_LOG == 1
    CloseHandle(fptr);
#endif
}

//https://www.ijera.com/papers/Vol4_issue6/Version%206/P0460699102.pdf (use taps 1,5,6,31 [0 indexed])
u_int Random() {
    const u_char bit = ((RNG_STATE >> 0) ^ (RNG_STATE >> 5) ^ (RNG_STATE >> 6) ^ (RNG_STATE >> 30)) & 0b1;
    RNG_STATE = RNG_STATE >> 1 | bit << 31;
    return RNG_STATE;
}

void MemCpy(void *dest, const void *src, const size_a bytes) {
    char *d = dest;
    const char *s = src;

    if (d == s || bytes == 0) return;

    if (d < s) {
        for (size_a i=0; i<bytes; i++) {
            d[i] = s[i];
        }
    } else {
        for (size_a i=bytes; i>0; i--) {
            d[i-1] = s[i-1];
        }
    }
}

int MaxInt(const int a, const int b) {
    return a > b ? a : b;
}

int MinInt(const int a, const int b) {
    return a < b ? a : b;
}

double MaxDouble(const double a, const double b) {
    return a > b ? a : b;
}

double MinDouble(const double a, const double b) {
    return a < b ? a : b;
}

u_int MaxUInt(const u_int a, const u_int b) {
    return a > b ? a : b;
}

u_int MinUInt(const u_int a, const u_int b) {
    return a < b ? a : b;
}

int Ceil(const double a) {
    const long long a_copy = (long long) a; // truncate double to non-floating point
    if ((double) a_copy == a) { // if same after truncation then no decimal component
        return (int) a_copy; // return lower half
    }
    if (a < 0) { // add 1 away from 0
        return (int) (a_copy - 1);
    }
    return (int) (a_copy + 1);
}

int Floor(const double a) {
    const long long a_copy = (long long) a; // truncate double to non-floating point
    if ((double) a_copy == a) { // if same after truncation then no decimal component
        return (int) a_copy; // return lower half
    }
    return (int) a_copy;
}

void MemSet(void *dest, const unsigned char val, const size_a bytes) {
    unsigned char *d = dest;
    for (size_a i=0; i<bytes; i++) {
        d[i] = val;
    }
}

double Abs(const double a) {
    if (a < 0.0) return -a;
    return a;
}

u_int Pow(const u_int a, const u_int b) {
    u_int result = 1;
    for (u_int i=0; i<b; i++) {
        result *= a;
    }
    return result;
}