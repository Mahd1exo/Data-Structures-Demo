#ifndef DS_JSON_UTILS_H
#define DS_JSON_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

	/* naive JSON parse for "value": "SOMESTRING" */
	int extract_value_from_body(const char* body, char* outValue, int maxLen);

	/* build JSON like { "list": ["A","B","C"] } */
	char* build_json_array_response(const char* wrapperKey, const char** items, int count);

#ifdef __cplusplus
}
#endif

#endif /* DS_JSON_UTILS_H */
