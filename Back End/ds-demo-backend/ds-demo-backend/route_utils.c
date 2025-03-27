#define _CRT_SECURE_NO_WARNINGS
#include "route_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* read_request_body */
int read_request_body(struct mg_connection* conn, char* buf, size_t bufSize)
{
    if (!conn || !buf) return 0;
    int r = mg_read(conn, buf, (int)bufSize - 1);
    if (r < 0) r = 0;
    buf[r] = '\0';
    return r;
}

/* send_json */
void send_json(struct mg_connection* conn, const char* json)
{
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Connection: close\r\n\r\n");
    mg_printf(conn, "%s", json);
}

/* extract_value_from_body: naive parser for {"value":"X"} */
int extract_value_from_body(const char* body, char* outVal, int maxLen)
{
    const char* p = strstr(body, "\"value\"");
    if (!p) return 0;
    p = strchr(p, ':');
    if (!p) return 0;
    p++;
    while (*p && (*p == ' ' || *p == '\"')) p++;
    int i = 0;
    while (*p && *p != '\"' && *p != '}' && i < (maxLen - 1)) {
        outVal[i++] = *p++;
    }
    outVal[i] = '\0';
    return 1;
}

/* extract_key_from_body: naive parser for {"key":"X"} */
int extract_key_from_body(const char* body, char* outKey, int keyMax)
{
    const char* p = strstr(body, "\"key\"");
    if (!p) return 0;
    p = strchr(p, ':');
    if (!p) return 0;
    p++;
    while (*p && (*p == ' ' || *p == '\"')) p++;
    int i = 0;
    while (*p && *p != '\"' && *p != '}' && i < (keyMax - 1)) {
        outKey[i++] = *p++;
    }
    outKey[i] = '\0';
    return 1;
}

/* extract_key_val: naive parser for {"key":"...","val":"..."} */
int extract_key_val(const char* body, char* outKey, int keyMax, char* outVal, int valMax)
{
    const char* kp = strstr(body, "\"key\"");
    const char* vp = strstr(body, "\"val\"");
    if (!kp || !vp) return 0;

    // parse key
    kp = strchr(kp, ':');
    if (!kp) return 0;
    kp++;
    while (*kp && (*kp == ' ' || *kp == '\"')) kp++;
    int i = 0;
    while (*kp && *kp != '\"' && *kp != '}' && i < (keyMax - 1)) {
        outKey[i++] = *kp++;
    }
    outKey[i] = '\0';

    // parse val
    vp = strchr(vp, ':');
    if (!vp) return 0;
    vp++;
    while (*vp && (*vp == ' ' || *vp == '\"')) vp++;
    i = 0;
    while (*vp && *vp != '\"' && *vp != '}' && i < (valMax - 1)) {
        outVal[i++] = *vp++;
    }
    outVal[i] = '\0';
    return 1;
}

/* build_json_array_response: e.g. { "stack": ["A","B"] } */
char* build_json_array_response(const char* wrapper, const char** items, int count)
{
    int capacity = 1024 + 64 * count;
    char* json = (char*)malloc(capacity);
    if (!json) return NULL;
    snprintf(json, capacity, "{ \"%s\": [", wrapper);
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

int extract_index_from_body(const char* body) {
    const char* p = strstr(body, "\"index\"");
    if (!p)
        return -1;
    p = strchr(p, ':');
    if (!p)
        return -1;
    p++; // Skip colon
    while (*p && (*p == ' ' || *p == '\"'))
        p++;
    return atoi(p);
}
