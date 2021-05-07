#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef bool (*BaseCompareFunc)(void const* left_key, void const* right_key);
typedef int (*BaseOrderFunc)(void const* left_key, void const* right_key);
typedef uint64_t (*BaseHashFunc)(void const* key);
