/******************************************************************************
 * civetweb.h - interface to the civetweb web server
 *
 * Project: https://github.com/civetweb/civetweb
 *
 * This file is based on the original mongoose.h header. Major changes
 * were made to allow usage in C and C++ programs.
 *
 ******************************************************************************/

 /*

	This file is part of the CivetWeb web server/library.
	(https://github.com/civetweb/civetweb)

	This library is free software; you can redistribute it and/or
	modify it under the terms of the MIT license. See the
	LICENSE file included with this distribution.

 */

#ifndef CIVETWEB_HEADER_INCLUDED
#define CIVETWEB_HEADER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(_WIN32) && !defined(__SYMBIAN32__)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS /* Disable deprecation warning in VS2005 */
#endif
#include <windows.h>
#else
#include <pthread.h>
#endif /* _WIN32 */

#include <stddef.h>

	/* Version number components */
#define CIVETWEB_VERSION_MAJOR (1)
#define CIVETWEB_VERSION_MINOR (15)
#define CIVETWEB_VERSION_PATCH (1)

/* ID for CGI environment variable for the server software */
#define CIVETWEB_VERSION_STRING "CivetWeb v1.15"

/* For additional features, see https://github.com/civetweb/civetweb/blob/master/docs/Features.md
 */

#ifdef __GNUC__
#define CIVETWEB_UNUSED __attribute__((unused))
#else
#define CIVETWEB_UNUSED
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201) /* nonstandard extension used : nameless struct/union */
#endif

 /* Manufacturer name, used as HTML server signature */
#ifndef MG_PRODUCT_NAME
#define MG_PRODUCT_NAME "CivetWeb"
#endif

/* If a CivetWeb build with combined support of IPv4 and IPv6 is used, IPv6
 * addresses might internally use "mapped IPv4 addresses". If no IPv6 is used,
 * only IPv4 addresses can occur. If IPv6 is used exclusively, only IPv6 can
 * occur. For details and usage examples, see the API documentation. */

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4820) /* 'bytes' bytes padding added after data member for alignment */
#pragma warning(disable : 4255) /* 'function': no function prototype given: converting '()' to '(void)' */
#endif

 /* Forward references */
	struct mg_connection;
	struct mg_context;
	struct mg_request_info;
	struct mg_header;
	struct mg_error_data;
	struct mg_ssl_if;
	/* ... [SNIP: The file is extremely long. Please expand below to see it in entirety.] */


#ifndef MG_EXPERIMENTAL_INTERFACES
#define MG_EXPERIMENTAL_INTERFACES 0
#endif


/****************************************************************************
 * FEATURE LIST
 ****************************************************************************/

 /* Start of definition: MUST be kept in sync with mg_feature_mask_names in civetweb.c */
#define MG_FEATURES_IPV6 1       /* IPv6 support */
#define MG_FEATURES_WEBSOCKETS 2 /* Websocket support */
#define MG_FEATURES_SSL 4        /* SSL support */
#define MG_FEATURES_ASYNC 8      /* Asynchronous client support */
#define MG_FEATURES_CGI 16       /* CGI support */
#define MG_FEATURES_IPV4 32      /* IPv4 support */
#define MG_FEATURES_HTTP2 64     /* HTTP/2 support */
/* End of definition: MG_FEATURES_ must be kept in sync with mg_feature_mask_names */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CIVETWEB_HEADER_INCLUDED */
