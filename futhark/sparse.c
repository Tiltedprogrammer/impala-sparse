#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wunused-label"
#endif
#ifdef __clang__
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wparentheses"
#pragma clang diagnostic ignored "-Wunused-label"
#endif
// Headers

#define _GNU_SOURCE
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <float.h>


// Initialisation

struct futhark_context_config ;
struct futhark_context_config *futhark_context_config_new(void);
void futhark_context_config_free(struct futhark_context_config *cfg);
void futhark_context_config_set_debugging(struct futhark_context_config *cfg,
                                          int flag);
void futhark_context_config_set_logging(struct futhark_context_config *cfg,
                                        int flag);
struct futhark_context ;
struct futhark_context *futhark_context_new(struct futhark_context_config *cfg);
void futhark_context_free(struct futhark_context *ctx);

// Arrays

struct futhark_f64_1d ;
struct futhark_f64_1d *futhark_new_f64_1d(struct futhark_context *ctx, const
                                          double *data, int64_t dim0);
struct futhark_f64_1d *futhark_new_raw_f64_1d(struct futhark_context *ctx, const
                                              char *data, int offset,
                                              int64_t dim0);
int futhark_free_f64_1d(struct futhark_context *ctx,
                        struct futhark_f64_1d *arr);
int futhark_values_f64_1d(struct futhark_context *ctx,
                          struct futhark_f64_1d *arr, double *data);
char *futhark_values_raw_f64_1d(struct futhark_context *ctx,
                                struct futhark_f64_1d *arr);
const int64_t *futhark_shape_f64_1d(struct futhark_context *ctx,
                                    struct futhark_f64_1d *arr);
struct futhark_i64_1d ;
struct futhark_i64_1d *futhark_new_i64_1d(struct futhark_context *ctx, const
                                          int64_t *data, int64_t dim0);
struct futhark_i64_1d *futhark_new_raw_i64_1d(struct futhark_context *ctx, const
                                              char *data, int offset,
                                              int64_t dim0);
int futhark_free_i64_1d(struct futhark_context *ctx,
                        struct futhark_i64_1d *arr);
int futhark_values_i64_1d(struct futhark_context *ctx,
                          struct futhark_i64_1d *arr, int64_t *data);
char *futhark_values_raw_i64_1d(struct futhark_context *ctx,
                                struct futhark_i64_1d *arr);
const int64_t *futhark_shape_i64_1d(struct futhark_context *ctx,
                                    struct futhark_i64_1d *arr);

// Opaque values


// Entry points

int futhark_entry_eWiseAddEntry(struct futhark_context *ctx,
                                struct futhark_f64_1d **out0,
                                struct futhark_i64_1d **out1,
                                struct futhark_i64_1d **out2, const
                                struct futhark_f64_1d *in0, const
                                struct futhark_i64_1d *in1, const
                                struct futhark_i64_1d *in2, const
                                struct futhark_f64_1d *in3, const
                                struct futhark_i64_1d *in4, const
                                struct futhark_i64_1d *in5);
int futhark_entry_eWiseAddMultEntry(struct futhark_context *ctx,
                                    struct futhark_f64_1d **out0,
                                    struct futhark_i64_1d **out1,
                                    struct futhark_i64_1d **out2, const
                                    struct futhark_f64_1d *in0, const
                                    struct futhark_i64_1d *in1, const
                                    struct futhark_i64_1d *in2, const
                                    struct futhark_f64_1d *in3, const
                                    struct futhark_i64_1d *in4, const
                                    struct futhark_i64_1d *in5);
int futhark_entry_eWiseMultEntry(struct futhark_context *ctx,
                                 struct futhark_f64_1d **out0,
                                 struct futhark_i64_1d **out1,
                                 struct futhark_i64_1d **out2, const
                                 struct futhark_f64_1d *in0, const
                                 struct futhark_i64_1d *in1, const
                                 struct futhark_i64_1d *in2, const
                                 struct futhark_f64_1d *in3, const
                                 struct futhark_i64_1d *in4, const
                                 struct futhark_i64_1d *in5);

// Miscellaneous

int futhark_context_sync(struct futhark_context *ctx);
int futhark_context_clear_caches(struct futhark_context *ctx);
char *futhark_context_report(struct futhark_context *ctx);
char *futhark_context_get_error(struct futhark_context *ctx);
void futhark_context_pause_profiling(struct futhark_context *ctx);
void futhark_context_unpause_profiling(struct futhark_context *ctx);
#define FUTHARK_BACKEND_c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#undef NDEBUG
#include <assert.h>
#include <stdarg.h>
// Start of util.h.
//
// Various helper functions that are useful in all generated C code.

#include <errno.h>
#include <string.h>

static const char *fut_progname = "(embedded Futhark)";

static void futhark_panic(int eval, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  fprintf(stderr, "%s: ", fut_progname);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  exit(eval);
}

// For generating arbitrary-sized error messages.  It is the callers
// responsibility to free the buffer at some point.
static char* msgprintf(const char *s, ...) {
  va_list vl;
  va_start(vl, s);
  size_t needed = 1 + (size_t)vsnprintf(NULL, 0, s, vl);
  char *buffer = (char*) malloc(needed);
  va_start(vl, s); // Must re-init.
  vsnprintf(buffer, needed, s, vl);
  return buffer;
}


static inline void check_err(int errval, int sets_errno, const char *fun, int line,
                            const char *msg, ...) {
  if (errval) {
    char errnum[10];

    va_list vl;
    va_start(vl, msg);

    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, msg, vl);
    fprintf(stderr, " in %s() at line %d with error code %s\n",
            fun, line,
            sets_errno ? strerror(errno) : errnum);
    exit(errval);
  }
}

#define CHECK_ERR(err, msg...) check_err(err, 0, __func__, __LINE__, msg)
#define CHECK_ERRNO(err, msg...) check_err(err, 1, __func__, __LINE__, msg)

// Read a file into a NUL-terminated string; returns NULL on error.
static void* slurp_file(const char *filename, size_t *size) {
  unsigned char *s;
  FILE *f = fopen(filename, "rb"); // To avoid Windows messing with linebreaks.
  if (f == NULL) return NULL;
  fseek(f, 0, SEEK_END);
  size_t src_size = ftell(f);
  fseek(f, 0, SEEK_SET);
  s = (unsigned char*) malloc(src_size + 1);
  if (fread(s, 1, src_size, f) != src_size) {
    free(s);
    s = NULL;
  } else {
    s[src_size] = '\0';
  }
  fclose(f);

  if (size) {
    *size = src_size;
  }

  return s;
}

// Dump 'n' bytes from 'buf' into the file at the designated location.
// Returns 0 on success.
static int dump_file(const char *file, const void *buf, size_t n) {
  FILE *f = fopen(file, "w");

  if (f == NULL) {
    return 1;
  }

  if (fwrite(buf, sizeof(char), n, f) != n) {
    return 1;
  }

  if (fclose(f) != 0) {
    return 1;
  }

  return 0;
}

struct str_builder {
  char *str;
  size_t capacity; // Size of buffer.
  size_t used; // Bytes used, *not* including final zero.
};

static void str_builder_init(struct str_builder *b) {
  b->capacity = 10;
  b->used = 0;
  b->str = malloc(b->capacity);
  b->str[0] = 0;
}

static void str_builder(struct str_builder *b, const char *s, ...) {
  va_list vl;
  va_start(vl, s);
  size_t needed = (size_t)vsnprintf(NULL, 0, s, vl);

  while (b->capacity < b->used + needed + 1) {
    b->capacity *= 2;
    b->str = realloc(b->str, b->capacity);
  }

  va_start(vl, s); // Must re-init.
  vsnprintf(b->str+b->used, b->capacity-b->used, s, vl);
  b->used += needed;
}

// End of util.h.

// Start of timing.h.

// The function get_wall_time() returns the wall time in microseconds
// (with an unspecified offset).

#ifdef _WIN32

#include <windows.h>

static int64_t get_wall_time(void) {
  LARGE_INTEGER time,freq;
  assert(QueryPerformanceFrequency(&freq));
  assert(QueryPerformanceCounter(&time));
  return ((double)time.QuadPart / freq.QuadPart) * 1000000;
}

#else
// Assuming POSIX

#include <time.h>
#include <sys/time.h>

static int64_t get_wall_time(void) {
  struct timeval time;
  assert(gettimeofday(&time,NULL) == 0);
  return time.tv_sec * 1000000 + time.tv_usec;
}

static int64_t get_wall_time_ns(void) {
  struct timespec time;
  assert(clock_gettime(CLOCK_REALTIME, &time) == 0);
  return time.tv_sec * 1000000000 + time.tv_nsec;
}

#endif

// End of timing.h.

#ifdef _MSC_VER
#define inline __inline
#endif
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>

// Start of lock.h.

// A very simple cross-platform implementation of locks.  Uses
// pthreads on Unix and some Windows thing there.  Futhark's
// host-level code is not multithreaded, but user code may be, so we
// need some mechanism for ensuring atomic access to API functions.
// This is that mechanism.  It is not exposed to user code at all, so
// we do not have to worry about name collisions.

#ifdef _WIN32

typedef HANDLE lock_t;

static void create_lock(lock_t *lock) {
  *lock = CreateMutex(NULL,  // Default security attributes.
                      FALSE, // Initially unlocked.
                      NULL); // Unnamed.
}

static void lock_lock(lock_t *lock) {
  assert(WaitForSingleObject(*lock, INFINITE) == WAIT_OBJECT_0);
}

static void lock_unlock(lock_t *lock) {
  assert(ReleaseMutex(*lock));
}

static void free_lock(lock_t *lock) {
  CloseHandle(*lock);
}

#else
// Assuming POSIX

#include <pthread.h>

typedef pthread_mutex_t lock_t;

static void create_lock(lock_t *lock) {
  int r = pthread_mutex_init(lock, NULL);
  assert(r == 0);
}

static void lock_lock(lock_t *lock) {
  int r = pthread_mutex_lock(lock);
  assert(r == 0);
}

static void lock_unlock(lock_t *lock) {
  int r = pthread_mutex_unlock(lock);
  assert(r == 0);
}

static void free_lock(lock_t *lock) {
  // Nothing to do for pthreads.
  (void)lock;
}

#endif

// End of lock.h.

static inline uint8_t add8(uint8_t x, uint8_t y)
{
    return x + y;
}
static inline uint16_t add16(uint16_t x, uint16_t y)
{
    return x + y;
}
static inline uint32_t add32(uint32_t x, uint32_t y)
{
    return x + y;
}
static inline uint64_t add64(uint64_t x, uint64_t y)
{
    return x + y;
}
static inline uint8_t sub8(uint8_t x, uint8_t y)
{
    return x - y;
}
static inline uint16_t sub16(uint16_t x, uint16_t y)
{
    return x - y;
}
static inline uint32_t sub32(uint32_t x, uint32_t y)
{
    return x - y;
}
static inline uint64_t sub64(uint64_t x, uint64_t y)
{
    return x - y;
}
static inline uint8_t mul8(uint8_t x, uint8_t y)
{
    return x * y;
}
static inline uint16_t mul16(uint16_t x, uint16_t y)
{
    return x * y;
}
static inline uint32_t mul32(uint32_t x, uint32_t y)
{
    return x * y;
}
static inline uint64_t mul64(uint64_t x, uint64_t y)
{
    return x * y;
}
static inline uint8_t udiv8(uint8_t x, uint8_t y)
{
    return x / y;
}
static inline uint16_t udiv16(uint16_t x, uint16_t y)
{
    return x / y;
}
static inline uint32_t udiv32(uint32_t x, uint32_t y)
{
    return x / y;
}
static inline uint64_t udiv64(uint64_t x, uint64_t y)
{
    return x / y;
}
static inline uint8_t udiv_up8(uint8_t x, uint8_t y)
{
    return (x + y - 1) / y;
}
static inline uint16_t udiv_up16(uint16_t x, uint16_t y)
{
    return (x + y - 1) / y;
}
static inline uint32_t udiv_up32(uint32_t x, uint32_t y)
{
    return (x + y - 1) / y;
}
static inline uint64_t udiv_up64(uint64_t x, uint64_t y)
{
    return (x + y - 1) / y;
}
static inline uint8_t umod8(uint8_t x, uint8_t y)
{
    return x % y;
}
static inline uint16_t umod16(uint16_t x, uint16_t y)
{
    return x % y;
}
static inline uint32_t umod32(uint32_t x, uint32_t y)
{
    return x % y;
}
static inline uint64_t umod64(uint64_t x, uint64_t y)
{
    return x % y;
}
static inline uint8_t udiv_safe8(uint8_t x, uint8_t y)
{
    return y == 0 ? 0 : x / y;
}
static inline uint16_t udiv_safe16(uint16_t x, uint16_t y)
{
    return y == 0 ? 0 : x / y;
}
static inline uint32_t udiv_safe32(uint32_t x, uint32_t y)
{
    return y == 0 ? 0 : x / y;
}
static inline uint64_t udiv_safe64(uint64_t x, uint64_t y)
{
    return y == 0 ? 0 : x / y;
}
static inline uint8_t udiv_up_safe8(uint8_t x, uint8_t y)
{
    return y == 0 ? 0 : (x + y - 1) / y;
}
static inline uint16_t udiv_up_safe16(uint16_t x, uint16_t y)
{
    return y == 0 ? 0 : (x + y - 1) / y;
}
static inline uint32_t udiv_up_safe32(uint32_t x, uint32_t y)
{
    return y == 0 ? 0 : (x + y - 1) / y;
}
static inline uint64_t udiv_up_safe64(uint64_t x, uint64_t y)
{
    return y == 0 ? 0 : (x + y - 1) / y;
}
static inline uint8_t umod_safe8(uint8_t x, uint8_t y)
{
    return y == 0 ? 0 : x % y;
}
static inline uint16_t umod_safe16(uint16_t x, uint16_t y)
{
    return y == 0 ? 0 : x % y;
}
static inline uint32_t umod_safe32(uint32_t x, uint32_t y)
{
    return y == 0 ? 0 : x % y;
}
static inline uint64_t umod_safe64(uint64_t x, uint64_t y)
{
    return y == 0 ? 0 : x % y;
}
static inline int8_t sdiv8(int8_t x, int8_t y)
{
    int8_t q = x / y;
    int8_t r = x % y;
    
    return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}
static inline int16_t sdiv16(int16_t x, int16_t y)
{
    int16_t q = x / y;
    int16_t r = x % y;
    
    return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}
static inline int32_t sdiv32(int32_t x, int32_t y)
{
    int32_t q = x / y;
    int32_t r = x % y;
    
    return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}
static inline int64_t sdiv64(int64_t x, int64_t y)
{
    int64_t q = x / y;
    int64_t r = x % y;
    
    return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}
static inline int8_t sdiv_up8(int8_t x, int8_t y)
{
    return sdiv8(x + y - 1, y);
}
static inline int16_t sdiv_up16(int16_t x, int16_t y)
{
    return sdiv16(x + y - 1, y);
}
static inline int32_t sdiv_up32(int32_t x, int32_t y)
{
    return sdiv32(x + y - 1, y);
}
static inline int64_t sdiv_up64(int64_t x, int64_t y)
{
    return sdiv64(x + y - 1, y);
}
static inline int8_t smod8(int8_t x, int8_t y)
{
    int8_t r = x % y;
    
    return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}
static inline int16_t smod16(int16_t x, int16_t y)
{
    int16_t r = x % y;
    
    return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}
static inline int32_t smod32(int32_t x, int32_t y)
{
    int32_t r = x % y;
    
    return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}
static inline int64_t smod64(int64_t x, int64_t y)
{
    int64_t r = x % y;
    
    return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}
static inline int8_t sdiv_safe8(int8_t x, int8_t y)
{
    return y == 0 ? 0 : sdiv8(x, y);
}
static inline int16_t sdiv_safe16(int16_t x, int16_t y)
{
    return y == 0 ? 0 : sdiv16(x, y);
}
static inline int32_t sdiv_safe32(int32_t x, int32_t y)
{
    return y == 0 ? 0 : sdiv32(x, y);
}
static inline int64_t sdiv_safe64(int64_t x, int64_t y)
{
    return y == 0 ? 0 : sdiv64(x, y);
}
static inline int8_t sdiv_up_safe8(int8_t x, int8_t y)
{
    return sdiv_safe8(x + y - 1, y);
}
static inline int16_t sdiv_up_safe16(int16_t x, int16_t y)
{
    return sdiv_safe16(x + y - 1, y);
}
static inline int32_t sdiv_up_safe32(int32_t x, int32_t y)
{
    return sdiv_safe32(x + y - 1, y);
}
static inline int64_t sdiv_up_safe64(int64_t x, int64_t y)
{
    return sdiv_safe64(x + y - 1, y);
}
static inline int8_t smod_safe8(int8_t x, int8_t y)
{
    return y == 0 ? 0 : smod8(x, y);
}
static inline int16_t smod_safe16(int16_t x, int16_t y)
{
    return y == 0 ? 0 : smod16(x, y);
}
static inline int32_t smod_safe32(int32_t x, int32_t y)
{
    return y == 0 ? 0 : smod32(x, y);
}
static inline int64_t smod_safe64(int64_t x, int64_t y)
{
    return y == 0 ? 0 : smod64(x, y);
}
static inline int8_t squot8(int8_t x, int8_t y)
{
    return x / y;
}
static inline int16_t squot16(int16_t x, int16_t y)
{
    return x / y;
}
static inline int32_t squot32(int32_t x, int32_t y)
{
    return x / y;
}
static inline int64_t squot64(int64_t x, int64_t y)
{
    return x / y;
}
static inline int8_t srem8(int8_t x, int8_t y)
{
    return x % y;
}
static inline int16_t srem16(int16_t x, int16_t y)
{
    return x % y;
}
static inline int32_t srem32(int32_t x, int32_t y)
{
    return x % y;
}
static inline int64_t srem64(int64_t x, int64_t y)
{
    return x % y;
}
static inline int8_t squot_safe8(int8_t x, int8_t y)
{
    return y == 0 ? 0 : x / y;
}
static inline int16_t squot_safe16(int16_t x, int16_t y)
{
    return y == 0 ? 0 : x / y;
}
static inline int32_t squot_safe32(int32_t x, int32_t y)
{
    return y == 0 ? 0 : x / y;
}
static inline int64_t squot_safe64(int64_t x, int64_t y)
{
    return y == 0 ? 0 : x / y;
}
static inline int8_t srem_safe8(int8_t x, int8_t y)
{
    return y == 0 ? 0 : x % y;
}
static inline int16_t srem_safe16(int16_t x, int16_t y)
{
    return y == 0 ? 0 : x % y;
}
static inline int32_t srem_safe32(int32_t x, int32_t y)
{
    return y == 0 ? 0 : x % y;
}
static inline int64_t srem_safe64(int64_t x, int64_t y)
{
    return y == 0 ? 0 : x % y;
}
static inline int8_t smin8(int8_t x, int8_t y)
{
    return x < y ? x : y;
}
static inline int16_t smin16(int16_t x, int16_t y)
{
    return x < y ? x : y;
}
static inline int32_t smin32(int32_t x, int32_t y)
{
    return x < y ? x : y;
}
static inline int64_t smin64(int64_t x, int64_t y)
{
    return x < y ? x : y;
}
static inline uint8_t umin8(uint8_t x, uint8_t y)
{
    return x < y ? x : y;
}
static inline uint16_t umin16(uint16_t x, uint16_t y)
{
    return x < y ? x : y;
}
static inline uint32_t umin32(uint32_t x, uint32_t y)
{
    return x < y ? x : y;
}
static inline uint64_t umin64(uint64_t x, uint64_t y)
{
    return x < y ? x : y;
}
static inline int8_t smax8(int8_t x, int8_t y)
{
    return x < y ? y : x;
}
static inline int16_t smax16(int16_t x, int16_t y)
{
    return x < y ? y : x;
}
static inline int32_t smax32(int32_t x, int32_t y)
{
    return x < y ? y : x;
}
static inline int64_t smax64(int64_t x, int64_t y)
{
    return x < y ? y : x;
}
static inline uint8_t umax8(uint8_t x, uint8_t y)
{
    return x < y ? y : x;
}
static inline uint16_t umax16(uint16_t x, uint16_t y)
{
    return x < y ? y : x;
}
static inline uint32_t umax32(uint32_t x, uint32_t y)
{
    return x < y ? y : x;
}
static inline uint64_t umax64(uint64_t x, uint64_t y)
{
    return x < y ? y : x;
}
static inline uint8_t shl8(uint8_t x, uint8_t y)
{
    return x << y;
}
static inline uint16_t shl16(uint16_t x, uint16_t y)
{
    return x << y;
}
static inline uint32_t shl32(uint32_t x, uint32_t y)
{
    return x << y;
}
static inline uint64_t shl64(uint64_t x, uint64_t y)
{
    return x << y;
}
static inline uint8_t lshr8(uint8_t x, uint8_t y)
{
    return x >> y;
}
static inline uint16_t lshr16(uint16_t x, uint16_t y)
{
    return x >> y;
}
static inline uint32_t lshr32(uint32_t x, uint32_t y)
{
    return x >> y;
}
static inline uint64_t lshr64(uint64_t x, uint64_t y)
{
    return x >> y;
}
static inline int8_t ashr8(int8_t x, int8_t y)
{
    return x >> y;
}
static inline int16_t ashr16(int16_t x, int16_t y)
{
    return x >> y;
}
static inline int32_t ashr32(int32_t x, int32_t y)
{
    return x >> y;
}
static inline int64_t ashr64(int64_t x, int64_t y)
{
    return x >> y;
}
static inline uint8_t and8(uint8_t x, uint8_t y)
{
    return x & y;
}
static inline uint16_t and16(uint16_t x, uint16_t y)
{
    return x & y;
}
static inline uint32_t and32(uint32_t x, uint32_t y)
{
    return x & y;
}
static inline uint64_t and64(uint64_t x, uint64_t y)
{
    return x & y;
}
static inline uint8_t or8(uint8_t x, uint8_t y)
{
    return x | y;
}
static inline uint16_t or16(uint16_t x, uint16_t y)
{
    return x | y;
}
static inline uint32_t or32(uint32_t x, uint32_t y)
{
    return x | y;
}
static inline uint64_t or64(uint64_t x, uint64_t y)
{
    return x | y;
}
static inline uint8_t xor8(uint8_t x, uint8_t y)
{
    return x ^ y;
}
static inline uint16_t xor16(uint16_t x, uint16_t y)
{
    return x ^ y;
}
static inline uint32_t xor32(uint32_t x, uint32_t y)
{
    return x ^ y;
}
static inline uint64_t xor64(uint64_t x, uint64_t y)
{
    return x ^ y;
}
static inline bool ult8(uint8_t x, uint8_t y)
{
    return x < y;
}
static inline bool ult16(uint16_t x, uint16_t y)
{
    return x < y;
}
static inline bool ult32(uint32_t x, uint32_t y)
{
    return x < y;
}
static inline bool ult64(uint64_t x, uint64_t y)
{
    return x < y;
}
static inline bool ule8(uint8_t x, uint8_t y)
{
    return x <= y;
}
static inline bool ule16(uint16_t x, uint16_t y)
{
    return x <= y;
}
static inline bool ule32(uint32_t x, uint32_t y)
{
    return x <= y;
}
static inline bool ule64(uint64_t x, uint64_t y)
{
    return x <= y;
}
static inline bool slt8(int8_t x, int8_t y)
{
    return x < y;
}
static inline bool slt16(int16_t x, int16_t y)
{
    return x < y;
}
static inline bool slt32(int32_t x, int32_t y)
{
    return x < y;
}
static inline bool slt64(int64_t x, int64_t y)
{
    return x < y;
}
static inline bool sle8(int8_t x, int8_t y)
{
    return x <= y;
}
static inline bool sle16(int16_t x, int16_t y)
{
    return x <= y;
}
static inline bool sle32(int32_t x, int32_t y)
{
    return x <= y;
}
static inline bool sle64(int64_t x, int64_t y)
{
    return x <= y;
}
static inline int8_t pow8(int8_t x, int8_t y)
{
    int8_t res = 1, rem = y;
    
    while (rem != 0) {
        if (rem & 1)
            res *= x;
        rem >>= 1;
        x *= x;
    }
    return res;
}
static inline int16_t pow16(int16_t x, int16_t y)
{
    int16_t res = 1, rem = y;
    
    while (rem != 0) {
        if (rem & 1)
            res *= x;
        rem >>= 1;
        x *= x;
    }
    return res;
}
static inline int32_t pow32(int32_t x, int32_t y)
{
    int32_t res = 1, rem = y;
    
    while (rem != 0) {
        if (rem & 1)
            res *= x;
        rem >>= 1;
        x *= x;
    }
    return res;
}
static inline int64_t pow64(int64_t x, int64_t y)
{
    int64_t res = 1, rem = y;
    
    while (rem != 0) {
        if (rem & 1)
            res *= x;
        rem >>= 1;
        x *= x;
    }
    return res;
}
static inline bool itob_i8_bool(int8_t x)
{
    return x;
}
static inline bool itob_i16_bool(int16_t x)
{
    return x;
}
static inline bool itob_i32_bool(int32_t x)
{
    return x;
}
static inline bool itob_i64_bool(int64_t x)
{
    return x;
}
static inline int8_t btoi_bool_i8(bool x)
{
    return x;
}
static inline int16_t btoi_bool_i16(bool x)
{
    return x;
}
static inline int32_t btoi_bool_i32(bool x)
{
    return x;
}
static inline int64_t btoi_bool_i64(bool x)
{
    return x;
}
#define sext_i8_i8(x) ((int8_t) (int8_t) x)
#define sext_i8_i16(x) ((int16_t) (int8_t) x)
#define sext_i8_i32(x) ((int32_t) (int8_t) x)
#define sext_i8_i64(x) ((int64_t) (int8_t) x)
#define sext_i16_i8(x) ((int8_t) (int16_t) x)
#define sext_i16_i16(x) ((int16_t) (int16_t) x)
#define sext_i16_i32(x) ((int32_t) (int16_t) x)
#define sext_i16_i64(x) ((int64_t) (int16_t) x)
#define sext_i32_i8(x) ((int8_t) (int32_t) x)
#define sext_i32_i16(x) ((int16_t) (int32_t) x)
#define sext_i32_i32(x) ((int32_t) (int32_t) x)
#define sext_i32_i64(x) ((int64_t) (int32_t) x)
#define sext_i64_i8(x) ((int8_t) (int64_t) x)
#define sext_i64_i16(x) ((int16_t) (int64_t) x)
#define sext_i64_i32(x) ((int32_t) (int64_t) x)
#define sext_i64_i64(x) ((int64_t) (int64_t) x)
#define zext_i8_i8(x) ((int8_t) (uint8_t) x)
#define zext_i8_i16(x) ((int16_t) (uint8_t) x)
#define zext_i8_i32(x) ((int32_t) (uint8_t) x)
#define zext_i8_i64(x) ((int64_t) (uint8_t) x)
#define zext_i16_i8(x) ((int8_t) (uint16_t) x)
#define zext_i16_i16(x) ((int16_t) (uint16_t) x)
#define zext_i16_i32(x) ((int32_t) (uint16_t) x)
#define zext_i16_i64(x) ((int64_t) (uint16_t) x)
#define zext_i32_i8(x) ((int8_t) (uint32_t) x)
#define zext_i32_i16(x) ((int16_t) (uint32_t) x)
#define zext_i32_i32(x) ((int32_t) (uint32_t) x)
#define zext_i32_i64(x) ((int64_t) (uint32_t) x)
#define zext_i64_i8(x) ((int8_t) (uint64_t) x)
#define zext_i64_i16(x) ((int16_t) (uint64_t) x)
#define zext_i64_i32(x) ((int32_t) (uint64_t) x)
#define zext_i64_i64(x) ((int64_t) (uint64_t) x)
#if defined(__OPENCL_VERSION__)
static int32_t futrts_popc8(int8_t x)
{
    return popcount(x);
}
static int32_t futrts_popc16(int16_t x)
{
    return popcount(x);
}
static int32_t futrts_popc32(int32_t x)
{
    return popcount(x);
}
static int32_t futrts_popc64(int64_t x)
{
    return popcount(x);
}
#elif defined(__CUDA_ARCH__)
static int32_t futrts_popc8(int8_t x)
{
    return __popc(zext_i8_i32(x));
}
static int32_t futrts_popc16(int16_t x)
{
    return __popc(zext_i16_i32(x));
}
static int32_t futrts_popc32(int32_t x)
{
    return __popc(x);
}
static int32_t futrts_popc64(int64_t x)
{
    return __popcll(x);
}
#else
static int32_t futrts_popc8(int8_t x)
{
    int c = 0;
    
    for (; x; ++c)
        x &= x - 1;
    return c;
}
static int32_t futrts_popc16(int16_t x)
{
    int c = 0;
    
    for (; x; ++c)
        x &= x - 1;
    return c;
}
static int32_t futrts_popc32(int32_t x)
{
    int c = 0;
    
    for (; x; ++c)
        x &= x - 1;
    return c;
}
static int32_t futrts_popc64(int64_t x)
{
    int c = 0;
    
    for (; x; ++c)
        x &= x - 1;
    return c;
}
#endif
#if defined(__OPENCL_VERSION__)
static uint8_t futrts_mul_hi8(uint8_t a, uint8_t b)
{
    return mul_hi(a, b);
}
static uint16_t futrts_mul_hi16(uint16_t a, uint16_t b)
{
    return mul_hi(a, b);
}
static uint32_t futrts_mul_hi32(uint32_t a, uint32_t b)
{
    return mul_hi(a, b);
}
static uint64_t futrts_mul_hi64(uint64_t a, uint64_t b)
{
    return mul_hi(a, b);
}
#elif defined(__CUDA_ARCH__)
static uint8_t futrts_mul_hi8(uint8_t a, uint8_t b)
{
    uint16_t aa = a;
    uint16_t bb = b;
    
    return aa * bb >> 8;
}
static uint16_t futrts_mul_hi16(uint16_t a, uint16_t b)
{
    uint32_t aa = a;
    uint32_t bb = b;
    
    return aa * bb >> 16;
}
static uint32_t futrts_mul_hi32(uint32_t a, uint32_t b)
{
    return mulhi(a, b);
}
static uint64_t futrts_mul_hi64(uint64_t a, uint64_t b)
{
    return mul64hi(a, b);
}
#else
static uint8_t futrts_mul_hi8(uint8_t a, uint8_t b)
{
    uint16_t aa = a;
    uint16_t bb = b;
    
    return aa * bb >> 8;
}
static uint16_t futrts_mul_hi16(uint16_t a, uint16_t b)
{
    uint32_t aa = a;
    uint32_t bb = b;
    
    return aa * bb >> 16;
}
static uint32_t futrts_mul_hi32(uint32_t a, uint32_t b)
{
    uint64_t aa = a;
    uint64_t bb = b;
    
    return aa * bb >> 32;
}
static uint64_t futrts_mul_hi64(uint64_t a, uint64_t b)
{
    __uint128_t aa = a;
    __uint128_t bb = b;
    
    return aa * bb >> 64;
}
#endif
#if defined(__OPENCL_VERSION__)
static uint8_t futrts_mad_hi8(uint8_t a, uint8_t b, uint8_t c)
{
    return mad_hi(a, b, c);
}
static uint16_t futrts_mad_hi16(uint16_t a, uint16_t b, uint16_t c)
{
    return mad_hi(a, b, c);
}
static uint32_t futrts_mad_hi32(uint32_t a, uint32_t b, uint32_t c)
{
    return mad_hi(a, b, c);
}
static uint64_t futrts_mad_hi64(uint64_t a, uint64_t b, uint64_t c)
{
    return mad_hi(a, b, c);
}
#else
static uint8_t futrts_mad_hi8(uint8_t a, uint8_t b, uint8_t c)
{
    return futrts_mul_hi8(a, b) + c;
}
static uint16_t futrts_mad_hi16(uint16_t a, uint16_t b, uint16_t c)
{
    return futrts_mul_hi16(a, b) + c;
}
static uint32_t futrts_mad_hi32(uint32_t a, uint32_t b, uint32_t c)
{
    return futrts_mul_hi32(a, b) + c;
}
static uint64_t futrts_mad_hi64(uint64_t a, uint64_t b, uint64_t c)
{
    return futrts_mul_hi64(a, b) + c;
}
#endif
#if defined(__OPENCL_VERSION__)
static int32_t futrts_clzz8(int8_t x)
{
    return clz(x);
}
static int32_t futrts_clzz16(int16_t x)
{
    return clz(x);
}
static int32_t futrts_clzz32(int32_t x)
{
    return clz(x);
}
static int32_t futrts_clzz64(int64_t x)
{
    return clz(x);
}
#elif defined(__CUDA_ARCH__)
static int32_t futrts_clzz8(int8_t x)
{
    return __clz(zext_i8_i32(x)) - 24;
}
static int32_t futrts_clzz16(int16_t x)
{
    return __clz(zext_i16_i32(x)) - 16;
}
static int32_t futrts_clzz32(int32_t x)
{
    return __clz(x);
}
static int32_t futrts_clzz64(int64_t x)
{
    return __clzll(x);
}
#else
static int32_t futrts_clzz8(int8_t x)
{
    int n = 0;
    int bits = sizeof(x) * 8;
    
    for (int i = 0; i < bits; i++) {
        if (x < 0)
            break;
        n++;
        x <<= 1;
    }
    return n;
}
static int32_t futrts_clzz16(int16_t x)
{
    int n = 0;
    int bits = sizeof(x) * 8;
    
    for (int i = 0; i < bits; i++) {
        if (x < 0)
            break;
        n++;
        x <<= 1;
    }
    return n;
}
static int32_t futrts_clzz32(int32_t x)
{
    int n = 0;
    int bits = sizeof(x) * 8;
    
    for (int i = 0; i < bits; i++) {
        if (x < 0)
            break;
        n++;
        x <<= 1;
    }
    return n;
}
static int32_t futrts_clzz64(int64_t x)
{
    int n = 0;
    int bits = sizeof(x) * 8;
    
    for (int i = 0; i < bits; i++) {
        if (x < 0)
            break;
        n++;
        x <<= 1;
    }
    return n;
}
#endif
#if defined(__OPENCL_VERSION__)
static int32_t futrts_ctzz8(int8_t x)
{
    int i = 0;
    
    for (; i < 8 && (x & 1) == 0; i++, x >>= 1)
        ;
    return i;
}
static int32_t futrts_ctzz16(int16_t x)
{
    int i = 0;
    
    for (; i < 16 && (x & 1) == 0; i++, x >>= 1)
        ;
    return i;
}
static int32_t futrts_ctzz32(int32_t x)
{
    int i = 0;
    
    for (; i < 32 && (x & 1) == 0; i++, x >>= 1)
        ;
    return i;
}
static int32_t futrts_ctzz64(int64_t x)
{
    int i = 0;
    
    for (; i < 64 && (x & 1) == 0; i++, x >>= 1)
        ;
    return i;
}
#elif defined(__CUDA_ARCH__)
static int32_t futrts_ctzz8(int8_t x)
{
    int y = __ffs(x);
    
    return y == 0 ? 8 : y - 1;
}
static int32_t futrts_ctzz16(int16_t x)
{
    int y = __ffs(x);
    
    return y == 0 ? 16 : y - 1;
}
static int32_t futrts_ctzz32(int32_t x)
{
    int y = __ffs(x);
    
    return y == 0 ? 32 : y - 1;
}
static int32_t futrts_ctzz64(int64_t x)
{
    int y = __ffsll(x);
    
    return y == 0 ? 64 : y - 1;
}
#else
static int32_t futrts_ctzz8(int8_t x)
{
    return x == 0 ? 8 : __builtin_ctz((uint32_t) x);
}
static int32_t futrts_ctzz16(int16_t x)
{
    return x == 0 ? 16 : __builtin_ctz((uint32_t) x);
}
static int32_t futrts_ctzz32(int32_t x)
{
    return x == 0 ? 32 : __builtin_ctz(x);
}
static int32_t futrts_ctzz64(int64_t x)
{
    return x == 0 ? 64 : __builtin_ctzll(x);
}
#endif
static inline float fdiv32(float x, float y)
{
    return x / y;
}
static inline float fadd32(float x, float y)
{
    return x + y;
}
static inline float fsub32(float x, float y)
{
    return x - y;
}
static inline float fmul32(float x, float y)
{
    return x * y;
}
static inline float fmin32(float x, float y)
{
    return fmin(x, y);
}
static inline float fmax32(float x, float y)
{
    return fmax(x, y);
}
static inline float fpow32(float x, float y)
{
    return pow(x, y);
}
static inline bool cmplt32(float x, float y)
{
    return x < y;
}
static inline bool cmple32(float x, float y)
{
    return x <= y;
}
static inline float sitofp_i8_f32(int8_t x)
{
    return (float) x;
}
static inline float sitofp_i16_f32(int16_t x)
{
    return (float) x;
}
static inline float sitofp_i32_f32(int32_t x)
{
    return (float) x;
}
static inline float sitofp_i64_f32(int64_t x)
{
    return (float) x;
}
static inline float uitofp_i8_f32(uint8_t x)
{
    return (float) x;
}
static inline float uitofp_i16_f32(uint16_t x)
{
    return (float) x;
}
static inline float uitofp_i32_f32(uint32_t x)
{
    return (float) x;
}
static inline float uitofp_i64_f32(uint64_t x)
{
    return (float) x;
}
static inline int8_t fptosi_f32_i8(float x)
{
    return (int8_t) x;
}
static inline int16_t fptosi_f32_i16(float x)
{
    return (int16_t) x;
}
static inline int32_t fptosi_f32_i32(float x)
{
    return (int32_t) x;
}
static inline int64_t fptosi_f32_i64(float x)
{
    return (int64_t) x;
}
static inline uint8_t fptoui_f32_i8(float x)
{
    return (uint8_t) x;
}
static inline uint16_t fptoui_f32_i16(float x)
{
    return (uint16_t) x;
}
static inline uint32_t fptoui_f32_i32(float x)
{
    return (uint32_t) x;
}
static inline uint64_t fptoui_f32_i64(float x)
{
    return (uint64_t) x;
}
static inline double fdiv64(double x, double y)
{
    return x / y;
}
static inline double fadd64(double x, double y)
{
    return x + y;
}
static inline double fsub64(double x, double y)
{
    return x - y;
}
static inline double fmul64(double x, double y)
{
    return x * y;
}
static inline double fmin64(double x, double y)
{
    return fmin(x, y);
}
static inline double fmax64(double x, double y)
{
    return fmax(x, y);
}
static inline double fpow64(double x, double y)
{
    return pow(x, y);
}
static inline bool cmplt64(double x, double y)
{
    return x < y;
}
static inline bool cmple64(double x, double y)
{
    return x <= y;
}
static inline double sitofp_i8_f64(int8_t x)
{
    return (double) x;
}
static inline double sitofp_i16_f64(int16_t x)
{
    return (double) x;
}
static inline double sitofp_i32_f64(int32_t x)
{
    return (double) x;
}
static inline double sitofp_i64_f64(int64_t x)
{
    return (double) x;
}
static inline double uitofp_i8_f64(uint8_t x)
{
    return (double) x;
}
static inline double uitofp_i16_f64(uint16_t x)
{
    return (double) x;
}
static inline double uitofp_i32_f64(uint32_t x)
{
    return (double) x;
}
static inline double uitofp_i64_f64(uint64_t x)
{
    return (double) x;
}
static inline int8_t fptosi_f64_i8(double x)
{
    return (int8_t) x;
}
static inline int16_t fptosi_f64_i16(double x)
{
    return (int16_t) x;
}
static inline int32_t fptosi_f64_i32(double x)
{
    return (int32_t) x;
}
static inline int64_t fptosi_f64_i64(double x)
{
    return (int64_t) x;
}
static inline uint8_t fptoui_f64_i8(double x)
{
    return (uint8_t) x;
}
static inline uint16_t fptoui_f64_i16(double x)
{
    return (uint16_t) x;
}
static inline uint32_t fptoui_f64_i32(double x)
{
    return (uint32_t) x;
}
static inline uint64_t fptoui_f64_i64(double x)
{
    return (uint64_t) x;
}
static inline float fpconv_f32_f32(float x)
{
    return (float) x;
}
static inline double fpconv_f32_f64(float x)
{
    return (double) x;
}
static inline float fpconv_f64_f32(double x)
{
    return (float) x;
}
static inline double fpconv_f64_f64(double x)
{
    return (double) x;
}
static inline float futrts_log32(float x)
{
    return log(x);
}
static inline float futrts_log2_32(float x)
{
    return log2(x);
}
static inline float futrts_log10_32(float x)
{
    return log10(x);
}
static inline float futrts_sqrt32(float x)
{
    return sqrt(x);
}
static inline float futrts_exp32(float x)
{
    return exp(x);
}
static inline float futrts_cos32(float x)
{
    return cos(x);
}
static inline float futrts_sin32(float x)
{
    return sin(x);
}
static inline float futrts_tan32(float x)
{
    return tan(x);
}
static inline float futrts_acos32(float x)
{
    return acos(x);
}
static inline float futrts_asin32(float x)
{
    return asin(x);
}
static inline float futrts_atan32(float x)
{
    return atan(x);
}
static inline float futrts_cosh32(float x)
{
    return cosh(x);
}
static inline float futrts_sinh32(float x)
{
    return sinh(x);
}
static inline float futrts_tanh32(float x)
{
    return tanh(x);
}
static inline float futrts_acosh32(float x)
{
    return acosh(x);
}
static inline float futrts_asinh32(float x)
{
    return asinh(x);
}
static inline float futrts_atanh32(float x)
{
    return atanh(x);
}
static inline float futrts_atan2_32(float x, float y)
{
    return atan2(x, y);
}
static inline float futrts_gamma32(float x)
{
    return tgamma(x);
}
static inline float futrts_lgamma32(float x)
{
    return lgamma(x);
}
static inline bool futrts_isnan32(float x)
{
    return isnan(x);
}
static inline bool futrts_isinf32(float x)
{
    return isinf(x);
}
static inline int32_t futrts_to_bits32(float x)
{
    union {
        float f;
        int32_t t;
    } p;
    
    p.f = x;
    return p.t;
}
static inline float futrts_from_bits32(int32_t x)
{
    union {
        int32_t f;
        float t;
    } p;
    
    p.f = x;
    return p.t;
}
#ifdef __OPENCL_VERSION__
static inline float fmod32(float x, float y)
{
    return fmod(x, y);
}
static inline float futrts_round32(float x)
{
    return rint(x);
}
static inline float futrts_floor32(float x)
{
    return floor(x);
}
static inline float futrts_ceil32(float x)
{
    return ceil(x);
}
static inline float futrts_lerp32(float v0, float v1, float t)
{
    return mix(v0, v1, t);
}
static inline float futrts_mad32(float a, float b, float c)
{
    return mad(a, b, c);
}
static inline float futrts_fma32(float a, float b, float c)
{
    return fma(a, b, c);
}
#else
static inline float fmod32(float x, float y)
{
    return fmodf(x, y);
}
static inline float futrts_round32(float x)
{
    return rintf(x);
}
static inline float futrts_floor32(float x)
{
    return floorf(x);
}
static inline float futrts_ceil32(float x)
{
    return ceilf(x);
}
static inline float futrts_lerp32(float v0, float v1, float t)
{
    return v0 + (v1 - v0) * t;
}
static inline float futrts_mad32(float a, float b, float c)
{
    return a * b + c;
}
static inline float futrts_fma32(float a, float b, float c)
{
    return fmaf(a, b, c);
}
#endif
static inline double futrts_log64(double x)
{
    return log(x);
}
static inline double futrts_log2_64(double x)
{
    return log2(x);
}
static inline double futrts_log10_64(double x)
{
    return log10(x);
}
static inline double futrts_sqrt64(double x)
{
    return sqrt(x);
}
static inline double futrts_exp64(double x)
{
    return exp(x);
}
static inline double futrts_cos64(double x)
{
    return cos(x);
}
static inline double futrts_sin64(double x)
{
    return sin(x);
}
static inline double futrts_tan64(double x)
{
    return tan(x);
}
static inline double futrts_acos64(double x)
{
    return acos(x);
}
static inline double futrts_asin64(double x)
{
    return asin(x);
}
static inline double futrts_atan64(double x)
{
    return atan(x);
}
static inline double futrts_cosh64(double x)
{
    return cosh(x);
}
static inline double futrts_sinh64(double x)
{
    return sinh(x);
}
static inline double futrts_tanh64(double x)
{
    return tanh(x);
}
static inline double futrts_acosh64(double x)
{
    return acosh(x);
}
static inline double futrts_asinh64(double x)
{
    return asinh(x);
}
static inline double futrts_atanh64(double x)
{
    return atanh(x);
}
static inline double futrts_atan2_64(double x, double y)
{
    return atan2(x, y);
}
static inline double futrts_gamma64(double x)
{
    return tgamma(x);
}
static inline double futrts_lgamma64(double x)
{
    return lgamma(x);
}
static inline double futrts_fma64(double a, double b, double c)
{
    return fma(a, b, c);
}
static inline double futrts_round64(double x)
{
    return rint(x);
}
static inline double futrts_ceil64(double x)
{
    return ceil(x);
}
static inline double futrts_floor64(double x)
{
    return floor(x);
}
static inline bool futrts_isnan64(double x)
{
    return isnan(x);
}
static inline bool futrts_isinf64(double x)
{
    return isinf(x);
}
static inline int64_t futrts_to_bits64(double x)
{
    union {
        double f;
        int64_t t;
    } p;
    
    p.f = x;
    return p.t;
}
static inline double futrts_from_bits64(int64_t x)
{
    union {
        int64_t f;
        double t;
    } p;
    
    p.f = x;
    return p.t;
}
static inline double fmod64(double x, double y)
{
    return fmod(x, y);
}
#ifdef __OPENCL_VERSION__
static inline double futrts_lerp64(double v0, double v1, double t)
{
    return mix(v0, v1, t);
}
static inline double futrts_mad64(double a, double b, double c)
{
    return mad(a, b, c);
}
#else
static inline double futrts_lerp64(double v0, double v1, double t)
{
    return v0 + (v1 - v0) * t;
}
static inline double futrts_mad64(double a, double b, double c)
{
    return a * b + c;
}
#endif
static int init_constants(struct futhark_context *);
static int free_constants(struct futhark_context *);
struct memblock {
    int *references;
    char *mem;
    int64_t size;
    const char *desc;
} ;
struct futhark_context_config {
    int debugging;
} ;
struct futhark_context_config *futhark_context_config_new(void)
{
    struct futhark_context_config *cfg =
                                  (struct futhark_context_config *) malloc(sizeof(struct futhark_context_config));
    
    if (cfg == NULL)
        return NULL;
    cfg->debugging = 0;
    return cfg;
}
void futhark_context_config_free(struct futhark_context_config *cfg)
{
    free(cfg);
}
void futhark_context_config_set_debugging(struct futhark_context_config *cfg,
                                          int detail)
{
    cfg->debugging = detail;
}
void futhark_context_config_set_logging(struct futhark_context_config *cfg,
                                        int detail)
{
    /* Does nothing for this backend. */
    (void) cfg;
    (void) detail;
}
struct futhark_context {
    int detail_memory;
    int debugging;
    int profiling;
    lock_t lock;
    char *error;
    int profiling_paused;
    int64_t peak_mem_usage_default;
    int64_t cur_mem_usage_default;
    struct {
        int dummy;
    } constants;
} ;
struct futhark_context *futhark_context_new(struct futhark_context_config *cfg)
{
    struct futhark_context *ctx =
                           (struct futhark_context *) malloc(sizeof(struct futhark_context));
    
    if (ctx == NULL)
        return NULL;
    ctx->detail_memory = cfg->debugging;
    ctx->debugging = cfg->debugging;
    ctx->profiling = cfg->debugging;
    ctx->error = NULL;
    create_lock(&ctx->lock);
    ctx->peak_mem_usage_default = 0;
    ctx->cur_mem_usage_default = 0;
    init_constants(ctx);
    return ctx;
}
void futhark_context_free(struct futhark_context *ctx)
{
    free_constants(ctx);
    free_lock(&ctx->lock);
    free(ctx);
}
int futhark_context_sync(struct futhark_context *ctx)
{
    (void) ctx;
    return 0;
}
int futhark_context_clear_caches(struct futhark_context *ctx)
{
    (void) ctx;
    return 0;
}
static int memblock_unref(struct futhark_context *ctx, struct memblock *block,
                          const char *desc)
{
    if (block->references != NULL) {
        *block->references -= 1;
        if (ctx->detail_memory)
            fprintf(stderr,
                    "Unreferencing block %s (allocated as %s) in %s: %d references remaining.\n",
                    desc, block->desc, "default space", *block->references);
        if (*block->references == 0) {
            ctx->cur_mem_usage_default -= block->size;
            free(block->mem);
            free(block->references);
            if (ctx->detail_memory)
                fprintf(stderr,
                        "%lld bytes freed (now allocated: %lld bytes)\n",
                        (long long) block->size,
                        (long long) ctx->cur_mem_usage_default);
        }
        block->references = NULL;
    }
    return 0;
}
static int memblock_alloc(struct futhark_context *ctx, struct memblock *block,
                          int64_t size, const char *desc)
{
    if (size < 0)
        futhark_panic(1,
                      "Negative allocation of %lld bytes attempted for %s in %s.\n",
                      (long long) size, desc, "default space",
                      ctx->cur_mem_usage_default);
    
    int ret = memblock_unref(ctx, block, desc);
    
    ctx->cur_mem_usage_default += size;
    if (ctx->detail_memory)
        fprintf(stderr,
                "Allocating %lld bytes for %s in %s (then allocated: %lld bytes)",
                (long long) size, desc, "default space",
                (long long) ctx->cur_mem_usage_default);
    if (ctx->cur_mem_usage_default > ctx->peak_mem_usage_default) {
        ctx->peak_mem_usage_default = ctx->cur_mem_usage_default;
        if (ctx->detail_memory)
            fprintf(stderr, " (new peak).\n");
    } else if (ctx->detail_memory)
        fprintf(stderr, ".\n");
    block->mem = (char *) malloc(size);
    block->references = (int *) malloc(sizeof(int));
    *block->references = 1;
    block->size = size;
    block->desc = desc;
    return ret;
}
static int memblock_set(struct futhark_context *ctx, struct memblock *lhs,
                        struct memblock *rhs, const char *lhs_desc)
{
    int ret = memblock_unref(ctx, lhs, lhs_desc);
    
    if (rhs->references != NULL)
        (*rhs->references)++;
    *lhs = *rhs;
    return ret;
}
char *futhark_context_report(struct futhark_context *ctx)
{
    struct str_builder builder;
    
    str_builder_init(&builder);
    if (ctx->detail_memory || ctx->profiling) {
        { }
    }
    if (ctx->profiling) { }
    return builder.str;
}
char *futhark_context_get_error(struct futhark_context *ctx)
{
    char *error = ctx->error;
    
    ctx->error = NULL;
    return error;
}
void futhark_context_pause_profiling(struct futhark_context *ctx)
{
    ctx->profiling_paused = 1;
}
void futhark_context_unpause_profiling(struct futhark_context *ctx)
{
    ctx->profiling_paused = 0;
}
static int futrts_eWiseAddEntry(struct futhark_context *ctx,
                                struct memblock *out_mem_p_29126,
                                int64_t *out_out_arrsizze_29127,
                                struct memblock *out_mem_p_29128,
                                int64_t *out_out_arrsizze_29129,
                                struct memblock *out_mem_p_29130,
                                struct memblock leftValues_mem_27964,
                                struct memblock leftColumns_mem_27965,
                                struct memblock leftOffsets_mem_27966,
                                struct memblock rightValues_mem_27967,
                                struct memblock rightColumns_mem_27968,
                                struct memblock rightOffsets_mem_27969,
                                int64_t n_25311, int64_t m_25312,
                                int64_t k_25313);
static int futrts_eWiseAddMultEntry(struct futhark_context *ctx,
                                    struct memblock *out_mem_p_29154,
                                    int64_t *out_out_arrsizze_29155,
                                    struct memblock *out_mem_p_29156,
                                    int64_t *out_out_arrsizze_29157,
                                    struct memblock *out_mem_p_29158,
                                    struct memblock leftValues_mem_27964,
                                    struct memblock leftColumns_mem_27965,
                                    struct memblock leftOffsets_mem_27966,
                                    struct memblock rightValues_mem_27967,
                                    struct memblock rightColumns_mem_27968,
                                    struct memblock rightOffsets_mem_27969,
                                    int64_t n_26449, int64_t m_26450,
                                    int64_t k_26451);
static int futrts_eWiseMultEntry(struct futhark_context *ctx,
                                 struct memblock *out_mem_p_29203,
                                 int64_t *out_out_arrsizze_29204,
                                 struct memblock *out_mem_p_29205,
                                 int64_t *out_out_arrsizze_29206,
                                 struct memblock *out_mem_p_29207,
                                 struct memblock leftValues_mem_27964,
                                 struct memblock leftColumns_mem_27965,
                                 struct memblock leftOffsets_mem_27966,
                                 struct memblock rightValues_mem_27967,
                                 struct memblock rightColumns_mem_27968,
                                 struct memblock rightOffsets_mem_27969,
                                 int64_t n_25922, int64_t m_25923,
                                 int64_t k_25924);
static int init_constants(struct futhark_context *ctx)
{
    (void) ctx;
    
    int err = 0;
    
    
  cleanup:
    return err;
}
static int free_constants(struct futhark_context *ctx)
{
    (void) ctx;
    return 0;
}
static int futrts_eWiseAddEntry(struct futhark_context *ctx,
                                struct memblock *out_mem_p_29126,
                                int64_t *out_out_arrsizze_29127,
                                struct memblock *out_mem_p_29128,
                                int64_t *out_out_arrsizze_29129,
                                struct memblock *out_mem_p_29130,
                                struct memblock leftValues_mem_27964,
                                struct memblock leftColumns_mem_27965,
                                struct memblock leftOffsets_mem_27966,
                                struct memblock rightValues_mem_27967,
                                struct memblock rightColumns_mem_27968,
                                struct memblock rightOffsets_mem_27969,
                                int64_t n_25311, int64_t m_25312,
                                int64_t k_25313)
{
    (void) ctx;
    
    int err = 0;
    size_t mem_27971_cached_sizze_29131 = 0;
    char *mem_27971 = NULL;
    size_t mem_27989_cached_sizze_29132 = 0;
    char *mem_27989 = NULL;
    size_t mem_27991_cached_sizze_29133 = 0;
    char *mem_27991 = NULL;
    size_t mem_27993_cached_sizze_29134 = 0;
    char *mem_27993 = NULL;
    size_t mem_27995_cached_sizze_29135 = 0;
    char *mem_27995 = NULL;
    size_t mem_27997_cached_sizze_29136 = 0;
    char *mem_27997 = NULL;
    size_t mem_28083_cached_sizze_29137 = 0;
    char *mem_28083 = NULL;
    size_t mem_28085_cached_sizze_29138 = 0;
    char *mem_28085 = NULL;
    size_t mem_28115_cached_sizze_29139 = 0;
    char *mem_28115 = NULL;
    size_t mem_28117_cached_sizze_29140 = 0;
    char *mem_28117 = NULL;
    size_t mem_28119_cached_sizze_29141 = 0;
    char *mem_28119 = NULL;
    size_t mem_28121_cached_sizze_29142 = 0;
    char *mem_28121 = NULL;
    size_t mem_28123_cached_sizze_29143 = 0;
    char *mem_28123 = NULL;
    size_t mem_28125_cached_sizze_29144 = 0;
    char *mem_28125 = NULL;
    size_t mem_28127_cached_sizze_29145 = 0;
    char *mem_28127 = NULL;
    size_t mem_28129_cached_sizze_29146 = 0;
    char *mem_28129 = NULL;
    size_t mem_28131_cached_sizze_29147 = 0;
    char *mem_28131 = NULL;
    size_t mem_28133_cached_sizze_29148 = 0;
    char *mem_28133 = NULL;
    size_t mem_28149_cached_sizze_29149 = 0;
    char *mem_28149 = NULL;
    size_t mem_28151_cached_sizze_29150 = 0;
    char *mem_28151 = NULL;
    size_t mem_28153_cached_sizze_29151 = 0;
    char *mem_28153 = NULL;
    size_t mem_28155_cached_sizze_29152 = 0;
    char *mem_28155 = NULL;
    size_t mem_28157_cached_sizze_29153 = 0;
    char *mem_28157 = NULL;
    struct memblock out_mem_29007;
    
    out_mem_29007.references = NULL;
    
    int64_t out_arrsizze_29008;
    struct memblock out_mem_29009;
    
    out_mem_29009.references = NULL;
    
    int64_t out_arrsizze_29010;
    struct memblock out_mem_29011;
    
    out_mem_29011.references = NULL;
    
    int64_t bytes_27970 = (int64_t) 8 * m_25312;
    
    if (mem_27971_cached_sizze_29131 < (size_t) bytes_27970) {
        mem_27971 = realloc(mem_27971, bytes_27970);
        mem_27971_cached_sizze_29131 = bytes_27970;
    }
    
    int64_t discard_27618;
    int64_t scanacc_27614 = (int64_t) 0;
    
    for (int64_t i_27616 = 0; i_27616 < m_25312; i_27616++) {
        int64_t x_25325 = ((int64_t *) leftOffsets_mem_27966.mem)[i_27616];
        int64_t x_25327 = ((int64_t *) rightOffsets_mem_27969.mem)[i_27616];
        bool cond_25328 = i_27616 == (int64_t) 0;
        int64_t prevx2_25329;
        
        if (cond_25328) {
            prevx2_25329 = i_27616;
        } else {
            int64_t prevx2_f_res_25330 = sub64(i_27616, (int64_t) 1);
            
            prevx2_25329 = prevx2_f_res_25330;
        }
        
        int64_t prevy2_25331;
        
        if (cond_25328) {
            prevy2_25331 = i_27616;
        } else {
            int64_t prevy2_f_res_25332 = sub64(i_27616, (int64_t) 1);
            
            prevy2_25331 = prevy2_f_res_25332;
        }
        
        bool x_25333 = sle64((int64_t) 0, prevy2_25331);
        bool y_25334 = slt64(prevy2_25331, m_25312);
        bool bounds_check_25335 = x_25333 && y_25334;
        bool index_certs_25336;
        
        if (!bounds_check_25335) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", prevy2_25331,
                                   "] out of bounds for array of shape [",
                                   m_25312, "].",
                                   "-> #0  sparse.fut:112:58-78\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:109:19-114:50\n   #4  sparse.fut:131:23-48\n   #5  sparse.fut:220:17-59\n   #6  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_25337 =
                ((int64_t *) rightOffsets_mem_27969.mem)[prevy2_25331];
        int64_t j_m_i_25338 = sub64(x_25327, i_25337);
        bool empty_slice_25339 = j_m_i_25338 == (int64_t) 0;
        int64_t m_25340 = sub64(j_m_i_25338, (int64_t) 1);
        int64_t i_p_m_t_s_25341 = add64(i_25337, m_25340);
        bool zzero_leq_i_p_m_t_s_25342 = sle64((int64_t) 0, i_p_m_t_s_25341);
        bool i_p_m_t_s_leq_w_25343 = slt64(i_p_m_t_s_25341, k_25313);
        bool zzero_lte_i_25344 = sle64((int64_t) 0, i_25337);
        bool i_lte_j_25345 = sle64(i_25337, x_25327);
        bool y_25346 = i_p_m_t_s_leq_w_25343 && zzero_lte_i_25344;
        bool y_25347 = zzero_leq_i_p_m_t_s_25342 && y_25346;
        bool y_25348 = i_lte_j_25345 && y_25347;
        bool forwards_ok_25349 = zzero_lte_i_25344 && y_25348;
        bool ok_or_empty_25350 = empty_slice_25339 || forwards_ok_25349;
        bool index_certs_25351;
        
        if (!ok_or_empty_25350) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_25337, ":", x_25327,
                          "] out of bounds for array of shape [", k_25313, "].",
                          "-> #0  sparse.fut:112:44-82\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:109:19-114:50\n   #4  sparse.fut:131:23-48\n   #5  sparse.fut:220:17-59\n   #6  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool x_25353 = sle64((int64_t) 0, prevx2_25329);
        bool y_25354 = slt64(prevx2_25329, m_25312);
        bool bounds_check_25355 = x_25353 && y_25354;
        bool index_certs_25356;
        
        if (!bounds_check_25355) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", prevx2_25329,
                                   "] out of bounds for array of shape [",
                                   m_25312, "].",
                                   "-> #0  sparse.fut:111:53-72\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:109:19-114:50\n   #4  sparse.fut:131:23-48\n   #5  sparse.fut:220:17-59\n   #6  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_25357 = ((int64_t *) leftOffsets_mem_27966.mem)[prevx2_25329];
        int64_t j_m_i_25358 = sub64(x_25325, i_25357);
        bool empty_slice_25359 = j_m_i_25358 == (int64_t) 0;
        int64_t m_25360 = sub64(j_m_i_25358, (int64_t) 1);
        int64_t i_p_m_t_s_25361 = add64(i_25357, m_25360);
        bool zzero_leq_i_p_m_t_s_25362 = sle64((int64_t) 0, i_p_m_t_s_25361);
        bool i_p_m_t_s_leq_w_25363 = slt64(i_p_m_t_s_25361, n_25311);
        bool zzero_lte_i_25364 = sle64((int64_t) 0, i_25357);
        bool i_lte_j_25365 = sle64(i_25357, x_25325);
        bool y_25366 = i_p_m_t_s_leq_w_25363 && zzero_lte_i_25364;
        bool y_25367 = zzero_leq_i_p_m_t_s_25362 && y_25366;
        bool y_25368 = i_lte_j_25365 && y_25367;
        bool forwards_ok_25369 = zzero_lte_i_25364 && y_25368;
        bool ok_or_empty_25370 = empty_slice_25359 || forwards_ok_25369;
        bool index_certs_25371;
        
        if (!ok_or_empty_25370) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_25357, ":", x_25325,
                          "] out of bounds for array of shape [", n_25311, "].",
                          "-> #0  sparse.fut:111:40-76\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:109:19-114:50\n   #4  sparse.fut:131:23-48\n   #5  sparse.fut:220:17-59\n   #6  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t unionSizze_res_25373;
        
        if (empty_slice_25359) {
            unionSizze_res_25373 = j_m_i_25338;
        } else {
            int64_t conc_tmp_25374 = j_m_i_25338 + j_m_i_25358;
            int64_t j_m_i_25375 = sub64(conc_tmp_25374, (int64_t) 1);
            int64_t bytes_27979 = (int64_t) 8 * conc_tmp_25374;
            int64_t bytes_27996 = (int64_t) 4 * conc_tmp_25374;
            int64_t unionSizze_res_f_res_25376;
            
            if (empty_slice_25339) {
                unionSizze_res_f_res_25376 = j_m_i_25358;
            } else {
                struct memblock mem_27980;
                
                mem_27980.references = NULL;
                if (memblock_alloc(ctx, &mem_27980, bytes_27979, "mem_27980")) {
                    err = 1;
                    goto cleanup;
                }
                
                int64_t tmp_offs_29014 = (int64_t) 0;
                
                memmove(mem_27980.mem + tmp_offs_29014 * (int64_t) 8,
                        leftColumns_mem_27965.mem + i_25357 * (int64_t) 8,
                        j_m_i_25358 * (int64_t) sizeof(int64_t));
                tmp_offs_29014 += j_m_i_25358;
                memmove(mem_27980.mem + tmp_offs_29014 * (int64_t) 8,
                        rightColumns_mem_27968.mem + i_25337 * (int64_t) 8,
                        j_m_i_25338 * (int64_t) sizeof(int64_t));
                tmp_offs_29014 += j_m_i_25338;
                
                bool cond_25378 = conc_tmp_25374 == (int64_t) 0;
                int32_t iters_25379;
                
                if (cond_25378) {
                    iters_25379 = 0;
                } else {
                    iters_25379 = 32;
                }
                
                bool loop_nonempty_25380 = slt32(0, iters_25379);
                bool x_25381 = sle64((int64_t) 0, j_m_i_25375);
                bool y_25382 = slt64(j_m_i_25375, conc_tmp_25374);
                bool bounds_check_25383 = x_25381 && y_25382;
                bool loop_not_taken_25384 = !loop_nonempty_25380;
                bool protect_assert_disj_25385 = bounds_check_25383 ||
                     loop_not_taken_25384;
                bool index_certs_25386;
                
                if (!protect_assert_disj_25385) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", j_m_i_25375,
                                  "] out of bounds for array of shape [",
                                  conc_tmp_25374, "].",
                                  "-> #0  /prelude/array.fut:18:29-34\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:25:24-35\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:53:31-62\n   #3  sparse.fut:3:25-4:48\n   #4  sparse.fut:40:12-24\n   #5  sparse.fut:111:30-112:82\n   #6  /prelude/soacs.fut:56:19-23\n   #7  /prelude/soacs.fut:56:3-37\n   #8  sparse.fut:109:19-114:50\n   #9  sparse.fut:131:23-48\n   #10 sparse.fut:220:17-59\n   #11 sparse.fut:211:1-221:48\n");
                    if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                if (mem_27989_cached_sizze_29132 < (size_t) bytes_27979) {
                    mem_27989 = realloc(mem_27989, bytes_27979);
                    mem_27989_cached_sizze_29132 = bytes_27979;
                }
                if (mem_27991_cached_sizze_29133 < (size_t) bytes_27979) {
                    mem_27991 = realloc(mem_27991, bytes_27979);
                    mem_27991_cached_sizze_29133 = bytes_27979;
                }
                if (mem_27993_cached_sizze_29134 < (size_t) bytes_27979) {
                    mem_27993 = realloc(mem_27993, bytes_27979);
                    mem_27993_cached_sizze_29134 = bytes_27979;
                }
                if (mem_27995_cached_sizze_29135 < (size_t) bytes_27979) {
                    mem_27995 = realloc(mem_27995, bytes_27979);
                    mem_27995_cached_sizze_29135 = bytes_27979;
                }
                if (mem_27997_cached_sizze_29136 < (size_t) bytes_27996) {
                    mem_27997 = realloc(mem_27997, bytes_27996);
                    mem_27997_cached_sizze_29136 = bytes_27996;
                }
                
                struct memblock defunc_2_radix_sort_res_mem_28076;
                
                defunc_2_radix_sort_res_mem_28076.references = NULL;
                
                struct memblock mem_param_27985;
                
                mem_param_27985.references = NULL;
                if (memblock_set(ctx, &mem_param_27985, &mem_27980,
                                 "mem_27980") != 0)
                    return 1;
                for (int32_t i_25388 = 0; i_25388 < iters_25379; i_25388++) {
                    int32_t defunc_2_radix_sort_step_arg_25390 = mul32(2,
                                                                       i_25388);
                    int32_t defunc_0_get_bit_arg_25391 = add32(1,
                                                               defunc_2_radix_sort_step_arg_25390);
                    int64_t i32_res_25392 =
                            sext_i32_i64(defunc_0_get_bit_arg_25391);
                    int64_t i32_res_25393 =
                            sext_i32_i64(defunc_2_radix_sort_step_arg_25390);
                    int64_t discard_27592;
                    int64_t discard_27593;
                    int64_t discard_27594;
                    int64_t discard_27595;
                    int64_t scanacc_27577;
                    int64_t scanacc_27578;
                    int64_t scanacc_27579;
                    int64_t scanacc_27580;
                    
                    scanacc_27577 = (int64_t) 0;
                    scanacc_27578 = (int64_t) 0;
                    scanacc_27579 = (int64_t) 0;
                    scanacc_27580 = (int64_t) 0;
                    for (int64_t i_27586 = 0; i_27586 < conc_tmp_25374;
                         i_27586++) {
                        int64_t x_25421 =
                                ((int64_t *) mem_param_27985.mem)[i_27586];
                        int64_t zgzg_res_25422 = ashr64(x_25421, i32_res_25392);
                        int64_t za_res_25423 = (int64_t) 1 & zgzg_res_25422;
                        int32_t to_i32_res_25424 = sext_i64_i32(za_res_25423);
                        int32_t x_25425 = mul32(2, to_i32_res_25424);
                        int64_t zgzg_res_25426 = ashr64(x_25421, i32_res_25393);
                        int64_t za_res_25427 = (int64_t) 1 & zgzg_res_25426;
                        int32_t to_i32_res_25428 = sext_i64_i32(za_res_25427);
                        int32_t defunc_0_f_res_25429 = add32(x_25425,
                                                             to_i32_res_25428);
                        bool cond_25430 = defunc_0_f_res_25429 == 0;
                        int64_t defunc_0_f_res_25431 =
                                btoi_bool_i64(cond_25430);
                        int64_t defunc_0_f_res_25432;
                        int64_t defunc_0_f_res_25433;
                        int64_t defunc_0_f_res_25434;
                        
                        if (cond_25430) {
                            defunc_0_f_res_25432 = (int64_t) 0;
                            defunc_0_f_res_25433 = (int64_t) 0;
                            defunc_0_f_res_25434 = (int64_t) 0;
                        } else {
                            bool cond_25435 = defunc_0_f_res_25429 == 1;
                            int64_t defunc_0_f_res_f_res_25436 =
                                    btoi_bool_i64(cond_25435);
                            int64_t defunc_0_f_res_f_res_25437;
                            int64_t defunc_0_f_res_f_res_25438;
                            
                            if (cond_25435) {
                                defunc_0_f_res_f_res_25437 = (int64_t) 0;
                                defunc_0_f_res_f_res_25438 = (int64_t) 0;
                            } else {
                                bool cond_25439 = defunc_0_f_res_25429 == 2;
                                int64_t defunc_0_f_res_f_res_f_res_25440 =
                                        btoi_bool_i64(cond_25439);
                                bool cond_neg_25441 = !cond_25439;
                                int64_t defunc_0_f_res_f_res_f_res_25442 =
                                        btoi_bool_i64(cond_neg_25441);
                                
                                defunc_0_f_res_f_res_25437 =
                                    defunc_0_f_res_f_res_f_res_25440;
                                defunc_0_f_res_f_res_25438 =
                                    defunc_0_f_res_f_res_f_res_25442;
                            }
                            defunc_0_f_res_25432 = defunc_0_f_res_f_res_25436;
                            defunc_0_f_res_25433 = defunc_0_f_res_f_res_25437;
                            defunc_0_f_res_25434 = defunc_0_f_res_f_res_25438;
                        }
                        
                        int64_t defunc_1_op_res_25417 =
                                add64(defunc_0_f_res_25431, scanacc_27577);
                        int64_t defunc_1_op_res_25418 =
                                add64(defunc_0_f_res_25432, scanacc_27578);
                        int64_t defunc_1_op_res_25419 =
                                add64(defunc_0_f_res_25433, scanacc_27579);
                        int64_t defunc_1_op_res_25420 =
                                add64(defunc_0_f_res_25434, scanacc_27580);
                        
                        ((int64_t *) mem_27989)[i_27586] =
                            defunc_1_op_res_25417;
                        ((int64_t *) mem_27991)[i_27586] =
                            defunc_1_op_res_25418;
                        ((int64_t *) mem_27993)[i_27586] =
                            defunc_1_op_res_25419;
                        ((int64_t *) mem_27995)[i_27586] =
                            defunc_1_op_res_25420;
                        ((int32_t *) mem_27997)[i_27586] = defunc_0_f_res_25429;
                        
                        int64_t scanacc_tmp_29017 = defunc_1_op_res_25417;
                        int64_t scanacc_tmp_29018 = defunc_1_op_res_25418;
                        int64_t scanacc_tmp_29019 = defunc_1_op_res_25419;
                        int64_t scanacc_tmp_29020 = defunc_1_op_res_25420;
                        
                        scanacc_27577 = scanacc_tmp_29017;
                        scanacc_27578 = scanacc_tmp_29018;
                        scanacc_27579 = scanacc_tmp_29019;
                        scanacc_27580 = scanacc_tmp_29020;
                    }
                    discard_27592 = scanacc_27577;
                    discard_27593 = scanacc_27578;
                    discard_27594 = scanacc_27579;
                    discard_27595 = scanacc_27580;
                    
                    int64_t last_res_25448 =
                            ((int64_t *) mem_27989)[j_m_i_25375];
                    int64_t last_res_25449 =
                            ((int64_t *) mem_27991)[j_m_i_25375];
                    int64_t last_res_25450 =
                            ((int64_t *) mem_27993)[j_m_i_25375];
                    struct memblock mem_28059;
                    
                    mem_28059.references = NULL;
                    if (memblock_alloc(ctx, &mem_28059, bytes_27979,
                                       "mem_28059")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28059.mem + (int64_t) 0, mem_param_27985.mem +
                            (int64_t) 0, conc_tmp_25374 *
                            (int64_t) sizeof(int64_t));
                    for (int64_t write_iter_27596 = 0; write_iter_27596 <
                         conc_tmp_25374; write_iter_27596++) {
                        int32_t write_iv_27598 =
                                ((int32_t *) mem_27997)[write_iter_27596];
                        int64_t write_iv_27599 =
                                ((int64_t *) mem_27989)[write_iter_27596];
                        int64_t write_iv_27600 =
                                ((int64_t *) mem_27991)[write_iter_27596];
                        int64_t write_iv_27601 =
                                ((int64_t *) mem_27993)[write_iter_27596];
                        int64_t write_iv_27602 =
                                ((int64_t *) mem_27995)[write_iter_27596];
                        bool match_lit_25459 = 0 == write_iv_27598;
                        int64_t defunc_1_f_res_25460;
                        
                        if (match_lit_25459) {
                            int64_t case_res_25461 = sub64(write_iv_27599,
                                                           (int64_t) 1);
                            
                            defunc_1_f_res_25460 = case_res_25461;
                        } else {
                            bool match_lit_25462 = 1 == write_iv_27598;
                            int64_t x_25463;
                            
                            if (match_lit_25462) {
                                int64_t x_25464 = add64(last_res_25448,
                                                        write_iv_27600);
                                int64_t case_res_25465 = sub64(x_25464,
                                                               (int64_t) 1);
                                
                                x_25463 = case_res_25465;
                            } else {
                                bool match_lit_25466 = 2 == write_iv_27598;
                                int64_t x_25467;
                                
                                if (match_lit_25466) {
                                    int64_t x_25468 = add64(last_res_25448,
                                                            last_res_25449);
                                    int64_t x_25469 = add64(x_25468,
                                                            write_iv_27601);
                                    int64_t case_res_25470 = sub64(x_25469,
                                                                   (int64_t) 1);
                                    
                                    x_25467 = case_res_25470;
                                } else {
                                    int64_t x_25471 = add64(last_res_25448,
                                                            last_res_25449);
                                    int64_t x_25472 = add64(last_res_25450,
                                                            x_25471);
                                    int64_t x_25473 = add64(x_25472,
                                                            write_iv_27602);
                                    int64_t defunc_1_f_res_res_25474 =
                                            sub64(x_25473, (int64_t) 1);
                                    
                                    x_25467 = defunc_1_f_res_res_25474;
                                }
                                x_25463 = x_25467;
                            }
                            defunc_1_f_res_25460 = x_25463;
                        }
                        
                        bool less_than_zzero_27604 = slt64(defunc_1_f_res_25460,
                                                           (int64_t) 0);
                        bool greater_than_sizze_27605 = sle64(conc_tmp_25374,
                                                              defunc_1_f_res_25460);
                        bool outside_bounds_dim_27606 = less_than_zzero_27604 ||
                             greater_than_sizze_27605;
                        
                        if (!outside_bounds_dim_27606) {
                            memmove(mem_28059.mem + defunc_1_f_res_25460 *
                                    (int64_t) 8, mem_param_27985.mem +
                                    write_iter_27596 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                    }
                    
                    struct memblock mem_param_tmp_29015;
                    
                    mem_param_tmp_29015.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29015, &mem_28059,
                                     "mem_28059") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_27985,
                                     &mem_param_tmp_29015,
                                     "mem_param_tmp_29015") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29015,
                                       "mem_param_tmp_29015") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28059, "mem_28059") != 0)
                        return 1;
                }
                if (memblock_set(ctx, &defunc_2_radix_sort_res_mem_28076,
                                 &mem_param_27985, "mem_param_27985") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                    return 1;
                
                bool empty_slice_25475 = j_m_i_25375 == (int64_t) 0;
                bool i_lte_j_25476 = sle64((int64_t) 1, conc_tmp_25374);
                bool y_25477 = bounds_check_25383 && i_lte_j_25476;
                bool ok_or_empty_25478 = empty_slice_25475 || y_25477;
                bool index_certs_25479;
                
                if (!ok_or_empty_25478) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", (int64_t) 1,
                                  ":] out of bounds for array of shape [",
                                  conc_tmp_25374, "].",
                                  "-> #0  sparse.fut:43:97-106\n   #1  sparse.fut:111:30-112:82\n   #2  /prelude/soacs.fut:56:19-23\n   #3  /prelude/soacs.fut:56:3-37\n   #4  sparse.fut:109:19-114:50\n   #5  sparse.fut:131:23-48\n   #6  sparse.fut:220:17-59\n   #7  sparse.fut:211:1-221:48\n");
                    if (memblock_unref(ctx, &mem_param_27985,
                                       "mem_param_27985") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28076,
                                       "defunc_2_radix_sort_res_mem_28076") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                
                int64_t defunc_2_reduce_res_25482;
                int64_t redout_27610 = (int64_t) 1;
                
                for (int64_t i_27612 = 0; i_27612 < j_m_i_25375; i_27612++) {
                    int64_t slice_27923 = (int64_t) 1 + i_27612;
                    int64_t x_25497 =
                            ((int64_t *) defunc_2_radix_sort_res_mem_28076.mem)[slice_27923];
                    bool y_25490 = slt64(i_27612, conc_tmp_25374);
                    bool index_certs_25492;
                    
                    if (!y_25490) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", i_27612,
                                      "] out of bounds for array of shape [",
                                      conc_tmp_25374, "].",
                                      "-> #0  sparse.fut:43:41-51\n   #1  sparse.fut:43:4-106\n   #2  sparse.fut:111:30-112:82\n   #3  /prelude/soacs.fut:56:19-23\n   #4  /prelude/soacs.fut:56:3-37\n   #5  sparse.fut:109:19-114:50\n   #6  sparse.fut:131:23-48\n   #7  sparse.fut:220:17-59\n   #8  sparse.fut:211:1-221:48\n");
                        if (memblock_unref(ctx, &mem_param_27985,
                                           "mem_param_27985") != 0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &defunc_2_radix_sort_res_mem_28076,
                                           "defunc_2_radix_sort_res_mem_28076") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    int64_t y_25493 =
                            ((int64_t *) defunc_2_radix_sort_res_mem_28076.mem)[i_27612];
                    bool cond_25494 = x_25497 == y_25493;
                    int64_t defunc_1_op_res_25495;
                    
                    if (cond_25494) {
                        defunc_1_op_res_25495 = redout_27610;
                    } else {
                        int64_t defunc_1_op_res_f_res_25496 = add64((int64_t) 1,
                                                                    redout_27610);
                        
                        defunc_1_op_res_25495 = defunc_1_op_res_f_res_25496;
                    }
                    
                    int64_t redout_tmp_29027 = defunc_1_op_res_25495;
                    
                    redout_27610 = redout_tmp_29027;
                }
                defunc_2_reduce_res_25482 = redout_27610;
                if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28076,
                                   "defunc_2_radix_sort_res_mem_28076") != 0)
                    return 1;
                unionSizze_res_f_res_25376 = defunc_2_reduce_res_25482;
                if (memblock_unref(ctx, &mem_param_27985, "mem_param_27985") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28076,
                                   "defunc_2_radix_sort_res_mem_28076") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                    return 1;
            }
            unionSizze_res_25373 = unionSizze_res_f_res_25376;
        }
        
        int64_t defunc_1_op_res_25324 = add64(unionSizze_res_25373,
                                              scanacc_27614);
        
        ((int64_t *) mem_27971)[i_27616] = defunc_1_op_res_25324;
        
        int64_t scanacc_tmp_29012 = defunc_1_op_res_25324;
        
        scanacc_27614 = scanacc_tmp_29012;
    }
    discard_27618 = scanacc_27614;
    
    int64_t i_25499 = sub64(m_25312, (int64_t) 1);
    bool x_25500 = sle64((int64_t) 0, i_25499);
    bool y_25501 = slt64(i_25499, m_25312);
    bool bounds_check_25502 = x_25500 && y_25501;
    bool index_certs_25503;
    
    if (!bounds_check_25502) {
        ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                               "Index [", i_25499,
                               "] out of bounds for array of shape [", m_25312,
                               "].",
                               "-> #0  sparse.fut:115:30-41\n   #1  sparse.fut:131:23-48\n   #2  sparse.fut:220:17-59\n   #3  sparse.fut:211:1-221:48\n");
        if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
            return 1;
        return 1;
    }
    
    int64_t eWiseAddOffsets_res_25504 = ((int64_t *) mem_27971)[i_25499];
    bool bounds_invalid_upwards_25505 = slt64(eWiseAddOffsets_res_25504,
                                              (int64_t) 0);
    bool valid_25506 = !bounds_invalid_upwards_25505;
    bool range_valid_c_25507;
    
    if (!valid_25506) {
        ctx->error = msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                               "Range ", (int64_t) 0, "..", (int64_t) 1, "..<",
                               eWiseAddOffsets_res_25504, " is invalid.",
                               "-> #0  /prelude/array.fut:60:3-10\n   #1  /prelude/array.fut:70:18-23\n   #2  sparse.fut:132:16-35\n   #3  sparse.fut:220:17-59\n   #4  sparse.fut:211:1-221:48\n");
        if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
            return 1;
        return 1;
    }
    
    int64_t bytes_28082 = (int64_t) 8 * eWiseAddOffsets_res_25504;
    
    if (mem_28083_cached_sizze_29137 < (size_t) bytes_28082) {
        mem_28083 = realloc(mem_28083, bytes_28082);
        mem_28083_cached_sizze_29137 = bytes_28082;
    }
    for (int64_t i_29028 = 0; i_29028 < eWiseAddOffsets_res_25504; i_29028++) {
        ((double *) mem_28083)[i_29028] = 0.0;
    }
    if (mem_28085_cached_sizze_29138 < (size_t) bytes_28082) {
        mem_28085 = realloc(mem_28085, bytes_28082);
        mem_28085_cached_sizze_29138 = bytes_28082;
    }
    for (int64_t i_29029 = 0; i_29029 < eWiseAddOffsets_res_25504; i_29029++) {
        ((int64_t *) mem_28085)[i_29029] = (int64_t) 0;
    }
    for (int64_t i_25512 = 0; i_25512 < i_25499; i_25512++) {
        bool y_25515 = slt64(i_25512, m_25312);
        bool index_certs_25516;
        
        if (!y_25515) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", i_25512,
                                   "] out of bounds for array of shape [",
                                   m_25312, "].",
                                   "-> #0  sparse.fut:140:46-61\n   #1  sparse.fut:220:17-59\n   #2  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_25517 = ((int64_t *) rightOffsets_mem_27969.mem)[i_25512];
        int64_t i_25518 = add64((int64_t) 1, i_25512);
        bool x_25519 = sle64((int64_t) 0, i_25518);
        bool y_25520 = slt64(i_25518, m_25312);
        bool bounds_check_25521 = x_25519 && y_25520;
        bool index_certs_25522;
        
        if (!bounds_check_25521) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", i_25518,
                                   "] out of bounds for array of shape [",
                                   m_25312, "].",
                                   "-> #0  sparse.fut:140:63-80\n   #1  sparse.fut:220:17-59\n   #2  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t j_25523 = ((int64_t *) rightOffsets_mem_27969.mem)[i_25518];
        int64_t j_m_i_25524 = sub64(j_25523, i_25517);
        bool empty_slice_25525 = j_m_i_25524 == (int64_t) 0;
        int64_t m_25526 = sub64(j_m_i_25524, (int64_t) 1);
        int64_t i_p_m_t_s_25527 = add64(i_25517, m_25526);
        bool zzero_leq_i_p_m_t_s_25528 = sle64((int64_t) 0, i_p_m_t_s_25527);
        bool i_p_m_t_s_leq_w_25529 = slt64(i_p_m_t_s_25527, k_25313);
        bool zzero_lte_i_25530 = sle64((int64_t) 0, i_25517);
        bool i_lte_j_25531 = sle64(i_25517, j_25523);
        bool y_25532 = i_p_m_t_s_leq_w_25529 && zzero_lte_i_25530;
        bool y_25533 = zzero_leq_i_p_m_t_s_25528 && y_25532;
        bool y_25534 = i_lte_j_25531 && y_25533;
        bool forwards_ok_25535 = zzero_lte_i_25530 && y_25534;
        bool ok_or_empty_25536 = empty_slice_25525 || forwards_ok_25535;
        bool index_certs_25537;
        
        if (!ok_or_empty_25536) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_25517, ":", j_25523,
                          "] out of bounds for array of shape [", k_25313, "].",
                          "-> #0  sparse.fut:140:32-81\n   #1  sparse.fut:220:17-59\n   #2  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_25539 = ((int64_t *) leftOffsets_mem_27966.mem)[i_25512];
        int64_t j_25540 = ((int64_t *) leftOffsets_mem_27966.mem)[i_25518];
        int64_t j_m_i_25541 = sub64(j_25540, i_25539);
        bool empty_slice_25542 = j_m_i_25541 == (int64_t) 0;
        int64_t m_25543 = sub64(j_m_i_25541, (int64_t) 1);
        int64_t i_p_m_t_s_25544 = add64(i_25539, m_25543);
        bool zzero_leq_i_p_m_t_s_25545 = sle64((int64_t) 0, i_p_m_t_s_25544);
        bool i_p_m_t_s_leq_w_25546 = slt64(i_p_m_t_s_25544, n_25311);
        bool zzero_lte_i_25547 = sle64((int64_t) 0, i_25539);
        bool i_lte_j_25548 = sle64(i_25539, j_25540);
        bool y_25549 = i_p_m_t_s_leq_w_25546 && zzero_lte_i_25547;
        bool y_25550 = zzero_leq_i_p_m_t_s_25545 && y_25549;
        bool y_25551 = i_lte_j_25548 && y_25550;
        bool forwards_ok_25552 = zzero_lte_i_25547 && y_25551;
        bool ok_or_empty_25553 = empty_slice_25542 || forwards_ok_25552;
        bool index_certs_25554;
        
        if (!ok_or_empty_25553) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_25539, ":", j_25540,
                          "] out of bounds for array of shape [", n_25311, "].",
                          "-> #0  sparse.fut:139:32-78\n   #1  sparse.fut:220:17-59\n   #2  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t bytes_28100 = (int64_t) 8 * j_m_i_25524;
        struct memblock mem_28101;
        
        mem_28101.references = NULL;
        if (memblock_alloc(ctx, &mem_28101, bytes_28100, "mem_28101")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_29032 = 0; i_29032 < j_m_i_25524; i_29032++) {
            ((int64_t *) mem_28101.mem)[i_29032] = (int64_t) 0;
        }
        
        struct memblock mem_28103;
        
        mem_28103.references = NULL;
        if (memblock_alloc(ctx, &mem_28103, bytes_28100, "mem_28103")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_29033 = 0; i_29033 < j_m_i_25524; i_29033++) {
            int64_t x_29034 = (int64_t) 0 + i_29033 * (int64_t) 1;
            
            ((int64_t *) mem_28103.mem)[i_29033] = x_29034;
        }
        
        struct memblock mem_28105;
        
        mem_28105.references = NULL;
        if (memblock_alloc(ctx, &mem_28105, bytes_28100, "mem_28105")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_29035 = 0; i_29035 < j_m_i_25524; i_29035++) {
            ((int64_t *) mem_28105.mem)[i_29035] = (int64_t) 1;
        }
        
        int64_t conc_tmp_25559 = j_m_i_25524 + j_m_i_25541;
        int64_t bytes_28106 = (int64_t) 8 * j_m_i_25541;
        int64_t bytes_28110 = (int64_t) 8 * conc_tmp_25559;
        int64_t sizze_25560;
        int64_t sizze_25561;
        int64_t sizze_25562;
        int64_t sizze_25563;
        int64_t unionArrays_res_ixfn_28452;
        int64_t unionArrays_res_ixfn_28453;
        int64_t unionArrays_res_ixfn_28455;
        int64_t unionArrays_res_ixfn_28457;
        int64_t unionArrays_res_ixfn_28458;
        int64_t unionArrays_res_ixfn_28462;
        int64_t unionArrays_res_ixfn_28463;
        int64_t unionArrays_res_ixfn_28467;
        int64_t unionArrays_res_ixfn_28468;
        struct memblock unionArrays_res_mem_28456;
        
        unionArrays_res_mem_28456.references = NULL;
        
        struct memblock unionArrays_res_mem_28461;
        
        unionArrays_res_mem_28461.references = NULL;
        
        struct memblock unionArrays_res_mem_28466;
        
        unionArrays_res_mem_28466.references = NULL;
        
        struct memblock unionArrays_res_mem_28471;
        
        unionArrays_res_mem_28471.references = NULL;
        if (empty_slice_25542) {
            sizze_25560 = j_m_i_25524;
            sizze_25561 = j_m_i_25524;
            sizze_25562 = j_m_i_25524;
            sizze_25563 = j_m_i_25524;
            unionArrays_res_ixfn_28452 = j_m_i_25524;
            unionArrays_res_ixfn_28453 = k_25313;
            unionArrays_res_ixfn_28455 = i_25517;
            unionArrays_res_ixfn_28457 = j_m_i_25524;
            unionArrays_res_ixfn_28458 = j_m_i_25524;
            unionArrays_res_ixfn_28462 = j_m_i_25524;
            unionArrays_res_ixfn_28463 = j_m_i_25524;
            unionArrays_res_ixfn_28467 = j_m_i_25524;
            unionArrays_res_ixfn_28468 = j_m_i_25524;
            if (memblock_set(ctx, &unionArrays_res_mem_28456,
                             &rightColumns_mem_27968,
                             "rightColumns_mem_27968") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28461, &mem_28105,
                             "mem_28105") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28466, &mem_28103,
                             "mem_28103") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28471, &mem_28101,
                             "mem_28101") != 0)
                return 1;
        } else {
            struct memblock mem_28107;
            
            mem_28107.references = NULL;
            if (memblock_alloc(ctx, &mem_28107, bytes_28106, "mem_28107")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_29036 = 0; i_29036 < j_m_i_25541; i_29036++) {
                ((int64_t *) mem_28107.mem)[i_29036] = (int64_t) 0;
            }
            
            struct memblock mem_28109;
            
            mem_28109.references = NULL;
            if (memblock_alloc(ctx, &mem_28109, bytes_28106, "mem_28109")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_29037 = 0; i_29037 < j_m_i_25541; i_29037++) {
                int64_t x_29038 = (int64_t) 0 + i_29037 * (int64_t) 1;
                
                ((int64_t *) mem_28109.mem)[i_29037] = x_29038;
            }
            
            struct memblock mem_28111;
            
            mem_28111.references = NULL;
            if (memblock_alloc(ctx, &mem_28111, bytes_28110, "mem_28111")) {
                err = 1;
                goto cleanup;
            }
            
            int64_t tmp_offs_29039 = (int64_t) 0;
            
            memmove(mem_28111.mem + tmp_offs_29039 * (int64_t) 8,
                    leftColumns_mem_27965.mem + i_25539 * (int64_t) 8,
                    j_m_i_25541 * (int64_t) sizeof(int64_t));
            tmp_offs_29039 += j_m_i_25541;
            memmove(mem_28111.mem + tmp_offs_29039 * (int64_t) 8,
                    rightColumns_mem_27968.mem + i_25517 * (int64_t) 8,
                    j_m_i_25524 * (int64_t) sizeof(int64_t));
            tmp_offs_29039 += j_m_i_25524;
            
            struct memblock mem_28113;
            
            mem_28113.references = NULL;
            if (memblock_alloc(ctx, &mem_28113, bytes_28110, "mem_28113")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_29040 = 0; i_29040 < conc_tmp_25559; i_29040++) {
                int64_t x_29041 = (int64_t) 0 + i_29040 * (int64_t) 1;
                
                ((int64_t *) mem_28113.mem)[i_29040] = x_29041;
            }
            
            bool cond_25572 = conc_tmp_25559 == (int64_t) 0;
            int32_t iters_25573;
            
            if (cond_25572) {
                iters_25573 = 0;
            } else {
                iters_25573 = 32;
            }
            
            bool loop_nonempty_25574 = slt32(0, iters_25573);
            int64_t i_25575 = sub64(conc_tmp_25559, (int64_t) 1);
            bool x_25576 = sle64((int64_t) 0, i_25575);
            bool y_25577 = slt64(i_25575, conc_tmp_25559);
            bool bounds_check_25578 = x_25576 && y_25577;
            bool loop_not_taken_25579 = !loop_nonempty_25574;
            bool protect_assert_disj_25580 = bounds_check_25578 ||
                 loop_not_taken_25579;
            
            if (mem_28115_cached_sizze_29139 < (size_t) bytes_28110) {
                mem_28115 = realloc(mem_28115, bytes_28110);
                mem_28115_cached_sizze_29139 = bytes_28110;
            }
            if (mem_28117_cached_sizze_29140 < (size_t) bytes_28110) {
                mem_28117 = realloc(mem_28117, bytes_28110);
                mem_28117_cached_sizze_29140 = bytes_28110;
            }
            if (mem_28119_cached_sizze_29141 < (size_t) bytes_28110) {
                mem_28119 = realloc(mem_28119, bytes_28110);
                mem_28119_cached_sizze_29141 = bytes_28110;
            }
            if (mem_28121_cached_sizze_29142 < (size_t) bytes_28110) {
                mem_28121 = realloc(mem_28121, bytes_28110);
                mem_28121_cached_sizze_29142 = bytes_28110;
            }
            if (mem_28123_cached_sizze_29143 < (size_t) bytes_28110) {
                mem_28123 = realloc(mem_28123, bytes_28110);
                mem_28123_cached_sizze_29143 = bytes_28110;
            }
            if (mem_28125_cached_sizze_29144 < (size_t) bytes_28110) {
                mem_28125 = realloc(mem_28125, bytes_28110);
                mem_28125_cached_sizze_29144 = bytes_28110;
            }
            if (mem_28127_cached_sizze_29145 < (size_t) bytes_28110) {
                mem_28127 = realloc(mem_28127, bytes_28110);
                mem_28127_cached_sizze_29145 = bytes_28110;
            }
            if (mem_28129_cached_sizze_29146 < (size_t) bytes_28110) {
                mem_28129 = realloc(mem_28129, bytes_28110);
                mem_28129_cached_sizze_29146 = bytes_28110;
            }
            if (mem_28131_cached_sizze_29147 < (size_t) bytes_28110) {
                mem_28131 = realloc(mem_28131, bytes_28110);
                mem_28131_cached_sizze_29147 = bytes_28110;
            }
            if (mem_28133_cached_sizze_29148 < (size_t) bytes_28110) {
                mem_28133 = realloc(mem_28133, bytes_28110);
                mem_28133_cached_sizze_29148 = bytes_28110;
            }
            
            int64_t bytes_28156 = (int64_t) 4 * conc_tmp_25559;
            int64_t unionArrays_res_f_res_ixfn_28433;
            
            if (empty_slice_25525) {
                unionArrays_res_f_res_ixfn_28433 = n_25311;
            } else {
                unionArrays_res_f_res_ixfn_28433 = conc_tmp_25559;
            }
            
            int64_t unionArrays_res_f_res_ixfn_28435;
            
            if (empty_slice_25525) {
                unionArrays_res_f_res_ixfn_28435 = i_25539;
            } else {
                unionArrays_res_f_res_ixfn_28435 = (int64_t) 0;
            }
            
            int64_t unionArrays_res_f_res_ixfn_28438;
            
            if (empty_slice_25525) {
                unionArrays_res_f_res_ixfn_28438 = j_m_i_25541;
            } else {
                unionArrays_res_f_res_ixfn_28438 = conc_tmp_25559;
            }
            
            int64_t unionArrays_res_f_res_ixfn_28443;
            
            if (empty_slice_25525) {
                unionArrays_res_f_res_ixfn_28443 = j_m_i_25541;
            } else {
                unionArrays_res_f_res_ixfn_28443 = conc_tmp_25559;
            }
            
            int64_t unionArrays_res_f_res_ixfn_28448;
            
            if (empty_slice_25525) {
                unionArrays_res_f_res_ixfn_28448 = j_m_i_25541;
            } else {
                unionArrays_res_f_res_ixfn_28448 = conc_tmp_25559;
            }
            
            int64_t sizze_25581;
            int64_t sizze_25582;
            int64_t sizze_25583;
            int64_t sizze_25584;
            int64_t unionArrays_res_f_res_ixfn_28432;
            int64_t unionArrays_res_f_res_ixfn_28437;
            int64_t unionArrays_res_f_res_ixfn_28442;
            int64_t unionArrays_res_f_res_ixfn_28447;
            struct memblock unionArrays_res_f_res_mem_28436;
            
            unionArrays_res_f_res_mem_28436.references = NULL;
            
            struct memblock unionArrays_res_f_res_mem_28441;
            
            unionArrays_res_f_res_mem_28441.references = NULL;
            
            struct memblock unionArrays_res_f_res_mem_28446;
            
            unionArrays_res_f_res_mem_28446.references = NULL;
            
            struct memblock unionArrays_res_f_res_mem_28451;
            
            unionArrays_res_f_res_mem_28451.references = NULL;
            if (empty_slice_25525) {
                sizze_25581 = j_m_i_25541;
                sizze_25582 = j_m_i_25541;
                sizze_25583 = j_m_i_25541;
                sizze_25584 = j_m_i_25541;
                unionArrays_res_f_res_ixfn_28432 = j_m_i_25541;
                unionArrays_res_f_res_ixfn_28437 = j_m_i_25541;
                unionArrays_res_f_res_ixfn_28442 = j_m_i_25541;
                unionArrays_res_f_res_ixfn_28447 = j_m_i_25541;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28436,
                                 &leftColumns_mem_27965,
                                 "leftColumns_mem_27965") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28441,
                                 &mem_28107, "mem_28107") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28446,
                                 &mem_28109, "mem_28109") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28451,
                                 &mem_28107, "mem_28107") != 0)
                    return 1;
            } else {
                bool index_certs_25589;
                
                if (!protect_assert_disj_25580) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", i_25575,
                                  "] out of bounds for array of shape [",
                                  conc_tmp_25559, "].",
                                  "-> #0  /prelude/array.fut:18:29-34\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:25:24-35\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:53:31-62\n   #3  /prelude/functional.fut:9:42-44\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:59:3-61:47\n   #5  lib/github.com/diku-dk/sorts/radix_sort.fut:68:3-51\n   #6  sparse.fut:5:34-6:77\n   #7  sparse.fut:53:12-35\n   #8  sparse.fut:138:59-140:81\n   #9  sparse.fut:220:17-59\n   #10 sparse.fut:211:1-221:48\n");
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28451,
                                       "unionArrays_res_f_res_mem_28451") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28446,
                                       "unionArrays_res_f_res_mem_28446") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28441,
                                       "unionArrays_res_f_res_mem_28441") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28436,
                                       "unionArrays_res_f_res_mem_28436") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28113, "mem_28113") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28111, "mem_28111") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28109, "mem_28109") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28107, "mem_28107") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                       "unionArrays_res_mem_28471") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                       "unionArrays_res_mem_28466") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                       "unionArrays_res_mem_28461") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                       "unionArrays_res_mem_28456") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                if (mem_28149_cached_sizze_29149 < (size_t) bytes_28110) {
                    mem_28149 = realloc(mem_28149, bytes_28110);
                    mem_28149_cached_sizze_29149 = bytes_28110;
                }
                if (mem_28151_cached_sizze_29150 < (size_t) bytes_28110) {
                    mem_28151 = realloc(mem_28151, bytes_28110);
                    mem_28151_cached_sizze_29150 = bytes_28110;
                }
                if (mem_28153_cached_sizze_29151 < (size_t) bytes_28110) {
                    mem_28153 = realloc(mem_28153, bytes_28110);
                    mem_28153_cached_sizze_29151 = bytes_28110;
                }
                if (mem_28155_cached_sizze_29152 < (size_t) bytes_28110) {
                    mem_28155 = realloc(mem_28155, bytes_28110);
                    mem_28155_cached_sizze_29152 = bytes_28110;
                }
                if (mem_28157_cached_sizze_29153 < (size_t) bytes_28156) {
                    mem_28157 = realloc(mem_28157, bytes_28156);
                    mem_28157_cached_sizze_29153 = bytes_28156;
                }
                
                struct memblock defunc_0_f_res_mem_28250;
                
                defunc_0_f_res_mem_28250.references = NULL;
                
                struct memblock defunc_0_f_res_mem_28255;
                
                defunc_0_f_res_mem_28255.references = NULL;
                
                struct memblock mem_param_28138;
                
                mem_param_28138.references = NULL;
                
                struct memblock mem_param_28143;
                
                mem_param_28143.references = NULL;
                if (memblock_set(ctx, &mem_param_28138, &mem_28111,
                                 "mem_28111") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_28143, &mem_28113,
                                 "mem_28113") != 0)
                    return 1;
                for (int32_t i_25592 = 0; i_25592 < iters_25573; i_25592++) {
                    int32_t defunc_2_radix_sort_step_arg_25595 = mul32(2,
                                                                       i_25592);
                    int32_t defunc_0_get_bit_arg_25596 = add32(1,
                                                               defunc_2_radix_sort_step_arg_25595);
                    int64_t i32_res_25597 =
                            sext_i32_i64(defunc_0_get_bit_arg_25596);
                    int64_t i32_res_25598 =
                            sext_i32_i64(defunc_2_radix_sort_step_arg_25595);
                    int64_t discard_27639;
                    int64_t discard_27640;
                    int64_t discard_27641;
                    int64_t discard_27642;
                    int64_t scanacc_27624;
                    int64_t scanacc_27625;
                    int64_t scanacc_27626;
                    int64_t scanacc_27627;
                    
                    scanacc_27624 = (int64_t) 0;
                    scanacc_27625 = (int64_t) 0;
                    scanacc_27626 = (int64_t) 0;
                    scanacc_27627 = (int64_t) 0;
                    for (int64_t i_27633 = 0; i_27633 < conc_tmp_25559;
                         i_27633++) {
                        int64_t x_25626 =
                                ((int64_t *) mem_param_28138.mem)[i_27633];
                        int64_t zgzg_res_25627 = ashr64(x_25626, i32_res_25597);
                        int64_t za_res_25628 = (int64_t) 1 & zgzg_res_25627;
                        int32_t to_i32_res_25629 = sext_i64_i32(za_res_25628);
                        int32_t x_25630 = mul32(2, to_i32_res_25629);
                        int64_t zgzg_res_25631 = ashr64(x_25626, i32_res_25598);
                        int64_t za_res_25632 = (int64_t) 1 & zgzg_res_25631;
                        int32_t to_i32_res_25633 = sext_i64_i32(za_res_25632);
                        int32_t defunc_0_f_res_25634 = add32(x_25630,
                                                             to_i32_res_25633);
                        bool cond_25635 = defunc_0_f_res_25634 == 0;
                        int64_t defunc_0_f_res_25636 =
                                btoi_bool_i64(cond_25635);
                        int64_t defunc_0_f_res_25637;
                        int64_t defunc_0_f_res_25638;
                        int64_t defunc_0_f_res_25639;
                        
                        if (cond_25635) {
                            defunc_0_f_res_25637 = (int64_t) 0;
                            defunc_0_f_res_25638 = (int64_t) 0;
                            defunc_0_f_res_25639 = (int64_t) 0;
                        } else {
                            bool cond_25640 = defunc_0_f_res_25634 == 1;
                            int64_t defunc_0_f_res_f_res_25641 =
                                    btoi_bool_i64(cond_25640);
                            int64_t defunc_0_f_res_f_res_25642;
                            int64_t defunc_0_f_res_f_res_25643;
                            
                            if (cond_25640) {
                                defunc_0_f_res_f_res_25642 = (int64_t) 0;
                                defunc_0_f_res_f_res_25643 = (int64_t) 0;
                            } else {
                                bool cond_25644 = defunc_0_f_res_25634 == 2;
                                int64_t defunc_0_f_res_f_res_f_res_25645 =
                                        btoi_bool_i64(cond_25644);
                                bool cond_neg_25646 = !cond_25644;
                                int64_t defunc_0_f_res_f_res_f_res_25647 =
                                        btoi_bool_i64(cond_neg_25646);
                                
                                defunc_0_f_res_f_res_25642 =
                                    defunc_0_f_res_f_res_f_res_25645;
                                defunc_0_f_res_f_res_25643 =
                                    defunc_0_f_res_f_res_f_res_25647;
                            }
                            defunc_0_f_res_25637 = defunc_0_f_res_f_res_25641;
                            defunc_0_f_res_25638 = defunc_0_f_res_f_res_25642;
                            defunc_0_f_res_25639 = defunc_0_f_res_f_res_25643;
                        }
                        
                        int64_t defunc_1_op_res_25622 =
                                add64(defunc_0_f_res_25636, scanacc_27624);
                        int64_t defunc_1_op_res_25623 =
                                add64(defunc_0_f_res_25637, scanacc_27625);
                        int64_t defunc_1_op_res_25624 =
                                add64(defunc_0_f_res_25638, scanacc_27626);
                        int64_t defunc_1_op_res_25625 =
                                add64(defunc_0_f_res_25639, scanacc_27627);
                        
                        ((int64_t *) mem_28149)[i_27633] =
                            defunc_1_op_res_25622;
                        ((int64_t *) mem_28151)[i_27633] =
                            defunc_1_op_res_25623;
                        ((int64_t *) mem_28153)[i_27633] =
                            defunc_1_op_res_25624;
                        ((int64_t *) mem_28155)[i_27633] =
                            defunc_1_op_res_25625;
                        ((int32_t *) mem_28157)[i_27633] = defunc_0_f_res_25634;
                        
                        int64_t scanacc_tmp_29046 = defunc_1_op_res_25622;
                        int64_t scanacc_tmp_29047 = defunc_1_op_res_25623;
                        int64_t scanacc_tmp_29048 = defunc_1_op_res_25624;
                        int64_t scanacc_tmp_29049 = defunc_1_op_res_25625;
                        
                        scanacc_27624 = scanacc_tmp_29046;
                        scanacc_27625 = scanacc_tmp_29047;
                        scanacc_27626 = scanacc_tmp_29048;
                        scanacc_27627 = scanacc_tmp_29049;
                    }
                    discard_27639 = scanacc_27624;
                    discard_27640 = scanacc_27625;
                    discard_27641 = scanacc_27626;
                    discard_27642 = scanacc_27627;
                    
                    int64_t last_res_25653 = ((int64_t *) mem_28149)[i_25575];
                    int64_t last_res_25654 = ((int64_t *) mem_28151)[i_25575];
                    int64_t last_res_25655 = ((int64_t *) mem_28153)[i_25575];
                    struct memblock mem_28219;
                    
                    mem_28219.references = NULL;
                    if (memblock_alloc(ctx, &mem_28219, bytes_28110,
                                       "mem_28219")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28219.mem + (int64_t) 0, mem_param_28138.mem +
                            (int64_t) 0, conc_tmp_25559 *
                            (int64_t) sizeof(int64_t));
                    
                    struct memblock mem_28221;
                    
                    mem_28221.references = NULL;
                    if (memblock_alloc(ctx, &mem_28221, bytes_28110,
                                       "mem_28221")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28221.mem + (int64_t) 0, mem_param_28143.mem +
                            (int64_t) 0, conc_tmp_25559 *
                            (int64_t) sizeof(int64_t));
                    for (int64_t write_iter_27643 = 0; write_iter_27643 <
                         conc_tmp_25559; write_iter_27643++) {
                        int32_t write_iv_27646 =
                                ((int32_t *) mem_28157)[write_iter_27643];
                        int64_t write_iv_27647 =
                                ((int64_t *) mem_28149)[write_iter_27643];
                        int64_t write_iv_27648 =
                                ((int64_t *) mem_28151)[write_iter_27643];
                        int64_t write_iv_27649 =
                                ((int64_t *) mem_28153)[write_iter_27643];
                        int64_t write_iv_27650 =
                                ((int64_t *) mem_28155)[write_iter_27643];
                        bool match_lit_25667 = 0 == write_iv_27646;
                        int64_t defunc_1_f_res_25668;
                        
                        if (match_lit_25667) {
                            int64_t case_res_25669 = sub64(write_iv_27647,
                                                           (int64_t) 1);
                            
                            defunc_1_f_res_25668 = case_res_25669;
                        } else {
                            bool match_lit_25670 = 1 == write_iv_27646;
                            int64_t x_25671;
                            
                            if (match_lit_25670) {
                                int64_t x_25672 = add64(last_res_25653,
                                                        write_iv_27648);
                                int64_t case_res_25673 = sub64(x_25672,
                                                               (int64_t) 1);
                                
                                x_25671 = case_res_25673;
                            } else {
                                bool match_lit_25674 = 2 == write_iv_27646;
                                int64_t x_25675;
                                
                                if (match_lit_25674) {
                                    int64_t x_25676 = add64(last_res_25653,
                                                            last_res_25654);
                                    int64_t x_25677 = add64(x_25676,
                                                            write_iv_27649);
                                    int64_t case_res_25678 = sub64(x_25677,
                                                                   (int64_t) 1);
                                    
                                    x_25675 = case_res_25678;
                                } else {
                                    int64_t x_25679 = add64(last_res_25653,
                                                            last_res_25654);
                                    int64_t x_25680 = add64(last_res_25655,
                                                            x_25679);
                                    int64_t x_25681 = add64(x_25680,
                                                            write_iv_27650);
                                    int64_t defunc_1_f_res_res_25682 =
                                            sub64(x_25681, (int64_t) 1);
                                    
                                    x_25675 = defunc_1_f_res_res_25682;
                                }
                                x_25671 = x_25675;
                            }
                            defunc_1_f_res_25668 = x_25671;
                        }
                        
                        bool less_than_zzero_27653 = slt64(defunc_1_f_res_25668,
                                                           (int64_t) 0);
                        bool greater_than_sizze_27654 = sle64(conc_tmp_25559,
                                                              defunc_1_f_res_25668);
                        bool outside_bounds_dim_27655 = less_than_zzero_27653 ||
                             greater_than_sizze_27654;
                        
                        if (!outside_bounds_dim_27655) {
                            memmove(mem_28219.mem + defunc_1_f_res_25668 *
                                    (int64_t) 8, mem_param_28138.mem +
                                    write_iter_27643 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                        if (!outside_bounds_dim_27655) {
                            memmove(mem_28221.mem + defunc_1_f_res_25668 *
                                    (int64_t) 8, mem_param_28143.mem +
                                    write_iter_27643 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                    }
                    
                    struct memblock mem_param_tmp_29042;
                    
                    mem_param_tmp_29042.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29042, &mem_28219,
                                     "mem_28219") != 0)
                        return 1;
                    
                    struct memblock mem_param_tmp_29043;
                    
                    mem_param_tmp_29043.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29043, &mem_28221,
                                     "mem_28221") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_28138,
                                     &mem_param_tmp_29042,
                                     "mem_param_tmp_29042") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_28143,
                                     &mem_param_tmp_29043,
                                     "mem_param_tmp_29043") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29043,
                                       "mem_param_tmp_29043") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29042,
                                       "mem_param_tmp_29042") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28221, "mem_28221") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28219, "mem_28219") != 0)
                        return 1;
                }
                if (memblock_set(ctx, &defunc_0_f_res_mem_28250,
                                 &mem_param_28138, "mem_param_28138") != 0)
                    return 1;
                if (memblock_set(ctx, &defunc_0_f_res_mem_28255,
                                 &mem_param_28143, "mem_param_28143") != 0)
                    return 1;
                for (int64_t i_27673 = 0; i_27673 < conc_tmp_25559; i_27673++) {
                    int64_t x_25687 =
                            ((int64_t *) defunc_0_f_res_mem_28255.mem)[i_27673];
                    bool x_25688 = sle64((int64_t) 0, x_25687);
                    bool y_25689 = slt64(x_25687, conc_tmp_25559);
                    bool bounds_check_25690 = x_25688 && y_25689;
                    bool index_certs_25691;
                    
                    if (!bounds_check_25690) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", x_25687,
                                      "] out of bounds for array of shape [",
                                      conc_tmp_25559, "].",
                                      "-> #0  lib/github.com/diku-dk/sorts/radix_sort.fut:62:28-32\n   #1  /prelude/functional.fut:9:42-44\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:59:3-62:33\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:68:3-51\n   #4  sparse.fut:5:34-6:77\n   #5  sparse.fut:53:12-35\n   #6  sparse.fut:138:59-140:81\n   #7  sparse.fut:220:17-59\n   #8  sparse.fut:211:1-221:48\n");
                        if (memblock_unref(ctx, &mem_param_28143,
                                           "mem_param_28143") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_param_28138,
                                           "mem_param_28138") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28255,
                                           "defunc_0_f_res_mem_28255") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28250,
                                           "defunc_0_f_res_mem_28250") != 0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &unionArrays_res_f_res_mem_28451,
                                           "unionArrays_res_f_res_mem_28451") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &unionArrays_res_f_res_mem_28446,
                                           "unionArrays_res_f_res_mem_28446") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &unionArrays_res_f_res_mem_28441,
                                           "unionArrays_res_f_res_mem_28441") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &unionArrays_res_f_res_mem_28436,
                                           "unionArrays_res_f_res_mem_28436") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28113, "mem_28113") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28111, "mem_28111") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28109, "mem_28109") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28107, "mem_28107") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                           "unionArrays_res_mem_28471") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                           "unionArrays_res_mem_28466") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                           "unionArrays_res_mem_28461") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                           "unionArrays_res_mem_28456") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    bool index_concat_cmp_25692 = sle64(j_m_i_25541, x_25687);
                    int64_t index_concat_branch_25693;
                    
                    if (index_concat_cmp_25692) {
                        int64_t index_concat_i_25694 = sub64(x_25687,
                                                             j_m_i_25541);
                        int64_t slice_25695 = i_25517 + index_concat_i_25694;
                        int64_t index_concat_25696 =
                                ((int64_t *) rightColumns_mem_27968.mem)[slice_25695];
                        
                        index_concat_branch_25693 = index_concat_25696;
                    } else {
                        int64_t slice_25697 = i_25539 + x_25687;
                        int64_t index_concat_25698 =
                                ((int64_t *) leftColumns_mem_27965.mem)[slice_25697];
                        
                        index_concat_branch_25693 = index_concat_25698;
                    }
                    
                    int64_t index_concat_branch_25700 =
                            btoi_bool_i64(index_concat_cmp_25692);
                    int64_t index_concat_branch_25702;
                    
                    if (index_concat_cmp_25692) {
                        int64_t index_concat_i_25703 = sub64(x_25687,
                                                             j_m_i_25541);
                        
                        index_concat_branch_25702 = index_concat_i_25703;
                    } else {
                        index_concat_branch_25702 = x_25687;
                    }
                    ((int64_t *) mem_28115)[i_27673] =
                        index_concat_branch_25693;
                    ((int64_t *) mem_28117)[i_27673] =
                        index_concat_branch_25700;
                    ((int64_t *) mem_28119)[i_27673] =
                        index_concat_branch_25702;
                    ((int64_t *) mem_28121)[i_27673] = (int64_t) 0;
                }
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28255,
                                   "defunc_0_f_res_mem_28255") != 0)
                    return 1;
                
                int64_t discard_27698;
                int64_t scanacc_27684 = (int64_t) 0;
                
                for (int64_t i_27691 = 0; i_27691 < conc_tmp_25559; i_27691++) {
                    int64_t x_25766 = ((int64_t *) mem_28115)[i_27691];
                    int64_t x_25767 = ((int64_t *) mem_28117)[i_27691];
                    int64_t x_25768 = ((int64_t *) mem_28119)[i_27691];
                    int64_t x_25769 = ((int64_t *) mem_28121)[i_27691];
                    bool cond_25771 = i_27691 == i_25575;
                    bool cond_25772 = !cond_25771;
                    bool cond_25773;
                    
                    if (cond_25772) {
                        int64_t i_25774 = add64((int64_t) 1, i_27691);
                        bool x_25775 = sle64((int64_t) 0, i_25774);
                        bool y_25776 = slt64(i_25774, conc_tmp_25559);
                        bool bounds_check_25777 = x_25775 && y_25776;
                        bool index_certs_25778;
                        
                        if (!bounds_check_25777) {
                            ctx->error =
                                msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                          "Index [", i_25774,
                                          "] out of bounds for array of shape [",
                                          conc_tmp_25559, "].",
                                          "-> #0  sparse.fut:60:63-73\n   #1  sparse.fut:58:33-68:55\n   #2  sparse.fut:138:59-140:81\n   #3  sparse.fut:220:17-59\n   #4  sparse.fut:211:1-221:48\n");
                            if (memblock_unref(ctx, &mem_param_28143,
                                               "mem_param_28143") != 0)
                                return 1;
                            if (memblock_unref(ctx, &mem_param_28138,
                                               "mem_param_28138") != 0)
                                return 1;
                            if (memblock_unref(ctx, &defunc_0_f_res_mem_28255,
                                               "defunc_0_f_res_mem_28255") != 0)
                                return 1;
                            if (memblock_unref(ctx, &defunc_0_f_res_mem_28250,
                                               "defunc_0_f_res_mem_28250") != 0)
                                return 1;
                            if (memblock_unref(ctx,
                                               &unionArrays_res_f_res_mem_28451,
                                               "unionArrays_res_f_res_mem_28451") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx,
                                               &unionArrays_res_f_res_mem_28446,
                                               "unionArrays_res_f_res_mem_28446") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx,
                                               &unionArrays_res_f_res_mem_28441,
                                               "unionArrays_res_f_res_mem_28441") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx,
                                               &unionArrays_res_f_res_mem_28436,
                                               "unionArrays_res_f_res_mem_28436") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28113, "mem_28113") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28111, "mem_28111") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28109, "mem_28109") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28107, "mem_28107") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                               "unionArrays_res_mem_28471") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                               "unionArrays_res_mem_28466") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                               "unionArrays_res_mem_28461") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                               "unionArrays_res_mem_28456") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28105, "mem_28105") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28103, "mem_28103") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28101, "mem_28101") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &out_mem_29011,
                                               "out_mem_29011") != 0)
                                return 1;
                            if (memblock_unref(ctx, &out_mem_29009,
                                               "out_mem_29009") != 0)
                                return 1;
                            if (memblock_unref(ctx, &out_mem_29007,
                                               "out_mem_29007") != 0)
                                return 1;
                            return 1;
                        }
                        
                        int64_t y_25779 = ((int64_t *) mem_28115)[i_25774];
                        bool cond_t_res_25780 = x_25766 == y_25779;
                        
                        cond_25773 = cond_t_res_25780;
                    } else {
                        cond_25773 = 0;
                    }
                    
                    int64_t defunc_0_f_res_25781;
                    
                    if (cond_25773) {
                        defunc_0_f_res_25781 = (int64_t) -1;
                    } else {
                        defunc_0_f_res_25781 = x_25766;
                    }
                    
                    int64_t defunc_0_f_res_25782;
                    int64_t defunc_0_f_res_25783;
                    int64_t defunc_0_f_res_25784;
                    
                    if (cond_25773) {
                        defunc_0_f_res_25782 = (int64_t) 0;
                        defunc_0_f_res_25783 = (int64_t) 0;
                        defunc_0_f_res_25784 = (int64_t) 0;
                    } else {
                        bool cond_25785 = i_27691 == (int64_t) 0;
                        bool cond_25786 = !cond_25785;
                        bool cond_25787;
                        
                        if (cond_25786) {
                            int64_t i_25788 = sub64(i_27691, (int64_t) 1);
                            bool x_25789 = sle64((int64_t) 0, i_25788);
                            bool y_25790 = slt64(i_25788, conc_tmp_25559);
                            bool bounds_check_25791 = x_25789 && y_25790;
                            bool index_certs_25792;
                            
                            if (!bounds_check_25791) {
                                ctx->error =
                                    msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                              "Index [", i_25788,
                                              "] out of bounds for array of shape [",
                                              conc_tmp_25559, "].",
                                              "-> #0  sparse.fut:63:59-69\n   #1  sparse.fut:58:33-68:55\n   #2  sparse.fut:138:59-140:81\n   #3  sparse.fut:220:17-59\n   #4  sparse.fut:211:1-221:48\n");
                                if (memblock_unref(ctx, &mem_param_28143,
                                                   "mem_param_28143") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_param_28138,
                                                   "mem_param_28138") != 0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &defunc_0_f_res_mem_28255,
                                                   "defunc_0_f_res_mem_28255") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &defunc_0_f_res_mem_28250,
                                                   "defunc_0_f_res_mem_28250") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_f_res_mem_28451,
                                                   "unionArrays_res_f_res_mem_28451") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_f_res_mem_28446,
                                                   "unionArrays_res_f_res_mem_28446") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_f_res_mem_28441,
                                                   "unionArrays_res_f_res_mem_28441") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_f_res_mem_28436,
                                                   "unionArrays_res_f_res_mem_28436") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28113,
                                                   "mem_28113") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28111,
                                                   "mem_28111") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28109,
                                                   "mem_28109") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28107,
                                                   "mem_28107") != 0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_mem_28471,
                                                   "unionArrays_res_mem_28471") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_mem_28466,
                                                   "unionArrays_res_mem_28466") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_mem_28461,
                                                   "unionArrays_res_mem_28461") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_mem_28456,
                                                   "unionArrays_res_mem_28456") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28105,
                                                   "mem_28105") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28103,
                                                   "mem_28103") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28101,
                                                   "mem_28101") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &out_mem_29011,
                                                   "out_mem_29011") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &out_mem_29009,
                                                   "out_mem_29009") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &out_mem_29007,
                                                   "out_mem_29007") != 0)
                                    return 1;
                                return 1;
                            }
                            
                            int64_t y_25793 = ((int64_t *) mem_28115)[i_25788];
                            bool cond_t_res_25794 = x_25766 == y_25793;
                            
                            cond_25787 = cond_t_res_25794;
                        } else {
                            cond_25787 = 0;
                        }
                        
                        int64_t defunc_0_f_res_f_res_25795;
                        
                        if (cond_25787) {
                            defunc_0_f_res_f_res_25795 = (int64_t) 2;
                        } else {
                            defunc_0_f_res_f_res_25795 = x_25767;
                        }
                        
                        int64_t defunc_0_f_res_f_res_25796;
                        int64_t defunc_0_f_res_f_res_25797;
                        
                        if (cond_25787) {
                            bool cond_25798 = x_25767 == (int64_t) 0;
                            int64_t defunc_0_f_res_f_res_t_res_25799;
                            int64_t defunc_0_f_res_f_res_t_res_25800;
                            
                            if (cond_25798) {
                                int64_t i_25801 = sub64(i_27691, (int64_t) 1);
                                bool x_25802 = sle64((int64_t) 0, i_25801);
                                bool y_25803 = slt64(i_25801, conc_tmp_25559);
                                bool bounds_check_25804 = x_25802 && y_25803;
                                bool index_certs_25805;
                                
                                if (!bounds_check_25804) {
                                    ctx->error =
                                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                                  "Index [", i_25801,
                                                  "] out of bounds for array of shape [",
                                                  conc_tmp_25559, "].",
                                                  "-> #0  sparse.fut:65:67-77\n   #1  sparse.fut:58:33-68:55\n   #2  sparse.fut:138:59-140:81\n   #3  sparse.fut:220:17-59\n   #4  sparse.fut:211:1-221:48\n");
                                    if (memblock_unref(ctx, &mem_param_28143,
                                                       "mem_param_28143") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_param_28138,
                                                       "mem_param_28138") != 0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &defunc_0_f_res_mem_28255,
                                                       "defunc_0_f_res_mem_28255") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &defunc_0_f_res_mem_28250,
                                                       "defunc_0_f_res_mem_28250") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28451,
                                                       "unionArrays_res_f_res_mem_28451") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28446,
                                                       "unionArrays_res_f_res_mem_28446") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28441,
                                                       "unionArrays_res_f_res_mem_28441") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28436,
                                                       "unionArrays_res_f_res_mem_28436") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28113,
                                                       "mem_28113") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28111,
                                                       "mem_28111") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28109,
                                                       "mem_28109") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28107,
                                                       "mem_28107") != 0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28471,
                                                       "unionArrays_res_mem_28471") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28466,
                                                       "unionArrays_res_mem_28466") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28461,
                                                       "unionArrays_res_mem_28461") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28456,
                                                       "unionArrays_res_mem_28456") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28105,
                                                       "mem_28105") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28103,
                                                       "mem_28103") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28101,
                                                       "mem_28101") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29011,
                                                       "out_mem_29011") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29009,
                                                       "out_mem_29009") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29007,
                                                       "out_mem_29007") != 0)
                                        return 1;
                                    return 1;
                                }
                                
                                int64_t defunc_0_f_res_f_res_t_res_t_res_25806 =
                                        ((int64_t *) mem_28119)[i_25801];
                                
                                defunc_0_f_res_f_res_t_res_25799 = x_25768;
                                defunc_0_f_res_f_res_t_res_25800 =
                                    defunc_0_f_res_f_res_t_res_t_res_25806;
                            } else {
                                int64_t i_25807 = sub64(i_27691, (int64_t) 1);
                                bool x_25808 = sle64((int64_t) 0, i_25807);
                                bool y_25809 = slt64(i_25807, conc_tmp_25559);
                                bool bounds_check_25810 = x_25808 && y_25809;
                                bool index_certs_25811;
                                
                                if (!bounds_check_25810) {
                                    ctx->error =
                                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                                  "Index [", i_25807,
                                                  "] out of bounds for array of shape [",
                                                  conc_tmp_25559, "].",
                                                  "-> #0  sparse.fut:67:59-69\n   #1  sparse.fut:58:33-68:55\n   #2  sparse.fut:138:59-140:81\n   #3  sparse.fut:220:17-59\n   #4  sparse.fut:211:1-221:48\n");
                                    if (memblock_unref(ctx, &mem_param_28143,
                                                       "mem_param_28143") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_param_28138,
                                                       "mem_param_28138") != 0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &defunc_0_f_res_mem_28255,
                                                       "defunc_0_f_res_mem_28255") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &defunc_0_f_res_mem_28250,
                                                       "defunc_0_f_res_mem_28250") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28451,
                                                       "unionArrays_res_f_res_mem_28451") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28446,
                                                       "unionArrays_res_f_res_mem_28446") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28441,
                                                       "unionArrays_res_f_res_mem_28441") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28436,
                                                       "unionArrays_res_f_res_mem_28436") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28113,
                                                       "mem_28113") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28111,
                                                       "mem_28111") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28109,
                                                       "mem_28109") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28107,
                                                       "mem_28107") != 0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28471,
                                                       "unionArrays_res_mem_28471") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28466,
                                                       "unionArrays_res_mem_28466") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28461,
                                                       "unionArrays_res_mem_28461") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28456,
                                                       "unionArrays_res_mem_28456") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28105,
                                                       "mem_28105") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28103,
                                                       "mem_28103") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28101,
                                                       "mem_28101") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29011,
                                                       "out_mem_29011") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29009,
                                                       "out_mem_29009") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29007,
                                                       "out_mem_29007") != 0)
                                        return 1;
                                    return 1;
                                }
                                
                                int64_t defunc_0_f_res_f_res_t_res_f_res_25812 =
                                        ((int64_t *) mem_28119)[i_25807];
                                
                                defunc_0_f_res_f_res_t_res_25799 =
                                    defunc_0_f_res_f_res_t_res_f_res_25812;
                                defunc_0_f_res_f_res_t_res_25800 = x_25768;
                            }
                            defunc_0_f_res_f_res_25796 =
                                defunc_0_f_res_f_res_t_res_25799;
                            defunc_0_f_res_f_res_25797 =
                                defunc_0_f_res_f_res_t_res_25800;
                        } else {
                            defunc_0_f_res_f_res_25796 = x_25768;
                            defunc_0_f_res_f_res_25797 = x_25769;
                        }
                        defunc_0_f_res_25782 = defunc_0_f_res_f_res_25795;
                        defunc_0_f_res_25783 = defunc_0_f_res_f_res_25796;
                        defunc_0_f_res_25784 = defunc_0_f_res_f_res_25797;
                    }
                    
                    bool eq_x_zz_25813 = (int64_t) -1 == x_25766;
                    bool not_p_25814 = !cond_25773;
                    bool p_and_eq_x_y_25815 = eq_x_zz_25813 && not_p_25814;
                    bool defunc_0_p_res_25816 = cond_25773 ||
                         p_and_eq_x_y_25815;
                    int64_t defunc_1_lifted_lambda_res_25817 =
                            btoi_bool_i64(defunc_0_p_res_25816);
                    bool is_i_25818 = defunc_1_lifted_lambda_res_25817 ==
                         (int64_t) 0;
                    bool cond_neg_25819 = !is_i_25818;
                    int64_t part_res_25820 = btoi_bool_i64(cond_neg_25819);
                    int64_t part_res_25821 = btoi_bool_i64(is_i_25818);
                    int64_t zz_25765 = part_res_25821 + scanacc_27684;
                    
                    ((int64_t *) mem_28123)[i_27691] = zz_25765;
                    ((int64_t *) mem_28125)[i_27691] = part_res_25820;
                    ((int64_t *) mem_28127)[i_27691] = defunc_0_f_res_25781;
                    ((int64_t *) mem_28129)[i_27691] = defunc_0_f_res_25782;
                    ((int64_t *) mem_28131)[i_27691] = defunc_0_f_res_25783;
                    ((int64_t *) mem_28133)[i_27691] = defunc_0_f_res_25784;
                    
                    int64_t scanacc_tmp_29061 = zz_25765;
                    
                    scanacc_27684 = scanacc_tmp_29061;
                }
                discard_27698 = scanacc_27684;
                
                int64_t partition_sizze_25828;
                
                if (cond_25572) {
                    partition_sizze_25828 = (int64_t) 0;
                } else {
                    int64_t last_offset_25829 =
                            ((int64_t *) mem_28123)[i_25575];
                    
                    partition_sizze_25828 = last_offset_25829;
                }
                
                struct memblock mem_28377;
                
                mem_28377.references = NULL;
                if (memblock_alloc(ctx, &mem_28377, bytes_28110, "mem_28377")) {
                    err = 1;
                    goto cleanup;
                }
                
                struct memblock mem_28379;
                
                mem_28379.references = NULL;
                if (memblock_alloc(ctx, &mem_28379, bytes_28110, "mem_28379")) {
                    err = 1;
                    goto cleanup;
                }
                
                struct memblock mem_28381;
                
                mem_28381.references = NULL;
                if (memblock_alloc(ctx, &mem_28381, bytes_28110, "mem_28381")) {
                    err = 1;
                    goto cleanup;
                }
                
                struct memblock mem_28383;
                
                mem_28383.references = NULL;
                if (memblock_alloc(ctx, &mem_28383, bytes_28110, "mem_28383")) {
                    err = 1;
                    goto cleanup;
                }
                for (int64_t write_iter_27699 = 0; write_iter_27699 <
                     conc_tmp_25559; write_iter_27699++) {
                    int64_t write_iv_27704 =
                            ((int64_t *) mem_28125)[write_iter_27699];
                    int64_t write_iv_27705 =
                            ((int64_t *) mem_28123)[write_iter_27699];
                    bool is_this_one_25844 = write_iv_27704 == (int64_t) 0;
                    int64_t this_offset_25845 = (int64_t) -1 + write_iv_27705;
                    int64_t total_res_25846;
                    
                    if (is_this_one_25844) {
                        total_res_25846 = this_offset_25845;
                    } else {
                        total_res_25846 = (int64_t) -1;
                    }
                    
                    bool less_than_zzero_27710 = slt64(total_res_25846,
                                                       (int64_t) 0);
                    bool greater_than_sizze_27711 = sle64(conc_tmp_25559,
                                                          total_res_25846);
                    bool outside_bounds_dim_27712 = less_than_zzero_27710 ||
                         greater_than_sizze_27711;
                    
                    if (!outside_bounds_dim_27712) {
                        memmove(mem_28377.mem + total_res_25846 * (int64_t) 8,
                                mem_28127 + write_iter_27699 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                    if (!outside_bounds_dim_27712) {
                        memmove(mem_28379.mem + total_res_25846 * (int64_t) 8,
                                mem_28129 + write_iter_27699 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                    if (!outside_bounds_dim_27712) {
                        memmove(mem_28381.mem + total_res_25846 * (int64_t) 8,
                                mem_28131 + write_iter_27699 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                    if (!outside_bounds_dim_27712) {
                        memmove(mem_28383.mem + total_res_25846 * (int64_t) 8,
                                mem_28133 + write_iter_27699 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                }
                
                bool empty_slice_25847 = partition_sizze_25828 == (int64_t) 0;
                int64_t m_25848 = sub64(partition_sizze_25828, (int64_t) 1);
                bool zzero_leq_i_p_m_t_s_25849 = sle64((int64_t) 0, m_25848);
                bool i_p_m_t_s_leq_w_25850 = slt64(m_25848, conc_tmp_25559);
                bool i_lte_j_25851 = sle64((int64_t) 0, partition_sizze_25828);
                bool y_25852 = zzero_leq_i_p_m_t_s_25849 &&
                     i_p_m_t_s_leq_w_25850;
                bool y_25853 = i_lte_j_25851 && y_25852;
                bool ok_or_empty_25854 = empty_slice_25847 || y_25853;
                bool index_certs_25855;
                
                if (!ok_or_empty_25854) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [:", partition_sizze_25828,
                                  "] out of bounds for array of shape [",
                                  conc_tmp_25559, "].",
                                  "-> #0  /prelude/soacs.fut:141:6-16\n   #1  sparse.fut:58:3-68:56\n   #2  sparse.fut:138:59-140:81\n   #3  sparse.fut:220:17-59\n   #4  sparse.fut:211:1-221:48\n");
                    if (memblock_unref(ctx, &mem_28383, "mem_28383") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28381, "mem_28381") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28379, "mem_28379") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28377, "mem_28377") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_28143,
                                       "mem_param_28143") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_28138,
                                       "mem_param_28138") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28255,
                                       "defunc_0_f_res_mem_28255") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28250,
                                       "defunc_0_f_res_mem_28250") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28451,
                                       "unionArrays_res_f_res_mem_28451") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28446,
                                       "unionArrays_res_f_res_mem_28446") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28441,
                                       "unionArrays_res_f_res_mem_28441") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28436,
                                       "unionArrays_res_f_res_mem_28436") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28113, "mem_28113") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28111, "mem_28111") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28109, "mem_28109") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28107, "mem_28107") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                       "unionArrays_res_mem_28471") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                       "unionArrays_res_mem_28466") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                       "unionArrays_res_mem_28461") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                       "unionArrays_res_mem_28456") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                sizze_25581 = partition_sizze_25828;
                sizze_25582 = partition_sizze_25828;
                sizze_25583 = partition_sizze_25828;
                sizze_25584 = partition_sizze_25828;
                unionArrays_res_f_res_ixfn_28432 = partition_sizze_25828;
                unionArrays_res_f_res_ixfn_28437 = partition_sizze_25828;
                unionArrays_res_f_res_ixfn_28442 = partition_sizze_25828;
                unionArrays_res_f_res_ixfn_28447 = partition_sizze_25828;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28436,
                                 &mem_28377, "mem_28377") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28441,
                                 &mem_28379, "mem_28379") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28446,
                                 &mem_28381, "mem_28381") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28451,
                                 &mem_28383, "mem_28383") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28383, "mem_28383") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28381, "mem_28381") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28379, "mem_28379") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28377, "mem_28377") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_param_28143, "mem_param_28143") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &mem_param_28138, "mem_param_28138") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28255,
                                   "defunc_0_f_res_mem_28255") != 0)
                    return 1;
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28250,
                                   "defunc_0_f_res_mem_28250") != 0)
                    return 1;
            }
            if (memblock_unref(ctx, &mem_28107, "mem_28107") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28109, "mem_28109") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28111, "mem_28111") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28113, "mem_28113") != 0)
                return 1;
            sizze_25560 = sizze_25581;
            sizze_25561 = sizze_25582;
            sizze_25562 = sizze_25583;
            sizze_25563 = sizze_25584;
            unionArrays_res_ixfn_28452 = unionArrays_res_f_res_ixfn_28432;
            unionArrays_res_ixfn_28453 = unionArrays_res_f_res_ixfn_28433;
            unionArrays_res_ixfn_28455 = unionArrays_res_f_res_ixfn_28435;
            unionArrays_res_ixfn_28457 = unionArrays_res_f_res_ixfn_28437;
            unionArrays_res_ixfn_28458 = unionArrays_res_f_res_ixfn_28438;
            unionArrays_res_ixfn_28462 = unionArrays_res_f_res_ixfn_28442;
            unionArrays_res_ixfn_28463 = unionArrays_res_f_res_ixfn_28443;
            unionArrays_res_ixfn_28467 = unionArrays_res_f_res_ixfn_28447;
            unionArrays_res_ixfn_28468 = unionArrays_res_f_res_ixfn_28448;
            if (memblock_set(ctx, &unionArrays_res_mem_28456,
                             &unionArrays_res_f_res_mem_28436,
                             "unionArrays_res_f_res_mem_28436") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28461,
                             &unionArrays_res_f_res_mem_28441,
                             "unionArrays_res_f_res_mem_28441") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28466,
                             &unionArrays_res_f_res_mem_28446,
                             "unionArrays_res_f_res_mem_28446") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28471,
                             &unionArrays_res_f_res_mem_28451,
                             "unionArrays_res_f_res_mem_28451") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28451,
                               "unionArrays_res_f_res_mem_28451") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28446,
                               "unionArrays_res_f_res_mem_28446") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28441,
                               "unionArrays_res_f_res_mem_28441") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28436,
                               "unionArrays_res_f_res_mem_28436") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28113, "mem_28113") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28111, "mem_28111") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28109, "mem_28109") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28107, "mem_28107") != 0)
                return 1;
        }
        if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
            return 1;
        
        bool dim_match_25860 = sizze_25560 == sizze_25561;
        bool empty_or_match_cert_25861;
        
        if (!dim_match_25860) {
            ctx->error = msgprintf("Error: %s\n\nBacktrace:\n%s",
                                   "Function return value does not match shape of declared return type.",
                                   "-> #0  sparse.fut:45:7-68:56\n   #1  sparse.fut:138:59-140:81\n   #2  sparse.fut:220:17-59\n   #3  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool dim_match_25862 = sizze_25560 == sizze_25562;
        bool empty_or_match_cert_25863;
        
        if (!dim_match_25862) {
            ctx->error = msgprintf("Error: %s\n\nBacktrace:\n%s",
                                   "Function return value does not match shape of declared return type.",
                                   "-> #0  sparse.fut:45:7-68:56\n   #1  sparse.fut:138:59-140:81\n   #2  sparse.fut:220:17-59\n   #3  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool dim_match_25864 = sizze_25560 == sizze_25563;
        bool empty_or_match_cert_25865;
        
        if (!dim_match_25864) {
            ctx->error = msgprintf("Error: %s\n\nBacktrace:\n%s",
                                   "Function return value does not match shape of declared return type.",
                                   "-> #0  sparse.fut:45:7-68:56\n   #1  sparse.fut:138:59-140:81\n   #2  sparse.fut:220:17-59\n   #3  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t x_25866 = ((int64_t *) mem_27971)[i_25518];
        int64_t y_25867 = ((int64_t *) mem_27971)[i_25512];
        int64_t indxs_25868 = sub64(x_25866, y_25867);
        bool bounds_invalid_upwards_25869 = slt64(x_25866, y_25867);
        bool valid_25870 = !bounds_invalid_upwards_25869;
        bool range_valid_c_25871;
        
        if (!valid_25870) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Range ", y_25867, "..<", x_25866,
                                   " is invalid.",
                                   "-> #0  sparse.fut:142:18-42\n   #1  sparse.fut:220:17-59\n   #2  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool dim_match_25873 = indxs_25868 == sizze_25560;
        bool empty_or_match_cert_25874;
        
        if (!dim_match_25873) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Value of (core language) shape (",
                                   sizze_25560,
                                   ") cannot match shape of type `[",
                                   indxs_25868, "]index`.",
                                   "-> #0  sparse.fut:146:26-51\n   #1  sparse.fut:220:17-59\n   #2  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool empty_or_match_cert_25875;
        
        if (!dim_match_25873) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Value of (core language) shape (",
                                   sizze_25560,
                                   ") cannot match shape of type `[",
                                   indxs_25868, "]t`.",
                                   "-> #0  sparse.fut:147:25-154:75\n   #1  sparse.fut:220:17-59\n   #2  sparse.fut:211:1-221:48\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        for (int64_t write_iter_27734 = 0; write_iter_27734 < indxs_25868;
             write_iter_27734++) {
            int64_t index_primexp_27940 = add64(y_25867, write_iter_27734);
            bool y_25885 = slt64(write_iter_27734, sizze_25560);
            bool index_certs_25887;
            
            if (!y_25885) {
                ctx->error =
                    msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                              "Index [", write_iter_27734,
                              "] out of bounds for array of shape [",
                              sizze_25560, "].",
                              "-> #0  sparse.fut:148:37-47\n   #1  sparse.fut:147:26-154:63\n   #2  sparse.fut:220:17-59\n   #3  sparse.fut:211:1-221:48\n");
                if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                   "unionArrays_res_mem_28471") != 0)
                    return 1;
                if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                   "unionArrays_res_mem_28466") != 0)
                    return 1;
                if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                   "unionArrays_res_mem_28461") != 0)
                    return 1;
                if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                   "unionArrays_res_mem_28456") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                    return 1;
                return 1;
            }
            
            int64_t x_25888 =
                    ((int64_t *) unionArrays_res_mem_28461.mem)[write_iter_27734];
            bool cond_25889 = x_25888 == (int64_t) 2;
            double defunc_0_f_res_25890;
            
            if (cond_25889) {
                int64_t y_25891 =
                        ((int64_t *) unionArrays_res_mem_28471.mem)[write_iter_27734];
                int64_t i_25892 = add64(i_25517, y_25891);
                bool x_25893 = sle64((int64_t) 0, i_25892);
                bool y_25894 = slt64(i_25892, k_25313);
                bool bounds_check_25895 = x_25893 && y_25894;
                bool index_certs_25896;
                
                if (!bounds_check_25895) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", i_25892,
                                  "] out of bounds for array of shape [",
                                  k_25313, "].",
                                  "-> #0  sparse.fut:150:39-85\n   #1  sparse.fut:147:26-154:63\n   #2  sparse.fut:220:17-59\n   #3  sparse.fut:211:1-221:48\n");
                    if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                       "unionArrays_res_mem_28471") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                       "unionArrays_res_mem_28466") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                       "unionArrays_res_mem_28461") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                       "unionArrays_res_mem_28456") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                
                double defunc_1_add_arg_25897 =
                       ((double *) rightValues_mem_27967.mem)[i_25892];
                int64_t y_25898 =
                        ((int64_t *) unionArrays_res_mem_28466.mem)[write_iter_27734];
                int64_t i_25899 = add64(i_25539, y_25898);
                bool x_25900 = sle64((int64_t) 0, i_25899);
                bool y_25901 = slt64(i_25899, n_25311);
                bool bounds_check_25902 = x_25900 && y_25901;
                bool index_certs_25903;
                
                if (!bounds_check_25902) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", i_25899,
                                  "] out of bounds for array of shape [",
                                  n_25311, "].",
                                  "-> #0  sparse.fut:149:46-90\n   #1  sparse.fut:147:26-154:63\n   #2  sparse.fut:220:17-59\n   #3  sparse.fut:211:1-221:48\n");
                    if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                       "unionArrays_res_mem_28471") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                       "unionArrays_res_mem_28466") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                       "unionArrays_res_mem_28461") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                       "unionArrays_res_mem_28456") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                
                double defunc_0_add_arg_25904 =
                       ((double *) leftValues_mem_27964.mem)[i_25899];
                double defunc_1_add_res_25905 = defunc_1_add_arg_25897 +
                       defunc_0_add_arg_25904;
                
                defunc_0_f_res_25890 = defunc_1_add_res_25905;
            } else {
                bool cond_25906 = x_25888 == (int64_t) 0;
                double defunc_0_f_res_f_res_25907;
                
                if (cond_25906) {
                    int64_t y_25908 =
                            ((int64_t *) unionArrays_res_mem_28466.mem)[write_iter_27734];
                    int64_t i_25909 = add64(i_25539, y_25908);
                    bool x_25910 = sle64((int64_t) 0, i_25909);
                    bool y_25911 = slt64(i_25909, n_25311);
                    bool bounds_check_25912 = x_25910 && y_25911;
                    bool index_certs_25913;
                    
                    if (!bounds_check_25912) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", i_25909,
                                      "] out of bounds for array of shape [",
                                      n_25311, "].",
                                      "-> #0  sparse.fut:152:39-83\n   #1  sparse.fut:147:26-154:63\n   #2  sparse.fut:220:17-59\n   #3  sparse.fut:211:1-221:48\n");
                        if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                           "unionArrays_res_mem_28471") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                           "unionArrays_res_mem_28466") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                           "unionArrays_res_mem_28461") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                           "unionArrays_res_mem_28456") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    double defunc_0_f_res_f_res_t_res_25914 =
                           ((double *) leftValues_mem_27964.mem)[i_25909];
                    
                    defunc_0_f_res_f_res_25907 =
                        defunc_0_f_res_f_res_t_res_25914;
                } else {
                    int64_t y_25915 =
                            ((int64_t *) unionArrays_res_mem_28466.mem)[write_iter_27734];
                    int64_t i_25916 = add64(i_25517, y_25915);
                    bool x_25917 = sle64((int64_t) 0, i_25916);
                    bool y_25918 = slt64(i_25916, k_25313);
                    bool bounds_check_25919 = x_25917 && y_25918;
                    bool index_certs_25920;
                    
                    if (!bounds_check_25919) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", i_25916,
                                      "] out of bounds for array of shape [",
                                      k_25313, "].",
                                      "-> #0  sparse.fut:153:39-85\n   #1  sparse.fut:147:26-154:63\n   #2  sparse.fut:220:17-59\n   #3  sparse.fut:211:1-221:48\n");
                        if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                           "unionArrays_res_mem_28471") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                           "unionArrays_res_mem_28466") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                           "unionArrays_res_mem_28461") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                           "unionArrays_res_mem_28456") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    double defunc_0_f_res_f_res_f_res_25921 =
                           ((double *) rightValues_mem_27967.mem)[i_25916];
                    
                    defunc_0_f_res_f_res_25907 =
                        defunc_0_f_res_f_res_f_res_25921;
                }
                defunc_0_f_res_25890 = defunc_0_f_res_f_res_25907;
            }
            
            bool less_than_zzero_27741 = slt64(index_primexp_27940,
                                               (int64_t) 0);
            bool greater_than_sizze_27742 = sle64(eWiseAddOffsets_res_25504,
                                                  index_primexp_27940);
            bool outside_bounds_dim_27743 = less_than_zzero_27741 ||
                 greater_than_sizze_27742;
            
            if (!outside_bounds_dim_27743) {
                ((double *) mem_28083)[index_primexp_27940] =
                    defunc_0_f_res_25890;
            }
            if (!outside_bounds_dim_27743) {
                memmove(mem_28085 + index_primexp_27940 * (int64_t) 8,
                        unionArrays_res_mem_28456.mem +
                        (unionArrays_res_ixfn_28455 + write_iter_27734) *
                        (int64_t) 8, (int64_t) sizeof(int64_t));
            }
        }
        if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                           "unionArrays_res_mem_28456") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                           "unionArrays_res_mem_28461") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                           "unionArrays_res_mem_28466") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                           "unionArrays_res_mem_28471") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                           "unionArrays_res_mem_28471") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                           "unionArrays_res_mem_28466") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                           "unionArrays_res_mem_28461") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                           "unionArrays_res_mem_28456") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
            return 1;
    }
    
    struct memblock mem_28503;
    
    mem_28503.references = NULL;
    if (memblock_alloc(ctx, &mem_28503, bytes_28082, "mem_28503")) {
        err = 1;
        goto cleanup;
    }
    memmove(mem_28503.mem + (int64_t) 0, mem_28083 + (int64_t) 0,
            eWiseAddOffsets_res_25504 * (int64_t) sizeof(double));
    
    struct memblock mem_28506;
    
    mem_28506.references = NULL;
    if (memblock_alloc(ctx, &mem_28506, bytes_28082, "mem_28506")) {
        err = 1;
        goto cleanup;
    }
    memmove(mem_28506.mem + (int64_t) 0, mem_28085 + (int64_t) 0,
            eWiseAddOffsets_res_25504 * (int64_t) sizeof(int64_t));
    
    struct memblock mem_28509;
    
    mem_28509.references = NULL;
    if (memblock_alloc(ctx, &mem_28509, bytes_27970, "mem_28509")) {
        err = 1;
        goto cleanup;
    }
    memmove(mem_28509.mem + (int64_t) 0, mem_27971 + (int64_t) 0, m_25312 *
            (int64_t) sizeof(int64_t));
    out_arrsizze_29008 = eWiseAddOffsets_res_25504;
    out_arrsizze_29010 = eWiseAddOffsets_res_25504;
    if (memblock_set(ctx, &out_mem_29007, &mem_28503, "mem_28503") != 0)
        return 1;
    if (memblock_set(ctx, &out_mem_29009, &mem_28506, "mem_28506") != 0)
        return 1;
    if (memblock_set(ctx, &out_mem_29011, &mem_28509, "mem_28509") != 0)
        return 1;
    (*out_mem_p_29126).references = NULL;
    if (memblock_set(ctx, &*out_mem_p_29126, &out_mem_29007, "out_mem_29007") !=
        0)
        return 1;
    *out_out_arrsizze_29127 = out_arrsizze_29008;
    (*out_mem_p_29128).references = NULL;
    if (memblock_set(ctx, &*out_mem_p_29128, &out_mem_29009, "out_mem_29009") !=
        0)
        return 1;
    *out_out_arrsizze_29129 = out_arrsizze_29010;
    (*out_mem_p_29130).references = NULL;
    if (memblock_set(ctx, &*out_mem_p_29130, &out_mem_29011, "out_mem_29011") !=
        0)
        return 1;
    if (memblock_unref(ctx, &mem_28509, "mem_28509") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_28506, "mem_28506") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_28503, "mem_28503") != 0)
        return 1;
    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
        return 1;
    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
        return 1;
    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
        return 1;
    
  cleanup:
    { }
    free(mem_27971);
    free(mem_27989);
    free(mem_27991);
    free(mem_27993);
    free(mem_27995);
    free(mem_27997);
    free(mem_28083);
    free(mem_28085);
    free(mem_28115);
    free(mem_28117);
    free(mem_28119);
    free(mem_28121);
    free(mem_28123);
    free(mem_28125);
    free(mem_28127);
    free(mem_28129);
    free(mem_28131);
    free(mem_28133);
    free(mem_28149);
    free(mem_28151);
    free(mem_28153);
    free(mem_28155);
    free(mem_28157);
    return err;
}
static int futrts_eWiseAddMultEntry(struct futhark_context *ctx,
                                    struct memblock *out_mem_p_29154,
                                    int64_t *out_out_arrsizze_29155,
                                    struct memblock *out_mem_p_29156,
                                    int64_t *out_out_arrsizze_29157,
                                    struct memblock *out_mem_p_29158,
                                    struct memblock leftValues_mem_27964,
                                    struct memblock leftColumns_mem_27965,
                                    struct memblock leftOffsets_mem_27966,
                                    struct memblock rightValues_mem_27967,
                                    struct memblock rightColumns_mem_27968,
                                    struct memblock rightOffsets_mem_27969,
                                    int64_t n_26449, int64_t m_26450,
                                    int64_t k_26451)
{
    (void) ctx;
    
    int err = 0;
    size_t mem_27971_cached_sizze_29159 = 0;
    char *mem_27971 = NULL;
    size_t mem_27989_cached_sizze_29160 = 0;
    char *mem_27989 = NULL;
    size_t mem_27991_cached_sizze_29161 = 0;
    char *mem_27991 = NULL;
    size_t mem_27993_cached_sizze_29162 = 0;
    char *mem_27993 = NULL;
    size_t mem_27995_cached_sizze_29163 = 0;
    char *mem_27995 = NULL;
    size_t mem_27997_cached_sizze_29164 = 0;
    char *mem_27997 = NULL;
    size_t mem_28083_cached_sizze_29165 = 0;
    char *mem_28083 = NULL;
    size_t mem_28085_cached_sizze_29166 = 0;
    char *mem_28085 = NULL;
    size_t mem_28115_cached_sizze_29167 = 0;
    char *mem_28115 = NULL;
    size_t mem_28117_cached_sizze_29168 = 0;
    char *mem_28117 = NULL;
    size_t mem_28119_cached_sizze_29169 = 0;
    char *mem_28119 = NULL;
    size_t mem_28121_cached_sizze_29170 = 0;
    char *mem_28121 = NULL;
    size_t mem_28123_cached_sizze_29171 = 0;
    char *mem_28123 = NULL;
    size_t mem_28125_cached_sizze_29172 = 0;
    char *mem_28125 = NULL;
    size_t mem_28127_cached_sizze_29173 = 0;
    char *mem_28127 = NULL;
    size_t mem_28129_cached_sizze_29174 = 0;
    char *mem_28129 = NULL;
    size_t mem_28131_cached_sizze_29175 = 0;
    char *mem_28131 = NULL;
    size_t mem_28133_cached_sizze_29176 = 0;
    char *mem_28133 = NULL;
    size_t mem_28149_cached_sizze_29177 = 0;
    char *mem_28149 = NULL;
    size_t mem_28151_cached_sizze_29178 = 0;
    char *mem_28151 = NULL;
    size_t mem_28153_cached_sizze_29179 = 0;
    char *mem_28153 = NULL;
    size_t mem_28155_cached_sizze_29180 = 0;
    char *mem_28155 = NULL;
    size_t mem_28157_cached_sizze_29181 = 0;
    char *mem_28157 = NULL;
    size_t mem_28503_cached_sizze_29182 = 0;
    char *mem_28503 = NULL;
    size_t mem_28521_cached_sizze_29183 = 0;
    char *mem_28521 = NULL;
    size_t mem_28523_cached_sizze_29184 = 0;
    char *mem_28523 = NULL;
    size_t mem_28525_cached_sizze_29185 = 0;
    char *mem_28525 = NULL;
    size_t mem_28527_cached_sizze_29186 = 0;
    char *mem_28527 = NULL;
    size_t mem_28529_cached_sizze_29187 = 0;
    char *mem_28529 = NULL;
    size_t mem_28615_cached_sizze_29188 = 0;
    char *mem_28615 = NULL;
    size_t mem_28617_cached_sizze_29189 = 0;
    char *mem_28617 = NULL;
    size_t mem_28639_cached_sizze_29190 = 0;
    char *mem_28639 = NULL;
    size_t mem_28641_cached_sizze_29191 = 0;
    char *mem_28641 = NULL;
    size_t mem_28643_cached_sizze_29192 = 0;
    char *mem_28643 = NULL;
    size_t mem_28659_cached_sizze_29193 = 0;
    char *mem_28659 = NULL;
    size_t mem_28661_cached_sizze_29194 = 0;
    char *mem_28661 = NULL;
    size_t mem_28663_cached_sizze_29195 = 0;
    char *mem_28663 = NULL;
    size_t mem_28665_cached_sizze_29196 = 0;
    char *mem_28665 = NULL;
    size_t mem_28667_cached_sizze_29197 = 0;
    char *mem_28667 = NULL;
    size_t mem_28803_cached_sizze_29198 = 0;
    char *mem_28803 = NULL;
    size_t mem_28805_cached_sizze_29199 = 0;
    char *mem_28805 = NULL;
    size_t mem_28807_cached_sizze_29200 = 0;
    char *mem_28807 = NULL;
    size_t mem_28809_cached_sizze_29201 = 0;
    char *mem_28809 = NULL;
    size_t mem_28811_cached_sizze_29202 = 0;
    char *mem_28811 = NULL;
    struct memblock out_mem_29007;
    
    out_mem_29007.references = NULL;
    
    int64_t out_arrsizze_29008;
    struct memblock out_mem_29009;
    
    out_mem_29009.references = NULL;
    
    int64_t out_arrsizze_29010;
    struct memblock out_mem_29011;
    
    out_mem_29011.references = NULL;
    
    int64_t bytes_27970 = (int64_t) 8 * m_26450;
    
    if (mem_27971_cached_sizze_29159 < (size_t) bytes_27970) {
        mem_27971 = realloc(mem_27971, bytes_27970);
        mem_27971_cached_sizze_29159 = bytes_27970;
    }
    
    int64_t discard_27618;
    int64_t scanacc_27614 = (int64_t) 0;
    
    for (int64_t i_27616 = 0; i_27616 < m_26450; i_27616++) {
        int64_t x_26463 = ((int64_t *) leftOffsets_mem_27966.mem)[i_27616];
        int64_t x_26465 = ((int64_t *) rightOffsets_mem_27969.mem)[i_27616];
        bool cond_26466 = i_27616 == (int64_t) 0;
        int64_t prevx2_26467;
        
        if (cond_26466) {
            prevx2_26467 = i_27616;
        } else {
            int64_t prevx2_f_res_26468 = sub64(i_27616, (int64_t) 1);
            
            prevx2_26467 = prevx2_f_res_26468;
        }
        
        int64_t prevy2_26469;
        
        if (cond_26466) {
            prevy2_26469 = i_27616;
        } else {
            int64_t prevy2_f_res_26470 = sub64(i_27616, (int64_t) 1);
            
            prevy2_26469 = prevy2_f_res_26470;
        }
        
        bool x_26471 = sle64((int64_t) 0, prevy2_26469);
        bool y_26472 = slt64(prevy2_26469, m_26450);
        bool bounds_check_26473 = x_26471 && y_26472;
        bool index_certs_26474;
        
        if (!bounds_check_26473) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", prevy2_26469,
                                   "] out of bounds for array of shape [",
                                   m_26450, "].",
                                   "-> #0  sparse.fut:112:58-78\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:109:19-114:50\n   #4  sparse.fut:131:23-48\n   #5  sparse.fut:244:20-62\n   #6  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_26475 =
                ((int64_t *) rightOffsets_mem_27969.mem)[prevy2_26469];
        int64_t j_m_i_26476 = sub64(x_26465, i_26475);
        bool empty_slice_26477 = j_m_i_26476 == (int64_t) 0;
        int64_t m_26478 = sub64(j_m_i_26476, (int64_t) 1);
        int64_t i_p_m_t_s_26479 = add64(i_26475, m_26478);
        bool zzero_leq_i_p_m_t_s_26480 = sle64((int64_t) 0, i_p_m_t_s_26479);
        bool i_p_m_t_s_leq_w_26481 = slt64(i_p_m_t_s_26479, k_26451);
        bool zzero_lte_i_26482 = sle64((int64_t) 0, i_26475);
        bool i_lte_j_26483 = sle64(i_26475, x_26465);
        bool y_26484 = i_p_m_t_s_leq_w_26481 && zzero_lte_i_26482;
        bool y_26485 = zzero_leq_i_p_m_t_s_26480 && y_26484;
        bool y_26486 = i_lte_j_26483 && y_26485;
        bool forwards_ok_26487 = zzero_lte_i_26482 && y_26486;
        bool ok_or_empty_26488 = empty_slice_26477 || forwards_ok_26487;
        bool index_certs_26489;
        
        if (!ok_or_empty_26488) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_26475, ":", x_26465,
                          "] out of bounds for array of shape [", k_26451, "].",
                          "-> #0  sparse.fut:112:44-82\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:109:19-114:50\n   #4  sparse.fut:131:23-48\n   #5  sparse.fut:244:20-62\n   #6  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool x_26491 = sle64((int64_t) 0, prevx2_26467);
        bool y_26492 = slt64(prevx2_26467, m_26450);
        bool bounds_check_26493 = x_26491 && y_26492;
        bool index_certs_26494;
        
        if (!bounds_check_26493) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", prevx2_26467,
                                   "] out of bounds for array of shape [",
                                   m_26450, "].",
                                   "-> #0  sparse.fut:111:53-72\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:109:19-114:50\n   #4  sparse.fut:131:23-48\n   #5  sparse.fut:244:20-62\n   #6  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_26495 = ((int64_t *) leftOffsets_mem_27966.mem)[prevx2_26467];
        int64_t j_m_i_26496 = sub64(x_26463, i_26495);
        bool empty_slice_26497 = j_m_i_26496 == (int64_t) 0;
        int64_t m_26498 = sub64(j_m_i_26496, (int64_t) 1);
        int64_t i_p_m_t_s_26499 = add64(i_26495, m_26498);
        bool zzero_leq_i_p_m_t_s_26500 = sle64((int64_t) 0, i_p_m_t_s_26499);
        bool i_p_m_t_s_leq_w_26501 = slt64(i_p_m_t_s_26499, n_26449);
        bool zzero_lte_i_26502 = sle64((int64_t) 0, i_26495);
        bool i_lte_j_26503 = sle64(i_26495, x_26463);
        bool y_26504 = i_p_m_t_s_leq_w_26501 && zzero_lte_i_26502;
        bool y_26505 = zzero_leq_i_p_m_t_s_26500 && y_26504;
        bool y_26506 = i_lte_j_26503 && y_26505;
        bool forwards_ok_26507 = zzero_lte_i_26502 && y_26506;
        bool ok_or_empty_26508 = empty_slice_26497 || forwards_ok_26507;
        bool index_certs_26509;
        
        if (!ok_or_empty_26508) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_26495, ":", x_26463,
                          "] out of bounds for array of shape [", n_26449, "].",
                          "-> #0  sparse.fut:111:40-76\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:109:19-114:50\n   #4  sparse.fut:131:23-48\n   #5  sparse.fut:244:20-62\n   #6  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t unionSizze_res_26511;
        
        if (empty_slice_26497) {
            unionSizze_res_26511 = j_m_i_26476;
        } else {
            int64_t conc_tmp_26512 = j_m_i_26476 + j_m_i_26496;
            int64_t j_m_i_26513 = sub64(conc_tmp_26512, (int64_t) 1);
            int64_t bytes_27979 = (int64_t) 8 * conc_tmp_26512;
            int64_t bytes_27996 = (int64_t) 4 * conc_tmp_26512;
            int64_t unionSizze_res_f_res_26514;
            
            if (empty_slice_26477) {
                unionSizze_res_f_res_26514 = j_m_i_26496;
            } else {
                struct memblock mem_27980;
                
                mem_27980.references = NULL;
                if (memblock_alloc(ctx, &mem_27980, bytes_27979, "mem_27980")) {
                    err = 1;
                    goto cleanup;
                }
                
                int64_t tmp_offs_29014 = (int64_t) 0;
                
                memmove(mem_27980.mem + tmp_offs_29014 * (int64_t) 8,
                        leftColumns_mem_27965.mem + i_26495 * (int64_t) 8,
                        j_m_i_26496 * (int64_t) sizeof(int64_t));
                tmp_offs_29014 += j_m_i_26496;
                memmove(mem_27980.mem + tmp_offs_29014 * (int64_t) 8,
                        rightColumns_mem_27968.mem + i_26475 * (int64_t) 8,
                        j_m_i_26476 * (int64_t) sizeof(int64_t));
                tmp_offs_29014 += j_m_i_26476;
                
                bool cond_26516 = conc_tmp_26512 == (int64_t) 0;
                int32_t iters_26517;
                
                if (cond_26516) {
                    iters_26517 = 0;
                } else {
                    iters_26517 = 32;
                }
                
                bool loop_nonempty_26518 = slt32(0, iters_26517);
                bool x_26519 = sle64((int64_t) 0, j_m_i_26513);
                bool y_26520 = slt64(j_m_i_26513, conc_tmp_26512);
                bool bounds_check_26521 = x_26519 && y_26520;
                bool loop_not_taken_26522 = !loop_nonempty_26518;
                bool protect_assert_disj_26523 = bounds_check_26521 ||
                     loop_not_taken_26522;
                bool index_certs_26524;
                
                if (!protect_assert_disj_26523) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", j_m_i_26513,
                                  "] out of bounds for array of shape [",
                                  conc_tmp_26512, "].",
                                  "-> #0  /prelude/array.fut:18:29-34\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:25:24-35\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:53:31-62\n   #3  sparse.fut:3:25-4:48\n   #4  sparse.fut:40:12-24\n   #5  sparse.fut:111:30-112:82\n   #6  /prelude/soacs.fut:56:19-23\n   #7  /prelude/soacs.fut:56:3-37\n   #8  sparse.fut:109:19-114:50\n   #9  sparse.fut:131:23-48\n   #10 sparse.fut:244:20-62\n   #11 sparse.fut:235:1-248:60\n");
                    if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                if (mem_27989_cached_sizze_29160 < (size_t) bytes_27979) {
                    mem_27989 = realloc(mem_27989, bytes_27979);
                    mem_27989_cached_sizze_29160 = bytes_27979;
                }
                if (mem_27991_cached_sizze_29161 < (size_t) bytes_27979) {
                    mem_27991 = realloc(mem_27991, bytes_27979);
                    mem_27991_cached_sizze_29161 = bytes_27979;
                }
                if (mem_27993_cached_sizze_29162 < (size_t) bytes_27979) {
                    mem_27993 = realloc(mem_27993, bytes_27979);
                    mem_27993_cached_sizze_29162 = bytes_27979;
                }
                if (mem_27995_cached_sizze_29163 < (size_t) bytes_27979) {
                    mem_27995 = realloc(mem_27995, bytes_27979);
                    mem_27995_cached_sizze_29163 = bytes_27979;
                }
                if (mem_27997_cached_sizze_29164 < (size_t) bytes_27996) {
                    mem_27997 = realloc(mem_27997, bytes_27996);
                    mem_27997_cached_sizze_29164 = bytes_27996;
                }
                
                struct memblock defunc_2_radix_sort_res_mem_28076;
                
                defunc_2_radix_sort_res_mem_28076.references = NULL;
                
                struct memblock mem_param_27985;
                
                mem_param_27985.references = NULL;
                if (memblock_set(ctx, &mem_param_27985, &mem_27980,
                                 "mem_27980") != 0)
                    return 1;
                for (int32_t i_26526 = 0; i_26526 < iters_26517; i_26526++) {
                    int32_t defunc_2_radix_sort_step_arg_26528 = mul32(2,
                                                                       i_26526);
                    int32_t defunc_0_get_bit_arg_26529 = add32(1,
                                                               defunc_2_radix_sort_step_arg_26528);
                    int64_t i32_res_26530 =
                            sext_i32_i64(defunc_0_get_bit_arg_26529);
                    int64_t i32_res_26531 =
                            sext_i32_i64(defunc_2_radix_sort_step_arg_26528);
                    int64_t discard_27592;
                    int64_t discard_27593;
                    int64_t discard_27594;
                    int64_t discard_27595;
                    int64_t scanacc_27577;
                    int64_t scanacc_27578;
                    int64_t scanacc_27579;
                    int64_t scanacc_27580;
                    
                    scanacc_27577 = (int64_t) 0;
                    scanacc_27578 = (int64_t) 0;
                    scanacc_27579 = (int64_t) 0;
                    scanacc_27580 = (int64_t) 0;
                    for (int64_t i_27586 = 0; i_27586 < conc_tmp_26512;
                         i_27586++) {
                        int64_t x_26559 =
                                ((int64_t *) mem_param_27985.mem)[i_27586];
                        int64_t zgzg_res_26560 = ashr64(x_26559, i32_res_26530);
                        int64_t za_res_26561 = (int64_t) 1 & zgzg_res_26560;
                        int32_t to_i32_res_26562 = sext_i64_i32(za_res_26561);
                        int32_t x_26563 = mul32(2, to_i32_res_26562);
                        int64_t zgzg_res_26564 = ashr64(x_26559, i32_res_26531);
                        int64_t za_res_26565 = (int64_t) 1 & zgzg_res_26564;
                        int32_t to_i32_res_26566 = sext_i64_i32(za_res_26565);
                        int32_t defunc_0_f_res_26567 = add32(x_26563,
                                                             to_i32_res_26566);
                        bool cond_26568 = defunc_0_f_res_26567 == 0;
                        int64_t defunc_0_f_res_26569 =
                                btoi_bool_i64(cond_26568);
                        int64_t defunc_0_f_res_26570;
                        int64_t defunc_0_f_res_26571;
                        int64_t defunc_0_f_res_26572;
                        
                        if (cond_26568) {
                            defunc_0_f_res_26570 = (int64_t) 0;
                            defunc_0_f_res_26571 = (int64_t) 0;
                            defunc_0_f_res_26572 = (int64_t) 0;
                        } else {
                            bool cond_26573 = defunc_0_f_res_26567 == 1;
                            int64_t defunc_0_f_res_f_res_26574 =
                                    btoi_bool_i64(cond_26573);
                            int64_t defunc_0_f_res_f_res_26575;
                            int64_t defunc_0_f_res_f_res_26576;
                            
                            if (cond_26573) {
                                defunc_0_f_res_f_res_26575 = (int64_t) 0;
                                defunc_0_f_res_f_res_26576 = (int64_t) 0;
                            } else {
                                bool cond_26577 = defunc_0_f_res_26567 == 2;
                                int64_t defunc_0_f_res_f_res_f_res_26578 =
                                        btoi_bool_i64(cond_26577);
                                bool cond_neg_26579 = !cond_26577;
                                int64_t defunc_0_f_res_f_res_f_res_26580 =
                                        btoi_bool_i64(cond_neg_26579);
                                
                                defunc_0_f_res_f_res_26575 =
                                    defunc_0_f_res_f_res_f_res_26578;
                                defunc_0_f_res_f_res_26576 =
                                    defunc_0_f_res_f_res_f_res_26580;
                            }
                            defunc_0_f_res_26570 = defunc_0_f_res_f_res_26574;
                            defunc_0_f_res_26571 = defunc_0_f_res_f_res_26575;
                            defunc_0_f_res_26572 = defunc_0_f_res_f_res_26576;
                        }
                        
                        int64_t defunc_1_op_res_26555 =
                                add64(defunc_0_f_res_26569, scanacc_27577);
                        int64_t defunc_1_op_res_26556 =
                                add64(defunc_0_f_res_26570, scanacc_27578);
                        int64_t defunc_1_op_res_26557 =
                                add64(defunc_0_f_res_26571, scanacc_27579);
                        int64_t defunc_1_op_res_26558 =
                                add64(defunc_0_f_res_26572, scanacc_27580);
                        
                        ((int64_t *) mem_27989)[i_27586] =
                            defunc_1_op_res_26555;
                        ((int64_t *) mem_27991)[i_27586] =
                            defunc_1_op_res_26556;
                        ((int64_t *) mem_27993)[i_27586] =
                            defunc_1_op_res_26557;
                        ((int64_t *) mem_27995)[i_27586] =
                            defunc_1_op_res_26558;
                        ((int32_t *) mem_27997)[i_27586] = defunc_0_f_res_26567;
                        
                        int64_t scanacc_tmp_29017 = defunc_1_op_res_26555;
                        int64_t scanacc_tmp_29018 = defunc_1_op_res_26556;
                        int64_t scanacc_tmp_29019 = defunc_1_op_res_26557;
                        int64_t scanacc_tmp_29020 = defunc_1_op_res_26558;
                        
                        scanacc_27577 = scanacc_tmp_29017;
                        scanacc_27578 = scanacc_tmp_29018;
                        scanacc_27579 = scanacc_tmp_29019;
                        scanacc_27580 = scanacc_tmp_29020;
                    }
                    discard_27592 = scanacc_27577;
                    discard_27593 = scanacc_27578;
                    discard_27594 = scanacc_27579;
                    discard_27595 = scanacc_27580;
                    
                    int64_t last_res_26586 =
                            ((int64_t *) mem_27989)[j_m_i_26513];
                    int64_t last_res_26587 =
                            ((int64_t *) mem_27991)[j_m_i_26513];
                    int64_t last_res_26588 =
                            ((int64_t *) mem_27993)[j_m_i_26513];
                    struct memblock mem_28059;
                    
                    mem_28059.references = NULL;
                    if (memblock_alloc(ctx, &mem_28059, bytes_27979,
                                       "mem_28059")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28059.mem + (int64_t) 0, mem_param_27985.mem +
                            (int64_t) 0, conc_tmp_26512 *
                            (int64_t) sizeof(int64_t));
                    for (int64_t write_iter_27596 = 0; write_iter_27596 <
                         conc_tmp_26512; write_iter_27596++) {
                        int32_t write_iv_27598 =
                                ((int32_t *) mem_27997)[write_iter_27596];
                        int64_t write_iv_27599 =
                                ((int64_t *) mem_27989)[write_iter_27596];
                        int64_t write_iv_27600 =
                                ((int64_t *) mem_27991)[write_iter_27596];
                        int64_t write_iv_27601 =
                                ((int64_t *) mem_27993)[write_iter_27596];
                        int64_t write_iv_27602 =
                                ((int64_t *) mem_27995)[write_iter_27596];
                        bool match_lit_26597 = 0 == write_iv_27598;
                        int64_t defunc_1_f_res_26598;
                        
                        if (match_lit_26597) {
                            int64_t case_res_26599 = sub64(write_iv_27599,
                                                           (int64_t) 1);
                            
                            defunc_1_f_res_26598 = case_res_26599;
                        } else {
                            bool match_lit_26600 = 1 == write_iv_27598;
                            int64_t x_26601;
                            
                            if (match_lit_26600) {
                                int64_t x_26602 = add64(last_res_26586,
                                                        write_iv_27600);
                                int64_t case_res_26603 = sub64(x_26602,
                                                               (int64_t) 1);
                                
                                x_26601 = case_res_26603;
                            } else {
                                bool match_lit_26604 = 2 == write_iv_27598;
                                int64_t x_26605;
                                
                                if (match_lit_26604) {
                                    int64_t x_26606 = add64(last_res_26586,
                                                            last_res_26587);
                                    int64_t x_26607 = add64(x_26606,
                                                            write_iv_27601);
                                    int64_t case_res_26608 = sub64(x_26607,
                                                                   (int64_t) 1);
                                    
                                    x_26605 = case_res_26608;
                                } else {
                                    int64_t x_26609 = add64(last_res_26586,
                                                            last_res_26587);
                                    int64_t x_26610 = add64(last_res_26588,
                                                            x_26609);
                                    int64_t x_26611 = add64(x_26610,
                                                            write_iv_27602);
                                    int64_t defunc_1_f_res_res_26612 =
                                            sub64(x_26611, (int64_t) 1);
                                    
                                    x_26605 = defunc_1_f_res_res_26612;
                                }
                                x_26601 = x_26605;
                            }
                            defunc_1_f_res_26598 = x_26601;
                        }
                        
                        bool less_than_zzero_27604 = slt64(defunc_1_f_res_26598,
                                                           (int64_t) 0);
                        bool greater_than_sizze_27605 = sle64(conc_tmp_26512,
                                                              defunc_1_f_res_26598);
                        bool outside_bounds_dim_27606 = less_than_zzero_27604 ||
                             greater_than_sizze_27605;
                        
                        if (!outside_bounds_dim_27606) {
                            memmove(mem_28059.mem + defunc_1_f_res_26598 *
                                    (int64_t) 8, mem_param_27985.mem +
                                    write_iter_27596 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                    }
                    
                    struct memblock mem_param_tmp_29015;
                    
                    mem_param_tmp_29015.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29015, &mem_28059,
                                     "mem_28059") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_27985,
                                     &mem_param_tmp_29015,
                                     "mem_param_tmp_29015") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29015,
                                       "mem_param_tmp_29015") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28059, "mem_28059") != 0)
                        return 1;
                }
                if (memblock_set(ctx, &defunc_2_radix_sort_res_mem_28076,
                                 &mem_param_27985, "mem_param_27985") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                    return 1;
                
                bool empty_slice_26613 = j_m_i_26513 == (int64_t) 0;
                bool i_lte_j_26614 = sle64((int64_t) 1, conc_tmp_26512);
                bool y_26615 = bounds_check_26521 && i_lte_j_26614;
                bool ok_or_empty_26616 = empty_slice_26613 || y_26615;
                bool index_certs_26617;
                
                if (!ok_or_empty_26616) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", (int64_t) 1,
                                  ":] out of bounds for array of shape [",
                                  conc_tmp_26512, "].",
                                  "-> #0  sparse.fut:43:97-106\n   #1  sparse.fut:111:30-112:82\n   #2  /prelude/soacs.fut:56:19-23\n   #3  /prelude/soacs.fut:56:3-37\n   #4  sparse.fut:109:19-114:50\n   #5  sparse.fut:131:23-48\n   #6  sparse.fut:244:20-62\n   #7  sparse.fut:235:1-248:60\n");
                    if (memblock_unref(ctx, &mem_param_27985,
                                       "mem_param_27985") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28076,
                                       "defunc_2_radix_sort_res_mem_28076") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                
                int64_t defunc_2_reduce_res_26620;
                int64_t redout_27610 = (int64_t) 1;
                
                for (int64_t i_27612 = 0; i_27612 < j_m_i_26513; i_27612++) {
                    int64_t slice_27923 = (int64_t) 1 + i_27612;
                    int64_t x_26635 =
                            ((int64_t *) defunc_2_radix_sort_res_mem_28076.mem)[slice_27923];
                    bool y_26628 = slt64(i_27612, conc_tmp_26512);
                    bool index_certs_26630;
                    
                    if (!y_26628) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", i_27612,
                                      "] out of bounds for array of shape [",
                                      conc_tmp_26512, "].",
                                      "-> #0  sparse.fut:43:41-51\n   #1  sparse.fut:43:4-106\n   #2  sparse.fut:111:30-112:82\n   #3  /prelude/soacs.fut:56:19-23\n   #4  /prelude/soacs.fut:56:3-37\n   #5  sparse.fut:109:19-114:50\n   #6  sparse.fut:131:23-48\n   #7  sparse.fut:244:20-62\n   #8  sparse.fut:235:1-248:60\n");
                        if (memblock_unref(ctx, &mem_param_27985,
                                           "mem_param_27985") != 0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &defunc_2_radix_sort_res_mem_28076,
                                           "defunc_2_radix_sort_res_mem_28076") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    int64_t y_26631 =
                            ((int64_t *) defunc_2_radix_sort_res_mem_28076.mem)[i_27612];
                    bool cond_26632 = x_26635 == y_26631;
                    int64_t defunc_1_op_res_26633;
                    
                    if (cond_26632) {
                        defunc_1_op_res_26633 = redout_27610;
                    } else {
                        int64_t defunc_1_op_res_f_res_26634 = add64((int64_t) 1,
                                                                    redout_27610);
                        
                        defunc_1_op_res_26633 = defunc_1_op_res_f_res_26634;
                    }
                    
                    int64_t redout_tmp_29027 = defunc_1_op_res_26633;
                    
                    redout_27610 = redout_tmp_29027;
                }
                defunc_2_reduce_res_26620 = redout_27610;
                if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28076,
                                   "defunc_2_radix_sort_res_mem_28076") != 0)
                    return 1;
                unionSizze_res_f_res_26514 = defunc_2_reduce_res_26620;
                if (memblock_unref(ctx, &mem_param_27985, "mem_param_27985") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28076,
                                   "defunc_2_radix_sort_res_mem_28076") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                    return 1;
            }
            unionSizze_res_26511 = unionSizze_res_f_res_26514;
        }
        
        int64_t defunc_1_op_res_26462 = add64(unionSizze_res_26511,
                                              scanacc_27614);
        
        ((int64_t *) mem_27971)[i_27616] = defunc_1_op_res_26462;
        
        int64_t scanacc_tmp_29012 = defunc_1_op_res_26462;
        
        scanacc_27614 = scanacc_tmp_29012;
    }
    discard_27618 = scanacc_27614;
    
    int64_t i_26637 = sub64(m_26450, (int64_t) 1);
    bool x_26638 = sle64((int64_t) 0, i_26637);
    bool y_26639 = slt64(i_26637, m_26450);
    bool bounds_check_26640 = x_26638 && y_26639;
    bool index_certs_26641;
    
    if (!bounds_check_26640) {
        ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                               "Index [", i_26637,
                               "] out of bounds for array of shape [", m_26450,
                               "].",
                               "-> #0  sparse.fut:115:30-41\n   #1  sparse.fut:131:23-48\n   #2  sparse.fut:244:20-62\n   #3  sparse.fut:235:1-248:60\n");
        if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
            return 1;
        return 1;
    }
    
    int64_t eWiseAddOffsets_res_26642 = ((int64_t *) mem_27971)[i_26637];
    bool bounds_invalid_upwards_26643 = slt64(eWiseAddOffsets_res_26642,
                                              (int64_t) 0);
    bool valid_26644 = !bounds_invalid_upwards_26643;
    bool range_valid_c_26645;
    
    if (!valid_26644) {
        ctx->error = msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                               "Range ", (int64_t) 0, "..", (int64_t) 1, "..<",
                               eWiseAddOffsets_res_26642, " is invalid.",
                               "-> #0  /prelude/array.fut:60:3-10\n   #1  /prelude/array.fut:70:18-23\n   #2  sparse.fut:132:16-35\n   #3  sparse.fut:244:20-62\n   #4  sparse.fut:235:1-248:60\n");
        if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
            return 1;
        return 1;
    }
    
    int64_t bytes_28082 = (int64_t) 8 * eWiseAddOffsets_res_26642;
    
    if (mem_28083_cached_sizze_29165 < (size_t) bytes_28082) {
        mem_28083 = realloc(mem_28083, bytes_28082);
        mem_28083_cached_sizze_29165 = bytes_28082;
    }
    for (int64_t i_29028 = 0; i_29028 < eWiseAddOffsets_res_26642; i_29028++) {
        ((double *) mem_28083)[i_29028] = 0.0;
    }
    if (mem_28085_cached_sizze_29166 < (size_t) bytes_28082) {
        mem_28085 = realloc(mem_28085, bytes_28082);
        mem_28085_cached_sizze_29166 = bytes_28082;
    }
    for (int64_t i_29029 = 0; i_29029 < eWiseAddOffsets_res_26642; i_29029++) {
        ((int64_t *) mem_28085)[i_29029] = (int64_t) 0;
    }
    for (int64_t i_26650 = 0; i_26650 < i_26637; i_26650++) {
        bool y_26653 = slt64(i_26650, m_26450);
        bool index_certs_26654;
        
        if (!y_26653) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", i_26650,
                                   "] out of bounds for array of shape [",
                                   m_26450, "].",
                                   "-> #0  sparse.fut:140:46-61\n   #1  sparse.fut:244:20-62\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_26655 = ((int64_t *) rightOffsets_mem_27969.mem)[i_26650];
        int64_t i_26656 = add64((int64_t) 1, i_26650);
        bool x_26657 = sle64((int64_t) 0, i_26656);
        bool y_26658 = slt64(i_26656, m_26450);
        bool bounds_check_26659 = x_26657 && y_26658;
        bool index_certs_26660;
        
        if (!bounds_check_26659) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", i_26656,
                                   "] out of bounds for array of shape [",
                                   m_26450, "].",
                                   "-> #0  sparse.fut:140:63-80\n   #1  sparse.fut:244:20-62\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t j_26661 = ((int64_t *) rightOffsets_mem_27969.mem)[i_26656];
        int64_t j_m_i_26662 = sub64(j_26661, i_26655);
        bool empty_slice_26663 = j_m_i_26662 == (int64_t) 0;
        int64_t m_26664 = sub64(j_m_i_26662, (int64_t) 1);
        int64_t i_p_m_t_s_26665 = add64(i_26655, m_26664);
        bool zzero_leq_i_p_m_t_s_26666 = sle64((int64_t) 0, i_p_m_t_s_26665);
        bool i_p_m_t_s_leq_w_26667 = slt64(i_p_m_t_s_26665, k_26451);
        bool zzero_lte_i_26668 = sle64((int64_t) 0, i_26655);
        bool i_lte_j_26669 = sle64(i_26655, j_26661);
        bool y_26670 = i_p_m_t_s_leq_w_26667 && zzero_lte_i_26668;
        bool y_26671 = zzero_leq_i_p_m_t_s_26666 && y_26670;
        bool y_26672 = i_lte_j_26669 && y_26671;
        bool forwards_ok_26673 = zzero_lte_i_26668 && y_26672;
        bool ok_or_empty_26674 = empty_slice_26663 || forwards_ok_26673;
        bool index_certs_26675;
        
        if (!ok_or_empty_26674) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_26655, ":", j_26661,
                          "] out of bounds for array of shape [", k_26451, "].",
                          "-> #0  sparse.fut:140:32-81\n   #1  sparse.fut:244:20-62\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_26677 = ((int64_t *) leftOffsets_mem_27966.mem)[i_26650];
        int64_t j_26678 = ((int64_t *) leftOffsets_mem_27966.mem)[i_26656];
        int64_t j_m_i_26679 = sub64(j_26678, i_26677);
        bool empty_slice_26680 = j_m_i_26679 == (int64_t) 0;
        int64_t m_26681 = sub64(j_m_i_26679, (int64_t) 1);
        int64_t i_p_m_t_s_26682 = add64(i_26677, m_26681);
        bool zzero_leq_i_p_m_t_s_26683 = sle64((int64_t) 0, i_p_m_t_s_26682);
        bool i_p_m_t_s_leq_w_26684 = slt64(i_p_m_t_s_26682, n_26449);
        bool zzero_lte_i_26685 = sle64((int64_t) 0, i_26677);
        bool i_lte_j_26686 = sle64(i_26677, j_26678);
        bool y_26687 = i_p_m_t_s_leq_w_26684 && zzero_lte_i_26685;
        bool y_26688 = zzero_leq_i_p_m_t_s_26683 && y_26687;
        bool y_26689 = i_lte_j_26686 && y_26688;
        bool forwards_ok_26690 = zzero_lte_i_26685 && y_26689;
        bool ok_or_empty_26691 = empty_slice_26680 || forwards_ok_26690;
        bool index_certs_26692;
        
        if (!ok_or_empty_26691) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_26677, ":", j_26678,
                          "] out of bounds for array of shape [", n_26449, "].",
                          "-> #0  sparse.fut:139:32-78\n   #1  sparse.fut:244:20-62\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t bytes_28100 = (int64_t) 8 * j_m_i_26662;
        struct memblock mem_28101;
        
        mem_28101.references = NULL;
        if (memblock_alloc(ctx, &mem_28101, bytes_28100, "mem_28101")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_29032 = 0; i_29032 < j_m_i_26662; i_29032++) {
            ((int64_t *) mem_28101.mem)[i_29032] = (int64_t) 0;
        }
        
        struct memblock mem_28103;
        
        mem_28103.references = NULL;
        if (memblock_alloc(ctx, &mem_28103, bytes_28100, "mem_28103")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_29033 = 0; i_29033 < j_m_i_26662; i_29033++) {
            int64_t x_29034 = (int64_t) 0 + i_29033 * (int64_t) 1;
            
            ((int64_t *) mem_28103.mem)[i_29033] = x_29034;
        }
        
        struct memblock mem_28105;
        
        mem_28105.references = NULL;
        if (memblock_alloc(ctx, &mem_28105, bytes_28100, "mem_28105")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_29035 = 0; i_29035 < j_m_i_26662; i_29035++) {
            ((int64_t *) mem_28105.mem)[i_29035] = (int64_t) 1;
        }
        
        int64_t conc_tmp_26697 = j_m_i_26662 + j_m_i_26679;
        int64_t bytes_28106 = (int64_t) 8 * j_m_i_26679;
        int64_t bytes_28110 = (int64_t) 8 * conc_tmp_26697;
        int64_t sizze_26698;
        int64_t sizze_26699;
        int64_t sizze_26700;
        int64_t sizze_26701;
        int64_t unionArrays_res_ixfn_28452;
        int64_t unionArrays_res_ixfn_28453;
        int64_t unionArrays_res_ixfn_28455;
        int64_t unionArrays_res_ixfn_28457;
        int64_t unionArrays_res_ixfn_28458;
        int64_t unionArrays_res_ixfn_28462;
        int64_t unionArrays_res_ixfn_28463;
        int64_t unionArrays_res_ixfn_28467;
        int64_t unionArrays_res_ixfn_28468;
        struct memblock unionArrays_res_mem_28456;
        
        unionArrays_res_mem_28456.references = NULL;
        
        struct memblock unionArrays_res_mem_28461;
        
        unionArrays_res_mem_28461.references = NULL;
        
        struct memblock unionArrays_res_mem_28466;
        
        unionArrays_res_mem_28466.references = NULL;
        
        struct memblock unionArrays_res_mem_28471;
        
        unionArrays_res_mem_28471.references = NULL;
        if (empty_slice_26680) {
            sizze_26698 = j_m_i_26662;
            sizze_26699 = j_m_i_26662;
            sizze_26700 = j_m_i_26662;
            sizze_26701 = j_m_i_26662;
            unionArrays_res_ixfn_28452 = j_m_i_26662;
            unionArrays_res_ixfn_28453 = k_26451;
            unionArrays_res_ixfn_28455 = i_26655;
            unionArrays_res_ixfn_28457 = j_m_i_26662;
            unionArrays_res_ixfn_28458 = j_m_i_26662;
            unionArrays_res_ixfn_28462 = j_m_i_26662;
            unionArrays_res_ixfn_28463 = j_m_i_26662;
            unionArrays_res_ixfn_28467 = j_m_i_26662;
            unionArrays_res_ixfn_28468 = j_m_i_26662;
            if (memblock_set(ctx, &unionArrays_res_mem_28456,
                             &rightColumns_mem_27968,
                             "rightColumns_mem_27968") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28461, &mem_28105,
                             "mem_28105") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28466, &mem_28103,
                             "mem_28103") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28471, &mem_28101,
                             "mem_28101") != 0)
                return 1;
        } else {
            struct memblock mem_28107;
            
            mem_28107.references = NULL;
            if (memblock_alloc(ctx, &mem_28107, bytes_28106, "mem_28107")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_29036 = 0; i_29036 < j_m_i_26679; i_29036++) {
                ((int64_t *) mem_28107.mem)[i_29036] = (int64_t) 0;
            }
            
            struct memblock mem_28109;
            
            mem_28109.references = NULL;
            if (memblock_alloc(ctx, &mem_28109, bytes_28106, "mem_28109")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_29037 = 0; i_29037 < j_m_i_26679; i_29037++) {
                int64_t x_29038 = (int64_t) 0 + i_29037 * (int64_t) 1;
                
                ((int64_t *) mem_28109.mem)[i_29037] = x_29038;
            }
            
            struct memblock mem_28111;
            
            mem_28111.references = NULL;
            if (memblock_alloc(ctx, &mem_28111, bytes_28110, "mem_28111")) {
                err = 1;
                goto cleanup;
            }
            
            int64_t tmp_offs_29039 = (int64_t) 0;
            
            memmove(mem_28111.mem + tmp_offs_29039 * (int64_t) 8,
                    leftColumns_mem_27965.mem + i_26677 * (int64_t) 8,
                    j_m_i_26679 * (int64_t) sizeof(int64_t));
            tmp_offs_29039 += j_m_i_26679;
            memmove(mem_28111.mem + tmp_offs_29039 * (int64_t) 8,
                    rightColumns_mem_27968.mem + i_26655 * (int64_t) 8,
                    j_m_i_26662 * (int64_t) sizeof(int64_t));
            tmp_offs_29039 += j_m_i_26662;
            
            struct memblock mem_28113;
            
            mem_28113.references = NULL;
            if (memblock_alloc(ctx, &mem_28113, bytes_28110, "mem_28113")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_29040 = 0; i_29040 < conc_tmp_26697; i_29040++) {
                int64_t x_29041 = (int64_t) 0 + i_29040 * (int64_t) 1;
                
                ((int64_t *) mem_28113.mem)[i_29040] = x_29041;
            }
            
            bool cond_26710 = conc_tmp_26697 == (int64_t) 0;
            int32_t iters_26711;
            
            if (cond_26710) {
                iters_26711 = 0;
            } else {
                iters_26711 = 32;
            }
            
            bool loop_nonempty_26712 = slt32(0, iters_26711);
            int64_t i_26713 = sub64(conc_tmp_26697, (int64_t) 1);
            bool x_26714 = sle64((int64_t) 0, i_26713);
            bool y_26715 = slt64(i_26713, conc_tmp_26697);
            bool bounds_check_26716 = x_26714 && y_26715;
            bool loop_not_taken_26717 = !loop_nonempty_26712;
            bool protect_assert_disj_26718 = bounds_check_26716 ||
                 loop_not_taken_26717;
            
            if (mem_28115_cached_sizze_29167 < (size_t) bytes_28110) {
                mem_28115 = realloc(mem_28115, bytes_28110);
                mem_28115_cached_sizze_29167 = bytes_28110;
            }
            if (mem_28117_cached_sizze_29168 < (size_t) bytes_28110) {
                mem_28117 = realloc(mem_28117, bytes_28110);
                mem_28117_cached_sizze_29168 = bytes_28110;
            }
            if (mem_28119_cached_sizze_29169 < (size_t) bytes_28110) {
                mem_28119 = realloc(mem_28119, bytes_28110);
                mem_28119_cached_sizze_29169 = bytes_28110;
            }
            if (mem_28121_cached_sizze_29170 < (size_t) bytes_28110) {
                mem_28121 = realloc(mem_28121, bytes_28110);
                mem_28121_cached_sizze_29170 = bytes_28110;
            }
            if (mem_28123_cached_sizze_29171 < (size_t) bytes_28110) {
                mem_28123 = realloc(mem_28123, bytes_28110);
                mem_28123_cached_sizze_29171 = bytes_28110;
            }
            if (mem_28125_cached_sizze_29172 < (size_t) bytes_28110) {
                mem_28125 = realloc(mem_28125, bytes_28110);
                mem_28125_cached_sizze_29172 = bytes_28110;
            }
            if (mem_28127_cached_sizze_29173 < (size_t) bytes_28110) {
                mem_28127 = realloc(mem_28127, bytes_28110);
                mem_28127_cached_sizze_29173 = bytes_28110;
            }
            if (mem_28129_cached_sizze_29174 < (size_t) bytes_28110) {
                mem_28129 = realloc(mem_28129, bytes_28110);
                mem_28129_cached_sizze_29174 = bytes_28110;
            }
            if (mem_28131_cached_sizze_29175 < (size_t) bytes_28110) {
                mem_28131 = realloc(mem_28131, bytes_28110);
                mem_28131_cached_sizze_29175 = bytes_28110;
            }
            if (mem_28133_cached_sizze_29176 < (size_t) bytes_28110) {
                mem_28133 = realloc(mem_28133, bytes_28110);
                mem_28133_cached_sizze_29176 = bytes_28110;
            }
            
            int64_t bytes_28156 = (int64_t) 4 * conc_tmp_26697;
            int64_t unionArrays_res_f_res_ixfn_28433;
            
            if (empty_slice_26663) {
                unionArrays_res_f_res_ixfn_28433 = n_26449;
            } else {
                unionArrays_res_f_res_ixfn_28433 = conc_tmp_26697;
            }
            
            int64_t unionArrays_res_f_res_ixfn_28435;
            
            if (empty_slice_26663) {
                unionArrays_res_f_res_ixfn_28435 = i_26677;
            } else {
                unionArrays_res_f_res_ixfn_28435 = (int64_t) 0;
            }
            
            int64_t unionArrays_res_f_res_ixfn_28438;
            
            if (empty_slice_26663) {
                unionArrays_res_f_res_ixfn_28438 = j_m_i_26679;
            } else {
                unionArrays_res_f_res_ixfn_28438 = conc_tmp_26697;
            }
            
            int64_t unionArrays_res_f_res_ixfn_28443;
            
            if (empty_slice_26663) {
                unionArrays_res_f_res_ixfn_28443 = j_m_i_26679;
            } else {
                unionArrays_res_f_res_ixfn_28443 = conc_tmp_26697;
            }
            
            int64_t unionArrays_res_f_res_ixfn_28448;
            
            if (empty_slice_26663) {
                unionArrays_res_f_res_ixfn_28448 = j_m_i_26679;
            } else {
                unionArrays_res_f_res_ixfn_28448 = conc_tmp_26697;
            }
            
            int64_t sizze_26719;
            int64_t sizze_26720;
            int64_t sizze_26721;
            int64_t sizze_26722;
            int64_t unionArrays_res_f_res_ixfn_28432;
            int64_t unionArrays_res_f_res_ixfn_28437;
            int64_t unionArrays_res_f_res_ixfn_28442;
            int64_t unionArrays_res_f_res_ixfn_28447;
            struct memblock unionArrays_res_f_res_mem_28436;
            
            unionArrays_res_f_res_mem_28436.references = NULL;
            
            struct memblock unionArrays_res_f_res_mem_28441;
            
            unionArrays_res_f_res_mem_28441.references = NULL;
            
            struct memblock unionArrays_res_f_res_mem_28446;
            
            unionArrays_res_f_res_mem_28446.references = NULL;
            
            struct memblock unionArrays_res_f_res_mem_28451;
            
            unionArrays_res_f_res_mem_28451.references = NULL;
            if (empty_slice_26663) {
                sizze_26719 = j_m_i_26679;
                sizze_26720 = j_m_i_26679;
                sizze_26721 = j_m_i_26679;
                sizze_26722 = j_m_i_26679;
                unionArrays_res_f_res_ixfn_28432 = j_m_i_26679;
                unionArrays_res_f_res_ixfn_28437 = j_m_i_26679;
                unionArrays_res_f_res_ixfn_28442 = j_m_i_26679;
                unionArrays_res_f_res_ixfn_28447 = j_m_i_26679;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28436,
                                 &leftColumns_mem_27965,
                                 "leftColumns_mem_27965") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28441,
                                 &mem_28107, "mem_28107") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28446,
                                 &mem_28109, "mem_28109") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28451,
                                 &mem_28107, "mem_28107") != 0)
                    return 1;
            } else {
                bool index_certs_26727;
                
                if (!protect_assert_disj_26718) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", i_26713,
                                  "] out of bounds for array of shape [",
                                  conc_tmp_26697, "].",
                                  "-> #0  /prelude/array.fut:18:29-34\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:25:24-35\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:53:31-62\n   #3  /prelude/functional.fut:9:42-44\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:59:3-61:47\n   #5  lib/github.com/diku-dk/sorts/radix_sort.fut:68:3-51\n   #6  sparse.fut:5:34-6:77\n   #7  sparse.fut:53:12-35\n   #8  sparse.fut:138:59-140:81\n   #9  sparse.fut:244:20-62\n   #10 sparse.fut:235:1-248:60\n");
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28451,
                                       "unionArrays_res_f_res_mem_28451") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28446,
                                       "unionArrays_res_f_res_mem_28446") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28441,
                                       "unionArrays_res_f_res_mem_28441") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28436,
                                       "unionArrays_res_f_res_mem_28436") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28113, "mem_28113") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28111, "mem_28111") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28109, "mem_28109") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28107, "mem_28107") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                       "unionArrays_res_mem_28471") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                       "unionArrays_res_mem_28466") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                       "unionArrays_res_mem_28461") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                       "unionArrays_res_mem_28456") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                if (mem_28149_cached_sizze_29177 < (size_t) bytes_28110) {
                    mem_28149 = realloc(mem_28149, bytes_28110);
                    mem_28149_cached_sizze_29177 = bytes_28110;
                }
                if (mem_28151_cached_sizze_29178 < (size_t) bytes_28110) {
                    mem_28151 = realloc(mem_28151, bytes_28110);
                    mem_28151_cached_sizze_29178 = bytes_28110;
                }
                if (mem_28153_cached_sizze_29179 < (size_t) bytes_28110) {
                    mem_28153 = realloc(mem_28153, bytes_28110);
                    mem_28153_cached_sizze_29179 = bytes_28110;
                }
                if (mem_28155_cached_sizze_29180 < (size_t) bytes_28110) {
                    mem_28155 = realloc(mem_28155, bytes_28110);
                    mem_28155_cached_sizze_29180 = bytes_28110;
                }
                if (mem_28157_cached_sizze_29181 < (size_t) bytes_28156) {
                    mem_28157 = realloc(mem_28157, bytes_28156);
                    mem_28157_cached_sizze_29181 = bytes_28156;
                }
                
                struct memblock defunc_0_f_res_mem_28250;
                
                defunc_0_f_res_mem_28250.references = NULL;
                
                struct memblock defunc_0_f_res_mem_28255;
                
                defunc_0_f_res_mem_28255.references = NULL;
                
                struct memblock mem_param_28138;
                
                mem_param_28138.references = NULL;
                
                struct memblock mem_param_28143;
                
                mem_param_28143.references = NULL;
                if (memblock_set(ctx, &mem_param_28138, &mem_28111,
                                 "mem_28111") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_28143, &mem_28113,
                                 "mem_28113") != 0)
                    return 1;
                for (int32_t i_26730 = 0; i_26730 < iters_26711; i_26730++) {
                    int32_t defunc_2_radix_sort_step_arg_26733 = mul32(2,
                                                                       i_26730);
                    int32_t defunc_0_get_bit_arg_26734 = add32(1,
                                                               defunc_2_radix_sort_step_arg_26733);
                    int64_t i32_res_26735 =
                            sext_i32_i64(defunc_0_get_bit_arg_26734);
                    int64_t i32_res_26736 =
                            sext_i32_i64(defunc_2_radix_sort_step_arg_26733);
                    int64_t discard_27639;
                    int64_t discard_27640;
                    int64_t discard_27641;
                    int64_t discard_27642;
                    int64_t scanacc_27624;
                    int64_t scanacc_27625;
                    int64_t scanacc_27626;
                    int64_t scanacc_27627;
                    
                    scanacc_27624 = (int64_t) 0;
                    scanacc_27625 = (int64_t) 0;
                    scanacc_27626 = (int64_t) 0;
                    scanacc_27627 = (int64_t) 0;
                    for (int64_t i_27633 = 0; i_27633 < conc_tmp_26697;
                         i_27633++) {
                        int64_t x_26764 =
                                ((int64_t *) mem_param_28138.mem)[i_27633];
                        int64_t zgzg_res_26765 = ashr64(x_26764, i32_res_26735);
                        int64_t za_res_26766 = (int64_t) 1 & zgzg_res_26765;
                        int32_t to_i32_res_26767 = sext_i64_i32(za_res_26766);
                        int32_t x_26768 = mul32(2, to_i32_res_26767);
                        int64_t zgzg_res_26769 = ashr64(x_26764, i32_res_26736);
                        int64_t za_res_26770 = (int64_t) 1 & zgzg_res_26769;
                        int32_t to_i32_res_26771 = sext_i64_i32(za_res_26770);
                        int32_t defunc_0_f_res_26772 = add32(x_26768,
                                                             to_i32_res_26771);
                        bool cond_26773 = defunc_0_f_res_26772 == 0;
                        int64_t defunc_0_f_res_26774 =
                                btoi_bool_i64(cond_26773);
                        int64_t defunc_0_f_res_26775;
                        int64_t defunc_0_f_res_26776;
                        int64_t defunc_0_f_res_26777;
                        
                        if (cond_26773) {
                            defunc_0_f_res_26775 = (int64_t) 0;
                            defunc_0_f_res_26776 = (int64_t) 0;
                            defunc_0_f_res_26777 = (int64_t) 0;
                        } else {
                            bool cond_26778 = defunc_0_f_res_26772 == 1;
                            int64_t defunc_0_f_res_f_res_26779 =
                                    btoi_bool_i64(cond_26778);
                            int64_t defunc_0_f_res_f_res_26780;
                            int64_t defunc_0_f_res_f_res_26781;
                            
                            if (cond_26778) {
                                defunc_0_f_res_f_res_26780 = (int64_t) 0;
                                defunc_0_f_res_f_res_26781 = (int64_t) 0;
                            } else {
                                bool cond_26782 = defunc_0_f_res_26772 == 2;
                                int64_t defunc_0_f_res_f_res_f_res_26783 =
                                        btoi_bool_i64(cond_26782);
                                bool cond_neg_26784 = !cond_26782;
                                int64_t defunc_0_f_res_f_res_f_res_26785 =
                                        btoi_bool_i64(cond_neg_26784);
                                
                                defunc_0_f_res_f_res_26780 =
                                    defunc_0_f_res_f_res_f_res_26783;
                                defunc_0_f_res_f_res_26781 =
                                    defunc_0_f_res_f_res_f_res_26785;
                            }
                            defunc_0_f_res_26775 = defunc_0_f_res_f_res_26779;
                            defunc_0_f_res_26776 = defunc_0_f_res_f_res_26780;
                            defunc_0_f_res_26777 = defunc_0_f_res_f_res_26781;
                        }
                        
                        int64_t defunc_1_op_res_26760 =
                                add64(defunc_0_f_res_26774, scanacc_27624);
                        int64_t defunc_1_op_res_26761 =
                                add64(defunc_0_f_res_26775, scanacc_27625);
                        int64_t defunc_1_op_res_26762 =
                                add64(defunc_0_f_res_26776, scanacc_27626);
                        int64_t defunc_1_op_res_26763 =
                                add64(defunc_0_f_res_26777, scanacc_27627);
                        
                        ((int64_t *) mem_28149)[i_27633] =
                            defunc_1_op_res_26760;
                        ((int64_t *) mem_28151)[i_27633] =
                            defunc_1_op_res_26761;
                        ((int64_t *) mem_28153)[i_27633] =
                            defunc_1_op_res_26762;
                        ((int64_t *) mem_28155)[i_27633] =
                            defunc_1_op_res_26763;
                        ((int32_t *) mem_28157)[i_27633] = defunc_0_f_res_26772;
                        
                        int64_t scanacc_tmp_29046 = defunc_1_op_res_26760;
                        int64_t scanacc_tmp_29047 = defunc_1_op_res_26761;
                        int64_t scanacc_tmp_29048 = defunc_1_op_res_26762;
                        int64_t scanacc_tmp_29049 = defunc_1_op_res_26763;
                        
                        scanacc_27624 = scanacc_tmp_29046;
                        scanacc_27625 = scanacc_tmp_29047;
                        scanacc_27626 = scanacc_tmp_29048;
                        scanacc_27627 = scanacc_tmp_29049;
                    }
                    discard_27639 = scanacc_27624;
                    discard_27640 = scanacc_27625;
                    discard_27641 = scanacc_27626;
                    discard_27642 = scanacc_27627;
                    
                    int64_t last_res_26791 = ((int64_t *) mem_28149)[i_26713];
                    int64_t last_res_26792 = ((int64_t *) mem_28151)[i_26713];
                    int64_t last_res_26793 = ((int64_t *) mem_28153)[i_26713];
                    struct memblock mem_28219;
                    
                    mem_28219.references = NULL;
                    if (memblock_alloc(ctx, &mem_28219, bytes_28110,
                                       "mem_28219")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28219.mem + (int64_t) 0, mem_param_28138.mem +
                            (int64_t) 0, conc_tmp_26697 *
                            (int64_t) sizeof(int64_t));
                    
                    struct memblock mem_28221;
                    
                    mem_28221.references = NULL;
                    if (memblock_alloc(ctx, &mem_28221, bytes_28110,
                                       "mem_28221")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28221.mem + (int64_t) 0, mem_param_28143.mem +
                            (int64_t) 0, conc_tmp_26697 *
                            (int64_t) sizeof(int64_t));
                    for (int64_t write_iter_27643 = 0; write_iter_27643 <
                         conc_tmp_26697; write_iter_27643++) {
                        int32_t write_iv_27646 =
                                ((int32_t *) mem_28157)[write_iter_27643];
                        int64_t write_iv_27647 =
                                ((int64_t *) mem_28149)[write_iter_27643];
                        int64_t write_iv_27648 =
                                ((int64_t *) mem_28151)[write_iter_27643];
                        int64_t write_iv_27649 =
                                ((int64_t *) mem_28153)[write_iter_27643];
                        int64_t write_iv_27650 =
                                ((int64_t *) mem_28155)[write_iter_27643];
                        bool match_lit_26805 = 0 == write_iv_27646;
                        int64_t defunc_1_f_res_26806;
                        
                        if (match_lit_26805) {
                            int64_t case_res_26807 = sub64(write_iv_27647,
                                                           (int64_t) 1);
                            
                            defunc_1_f_res_26806 = case_res_26807;
                        } else {
                            bool match_lit_26808 = 1 == write_iv_27646;
                            int64_t x_26809;
                            
                            if (match_lit_26808) {
                                int64_t x_26810 = add64(last_res_26791,
                                                        write_iv_27648);
                                int64_t case_res_26811 = sub64(x_26810,
                                                               (int64_t) 1);
                                
                                x_26809 = case_res_26811;
                            } else {
                                bool match_lit_26812 = 2 == write_iv_27646;
                                int64_t x_26813;
                                
                                if (match_lit_26812) {
                                    int64_t x_26814 = add64(last_res_26791,
                                                            last_res_26792);
                                    int64_t x_26815 = add64(x_26814,
                                                            write_iv_27649);
                                    int64_t case_res_26816 = sub64(x_26815,
                                                                   (int64_t) 1);
                                    
                                    x_26813 = case_res_26816;
                                } else {
                                    int64_t x_26817 = add64(last_res_26791,
                                                            last_res_26792);
                                    int64_t x_26818 = add64(last_res_26793,
                                                            x_26817);
                                    int64_t x_26819 = add64(x_26818,
                                                            write_iv_27650);
                                    int64_t defunc_1_f_res_res_26820 =
                                            sub64(x_26819, (int64_t) 1);
                                    
                                    x_26813 = defunc_1_f_res_res_26820;
                                }
                                x_26809 = x_26813;
                            }
                            defunc_1_f_res_26806 = x_26809;
                        }
                        
                        bool less_than_zzero_27653 = slt64(defunc_1_f_res_26806,
                                                           (int64_t) 0);
                        bool greater_than_sizze_27654 = sle64(conc_tmp_26697,
                                                              defunc_1_f_res_26806);
                        bool outside_bounds_dim_27655 = less_than_zzero_27653 ||
                             greater_than_sizze_27654;
                        
                        if (!outside_bounds_dim_27655) {
                            memmove(mem_28219.mem + defunc_1_f_res_26806 *
                                    (int64_t) 8, mem_param_28138.mem +
                                    write_iter_27643 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                        if (!outside_bounds_dim_27655) {
                            memmove(mem_28221.mem + defunc_1_f_res_26806 *
                                    (int64_t) 8, mem_param_28143.mem +
                                    write_iter_27643 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                    }
                    
                    struct memblock mem_param_tmp_29042;
                    
                    mem_param_tmp_29042.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29042, &mem_28219,
                                     "mem_28219") != 0)
                        return 1;
                    
                    struct memblock mem_param_tmp_29043;
                    
                    mem_param_tmp_29043.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29043, &mem_28221,
                                     "mem_28221") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_28138,
                                     &mem_param_tmp_29042,
                                     "mem_param_tmp_29042") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_28143,
                                     &mem_param_tmp_29043,
                                     "mem_param_tmp_29043") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29043,
                                       "mem_param_tmp_29043") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29042,
                                       "mem_param_tmp_29042") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28221, "mem_28221") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28219, "mem_28219") != 0)
                        return 1;
                }
                if (memblock_set(ctx, &defunc_0_f_res_mem_28250,
                                 &mem_param_28138, "mem_param_28138") != 0)
                    return 1;
                if (memblock_set(ctx, &defunc_0_f_res_mem_28255,
                                 &mem_param_28143, "mem_param_28143") != 0)
                    return 1;
                for (int64_t i_27673 = 0; i_27673 < conc_tmp_26697; i_27673++) {
                    int64_t x_26825 =
                            ((int64_t *) defunc_0_f_res_mem_28255.mem)[i_27673];
                    bool x_26826 = sle64((int64_t) 0, x_26825);
                    bool y_26827 = slt64(x_26825, conc_tmp_26697);
                    bool bounds_check_26828 = x_26826 && y_26827;
                    bool index_certs_26829;
                    
                    if (!bounds_check_26828) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", x_26825,
                                      "] out of bounds for array of shape [",
                                      conc_tmp_26697, "].",
                                      "-> #0  lib/github.com/diku-dk/sorts/radix_sort.fut:62:28-32\n   #1  /prelude/functional.fut:9:42-44\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:59:3-62:33\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:68:3-51\n   #4  sparse.fut:5:34-6:77\n   #5  sparse.fut:53:12-35\n   #6  sparse.fut:138:59-140:81\n   #7  sparse.fut:244:20-62\n   #8  sparse.fut:235:1-248:60\n");
                        if (memblock_unref(ctx, &mem_param_28143,
                                           "mem_param_28143") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_param_28138,
                                           "mem_param_28138") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28255,
                                           "defunc_0_f_res_mem_28255") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28250,
                                           "defunc_0_f_res_mem_28250") != 0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &unionArrays_res_f_res_mem_28451,
                                           "unionArrays_res_f_res_mem_28451") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &unionArrays_res_f_res_mem_28446,
                                           "unionArrays_res_f_res_mem_28446") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &unionArrays_res_f_res_mem_28441,
                                           "unionArrays_res_f_res_mem_28441") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &unionArrays_res_f_res_mem_28436,
                                           "unionArrays_res_f_res_mem_28436") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28113, "mem_28113") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28111, "mem_28111") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28109, "mem_28109") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28107, "mem_28107") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                           "unionArrays_res_mem_28471") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                           "unionArrays_res_mem_28466") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                           "unionArrays_res_mem_28461") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                           "unionArrays_res_mem_28456") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    bool index_concat_cmp_26830 = sle64(j_m_i_26679, x_26825);
                    int64_t index_concat_branch_26831;
                    
                    if (index_concat_cmp_26830) {
                        int64_t index_concat_i_26832 = sub64(x_26825,
                                                             j_m_i_26679);
                        int64_t slice_26833 = i_26655 + index_concat_i_26832;
                        int64_t index_concat_26834 =
                                ((int64_t *) rightColumns_mem_27968.mem)[slice_26833];
                        
                        index_concat_branch_26831 = index_concat_26834;
                    } else {
                        int64_t slice_26835 = i_26677 + x_26825;
                        int64_t index_concat_26836 =
                                ((int64_t *) leftColumns_mem_27965.mem)[slice_26835];
                        
                        index_concat_branch_26831 = index_concat_26836;
                    }
                    
                    int64_t index_concat_branch_26838 =
                            btoi_bool_i64(index_concat_cmp_26830);
                    int64_t index_concat_branch_26840;
                    
                    if (index_concat_cmp_26830) {
                        int64_t index_concat_i_26841 = sub64(x_26825,
                                                             j_m_i_26679);
                        
                        index_concat_branch_26840 = index_concat_i_26841;
                    } else {
                        index_concat_branch_26840 = x_26825;
                    }
                    ((int64_t *) mem_28115)[i_27673] =
                        index_concat_branch_26831;
                    ((int64_t *) mem_28117)[i_27673] =
                        index_concat_branch_26838;
                    ((int64_t *) mem_28119)[i_27673] =
                        index_concat_branch_26840;
                    ((int64_t *) mem_28121)[i_27673] = (int64_t) 0;
                }
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28255,
                                   "defunc_0_f_res_mem_28255") != 0)
                    return 1;
                
                int64_t discard_27698;
                int64_t scanacc_27684 = (int64_t) 0;
                
                for (int64_t i_27691 = 0; i_27691 < conc_tmp_26697; i_27691++) {
                    int64_t x_26904 = ((int64_t *) mem_28115)[i_27691];
                    int64_t x_26905 = ((int64_t *) mem_28117)[i_27691];
                    int64_t x_26906 = ((int64_t *) mem_28119)[i_27691];
                    int64_t x_26907 = ((int64_t *) mem_28121)[i_27691];
                    bool cond_26909 = i_27691 == i_26713;
                    bool cond_26910 = !cond_26909;
                    bool cond_26911;
                    
                    if (cond_26910) {
                        int64_t i_26912 = add64((int64_t) 1, i_27691);
                        bool x_26913 = sle64((int64_t) 0, i_26912);
                        bool y_26914 = slt64(i_26912, conc_tmp_26697);
                        bool bounds_check_26915 = x_26913 && y_26914;
                        bool index_certs_26916;
                        
                        if (!bounds_check_26915) {
                            ctx->error =
                                msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                          "Index [", i_26912,
                                          "] out of bounds for array of shape [",
                                          conc_tmp_26697, "].",
                                          "-> #0  sparse.fut:60:63-73\n   #1  sparse.fut:58:33-68:55\n   #2  sparse.fut:138:59-140:81\n   #3  sparse.fut:244:20-62\n   #4  sparse.fut:235:1-248:60\n");
                            if (memblock_unref(ctx, &mem_param_28143,
                                               "mem_param_28143") != 0)
                                return 1;
                            if (memblock_unref(ctx, &mem_param_28138,
                                               "mem_param_28138") != 0)
                                return 1;
                            if (memblock_unref(ctx, &defunc_0_f_res_mem_28255,
                                               "defunc_0_f_res_mem_28255") != 0)
                                return 1;
                            if (memblock_unref(ctx, &defunc_0_f_res_mem_28250,
                                               "defunc_0_f_res_mem_28250") != 0)
                                return 1;
                            if (memblock_unref(ctx,
                                               &unionArrays_res_f_res_mem_28451,
                                               "unionArrays_res_f_res_mem_28451") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx,
                                               &unionArrays_res_f_res_mem_28446,
                                               "unionArrays_res_f_res_mem_28446") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx,
                                               &unionArrays_res_f_res_mem_28441,
                                               "unionArrays_res_f_res_mem_28441") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx,
                                               &unionArrays_res_f_res_mem_28436,
                                               "unionArrays_res_f_res_mem_28436") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28113, "mem_28113") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28111, "mem_28111") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28109, "mem_28109") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28107, "mem_28107") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                               "unionArrays_res_mem_28471") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                               "unionArrays_res_mem_28466") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                               "unionArrays_res_mem_28461") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                               "unionArrays_res_mem_28456") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28105, "mem_28105") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28103, "mem_28103") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &mem_28101, "mem_28101") !=
                                0)
                                return 1;
                            if (memblock_unref(ctx, &out_mem_29011,
                                               "out_mem_29011") != 0)
                                return 1;
                            if (memblock_unref(ctx, &out_mem_29009,
                                               "out_mem_29009") != 0)
                                return 1;
                            if (memblock_unref(ctx, &out_mem_29007,
                                               "out_mem_29007") != 0)
                                return 1;
                            return 1;
                        }
                        
                        int64_t y_26917 = ((int64_t *) mem_28115)[i_26912];
                        bool cond_t_res_26918 = x_26904 == y_26917;
                        
                        cond_26911 = cond_t_res_26918;
                    } else {
                        cond_26911 = 0;
                    }
                    
                    int64_t defunc_0_f_res_26919;
                    
                    if (cond_26911) {
                        defunc_0_f_res_26919 = (int64_t) -1;
                    } else {
                        defunc_0_f_res_26919 = x_26904;
                    }
                    
                    int64_t defunc_0_f_res_26920;
                    int64_t defunc_0_f_res_26921;
                    int64_t defunc_0_f_res_26922;
                    
                    if (cond_26911) {
                        defunc_0_f_res_26920 = (int64_t) 0;
                        defunc_0_f_res_26921 = (int64_t) 0;
                        defunc_0_f_res_26922 = (int64_t) 0;
                    } else {
                        bool cond_26923 = i_27691 == (int64_t) 0;
                        bool cond_26924 = !cond_26923;
                        bool cond_26925;
                        
                        if (cond_26924) {
                            int64_t i_26926 = sub64(i_27691, (int64_t) 1);
                            bool x_26927 = sle64((int64_t) 0, i_26926);
                            bool y_26928 = slt64(i_26926, conc_tmp_26697);
                            bool bounds_check_26929 = x_26927 && y_26928;
                            bool index_certs_26930;
                            
                            if (!bounds_check_26929) {
                                ctx->error =
                                    msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                              "Index [", i_26926,
                                              "] out of bounds for array of shape [",
                                              conc_tmp_26697, "].",
                                              "-> #0  sparse.fut:63:59-69\n   #1  sparse.fut:58:33-68:55\n   #2  sparse.fut:138:59-140:81\n   #3  sparse.fut:244:20-62\n   #4  sparse.fut:235:1-248:60\n");
                                if (memblock_unref(ctx, &mem_param_28143,
                                                   "mem_param_28143") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_param_28138,
                                                   "mem_param_28138") != 0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &defunc_0_f_res_mem_28255,
                                                   "defunc_0_f_res_mem_28255") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &defunc_0_f_res_mem_28250,
                                                   "defunc_0_f_res_mem_28250") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_f_res_mem_28451,
                                                   "unionArrays_res_f_res_mem_28451") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_f_res_mem_28446,
                                                   "unionArrays_res_f_res_mem_28446") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_f_res_mem_28441,
                                                   "unionArrays_res_f_res_mem_28441") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_f_res_mem_28436,
                                                   "unionArrays_res_f_res_mem_28436") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28113,
                                                   "mem_28113") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28111,
                                                   "mem_28111") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28109,
                                                   "mem_28109") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28107,
                                                   "mem_28107") != 0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_mem_28471,
                                                   "unionArrays_res_mem_28471") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_mem_28466,
                                                   "unionArrays_res_mem_28466") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_mem_28461,
                                                   "unionArrays_res_mem_28461") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx,
                                                   &unionArrays_res_mem_28456,
                                                   "unionArrays_res_mem_28456") !=
                                    0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28105,
                                                   "mem_28105") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28103,
                                                   "mem_28103") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &mem_28101,
                                                   "mem_28101") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &out_mem_29011,
                                                   "out_mem_29011") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &out_mem_29009,
                                                   "out_mem_29009") != 0)
                                    return 1;
                                if (memblock_unref(ctx, &out_mem_29007,
                                                   "out_mem_29007") != 0)
                                    return 1;
                                return 1;
                            }
                            
                            int64_t y_26931 = ((int64_t *) mem_28115)[i_26926];
                            bool cond_t_res_26932 = x_26904 == y_26931;
                            
                            cond_26925 = cond_t_res_26932;
                        } else {
                            cond_26925 = 0;
                        }
                        
                        int64_t defunc_0_f_res_f_res_26933;
                        
                        if (cond_26925) {
                            defunc_0_f_res_f_res_26933 = (int64_t) 2;
                        } else {
                            defunc_0_f_res_f_res_26933 = x_26905;
                        }
                        
                        int64_t defunc_0_f_res_f_res_26934;
                        int64_t defunc_0_f_res_f_res_26935;
                        
                        if (cond_26925) {
                            bool cond_26936 = x_26905 == (int64_t) 0;
                            int64_t defunc_0_f_res_f_res_t_res_26937;
                            int64_t defunc_0_f_res_f_res_t_res_26938;
                            
                            if (cond_26936) {
                                int64_t i_26939 = sub64(i_27691, (int64_t) 1);
                                bool x_26940 = sle64((int64_t) 0, i_26939);
                                bool y_26941 = slt64(i_26939, conc_tmp_26697);
                                bool bounds_check_26942 = x_26940 && y_26941;
                                bool index_certs_26943;
                                
                                if (!bounds_check_26942) {
                                    ctx->error =
                                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                                  "Index [", i_26939,
                                                  "] out of bounds for array of shape [",
                                                  conc_tmp_26697, "].",
                                                  "-> #0  sparse.fut:65:67-77\n   #1  sparse.fut:58:33-68:55\n   #2  sparse.fut:138:59-140:81\n   #3  sparse.fut:244:20-62\n   #4  sparse.fut:235:1-248:60\n");
                                    if (memblock_unref(ctx, &mem_param_28143,
                                                       "mem_param_28143") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_param_28138,
                                                       "mem_param_28138") != 0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &defunc_0_f_res_mem_28255,
                                                       "defunc_0_f_res_mem_28255") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &defunc_0_f_res_mem_28250,
                                                       "defunc_0_f_res_mem_28250") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28451,
                                                       "unionArrays_res_f_res_mem_28451") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28446,
                                                       "unionArrays_res_f_res_mem_28446") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28441,
                                                       "unionArrays_res_f_res_mem_28441") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28436,
                                                       "unionArrays_res_f_res_mem_28436") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28113,
                                                       "mem_28113") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28111,
                                                       "mem_28111") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28109,
                                                       "mem_28109") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28107,
                                                       "mem_28107") != 0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28471,
                                                       "unionArrays_res_mem_28471") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28466,
                                                       "unionArrays_res_mem_28466") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28461,
                                                       "unionArrays_res_mem_28461") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28456,
                                                       "unionArrays_res_mem_28456") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28105,
                                                       "mem_28105") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28103,
                                                       "mem_28103") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28101,
                                                       "mem_28101") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29011,
                                                       "out_mem_29011") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29009,
                                                       "out_mem_29009") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29007,
                                                       "out_mem_29007") != 0)
                                        return 1;
                                    return 1;
                                }
                                
                                int64_t defunc_0_f_res_f_res_t_res_t_res_26944 =
                                        ((int64_t *) mem_28119)[i_26939];
                                
                                defunc_0_f_res_f_res_t_res_26937 = x_26906;
                                defunc_0_f_res_f_res_t_res_26938 =
                                    defunc_0_f_res_f_res_t_res_t_res_26944;
                            } else {
                                int64_t i_26945 = sub64(i_27691, (int64_t) 1);
                                bool x_26946 = sle64((int64_t) 0, i_26945);
                                bool y_26947 = slt64(i_26945, conc_tmp_26697);
                                bool bounds_check_26948 = x_26946 && y_26947;
                                bool index_certs_26949;
                                
                                if (!bounds_check_26948) {
                                    ctx->error =
                                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                                  "Index [", i_26945,
                                                  "] out of bounds for array of shape [",
                                                  conc_tmp_26697, "].",
                                                  "-> #0  sparse.fut:67:59-69\n   #1  sparse.fut:58:33-68:55\n   #2  sparse.fut:138:59-140:81\n   #3  sparse.fut:244:20-62\n   #4  sparse.fut:235:1-248:60\n");
                                    if (memblock_unref(ctx, &mem_param_28143,
                                                       "mem_param_28143") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_param_28138,
                                                       "mem_param_28138") != 0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &defunc_0_f_res_mem_28255,
                                                       "defunc_0_f_res_mem_28255") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &defunc_0_f_res_mem_28250,
                                                       "defunc_0_f_res_mem_28250") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28451,
                                                       "unionArrays_res_f_res_mem_28451") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28446,
                                                       "unionArrays_res_f_res_mem_28446") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28441,
                                                       "unionArrays_res_f_res_mem_28441") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_f_res_mem_28436,
                                                       "unionArrays_res_f_res_mem_28436") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28113,
                                                       "mem_28113") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28111,
                                                       "mem_28111") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28109,
                                                       "mem_28109") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28107,
                                                       "mem_28107") != 0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28471,
                                                       "unionArrays_res_mem_28471") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28466,
                                                       "unionArrays_res_mem_28466") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28461,
                                                       "unionArrays_res_mem_28461") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx,
                                                       &unionArrays_res_mem_28456,
                                                       "unionArrays_res_mem_28456") !=
                                        0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28105,
                                                       "mem_28105") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28103,
                                                       "mem_28103") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &mem_28101,
                                                       "mem_28101") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29011,
                                                       "out_mem_29011") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29009,
                                                       "out_mem_29009") != 0)
                                        return 1;
                                    if (memblock_unref(ctx, &out_mem_29007,
                                                       "out_mem_29007") != 0)
                                        return 1;
                                    return 1;
                                }
                                
                                int64_t defunc_0_f_res_f_res_t_res_f_res_26950 =
                                        ((int64_t *) mem_28119)[i_26945];
                                
                                defunc_0_f_res_f_res_t_res_26937 =
                                    defunc_0_f_res_f_res_t_res_f_res_26950;
                                defunc_0_f_res_f_res_t_res_26938 = x_26906;
                            }
                            defunc_0_f_res_f_res_26934 =
                                defunc_0_f_res_f_res_t_res_26937;
                            defunc_0_f_res_f_res_26935 =
                                defunc_0_f_res_f_res_t_res_26938;
                        } else {
                            defunc_0_f_res_f_res_26934 = x_26906;
                            defunc_0_f_res_f_res_26935 = x_26907;
                        }
                        defunc_0_f_res_26920 = defunc_0_f_res_f_res_26933;
                        defunc_0_f_res_26921 = defunc_0_f_res_f_res_26934;
                        defunc_0_f_res_26922 = defunc_0_f_res_f_res_26935;
                    }
                    
                    bool eq_x_zz_26951 = (int64_t) -1 == x_26904;
                    bool not_p_26952 = !cond_26911;
                    bool p_and_eq_x_y_26953 = eq_x_zz_26951 && not_p_26952;
                    bool defunc_0_p_res_26954 = cond_26911 ||
                         p_and_eq_x_y_26953;
                    int64_t defunc_1_lifted_lambda_res_26955 =
                            btoi_bool_i64(defunc_0_p_res_26954);
                    bool is_i_26956 = defunc_1_lifted_lambda_res_26955 ==
                         (int64_t) 0;
                    bool cond_neg_26957 = !is_i_26956;
                    int64_t part_res_26958 = btoi_bool_i64(cond_neg_26957);
                    int64_t part_res_26959 = btoi_bool_i64(is_i_26956);
                    int64_t zz_26903 = part_res_26959 + scanacc_27684;
                    
                    ((int64_t *) mem_28123)[i_27691] = zz_26903;
                    ((int64_t *) mem_28125)[i_27691] = part_res_26958;
                    ((int64_t *) mem_28127)[i_27691] = defunc_0_f_res_26919;
                    ((int64_t *) mem_28129)[i_27691] = defunc_0_f_res_26920;
                    ((int64_t *) mem_28131)[i_27691] = defunc_0_f_res_26921;
                    ((int64_t *) mem_28133)[i_27691] = defunc_0_f_res_26922;
                    
                    int64_t scanacc_tmp_29061 = zz_26903;
                    
                    scanacc_27684 = scanacc_tmp_29061;
                }
                discard_27698 = scanacc_27684;
                
                int64_t partition_sizze_26966;
                
                if (cond_26710) {
                    partition_sizze_26966 = (int64_t) 0;
                } else {
                    int64_t last_offset_26967 =
                            ((int64_t *) mem_28123)[i_26713];
                    
                    partition_sizze_26966 = last_offset_26967;
                }
                
                struct memblock mem_28377;
                
                mem_28377.references = NULL;
                if (memblock_alloc(ctx, &mem_28377, bytes_28110, "mem_28377")) {
                    err = 1;
                    goto cleanup;
                }
                
                struct memblock mem_28379;
                
                mem_28379.references = NULL;
                if (memblock_alloc(ctx, &mem_28379, bytes_28110, "mem_28379")) {
                    err = 1;
                    goto cleanup;
                }
                
                struct memblock mem_28381;
                
                mem_28381.references = NULL;
                if (memblock_alloc(ctx, &mem_28381, bytes_28110, "mem_28381")) {
                    err = 1;
                    goto cleanup;
                }
                
                struct memblock mem_28383;
                
                mem_28383.references = NULL;
                if (memblock_alloc(ctx, &mem_28383, bytes_28110, "mem_28383")) {
                    err = 1;
                    goto cleanup;
                }
                for (int64_t write_iter_27699 = 0; write_iter_27699 <
                     conc_tmp_26697; write_iter_27699++) {
                    int64_t write_iv_27704 =
                            ((int64_t *) mem_28125)[write_iter_27699];
                    int64_t write_iv_27705 =
                            ((int64_t *) mem_28123)[write_iter_27699];
                    bool is_this_one_26982 = write_iv_27704 == (int64_t) 0;
                    int64_t this_offset_26983 = (int64_t) -1 + write_iv_27705;
                    int64_t total_res_26984;
                    
                    if (is_this_one_26982) {
                        total_res_26984 = this_offset_26983;
                    } else {
                        total_res_26984 = (int64_t) -1;
                    }
                    
                    bool less_than_zzero_27710 = slt64(total_res_26984,
                                                       (int64_t) 0);
                    bool greater_than_sizze_27711 = sle64(conc_tmp_26697,
                                                          total_res_26984);
                    bool outside_bounds_dim_27712 = less_than_zzero_27710 ||
                         greater_than_sizze_27711;
                    
                    if (!outside_bounds_dim_27712) {
                        memmove(mem_28377.mem + total_res_26984 * (int64_t) 8,
                                mem_28127 + write_iter_27699 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                    if (!outside_bounds_dim_27712) {
                        memmove(mem_28379.mem + total_res_26984 * (int64_t) 8,
                                mem_28129 + write_iter_27699 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                    if (!outside_bounds_dim_27712) {
                        memmove(mem_28381.mem + total_res_26984 * (int64_t) 8,
                                mem_28131 + write_iter_27699 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                    if (!outside_bounds_dim_27712) {
                        memmove(mem_28383.mem + total_res_26984 * (int64_t) 8,
                                mem_28133 + write_iter_27699 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                }
                
                bool empty_slice_26985 = partition_sizze_26966 == (int64_t) 0;
                int64_t m_26986 = sub64(partition_sizze_26966, (int64_t) 1);
                bool zzero_leq_i_p_m_t_s_26987 = sle64((int64_t) 0, m_26986);
                bool i_p_m_t_s_leq_w_26988 = slt64(m_26986, conc_tmp_26697);
                bool i_lte_j_26989 = sle64((int64_t) 0, partition_sizze_26966);
                bool y_26990 = zzero_leq_i_p_m_t_s_26987 &&
                     i_p_m_t_s_leq_w_26988;
                bool y_26991 = i_lte_j_26989 && y_26990;
                bool ok_or_empty_26992 = empty_slice_26985 || y_26991;
                bool index_certs_26993;
                
                if (!ok_or_empty_26992) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [:", partition_sizze_26966,
                                  "] out of bounds for array of shape [",
                                  conc_tmp_26697, "].",
                                  "-> #0  /prelude/soacs.fut:141:6-16\n   #1  sparse.fut:58:3-68:56\n   #2  sparse.fut:138:59-140:81\n   #3  sparse.fut:244:20-62\n   #4  sparse.fut:235:1-248:60\n");
                    if (memblock_unref(ctx, &mem_28383, "mem_28383") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28381, "mem_28381") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28379, "mem_28379") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28377, "mem_28377") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_28143,
                                       "mem_param_28143") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_28138,
                                       "mem_param_28138") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28255,
                                       "defunc_0_f_res_mem_28255") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28250,
                                       "defunc_0_f_res_mem_28250") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28451,
                                       "unionArrays_res_f_res_mem_28451") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28446,
                                       "unionArrays_res_f_res_mem_28446") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28441,
                                       "unionArrays_res_f_res_mem_28441") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28436,
                                       "unionArrays_res_f_res_mem_28436") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28113, "mem_28113") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28111, "mem_28111") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28109, "mem_28109") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28107, "mem_28107") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                       "unionArrays_res_mem_28471") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                       "unionArrays_res_mem_28466") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                       "unionArrays_res_mem_28461") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                       "unionArrays_res_mem_28456") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                sizze_26719 = partition_sizze_26966;
                sizze_26720 = partition_sizze_26966;
                sizze_26721 = partition_sizze_26966;
                sizze_26722 = partition_sizze_26966;
                unionArrays_res_f_res_ixfn_28432 = partition_sizze_26966;
                unionArrays_res_f_res_ixfn_28437 = partition_sizze_26966;
                unionArrays_res_f_res_ixfn_28442 = partition_sizze_26966;
                unionArrays_res_f_res_ixfn_28447 = partition_sizze_26966;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28436,
                                 &mem_28377, "mem_28377") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28441,
                                 &mem_28379, "mem_28379") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28446,
                                 &mem_28381, "mem_28381") != 0)
                    return 1;
                if (memblock_set(ctx, &unionArrays_res_f_res_mem_28451,
                                 &mem_28383, "mem_28383") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28383, "mem_28383") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28381, "mem_28381") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28379, "mem_28379") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28377, "mem_28377") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_param_28143, "mem_param_28143") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &mem_param_28138, "mem_param_28138") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28255,
                                   "defunc_0_f_res_mem_28255") != 0)
                    return 1;
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28250,
                                   "defunc_0_f_res_mem_28250") != 0)
                    return 1;
            }
            if (memblock_unref(ctx, &mem_28107, "mem_28107") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28109, "mem_28109") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28111, "mem_28111") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28113, "mem_28113") != 0)
                return 1;
            sizze_26698 = sizze_26719;
            sizze_26699 = sizze_26720;
            sizze_26700 = sizze_26721;
            sizze_26701 = sizze_26722;
            unionArrays_res_ixfn_28452 = unionArrays_res_f_res_ixfn_28432;
            unionArrays_res_ixfn_28453 = unionArrays_res_f_res_ixfn_28433;
            unionArrays_res_ixfn_28455 = unionArrays_res_f_res_ixfn_28435;
            unionArrays_res_ixfn_28457 = unionArrays_res_f_res_ixfn_28437;
            unionArrays_res_ixfn_28458 = unionArrays_res_f_res_ixfn_28438;
            unionArrays_res_ixfn_28462 = unionArrays_res_f_res_ixfn_28442;
            unionArrays_res_ixfn_28463 = unionArrays_res_f_res_ixfn_28443;
            unionArrays_res_ixfn_28467 = unionArrays_res_f_res_ixfn_28447;
            unionArrays_res_ixfn_28468 = unionArrays_res_f_res_ixfn_28448;
            if (memblock_set(ctx, &unionArrays_res_mem_28456,
                             &unionArrays_res_f_res_mem_28436,
                             "unionArrays_res_f_res_mem_28436") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28461,
                             &unionArrays_res_f_res_mem_28441,
                             "unionArrays_res_f_res_mem_28441") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28466,
                             &unionArrays_res_f_res_mem_28446,
                             "unionArrays_res_f_res_mem_28446") != 0)
                return 1;
            if (memblock_set(ctx, &unionArrays_res_mem_28471,
                             &unionArrays_res_f_res_mem_28451,
                             "unionArrays_res_f_res_mem_28451") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28451,
                               "unionArrays_res_f_res_mem_28451") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28446,
                               "unionArrays_res_f_res_mem_28446") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28441,
                               "unionArrays_res_f_res_mem_28441") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_f_res_mem_28436,
                               "unionArrays_res_f_res_mem_28436") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28113, "mem_28113") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28111, "mem_28111") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28109, "mem_28109") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28107, "mem_28107") != 0)
                return 1;
        }
        if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
            return 1;
        
        bool dim_match_26998 = sizze_26698 == sizze_26699;
        bool empty_or_match_cert_26999;
        
        if (!dim_match_26998) {
            ctx->error = msgprintf("Error: %s\n\nBacktrace:\n%s",
                                   "Function return value does not match shape of declared return type.",
                                   "-> #0  sparse.fut:45:7-68:56\n   #1  sparse.fut:138:59-140:81\n   #2  sparse.fut:244:20-62\n   #3  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool dim_match_27000 = sizze_26698 == sizze_26700;
        bool empty_or_match_cert_27001;
        
        if (!dim_match_27000) {
            ctx->error = msgprintf("Error: %s\n\nBacktrace:\n%s",
                                   "Function return value does not match shape of declared return type.",
                                   "-> #0  sparse.fut:45:7-68:56\n   #1  sparse.fut:138:59-140:81\n   #2  sparse.fut:244:20-62\n   #3  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool dim_match_27002 = sizze_26698 == sizze_26701;
        bool empty_or_match_cert_27003;
        
        if (!dim_match_27002) {
            ctx->error = msgprintf("Error: %s\n\nBacktrace:\n%s",
                                   "Function return value does not match shape of declared return type.",
                                   "-> #0  sparse.fut:45:7-68:56\n   #1  sparse.fut:138:59-140:81\n   #2  sparse.fut:244:20-62\n   #3  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t x_27004 = ((int64_t *) mem_27971)[i_26656];
        int64_t y_27005 = ((int64_t *) mem_27971)[i_26650];
        int64_t indxs_27006 = sub64(x_27004, y_27005);
        bool bounds_invalid_upwards_27007 = slt64(x_27004, y_27005);
        bool valid_27008 = !bounds_invalid_upwards_27007;
        bool range_valid_c_27009;
        
        if (!valid_27008) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Range ", y_27005, "..<", x_27004,
                                   " is invalid.",
                                   "-> #0  sparse.fut:142:18-42\n   #1  sparse.fut:244:20-62\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool dim_match_27011 = indxs_27006 == sizze_26698;
        bool empty_or_match_cert_27012;
        
        if (!dim_match_27011) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Value of (core language) shape (",
                                   sizze_26698,
                                   ") cannot match shape of type `[",
                                   indxs_27006, "]index`.",
                                   "-> #0  sparse.fut:146:26-51\n   #1  sparse.fut:244:20-62\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool empty_or_match_cert_27013;
        
        if (!dim_match_27011) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Value of (core language) shape (",
                                   sizze_26698,
                                   ") cannot match shape of type `[",
                                   indxs_27006, "]t`.",
                                   "-> #0  sparse.fut:147:25-154:75\n   #1  sparse.fut:244:20-62\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                               "unionArrays_res_mem_28471") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                               "unionArrays_res_mem_28466") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                               "unionArrays_res_mem_28461") != 0)
                return 1;
            if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                               "unionArrays_res_mem_28456") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        for (int64_t write_iter_27734 = 0; write_iter_27734 < indxs_27006;
             write_iter_27734++) {
            int64_t index_primexp_27940 = add64(y_27005, write_iter_27734);
            bool y_27023 = slt64(write_iter_27734, sizze_26698);
            bool index_certs_27025;
            
            if (!y_27023) {
                ctx->error =
                    msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                              "Index [", write_iter_27734,
                              "] out of bounds for array of shape [",
                              sizze_26698, "].",
                              "-> #0  sparse.fut:148:37-47\n   #1  sparse.fut:147:26-154:63\n   #2  sparse.fut:244:20-62\n   #3  sparse.fut:235:1-248:60\n");
                if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                   "unionArrays_res_mem_28471") != 0)
                    return 1;
                if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                   "unionArrays_res_mem_28466") != 0)
                    return 1;
                if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                   "unionArrays_res_mem_28461") != 0)
                    return 1;
                if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                   "unionArrays_res_mem_28456") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                    return 1;
                return 1;
            }
            
            int64_t x_27026 =
                    ((int64_t *) unionArrays_res_mem_28461.mem)[write_iter_27734];
            bool cond_27027 = x_27026 == (int64_t) 2;
            double defunc_0_f_res_27028;
            
            if (cond_27027) {
                int64_t y_27029 =
                        ((int64_t *) unionArrays_res_mem_28471.mem)[write_iter_27734];
                int64_t i_27030 = add64(i_26655, y_27029);
                bool x_27031 = sle64((int64_t) 0, i_27030);
                bool y_27032 = slt64(i_27030, k_26451);
                bool bounds_check_27033 = x_27031 && y_27032;
                bool index_certs_27034;
                
                if (!bounds_check_27033) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", i_27030,
                                  "] out of bounds for array of shape [",
                                  k_26451, "].",
                                  "-> #0  sparse.fut:150:39-85\n   #1  sparse.fut:147:26-154:63\n   #2  sparse.fut:244:20-62\n   #3  sparse.fut:235:1-248:60\n");
                    if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                       "unionArrays_res_mem_28471") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                       "unionArrays_res_mem_28466") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                       "unionArrays_res_mem_28461") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                       "unionArrays_res_mem_28456") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                
                double defunc_1_add_arg_27035 =
                       ((double *) rightValues_mem_27967.mem)[i_27030];
                int64_t y_27036 =
                        ((int64_t *) unionArrays_res_mem_28466.mem)[write_iter_27734];
                int64_t i_27037 = add64(i_26677, y_27036);
                bool x_27038 = sle64((int64_t) 0, i_27037);
                bool y_27039 = slt64(i_27037, n_26449);
                bool bounds_check_27040 = x_27038 && y_27039;
                bool index_certs_27041;
                
                if (!bounds_check_27040) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", i_27037,
                                  "] out of bounds for array of shape [",
                                  n_26449, "].",
                                  "-> #0  sparse.fut:149:46-90\n   #1  sparse.fut:147:26-154:63\n   #2  sparse.fut:244:20-62\n   #3  sparse.fut:235:1-248:60\n");
                    if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                       "unionArrays_res_mem_28471") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                       "unionArrays_res_mem_28466") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                       "unionArrays_res_mem_28461") != 0)
                        return 1;
                    if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                       "unionArrays_res_mem_28456") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                
                double defunc_0_add_arg_27042 =
                       ((double *) leftValues_mem_27964.mem)[i_27037];
                double defunc_1_add_res_27043 = defunc_1_add_arg_27035 +
                       defunc_0_add_arg_27042;
                
                defunc_0_f_res_27028 = defunc_1_add_res_27043;
            } else {
                bool cond_27044 = x_27026 == (int64_t) 0;
                double defunc_0_f_res_f_res_27045;
                
                if (cond_27044) {
                    int64_t y_27046 =
                            ((int64_t *) unionArrays_res_mem_28466.mem)[write_iter_27734];
                    int64_t i_27047 = add64(i_26677, y_27046);
                    bool x_27048 = sle64((int64_t) 0, i_27047);
                    bool y_27049 = slt64(i_27047, n_26449);
                    bool bounds_check_27050 = x_27048 && y_27049;
                    bool index_certs_27051;
                    
                    if (!bounds_check_27050) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", i_27047,
                                      "] out of bounds for array of shape [",
                                      n_26449, "].",
                                      "-> #0  sparse.fut:152:39-83\n   #1  sparse.fut:147:26-154:63\n   #2  sparse.fut:244:20-62\n   #3  sparse.fut:235:1-248:60\n");
                        if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                           "unionArrays_res_mem_28471") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                           "unionArrays_res_mem_28466") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                           "unionArrays_res_mem_28461") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                           "unionArrays_res_mem_28456") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    double defunc_0_f_res_f_res_t_res_27052 =
                           ((double *) leftValues_mem_27964.mem)[i_27047];
                    
                    defunc_0_f_res_f_res_27045 =
                        defunc_0_f_res_f_res_t_res_27052;
                } else {
                    int64_t y_27053 =
                            ((int64_t *) unionArrays_res_mem_28466.mem)[write_iter_27734];
                    int64_t i_27054 = add64(i_26655, y_27053);
                    bool x_27055 = sle64((int64_t) 0, i_27054);
                    bool y_27056 = slt64(i_27054, k_26451);
                    bool bounds_check_27057 = x_27055 && y_27056;
                    bool index_certs_27058;
                    
                    if (!bounds_check_27057) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", i_27054,
                                      "] out of bounds for array of shape [",
                                      k_26451, "].",
                                      "-> #0  sparse.fut:153:39-85\n   #1  sparse.fut:147:26-154:63\n   #2  sparse.fut:244:20-62\n   #3  sparse.fut:235:1-248:60\n");
                        if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                                           "unionArrays_res_mem_28471") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                                           "unionArrays_res_mem_28466") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                                           "unionArrays_res_mem_28461") != 0)
                            return 1;
                        if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                                           "unionArrays_res_mem_28456") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    double defunc_0_f_res_f_res_f_res_27059 =
                           ((double *) rightValues_mem_27967.mem)[i_27054];
                    
                    defunc_0_f_res_f_res_27045 =
                        defunc_0_f_res_f_res_f_res_27059;
                }
                defunc_0_f_res_27028 = defunc_0_f_res_f_res_27045;
            }
            
            bool less_than_zzero_27741 = slt64(index_primexp_27940,
                                               (int64_t) 0);
            bool greater_than_sizze_27742 = sle64(eWiseAddOffsets_res_26642,
                                                  index_primexp_27940);
            bool outside_bounds_dim_27743 = less_than_zzero_27741 ||
                 greater_than_sizze_27742;
            
            if (!outside_bounds_dim_27743) {
                ((double *) mem_28083)[index_primexp_27940] =
                    defunc_0_f_res_27028;
            }
            if (!outside_bounds_dim_27743) {
                memmove(mem_28085 + index_primexp_27940 * (int64_t) 8,
                        unionArrays_res_mem_28456.mem +
                        (unionArrays_res_ixfn_28455 + write_iter_27734) *
                        (int64_t) 8, (int64_t) sizeof(int64_t));
            }
        }
        if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                           "unionArrays_res_mem_28456") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                           "unionArrays_res_mem_28461") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                           "unionArrays_res_mem_28466") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                           "unionArrays_res_mem_28471") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28471,
                           "unionArrays_res_mem_28471") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28466,
                           "unionArrays_res_mem_28466") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28461,
                           "unionArrays_res_mem_28461") != 0)
            return 1;
        if (memblock_unref(ctx, &unionArrays_res_mem_28456,
                           "unionArrays_res_mem_28456") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_28101, "mem_28101") != 0)
            return 1;
    }
    if (mem_28503_cached_sizze_29182 < (size_t) bytes_27970) {
        mem_28503 = realloc(mem_28503, bytes_27970);
        mem_28503_cached_sizze_29182 = bytes_27970;
    }
    
    int64_t discard_27799;
    int64_t scanacc_27795 = (int64_t) 0;
    
    for (int64_t i_27797 = 0; i_27797 < m_26450; i_27797++) {
        int64_t x_27064 = ((int64_t *) mem_27971)[i_27797];
        int64_t x_27066 = ((int64_t *) rightOffsets_mem_27969.mem)[i_27797];
        bool cond_27067 = i_27797 == (int64_t) 0;
        int64_t prevx2_27068;
        
        if (cond_27067) {
            prevx2_27068 = i_27797;
        } else {
            int64_t prevx2_f_res_27069 = sub64(i_27797, (int64_t) 1);
            
            prevx2_27068 = prevx2_f_res_27069;
        }
        
        int64_t prevy2_27070;
        
        if (cond_27067) {
            prevy2_27070 = i_27797;
        } else {
            int64_t prevy2_f_res_27071 = sub64(i_27797, (int64_t) 1);
            
            prevy2_27070 = prevy2_f_res_27071;
        }
        
        bool x_27072 = sle64((int64_t) 0, prevy2_27070);
        bool y_27073 = slt64(prevy2_27070, m_26450);
        bool bounds_check_27074 = x_27072 && y_27073;
        bool index_certs_27075;
        
        if (!bounds_check_27074) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", prevy2_27070,
                                   "] out of bounds for array of shape [",
                                   m_26450, "].",
                                   "-> #0  sparse.fut:123:58-78\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:120:19-125:50\n   #4  sparse.fut:162:23-49\n   #5  sparse.fut:246:21-96\n   #6  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_27076 =
                ((int64_t *) rightOffsets_mem_27969.mem)[prevy2_27070];
        int64_t j_m_i_27077 = sub64(x_27066, i_27076);
        bool empty_slice_27078 = j_m_i_27077 == (int64_t) 0;
        int64_t m_27079 = sub64(j_m_i_27077, (int64_t) 1);
        int64_t i_p_m_t_s_27080 = add64(i_27076, m_27079);
        bool zzero_leq_i_p_m_t_s_27081 = sle64((int64_t) 0, i_p_m_t_s_27080);
        bool i_p_m_t_s_leq_w_27082 = slt64(i_p_m_t_s_27080, k_26451);
        bool zzero_lte_i_27083 = sle64((int64_t) 0, i_27076);
        bool i_lte_j_27084 = sle64(i_27076, x_27066);
        bool y_27085 = i_p_m_t_s_leq_w_27082 && zzero_lte_i_27083;
        bool y_27086 = zzero_leq_i_p_m_t_s_27081 && y_27085;
        bool y_27087 = i_lte_j_27084 && y_27086;
        bool forwards_ok_27088 = zzero_lte_i_27083 && y_27087;
        bool ok_or_empty_27089 = empty_slice_27078 || forwards_ok_27088;
        bool index_certs_27090;
        
        if (!ok_or_empty_27089) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_27076, ":", x_27066,
                          "] out of bounds for array of shape [", k_26451, "].",
                          "-> #0  sparse.fut:123:44-82\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:120:19-125:50\n   #4  sparse.fut:162:23-49\n   #5  sparse.fut:246:21-96\n   #6  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool x_27092 = sle64((int64_t) 0, prevx2_27068);
        bool y_27093 = slt64(prevx2_27068, m_26450);
        bool bounds_check_27094 = x_27092 && y_27093;
        bool index_certs_27095;
        
        if (!bounds_check_27094) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", prevx2_27068,
                                   "] out of bounds for array of shape [",
                                   m_26450, "].",
                                   "-> #0  sparse.fut:122:57-76\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:120:19-125:50\n   #4  sparse.fut:162:23-49\n   #5  sparse.fut:246:21-96\n   #6  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_27096 = ((int64_t *) mem_27971)[prevx2_27068];
        int64_t j_m_i_27097 = sub64(x_27064, i_27096);
        bool empty_slice_27098 = j_m_i_27097 == (int64_t) 0;
        int64_t m_27099 = sub64(j_m_i_27097, (int64_t) 1);
        int64_t i_p_m_t_s_27100 = add64(i_27096, m_27099);
        bool zzero_leq_i_p_m_t_s_27101 = sle64((int64_t) 0, i_p_m_t_s_27100);
        bool i_p_m_t_s_leq_w_27102 = slt64(i_p_m_t_s_27100,
                                           eWiseAddOffsets_res_26642);
        bool zzero_lte_i_27103 = sle64((int64_t) 0, i_27096);
        bool i_lte_j_27104 = sle64(i_27096, x_27064);
        bool y_27105 = i_p_m_t_s_leq_w_27102 && zzero_lte_i_27103;
        bool y_27106 = zzero_leq_i_p_m_t_s_27101 && y_27105;
        bool y_27107 = i_lte_j_27104 && y_27106;
        bool forwards_ok_27108 = zzero_lte_i_27103 && y_27107;
        bool ok_or_empty_27109 = empty_slice_27098 || forwards_ok_27108;
        bool index_certs_27110;
        
        if (!ok_or_empty_27109) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_27096, ":", x_27064,
                          "] out of bounds for array of shape [",
                          eWiseAddOffsets_res_26642, "].",
                          "-> #0  sparse.fut:122:44-80\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:120:19-125:50\n   #4  sparse.fut:162:23-49\n   #5  sparse.fut:246:21-96\n   #6  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t intersectSizze_res_27112;
        
        if (empty_slice_27098) {
            intersectSizze_res_27112 = (int64_t) 0;
        } else {
            int64_t conc_tmp_27113 = j_m_i_27077 + j_m_i_27097;
            int64_t j_m_i_27114 = sub64(conc_tmp_27113, (int64_t) 1);
            int64_t bytes_28511 = (int64_t) 8 * conc_tmp_27113;
            int64_t bytes_28528 = (int64_t) 4 * conc_tmp_27113;
            int64_t intersectSizze_res_f_res_27115;
            
            if (empty_slice_27078) {
                intersectSizze_res_f_res_27115 = (int64_t) 0;
            } else {
                struct memblock mem_28512;
                
                mem_28512.references = NULL;
                if (memblock_alloc(ctx, &mem_28512, bytes_28511, "mem_28512")) {
                    err = 1;
                    goto cleanup;
                }
                
                int64_t tmp_offs_29076 = (int64_t) 0;
                
                memmove(mem_28512.mem + tmp_offs_29076 * (int64_t) 8,
                        mem_28085 + i_27096 * (int64_t) 8, j_m_i_27097 *
                        (int64_t) sizeof(int64_t));
                tmp_offs_29076 += j_m_i_27097;
                memmove(mem_28512.mem + tmp_offs_29076 * (int64_t) 8,
                        rightColumns_mem_27968.mem + i_27076 * (int64_t) 8,
                        j_m_i_27077 * (int64_t) sizeof(int64_t));
                tmp_offs_29076 += j_m_i_27077;
                
                bool cond_27117 = conc_tmp_27113 == (int64_t) 0;
                int32_t iters_27118;
                
                if (cond_27117) {
                    iters_27118 = 0;
                } else {
                    iters_27118 = 32;
                }
                
                bool loop_nonempty_27119 = slt32(0, iters_27118);
                bool x_27120 = sle64((int64_t) 0, j_m_i_27114);
                bool y_27121 = slt64(j_m_i_27114, conc_tmp_27113);
                bool bounds_check_27122 = x_27120 && y_27121;
                bool loop_not_taken_27123 = !loop_nonempty_27119;
                bool protect_assert_disj_27124 = bounds_check_27122 ||
                     loop_not_taken_27123;
                bool index_certs_27125;
                
                if (!protect_assert_disj_27124) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", j_m_i_27114,
                                  "] out of bounds for array of shape [",
                                  conc_tmp_27113, "].",
                                  "-> #0  /prelude/array.fut:18:29-34\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:25:24-35\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:53:31-62\n   #3  sparse.fut:3:25-4:48\n   #4  sparse.fut:76:12-24\n   #5  sparse.fut:122:30-123:82\n   #6  /prelude/soacs.fut:56:19-23\n   #7  /prelude/soacs.fut:56:3-37\n   #8  sparse.fut:120:19-125:50\n   #9  sparse.fut:162:23-49\n   #10 sparse.fut:246:21-96\n   #11 sparse.fut:235:1-248:60\n");
                    if (memblock_unref(ctx, &mem_28512, "mem_28512") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                if (mem_28521_cached_sizze_29183 < (size_t) bytes_28511) {
                    mem_28521 = realloc(mem_28521, bytes_28511);
                    mem_28521_cached_sizze_29183 = bytes_28511;
                }
                if (mem_28523_cached_sizze_29184 < (size_t) bytes_28511) {
                    mem_28523 = realloc(mem_28523, bytes_28511);
                    mem_28523_cached_sizze_29184 = bytes_28511;
                }
                if (mem_28525_cached_sizze_29185 < (size_t) bytes_28511) {
                    mem_28525 = realloc(mem_28525, bytes_28511);
                    mem_28525_cached_sizze_29185 = bytes_28511;
                }
                if (mem_28527_cached_sizze_29186 < (size_t) bytes_28511) {
                    mem_28527 = realloc(mem_28527, bytes_28511);
                    mem_28527_cached_sizze_29186 = bytes_28511;
                }
                if (mem_28529_cached_sizze_29187 < (size_t) bytes_28528) {
                    mem_28529 = realloc(mem_28529, bytes_28528);
                    mem_28529_cached_sizze_29187 = bytes_28528;
                }
                
                struct memblock defunc_2_radix_sort_res_mem_28608;
                
                defunc_2_radix_sort_res_mem_28608.references = NULL;
                
                struct memblock mem_param_28517;
                
                mem_param_28517.references = NULL;
                if (memblock_set(ctx, &mem_param_28517, &mem_28512,
                                 "mem_28512") != 0)
                    return 1;
                for (int32_t i_27127 = 0; i_27127 < iters_27118; i_27127++) {
                    int32_t defunc_2_radix_sort_step_arg_27129 = mul32(2,
                                                                       i_27127);
                    int32_t defunc_0_get_bit_arg_27130 = add32(1,
                                                               defunc_2_radix_sort_step_arg_27129);
                    int64_t i32_res_27131 =
                            sext_i32_i64(defunc_0_get_bit_arg_27130);
                    int64_t i32_res_27132 =
                            sext_i32_i64(defunc_2_radix_sort_step_arg_27129);
                    int64_t discard_27773;
                    int64_t discard_27774;
                    int64_t discard_27775;
                    int64_t discard_27776;
                    int64_t scanacc_27758;
                    int64_t scanacc_27759;
                    int64_t scanacc_27760;
                    int64_t scanacc_27761;
                    
                    scanacc_27758 = (int64_t) 0;
                    scanacc_27759 = (int64_t) 0;
                    scanacc_27760 = (int64_t) 0;
                    scanacc_27761 = (int64_t) 0;
                    for (int64_t i_27767 = 0; i_27767 < conc_tmp_27113;
                         i_27767++) {
                        int64_t x_27160 =
                                ((int64_t *) mem_param_28517.mem)[i_27767];
                        int64_t zgzg_res_27161 = ashr64(x_27160, i32_res_27131);
                        int64_t za_res_27162 = (int64_t) 1 & zgzg_res_27161;
                        int32_t to_i32_res_27163 = sext_i64_i32(za_res_27162);
                        int32_t x_27164 = mul32(2, to_i32_res_27163);
                        int64_t zgzg_res_27165 = ashr64(x_27160, i32_res_27132);
                        int64_t za_res_27166 = (int64_t) 1 & zgzg_res_27165;
                        int32_t to_i32_res_27167 = sext_i64_i32(za_res_27166);
                        int32_t defunc_0_f_res_27168 = add32(x_27164,
                                                             to_i32_res_27167);
                        bool cond_27169 = defunc_0_f_res_27168 == 0;
                        int64_t defunc_0_f_res_27170 =
                                btoi_bool_i64(cond_27169);
                        int64_t defunc_0_f_res_27171;
                        int64_t defunc_0_f_res_27172;
                        int64_t defunc_0_f_res_27173;
                        
                        if (cond_27169) {
                            defunc_0_f_res_27171 = (int64_t) 0;
                            defunc_0_f_res_27172 = (int64_t) 0;
                            defunc_0_f_res_27173 = (int64_t) 0;
                        } else {
                            bool cond_27174 = defunc_0_f_res_27168 == 1;
                            int64_t defunc_0_f_res_f_res_27175 =
                                    btoi_bool_i64(cond_27174);
                            int64_t defunc_0_f_res_f_res_27176;
                            int64_t defunc_0_f_res_f_res_27177;
                            
                            if (cond_27174) {
                                defunc_0_f_res_f_res_27176 = (int64_t) 0;
                                defunc_0_f_res_f_res_27177 = (int64_t) 0;
                            } else {
                                bool cond_27178 = defunc_0_f_res_27168 == 2;
                                int64_t defunc_0_f_res_f_res_f_res_27179 =
                                        btoi_bool_i64(cond_27178);
                                bool cond_neg_27180 = !cond_27178;
                                int64_t defunc_0_f_res_f_res_f_res_27181 =
                                        btoi_bool_i64(cond_neg_27180);
                                
                                defunc_0_f_res_f_res_27176 =
                                    defunc_0_f_res_f_res_f_res_27179;
                                defunc_0_f_res_f_res_27177 =
                                    defunc_0_f_res_f_res_f_res_27181;
                            }
                            defunc_0_f_res_27171 = defunc_0_f_res_f_res_27175;
                            defunc_0_f_res_27172 = defunc_0_f_res_f_res_27176;
                            defunc_0_f_res_27173 = defunc_0_f_res_f_res_27177;
                        }
                        
                        int64_t defunc_1_op_res_27156 =
                                add64(defunc_0_f_res_27170, scanacc_27758);
                        int64_t defunc_1_op_res_27157 =
                                add64(defunc_0_f_res_27171, scanacc_27759);
                        int64_t defunc_1_op_res_27158 =
                                add64(defunc_0_f_res_27172, scanacc_27760);
                        int64_t defunc_1_op_res_27159 =
                                add64(defunc_0_f_res_27173, scanacc_27761);
                        
                        ((int64_t *) mem_28521)[i_27767] =
                            defunc_1_op_res_27156;
                        ((int64_t *) mem_28523)[i_27767] =
                            defunc_1_op_res_27157;
                        ((int64_t *) mem_28525)[i_27767] =
                            defunc_1_op_res_27158;
                        ((int64_t *) mem_28527)[i_27767] =
                            defunc_1_op_res_27159;
                        ((int32_t *) mem_28529)[i_27767] = defunc_0_f_res_27168;
                        
                        int64_t scanacc_tmp_29079 = defunc_1_op_res_27156;
                        int64_t scanacc_tmp_29080 = defunc_1_op_res_27157;
                        int64_t scanacc_tmp_29081 = defunc_1_op_res_27158;
                        int64_t scanacc_tmp_29082 = defunc_1_op_res_27159;
                        
                        scanacc_27758 = scanacc_tmp_29079;
                        scanacc_27759 = scanacc_tmp_29080;
                        scanacc_27760 = scanacc_tmp_29081;
                        scanacc_27761 = scanacc_tmp_29082;
                    }
                    discard_27773 = scanacc_27758;
                    discard_27774 = scanacc_27759;
                    discard_27775 = scanacc_27760;
                    discard_27776 = scanacc_27761;
                    
                    int64_t last_res_27187 =
                            ((int64_t *) mem_28521)[j_m_i_27114];
                    int64_t last_res_27188 =
                            ((int64_t *) mem_28523)[j_m_i_27114];
                    int64_t last_res_27189 =
                            ((int64_t *) mem_28525)[j_m_i_27114];
                    struct memblock mem_28591;
                    
                    mem_28591.references = NULL;
                    if (memblock_alloc(ctx, &mem_28591, bytes_28511,
                                       "mem_28591")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28591.mem + (int64_t) 0, mem_param_28517.mem +
                            (int64_t) 0, conc_tmp_27113 *
                            (int64_t) sizeof(int64_t));
                    for (int64_t write_iter_27777 = 0; write_iter_27777 <
                         conc_tmp_27113; write_iter_27777++) {
                        int32_t write_iv_27779 =
                                ((int32_t *) mem_28529)[write_iter_27777];
                        int64_t write_iv_27780 =
                                ((int64_t *) mem_28521)[write_iter_27777];
                        int64_t write_iv_27781 =
                                ((int64_t *) mem_28523)[write_iter_27777];
                        int64_t write_iv_27782 =
                                ((int64_t *) mem_28525)[write_iter_27777];
                        int64_t write_iv_27783 =
                                ((int64_t *) mem_28527)[write_iter_27777];
                        bool match_lit_27198 = 0 == write_iv_27779;
                        int64_t defunc_1_f_res_27199;
                        
                        if (match_lit_27198) {
                            int64_t case_res_27200 = sub64(write_iv_27780,
                                                           (int64_t) 1);
                            
                            defunc_1_f_res_27199 = case_res_27200;
                        } else {
                            bool match_lit_27201 = 1 == write_iv_27779;
                            int64_t x_27202;
                            
                            if (match_lit_27201) {
                                int64_t x_27203 = add64(last_res_27187,
                                                        write_iv_27781);
                                int64_t case_res_27204 = sub64(x_27203,
                                                               (int64_t) 1);
                                
                                x_27202 = case_res_27204;
                            } else {
                                bool match_lit_27205 = 2 == write_iv_27779;
                                int64_t x_27206;
                                
                                if (match_lit_27205) {
                                    int64_t x_27207 = add64(last_res_27187,
                                                            last_res_27188);
                                    int64_t x_27208 = add64(x_27207,
                                                            write_iv_27782);
                                    int64_t case_res_27209 = sub64(x_27208,
                                                                   (int64_t) 1);
                                    
                                    x_27206 = case_res_27209;
                                } else {
                                    int64_t x_27210 = add64(last_res_27187,
                                                            last_res_27188);
                                    int64_t x_27211 = add64(last_res_27189,
                                                            x_27210);
                                    int64_t x_27212 = add64(x_27211,
                                                            write_iv_27783);
                                    int64_t defunc_1_f_res_res_27213 =
                                            sub64(x_27212, (int64_t) 1);
                                    
                                    x_27206 = defunc_1_f_res_res_27213;
                                }
                                x_27202 = x_27206;
                            }
                            defunc_1_f_res_27199 = x_27202;
                        }
                        
                        bool less_than_zzero_27785 = slt64(defunc_1_f_res_27199,
                                                           (int64_t) 0);
                        bool greater_than_sizze_27786 = sle64(conc_tmp_27113,
                                                              defunc_1_f_res_27199);
                        bool outside_bounds_dim_27787 = less_than_zzero_27785 ||
                             greater_than_sizze_27786;
                        
                        if (!outside_bounds_dim_27787) {
                            memmove(mem_28591.mem + defunc_1_f_res_27199 *
                                    (int64_t) 8, mem_param_28517.mem +
                                    write_iter_27777 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                    }
                    
                    struct memblock mem_param_tmp_29077;
                    
                    mem_param_tmp_29077.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29077, &mem_28591,
                                     "mem_28591") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_28517,
                                     &mem_param_tmp_29077,
                                     "mem_param_tmp_29077") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29077,
                                       "mem_param_tmp_29077") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28591, "mem_28591") != 0)
                        return 1;
                }
                if (memblock_set(ctx, &defunc_2_radix_sort_res_mem_28608,
                                 &mem_param_28517, "mem_param_28517") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28512, "mem_28512") != 0)
                    return 1;
                
                bool empty_slice_27214 = j_m_i_27114 == (int64_t) 0;
                bool i_lte_j_27215 = sle64((int64_t) 1, conc_tmp_27113);
                bool y_27216 = bounds_check_27122 && i_lte_j_27215;
                bool ok_or_empty_27217 = empty_slice_27214 || y_27216;
                bool index_certs_27218;
                
                if (!ok_or_empty_27217) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", (int64_t) 1,
                                  ":] out of bounds for array of shape [",
                                  conc_tmp_27113, "].",
                                  "-> #0  sparse.fut:79:97-106\n   #1  sparse.fut:122:30-123:82\n   #2  /prelude/soacs.fut:56:19-23\n   #3  /prelude/soacs.fut:56:3-37\n   #4  sparse.fut:120:19-125:50\n   #5  sparse.fut:162:23-49\n   #6  sparse.fut:246:21-96\n   #7  sparse.fut:235:1-248:60\n");
                    if (memblock_unref(ctx, &mem_param_28517,
                                       "mem_param_28517") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28608,
                                       "defunc_2_radix_sort_res_mem_28608") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28512, "mem_28512") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                
                int64_t defunc_2_reduce_res_27221;
                int64_t redout_27791 = (int64_t) 0;
                
                for (int64_t i_27793 = 0; i_27793 < j_m_i_27114; i_27793++) {
                    int64_t slice_27944 = (int64_t) 1 + i_27793;
                    int64_t x_27236 =
                            ((int64_t *) defunc_2_radix_sort_res_mem_28608.mem)[slice_27944];
                    bool y_27229 = slt64(i_27793, conc_tmp_27113);
                    bool index_certs_27231;
                    
                    if (!y_27229) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", i_27793,
                                      "] out of bounds for array of shape [",
                                      conc_tmp_27113, "].",
                                      "-> #0  sparse.fut:79:41-51\n   #1  sparse.fut:79:4-106\n   #2  sparse.fut:122:30-123:82\n   #3  /prelude/soacs.fut:56:19-23\n   #4  /prelude/soacs.fut:56:3-37\n   #5  sparse.fut:120:19-125:50\n   #6  sparse.fut:162:23-49\n   #7  sparse.fut:246:21-96\n   #8  sparse.fut:235:1-248:60\n");
                        if (memblock_unref(ctx, &mem_param_28517,
                                           "mem_param_28517") != 0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &defunc_2_radix_sort_res_mem_28608,
                                           "defunc_2_radix_sort_res_mem_28608") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28512, "mem_28512") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    int64_t y_27232 =
                            ((int64_t *) defunc_2_radix_sort_res_mem_28608.mem)[i_27793];
                    bool cond_27233 = x_27236 == y_27232;
                    int64_t defunc_1_op_res_27234;
                    
                    if (cond_27233) {
                        int64_t defunc_1_op_res_t_res_27235 = add64((int64_t) 1,
                                                                    redout_27791);
                        
                        defunc_1_op_res_27234 = defunc_1_op_res_t_res_27235;
                    } else {
                        defunc_1_op_res_27234 = redout_27791;
                    }
                    
                    int64_t redout_tmp_29089 = defunc_1_op_res_27234;
                    
                    redout_27791 = redout_tmp_29089;
                }
                defunc_2_reduce_res_27221 = redout_27791;
                if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28608,
                                   "defunc_2_radix_sort_res_mem_28608") != 0)
                    return 1;
                intersectSizze_res_f_res_27115 = defunc_2_reduce_res_27221;
                if (memblock_unref(ctx, &mem_param_28517, "mem_param_28517") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28608,
                                   "defunc_2_radix_sort_res_mem_28608") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28512, "mem_28512") != 0)
                    return 1;
            }
            intersectSizze_res_27112 = intersectSizze_res_f_res_27115;
        }
        
        int64_t defunc_1_op_res_27063 = add64(intersectSizze_res_27112,
                                              scanacc_27795);
        
        ((int64_t *) mem_28503)[i_27797] = defunc_1_op_res_27063;
        
        int64_t scanacc_tmp_29074 = defunc_1_op_res_27063;
        
        scanacc_27795 = scanacc_tmp_29074;
    }
    discard_27799 = scanacc_27795;
    
    int64_t eWiseMultOffsets_res_27238 = ((int64_t *) mem_28503)[i_26637];
    bool bounds_invalid_upwards_27239 = slt64(eWiseMultOffsets_res_27238,
                                              (int64_t) 0);
    bool valid_27240 = !bounds_invalid_upwards_27239;
    bool range_valid_c_27241;
    
    if (!valid_27240) {
        ctx->error = msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                               "Range ", (int64_t) 0, "..", (int64_t) 1, "..<",
                               eWiseMultOffsets_res_27238, " is invalid.",
                               "-> #0  /prelude/array.fut:60:3-10\n   #1  /prelude/array.fut:70:18-23\n   #2  sparse.fut:163:16-35\n   #3  sparse.fut:246:21-96\n   #4  sparse.fut:235:1-248:60\n");
        if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
            return 1;
        return 1;
    }
    
    int64_t bytes_28614 = (int64_t) 8 * eWiseMultOffsets_res_27238;
    
    if (mem_28615_cached_sizze_29188 < (size_t) bytes_28614) {
        mem_28615 = realloc(mem_28615, bytes_28614);
        mem_28615_cached_sizze_29188 = bytes_28614;
    }
    for (int64_t i_29090 = 0; i_29090 < eWiseMultOffsets_res_27238; i_29090++) {
        ((double *) mem_28615)[i_29090] = 0.0;
    }
    if (mem_28617_cached_sizze_29189 < (size_t) bytes_28614) {
        mem_28617 = realloc(mem_28617, bytes_28614);
        mem_28617_cached_sizze_29189 = bytes_28614;
    }
    for (int64_t i_29091 = 0; i_29091 < eWiseMultOffsets_res_27238; i_29091++) {
        ((int64_t *) mem_28617)[i_29091] = (int64_t) 0;
    }
    
    struct memblock mem_28619;
    
    mem_28619.references = NULL;
    if (memblock_alloc(ctx, &mem_28619, (int64_t) 0, "mem_28619")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_27247 = 0; i_27247 < i_26637; i_27247++) {
        bool y_27250 = slt64(i_27247, m_26450);
        bool index_certs_27251;
        
        if (!y_27250) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", i_27247,
                                   "] out of bounds for array of shape [",
                                   m_26450, "].",
                                   "-> #0  sparse.fut:171:46-61\n   #1  sparse.fut:246:21-96\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_27252 = ((int64_t *) rightOffsets_mem_27969.mem)[i_27247];
        int64_t i_27253 = add64((int64_t) 1, i_27247);
        bool x_27254 = sle64((int64_t) 0, i_27253);
        bool y_27255 = slt64(i_27253, m_26450);
        bool bounds_check_27256 = x_27254 && y_27255;
        bool index_certs_27257;
        
        if (!bounds_check_27256) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", i_27253,
                                   "] out of bounds for array of shape [",
                                   m_26450, "].",
                                   "-> #0  sparse.fut:171:63-80\n   #1  sparse.fut:246:21-96\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t j_27258 = ((int64_t *) rightOffsets_mem_27969.mem)[i_27253];
        int64_t j_m_i_27259 = sub64(j_27258, i_27252);
        bool empty_slice_27260 = j_m_i_27259 == (int64_t) 0;
        int64_t m_27261 = sub64(j_m_i_27259, (int64_t) 1);
        int64_t i_p_m_t_s_27262 = add64(i_27252, m_27261);
        bool zzero_leq_i_p_m_t_s_27263 = sle64((int64_t) 0, i_p_m_t_s_27262);
        bool i_p_m_t_s_leq_w_27264 = slt64(i_p_m_t_s_27262, k_26451);
        bool zzero_lte_i_27265 = sle64((int64_t) 0, i_27252);
        bool i_lte_j_27266 = sle64(i_27252, j_27258);
        bool y_27267 = i_p_m_t_s_leq_w_27264 && zzero_lte_i_27265;
        bool y_27268 = zzero_leq_i_p_m_t_s_27263 && y_27267;
        bool y_27269 = i_lte_j_27266 && y_27268;
        bool forwards_ok_27270 = zzero_lte_i_27265 && y_27269;
        bool ok_or_empty_27271 = empty_slice_27260 || forwards_ok_27270;
        bool index_certs_27272;
        
        if (!ok_or_empty_27271) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_27252, ":", j_27258,
                          "] out of bounds for array of shape [", k_26451, "].",
                          "-> #0  sparse.fut:171:32-81\n   #1  sparse.fut:246:21-96\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_27274 = ((int64_t *) mem_27971)[i_27247];
        int64_t j_27275 = ((int64_t *) mem_27971)[i_27253];
        int64_t j_m_i_27276 = sub64(j_27275, i_27274);
        bool empty_slice_27277 = j_m_i_27276 == (int64_t) 0;
        int64_t m_27278 = sub64(j_m_i_27276, (int64_t) 1);
        int64_t i_p_m_t_s_27279 = add64(i_27274, m_27278);
        bool zzero_leq_i_p_m_t_s_27280 = sle64((int64_t) 0, i_p_m_t_s_27279);
        bool i_p_m_t_s_leq_w_27281 = slt64(i_p_m_t_s_27279,
                                           eWiseAddOffsets_res_26642);
        bool zzero_lte_i_27282 = sle64((int64_t) 0, i_27274);
        bool i_lte_j_27283 = sle64(i_27274, j_27275);
        bool y_27284 = i_p_m_t_s_leq_w_27281 && zzero_lte_i_27282;
        bool y_27285 = zzero_leq_i_p_m_t_s_27280 && y_27284;
        bool y_27286 = i_lte_j_27283 && y_27285;
        bool forwards_ok_27287 = zzero_lte_i_27282 && y_27286;
        bool ok_or_empty_27288 = empty_slice_27277 || forwards_ok_27287;
        bool index_certs_27289;
        
        if (!ok_or_empty_27288) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_27274, ":", j_27275,
                          "] out of bounds for array of shape [",
                          eWiseAddOffsets_res_26642, "].",
                          "-> #0  sparse.fut:170:32-78\n   #1  sparse.fut:246:21-96\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t conc_tmp_27291 = j_m_i_27259 + j_m_i_27276;
        int64_t j_m_i_27292 = sub64(conc_tmp_27291, (int64_t) 1);
        int64_t bytes_28634 = (int64_t) 8 * conc_tmp_27291;
        int64_t sizze_27293;
        int64_t sizze_27294;
        int64_t sizze_27295;
        int64_t intersectArrays_res_ixfn_28929;
        int64_t intersectArrays_res_ixfn_28930;
        int64_t intersectArrays_res_ixfn_28934;
        int64_t intersectArrays_res_ixfn_28935;
        int64_t intersectArrays_res_ixfn_28939;
        int64_t intersectArrays_res_ixfn_28940;
        struct memblock intersectArrays_res_mem_28933;
        
        intersectArrays_res_mem_28933.references = NULL;
        
        struct memblock intersectArrays_res_mem_28938;
        
        intersectArrays_res_mem_28938.references = NULL;
        
        struct memblock intersectArrays_res_mem_28943;
        
        intersectArrays_res_mem_28943.references = NULL;
        if (empty_slice_27277) {
            sizze_27293 = (int64_t) 0;
            sizze_27294 = (int64_t) 0;
            sizze_27295 = (int64_t) 0;
            intersectArrays_res_ixfn_28929 = (int64_t) 0;
            intersectArrays_res_ixfn_28930 = (int64_t) 0;
            intersectArrays_res_ixfn_28934 = (int64_t) 0;
            intersectArrays_res_ixfn_28935 = (int64_t) 0;
            intersectArrays_res_ixfn_28939 = (int64_t) 0;
            intersectArrays_res_ixfn_28940 = (int64_t) 0;
            if (memblock_set(ctx, &intersectArrays_res_mem_28933, &mem_28619,
                             "mem_28619") != 0)
                return 1;
            if (memblock_set(ctx, &intersectArrays_res_mem_28938, &mem_28619,
                             "mem_28619") != 0)
                return 1;
            if (memblock_set(ctx, &intersectArrays_res_mem_28943, &mem_28619,
                             "mem_28619") != 0)
                return 1;
        } else {
            struct memblock mem_28635;
            
            mem_28635.references = NULL;
            if (memblock_alloc(ctx, &mem_28635, bytes_28634, "mem_28635")) {
                err = 1;
                goto cleanup;
            }
            
            int64_t tmp_offs_29094 = (int64_t) 0;
            
            memmove(mem_28635.mem + tmp_offs_29094 * (int64_t) 8, mem_28085 +
                    i_27274 * (int64_t) 8, j_m_i_27276 *
                    (int64_t) sizeof(int64_t));
            tmp_offs_29094 += j_m_i_27276;
            memmove(mem_28635.mem + tmp_offs_29094 * (int64_t) 8,
                    rightColumns_mem_27968.mem + i_27252 * (int64_t) 8,
                    j_m_i_27259 * (int64_t) sizeof(int64_t));
            tmp_offs_29094 += j_m_i_27259;
            
            struct memblock mem_28637;
            
            mem_28637.references = NULL;
            if (memblock_alloc(ctx, &mem_28637, bytes_28634, "mem_28637")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_29095 = 0; i_29095 < conc_tmp_27291; i_29095++) {
                int64_t x_29096 = (int64_t) 0 + i_29095 * (int64_t) 1;
                
                ((int64_t *) mem_28637.mem)[i_29095] = x_29096;
            }
            
            bool cond_27301 = conc_tmp_27291 == (int64_t) 0;
            int32_t iters_27302;
            
            if (cond_27301) {
                iters_27302 = 0;
            } else {
                iters_27302 = 32;
            }
            
            bool loop_nonempty_27303 = slt32(0, iters_27302);
            bool x_27304 = sle64((int64_t) 0, j_m_i_27292);
            bool y_27305 = slt64(j_m_i_27292, conc_tmp_27291);
            bool bounds_check_27306 = x_27304 && y_27305;
            bool loop_not_taken_27307 = !loop_nonempty_27303;
            bool protect_assert_disj_27308 = bounds_check_27306 ||
                 loop_not_taken_27307;
            int64_t last_index_27310 = j_m_i_27292 - (int64_t) 1;
            bool is_empty_27311 = j_m_i_27292 == (int64_t) 0;
            
            if (mem_28639_cached_sizze_29190 < (size_t) bytes_28634) {
                mem_28639 = realloc(mem_28639, bytes_28634);
                mem_28639_cached_sizze_29190 = bytes_28634;
            }
            if (mem_28641_cached_sizze_29191 < (size_t) bytes_28634) {
                mem_28641 = realloc(mem_28641, bytes_28634);
                mem_28641_cached_sizze_29191 = bytes_28634;
            }
            if (mem_28643_cached_sizze_29192 < (size_t) bytes_28634) {
                mem_28643 = realloc(mem_28643, bytes_28634);
                mem_28643_cached_sizze_29192 = bytes_28634;
            }
            
            bool i_lte_j_27433 = sle64((int64_t) 1, conc_tmp_27291);
            bool y_27434 = bounds_check_27306 && i_lte_j_27433;
            bool ok_or_empty_27435 = is_empty_27311 || y_27434;
            int64_t bytes_28666 = (int64_t) 4 * conc_tmp_27291;
            int64_t intersectArrays_res_f_res_ixfn_28915;
            
            if (empty_slice_27260) {
                intersectArrays_res_f_res_ixfn_28915 = (int64_t) 0;
            } else {
                intersectArrays_res_f_res_ixfn_28915 = j_m_i_27292;
            }
            
            int64_t intersectArrays_res_f_res_ixfn_28920;
            
            if (empty_slice_27260) {
                intersectArrays_res_f_res_ixfn_28920 = (int64_t) 0;
            } else {
                intersectArrays_res_f_res_ixfn_28920 = j_m_i_27292;
            }
            
            int64_t intersectArrays_res_f_res_ixfn_28925;
            
            if (empty_slice_27260) {
                intersectArrays_res_f_res_ixfn_28925 = (int64_t) 0;
            } else {
                intersectArrays_res_f_res_ixfn_28925 = j_m_i_27292;
            }
            
            int64_t sizze_27312;
            int64_t sizze_27313;
            int64_t sizze_27314;
            int64_t intersectArrays_res_f_res_ixfn_28914;
            int64_t intersectArrays_res_f_res_ixfn_28919;
            int64_t intersectArrays_res_f_res_ixfn_28924;
            struct memblock intersectArrays_res_f_res_mem_28918;
            
            intersectArrays_res_f_res_mem_28918.references = NULL;
            
            struct memblock intersectArrays_res_f_res_mem_28923;
            
            intersectArrays_res_f_res_mem_28923.references = NULL;
            
            struct memblock intersectArrays_res_f_res_mem_28928;
            
            intersectArrays_res_f_res_mem_28928.references = NULL;
            if (empty_slice_27260) {
                sizze_27312 = (int64_t) 0;
                sizze_27313 = (int64_t) 0;
                sizze_27314 = (int64_t) 0;
                intersectArrays_res_f_res_ixfn_28914 = (int64_t) 0;
                intersectArrays_res_f_res_ixfn_28919 = (int64_t) 0;
                intersectArrays_res_f_res_ixfn_28924 = (int64_t) 0;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28918,
                                 &mem_28619, "mem_28619") != 0)
                    return 1;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28923,
                                 &mem_28619, "mem_28619") != 0)
                    return 1;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28928,
                                 &mem_28619, "mem_28619") != 0)
                    return 1;
            } else {
                bool index_certs_27318;
                
                if (!protect_assert_disj_27308) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", j_m_i_27292,
                                  "] out of bounds for array of shape [",
                                  conc_tmp_27291, "].",
                                  "-> #0  /prelude/array.fut:18:29-34\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:25:24-35\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:53:31-62\n   #3  /prelude/functional.fut:9:42-44\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:59:3-61:47\n   #5  lib/github.com/diku-dk/sorts/radix_sort.fut:68:3-51\n   #6  sparse.fut:5:34-6:77\n   #7  sparse.fut:86:12-35\n   #8  sparse.fut:169:49-171:81\n   #9  sparse.fut:246:21-96\n   #10 sparse.fut:235:1-248:60\n");
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28928,
                                       "intersectArrays_res_f_res_mem_28928") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28923,
                                       "intersectArrays_res_f_res_mem_28923") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28918,
                                       "intersectArrays_res_f_res_mem_28918") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28637, "mem_28637") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28635, "mem_28635") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                                       "intersectArrays_res_mem_28943") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                                       "intersectArrays_res_mem_28938") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                                       "intersectArrays_res_mem_28933") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                if (mem_28659_cached_sizze_29193 < (size_t) bytes_28634) {
                    mem_28659 = realloc(mem_28659, bytes_28634);
                    mem_28659_cached_sizze_29193 = bytes_28634;
                }
                if (mem_28661_cached_sizze_29194 < (size_t) bytes_28634) {
                    mem_28661 = realloc(mem_28661, bytes_28634);
                    mem_28661_cached_sizze_29194 = bytes_28634;
                }
                if (mem_28663_cached_sizze_29195 < (size_t) bytes_28634) {
                    mem_28663 = realloc(mem_28663, bytes_28634);
                    mem_28663_cached_sizze_29195 = bytes_28634;
                }
                if (mem_28665_cached_sizze_29196 < (size_t) bytes_28634) {
                    mem_28665 = realloc(mem_28665, bytes_28634);
                    mem_28665_cached_sizze_29196 = bytes_28634;
                }
                if (mem_28667_cached_sizze_29197 < (size_t) bytes_28666) {
                    mem_28667 = realloc(mem_28667, bytes_28666);
                    mem_28667_cached_sizze_29197 = bytes_28666;
                }
                
                struct memblock defunc_0_f_res_mem_28760;
                
                defunc_0_f_res_mem_28760.references = NULL;
                
                struct memblock defunc_0_f_res_mem_28765;
                
                defunc_0_f_res_mem_28765.references = NULL;
                
                struct memblock mem_param_28648;
                
                mem_param_28648.references = NULL;
                
                struct memblock mem_param_28653;
                
                mem_param_28653.references = NULL;
                if (memblock_set(ctx, &mem_param_28648, &mem_28635,
                                 "mem_28635") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_28653, &mem_28637,
                                 "mem_28637") != 0)
                    return 1;
                for (int32_t i_27321 = 0; i_27321 < iters_27302; i_27321++) {
                    int32_t defunc_2_radix_sort_step_arg_27324 = mul32(2,
                                                                       i_27321);
                    int32_t defunc_0_get_bit_arg_27325 = add32(1,
                                                               defunc_2_radix_sort_step_arg_27324);
                    int64_t i32_res_27326 =
                            sext_i32_i64(defunc_0_get_bit_arg_27325);
                    int64_t i32_res_27327 =
                            sext_i32_i64(defunc_2_radix_sort_step_arg_27324);
                    int64_t discard_27820;
                    int64_t discard_27821;
                    int64_t discard_27822;
                    int64_t discard_27823;
                    int64_t scanacc_27805;
                    int64_t scanacc_27806;
                    int64_t scanacc_27807;
                    int64_t scanacc_27808;
                    
                    scanacc_27805 = (int64_t) 0;
                    scanacc_27806 = (int64_t) 0;
                    scanacc_27807 = (int64_t) 0;
                    scanacc_27808 = (int64_t) 0;
                    for (int64_t i_27814 = 0; i_27814 < conc_tmp_27291;
                         i_27814++) {
                        int64_t x_27355 =
                                ((int64_t *) mem_param_28648.mem)[i_27814];
                        int64_t zgzg_res_27356 = ashr64(x_27355, i32_res_27326);
                        int64_t za_res_27357 = (int64_t) 1 & zgzg_res_27356;
                        int32_t to_i32_res_27358 = sext_i64_i32(za_res_27357);
                        int32_t x_27359 = mul32(2, to_i32_res_27358);
                        int64_t zgzg_res_27360 = ashr64(x_27355, i32_res_27327);
                        int64_t za_res_27361 = (int64_t) 1 & zgzg_res_27360;
                        int32_t to_i32_res_27362 = sext_i64_i32(za_res_27361);
                        int32_t defunc_0_f_res_27363 = add32(x_27359,
                                                             to_i32_res_27362);
                        bool cond_27364 = defunc_0_f_res_27363 == 0;
                        int64_t defunc_0_f_res_27365 =
                                btoi_bool_i64(cond_27364);
                        int64_t defunc_0_f_res_27366;
                        int64_t defunc_0_f_res_27367;
                        int64_t defunc_0_f_res_27368;
                        
                        if (cond_27364) {
                            defunc_0_f_res_27366 = (int64_t) 0;
                            defunc_0_f_res_27367 = (int64_t) 0;
                            defunc_0_f_res_27368 = (int64_t) 0;
                        } else {
                            bool cond_27369 = defunc_0_f_res_27363 == 1;
                            int64_t defunc_0_f_res_f_res_27370 =
                                    btoi_bool_i64(cond_27369);
                            int64_t defunc_0_f_res_f_res_27371;
                            int64_t defunc_0_f_res_f_res_27372;
                            
                            if (cond_27369) {
                                defunc_0_f_res_f_res_27371 = (int64_t) 0;
                                defunc_0_f_res_f_res_27372 = (int64_t) 0;
                            } else {
                                bool cond_27373 = defunc_0_f_res_27363 == 2;
                                int64_t defunc_0_f_res_f_res_f_res_27374 =
                                        btoi_bool_i64(cond_27373);
                                bool cond_neg_27375 = !cond_27373;
                                int64_t defunc_0_f_res_f_res_f_res_27376 =
                                        btoi_bool_i64(cond_neg_27375);
                                
                                defunc_0_f_res_f_res_27371 =
                                    defunc_0_f_res_f_res_f_res_27374;
                                defunc_0_f_res_f_res_27372 =
                                    defunc_0_f_res_f_res_f_res_27376;
                            }
                            defunc_0_f_res_27366 = defunc_0_f_res_f_res_27370;
                            defunc_0_f_res_27367 = defunc_0_f_res_f_res_27371;
                            defunc_0_f_res_27368 = defunc_0_f_res_f_res_27372;
                        }
                        
                        int64_t defunc_1_op_res_27351 =
                                add64(defunc_0_f_res_27365, scanacc_27805);
                        int64_t defunc_1_op_res_27352 =
                                add64(defunc_0_f_res_27366, scanacc_27806);
                        int64_t defunc_1_op_res_27353 =
                                add64(defunc_0_f_res_27367, scanacc_27807);
                        int64_t defunc_1_op_res_27354 =
                                add64(defunc_0_f_res_27368, scanacc_27808);
                        
                        ((int64_t *) mem_28659)[i_27814] =
                            defunc_1_op_res_27351;
                        ((int64_t *) mem_28661)[i_27814] =
                            defunc_1_op_res_27352;
                        ((int64_t *) mem_28663)[i_27814] =
                            defunc_1_op_res_27353;
                        ((int64_t *) mem_28665)[i_27814] =
                            defunc_1_op_res_27354;
                        ((int32_t *) mem_28667)[i_27814] = defunc_0_f_res_27363;
                        
                        int64_t scanacc_tmp_29101 = defunc_1_op_res_27351;
                        int64_t scanacc_tmp_29102 = defunc_1_op_res_27352;
                        int64_t scanacc_tmp_29103 = defunc_1_op_res_27353;
                        int64_t scanacc_tmp_29104 = defunc_1_op_res_27354;
                        
                        scanacc_27805 = scanacc_tmp_29101;
                        scanacc_27806 = scanacc_tmp_29102;
                        scanacc_27807 = scanacc_tmp_29103;
                        scanacc_27808 = scanacc_tmp_29104;
                    }
                    discard_27820 = scanacc_27805;
                    discard_27821 = scanacc_27806;
                    discard_27822 = scanacc_27807;
                    discard_27823 = scanacc_27808;
                    
                    int64_t last_res_27382 =
                            ((int64_t *) mem_28659)[j_m_i_27292];
                    int64_t last_res_27383 =
                            ((int64_t *) mem_28661)[j_m_i_27292];
                    int64_t last_res_27384 =
                            ((int64_t *) mem_28663)[j_m_i_27292];
                    struct memblock mem_28729;
                    
                    mem_28729.references = NULL;
                    if (memblock_alloc(ctx, &mem_28729, bytes_28634,
                                       "mem_28729")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28729.mem + (int64_t) 0, mem_param_28648.mem +
                            (int64_t) 0, conc_tmp_27291 *
                            (int64_t) sizeof(int64_t));
                    
                    struct memblock mem_28731;
                    
                    mem_28731.references = NULL;
                    if (memblock_alloc(ctx, &mem_28731, bytes_28634,
                                       "mem_28731")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28731.mem + (int64_t) 0, mem_param_28653.mem +
                            (int64_t) 0, conc_tmp_27291 *
                            (int64_t) sizeof(int64_t));
                    for (int64_t write_iter_27824 = 0; write_iter_27824 <
                         conc_tmp_27291; write_iter_27824++) {
                        int32_t write_iv_27827 =
                                ((int32_t *) mem_28667)[write_iter_27824];
                        int64_t write_iv_27828 =
                                ((int64_t *) mem_28659)[write_iter_27824];
                        int64_t write_iv_27829 =
                                ((int64_t *) mem_28661)[write_iter_27824];
                        int64_t write_iv_27830 =
                                ((int64_t *) mem_28663)[write_iter_27824];
                        int64_t write_iv_27831 =
                                ((int64_t *) mem_28665)[write_iter_27824];
                        bool match_lit_27396 = 0 == write_iv_27827;
                        int64_t defunc_1_f_res_27397;
                        
                        if (match_lit_27396) {
                            int64_t case_res_27398 = sub64(write_iv_27828,
                                                           (int64_t) 1);
                            
                            defunc_1_f_res_27397 = case_res_27398;
                        } else {
                            bool match_lit_27399 = 1 == write_iv_27827;
                            int64_t x_27400;
                            
                            if (match_lit_27399) {
                                int64_t x_27401 = add64(last_res_27382,
                                                        write_iv_27829);
                                int64_t case_res_27402 = sub64(x_27401,
                                                               (int64_t) 1);
                                
                                x_27400 = case_res_27402;
                            } else {
                                bool match_lit_27403 = 2 == write_iv_27827;
                                int64_t x_27404;
                                
                                if (match_lit_27403) {
                                    int64_t x_27405 = add64(last_res_27382,
                                                            last_res_27383);
                                    int64_t x_27406 = add64(x_27405,
                                                            write_iv_27830);
                                    int64_t case_res_27407 = sub64(x_27406,
                                                                   (int64_t) 1);
                                    
                                    x_27404 = case_res_27407;
                                } else {
                                    int64_t x_27408 = add64(last_res_27382,
                                                            last_res_27383);
                                    int64_t x_27409 = add64(last_res_27384,
                                                            x_27408);
                                    int64_t x_27410 = add64(x_27409,
                                                            write_iv_27831);
                                    int64_t defunc_1_f_res_res_27411 =
                                            sub64(x_27410, (int64_t) 1);
                                    
                                    x_27404 = defunc_1_f_res_res_27411;
                                }
                                x_27400 = x_27404;
                            }
                            defunc_1_f_res_27397 = x_27400;
                        }
                        
                        bool less_than_zzero_27834 = slt64(defunc_1_f_res_27397,
                                                           (int64_t) 0);
                        bool greater_than_sizze_27835 = sle64(conc_tmp_27291,
                                                              defunc_1_f_res_27397);
                        bool outside_bounds_dim_27836 = less_than_zzero_27834 ||
                             greater_than_sizze_27835;
                        
                        if (!outside_bounds_dim_27836) {
                            memmove(mem_28729.mem + defunc_1_f_res_27397 *
                                    (int64_t) 8, mem_param_28648.mem +
                                    write_iter_27824 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                        if (!outside_bounds_dim_27836) {
                            memmove(mem_28731.mem + defunc_1_f_res_27397 *
                                    (int64_t) 8, mem_param_28653.mem +
                                    write_iter_27824 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                    }
                    
                    struct memblock mem_param_tmp_29097;
                    
                    mem_param_tmp_29097.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29097, &mem_28729,
                                     "mem_28729") != 0)
                        return 1;
                    
                    struct memblock mem_param_tmp_29098;
                    
                    mem_param_tmp_29098.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29098, &mem_28731,
                                     "mem_28731") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_28648,
                                     &mem_param_tmp_29097,
                                     "mem_param_tmp_29097") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_28653,
                                     &mem_param_tmp_29098,
                                     "mem_param_tmp_29098") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29098,
                                       "mem_param_tmp_29098") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29097,
                                       "mem_param_tmp_29097") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28731, "mem_28731") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28729, "mem_28729") != 0)
                        return 1;
                }
                if (memblock_set(ctx, &defunc_0_f_res_mem_28760,
                                 &mem_param_28648, "mem_param_28648") != 0)
                    return 1;
                if (memblock_set(ctx, &defunc_0_f_res_mem_28765,
                                 &mem_param_28653, "mem_param_28653") != 0)
                    return 1;
                for (int64_t i_27852 = 0; i_27852 < conc_tmp_27291; i_27852++) {
                    int64_t x_27415 =
                            ((int64_t *) defunc_0_f_res_mem_28765.mem)[i_27852];
                    bool x_27416 = sle64((int64_t) 0, x_27415);
                    bool y_27417 = slt64(x_27415, conc_tmp_27291);
                    bool bounds_check_27418 = x_27416 && y_27417;
                    bool index_certs_27419;
                    
                    if (!bounds_check_27418) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", x_27415,
                                      "] out of bounds for array of shape [",
                                      conc_tmp_27291, "].",
                                      "-> #0  lib/github.com/diku-dk/sorts/radix_sort.fut:62:28-32\n   #1  /prelude/functional.fut:9:42-44\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:59:3-62:33\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:68:3-51\n   #4  sparse.fut:5:34-6:77\n   #5  sparse.fut:86:12-35\n   #6  sparse.fut:169:49-171:81\n   #7  sparse.fut:246:21-96\n   #8  sparse.fut:235:1-248:60\n");
                        if (memblock_unref(ctx, &mem_param_28653,
                                           "mem_param_28653") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_param_28648,
                                           "mem_param_28648") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28765,
                                           "defunc_0_f_res_mem_28765") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28760,
                                           "defunc_0_f_res_mem_28760") != 0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28928,
                                           "intersectArrays_res_f_res_mem_28928") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28923,
                                           "intersectArrays_res_f_res_mem_28923") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28918,
                                           "intersectArrays_res_f_res_mem_28918") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28637, "mem_28637") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28635, "mem_28635") != 0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                                           "intersectArrays_res_mem_28943") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                                           "intersectArrays_res_mem_28938") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                                           "intersectArrays_res_mem_28933") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    bool index_concat_cmp_27420 = sle64(j_m_i_27276, x_27415);
                    int64_t index_concat_branch_27421;
                    
                    if (index_concat_cmp_27420) {
                        int64_t index_concat_i_27422 = sub64(x_27415,
                                                             j_m_i_27276);
                        int64_t slice_27423 = i_27252 + index_concat_i_27422;
                        int64_t index_concat_27424 =
                                ((int64_t *) rightColumns_mem_27968.mem)[slice_27423];
                        
                        index_concat_branch_27421 = index_concat_27424;
                    } else {
                        int64_t slice_27425 = i_27274 + x_27415;
                        int64_t index_concat_27426 =
                                ((int64_t *) mem_28085)[slice_27425];
                        
                        index_concat_branch_27421 = index_concat_27426;
                    }
                    
                    int64_t index_concat_branch_27428 =
                            btoi_bool_i64(index_concat_cmp_27420);
                    int64_t index_concat_branch_27430;
                    
                    if (index_concat_cmp_27420) {
                        int64_t index_concat_i_27431 = sub64(x_27415,
                                                             j_m_i_27276);
                        
                        index_concat_branch_27430 = index_concat_i_27431;
                    } else {
                        index_concat_branch_27430 = x_27415;
                    }
                    ((int64_t *) mem_28639)[i_27852] =
                        index_concat_branch_27421;
                    ((int64_t *) mem_28641)[i_27852] =
                        index_concat_branch_27428;
                    ((int64_t *) mem_28643)[i_27852] =
                        index_concat_branch_27430;
                }
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28765,
                                   "defunc_0_f_res_mem_28765") != 0)
                    return 1;
                
                bool index_certs_27436;
                
                if (!ok_or_empty_27435) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", (int64_t) 1,
                                  ":] out of bounds for array of shape [",
                                  conc_tmp_27291, "].",
                                  "-> #0  sparse.fut:100:59-68\n   #1  sparse.fut:169:49-171:81\n   #2  sparse.fut:246:21-96\n   #3  sparse.fut:235:1-248:60\n");
                    if (memblock_unref(ctx, &mem_param_28653,
                                       "mem_param_28653") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_28648,
                                       "mem_param_28648") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28765,
                                       "defunc_0_f_res_mem_28765") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28760,
                                       "defunc_0_f_res_mem_28760") != 0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28928,
                                       "intersectArrays_res_f_res_mem_28928") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28923,
                                       "intersectArrays_res_f_res_mem_28923") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28918,
                                       "intersectArrays_res_f_res_mem_28918") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28637, "mem_28637") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28635, "mem_28635") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                                       "intersectArrays_res_mem_28943") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                                       "intersectArrays_res_mem_28938") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                                       "intersectArrays_res_mem_28933") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                
                int64_t bytes_28802 = (int64_t) 8 * j_m_i_27292;
                
                if (mem_28803_cached_sizze_29198 < (size_t) bytes_28802) {
                    mem_28803 = realloc(mem_28803, bytes_28802);
                    mem_28803_cached_sizze_29198 = bytes_28802;
                }
                if (mem_28805_cached_sizze_29199 < (size_t) bytes_28802) {
                    mem_28805 = realloc(mem_28805, bytes_28802);
                    mem_28805_cached_sizze_29199 = bytes_28802;
                }
                if (mem_28807_cached_sizze_29200 < (size_t) bytes_28802) {
                    mem_28807 = realloc(mem_28807, bytes_28802);
                    mem_28807_cached_sizze_29200 = bytes_28802;
                }
                if (mem_28809_cached_sizze_29201 < (size_t) bytes_28802) {
                    mem_28809 = realloc(mem_28809, bytes_28802);
                    mem_28809_cached_sizze_29201 = bytes_28802;
                }
                if (mem_28811_cached_sizze_29202 < (size_t) bytes_28802) {
                    mem_28811 = realloc(mem_28811, bytes_28802);
                    mem_28811_cached_sizze_29202 = bytes_28802;
                }
                
                int64_t discard_27873;
                int64_t scanacc_27861 = (int64_t) 0;
                
                for (int64_t i_27867 = 0; i_27867 < j_m_i_27292; i_27867++) {
                    int64_t slice_27953 = (int64_t) 1 + i_27867;
                    int64_t x_27470 = ((int64_t *) mem_28639)[slice_27953];
                    int64_t x_27471 = ((int64_t *) mem_28641)[slice_27953];
                    int64_t x_27472 = ((int64_t *) mem_28643)[slice_27953];
                    bool y_27476 = slt64(i_27867, conc_tmp_27291);
                    bool index_certs_27478;
                    
                    if (!y_27476) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", i_27867,
                                      "] out of bounds for array of shape [",
                                      conc_tmp_27291, "].",
                                      "-> #0  sparse.fut:91:47-57\n   #1  sparse.fut:90:31-100:68\n   #2  sparse.fut:169:49-171:81\n   #3  sparse.fut:246:21-96\n   #4  sparse.fut:235:1-248:60\n");
                        if (memblock_unref(ctx, &mem_param_28653,
                                           "mem_param_28653") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_param_28648,
                                           "mem_param_28648") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28765,
                                           "defunc_0_f_res_mem_28765") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28760,
                                           "defunc_0_f_res_mem_28760") != 0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28928,
                                           "intersectArrays_res_f_res_mem_28928") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28923,
                                           "intersectArrays_res_f_res_mem_28923") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28918,
                                           "intersectArrays_res_f_res_mem_28918") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28637, "mem_28637") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28635, "mem_28635") != 0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                                           "intersectArrays_res_mem_28943") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                                           "intersectArrays_res_mem_28938") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                                           "intersectArrays_res_mem_28933") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    int64_t y_27479 = ((int64_t *) mem_28639)[i_27867];
                    bool cond_27480 = x_27470 == y_27479;
                    int64_t defunc_0_f_res_27481;
                    
                    if (cond_27480) {
                        defunc_0_f_res_27481 = x_27470;
                    } else {
                        defunc_0_f_res_27481 = (int64_t) -1;
                    }
                    
                    int64_t defunc_0_f_res_27482;
                    int64_t defunc_0_f_res_27483;
                    
                    if (cond_27480) {
                        bool cond_27484 = x_27471 == (int64_t) 0;
                        int64_t defunc_0_f_res_t_res_27485;
                        int64_t defunc_0_f_res_t_res_27486;
                        
                        if (cond_27484) {
                            int64_t defunc_0_f_res_t_res_t_res_27487 =
                                    ((int64_t *) mem_28643)[i_27867];
                            
                            defunc_0_f_res_t_res_27485 = x_27472;
                            defunc_0_f_res_t_res_27486 =
                                defunc_0_f_res_t_res_t_res_27487;
                        } else {
                            int64_t defunc_0_f_res_t_res_f_res_27488 =
                                    ((int64_t *) mem_28643)[i_27867];
                            
                            defunc_0_f_res_t_res_27485 =
                                defunc_0_f_res_t_res_f_res_27488;
                            defunc_0_f_res_t_res_27486 = x_27472;
                        }
                        defunc_0_f_res_27482 = defunc_0_f_res_t_res_27485;
                        defunc_0_f_res_27483 = defunc_0_f_res_t_res_27486;
                    } else {
                        defunc_0_f_res_27482 = (int64_t) 0;
                        defunc_0_f_res_27483 = (int64_t) 0;
                    }
                    
                    bool eq_x_y_27489 = (int64_t) -1 == x_27470;
                    bool p_and_eq_x_y_27490 = cond_27480 && eq_x_y_27489;
                    bool not_p_27491 = !cond_27480;
                    bool defunc_0_p_res_27492 = p_and_eq_x_y_27490 ||
                         not_p_27491;
                    int64_t defunc_1_lifted_lambda_res_27493 =
                            btoi_bool_i64(defunc_0_p_res_27492);
                    bool is_i_27494 = defunc_1_lifted_lambda_res_27493 ==
                         (int64_t) 0;
                    bool cond_neg_27495 = !is_i_27494;
                    int64_t part_res_27496 = btoi_bool_i64(cond_neg_27495);
                    int64_t part_res_27497 = btoi_bool_i64(is_i_27494);
                    int64_t zz_27469 = part_res_27497 + scanacc_27861;
                    
                    ((int64_t *) mem_28803)[i_27867] = zz_27469;
                    ((int64_t *) mem_28805)[i_27867] = part_res_27496;
                    ((int64_t *) mem_28807)[i_27867] = defunc_0_f_res_27481;
                    ((int64_t *) mem_28809)[i_27867] = defunc_0_f_res_27482;
                    ((int64_t *) mem_28811)[i_27867] = defunc_0_f_res_27483;
                    
                    int64_t scanacc_tmp_29115 = zz_27469;
                    
                    scanacc_27861 = scanacc_tmp_29115;
                }
                discard_27873 = scanacc_27861;
                
                int64_t partition_sizze_27503;
                
                if (is_empty_27311) {
                    partition_sizze_27503 = (int64_t) 0;
                } else {
                    int64_t last_offset_27504 =
                            ((int64_t *) mem_28803)[last_index_27310];
                    
                    partition_sizze_27503 = last_offset_27504;
                }
                
                struct memblock mem_28873;
                
                mem_28873.references = NULL;
                if (memblock_alloc(ctx, &mem_28873, bytes_28802, "mem_28873")) {
                    err = 1;
                    goto cleanup;
                }
                
                struct memblock mem_28875;
                
                mem_28875.references = NULL;
                if (memblock_alloc(ctx, &mem_28875, bytes_28802, "mem_28875")) {
                    err = 1;
                    goto cleanup;
                }
                
                struct memblock mem_28877;
                
                mem_28877.references = NULL;
                if (memblock_alloc(ctx, &mem_28877, bytes_28802, "mem_28877")) {
                    err = 1;
                    goto cleanup;
                }
                for (int64_t write_iter_27874 = 0; write_iter_27874 <
                     j_m_i_27292; write_iter_27874++) {
                    int64_t write_iv_27878 =
                            ((int64_t *) mem_28805)[write_iter_27874];
                    int64_t write_iv_27879 =
                            ((int64_t *) mem_28803)[write_iter_27874];
                    bool is_this_one_27516 = write_iv_27878 == (int64_t) 0;
                    int64_t this_offset_27517 = (int64_t) -1 + write_iv_27879;
                    int64_t total_res_27518;
                    
                    if (is_this_one_27516) {
                        total_res_27518 = this_offset_27517;
                    } else {
                        total_res_27518 = (int64_t) -1;
                    }
                    
                    bool less_than_zzero_27883 = slt64(total_res_27518,
                                                       (int64_t) 0);
                    bool greater_than_sizze_27884 = sle64(j_m_i_27292,
                                                          total_res_27518);
                    bool outside_bounds_dim_27885 = less_than_zzero_27883 ||
                         greater_than_sizze_27884;
                    
                    if (!outside_bounds_dim_27885) {
                        memmove(mem_28873.mem + total_res_27518 * (int64_t) 8,
                                mem_28807 + write_iter_27874 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                    if (!outside_bounds_dim_27885) {
                        memmove(mem_28875.mem + total_res_27518 * (int64_t) 8,
                                mem_28809 + write_iter_27874 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                    if (!outside_bounds_dim_27885) {
                        memmove(mem_28877.mem + total_res_27518 * (int64_t) 8,
                                mem_28811 + write_iter_27874 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                }
                
                bool empty_slice_27519 = partition_sizze_27503 == (int64_t) 0;
                int64_t m_27520 = sub64(partition_sizze_27503, (int64_t) 1);
                bool zzero_leq_i_p_m_t_s_27521 = sle64((int64_t) 0, m_27520);
                bool i_p_m_t_s_leq_w_27522 = slt64(m_27520, j_m_i_27292);
                bool i_lte_j_27523 = sle64((int64_t) 0, partition_sizze_27503);
                bool y_27524 = zzero_leq_i_p_m_t_s_27521 &&
                     i_p_m_t_s_leq_w_27522;
                bool y_27525 = i_lte_j_27523 && y_27524;
                bool ok_or_empty_27526 = empty_slice_27519 || y_27525;
                bool index_certs_27527;
                
                if (!ok_or_empty_27526) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [:", partition_sizze_27503,
                                  "] out of bounds for array of shape [",
                                  j_m_i_27292, "].",
                                  "-> #0  /prelude/soacs.fut:141:6-16\n   #1  sparse.fut:90:3-100:69\n   #2  sparse.fut:169:49-171:81\n   #3  sparse.fut:246:21-96\n   #4  sparse.fut:235:1-248:60\n");
                    if (memblock_unref(ctx, &mem_28877, "mem_28877") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28875, "mem_28875") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28873, "mem_28873") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_28653,
                                       "mem_param_28653") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_28648,
                                       "mem_param_28648") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28765,
                                       "defunc_0_f_res_mem_28765") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28760,
                                       "defunc_0_f_res_mem_28760") != 0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28928,
                                       "intersectArrays_res_f_res_mem_28928") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28923,
                                       "intersectArrays_res_f_res_mem_28923") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28918,
                                       "intersectArrays_res_f_res_mem_28918") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28637, "mem_28637") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28635, "mem_28635") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                                       "intersectArrays_res_mem_28943") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                                       "intersectArrays_res_mem_28938") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                                       "intersectArrays_res_mem_28933") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                sizze_27312 = partition_sizze_27503;
                sizze_27313 = partition_sizze_27503;
                sizze_27314 = partition_sizze_27503;
                intersectArrays_res_f_res_ixfn_28914 = partition_sizze_27503;
                intersectArrays_res_f_res_ixfn_28919 = partition_sizze_27503;
                intersectArrays_res_f_res_ixfn_28924 = partition_sizze_27503;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28918,
                                 &mem_28873, "mem_28873") != 0)
                    return 1;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28923,
                                 &mem_28875, "mem_28875") != 0)
                    return 1;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28928,
                                 &mem_28877, "mem_28877") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28877, "mem_28877") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28875, "mem_28875") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28873, "mem_28873") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_param_28653, "mem_param_28653") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &mem_param_28648, "mem_param_28648") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28765,
                                   "defunc_0_f_res_mem_28765") != 0)
                    return 1;
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28760,
                                   "defunc_0_f_res_mem_28760") != 0)
                    return 1;
            }
            if (memblock_unref(ctx, &mem_28635, "mem_28635") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28637, "mem_28637") != 0)
                return 1;
            sizze_27293 = sizze_27312;
            sizze_27294 = sizze_27313;
            sizze_27295 = sizze_27314;
            intersectArrays_res_ixfn_28929 =
                intersectArrays_res_f_res_ixfn_28914;
            intersectArrays_res_ixfn_28930 =
                intersectArrays_res_f_res_ixfn_28915;
            intersectArrays_res_ixfn_28934 =
                intersectArrays_res_f_res_ixfn_28919;
            intersectArrays_res_ixfn_28935 =
                intersectArrays_res_f_res_ixfn_28920;
            intersectArrays_res_ixfn_28939 =
                intersectArrays_res_f_res_ixfn_28924;
            intersectArrays_res_ixfn_28940 =
                intersectArrays_res_f_res_ixfn_28925;
            if (memblock_set(ctx, &intersectArrays_res_mem_28933,
                             &intersectArrays_res_f_res_mem_28918,
                             "intersectArrays_res_f_res_mem_28918") != 0)
                return 1;
            if (memblock_set(ctx, &intersectArrays_res_mem_28938,
                             &intersectArrays_res_f_res_mem_28923,
                             "intersectArrays_res_f_res_mem_28923") != 0)
                return 1;
            if (memblock_set(ctx, &intersectArrays_res_mem_28943,
                             &intersectArrays_res_f_res_mem_28928,
                             "intersectArrays_res_f_res_mem_28928") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_f_res_mem_28928,
                               "intersectArrays_res_f_res_mem_28928") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_f_res_mem_28923,
                               "intersectArrays_res_f_res_mem_28923") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_f_res_mem_28918,
                               "intersectArrays_res_f_res_mem_28918") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28637, "mem_28637") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28635, "mem_28635") != 0)
                return 1;
        }
        
        bool dim_match_27531 = sizze_27293 == sizze_27294;
        bool empty_or_match_cert_27532;
        
        if (!dim_match_27531) {
            ctx->error = msgprintf("Error: %s\n\nBacktrace:\n%s",
                                   "Function return value does not match shape of declared return type.",
                                   "-> #0  sparse.fut:81:7-100:69\n   #1  sparse.fut:169:49-171:81\n   #2  sparse.fut:246:21-96\n   #3  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                               "intersectArrays_res_mem_28943") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                               "intersectArrays_res_mem_28938") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                               "intersectArrays_res_mem_28933") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool dim_match_27533 = sizze_27293 == sizze_27295;
        bool empty_or_match_cert_27534;
        
        if (!dim_match_27533) {
            ctx->error = msgprintf("Error: %s\n\nBacktrace:\n%s",
                                   "Function return value does not match shape of declared return type.",
                                   "-> #0  sparse.fut:81:7-100:69\n   #1  sparse.fut:169:49-171:81\n   #2  sparse.fut:246:21-96\n   #3  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                               "intersectArrays_res_mem_28943") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                               "intersectArrays_res_mem_28938") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                               "intersectArrays_res_mem_28933") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t x_27535 = ((int64_t *) mem_28503)[i_27253];
        int64_t y_27536 = ((int64_t *) mem_28503)[i_27247];
        int64_t indxs_27537 = sub64(x_27535, y_27536);
        bool bounds_invalid_upwards_27538 = slt64(x_27535, y_27536);
        bool valid_27539 = !bounds_invalid_upwards_27538;
        bool range_valid_c_27540;
        
        if (!valid_27539) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Range ", y_27536, "..<", x_27535,
                                   " is invalid.",
                                   "-> #0  sparse.fut:173:18-42\n   #1  sparse.fut:246:21-96\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                               "intersectArrays_res_mem_28943") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                               "intersectArrays_res_mem_28938") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                               "intersectArrays_res_mem_28933") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool dim_match_27542 = indxs_27537 == sizze_27293;
        bool empty_or_match_cert_27543;
        
        if (!dim_match_27542) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Value of (core language) shape (",
                                   sizze_27293,
                                   ") cannot match shape of type `[",
                                   indxs_27537, "]index`.",
                                   "-> #0  sparse.fut:177:26-51\n   #1  sparse.fut:246:21-96\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                               "intersectArrays_res_mem_28943") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                               "intersectArrays_res_mem_28938") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                               "intersectArrays_res_mem_28933") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool empty_or_match_cert_27544;
        
        if (!dim_match_27542) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Value of (core language) shape (",
                                   sizze_27293,
                                   ") cannot match shape of type `[",
                                   indxs_27537, "]t`.",
                                   "-> #0  sparse.fut:178:25-181:75\n   #1  sparse.fut:246:21-96\n   #2  sparse.fut:235:1-248:60\n");
            if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                               "intersectArrays_res_mem_28943") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                               "intersectArrays_res_mem_28938") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                               "intersectArrays_res_mem_28933") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        for (int64_t write_iter_27901 = 0; write_iter_27901 < indxs_27537;
             write_iter_27901++) {
            int64_t index_primexp_27963 = add64(y_27536, write_iter_27901);
            bool y_27554 = slt64(write_iter_27901, sizze_27293);
            bool index_certs_27556;
            
            if (!y_27554) {
                ctx->error =
                    msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                              "Index [", write_iter_27901,
                              "] out of bounds for array of shape [",
                              sizze_27293, "].",
                              "-> #0  sparse.fut:180:58-76\n   #1  sparse.fut:178:26-181:63\n   #2  sparse.fut:246:21-96\n   #3  sparse.fut:235:1-248:60\n");
                if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                                   "intersectArrays_res_mem_28943") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                                   "intersectArrays_res_mem_28938") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                                   "intersectArrays_res_mem_28933") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                    return 1;
                return 1;
            }
            
            int64_t y_27557 =
                    ((int64_t *) intersectArrays_res_mem_28943.mem)[write_iter_27901];
            int64_t i_27558 = add64(i_27252, y_27557);
            bool x_27559 = sle64((int64_t) 0, i_27558);
            bool y_27560 = slt64(i_27558, k_26451);
            bool bounds_check_27561 = x_27559 && y_27560;
            bool index_certs_27562;
            
            if (!bounds_check_27561) {
                ctx->error =
                    msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                              "Index [", i_27558,
                              "] out of bounds for array of shape [", k_26451,
                              "].",
                              "-> #0  sparse.fut:180:33-79\n   #1  sparse.fut:178:26-181:63\n   #2  sparse.fut:246:21-96\n   #3  sparse.fut:235:1-248:60\n");
                if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                                   "intersectArrays_res_mem_28943") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                                   "intersectArrays_res_mem_28938") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                                   "intersectArrays_res_mem_28933") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                    return 1;
                return 1;
            }
            
            double defunc_1_mult_arg_27563 =
                   ((double *) rightValues_mem_27967.mem)[i_27558];
            int64_t y_27564 =
                    ((int64_t *) intersectArrays_res_mem_28938.mem)[write_iter_27901];
            int64_t i_27565 = add64(i_27274, y_27564);
            bool x_27566 = sle64((int64_t) 0, i_27565);
            bool y_27567 = slt64(i_27565, eWiseAddOffsets_res_26642);
            bool bounds_check_27568 = x_27566 && y_27567;
            bool index_certs_27569;
            
            if (!bounds_check_27568) {
                ctx->error =
                    msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                              "Index [", i_27565,
                              "] out of bounds for array of shape [",
                              eWiseAddOffsets_res_26642, "].",
                              "-> #0  sparse.fut:179:33-77\n   #1  sparse.fut:178:26-181:63\n   #2  sparse.fut:246:21-96\n   #3  sparse.fut:235:1-248:60\n");
                if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                                   "intersectArrays_res_mem_28943") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                                   "intersectArrays_res_mem_28938") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                                   "intersectArrays_res_mem_28933") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                    return 1;
                return 1;
            }
            
            double defunc_0_mult_arg_27570 = ((double *) mem_28083)[i_27565];
            double defunc_1_mult_res_27571 = defunc_1_mult_arg_27563 *
                   defunc_0_mult_arg_27570;
            bool less_than_zzero_27908 = slt64(index_primexp_27963,
                                               (int64_t) 0);
            bool greater_than_sizze_27909 = sle64(eWiseMultOffsets_res_27238,
                                                  index_primexp_27963);
            bool outside_bounds_dim_27910 = less_than_zzero_27908 ||
                 greater_than_sizze_27909;
            
            if (!outside_bounds_dim_27910) {
                ((double *) mem_28615)[index_primexp_27963] =
                    defunc_1_mult_res_27571;
            }
            if (!outside_bounds_dim_27910) {
                memmove(mem_28617 + index_primexp_27963 * (int64_t) 8,
                        intersectArrays_res_mem_28933.mem + write_iter_27901 *
                        (int64_t) 8, (int64_t) sizeof(int64_t));
            }
        }
        if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                           "intersectArrays_res_mem_28933") != 0)
            return 1;
        if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                           "intersectArrays_res_mem_28938") != 0)
            return 1;
        if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                           "intersectArrays_res_mem_28943") != 0)
            return 1;
        if (memblock_unref(ctx, &intersectArrays_res_mem_28943,
                           "intersectArrays_res_mem_28943") != 0)
            return 1;
        if (memblock_unref(ctx, &intersectArrays_res_mem_28938,
                           "intersectArrays_res_mem_28938") != 0)
            return 1;
        if (memblock_unref(ctx, &intersectArrays_res_mem_28933,
                           "intersectArrays_res_mem_28933") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
        return 1;
    
    struct memblock mem_28975;
    
    mem_28975.references = NULL;
    if (memblock_alloc(ctx, &mem_28975, bytes_28614, "mem_28975")) {
        err = 1;
        goto cleanup;
    }
    memmove(mem_28975.mem + (int64_t) 0, mem_28615 + (int64_t) 0,
            eWiseMultOffsets_res_27238 * (int64_t) sizeof(double));
    
    struct memblock mem_28978;
    
    mem_28978.references = NULL;
    if (memblock_alloc(ctx, &mem_28978, bytes_28614, "mem_28978")) {
        err = 1;
        goto cleanup;
    }
    memmove(mem_28978.mem + (int64_t) 0, mem_28617 + (int64_t) 0,
            eWiseMultOffsets_res_27238 * (int64_t) sizeof(int64_t));
    
    struct memblock mem_28981;
    
    mem_28981.references = NULL;
    if (memblock_alloc(ctx, &mem_28981, bytes_27970, "mem_28981")) {
        err = 1;
        goto cleanup;
    }
    memmove(mem_28981.mem + (int64_t) 0, mem_28503 + (int64_t) 0, m_26450 *
            (int64_t) sizeof(int64_t));
    out_arrsizze_29008 = eWiseMultOffsets_res_27238;
    out_arrsizze_29010 = eWiseMultOffsets_res_27238;
    if (memblock_set(ctx, &out_mem_29007, &mem_28975, "mem_28975") != 0)
        return 1;
    if (memblock_set(ctx, &out_mem_29009, &mem_28978, "mem_28978") != 0)
        return 1;
    if (memblock_set(ctx, &out_mem_29011, &mem_28981, "mem_28981") != 0)
        return 1;
    (*out_mem_p_29154).references = NULL;
    if (memblock_set(ctx, &*out_mem_p_29154, &out_mem_29007, "out_mem_29007") !=
        0)
        return 1;
    *out_out_arrsizze_29155 = out_arrsizze_29008;
    (*out_mem_p_29156).references = NULL;
    if (memblock_set(ctx, &*out_mem_p_29156, &out_mem_29009, "out_mem_29009") !=
        0)
        return 1;
    *out_out_arrsizze_29157 = out_arrsizze_29010;
    (*out_mem_p_29158).references = NULL;
    if (memblock_set(ctx, &*out_mem_p_29158, &out_mem_29011, "out_mem_29011") !=
        0)
        return 1;
    if (memblock_unref(ctx, &mem_28981, "mem_28981") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_28978, "mem_28978") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_28975, "mem_28975") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_28619, "mem_28619") != 0)
        return 1;
    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
        return 1;
    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
        return 1;
    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
        return 1;
    
  cleanup:
    { }
    free(mem_27971);
    free(mem_27989);
    free(mem_27991);
    free(mem_27993);
    free(mem_27995);
    free(mem_27997);
    free(mem_28083);
    free(mem_28085);
    free(mem_28115);
    free(mem_28117);
    free(mem_28119);
    free(mem_28121);
    free(mem_28123);
    free(mem_28125);
    free(mem_28127);
    free(mem_28129);
    free(mem_28131);
    free(mem_28133);
    free(mem_28149);
    free(mem_28151);
    free(mem_28153);
    free(mem_28155);
    free(mem_28157);
    free(mem_28503);
    free(mem_28521);
    free(mem_28523);
    free(mem_28525);
    free(mem_28527);
    free(mem_28529);
    free(mem_28615);
    free(mem_28617);
    free(mem_28639);
    free(mem_28641);
    free(mem_28643);
    free(mem_28659);
    free(mem_28661);
    free(mem_28663);
    free(mem_28665);
    free(mem_28667);
    free(mem_28803);
    free(mem_28805);
    free(mem_28807);
    free(mem_28809);
    free(mem_28811);
    return err;
}
static int futrts_eWiseMultEntry(struct futhark_context *ctx,
                                 struct memblock *out_mem_p_29203,
                                 int64_t *out_out_arrsizze_29204,
                                 struct memblock *out_mem_p_29205,
                                 int64_t *out_out_arrsizze_29206,
                                 struct memblock *out_mem_p_29207,
                                 struct memblock leftValues_mem_27964,
                                 struct memblock leftColumns_mem_27965,
                                 struct memblock leftOffsets_mem_27966,
                                 struct memblock rightValues_mem_27967,
                                 struct memblock rightColumns_mem_27968,
                                 struct memblock rightOffsets_mem_27969,
                                 int64_t n_25922, int64_t m_25923,
                                 int64_t k_25924)
{
    (void) ctx;
    
    int err = 0;
    size_t mem_27971_cached_sizze_29208 = 0;
    char *mem_27971 = NULL;
    size_t mem_27989_cached_sizze_29209 = 0;
    char *mem_27989 = NULL;
    size_t mem_27991_cached_sizze_29210 = 0;
    char *mem_27991 = NULL;
    size_t mem_27993_cached_sizze_29211 = 0;
    char *mem_27993 = NULL;
    size_t mem_27995_cached_sizze_29212 = 0;
    char *mem_27995 = NULL;
    size_t mem_27997_cached_sizze_29213 = 0;
    char *mem_27997 = NULL;
    size_t mem_28083_cached_sizze_29214 = 0;
    char *mem_28083 = NULL;
    size_t mem_28085_cached_sizze_29215 = 0;
    char *mem_28085 = NULL;
    size_t mem_28107_cached_sizze_29216 = 0;
    char *mem_28107 = NULL;
    size_t mem_28109_cached_sizze_29217 = 0;
    char *mem_28109 = NULL;
    size_t mem_28111_cached_sizze_29218 = 0;
    char *mem_28111 = NULL;
    size_t mem_28127_cached_sizze_29219 = 0;
    char *mem_28127 = NULL;
    size_t mem_28129_cached_sizze_29220 = 0;
    char *mem_28129 = NULL;
    size_t mem_28131_cached_sizze_29221 = 0;
    char *mem_28131 = NULL;
    size_t mem_28133_cached_sizze_29222 = 0;
    char *mem_28133 = NULL;
    size_t mem_28135_cached_sizze_29223 = 0;
    char *mem_28135 = NULL;
    size_t mem_28271_cached_sizze_29224 = 0;
    char *mem_28271 = NULL;
    size_t mem_28273_cached_sizze_29225 = 0;
    char *mem_28273 = NULL;
    size_t mem_28275_cached_sizze_29226 = 0;
    char *mem_28275 = NULL;
    size_t mem_28277_cached_sizze_29227 = 0;
    char *mem_28277 = NULL;
    size_t mem_28279_cached_sizze_29228 = 0;
    char *mem_28279 = NULL;
    struct memblock out_mem_29007;
    
    out_mem_29007.references = NULL;
    
    int64_t out_arrsizze_29008;
    struct memblock out_mem_29009;
    
    out_mem_29009.references = NULL;
    
    int64_t out_arrsizze_29010;
    struct memblock out_mem_29011;
    
    out_mem_29011.references = NULL;
    
    int64_t bytes_27970 = (int64_t) 8 * m_25923;
    
    if (mem_27971_cached_sizze_29208 < (size_t) bytes_27970) {
        mem_27971 = realloc(mem_27971, bytes_27970);
        mem_27971_cached_sizze_29208 = bytes_27970;
    }
    
    int64_t discard_27618;
    int64_t scanacc_27614 = (int64_t) 0;
    
    for (int64_t i_27616 = 0; i_27616 < m_25923; i_27616++) {
        int64_t x_25936 = ((int64_t *) leftOffsets_mem_27966.mem)[i_27616];
        int64_t x_25938 = ((int64_t *) rightOffsets_mem_27969.mem)[i_27616];
        bool cond_25939 = i_27616 == (int64_t) 0;
        int64_t prevx2_25940;
        
        if (cond_25939) {
            prevx2_25940 = i_27616;
        } else {
            int64_t prevx2_f_res_25941 = sub64(i_27616, (int64_t) 1);
            
            prevx2_25940 = prevx2_f_res_25941;
        }
        
        int64_t prevy2_25942;
        
        if (cond_25939) {
            prevy2_25942 = i_27616;
        } else {
            int64_t prevy2_f_res_25943 = sub64(i_27616, (int64_t) 1);
            
            prevy2_25942 = prevy2_f_res_25943;
        }
        
        bool x_25944 = sle64((int64_t) 0, prevy2_25942);
        bool y_25945 = slt64(prevy2_25942, m_25923);
        bool bounds_check_25946 = x_25944 && y_25945;
        bool index_certs_25947;
        
        if (!bounds_check_25946) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", prevy2_25942,
                                   "] out of bounds for array of shape [",
                                   m_25923, "].",
                                   "-> #0  sparse.fut:123:58-78\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:120:19-125:50\n   #4  sparse.fut:162:23-49\n   #5  sparse.fut:232:17-60\n   #6  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_25948 =
                ((int64_t *) rightOffsets_mem_27969.mem)[prevy2_25942];
        int64_t j_m_i_25949 = sub64(x_25938, i_25948);
        bool empty_slice_25950 = j_m_i_25949 == (int64_t) 0;
        int64_t m_25951 = sub64(j_m_i_25949, (int64_t) 1);
        int64_t i_p_m_t_s_25952 = add64(i_25948, m_25951);
        bool zzero_leq_i_p_m_t_s_25953 = sle64((int64_t) 0, i_p_m_t_s_25952);
        bool i_p_m_t_s_leq_w_25954 = slt64(i_p_m_t_s_25952, k_25924);
        bool zzero_lte_i_25955 = sle64((int64_t) 0, i_25948);
        bool i_lte_j_25956 = sle64(i_25948, x_25938);
        bool y_25957 = i_p_m_t_s_leq_w_25954 && zzero_lte_i_25955;
        bool y_25958 = zzero_leq_i_p_m_t_s_25953 && y_25957;
        bool y_25959 = i_lte_j_25956 && y_25958;
        bool forwards_ok_25960 = zzero_lte_i_25955 && y_25959;
        bool ok_or_empty_25961 = empty_slice_25950 || forwards_ok_25960;
        bool index_certs_25962;
        
        if (!ok_or_empty_25961) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_25948, ":", x_25938,
                          "] out of bounds for array of shape [", k_25924, "].",
                          "-> #0  sparse.fut:123:44-82\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:120:19-125:50\n   #4  sparse.fut:162:23-49\n   #5  sparse.fut:232:17-60\n   #6  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool x_25964 = sle64((int64_t) 0, prevx2_25940);
        bool y_25965 = slt64(prevx2_25940, m_25923);
        bool bounds_check_25966 = x_25964 && y_25965;
        bool index_certs_25967;
        
        if (!bounds_check_25966) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", prevx2_25940,
                                   "] out of bounds for array of shape [",
                                   m_25923, "].",
                                   "-> #0  sparse.fut:122:57-76\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:120:19-125:50\n   #4  sparse.fut:162:23-49\n   #5  sparse.fut:232:17-60\n   #6  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_25968 = ((int64_t *) leftOffsets_mem_27966.mem)[prevx2_25940];
        int64_t j_m_i_25969 = sub64(x_25936, i_25968);
        bool empty_slice_25970 = j_m_i_25969 == (int64_t) 0;
        int64_t m_25971 = sub64(j_m_i_25969, (int64_t) 1);
        int64_t i_p_m_t_s_25972 = add64(i_25968, m_25971);
        bool zzero_leq_i_p_m_t_s_25973 = sle64((int64_t) 0, i_p_m_t_s_25972);
        bool i_p_m_t_s_leq_w_25974 = slt64(i_p_m_t_s_25972, n_25922);
        bool zzero_lte_i_25975 = sle64((int64_t) 0, i_25968);
        bool i_lte_j_25976 = sle64(i_25968, x_25936);
        bool y_25977 = i_p_m_t_s_leq_w_25974 && zzero_lte_i_25975;
        bool y_25978 = zzero_leq_i_p_m_t_s_25973 && y_25977;
        bool y_25979 = i_lte_j_25976 && y_25978;
        bool forwards_ok_25980 = zzero_lte_i_25975 && y_25979;
        bool ok_or_empty_25981 = empty_slice_25970 || forwards_ok_25980;
        bool index_certs_25982;
        
        if (!ok_or_empty_25981) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_25968, ":", x_25936,
                          "] out of bounds for array of shape [", n_25922, "].",
                          "-> #0  sparse.fut:122:44-80\n   #1  /prelude/soacs.fut:56:19-23\n   #2  /prelude/soacs.fut:56:3-37\n   #3  sparse.fut:120:19-125:50\n   #4  sparse.fut:162:23-49\n   #5  sparse.fut:232:17-60\n   #6  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t intersectSizze_res_25984;
        
        if (empty_slice_25970) {
            intersectSizze_res_25984 = (int64_t) 0;
        } else {
            int64_t conc_tmp_25985 = j_m_i_25949 + j_m_i_25969;
            int64_t j_m_i_25986 = sub64(conc_tmp_25985, (int64_t) 1);
            int64_t bytes_27979 = (int64_t) 8 * conc_tmp_25985;
            int64_t bytes_27996 = (int64_t) 4 * conc_tmp_25985;
            int64_t intersectSizze_res_f_res_25987;
            
            if (empty_slice_25950) {
                intersectSizze_res_f_res_25987 = (int64_t) 0;
            } else {
                struct memblock mem_27980;
                
                mem_27980.references = NULL;
                if (memblock_alloc(ctx, &mem_27980, bytes_27979, "mem_27980")) {
                    err = 1;
                    goto cleanup;
                }
                
                int64_t tmp_offs_29014 = (int64_t) 0;
                
                memmove(mem_27980.mem + tmp_offs_29014 * (int64_t) 8,
                        leftColumns_mem_27965.mem + i_25968 * (int64_t) 8,
                        j_m_i_25969 * (int64_t) sizeof(int64_t));
                tmp_offs_29014 += j_m_i_25969;
                memmove(mem_27980.mem + tmp_offs_29014 * (int64_t) 8,
                        rightColumns_mem_27968.mem + i_25948 * (int64_t) 8,
                        j_m_i_25949 * (int64_t) sizeof(int64_t));
                tmp_offs_29014 += j_m_i_25949;
                
                bool cond_25989 = conc_tmp_25985 == (int64_t) 0;
                int32_t iters_25990;
                
                if (cond_25989) {
                    iters_25990 = 0;
                } else {
                    iters_25990 = 32;
                }
                
                bool loop_nonempty_25991 = slt32(0, iters_25990);
                bool x_25992 = sle64((int64_t) 0, j_m_i_25986);
                bool y_25993 = slt64(j_m_i_25986, conc_tmp_25985);
                bool bounds_check_25994 = x_25992 && y_25993;
                bool loop_not_taken_25995 = !loop_nonempty_25991;
                bool protect_assert_disj_25996 = bounds_check_25994 ||
                     loop_not_taken_25995;
                bool index_certs_25997;
                
                if (!protect_assert_disj_25996) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", j_m_i_25986,
                                  "] out of bounds for array of shape [",
                                  conc_tmp_25985, "].",
                                  "-> #0  /prelude/array.fut:18:29-34\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:25:24-35\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:53:31-62\n   #3  sparse.fut:3:25-4:48\n   #4  sparse.fut:76:12-24\n   #5  sparse.fut:122:30-123:82\n   #6  /prelude/soacs.fut:56:19-23\n   #7  /prelude/soacs.fut:56:3-37\n   #8  sparse.fut:120:19-125:50\n   #9  sparse.fut:162:23-49\n   #10 sparse.fut:232:17-60\n   #11 sparse.fut:223:1-233:48\n");
                    if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                if (mem_27989_cached_sizze_29209 < (size_t) bytes_27979) {
                    mem_27989 = realloc(mem_27989, bytes_27979);
                    mem_27989_cached_sizze_29209 = bytes_27979;
                }
                if (mem_27991_cached_sizze_29210 < (size_t) bytes_27979) {
                    mem_27991 = realloc(mem_27991, bytes_27979);
                    mem_27991_cached_sizze_29210 = bytes_27979;
                }
                if (mem_27993_cached_sizze_29211 < (size_t) bytes_27979) {
                    mem_27993 = realloc(mem_27993, bytes_27979);
                    mem_27993_cached_sizze_29211 = bytes_27979;
                }
                if (mem_27995_cached_sizze_29212 < (size_t) bytes_27979) {
                    mem_27995 = realloc(mem_27995, bytes_27979);
                    mem_27995_cached_sizze_29212 = bytes_27979;
                }
                if (mem_27997_cached_sizze_29213 < (size_t) bytes_27996) {
                    mem_27997 = realloc(mem_27997, bytes_27996);
                    mem_27997_cached_sizze_29213 = bytes_27996;
                }
                
                struct memblock defunc_2_radix_sort_res_mem_28076;
                
                defunc_2_radix_sort_res_mem_28076.references = NULL;
                
                struct memblock mem_param_27985;
                
                mem_param_27985.references = NULL;
                if (memblock_set(ctx, &mem_param_27985, &mem_27980,
                                 "mem_27980") != 0)
                    return 1;
                for (int32_t i_25999 = 0; i_25999 < iters_25990; i_25999++) {
                    int32_t defunc_2_radix_sort_step_arg_26001 = mul32(2,
                                                                       i_25999);
                    int32_t defunc_0_get_bit_arg_26002 = add32(1,
                                                               defunc_2_radix_sort_step_arg_26001);
                    int64_t i32_res_26003 =
                            sext_i32_i64(defunc_0_get_bit_arg_26002);
                    int64_t i32_res_26004 =
                            sext_i32_i64(defunc_2_radix_sort_step_arg_26001);
                    int64_t discard_27592;
                    int64_t discard_27593;
                    int64_t discard_27594;
                    int64_t discard_27595;
                    int64_t scanacc_27577;
                    int64_t scanacc_27578;
                    int64_t scanacc_27579;
                    int64_t scanacc_27580;
                    
                    scanacc_27577 = (int64_t) 0;
                    scanacc_27578 = (int64_t) 0;
                    scanacc_27579 = (int64_t) 0;
                    scanacc_27580 = (int64_t) 0;
                    for (int64_t i_27586 = 0; i_27586 < conc_tmp_25985;
                         i_27586++) {
                        int64_t x_26032 =
                                ((int64_t *) mem_param_27985.mem)[i_27586];
                        int64_t zgzg_res_26033 = ashr64(x_26032, i32_res_26003);
                        int64_t za_res_26034 = (int64_t) 1 & zgzg_res_26033;
                        int32_t to_i32_res_26035 = sext_i64_i32(za_res_26034);
                        int32_t x_26036 = mul32(2, to_i32_res_26035);
                        int64_t zgzg_res_26037 = ashr64(x_26032, i32_res_26004);
                        int64_t za_res_26038 = (int64_t) 1 & zgzg_res_26037;
                        int32_t to_i32_res_26039 = sext_i64_i32(za_res_26038);
                        int32_t defunc_0_f_res_26040 = add32(x_26036,
                                                             to_i32_res_26039);
                        bool cond_26041 = defunc_0_f_res_26040 == 0;
                        int64_t defunc_0_f_res_26042 =
                                btoi_bool_i64(cond_26041);
                        int64_t defunc_0_f_res_26043;
                        int64_t defunc_0_f_res_26044;
                        int64_t defunc_0_f_res_26045;
                        
                        if (cond_26041) {
                            defunc_0_f_res_26043 = (int64_t) 0;
                            defunc_0_f_res_26044 = (int64_t) 0;
                            defunc_0_f_res_26045 = (int64_t) 0;
                        } else {
                            bool cond_26046 = defunc_0_f_res_26040 == 1;
                            int64_t defunc_0_f_res_f_res_26047 =
                                    btoi_bool_i64(cond_26046);
                            int64_t defunc_0_f_res_f_res_26048;
                            int64_t defunc_0_f_res_f_res_26049;
                            
                            if (cond_26046) {
                                defunc_0_f_res_f_res_26048 = (int64_t) 0;
                                defunc_0_f_res_f_res_26049 = (int64_t) 0;
                            } else {
                                bool cond_26050 = defunc_0_f_res_26040 == 2;
                                int64_t defunc_0_f_res_f_res_f_res_26051 =
                                        btoi_bool_i64(cond_26050);
                                bool cond_neg_26052 = !cond_26050;
                                int64_t defunc_0_f_res_f_res_f_res_26053 =
                                        btoi_bool_i64(cond_neg_26052);
                                
                                defunc_0_f_res_f_res_26048 =
                                    defunc_0_f_res_f_res_f_res_26051;
                                defunc_0_f_res_f_res_26049 =
                                    defunc_0_f_res_f_res_f_res_26053;
                            }
                            defunc_0_f_res_26043 = defunc_0_f_res_f_res_26047;
                            defunc_0_f_res_26044 = defunc_0_f_res_f_res_26048;
                            defunc_0_f_res_26045 = defunc_0_f_res_f_res_26049;
                        }
                        
                        int64_t defunc_1_op_res_26028 =
                                add64(defunc_0_f_res_26042, scanacc_27577);
                        int64_t defunc_1_op_res_26029 =
                                add64(defunc_0_f_res_26043, scanacc_27578);
                        int64_t defunc_1_op_res_26030 =
                                add64(defunc_0_f_res_26044, scanacc_27579);
                        int64_t defunc_1_op_res_26031 =
                                add64(defunc_0_f_res_26045, scanacc_27580);
                        
                        ((int64_t *) mem_27989)[i_27586] =
                            defunc_1_op_res_26028;
                        ((int64_t *) mem_27991)[i_27586] =
                            defunc_1_op_res_26029;
                        ((int64_t *) mem_27993)[i_27586] =
                            defunc_1_op_res_26030;
                        ((int64_t *) mem_27995)[i_27586] =
                            defunc_1_op_res_26031;
                        ((int32_t *) mem_27997)[i_27586] = defunc_0_f_res_26040;
                        
                        int64_t scanacc_tmp_29017 = defunc_1_op_res_26028;
                        int64_t scanacc_tmp_29018 = defunc_1_op_res_26029;
                        int64_t scanacc_tmp_29019 = defunc_1_op_res_26030;
                        int64_t scanacc_tmp_29020 = defunc_1_op_res_26031;
                        
                        scanacc_27577 = scanacc_tmp_29017;
                        scanacc_27578 = scanacc_tmp_29018;
                        scanacc_27579 = scanacc_tmp_29019;
                        scanacc_27580 = scanacc_tmp_29020;
                    }
                    discard_27592 = scanacc_27577;
                    discard_27593 = scanacc_27578;
                    discard_27594 = scanacc_27579;
                    discard_27595 = scanacc_27580;
                    
                    int64_t last_res_26059 =
                            ((int64_t *) mem_27989)[j_m_i_25986];
                    int64_t last_res_26060 =
                            ((int64_t *) mem_27991)[j_m_i_25986];
                    int64_t last_res_26061 =
                            ((int64_t *) mem_27993)[j_m_i_25986];
                    struct memblock mem_28059;
                    
                    mem_28059.references = NULL;
                    if (memblock_alloc(ctx, &mem_28059, bytes_27979,
                                       "mem_28059")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28059.mem + (int64_t) 0, mem_param_27985.mem +
                            (int64_t) 0, conc_tmp_25985 *
                            (int64_t) sizeof(int64_t));
                    for (int64_t write_iter_27596 = 0; write_iter_27596 <
                         conc_tmp_25985; write_iter_27596++) {
                        int32_t write_iv_27598 =
                                ((int32_t *) mem_27997)[write_iter_27596];
                        int64_t write_iv_27599 =
                                ((int64_t *) mem_27989)[write_iter_27596];
                        int64_t write_iv_27600 =
                                ((int64_t *) mem_27991)[write_iter_27596];
                        int64_t write_iv_27601 =
                                ((int64_t *) mem_27993)[write_iter_27596];
                        int64_t write_iv_27602 =
                                ((int64_t *) mem_27995)[write_iter_27596];
                        bool match_lit_26070 = 0 == write_iv_27598;
                        int64_t defunc_1_f_res_26071;
                        
                        if (match_lit_26070) {
                            int64_t case_res_26072 = sub64(write_iv_27599,
                                                           (int64_t) 1);
                            
                            defunc_1_f_res_26071 = case_res_26072;
                        } else {
                            bool match_lit_26073 = 1 == write_iv_27598;
                            int64_t x_26074;
                            
                            if (match_lit_26073) {
                                int64_t x_26075 = add64(last_res_26059,
                                                        write_iv_27600);
                                int64_t case_res_26076 = sub64(x_26075,
                                                               (int64_t) 1);
                                
                                x_26074 = case_res_26076;
                            } else {
                                bool match_lit_26077 = 2 == write_iv_27598;
                                int64_t x_26078;
                                
                                if (match_lit_26077) {
                                    int64_t x_26079 = add64(last_res_26059,
                                                            last_res_26060);
                                    int64_t x_26080 = add64(x_26079,
                                                            write_iv_27601);
                                    int64_t case_res_26081 = sub64(x_26080,
                                                                   (int64_t) 1);
                                    
                                    x_26078 = case_res_26081;
                                } else {
                                    int64_t x_26082 = add64(last_res_26059,
                                                            last_res_26060);
                                    int64_t x_26083 = add64(last_res_26061,
                                                            x_26082);
                                    int64_t x_26084 = add64(x_26083,
                                                            write_iv_27602);
                                    int64_t defunc_1_f_res_res_26085 =
                                            sub64(x_26084, (int64_t) 1);
                                    
                                    x_26078 = defunc_1_f_res_res_26085;
                                }
                                x_26074 = x_26078;
                            }
                            defunc_1_f_res_26071 = x_26074;
                        }
                        
                        bool less_than_zzero_27604 = slt64(defunc_1_f_res_26071,
                                                           (int64_t) 0);
                        bool greater_than_sizze_27605 = sle64(conc_tmp_25985,
                                                              defunc_1_f_res_26071);
                        bool outside_bounds_dim_27606 = less_than_zzero_27604 ||
                             greater_than_sizze_27605;
                        
                        if (!outside_bounds_dim_27606) {
                            memmove(mem_28059.mem + defunc_1_f_res_26071 *
                                    (int64_t) 8, mem_param_27985.mem +
                                    write_iter_27596 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                    }
                    
                    struct memblock mem_param_tmp_29015;
                    
                    mem_param_tmp_29015.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29015, &mem_28059,
                                     "mem_28059") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_27985,
                                     &mem_param_tmp_29015,
                                     "mem_param_tmp_29015") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29015,
                                       "mem_param_tmp_29015") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28059, "mem_28059") != 0)
                        return 1;
                }
                if (memblock_set(ctx, &defunc_2_radix_sort_res_mem_28076,
                                 &mem_param_27985, "mem_param_27985") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                    return 1;
                
                bool empty_slice_26086 = j_m_i_25986 == (int64_t) 0;
                bool i_lte_j_26087 = sle64((int64_t) 1, conc_tmp_25985);
                bool y_26088 = bounds_check_25994 && i_lte_j_26087;
                bool ok_or_empty_26089 = empty_slice_26086 || y_26088;
                bool index_certs_26090;
                
                if (!ok_or_empty_26089) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", (int64_t) 1,
                                  ":] out of bounds for array of shape [",
                                  conc_tmp_25985, "].",
                                  "-> #0  sparse.fut:79:97-106\n   #1  sparse.fut:122:30-123:82\n   #2  /prelude/soacs.fut:56:19-23\n   #3  /prelude/soacs.fut:56:3-37\n   #4  sparse.fut:120:19-125:50\n   #5  sparse.fut:162:23-49\n   #6  sparse.fut:232:17-60\n   #7  sparse.fut:223:1-233:48\n");
                    if (memblock_unref(ctx, &mem_param_27985,
                                       "mem_param_27985") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28076,
                                       "defunc_2_radix_sort_res_mem_28076") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                
                int64_t defunc_2_reduce_res_26093;
                int64_t redout_27610 = (int64_t) 0;
                
                for (int64_t i_27612 = 0; i_27612 < j_m_i_25986; i_27612++) {
                    int64_t slice_27923 = (int64_t) 1 + i_27612;
                    int64_t x_26108 =
                            ((int64_t *) defunc_2_radix_sort_res_mem_28076.mem)[slice_27923];
                    bool y_26101 = slt64(i_27612, conc_tmp_25985);
                    bool index_certs_26103;
                    
                    if (!y_26101) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", i_27612,
                                      "] out of bounds for array of shape [",
                                      conc_tmp_25985, "].",
                                      "-> #0  sparse.fut:79:41-51\n   #1  sparse.fut:79:4-106\n   #2  sparse.fut:122:30-123:82\n   #3  /prelude/soacs.fut:56:19-23\n   #4  /prelude/soacs.fut:56:3-37\n   #5  sparse.fut:120:19-125:50\n   #6  sparse.fut:162:23-49\n   #7  sparse.fut:232:17-60\n   #8  sparse.fut:223:1-233:48\n");
                        if (memblock_unref(ctx, &mem_param_27985,
                                           "mem_param_27985") != 0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &defunc_2_radix_sort_res_mem_28076,
                                           "defunc_2_radix_sort_res_mem_28076") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    int64_t y_26104 =
                            ((int64_t *) defunc_2_radix_sort_res_mem_28076.mem)[i_27612];
                    bool cond_26105 = x_26108 == y_26104;
                    int64_t defunc_1_op_res_26106;
                    
                    if (cond_26105) {
                        int64_t defunc_1_op_res_t_res_26107 = add64((int64_t) 1,
                                                                    redout_27610);
                        
                        defunc_1_op_res_26106 = defunc_1_op_res_t_res_26107;
                    } else {
                        defunc_1_op_res_26106 = redout_27610;
                    }
                    
                    int64_t redout_tmp_29027 = defunc_1_op_res_26106;
                    
                    redout_27610 = redout_tmp_29027;
                }
                defunc_2_reduce_res_26093 = redout_27610;
                if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28076,
                                   "defunc_2_radix_sort_res_mem_28076") != 0)
                    return 1;
                intersectSizze_res_f_res_25987 = defunc_2_reduce_res_26093;
                if (memblock_unref(ctx, &mem_param_27985, "mem_param_27985") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &defunc_2_radix_sort_res_mem_28076,
                                   "defunc_2_radix_sort_res_mem_28076") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_27980, "mem_27980") != 0)
                    return 1;
            }
            intersectSizze_res_25984 = intersectSizze_res_f_res_25987;
        }
        
        int64_t defunc_1_op_res_25935 = add64(intersectSizze_res_25984,
                                              scanacc_27614);
        
        ((int64_t *) mem_27971)[i_27616] = defunc_1_op_res_25935;
        
        int64_t scanacc_tmp_29012 = defunc_1_op_res_25935;
        
        scanacc_27614 = scanacc_tmp_29012;
    }
    discard_27618 = scanacc_27614;
    
    int64_t i_26110 = sub64(m_25923, (int64_t) 1);
    bool x_26111 = sle64((int64_t) 0, i_26110);
    bool y_26112 = slt64(i_26110, m_25923);
    bool bounds_check_26113 = x_26111 && y_26112;
    bool index_certs_26114;
    
    if (!bounds_check_26113) {
        ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                               "Index [", i_26110,
                               "] out of bounds for array of shape [", m_25923,
                               "].",
                               "-> #0  sparse.fut:126:26-37\n   #1  sparse.fut:162:23-49\n   #2  sparse.fut:232:17-60\n   #3  sparse.fut:223:1-233:48\n");
        if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
            return 1;
        return 1;
    }
    
    int64_t eWiseMultOffsets_res_26115 = ((int64_t *) mem_27971)[i_26110];
    bool bounds_invalid_upwards_26116 = slt64(eWiseMultOffsets_res_26115,
                                              (int64_t) 0);
    bool valid_26117 = !bounds_invalid_upwards_26116;
    bool range_valid_c_26118;
    
    if (!valid_26117) {
        ctx->error = msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                               "Range ", (int64_t) 0, "..", (int64_t) 1, "..<",
                               eWiseMultOffsets_res_26115, " is invalid.",
                               "-> #0  /prelude/array.fut:60:3-10\n   #1  /prelude/array.fut:70:18-23\n   #2  sparse.fut:163:16-35\n   #3  sparse.fut:232:17-60\n   #4  sparse.fut:223:1-233:48\n");
        if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
            return 1;
        if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
            return 1;
        return 1;
    }
    
    int64_t bytes_28082 = (int64_t) 8 * eWiseMultOffsets_res_26115;
    
    if (mem_28083_cached_sizze_29214 < (size_t) bytes_28082) {
        mem_28083 = realloc(mem_28083, bytes_28082);
        mem_28083_cached_sizze_29214 = bytes_28082;
    }
    for (int64_t i_29028 = 0; i_29028 < eWiseMultOffsets_res_26115; i_29028++) {
        ((double *) mem_28083)[i_29028] = 0.0;
    }
    if (mem_28085_cached_sizze_29215 < (size_t) bytes_28082) {
        mem_28085 = realloc(mem_28085, bytes_28082);
        mem_28085_cached_sizze_29215 = bytes_28082;
    }
    for (int64_t i_29029 = 0; i_29029 < eWiseMultOffsets_res_26115; i_29029++) {
        ((int64_t *) mem_28085)[i_29029] = (int64_t) 0;
    }
    
    struct memblock mem_28087;
    
    mem_28087.references = NULL;
    if (memblock_alloc(ctx, &mem_28087, (int64_t) 0, "mem_28087")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_26124 = 0; i_26124 < i_26110; i_26124++) {
        bool y_26127 = slt64(i_26124, m_25923);
        bool index_certs_26128;
        
        if (!y_26127) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", i_26124,
                                   "] out of bounds for array of shape [",
                                   m_25923, "].",
                                   "-> #0  sparse.fut:171:46-61\n   #1  sparse.fut:232:17-60\n   #2  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_26129 = ((int64_t *) rightOffsets_mem_27969.mem)[i_26124];
        int64_t i_26130 = add64((int64_t) 1, i_26124);
        bool x_26131 = sle64((int64_t) 0, i_26130);
        bool y_26132 = slt64(i_26130, m_25923);
        bool bounds_check_26133 = x_26131 && y_26132;
        bool index_certs_26134;
        
        if (!bounds_check_26133) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Index [", i_26130,
                                   "] out of bounds for array of shape [",
                                   m_25923, "].",
                                   "-> #0  sparse.fut:171:63-80\n   #1  sparse.fut:232:17-60\n   #2  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t j_26135 = ((int64_t *) rightOffsets_mem_27969.mem)[i_26130];
        int64_t j_m_i_26136 = sub64(j_26135, i_26129);
        bool empty_slice_26137 = j_m_i_26136 == (int64_t) 0;
        int64_t m_26138 = sub64(j_m_i_26136, (int64_t) 1);
        int64_t i_p_m_t_s_26139 = add64(i_26129, m_26138);
        bool zzero_leq_i_p_m_t_s_26140 = sle64((int64_t) 0, i_p_m_t_s_26139);
        bool i_p_m_t_s_leq_w_26141 = slt64(i_p_m_t_s_26139, k_25924);
        bool zzero_lte_i_26142 = sle64((int64_t) 0, i_26129);
        bool i_lte_j_26143 = sle64(i_26129, j_26135);
        bool y_26144 = i_p_m_t_s_leq_w_26141 && zzero_lte_i_26142;
        bool y_26145 = zzero_leq_i_p_m_t_s_26140 && y_26144;
        bool y_26146 = i_lte_j_26143 && y_26145;
        bool forwards_ok_26147 = zzero_lte_i_26142 && y_26146;
        bool ok_or_empty_26148 = empty_slice_26137 || forwards_ok_26147;
        bool index_certs_26149;
        
        if (!ok_or_empty_26148) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_26129, ":", j_26135,
                          "] out of bounds for array of shape [", k_25924, "].",
                          "-> #0  sparse.fut:171:32-81\n   #1  sparse.fut:232:17-60\n   #2  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t i_26151 = ((int64_t *) leftOffsets_mem_27966.mem)[i_26124];
        int64_t j_26152 = ((int64_t *) leftOffsets_mem_27966.mem)[i_26130];
        int64_t j_m_i_26153 = sub64(j_26152, i_26151);
        bool empty_slice_26154 = j_m_i_26153 == (int64_t) 0;
        int64_t m_26155 = sub64(j_m_i_26153, (int64_t) 1);
        int64_t i_p_m_t_s_26156 = add64(i_26151, m_26155);
        bool zzero_leq_i_p_m_t_s_26157 = sle64((int64_t) 0, i_p_m_t_s_26156);
        bool i_p_m_t_s_leq_w_26158 = slt64(i_p_m_t_s_26156, n_25922);
        bool zzero_lte_i_26159 = sle64((int64_t) 0, i_26151);
        bool i_lte_j_26160 = sle64(i_26151, j_26152);
        bool y_26161 = i_p_m_t_s_leq_w_26158 && zzero_lte_i_26159;
        bool y_26162 = zzero_leq_i_p_m_t_s_26157 && y_26161;
        bool y_26163 = i_lte_j_26160 && y_26162;
        bool forwards_ok_26164 = zzero_lte_i_26159 && y_26163;
        bool ok_or_empty_26165 = empty_slice_26154 || forwards_ok_26164;
        bool index_certs_26166;
        
        if (!ok_or_empty_26165) {
            ctx->error =
                msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s",
                          "Index [", i_26151, ":", j_26152,
                          "] out of bounds for array of shape [", n_25922, "].",
                          "-> #0  sparse.fut:170:32-78\n   #1  sparse.fut:232:17-60\n   #2  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t conc_tmp_26168 = j_m_i_26136 + j_m_i_26153;
        int64_t j_m_i_26169 = sub64(conc_tmp_26168, (int64_t) 1);
        int64_t bytes_28102 = (int64_t) 8 * conc_tmp_26168;
        int64_t sizze_26170;
        int64_t sizze_26171;
        int64_t sizze_26172;
        int64_t intersectArrays_res_ixfn_28397;
        int64_t intersectArrays_res_ixfn_28398;
        int64_t intersectArrays_res_ixfn_28402;
        int64_t intersectArrays_res_ixfn_28403;
        int64_t intersectArrays_res_ixfn_28407;
        int64_t intersectArrays_res_ixfn_28408;
        struct memblock intersectArrays_res_mem_28401;
        
        intersectArrays_res_mem_28401.references = NULL;
        
        struct memblock intersectArrays_res_mem_28406;
        
        intersectArrays_res_mem_28406.references = NULL;
        
        struct memblock intersectArrays_res_mem_28411;
        
        intersectArrays_res_mem_28411.references = NULL;
        if (empty_slice_26154) {
            sizze_26170 = (int64_t) 0;
            sizze_26171 = (int64_t) 0;
            sizze_26172 = (int64_t) 0;
            intersectArrays_res_ixfn_28397 = (int64_t) 0;
            intersectArrays_res_ixfn_28398 = (int64_t) 0;
            intersectArrays_res_ixfn_28402 = (int64_t) 0;
            intersectArrays_res_ixfn_28403 = (int64_t) 0;
            intersectArrays_res_ixfn_28407 = (int64_t) 0;
            intersectArrays_res_ixfn_28408 = (int64_t) 0;
            if (memblock_set(ctx, &intersectArrays_res_mem_28401, &mem_28087,
                             "mem_28087") != 0)
                return 1;
            if (memblock_set(ctx, &intersectArrays_res_mem_28406, &mem_28087,
                             "mem_28087") != 0)
                return 1;
            if (memblock_set(ctx, &intersectArrays_res_mem_28411, &mem_28087,
                             "mem_28087") != 0)
                return 1;
        } else {
            struct memblock mem_28103;
            
            mem_28103.references = NULL;
            if (memblock_alloc(ctx, &mem_28103, bytes_28102, "mem_28103")) {
                err = 1;
                goto cleanup;
            }
            
            int64_t tmp_offs_29032 = (int64_t) 0;
            
            memmove(mem_28103.mem + tmp_offs_29032 * (int64_t) 8,
                    leftColumns_mem_27965.mem + i_26151 * (int64_t) 8,
                    j_m_i_26153 * (int64_t) sizeof(int64_t));
            tmp_offs_29032 += j_m_i_26153;
            memmove(mem_28103.mem + tmp_offs_29032 * (int64_t) 8,
                    rightColumns_mem_27968.mem + i_26129 * (int64_t) 8,
                    j_m_i_26136 * (int64_t) sizeof(int64_t));
            tmp_offs_29032 += j_m_i_26136;
            
            struct memblock mem_28105;
            
            mem_28105.references = NULL;
            if (memblock_alloc(ctx, &mem_28105, bytes_28102, "mem_28105")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_29033 = 0; i_29033 < conc_tmp_26168; i_29033++) {
                int64_t x_29034 = (int64_t) 0 + i_29033 * (int64_t) 1;
                
                ((int64_t *) mem_28105.mem)[i_29033] = x_29034;
            }
            
            bool cond_26178 = conc_tmp_26168 == (int64_t) 0;
            int32_t iters_26179;
            
            if (cond_26178) {
                iters_26179 = 0;
            } else {
                iters_26179 = 32;
            }
            
            bool loop_nonempty_26180 = slt32(0, iters_26179);
            bool x_26181 = sle64((int64_t) 0, j_m_i_26169);
            bool y_26182 = slt64(j_m_i_26169, conc_tmp_26168);
            bool bounds_check_26183 = x_26181 && y_26182;
            bool loop_not_taken_26184 = !loop_nonempty_26180;
            bool protect_assert_disj_26185 = bounds_check_26183 ||
                 loop_not_taken_26184;
            int64_t last_index_26187 = j_m_i_26169 - (int64_t) 1;
            bool is_empty_26188 = j_m_i_26169 == (int64_t) 0;
            
            if (mem_28107_cached_sizze_29216 < (size_t) bytes_28102) {
                mem_28107 = realloc(mem_28107, bytes_28102);
                mem_28107_cached_sizze_29216 = bytes_28102;
            }
            if (mem_28109_cached_sizze_29217 < (size_t) bytes_28102) {
                mem_28109 = realloc(mem_28109, bytes_28102);
                mem_28109_cached_sizze_29217 = bytes_28102;
            }
            if (mem_28111_cached_sizze_29218 < (size_t) bytes_28102) {
                mem_28111 = realloc(mem_28111, bytes_28102);
                mem_28111_cached_sizze_29218 = bytes_28102;
            }
            
            bool i_lte_j_26310 = sle64((int64_t) 1, conc_tmp_26168);
            bool y_26311 = bounds_check_26183 && i_lte_j_26310;
            bool ok_or_empty_26312 = is_empty_26188 || y_26311;
            int64_t bytes_28134 = (int64_t) 4 * conc_tmp_26168;
            int64_t intersectArrays_res_f_res_ixfn_28383;
            
            if (empty_slice_26137) {
                intersectArrays_res_f_res_ixfn_28383 = (int64_t) 0;
            } else {
                intersectArrays_res_f_res_ixfn_28383 = j_m_i_26169;
            }
            
            int64_t intersectArrays_res_f_res_ixfn_28388;
            
            if (empty_slice_26137) {
                intersectArrays_res_f_res_ixfn_28388 = (int64_t) 0;
            } else {
                intersectArrays_res_f_res_ixfn_28388 = j_m_i_26169;
            }
            
            int64_t intersectArrays_res_f_res_ixfn_28393;
            
            if (empty_slice_26137) {
                intersectArrays_res_f_res_ixfn_28393 = (int64_t) 0;
            } else {
                intersectArrays_res_f_res_ixfn_28393 = j_m_i_26169;
            }
            
            int64_t sizze_26189;
            int64_t sizze_26190;
            int64_t sizze_26191;
            int64_t intersectArrays_res_f_res_ixfn_28382;
            int64_t intersectArrays_res_f_res_ixfn_28387;
            int64_t intersectArrays_res_f_res_ixfn_28392;
            struct memblock intersectArrays_res_f_res_mem_28386;
            
            intersectArrays_res_f_res_mem_28386.references = NULL;
            
            struct memblock intersectArrays_res_f_res_mem_28391;
            
            intersectArrays_res_f_res_mem_28391.references = NULL;
            
            struct memblock intersectArrays_res_f_res_mem_28396;
            
            intersectArrays_res_f_res_mem_28396.references = NULL;
            if (empty_slice_26137) {
                sizze_26189 = (int64_t) 0;
                sizze_26190 = (int64_t) 0;
                sizze_26191 = (int64_t) 0;
                intersectArrays_res_f_res_ixfn_28382 = (int64_t) 0;
                intersectArrays_res_f_res_ixfn_28387 = (int64_t) 0;
                intersectArrays_res_f_res_ixfn_28392 = (int64_t) 0;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28386,
                                 &mem_28087, "mem_28087") != 0)
                    return 1;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28391,
                                 &mem_28087, "mem_28087") != 0)
                    return 1;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28396,
                                 &mem_28087, "mem_28087") != 0)
                    return 1;
            } else {
                bool index_certs_26195;
                
                if (!protect_assert_disj_26185) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", j_m_i_26169,
                                  "] out of bounds for array of shape [",
                                  conc_tmp_26168, "].",
                                  "-> #0  /prelude/array.fut:18:29-34\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:25:24-35\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:53:31-62\n   #3  /prelude/functional.fut:9:42-44\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:59:3-61:47\n   #5  lib/github.com/diku-dk/sorts/radix_sort.fut:68:3-51\n   #6  sparse.fut:5:34-6:77\n   #7  sparse.fut:86:12-35\n   #8  sparse.fut:169:49-171:81\n   #9  sparse.fut:232:17-60\n   #10 sparse.fut:223:1-233:48\n");
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28396,
                                       "intersectArrays_res_f_res_mem_28396") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28391,
                                       "intersectArrays_res_f_res_mem_28391") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28386,
                                       "intersectArrays_res_f_res_mem_28386") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                                       "intersectArrays_res_mem_28411") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                                       "intersectArrays_res_mem_28406") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                                       "intersectArrays_res_mem_28401") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                if (mem_28127_cached_sizze_29219 < (size_t) bytes_28102) {
                    mem_28127 = realloc(mem_28127, bytes_28102);
                    mem_28127_cached_sizze_29219 = bytes_28102;
                }
                if (mem_28129_cached_sizze_29220 < (size_t) bytes_28102) {
                    mem_28129 = realloc(mem_28129, bytes_28102);
                    mem_28129_cached_sizze_29220 = bytes_28102;
                }
                if (mem_28131_cached_sizze_29221 < (size_t) bytes_28102) {
                    mem_28131 = realloc(mem_28131, bytes_28102);
                    mem_28131_cached_sizze_29221 = bytes_28102;
                }
                if (mem_28133_cached_sizze_29222 < (size_t) bytes_28102) {
                    mem_28133 = realloc(mem_28133, bytes_28102);
                    mem_28133_cached_sizze_29222 = bytes_28102;
                }
                if (mem_28135_cached_sizze_29223 < (size_t) bytes_28134) {
                    mem_28135 = realloc(mem_28135, bytes_28134);
                    mem_28135_cached_sizze_29223 = bytes_28134;
                }
                
                struct memblock defunc_0_f_res_mem_28228;
                
                defunc_0_f_res_mem_28228.references = NULL;
                
                struct memblock defunc_0_f_res_mem_28233;
                
                defunc_0_f_res_mem_28233.references = NULL;
                
                struct memblock mem_param_28116;
                
                mem_param_28116.references = NULL;
                
                struct memblock mem_param_28121;
                
                mem_param_28121.references = NULL;
                if (memblock_set(ctx, &mem_param_28116, &mem_28103,
                                 "mem_28103") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_28121, &mem_28105,
                                 "mem_28105") != 0)
                    return 1;
                for (int32_t i_26198 = 0; i_26198 < iters_26179; i_26198++) {
                    int32_t defunc_2_radix_sort_step_arg_26201 = mul32(2,
                                                                       i_26198);
                    int32_t defunc_0_get_bit_arg_26202 = add32(1,
                                                               defunc_2_radix_sort_step_arg_26201);
                    int64_t i32_res_26203 =
                            sext_i32_i64(defunc_0_get_bit_arg_26202);
                    int64_t i32_res_26204 =
                            sext_i32_i64(defunc_2_radix_sort_step_arg_26201);
                    int64_t discard_27639;
                    int64_t discard_27640;
                    int64_t discard_27641;
                    int64_t discard_27642;
                    int64_t scanacc_27624;
                    int64_t scanacc_27625;
                    int64_t scanacc_27626;
                    int64_t scanacc_27627;
                    
                    scanacc_27624 = (int64_t) 0;
                    scanacc_27625 = (int64_t) 0;
                    scanacc_27626 = (int64_t) 0;
                    scanacc_27627 = (int64_t) 0;
                    for (int64_t i_27633 = 0; i_27633 < conc_tmp_26168;
                         i_27633++) {
                        int64_t x_26232 =
                                ((int64_t *) mem_param_28116.mem)[i_27633];
                        int64_t zgzg_res_26233 = ashr64(x_26232, i32_res_26203);
                        int64_t za_res_26234 = (int64_t) 1 & zgzg_res_26233;
                        int32_t to_i32_res_26235 = sext_i64_i32(za_res_26234);
                        int32_t x_26236 = mul32(2, to_i32_res_26235);
                        int64_t zgzg_res_26237 = ashr64(x_26232, i32_res_26204);
                        int64_t za_res_26238 = (int64_t) 1 & zgzg_res_26237;
                        int32_t to_i32_res_26239 = sext_i64_i32(za_res_26238);
                        int32_t defunc_0_f_res_26240 = add32(x_26236,
                                                             to_i32_res_26239);
                        bool cond_26241 = defunc_0_f_res_26240 == 0;
                        int64_t defunc_0_f_res_26242 =
                                btoi_bool_i64(cond_26241);
                        int64_t defunc_0_f_res_26243;
                        int64_t defunc_0_f_res_26244;
                        int64_t defunc_0_f_res_26245;
                        
                        if (cond_26241) {
                            defunc_0_f_res_26243 = (int64_t) 0;
                            defunc_0_f_res_26244 = (int64_t) 0;
                            defunc_0_f_res_26245 = (int64_t) 0;
                        } else {
                            bool cond_26246 = defunc_0_f_res_26240 == 1;
                            int64_t defunc_0_f_res_f_res_26247 =
                                    btoi_bool_i64(cond_26246);
                            int64_t defunc_0_f_res_f_res_26248;
                            int64_t defunc_0_f_res_f_res_26249;
                            
                            if (cond_26246) {
                                defunc_0_f_res_f_res_26248 = (int64_t) 0;
                                defunc_0_f_res_f_res_26249 = (int64_t) 0;
                            } else {
                                bool cond_26250 = defunc_0_f_res_26240 == 2;
                                int64_t defunc_0_f_res_f_res_f_res_26251 =
                                        btoi_bool_i64(cond_26250);
                                bool cond_neg_26252 = !cond_26250;
                                int64_t defunc_0_f_res_f_res_f_res_26253 =
                                        btoi_bool_i64(cond_neg_26252);
                                
                                defunc_0_f_res_f_res_26248 =
                                    defunc_0_f_res_f_res_f_res_26251;
                                defunc_0_f_res_f_res_26249 =
                                    defunc_0_f_res_f_res_f_res_26253;
                            }
                            defunc_0_f_res_26243 = defunc_0_f_res_f_res_26247;
                            defunc_0_f_res_26244 = defunc_0_f_res_f_res_26248;
                            defunc_0_f_res_26245 = defunc_0_f_res_f_res_26249;
                        }
                        
                        int64_t defunc_1_op_res_26228 =
                                add64(defunc_0_f_res_26242, scanacc_27624);
                        int64_t defunc_1_op_res_26229 =
                                add64(defunc_0_f_res_26243, scanacc_27625);
                        int64_t defunc_1_op_res_26230 =
                                add64(defunc_0_f_res_26244, scanacc_27626);
                        int64_t defunc_1_op_res_26231 =
                                add64(defunc_0_f_res_26245, scanacc_27627);
                        
                        ((int64_t *) mem_28127)[i_27633] =
                            defunc_1_op_res_26228;
                        ((int64_t *) mem_28129)[i_27633] =
                            defunc_1_op_res_26229;
                        ((int64_t *) mem_28131)[i_27633] =
                            defunc_1_op_res_26230;
                        ((int64_t *) mem_28133)[i_27633] =
                            defunc_1_op_res_26231;
                        ((int32_t *) mem_28135)[i_27633] = defunc_0_f_res_26240;
                        
                        int64_t scanacc_tmp_29039 = defunc_1_op_res_26228;
                        int64_t scanacc_tmp_29040 = defunc_1_op_res_26229;
                        int64_t scanacc_tmp_29041 = defunc_1_op_res_26230;
                        int64_t scanacc_tmp_29042 = defunc_1_op_res_26231;
                        
                        scanacc_27624 = scanacc_tmp_29039;
                        scanacc_27625 = scanacc_tmp_29040;
                        scanacc_27626 = scanacc_tmp_29041;
                        scanacc_27627 = scanacc_tmp_29042;
                    }
                    discard_27639 = scanacc_27624;
                    discard_27640 = scanacc_27625;
                    discard_27641 = scanacc_27626;
                    discard_27642 = scanacc_27627;
                    
                    int64_t last_res_26259 =
                            ((int64_t *) mem_28127)[j_m_i_26169];
                    int64_t last_res_26260 =
                            ((int64_t *) mem_28129)[j_m_i_26169];
                    int64_t last_res_26261 =
                            ((int64_t *) mem_28131)[j_m_i_26169];
                    struct memblock mem_28197;
                    
                    mem_28197.references = NULL;
                    if (memblock_alloc(ctx, &mem_28197, bytes_28102,
                                       "mem_28197")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28197.mem + (int64_t) 0, mem_param_28116.mem +
                            (int64_t) 0, conc_tmp_26168 *
                            (int64_t) sizeof(int64_t));
                    
                    struct memblock mem_28199;
                    
                    mem_28199.references = NULL;
                    if (memblock_alloc(ctx, &mem_28199, bytes_28102,
                                       "mem_28199")) {
                        err = 1;
                        goto cleanup;
                    }
                    memmove(mem_28199.mem + (int64_t) 0, mem_param_28121.mem +
                            (int64_t) 0, conc_tmp_26168 *
                            (int64_t) sizeof(int64_t));
                    for (int64_t write_iter_27643 = 0; write_iter_27643 <
                         conc_tmp_26168; write_iter_27643++) {
                        int32_t write_iv_27646 =
                                ((int32_t *) mem_28135)[write_iter_27643];
                        int64_t write_iv_27647 =
                                ((int64_t *) mem_28127)[write_iter_27643];
                        int64_t write_iv_27648 =
                                ((int64_t *) mem_28129)[write_iter_27643];
                        int64_t write_iv_27649 =
                                ((int64_t *) mem_28131)[write_iter_27643];
                        int64_t write_iv_27650 =
                                ((int64_t *) mem_28133)[write_iter_27643];
                        bool match_lit_26273 = 0 == write_iv_27646;
                        int64_t defunc_1_f_res_26274;
                        
                        if (match_lit_26273) {
                            int64_t case_res_26275 = sub64(write_iv_27647,
                                                           (int64_t) 1);
                            
                            defunc_1_f_res_26274 = case_res_26275;
                        } else {
                            bool match_lit_26276 = 1 == write_iv_27646;
                            int64_t x_26277;
                            
                            if (match_lit_26276) {
                                int64_t x_26278 = add64(last_res_26259,
                                                        write_iv_27648);
                                int64_t case_res_26279 = sub64(x_26278,
                                                               (int64_t) 1);
                                
                                x_26277 = case_res_26279;
                            } else {
                                bool match_lit_26280 = 2 == write_iv_27646;
                                int64_t x_26281;
                                
                                if (match_lit_26280) {
                                    int64_t x_26282 = add64(last_res_26259,
                                                            last_res_26260);
                                    int64_t x_26283 = add64(x_26282,
                                                            write_iv_27649);
                                    int64_t case_res_26284 = sub64(x_26283,
                                                                   (int64_t) 1);
                                    
                                    x_26281 = case_res_26284;
                                } else {
                                    int64_t x_26285 = add64(last_res_26259,
                                                            last_res_26260);
                                    int64_t x_26286 = add64(last_res_26261,
                                                            x_26285);
                                    int64_t x_26287 = add64(x_26286,
                                                            write_iv_27650);
                                    int64_t defunc_1_f_res_res_26288 =
                                            sub64(x_26287, (int64_t) 1);
                                    
                                    x_26281 = defunc_1_f_res_res_26288;
                                }
                                x_26277 = x_26281;
                            }
                            defunc_1_f_res_26274 = x_26277;
                        }
                        
                        bool less_than_zzero_27653 = slt64(defunc_1_f_res_26274,
                                                           (int64_t) 0);
                        bool greater_than_sizze_27654 = sle64(conc_tmp_26168,
                                                              defunc_1_f_res_26274);
                        bool outside_bounds_dim_27655 = less_than_zzero_27653 ||
                             greater_than_sizze_27654;
                        
                        if (!outside_bounds_dim_27655) {
                            memmove(mem_28197.mem + defunc_1_f_res_26274 *
                                    (int64_t) 8, mem_param_28116.mem +
                                    write_iter_27643 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                        if (!outside_bounds_dim_27655) {
                            memmove(mem_28199.mem + defunc_1_f_res_26274 *
                                    (int64_t) 8, mem_param_28121.mem +
                                    write_iter_27643 * (int64_t) 8,
                                    (int64_t) sizeof(int64_t));
                        }
                    }
                    
                    struct memblock mem_param_tmp_29035;
                    
                    mem_param_tmp_29035.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29035, &mem_28197,
                                     "mem_28197") != 0)
                        return 1;
                    
                    struct memblock mem_param_tmp_29036;
                    
                    mem_param_tmp_29036.references = NULL;
                    if (memblock_set(ctx, &mem_param_tmp_29036, &mem_28199,
                                     "mem_28199") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_28116,
                                     &mem_param_tmp_29035,
                                     "mem_param_tmp_29035") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_28121,
                                     &mem_param_tmp_29036,
                                     "mem_param_tmp_29036") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29036,
                                       "mem_param_tmp_29036") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_tmp_29035,
                                       "mem_param_tmp_29035") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28199, "mem_28199") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28197, "mem_28197") != 0)
                        return 1;
                }
                if (memblock_set(ctx, &defunc_0_f_res_mem_28228,
                                 &mem_param_28116, "mem_param_28116") != 0)
                    return 1;
                if (memblock_set(ctx, &defunc_0_f_res_mem_28233,
                                 &mem_param_28121, "mem_param_28121") != 0)
                    return 1;
                for (int64_t i_27671 = 0; i_27671 < conc_tmp_26168; i_27671++) {
                    int64_t x_26292 =
                            ((int64_t *) defunc_0_f_res_mem_28233.mem)[i_27671];
                    bool x_26293 = sle64((int64_t) 0, x_26292);
                    bool y_26294 = slt64(x_26292, conc_tmp_26168);
                    bool bounds_check_26295 = x_26293 && y_26294;
                    bool index_certs_26296;
                    
                    if (!bounds_check_26295) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", x_26292,
                                      "] out of bounds for array of shape [",
                                      conc_tmp_26168, "].",
                                      "-> #0  lib/github.com/diku-dk/sorts/radix_sort.fut:62:28-32\n   #1  /prelude/functional.fut:9:42-44\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:59:3-62:33\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:68:3-51\n   #4  sparse.fut:5:34-6:77\n   #5  sparse.fut:86:12-35\n   #6  sparse.fut:169:49-171:81\n   #7  sparse.fut:232:17-60\n   #8  sparse.fut:223:1-233:48\n");
                        if (memblock_unref(ctx, &mem_param_28121,
                                           "mem_param_28121") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_param_28116,
                                           "mem_param_28116") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28233,
                                           "defunc_0_f_res_mem_28233") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28228,
                                           "defunc_0_f_res_mem_28228") != 0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28396,
                                           "intersectArrays_res_f_res_mem_28396") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28391,
                                           "intersectArrays_res_f_res_mem_28391") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28386,
                                           "intersectArrays_res_f_res_mem_28386") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                                           "intersectArrays_res_mem_28411") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                                           "intersectArrays_res_mem_28406") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                                           "intersectArrays_res_mem_28401") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    bool index_concat_cmp_26297 = sle64(j_m_i_26153, x_26292);
                    int64_t index_concat_branch_26298;
                    
                    if (index_concat_cmp_26297) {
                        int64_t index_concat_i_26299 = sub64(x_26292,
                                                             j_m_i_26153);
                        int64_t slice_26300 = i_26129 + index_concat_i_26299;
                        int64_t index_concat_26301 =
                                ((int64_t *) rightColumns_mem_27968.mem)[slice_26300];
                        
                        index_concat_branch_26298 = index_concat_26301;
                    } else {
                        int64_t slice_26302 = i_26151 + x_26292;
                        int64_t index_concat_26303 =
                                ((int64_t *) leftColumns_mem_27965.mem)[slice_26302];
                        
                        index_concat_branch_26298 = index_concat_26303;
                    }
                    
                    int64_t index_concat_branch_26305 =
                            btoi_bool_i64(index_concat_cmp_26297);
                    int64_t index_concat_branch_26307;
                    
                    if (index_concat_cmp_26297) {
                        int64_t index_concat_i_26308 = sub64(x_26292,
                                                             j_m_i_26153);
                        
                        index_concat_branch_26307 = index_concat_i_26308;
                    } else {
                        index_concat_branch_26307 = x_26292;
                    }
                    ((int64_t *) mem_28107)[i_27671] =
                        index_concat_branch_26298;
                    ((int64_t *) mem_28109)[i_27671] =
                        index_concat_branch_26305;
                    ((int64_t *) mem_28111)[i_27671] =
                        index_concat_branch_26307;
                }
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28233,
                                   "defunc_0_f_res_mem_28233") != 0)
                    return 1;
                
                bool index_certs_26313;
                
                if (!ok_or_empty_26312) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [", (int64_t) 1,
                                  ":] out of bounds for array of shape [",
                                  conc_tmp_26168, "].",
                                  "-> #0  sparse.fut:100:59-68\n   #1  sparse.fut:169:49-171:81\n   #2  sparse.fut:232:17-60\n   #3  sparse.fut:223:1-233:48\n");
                    if (memblock_unref(ctx, &mem_param_28121,
                                       "mem_param_28121") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_28116,
                                       "mem_param_28116") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28233,
                                       "defunc_0_f_res_mem_28233") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28228,
                                       "defunc_0_f_res_mem_28228") != 0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28396,
                                       "intersectArrays_res_f_res_mem_28396") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28391,
                                       "intersectArrays_res_f_res_mem_28391") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28386,
                                       "intersectArrays_res_f_res_mem_28386") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                                       "intersectArrays_res_mem_28411") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                                       "intersectArrays_res_mem_28406") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                                       "intersectArrays_res_mem_28401") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                
                int64_t bytes_28270 = (int64_t) 8 * j_m_i_26169;
                
                if (mem_28271_cached_sizze_29224 < (size_t) bytes_28270) {
                    mem_28271 = realloc(mem_28271, bytes_28270);
                    mem_28271_cached_sizze_29224 = bytes_28270;
                }
                if (mem_28273_cached_sizze_29225 < (size_t) bytes_28270) {
                    mem_28273 = realloc(mem_28273, bytes_28270);
                    mem_28273_cached_sizze_29225 = bytes_28270;
                }
                if (mem_28275_cached_sizze_29226 < (size_t) bytes_28270) {
                    mem_28275 = realloc(mem_28275, bytes_28270);
                    mem_28275_cached_sizze_29226 = bytes_28270;
                }
                if (mem_28277_cached_sizze_29227 < (size_t) bytes_28270) {
                    mem_28277 = realloc(mem_28277, bytes_28270);
                    mem_28277_cached_sizze_29227 = bytes_28270;
                }
                if (mem_28279_cached_sizze_29228 < (size_t) bytes_28270) {
                    mem_28279 = realloc(mem_28279, bytes_28270);
                    mem_28279_cached_sizze_29228 = bytes_28270;
                }
                
                int64_t discard_27692;
                int64_t scanacc_27680 = (int64_t) 0;
                
                for (int64_t i_27686 = 0; i_27686 < j_m_i_26169; i_27686++) {
                    int64_t slice_27932 = (int64_t) 1 + i_27686;
                    int64_t x_26347 = ((int64_t *) mem_28107)[slice_27932];
                    int64_t x_26348 = ((int64_t *) mem_28109)[slice_27932];
                    int64_t x_26349 = ((int64_t *) mem_28111)[slice_27932];
                    bool y_26353 = slt64(i_27686, conc_tmp_26168);
                    bool index_certs_26355;
                    
                    if (!y_26353) {
                        ctx->error =
                            msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                      "Index [", i_27686,
                                      "] out of bounds for array of shape [",
                                      conc_tmp_26168, "].",
                                      "-> #0  sparse.fut:91:47-57\n   #1  sparse.fut:90:31-100:68\n   #2  sparse.fut:169:49-171:81\n   #3  sparse.fut:232:17-60\n   #4  sparse.fut:223:1-233:48\n");
                        if (memblock_unref(ctx, &mem_param_28121,
                                           "mem_param_28121") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_param_28116,
                                           "mem_param_28116") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28233,
                                           "defunc_0_f_res_mem_28233") != 0)
                            return 1;
                        if (memblock_unref(ctx, &defunc_0_f_res_mem_28228,
                                           "defunc_0_f_res_mem_28228") != 0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28396,
                                           "intersectArrays_res_f_res_mem_28396") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28391,
                                           "intersectArrays_res_f_res_mem_28391") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx,
                                           &intersectArrays_res_f_res_mem_28386,
                                           "intersectArrays_res_f_res_mem_28386") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                                           "intersectArrays_res_mem_28411") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                                           "intersectArrays_res_mem_28406") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                                           "intersectArrays_res_mem_28401") !=
                            0)
                            return 1;
                        if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29011,
                                           "out_mem_29011") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29009,
                                           "out_mem_29009") != 0)
                            return 1;
                        if (memblock_unref(ctx, &out_mem_29007,
                                           "out_mem_29007") != 0)
                            return 1;
                        return 1;
                    }
                    
                    int64_t y_26356 = ((int64_t *) mem_28107)[i_27686];
                    bool cond_26357 = x_26347 == y_26356;
                    int64_t defunc_0_f_res_26358;
                    
                    if (cond_26357) {
                        defunc_0_f_res_26358 = x_26347;
                    } else {
                        defunc_0_f_res_26358 = (int64_t) -1;
                    }
                    
                    int64_t defunc_0_f_res_26359;
                    int64_t defunc_0_f_res_26360;
                    
                    if (cond_26357) {
                        bool cond_26361 = x_26348 == (int64_t) 0;
                        int64_t defunc_0_f_res_t_res_26362;
                        int64_t defunc_0_f_res_t_res_26363;
                        
                        if (cond_26361) {
                            int64_t defunc_0_f_res_t_res_t_res_26364 =
                                    ((int64_t *) mem_28111)[i_27686];
                            
                            defunc_0_f_res_t_res_26362 = x_26349;
                            defunc_0_f_res_t_res_26363 =
                                defunc_0_f_res_t_res_t_res_26364;
                        } else {
                            int64_t defunc_0_f_res_t_res_f_res_26365 =
                                    ((int64_t *) mem_28111)[i_27686];
                            
                            defunc_0_f_res_t_res_26362 =
                                defunc_0_f_res_t_res_f_res_26365;
                            defunc_0_f_res_t_res_26363 = x_26349;
                        }
                        defunc_0_f_res_26359 = defunc_0_f_res_t_res_26362;
                        defunc_0_f_res_26360 = defunc_0_f_res_t_res_26363;
                    } else {
                        defunc_0_f_res_26359 = (int64_t) 0;
                        defunc_0_f_res_26360 = (int64_t) 0;
                    }
                    
                    bool eq_x_y_26366 = (int64_t) -1 == x_26347;
                    bool p_and_eq_x_y_26367 = cond_26357 && eq_x_y_26366;
                    bool not_p_26368 = !cond_26357;
                    bool defunc_0_p_res_26369 = p_and_eq_x_y_26367 ||
                         not_p_26368;
                    int64_t defunc_1_lifted_lambda_res_26370 =
                            btoi_bool_i64(defunc_0_p_res_26369);
                    bool is_i_26371 = defunc_1_lifted_lambda_res_26370 ==
                         (int64_t) 0;
                    bool cond_neg_26372 = !is_i_26371;
                    int64_t part_res_26373 = btoi_bool_i64(cond_neg_26372);
                    int64_t part_res_26374 = btoi_bool_i64(is_i_26371);
                    int64_t zz_26346 = part_res_26374 + scanacc_27680;
                    
                    ((int64_t *) mem_28271)[i_27686] = zz_26346;
                    ((int64_t *) mem_28273)[i_27686] = part_res_26373;
                    ((int64_t *) mem_28275)[i_27686] = defunc_0_f_res_26358;
                    ((int64_t *) mem_28277)[i_27686] = defunc_0_f_res_26359;
                    ((int64_t *) mem_28279)[i_27686] = defunc_0_f_res_26360;
                    
                    int64_t scanacc_tmp_29053 = zz_26346;
                    
                    scanacc_27680 = scanacc_tmp_29053;
                }
                discard_27692 = scanacc_27680;
                
                int64_t partition_sizze_26380;
                
                if (is_empty_26188) {
                    partition_sizze_26380 = (int64_t) 0;
                } else {
                    int64_t last_offset_26381 =
                            ((int64_t *) mem_28271)[last_index_26187];
                    
                    partition_sizze_26380 = last_offset_26381;
                }
                
                struct memblock mem_28341;
                
                mem_28341.references = NULL;
                if (memblock_alloc(ctx, &mem_28341, bytes_28270, "mem_28341")) {
                    err = 1;
                    goto cleanup;
                }
                
                struct memblock mem_28343;
                
                mem_28343.references = NULL;
                if (memblock_alloc(ctx, &mem_28343, bytes_28270, "mem_28343")) {
                    err = 1;
                    goto cleanup;
                }
                
                struct memblock mem_28345;
                
                mem_28345.references = NULL;
                if (memblock_alloc(ctx, &mem_28345, bytes_28270, "mem_28345")) {
                    err = 1;
                    goto cleanup;
                }
                for (int64_t write_iter_27693 = 0; write_iter_27693 <
                     j_m_i_26169; write_iter_27693++) {
                    int64_t write_iv_27697 =
                            ((int64_t *) mem_28273)[write_iter_27693];
                    int64_t write_iv_27698 =
                            ((int64_t *) mem_28271)[write_iter_27693];
                    bool is_this_one_26393 = write_iv_27697 == (int64_t) 0;
                    int64_t this_offset_26394 = (int64_t) -1 + write_iv_27698;
                    int64_t total_res_26395;
                    
                    if (is_this_one_26393) {
                        total_res_26395 = this_offset_26394;
                    } else {
                        total_res_26395 = (int64_t) -1;
                    }
                    
                    bool less_than_zzero_27702 = slt64(total_res_26395,
                                                       (int64_t) 0);
                    bool greater_than_sizze_27703 = sle64(j_m_i_26169,
                                                          total_res_26395);
                    bool outside_bounds_dim_27704 = less_than_zzero_27702 ||
                         greater_than_sizze_27703;
                    
                    if (!outside_bounds_dim_27704) {
                        memmove(mem_28341.mem + total_res_26395 * (int64_t) 8,
                                mem_28275 + write_iter_27693 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                    if (!outside_bounds_dim_27704) {
                        memmove(mem_28343.mem + total_res_26395 * (int64_t) 8,
                                mem_28277 + write_iter_27693 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                    if (!outside_bounds_dim_27704) {
                        memmove(mem_28345.mem + total_res_26395 * (int64_t) 8,
                                mem_28279 + write_iter_27693 * (int64_t) 8,
                                (int64_t) sizeof(int64_t));
                    }
                }
                
                bool empty_slice_26396 = partition_sizze_26380 == (int64_t) 0;
                int64_t m_26397 = sub64(partition_sizze_26380, (int64_t) 1);
                bool zzero_leq_i_p_m_t_s_26398 = sle64((int64_t) 0, m_26397);
                bool i_p_m_t_s_leq_w_26399 = slt64(m_26397, j_m_i_26169);
                bool i_lte_j_26400 = sle64((int64_t) 0, partition_sizze_26380);
                bool y_26401 = zzero_leq_i_p_m_t_s_26398 &&
                     i_p_m_t_s_leq_w_26399;
                bool y_26402 = i_lte_j_26400 && y_26401;
                bool ok_or_empty_26403 = empty_slice_26396 || y_26402;
                bool index_certs_26404;
                
                if (!ok_or_empty_26403) {
                    ctx->error =
                        msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                  "Index [:", partition_sizze_26380,
                                  "] out of bounds for array of shape [",
                                  j_m_i_26169, "].",
                                  "-> #0  /prelude/soacs.fut:141:6-16\n   #1  sparse.fut:90:3-100:69\n   #2  sparse.fut:169:49-171:81\n   #3  sparse.fut:232:17-60\n   #4  sparse.fut:223:1-233:48\n");
                    if (memblock_unref(ctx, &mem_28345, "mem_28345") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28343, "mem_28343") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28341, "mem_28341") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_28121,
                                       "mem_param_28121") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_param_28116,
                                       "mem_param_28116") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28233,
                                       "defunc_0_f_res_mem_28233") != 0)
                        return 1;
                    if (memblock_unref(ctx, &defunc_0_f_res_mem_28228,
                                       "defunc_0_f_res_mem_28228") != 0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28396,
                                       "intersectArrays_res_f_res_mem_28396") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28391,
                                       "intersectArrays_res_f_res_mem_28391") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx,
                                       &intersectArrays_res_f_res_mem_28386,
                                       "intersectArrays_res_f_res_mem_28386") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                                       "intersectArrays_res_mem_28411") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                                       "intersectArrays_res_mem_28406") != 0)
                        return 1;
                    if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                                       "intersectArrays_res_mem_28401") != 0)
                        return 1;
                    if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") !=
                        0)
                        return 1;
                    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") !=
                        0)
                        return 1;
                    return 1;
                }
                sizze_26189 = partition_sizze_26380;
                sizze_26190 = partition_sizze_26380;
                sizze_26191 = partition_sizze_26380;
                intersectArrays_res_f_res_ixfn_28382 = partition_sizze_26380;
                intersectArrays_res_f_res_ixfn_28387 = partition_sizze_26380;
                intersectArrays_res_f_res_ixfn_28392 = partition_sizze_26380;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28386,
                                 &mem_28341, "mem_28341") != 0)
                    return 1;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28391,
                                 &mem_28343, "mem_28343") != 0)
                    return 1;
                if (memblock_set(ctx, &intersectArrays_res_f_res_mem_28396,
                                 &mem_28345, "mem_28345") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28345, "mem_28345") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28343, "mem_28343") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28341, "mem_28341") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_param_28121, "mem_param_28121") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &mem_param_28116, "mem_param_28116") !=
                    0)
                    return 1;
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28233,
                                   "defunc_0_f_res_mem_28233") != 0)
                    return 1;
                if (memblock_unref(ctx, &defunc_0_f_res_mem_28228,
                                   "defunc_0_f_res_mem_28228") != 0)
                    return 1;
            }
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            sizze_26170 = sizze_26189;
            sizze_26171 = sizze_26190;
            sizze_26172 = sizze_26191;
            intersectArrays_res_ixfn_28397 =
                intersectArrays_res_f_res_ixfn_28382;
            intersectArrays_res_ixfn_28398 =
                intersectArrays_res_f_res_ixfn_28383;
            intersectArrays_res_ixfn_28402 =
                intersectArrays_res_f_res_ixfn_28387;
            intersectArrays_res_ixfn_28403 =
                intersectArrays_res_f_res_ixfn_28388;
            intersectArrays_res_ixfn_28407 =
                intersectArrays_res_f_res_ixfn_28392;
            intersectArrays_res_ixfn_28408 =
                intersectArrays_res_f_res_ixfn_28393;
            if (memblock_set(ctx, &intersectArrays_res_mem_28401,
                             &intersectArrays_res_f_res_mem_28386,
                             "intersectArrays_res_f_res_mem_28386") != 0)
                return 1;
            if (memblock_set(ctx, &intersectArrays_res_mem_28406,
                             &intersectArrays_res_f_res_mem_28391,
                             "intersectArrays_res_f_res_mem_28391") != 0)
                return 1;
            if (memblock_set(ctx, &intersectArrays_res_mem_28411,
                             &intersectArrays_res_f_res_mem_28396,
                             "intersectArrays_res_f_res_mem_28396") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_f_res_mem_28396,
                               "intersectArrays_res_f_res_mem_28396") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_f_res_mem_28391,
                               "intersectArrays_res_f_res_mem_28391") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_f_res_mem_28386,
                               "intersectArrays_res_f_res_mem_28386") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28105, "mem_28105") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28103, "mem_28103") != 0)
                return 1;
        }
        
        bool dim_match_26408 = sizze_26170 == sizze_26171;
        bool empty_or_match_cert_26409;
        
        if (!dim_match_26408) {
            ctx->error = msgprintf("Error: %s\n\nBacktrace:\n%s",
                                   "Function return value does not match shape of declared return type.",
                                   "-> #0  sparse.fut:81:7-100:69\n   #1  sparse.fut:169:49-171:81\n   #2  sparse.fut:232:17-60\n   #3  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                               "intersectArrays_res_mem_28411") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                               "intersectArrays_res_mem_28406") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                               "intersectArrays_res_mem_28401") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool dim_match_26410 = sizze_26170 == sizze_26172;
        bool empty_or_match_cert_26411;
        
        if (!dim_match_26410) {
            ctx->error = msgprintf("Error: %s\n\nBacktrace:\n%s",
                                   "Function return value does not match shape of declared return type.",
                                   "-> #0  sparse.fut:81:7-100:69\n   #1  sparse.fut:169:49-171:81\n   #2  sparse.fut:232:17-60\n   #3  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                               "intersectArrays_res_mem_28411") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                               "intersectArrays_res_mem_28406") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                               "intersectArrays_res_mem_28401") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        int64_t x_26412 = ((int64_t *) mem_27971)[i_26130];
        int64_t y_26413 = ((int64_t *) mem_27971)[i_26124];
        int64_t indxs_26414 = sub64(x_26412, y_26413);
        bool bounds_invalid_upwards_26415 = slt64(x_26412, y_26413);
        bool valid_26416 = !bounds_invalid_upwards_26415;
        bool range_valid_c_26417;
        
        if (!valid_26416) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Range ", y_26413, "..<", x_26412,
                                   " is invalid.",
                                   "-> #0  sparse.fut:173:18-42\n   #1  sparse.fut:232:17-60\n   #2  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                               "intersectArrays_res_mem_28411") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                               "intersectArrays_res_mem_28406") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                               "intersectArrays_res_mem_28401") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool dim_match_26419 = indxs_26414 == sizze_26170;
        bool empty_or_match_cert_26420;
        
        if (!dim_match_26419) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Value of (core language) shape (",
                                   sizze_26170,
                                   ") cannot match shape of type `[",
                                   indxs_26414, "]index`.",
                                   "-> #0  sparse.fut:177:26-51\n   #1  sparse.fut:232:17-60\n   #2  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                               "intersectArrays_res_mem_28411") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                               "intersectArrays_res_mem_28406") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                               "intersectArrays_res_mem_28401") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        
        bool empty_or_match_cert_26421;
        
        if (!dim_match_26419) {
            ctx->error = msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                                   "Value of (core language) shape (",
                                   sizze_26170,
                                   ") cannot match shape of type `[",
                                   indxs_26414, "]t`.",
                                   "-> #0  sparse.fut:178:25-181:75\n   #1  sparse.fut:232:17-60\n   #2  sparse.fut:223:1-233:48\n");
            if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                               "intersectArrays_res_mem_28411") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                               "intersectArrays_res_mem_28406") != 0)
                return 1;
            if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                               "intersectArrays_res_mem_28401") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                return 1;
            if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                return 1;
            return 1;
        }
        for (int64_t write_iter_27720 = 0; write_iter_27720 < indxs_26414;
             write_iter_27720++) {
            int64_t index_primexp_27942 = add64(y_26413, write_iter_27720);
            bool y_26431 = slt64(write_iter_27720, sizze_26170);
            bool index_certs_26433;
            
            if (!y_26431) {
                ctx->error =
                    msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                              "Index [", write_iter_27720,
                              "] out of bounds for array of shape [",
                              sizze_26170, "].",
                              "-> #0  sparse.fut:180:58-76\n   #1  sparse.fut:178:26-181:63\n   #2  sparse.fut:232:17-60\n   #3  sparse.fut:223:1-233:48\n");
                if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                                   "intersectArrays_res_mem_28411") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                                   "intersectArrays_res_mem_28406") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                                   "intersectArrays_res_mem_28401") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                    return 1;
                return 1;
            }
            
            int64_t y_26434 =
                    ((int64_t *) intersectArrays_res_mem_28411.mem)[write_iter_27720];
            int64_t i_26435 = add64(i_26129, y_26434);
            bool x_26436 = sle64((int64_t) 0, i_26435);
            bool y_26437 = slt64(i_26435, k_25924);
            bool bounds_check_26438 = x_26436 && y_26437;
            bool index_certs_26439;
            
            if (!bounds_check_26438) {
                ctx->error =
                    msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                              "Index [", i_26435,
                              "] out of bounds for array of shape [", k_25924,
                              "].",
                              "-> #0  sparse.fut:180:33-79\n   #1  sparse.fut:178:26-181:63\n   #2  sparse.fut:232:17-60\n   #3  sparse.fut:223:1-233:48\n");
                if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                                   "intersectArrays_res_mem_28411") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                                   "intersectArrays_res_mem_28406") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                                   "intersectArrays_res_mem_28401") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                    return 1;
                return 1;
            }
            
            double defunc_1_mult_arg_26440 =
                   ((double *) rightValues_mem_27967.mem)[i_26435];
            int64_t y_26441 =
                    ((int64_t *) intersectArrays_res_mem_28406.mem)[write_iter_27720];
            int64_t i_26442 = add64(i_26151, y_26441);
            bool x_26443 = sle64((int64_t) 0, i_26442);
            bool y_26444 = slt64(i_26442, n_25922);
            bool bounds_check_26445 = x_26443 && y_26444;
            bool index_certs_26446;
            
            if (!bounds_check_26445) {
                ctx->error =
                    msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s",
                              "Index [", i_26442,
                              "] out of bounds for array of shape [", n_25922,
                              "].",
                              "-> #0  sparse.fut:179:33-77\n   #1  sparse.fut:178:26-181:63\n   #2  sparse.fut:232:17-60\n   #3  sparse.fut:223:1-233:48\n");
                if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                                   "intersectArrays_res_mem_28411") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                                   "intersectArrays_res_mem_28406") != 0)
                    return 1;
                if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                                   "intersectArrays_res_mem_28401") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
                    return 1;
                if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
                    return 1;
                return 1;
            }
            
            double defunc_0_mult_arg_26447 =
                   ((double *) leftValues_mem_27964.mem)[i_26442];
            double defunc_1_mult_res_26448 = defunc_1_mult_arg_26440 *
                   defunc_0_mult_arg_26447;
            bool less_than_zzero_27727 = slt64(index_primexp_27942,
                                               (int64_t) 0);
            bool greater_than_sizze_27728 = sle64(eWiseMultOffsets_res_26115,
                                                  index_primexp_27942);
            bool outside_bounds_dim_27729 = less_than_zzero_27727 ||
                 greater_than_sizze_27728;
            
            if (!outside_bounds_dim_27729) {
                ((double *) mem_28083)[index_primexp_27942] =
                    defunc_1_mult_res_26448;
            }
            if (!outside_bounds_dim_27729) {
                memmove(mem_28085 + index_primexp_27942 * (int64_t) 8,
                        intersectArrays_res_mem_28401.mem + write_iter_27720 *
                        (int64_t) 8, (int64_t) sizeof(int64_t));
            }
        }
        if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                           "intersectArrays_res_mem_28401") != 0)
            return 1;
        if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                           "intersectArrays_res_mem_28406") != 0)
            return 1;
        if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                           "intersectArrays_res_mem_28411") != 0)
            return 1;
        if (memblock_unref(ctx, &intersectArrays_res_mem_28411,
                           "intersectArrays_res_mem_28411") != 0)
            return 1;
        if (memblock_unref(ctx, &intersectArrays_res_mem_28406,
                           "intersectArrays_res_mem_28406") != 0)
            return 1;
        if (memblock_unref(ctx, &intersectArrays_res_mem_28401,
                           "intersectArrays_res_mem_28401") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
        return 1;
    
    struct memblock mem_28443;
    
    mem_28443.references = NULL;
    if (memblock_alloc(ctx, &mem_28443, bytes_28082, "mem_28443")) {
        err = 1;
        goto cleanup;
    }
    memmove(mem_28443.mem + (int64_t) 0, mem_28083 + (int64_t) 0,
            eWiseMultOffsets_res_26115 * (int64_t) sizeof(double));
    
    struct memblock mem_28446;
    
    mem_28446.references = NULL;
    if (memblock_alloc(ctx, &mem_28446, bytes_28082, "mem_28446")) {
        err = 1;
        goto cleanup;
    }
    memmove(mem_28446.mem + (int64_t) 0, mem_28085 + (int64_t) 0,
            eWiseMultOffsets_res_26115 * (int64_t) sizeof(int64_t));
    
    struct memblock mem_28449;
    
    mem_28449.references = NULL;
    if (memblock_alloc(ctx, &mem_28449, bytes_27970, "mem_28449")) {
        err = 1;
        goto cleanup;
    }
    memmove(mem_28449.mem + (int64_t) 0, mem_27971 + (int64_t) 0, m_25923 *
            (int64_t) sizeof(int64_t));
    out_arrsizze_29008 = eWiseMultOffsets_res_26115;
    out_arrsizze_29010 = eWiseMultOffsets_res_26115;
    if (memblock_set(ctx, &out_mem_29007, &mem_28443, "mem_28443") != 0)
        return 1;
    if (memblock_set(ctx, &out_mem_29009, &mem_28446, "mem_28446") != 0)
        return 1;
    if (memblock_set(ctx, &out_mem_29011, &mem_28449, "mem_28449") != 0)
        return 1;
    (*out_mem_p_29203).references = NULL;
    if (memblock_set(ctx, &*out_mem_p_29203, &out_mem_29007, "out_mem_29007") !=
        0)
        return 1;
    *out_out_arrsizze_29204 = out_arrsizze_29008;
    (*out_mem_p_29205).references = NULL;
    if (memblock_set(ctx, &*out_mem_p_29205, &out_mem_29009, "out_mem_29009") !=
        0)
        return 1;
    *out_out_arrsizze_29206 = out_arrsizze_29010;
    (*out_mem_p_29207).references = NULL;
    if (memblock_set(ctx, &*out_mem_p_29207, &out_mem_29011, "out_mem_29011") !=
        0)
        return 1;
    if (memblock_unref(ctx, &mem_28449, "mem_28449") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_28446, "mem_28446") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_28443, "mem_28443") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_28087, "mem_28087") != 0)
        return 1;
    if (memblock_unref(ctx, &out_mem_29011, "out_mem_29011") != 0)
        return 1;
    if (memblock_unref(ctx, &out_mem_29009, "out_mem_29009") != 0)
        return 1;
    if (memblock_unref(ctx, &out_mem_29007, "out_mem_29007") != 0)
        return 1;
    
  cleanup:
    { }
    free(mem_27971);
    free(mem_27989);
    free(mem_27991);
    free(mem_27993);
    free(mem_27995);
    free(mem_27997);
    free(mem_28083);
    free(mem_28085);
    free(mem_28107);
    free(mem_28109);
    free(mem_28111);
    free(mem_28127);
    free(mem_28129);
    free(mem_28131);
    free(mem_28133);
    free(mem_28135);
    free(mem_28271);
    free(mem_28273);
    free(mem_28275);
    free(mem_28277);
    free(mem_28279);
    return err;
}
struct futhark_i64_1d {
    struct memblock mem;
    int64_t shape[1];
} ;
struct futhark_i64_1d *futhark_new_i64_1d(struct futhark_context *ctx, const
                                          int64_t *data, int64_t dim0)
{
    struct futhark_i64_1d *bad = NULL;
    struct futhark_i64_1d *arr =
                          (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    if (memblock_alloc(ctx, &arr->mem, (size_t) dim0 * sizeof(int64_t),
                       "arr->mem"))
        return NULL;
    arr->shape[0] = dim0;
    memmove(arr->mem.mem + 0, data + 0, (size_t) dim0 * sizeof(int64_t));
    lock_unlock(&ctx->lock);
    return arr;
}
struct futhark_i64_1d *futhark_new_raw_i64_1d(struct futhark_context *ctx, const
                                              char *data, int offset,
                                              int64_t dim0)
{
    struct futhark_i64_1d *bad = NULL;
    struct futhark_i64_1d *arr =
                          (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    if (memblock_alloc(ctx, &arr->mem, (size_t) dim0 * sizeof(int64_t),
                       "arr->mem"))
        return NULL;
    arr->shape[0] = dim0;
    memmove(arr->mem.mem + 0, data + offset, (size_t) dim0 * sizeof(int64_t));
    lock_unlock(&ctx->lock);
    return arr;
}
int futhark_free_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr)
{
    lock_lock(&ctx->lock);
    if (memblock_unref(ctx, &arr->mem, "arr->mem") != 0)
        return 1;
    lock_unlock(&ctx->lock);
    free(arr);
    return 0;
}
int futhark_values_i64_1d(struct futhark_context *ctx,
                          struct futhark_i64_1d *arr, int64_t *data)
{
    lock_lock(&ctx->lock);
    memmove(data + 0, arr->mem.mem + 0, (size_t) arr->shape[0] *
            sizeof(int64_t));
    lock_unlock(&ctx->lock);
    return 0;
}
char *futhark_values_raw_i64_1d(struct futhark_context *ctx,
                                struct futhark_i64_1d *arr)
{
    (void) ctx;
    return arr->mem.mem;
}
const int64_t *futhark_shape_i64_1d(struct futhark_context *ctx,
                                    struct futhark_i64_1d *arr)
{
    (void) ctx;
    return arr->shape;
}
struct futhark_f64_1d {
    struct memblock mem;
    int64_t shape[1];
} ;
struct futhark_f64_1d *futhark_new_f64_1d(struct futhark_context *ctx, const
                                          double *data, int64_t dim0)
{
    struct futhark_f64_1d *bad = NULL;
    struct futhark_f64_1d *arr =
                          (struct futhark_f64_1d *) malloc(sizeof(struct futhark_f64_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    if (memblock_alloc(ctx, &arr->mem, (size_t) dim0 * sizeof(double),
                       "arr->mem"))
        return NULL;
    arr->shape[0] = dim0;
    memmove(arr->mem.mem + 0, data + 0, (size_t) dim0 * sizeof(double));
    lock_unlock(&ctx->lock);
    return arr;
}
struct futhark_f64_1d *futhark_new_raw_f64_1d(struct futhark_context *ctx, const
                                              char *data, int offset,
                                              int64_t dim0)
{
    struct futhark_f64_1d *bad = NULL;
    struct futhark_f64_1d *arr =
                          (struct futhark_f64_1d *) malloc(sizeof(struct futhark_f64_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    if (memblock_alloc(ctx, &arr->mem, (size_t) dim0 * sizeof(double),
                       "arr->mem"))
        return NULL;
    arr->shape[0] = dim0;
    memmove(arr->mem.mem + 0, data + offset, (size_t) dim0 * sizeof(double));
    lock_unlock(&ctx->lock);
    return arr;
}
int futhark_free_f64_1d(struct futhark_context *ctx, struct futhark_f64_1d *arr)
{
    lock_lock(&ctx->lock);
    if (memblock_unref(ctx, &arr->mem, "arr->mem") != 0)
        return 1;
    lock_unlock(&ctx->lock);
    free(arr);
    return 0;
}
int futhark_values_f64_1d(struct futhark_context *ctx,
                          struct futhark_f64_1d *arr, double *data)
{
    lock_lock(&ctx->lock);
    memmove(data + 0, arr->mem.mem + 0, (size_t) arr->shape[0] *
            sizeof(double));
    lock_unlock(&ctx->lock);
    return 0;
}
char *futhark_values_raw_f64_1d(struct futhark_context *ctx,
                                struct futhark_f64_1d *arr)
{
    (void) ctx;
    return arr->mem.mem;
}
const int64_t *futhark_shape_f64_1d(struct futhark_context *ctx,
                                    struct futhark_f64_1d *arr)
{
    (void) ctx;
    return arr->shape;
}
int futhark_entry_eWiseAddEntry(struct futhark_context *ctx,
                                struct futhark_f64_1d **out0,
                                struct futhark_i64_1d **out1,
                                struct futhark_i64_1d **out2, const
                                struct futhark_f64_1d *in0, const
                                struct futhark_i64_1d *in1, const
                                struct futhark_i64_1d *in2, const
                                struct futhark_f64_1d *in3, const
                                struct futhark_i64_1d *in4, const
                                struct futhark_i64_1d *in5)
{
    struct memblock leftValues_mem_27964;
    
    leftValues_mem_27964.references = NULL;
    
    struct memblock leftColumns_mem_27965;
    
    leftColumns_mem_27965.references = NULL;
    
    struct memblock leftOffsets_mem_27966;
    
    leftOffsets_mem_27966.references = NULL;
    
    struct memblock rightValues_mem_27967;
    
    rightValues_mem_27967.references = NULL;
    
    struct memblock rightColumns_mem_27968;
    
    rightColumns_mem_27968.references = NULL;
    
    struct memblock rightOffsets_mem_27969;
    
    rightOffsets_mem_27969.references = NULL;
    
    int64_t n_25311;
    int64_t m_25312;
    int64_t k_25313;
    struct memblock out_mem_29007;
    
    out_mem_29007.references = NULL;
    
    int64_t out_arrsizze_29008;
    struct memblock out_mem_29009;
    
    out_mem_29009.references = NULL;
    
    int64_t out_arrsizze_29010;
    struct memblock out_mem_29011;
    
    out_mem_29011.references = NULL;
    
    int ret = 0;
    
    lock_lock(&ctx->lock);
    leftValues_mem_27964 = in0->mem;
    n_25311 = in0->shape[0];
    leftColumns_mem_27965 = in1->mem;
    n_25311 = in1->shape[0];
    leftOffsets_mem_27966 = in2->mem;
    m_25312 = in2->shape[0];
    rightValues_mem_27967 = in3->mem;
    k_25313 = in3->shape[0];
    rightColumns_mem_27968 = in4->mem;
    k_25313 = in4->shape[0];
    rightOffsets_mem_27969 = in5->mem;
    m_25312 = in5->shape[0];
    if (!(n_25311 == in0->shape[0] && (n_25311 == in1->shape[0] && (m_25312 ==
                                                                    in2->shape[0] &&
                                                                    (k_25313 ==
                                                                     in3->shape[0] &&
                                                                     (k_25313 ==
                                                                      in4->shape[0] &&
                                                                      m_25312 ==
                                                                      in5->shape[0])))))) {
        ret = 1;
        if (!ctx->error)
            ctx->error =
                msgprintf("Error: entry point arguments have invalid sizes.\n");
    } else {
        ret = futrts_eWiseAddEntry(ctx, &out_mem_29007, &out_arrsizze_29008,
                                   &out_mem_29009, &out_arrsizze_29010,
                                   &out_mem_29011, leftValues_mem_27964,
                                   leftColumns_mem_27965, leftOffsets_mem_27966,
                                   rightValues_mem_27967,
                                   rightColumns_mem_27968,
                                   rightOffsets_mem_27969, n_25311, m_25312,
                                   k_25313);
        if (ret == 0) {
            assert((*out0 =
                    (struct futhark_f64_1d *) malloc(sizeof(struct futhark_f64_1d))) !=
                NULL);
            (*out0)->mem = out_mem_29007;
            (*out0)->shape[0] = out_arrsizze_29008;
            assert((*out1 =
                    (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) !=
                NULL);
            (*out1)->mem = out_mem_29009;
            (*out1)->shape[0] = out_arrsizze_29010;
            assert((*out2 =
                    (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) !=
                NULL);
            (*out2)->mem = out_mem_29011;
            (*out2)->shape[0] = m_25312;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
int futhark_entry_eWiseAddMultEntry(struct futhark_context *ctx,
                                    struct futhark_f64_1d **out0,
                                    struct futhark_i64_1d **out1,
                                    struct futhark_i64_1d **out2, const
                                    struct futhark_f64_1d *in0, const
                                    struct futhark_i64_1d *in1, const
                                    struct futhark_i64_1d *in2, const
                                    struct futhark_f64_1d *in3, const
                                    struct futhark_i64_1d *in4, const
                                    struct futhark_i64_1d *in5)
{
    struct memblock leftValues_mem_27964;
    
    leftValues_mem_27964.references = NULL;
    
    struct memblock leftColumns_mem_27965;
    
    leftColumns_mem_27965.references = NULL;
    
    struct memblock leftOffsets_mem_27966;
    
    leftOffsets_mem_27966.references = NULL;
    
    struct memblock rightValues_mem_27967;
    
    rightValues_mem_27967.references = NULL;
    
    struct memblock rightColumns_mem_27968;
    
    rightColumns_mem_27968.references = NULL;
    
    struct memblock rightOffsets_mem_27969;
    
    rightOffsets_mem_27969.references = NULL;
    
    int64_t n_26449;
    int64_t m_26450;
    int64_t k_26451;
    struct memblock out_mem_29007;
    
    out_mem_29007.references = NULL;
    
    int64_t out_arrsizze_29008;
    struct memblock out_mem_29009;
    
    out_mem_29009.references = NULL;
    
    int64_t out_arrsizze_29010;
    struct memblock out_mem_29011;
    
    out_mem_29011.references = NULL;
    
    int ret = 0;
    
    lock_lock(&ctx->lock);
    leftValues_mem_27964 = in0->mem;
    n_26449 = in0->shape[0];
    leftColumns_mem_27965 = in1->mem;
    n_26449 = in1->shape[0];
    leftOffsets_mem_27966 = in2->mem;
    m_26450 = in2->shape[0];
    rightValues_mem_27967 = in3->mem;
    k_26451 = in3->shape[0];
    rightColumns_mem_27968 = in4->mem;
    k_26451 = in4->shape[0];
    rightOffsets_mem_27969 = in5->mem;
    m_26450 = in5->shape[0];
    if (!(n_26449 == in0->shape[0] && (n_26449 == in1->shape[0] && (m_26450 ==
                                                                    in2->shape[0] &&
                                                                    (k_26451 ==
                                                                     in3->shape[0] &&
                                                                     (k_26451 ==
                                                                      in4->shape[0] &&
                                                                      m_26450 ==
                                                                      in5->shape[0])))))) {
        ret = 1;
        if (!ctx->error)
            ctx->error =
                msgprintf("Error: entry point arguments have invalid sizes.\n");
    } else {
        ret = futrts_eWiseAddMultEntry(ctx, &out_mem_29007, &out_arrsizze_29008,
                                       &out_mem_29009, &out_arrsizze_29010,
                                       &out_mem_29011, leftValues_mem_27964,
                                       leftColumns_mem_27965,
                                       leftOffsets_mem_27966,
                                       rightValues_mem_27967,
                                       rightColumns_mem_27968,
                                       rightOffsets_mem_27969, n_26449, m_26450,
                                       k_26451);
        if (ret == 0) {
            assert((*out0 =
                    (struct futhark_f64_1d *) malloc(sizeof(struct futhark_f64_1d))) !=
                NULL);
            (*out0)->mem = out_mem_29007;
            (*out0)->shape[0] = out_arrsizze_29008;
            assert((*out1 =
                    (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) !=
                NULL);
            (*out1)->mem = out_mem_29009;
            (*out1)->shape[0] = out_arrsizze_29010;
            assert((*out2 =
                    (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) !=
                NULL);
            (*out2)->mem = out_mem_29011;
            (*out2)->shape[0] = m_26450;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
int futhark_entry_eWiseMultEntry(struct futhark_context *ctx,
                                 struct futhark_f64_1d **out0,
                                 struct futhark_i64_1d **out1,
                                 struct futhark_i64_1d **out2, const
                                 struct futhark_f64_1d *in0, const
                                 struct futhark_i64_1d *in1, const
                                 struct futhark_i64_1d *in2, const
                                 struct futhark_f64_1d *in3, const
                                 struct futhark_i64_1d *in4, const
                                 struct futhark_i64_1d *in5)
{
    struct memblock leftValues_mem_27964;
    
    leftValues_mem_27964.references = NULL;
    
    struct memblock leftColumns_mem_27965;
    
    leftColumns_mem_27965.references = NULL;
    
    struct memblock leftOffsets_mem_27966;
    
    leftOffsets_mem_27966.references = NULL;
    
    struct memblock rightValues_mem_27967;
    
    rightValues_mem_27967.references = NULL;
    
    struct memblock rightColumns_mem_27968;
    
    rightColumns_mem_27968.references = NULL;
    
    struct memblock rightOffsets_mem_27969;
    
    rightOffsets_mem_27969.references = NULL;
    
    int64_t n_25922;
    int64_t m_25923;
    int64_t k_25924;
    struct memblock out_mem_29007;
    
    out_mem_29007.references = NULL;
    
    int64_t out_arrsizze_29008;
    struct memblock out_mem_29009;
    
    out_mem_29009.references = NULL;
    
    int64_t out_arrsizze_29010;
    struct memblock out_mem_29011;
    
    out_mem_29011.references = NULL;
    
    int ret = 0;
    
    lock_lock(&ctx->lock);
    leftValues_mem_27964 = in0->mem;
    n_25922 = in0->shape[0];
    leftColumns_mem_27965 = in1->mem;
    n_25922 = in1->shape[0];
    leftOffsets_mem_27966 = in2->mem;
    m_25923 = in2->shape[0];
    rightValues_mem_27967 = in3->mem;
    k_25924 = in3->shape[0];
    rightColumns_mem_27968 = in4->mem;
    k_25924 = in4->shape[0];
    rightOffsets_mem_27969 = in5->mem;
    m_25923 = in5->shape[0];
    if (!(n_25922 == in0->shape[0] && (n_25922 == in1->shape[0] && (m_25923 ==
                                                                    in2->shape[0] &&
                                                                    (k_25924 ==
                                                                     in3->shape[0] &&
                                                                     (k_25924 ==
                                                                      in4->shape[0] &&
                                                                      m_25923 ==
                                                                      in5->shape[0])))))) {
        ret = 1;
        if (!ctx->error)
            ctx->error =
                msgprintf("Error: entry point arguments have invalid sizes.\n");
    } else {
        ret = futrts_eWiseMultEntry(ctx, &out_mem_29007, &out_arrsizze_29008,
                                    &out_mem_29009, &out_arrsizze_29010,
                                    &out_mem_29011, leftValues_mem_27964,
                                    leftColumns_mem_27965,
                                    leftOffsets_mem_27966,
                                    rightValues_mem_27967,
                                    rightColumns_mem_27968,
                                    rightOffsets_mem_27969, n_25922, m_25923,
                                    k_25924);
        if (ret == 0) {
            assert((*out0 =
                    (struct futhark_f64_1d *) malloc(sizeof(struct futhark_f64_1d))) !=
                NULL);
            (*out0)->mem = out_mem_29007;
            (*out0)->shape[0] = out_arrsizze_29008;
            assert((*out1 =
                    (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) !=
                NULL);
            (*out1)->mem = out_mem_29009;
            (*out1)->shape[0] = out_arrsizze_29010;
            assert((*out2 =
                    (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) !=
                NULL);
            (*out2)->mem = out_mem_29011;
            (*out2)->shape[0] = m_25923;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
