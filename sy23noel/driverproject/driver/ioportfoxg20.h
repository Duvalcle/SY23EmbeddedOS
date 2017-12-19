
#ifndef __IOPORTFOXG20_H
#define __IOPORTFOXG20_H

/* routines gpio

export the GPIO to userspace :
  int gpio_export(unsigned gpio, bool direction_may_change);
reverse gpio_export :
  void gpio_unexport();
create a sysfs link to an exported GPIO node :
  int gpio_export_link(struct device *dev, const char *name, unsigned gpio)
change the polarity of a GPIO node in sysfs :
  int gpio_sysfs_set_active_low(unsigned gpio, int value);
 
set as input or output, returning 0 or negative errno 
  int gpio_is_valid(int number);
  int gpio_direction_input(unsigned gpio);
  int gpio_direction_output(unsigned gpio, int value);

GPIO INPUT:  return zero or nonzero :
  int gpio_get_value(unsigned gpio);
GPIO OUTPUT 
  void gpio_set_value(unsigned gpio, int value);

*/

//#include <asm/arch/gpio.h>



#ifndef OLD_RELEASE
/* new release
   kernel ID port A 
 43 54 42 41 39 38 94 95
*/
#define FOXG20_KID_PA0	43
#define FOXG20_KID_PA1	54
#define FOXG20_KID_PA2	42
#define FOXG20_KID_PA3	41
#define FOXG20_KID_PA4	39
#define FOXG20_KID_PA5	38
#define FOXG20_KID_PA6	94
#define FOXG20_KID_PA7	95

/* new release
   kernel ID port B 
 67 65 64 66 59 60 57 58
*/
#define FOXG20_KID_PB0	67
#define FOXG20_KID_PB1	65
#define FOXG20_KID_PB2	64
#define FOXG20_KID_PB3	66
#define FOXG20_KID_PB4	59
#define FOXG20_KID_PB5	60
#define FOXG20_KID_PB6	57
#define FOXG20_KID_PB7	58

#else
/* old release 
  PA0   PA1   PA2   PA3    PA4    PA5   PA6   PA7    
  J6.18 J6.24 J6.37 J6.38  J6.17  J6.20 J6.25 J6.36  
  84    38    54    43     85     94    39    42    
*/
#define FOXG20_KID_PA0	84
#define FOXG20_KID_PA1	38
#define FOXG20_KID_PA2	54
#define FOXG20_KID_PA3	43
#define FOXG20_KID_PA4	85
#define FOXG20_KID_PA5	94
#define FOXG20_KID_PA6	39
#define FOXG20_KID_PA7	42

/* old release 
  PB0    PB1   PB2   PB3    PB4  PB5  PB6   PB7
  J7.36  J7.35 J7.38 J7.37 J7.3  J7.5 J7.4  J7.6
  59     60    57    58    82    80    83   81
*/
#define FOXG20_KID_PB0	59
#define FOXG20_KID_PB1	60
#define FOXG20_KID_PB2	57
#define FOXG20_KID_PB3	58
#define FOXG20_KID_PB4	82
#define FOXG20_KID_PB5	80
#define FOXG20_KID_PB6	83
#define FOXG20_KID_PB7	81

#endif


#endif




