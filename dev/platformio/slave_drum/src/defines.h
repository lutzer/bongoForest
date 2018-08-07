/**
 * @Author: Lutz Reiter [http://www.lu-re.de] <lutz>
 * @Date:   2018-07-22T19:28:48+02:00
 * @Last modified by:   lutz
 * @Last modified time: 2018-08-07T23:07:00+02:00
 */

#define BOARD_ID _ID

// default parameter
#define DEFAULT_DELAY_TIME 300
#define DEFAULT_DELAY_CHANGE 100
#define DEFAULT_CHANGE_INTERVAL 2000
#define DEFAULT_THRESHOLD 0

// EEPROM adresses
#define EEPROM_DELAY_TIME 16
#define EEPROM_THRESHOLD 20
#define EEPROM_DELAY_CHANGE 24
#define EEPROM_CHANGE_INTERVAL 28

// Build flag specific vars
#define TWO_PI 6.283185307179586476925286766559

#ifdef  _IS_DRUM
	#define BOARD_TYPE "DRUM"
	#define MIN_TRIGGER_TIME 0
	#define MAX_TRIGGER_TIME 50
#else
	#define BOARD_TYPE "OTHER"
	#define MIN_TRIGGER_TIME 0
	#define MAX_TRIGGER_TIME 50
#endif
