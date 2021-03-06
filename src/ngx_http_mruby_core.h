/**
 *  Copyright (c) 2013 Tatsuhiko Kubo <cubicdaiya@gmail.com>
 *  Copyright (c) ngx_mruby developers 2012-
 *
 *  Use and distribution licensed under the MIT license.
 *  See LICENSE for full text.
 *
 */

#ifndef NGX_HTTP_MRUBY_CORE_H
#define NGX_HTTP_MRUBY_CORE_H

#include <ngx_http.h>
#include <mruby.h>

typedef enum code_type_t {
    NGX_HTTP_MRUBY_CODE_TYPE_FILE,
    NGX_HTTP_MRUBY_CODE_TYPE_STRING
} code_type_t;

typedef struct ngx_http_mruby_state_t {
    mrb_state *mrb;
    int ai;
} ngx_http_mruby_state_t;

typedef struct ngx_http_mruby_code_t {
    union code {
        char *file;
        char *string;
    } code;
    code_type_t code_type;
    struct RProc* proc;
} ngx_http_mruby_code_t;

typedef struct ngx_http_mruby_rputs_chain_list_t {
    ngx_chain_t **last;
    ngx_chain_t *out;
} ngx_http_mruby_rputs_chain_list_t;

typedef struct ngx_http_mruby_filter_ctx_t {
    u_char *body;
    u_char *last;
    size_t body_length;
} ngx_http_mruby_filter_ctx_t;

typedef enum ngx_http_mruby_phase_t {
    NGX_HTTP_MRUBY_PHASE_SET,
    NGX_HTTP_MRUBY_PHASE_POST_READ,
    NGX_HTTP_MRUBY_PHASE_SERVER_REWRITE,
    NGX_HTTP_MRUBY_PHASE_REWRITE,
    NGX_HTTP_MRUBY_PHASE_ACCESS,
    NGX_HTTP_MRUBY_PHASE_CONTENT,
    NGX_HTTP_MRUBY_PHASE_LOG,
    NGX_HTTP_MRUBY_PHASE_HEADER_FILTER,
    NGX_HTTP_MRUBY_PHASE_BODY_FILTER,
    NGX_HTTP_MRUBY_PHASE_MAX
} ngx_http_mruby_phase_t;

typedef struct ngx_http_mruby_ctx_t {
    ngx_http_mruby_rputs_chain_list_t *rputs_chain;
    ngx_http_mruby_filter_ctx_t filter_ctx;
    ngx_http_mruby_phase_t phase;
    mrb_value table;
    ngx_int_t exited;
    ngx_int_t exit_code;
} ngx_http_mruby_ctx_t;

ngx_int_t ngx_http_mruby_run_conf(ngx_conf_t *cf, ngx_http_mruby_state_t *state, ngx_http_mruby_code_t *code);
ngx_int_t ngx_http_mruby_run_args(ngx_http_request_t *r, ngx_http_mruby_state_t *state, ngx_http_mruby_code_t *code, 
                           ngx_flag_t cached, ngx_http_variable_value_t *args, size_t nargs, ngx_str_t *result);
ngx_int_t ngx_http_mruby_run(ngx_http_request_t *r, ngx_http_mruby_state_t *state, ngx_http_mruby_code_t *code, ngx_flag_t cached);
ngx_int_t ngx_http_mruby_run_header_filter(ngx_http_request_t *r, ngx_http_mruby_state_t *state, ngx_http_mruby_code_t *code, ngx_flag_t cached);
ngx_int_t ngx_http_mruby_run_body_filter(ngx_http_request_t *r, ngx_http_mruby_state_t *state, ngx_http_mruby_code_t *code, ngx_flag_t cached);
void ngx_http_mruby_core_init(mrb_state *mrb, struct RClass *class);

#endif // NGX_HTTP_MRUBY_CORE_H
