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
#define evt_queue_dbg(format, ...) \
	do                                       \
	{ /* Do nothing */                       \
	} while (0)
#endif


static void event_print_info(event_t *evt)
{
    evt_queue_dbg("event info : \r\n");
    evt_queue_dbg("FSM SRC : [0x%X]\r\n", evt->fsm_src);
    evt_queue_dbg("FSM DST : [0x%X]\r\n", evt->fsm_dst);
    evt_queue_dbg("FSM EVT : [0x%X]\r\n", evt->evt);
    evt_queue_dbg("FSM Payload Len : [%d]\r\n", evt->header.payload_len);

    if(evt->header.payload_len)
        print_buffer('X', (uint8_t*)evt->payload.buff, evt->header.payload_len);
}

void event_queue_init(event_queue_t *queue, uint8_t *buff, uint8_t buff_len)
{
    evt_queue_dbg("initializing queue\r\n");
    queue->rb = ring_buffer_init(buff, buff_len);
    queue->pending_cnt = 0;
}

uint8_t event_queue_get_pending(event_queue_t *queue)
{
    evt_queue_dbg("pending evts [%d]\r\n", queue->pending_cnt);
    return queue->pending_cnt; 
}

uint8_t event_queue_write(event_queue_t *queue, event_t *evt)
{
    ring_buffer_st_t write_st = RING_BUFF_OK;
    evt_queue_dbg("writing event \r\n");
    event_print_info(evt);

    write_st = ring_buffer_write(queue->rb, (uint8_t *)&evt->header, EVENT_HEADER_SIZE);
    if (evt->header.payload_len)
        write_st = ring_buffer_write(queue->rb, (uint8_t *)evt->payload.buff, evt->header.payload_len);

    if (write_st == RING_BUFF_OK)
        queue->pending_cnt++;
    else
        printf_dbg_error("could not write on evt queue\r\n");

    return write_st;
}

uint8_t event_queue_read(event_queue_t *queue, event_t *evt)
{
    ring_buffer_st_t read_st = RING_BUFF_OK;

    if (event_queue_get_pending(queue))
    {
        evt_queue_dbg("reading event \r\n");
        read_st = ring_buffer_read(queue->rb, (uint8_t *)&evt->header, EVENT_HEADER_SIZE);
        if (evt->header.payload_len)
            read_st = ring_buffer_read(queue->rb, (uint8_t *)evt->payload.buff, evt->header.payload_len);

        if (read_st == RING_BUFF_OK)
        {
            queue->pending_cnt--;
            event_print_info(evt);
        }
        else
            printf_dbg_error("could not read evt in queue\r\n");
    }
    else
        evt_queue_dbg("error, no evt in queue \r\n");

    return read_st;
}

void event_queue_fetch(event_queue_t *queue, event_t *evt)
{
    ring_buffer_st_t read_st = RING_BUFF_OK;

    if (event_queue_get_pending(queue))
    {
        evt_queue_dbg("fetching event \r\n");
        read_st = ring_buffer_fetch(queue->rb, (uint8_t *)&evt->header, EVENT_HEADER_SIZE);
        if (evt->header.payload_len)
            read_st = ring_buffer_fetch(queue->rb, (uint8_t *)evt->payload.buff, evt->header.payload_len);

        if (read_st == RING_BUFF_OK)
            event_print_info(evt);
        else
            printf_dbg_error("could not fetch evt in queue\r\n");
    }
    else
        evt_queue_dbg("error, no evt in queue \r\n");

    return read_st;
}

void event_queue_reset(event_queue_t *queue)
{
    evt_queue_dbg("queue reset \r\n");
    ring_buffer_reset(queue->rb);
    queue->pending_cnt = 0;
}

