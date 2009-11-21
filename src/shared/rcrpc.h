#ifndef RAMCLOUD_SHARED_RCRPC_H
#define RAMCLOUD_SHARED_RCRPC_H

// #include <cinttypes> // this requires c++0x support because it's c99
// so we'll go ahead and use the C header
#include <inttypes.h>

#define RCRPC_HEADER_LEN                ((size_t) &(((struct rcrpc *) 0)->ping_request))
#define RCRPC_PING_REQUEST_LEN          (RCRPC_HEADER_LEN + sizeof(struct rcrpc_ping_request))
#define RCRPC_PING_RESPONSE_LEN         (RCRPC_HEADER_LEN + sizeof(struct rcrpc_ping_response))
#define RCRPC_READ_REQUEST_LEN          (RCRPC_HEADER_LEN + sizeof(struct rcrpc_read_request))
#define RCRPC_READ_RESPONSE_LEN_WODATA  (RCRPC_HEADER_LEN + sizeof(struct rcrpc_read_response))
#define RCRPC_WRITE_REQUEST_LEN_WODATA  (RCRPC_HEADER_LEN + sizeof(struct rcrpc_write_request))
#define RCRPC_WRITE_RESPONSE_LEN        (RCRPC_HEADER_LEN + sizeof(struct rcrpc_write_response))
#define RCRPC_INSERT_REQUEST_LEN        (RCRPC_HEADER_LEN + sizeof(struct rcrpc_insert_request))
#define RCRPC_INSERT_RESPONSE_LEN       (RCRPC_HEADER_LEN + sizeof(struct rcrpc_insert_response))
#define RCRPC_DELETE_REQUEST_LEN        (RCRPC_HEADER_LEN + sizeof(struct rcrpc_delete_request))
#define RCRPC_DELETE_RESPONSE_LEN       (RCRPC_HEADER_LEN + sizeof(struct rcrpc_delete_response))
#define RCRPC_CREATE_TABLE_REQUEST_LEN  (RCRPC_HEADER_LEN + sizeof(struct rcrpc_create_table_request))
#define RCRPC_CREATE_TABLE_RESPONSE_LEN (RCRPC_HEADER_LEN + sizeof(struct rcrpc_create_table_response))
#define RCRPC_OPEN_TABLE_REQUEST_LEN    (RCRPC_HEADER_LEN + sizeof(struct rcrpc_open_table_request))
#define RCRPC_OPEN_TABLE_RESPONSE_LEN   (RCRPC_HEADER_LEN + sizeof(struct rcrpc_open_table_response))
#define RCRPC_DROP_TABLE_REQUEST_LEN    (RCRPC_HEADER_LEN + sizeof(struct rcrpc_drop_table_request))
#define RCRPC_DROP_TABLE_RESPONSE_LEN   (RCRPC_HEADER_LEN + sizeof(struct rcrpc_drop_table_response))

//namespace RAMCloud {

enum RCRPC_TYPE {
    RCRPC_PING_REQUEST,
    RCRPC_PING_RESPONSE,
    RCRPC_READ_REQUEST,
    RCRPC_READ_RESPONSE,
    RCRPC_WRITE_REQUEST,
    RCRPC_WRITE_RESPONSE,
    RCRPC_INSERT_REQUEST,
    RCRPC_INSERT_RESPONSE,
    RCRPC_DELETE_REQUEST,
    RCRPC_DELETE_RESPONSE,
    RCRPC_CREATE_TABLE_REQUEST,
    RCRPC_CREATE_TABLE_RESPONSE,
    RCRPC_OPEN_TABLE_REQUEST,
    RCRPC_OPEN_TABLE_RESPONSE,
    RCRPC_DROP_TABLE_REQUEST,
    RCRPC_DROP_TABLE_RESPONSE,
};


struct rcrpc_ping_request {
};

struct rcrpc_ping_response {
};


struct rcrpc_read_request {
    uint64_t table;
    uint64_t key;
};

struct rcrpc_read_response {
    uint64_t buf_len;
    char buf[0];                        /* Variable length */
};

struct rcrpc_write_request {
    uint64_t table;
    uint64_t key;
    uint64_t buf_len;
    char buf[0];                        /* Variable length */
};

struct rcrpc_write_response {
};

struct rcrpc_insert_request {
    uint64_t table;
    char buf[100];
};

struct rcrpc_insert_response {
    uint64_t key;
};

struct rcrpc_delete_request {
    uint64_t table;
    uint64_t key;
};

struct rcrpc_delete_response {
};


struct rcrpc_create_table_request {
    char name[64];
};

struct rcrpc_create_table_response {
};


struct rcrpc_open_table_request {
    char name[64];
};

struct rcrpc_open_table_response {
    uint64_t handle;
};


struct rcrpc_drop_table_request {
    char name[64];
};

struct rcrpc_drop_table_response {
};


struct rcrpc {
    uint32_t type;
    uint32_t len;
    union {
        struct rcrpc_ping_request ping_request;
        struct rcrpc_ping_response ping_response;

        struct rcrpc_read_request read_request;
        struct rcrpc_read_response read_response;

        struct rcrpc_write_request write_request;
        struct rcrpc_write_response write_response;

        struct rcrpc_insert_request insert_request;
        struct rcrpc_insert_response insert_response;

        struct rcrpc_delete_request delete_request;
        struct rcrpc_delete_response delete_response;

        struct rcrpc_create_table_request create_table_request;
        struct rcrpc_create_table_response create_table_response;

        struct rcrpc_open_table_request open_table_request;
        struct rcrpc_open_table_response open_table_response;

        struct rcrpc_drop_table_request drop_table_request;
        struct rcrpc_drop_table_response drop_table_response;
    };
};

//} // namespace RAMCloud

#endif

