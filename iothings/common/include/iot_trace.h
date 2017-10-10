#ifndef IOT_TRACE_H__
#define IOT_TRACE_H__

#include "nrf_log.h"

#if defined(IOT_APP)
#include "app_scheduler.h"
#include "clock.h"
#include "dstore.h"
#endif

#include <inttypes.h>
#include "nrf_delay.h"


/* clang-format off */

#define L_VERBOSE   "0"
#define L_INFO      "1"
#define L_WARNING   "2"
#define L_ERROR     "3"
#define L_FATAL     "4"

/* clang-format on */

/*
#ifndef ENABLE_DEBUG_LOG_SUPPORT

#define IOT_VERBOSE(...)                                    \
    do                                                      \
    {                                                       \
		dstore_save(time_get_time(), app_sched_depth_get(), L_VERBOSE, __FILE__, __LINE__, __VA_ARGS__); \
	} while (0)

#define IOT_INFO(...)                                       \
    do                                                      \
    {                                                       \
        dstore_save(time_get_time(), app_sched_depth_get(), L_INFO, __FILE__, __LINE__, __VA_ARGS__); \
    } while (0)

#define IOT_WARN(...)                                       \
    do                                                      \
    {                                                       \
        dstore_save(time_get_time(), app_sched_depth_get(), L_WARNING, __FILE__, __LINE__, __VA_ARGS__); \
    } while (0)

#define IOT_ERROR(...)                                      \
    do                                                      \
    {                                                       \
        dstore_save(time_get_time(), app_sched_depth_get(), L_ERROR, __FILE__, __LINE__, __VA_ARGS__); \
    } while (0)

#define IOT_FATAL(...)                                      \
    do                                                      \
    {                                                       \
        dstore_save(time_get_time(), app_sched_depth_get(), L_FATAL, __FILE__, __LINE__, __VA_ARGS__); \
    } while (0)

#define IOT_LOG(...)                \
    do                              \
    {                               \
        app_trace_log(__VA_ARGS__); \
        app_trace_log("\r\n");      \
    } while (0)

#else
*/
#define IOT_VERBOSE(...)                                    \
    do                                                      \
    {                                                       \
        app_trace_log("[%010" PRIu32 ":", time_get_time()); \
        app_trace_log("%02u:", app_sched_depth_get());      \
        app_trace_log(L_VERBOSE);                           \
        app_trace_log(":%s:%d] ", __FILE__, __LINE__);      \
        app_trace_log(__VA_ARGS__);                         \
        app_trace_log("\r\n");                              \
	} while (0)

#define IOT_INFO(...)                                       \
    do                                                      \
    {                                                       \
        app_trace_log("[%010" PRIu32 ":", time_get_time()); \
        app_trace_log("%02u:", app_sched_depth_get());      \
        app_trace_log(L_INFO);                              \
        app_trace_log(":%s:%d] ", __FILE__, __LINE__);      \
        app_trace_log(__VA_ARGS__);                         \
        app_trace_log("\r\n");                              \
    } while (0)

#define IOT_WARN(...)                                       \
    do                                                      \
    {                                                       \
        app_trace_log("[%010" PRIu32 ":", time_get_time()); \
        app_trace_log("%02u:", app_sched_depth_get());      \
        app_trace_log(L_WARNING);                           \
        app_trace_log(":%s:%d] ", __FILE__, __LINE__);      \
        app_trace_log(__VA_ARGS__);                         \
        app_trace_log("\r\n");                              \
    } while (0)

#define IOT_ERROR(...)                                      \
    do                                                      \
    {                                                       \
        app_trace_log("[%010" PRIu32 ":", time_get_time()); \
        app_trace_log("%02u:", app_sched_depth_get());      \
        app_trace_log(L_ERROR);                             \
        app_trace_log(":%s:%d] ", __FILE__, __LINE__);      \
        app_trace_log(__VA_ARGS__);                         \
        app_trace_log("\r\n");                              \
    } while (0)

#define IOT_FATAL(...)                                      \
    do                                                      \
    {                                                       \
        app_trace_log("[%010" PRIu32 ":", time_get_time()); \
        app_trace_log("%02u:", app_sched_depth_get());      \
        app_trace_log(L_FATAL);                             \
        app_trace_log(":%s:%d] ", __FILE__, __LINE__);      \
        app_trace_log(__VA_ARGS__);                         \
        app_trace_log("\r\n");                              \
    } while (0)
        //dstore_save(time_get_time(), app_sched_depth_get(), L_FATAL, __FILE__, __LINE__, __VA_ARGS__); 

#define IOT_LOG(...)                \
    do                              \
    {                               \
        app_trace_log(__VA_ARGS__); \
        app_trace_log("\r\n");      \
    } while (0)


//#endif

#endif //IOT_TRACE_H__
