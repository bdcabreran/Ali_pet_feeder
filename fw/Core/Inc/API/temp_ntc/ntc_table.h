#include "main.h"
#include "pgmspace.h"

// Temperature value returned if the sum of the ADC results is greater than the first table value
#define TEMPERATURE_UNDER -550
// Temperature value returned if the sum of the ADC results is less than the last table value
#define TEMPERATURE_OVER 1250
// Temperature value corresponding to the first value of the table
#define TEMPERATURE_TABLE_START -550
// Table step 
#define TEMPERATURE_TABLE_STEP 5

// The type of each element in the table, if the sum is within 16 bits - uint16_t, otherwise - uint32_t
typedef uint16_t temperature_table_entry_type;
// Table index type. If the table has more than 256 elements, then uint16_t, otherwise - uint8_t
typedef uint16_t temperature_table_index_type;
// Table element access method, must match temperature_table_entry_type

#define TEMPERATURE_TABLE_READ(i) pgm_read_word(&termo_table[i])

extern const temperature_table_entry_type termo_table[] PROGMEM;
 
int16_t calc_temperature(temperature_table_entry_type adcsum);
