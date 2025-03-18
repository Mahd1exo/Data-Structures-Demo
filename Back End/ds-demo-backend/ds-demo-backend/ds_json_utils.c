#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_json_utils.h"

int extract_value_from_body(const char* body, char* outValue, int maxLen) {
    if (!body || !outValue || maxLen <= 0) return 0;
    const char* p = strstr(body, "\"value\"");
    if (!p) {
        return 0; /* not found */
    }
    p = strchr(p, ':');
    if (!p) return 0;
    p++; // skip colon

    // skip spaces or quotes
    while (*p && (*p == ' ' || *p == '\"' || *p == '\t' || *p == ':')) {
        p++;
    }
    if (!*p) return 0;

    int i = 0;
    while (*p && *p != '\"' && *p != '}' && *p != '\n' && i < (maxLen - 1)) {
        outValue[i++] = *p++;
    }
    outValue[i] = '\0';
    return 1;
}

char* build_json_array_response(const char* wrapperKey, const char** items, int count) {
    // Example: { "stack": ["top","next"] }
    if (!wrapperKey) wrapperKey = "data";
    // allocate enough space
    int capacity = 1024 + (count * 64);
    char* json = (char*)malloc(capacity);
    if (!json) return NULL;

    snprintf(json, capacity, "{ \"%s\": [", wrapperKey);

    for (int i = 0; i < count; i++) {
        char temp[256];
        snprintf(temp, sizeof(temp), "\"%s\"", items[i]);
        strncat(json, temp, capacity - strlen(json) - 1);

        if (i < count - 1) {
            strncat(json, ",", capacity - strlen(json) - 1);
        }
    }

    strncat(json, "]}", capacity - strlen(json) - 1);
    return json;
}
