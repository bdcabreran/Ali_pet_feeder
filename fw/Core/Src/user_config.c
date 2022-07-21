
#include "user_config.h"
#include "printf_dbg.h"

/**@brief Enable/Disable debug messages */
#define USER_CNF_FLASH_DBG 1
#define USER_CNF_FLASH_TAG "user config flash : "

/**@brief uart debug function for server comm operations  */
#if USER_CNF_FLASH_DBG
#define user_cfg_dbg(format, ...)         \
    do                                           \
    {											\
    	printf_dbg_color(E_MAG, USER_CNF_FLASH_TAG); \
        printf_dbg(format,##__VA_ARGS__ );       \
    } while (0)
#else
#define user_cfg_dbg(format, ...) \
	do                                       \
	{ /* Do nothing */                       \
	} while (0)
#endif


/*Function should be call when new configuration is set by the user */
void user_config_set(void)
{
    user_cfg_dbg("saving user configuration to flash...\r\n");

    user_config_t user_config;
    user_config.data_available = true;

    /*Copy the latest information from all FSMs to flash*/
    memcpy((uint8_t *)&user_config.thermostat_info, (uint8_t *)temp_ctrl_fsm_get_info(), sizeof(thermostat_config_info_t));
    memcpy((uint8_t *)&user_config.petcall_info, (uint8_t *)petcall_fsm_get_info(), sizeof(petcall_config_info_t));
    memcpy((uint8_t *)&user_config.feeder_info, (uint8_t *)feeder_fsm_get_info(), sizeof(feeder_config_info_t));

    uint32_t data_len = sizeof(user_config_t)/sizeof(uint32_t) + 1;

    flash_memory_write((uint8_t*)&user_config, data_len);
}

/*Function called by all FSM is a shutdown occurs to load new data*/
user_config_t *user_config_get(void)
{
    user_cfg_dbg("getting user configuration from flash...\r\n");

    static user_config_t user_config;
    uint32_t data_len = sizeof(user_config_t)/sizeof(uint32_t) + 1;

    flash_memory_read((uint8_t *)&user_config, data_len);
    return &user_config;
}
