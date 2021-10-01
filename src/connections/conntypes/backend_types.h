


#ifndef BACKEND_TYPES_H
#define BACKEND_TYPES_H

#include <sys/types.h>

#include "utils/io/buffer_mamager.h"

typedef void (*zxy_backend_callback_func)(void*);

typedef void (*zxy_backend_event_callback)(int32_t, u_int32_t, void*);

typedef int  (*zxy_backend_memebr_func)(u_int32_t, void*);

/**
 * backend base connection which is all other type of backend connection should implement this
 */
typedef struct 
{
    /**
     * backend callback functions
     */
    zxy_backend_callback_func on_read;
    zxy_backend_callback_func on_write;
    zxy_backend_callback_func on_close;

    /**
     * backend extra function for controll object
     */
    zxy_backend_callback_func force_close;
    zxy_backend_memebr_func is_ready_event;

    /**
     * general callback function which is responsible for handling events
     */ 
    zxy_backend_event_callback callback;


    /**
     * params which is passed to each of above functions and is custom impelementation of your connection
     * 
     */
    void *params;
} zxy_backend_base_t;


/**
 * backend plain connection
 */
typedef struct 
{
    /**
     * sock fd properties
     */
    int sock_fd;
    int8_t is_closed;
    u_int32_t events;

    /**
     * buffer manager which is controll buffer resize and nasty bit hacks
     */
    zxy_buffer_manager_t *buffer_manager;

    /**
     * should free?
     */
    int8_t set_free;

} zxy_backend_conn_t;


#endif /* BACKEND_TYPES_H */