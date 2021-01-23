#pragma once

// Headers

#define _GNU_SOURCE
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <float.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <nvrtc.h>


// Initialisation

int futhark_get_num_sizes(void);
const char *futhark_get_size_name(int);
const char *futhark_get_size_class(int);
struct futhark_context_config ;
struct futhark_context_config *futhark_context_config_new(void);
void futhark_context_config_free(struct futhark_context_config *cfg);
void futhark_context_config_add_nvrtc_option(struct futhark_context_config *cfg,
                                             const char *opt);
void futhark_context_config_set_debugging(struct futhark_context_config *cfg,
                                          int flag);
void futhark_context_config_set_profiling(struct futhark_context_config *cfg,
                                          int flag);
void futhark_context_config_set_logging(struct futhark_context_config *cfg,
                                        int flag);
void futhark_context_config_set_device(struct futhark_context_config *cfg, const
                                       char *s);
void futhark_context_config_dump_program_to(struct futhark_context_config *cfg,
                                            const char *path);
void
futhark_context_config_load_program_from(struct futhark_context_config *cfg,
                                         const char *path);
void futhark_context_config_dump_ptx_to(struct futhark_context_config *cfg,
                                        const char *path);
void futhark_context_config_load_ptx_from(struct futhark_context_config *cfg,
                                          const char *path);
void
futhark_context_config_set_default_group_size(struct futhark_context_config *cfg,
                                              int size);
void
futhark_context_config_set_default_num_groups(struct futhark_context_config *cfg,
                                              int num);
void
futhark_context_config_set_default_tile_size(struct futhark_context_config *cfg,
                                             int num);
void
futhark_context_config_set_default_threshold(struct futhark_context_config *cfg,
                                             int num);
int futhark_context_config_set_size(struct futhark_context_config *cfg, const
                                    char *size_name, size_t size_value);
struct futhark_context ;
struct futhark_context *futhark_context_new(struct futhark_context_config *cfg);
void futhark_context_free(struct futhark_context *ctx);

// Arrays

struct futhark_f64_1d ;
struct futhark_f64_1d *futhark_new_f64_1d(struct futhark_context *ctx, const
                                          double *data, int64_t dim0);
struct futhark_f64_1d *futhark_new_raw_f64_1d(struct futhark_context *ctx, const
                                              CUdeviceptr data, int offset,
                                              int64_t dim0);
int futhark_free_f64_1d(struct futhark_context *ctx,
                        struct futhark_f64_1d *arr);
int futhark_values_f64_1d(struct futhark_context *ctx,
                          struct futhark_f64_1d *arr, double *data);
CUdeviceptr futhark_values_raw_f64_1d(struct futhark_context *ctx,
                                      struct futhark_f64_1d *arr);
const int64_t *futhark_shape_f64_1d(struct futhark_context *ctx,
                                    struct futhark_f64_1d *arr);
struct futhark_i64_1d ;
struct futhark_i64_1d *futhark_new_i64_1d(struct futhark_context *ctx, const
                                          int64_t *data, int64_t dim0);
struct futhark_i64_1d *futhark_new_raw_i64_1d(struct futhark_context *ctx, const
                                              CUdeviceptr data, int offset,
                                              int64_t dim0);
int futhark_free_i64_1d(struct futhark_context *ctx,
                        struct futhark_i64_1d *arr);
int futhark_values_i64_1d(struct futhark_context *ctx,
                          struct futhark_i64_1d *arr, int64_t *data);
CUdeviceptr futhark_values_raw_i64_1d(struct futhark_context *ctx,
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
#define FUTHARK_BACKEND_cuda
