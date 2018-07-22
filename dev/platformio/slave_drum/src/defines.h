/**
 * @Author: Lutz Reiter [http://www.lu-re.de] <lutz>
 * @Date:   2018-07-22T19:28:48+02:00
 * @Project: Brain String
 * @Last modified by:   lutz
 * @Last modified time: 2018-07-22T22:23:32+02:00
 */

#define ID 0
#define DEFAULT_DELAY_TIME 300

#define EEPROM_DELAY 16

#ifdef  __IS_DRUM
	#define TYPE "DRUM"
	#define MIN_TRIGGER_TIME 0
	#define MAX_TRIGGER_TIME 50
#else
	#define TYPE "OTHER"
	#define MIN_TRIGGER_TIME 0
	#define MAX_TRIGGER_TIME 50
#endif
