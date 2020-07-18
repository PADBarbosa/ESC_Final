/* Generated by the Systemtap dtrace wrapper */


#define _SDT_HAS_SEMAPHORES 1


#define STAP_HAS_SEMAPHORES 1 /* deprecated */


#include <sys/sdt.h>

/* HISTOGRAM_EQUALIZATION_EQUALIZATION_START ( int size ) */
#if defined STAP_SDT_V1
#define HISTOGRAM_EQUALIZATION_EQUALIZATION_START_ENABLED() __builtin_expect (equalization__start_semaphore, 0)
#define histogram_equalization_equalization__start_semaphore equalization__start_semaphore
#else
#define HISTOGRAM_EQUALIZATION_EQUALIZATION_START_ENABLED() __builtin_expect (histogram_equalization_equalization__start_semaphore, 0)
#endif
__extension__ extern unsigned short histogram_equalization_equalization__start_semaphore __attribute__ ((unused)) __attribute__ ((section (".probes")));
#define HISTOGRAM_EQUALIZATION_EQUALIZATION_START(arg1) \
DTRACE_PROBE1 (histogram_equalization, equalization__start, arg1)

/* HISTOGRAM_EQUALIZATION_COMPUTING_DONE ( int size ) */
#if defined STAP_SDT_V1
#define HISTOGRAM_EQUALIZATION_COMPUTING_DONE_ENABLED() __builtin_expect (computing__done_semaphore, 0)
#define histogram_equalization_computing__done_semaphore computing__done_semaphore
#else
#define HISTOGRAM_EQUALIZATION_COMPUTING_DONE_ENABLED() __builtin_expect (histogram_equalization_computing__done_semaphore, 0)
#endif
__extension__ extern unsigned short histogram_equalization_computing__done_semaphore __attribute__ ((unused)) __attribute__ ((section (".probes")));
#define HISTOGRAM_EQUALIZATION_COMPUTING_DONE(arg1) \
DTRACE_PROBE1 (histogram_equalization, computing__done, arg1)

/* HISTOGRAM_EQUALIZATION_NORMALIZING_DONE ( int size ) */
#if defined STAP_SDT_V1
#define HISTOGRAM_EQUALIZATION_NORMALIZING_DONE_ENABLED() __builtin_expect (normalizing__done_semaphore, 0)
#define histogram_equalization_normalizing__done_semaphore normalizing__done_semaphore
#else
#define HISTOGRAM_EQUALIZATION_NORMALIZING_DONE_ENABLED() __builtin_expect (histogram_equalization_normalizing__done_semaphore, 0)
#endif
__extension__ extern unsigned short histogram_equalization_normalizing__done_semaphore __attribute__ ((unused)) __attribute__ ((section (".probes")));
#define HISTOGRAM_EQUALIZATION_NORMALIZING_DONE(arg1) \
DTRACE_PROBE1 (histogram_equalization, normalizing__done, arg1)

/* HISTOGRAM_EQUALIZATION_MAPPING_DONE ( int size ) */
#if defined STAP_SDT_V1
#define HISTOGRAM_EQUALIZATION_MAPPING_DONE_ENABLED() __builtin_expect (mapping__done_semaphore, 0)
#define histogram_equalization_mapping__done_semaphore mapping__done_semaphore
#else
#define HISTOGRAM_EQUALIZATION_MAPPING_DONE_ENABLED() __builtin_expect (histogram_equalization_mapping__done_semaphore, 0)
#endif
__extension__ extern unsigned short histogram_equalization_mapping__done_semaphore __attribute__ ((unused)) __attribute__ ((section (".probes")));
#define HISTOGRAM_EQUALIZATION_MAPPING_DONE(arg1) \
DTRACE_PROBE1 (histogram_equalization, mapping__done, arg1)

/* HISTOGRAM_EQUALIZATION_AFTER_READ ( int size ) */
#if defined STAP_SDT_V1
#define HISTOGRAM_EQUALIZATION_AFTER_READ_ENABLED() __builtin_expect (after__read_semaphore, 0)
#define histogram_equalization_after__read_semaphore after__read_semaphore
#else
#define HISTOGRAM_EQUALIZATION_AFTER_READ_ENABLED() __builtin_expect (histogram_equalization_after__read_semaphore, 0)
#endif
__extension__ extern unsigned short histogram_equalization_after__read_semaphore __attribute__ ((unused)) __attribute__ ((section (".probes")));
#define HISTOGRAM_EQUALIZATION_AFTER_READ(arg1) \
DTRACE_PROBE1 (histogram_equalization, after__read, arg1)

/* HISTOGRAM_EQUALIZATION_BEFORE_WRITE ( int size ) */
#if defined STAP_SDT_V1
#define HISTOGRAM_EQUALIZATION_BEFORE_WRITE_ENABLED() __builtin_expect (before__write_semaphore, 0)
#define histogram_equalization_before__write_semaphore before__write_semaphore
#else
#define HISTOGRAM_EQUALIZATION_BEFORE_WRITE_ENABLED() __builtin_expect (histogram_equalization_before__write_semaphore, 0)
#endif
__extension__ extern unsigned short histogram_equalization_before__write_semaphore __attribute__ ((unused)) __attribute__ ((section (".probes")));
#define HISTOGRAM_EQUALIZATION_BEFORE_WRITE(arg1) \
DTRACE_PROBE1 (histogram_equalization, before__write, arg1)
