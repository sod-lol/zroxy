

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <memory.h>
#include <sys/epoll.h>

#include "io_helper.h"
#include "logs.h"


int read_socket_non_block(read_io_req_t *io_req)
{
    int nbytes = recv(io_req->req_fd, 
                        io_req->buffer, 
                        io_req->maximum_read_buffer_size, io_req->flags);

    if (nbytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
        return WOULD_BLOCK;
    }
    else if (nbytes == -1) {
        LOG_ERROR("Cannot read from fd(%d) becuase: %s\n", io_req->req_fd, strerror(errno));
        return UNKOWN_ERROR;
    }

    return nbytes;
}

int write_socket_non_block(write_io_req_t *io_req)
{
    int nbytes = send(io_req->req_fd, 
                        io_req->buffer, 
                        io_req->send_nbytes, io_req->flags);

    if (nbytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
        return WOULD_BLOCK;
    }
    else if (nbytes == -1) {
        LOG_ERROR("Cannot write to fd (%d) becuase: %s\n", 
                    io_req->req_fd, strerror(errno));
        return UNKOWN_ERROR;
    }

    return nbytes;
}

int write_socket_non_block_and_clear_buf(write_io_req_t *io_req)
{
    int nbytes = send(io_req->req_fd, 
                        io_req->buffer, 
                        io_req->send_nbytes, io_req->flags);

    if (nbytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
        return WOULD_BLOCK;
    }
    else if (nbytes == -1) {
        LOG_ERROR("Cannot write to fd (%d) becuase: %s\n", 
                    io_req->req_fd, strerror(errno));
        return UNKOWN_ERROR;
    }

    memset(io_req->buffer, '\0', io_req->clear_nbytes);

    return nbytes;
}



int8_t is_readable_event(u_int32_t event)
{
    return (event & EPOLLIN);
}

int8_t is_writable_event(u_int32_t event)
{
    return (event & EPOLLOUT);
}