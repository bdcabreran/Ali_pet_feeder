#include "event_queue.h"
#include "printf_dbg.h"

/**@brief Enable/Disable debug messages */
#define EVT_QUEUE_DEBUG 0
#define EVT_QUEUE_TAG "evt queue : "

/**@brief uart debug function for server comm operations  */
#if EVT_QUEUE_DEBUG
#define evt_queue_dbg(format, ...)         \
    do                                           \
    {											\
    	printf_dbg_color(E_MAG, EVT_QUEUE_TAG); \
        printf_dbg(format,##__VA_ARGS__ );       \
    } while (0)
#else

static void event_print_info(event_t *evt)
{
    evt_queue_dbg("event info : \r\n");
    evt_queue_dbg("FSM SRC : [0x%X]\r\n", evt->fsm_src);
    evt_queue_dbg("FSM DST : [0x%X]\r\n", evt->fsm_dst);
    evt_queue_dbg("FSM EVT : [0x%X]\r\n", evt->evt);
    evt_queue_dbg("FSM Payload Len : [%d]\r\n", evt->header.payload_len);

    if(evt->header.payload_len)
        print_buffer('X', (uint8_t*)&evt->payload.data[0], evt->header.payload_len);
}

void event_queue_init(event_queue_t *queue, uint8_t *buff, uint8_t buff_len)
{
    evt_queue_dbg("initializing queue\r\n");
    queue->rb = ring_buffer_init(buff, buff_len);
    queue->pending_cnt = 0;
}

uint8_t event_queue_get_pending(event_queue_t *queue)
{
    return queue->pending_cnt; 
}

uint8_t event_queue_write(event_queue_t *queue, event_t *evt)
{
    evt_queue_dbg("writing event \r\n");
    event_print_info(evt);

    /*write header */
    ring_buffer_write(, EVENT_HEADER_SIZE);

    /*write payload */


    return write_ok;

}

void event_queue_read(event_queue_t *queue)
{

}

void event_queue_fetch(event_queue_t *queue)
{

}

void event_queue_reset(event_queue_t *queue)
{

}

