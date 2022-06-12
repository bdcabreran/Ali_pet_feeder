
#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "ring_buffer.h"

#define EVENT_MAX_PAYLOAD_SIZE (4*20) //80 bytes per packet
#define EVENT_HEADER_SIZE      (sizeof(event_header_t))

typedef struct 
{
    uint8_t name;
    uint8_t fsm_src; 
    uint8_t fsm_dst;
    uint8_t payload_len;
}event_header_t;

typedef struct
{
    uint8_t buff[EVENT_MAX_PAYLOAD_SIZE];
}event_payload_t;


typedef struct
{
    event_header_t  header;
    event_payload_t payload;
}event_t;

typedef struct 
{
    ring_buffer_handle_t rb;
    uint8_t pending_cnt;
}event_queue_t;

void event_queue_init(event_queue_t *, uint8_t *buff, uint8_t buff_len);
uint8_t event_queue_get_pending(event_queue_t *queue);
uint8_t event_queue_write(event_queue_t *queue, event_t *evt);
uint8_t event_queue_read(event_queue_t *queue, event_t *evt);
uint8_t event_queue_fetch(event_queue_t *queue, event_t *evt);
void event_queue_reset(event_queue_t *);


#endif 



