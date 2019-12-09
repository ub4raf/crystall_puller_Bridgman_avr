# crystall_puller_Bridgman_avr
Step motor controller for puller of crucible for Bridgman method crystal growth machine.
AVR (arduino hardware). CV_AVR compiler.

Arduino bootloader was cleared because of bugs with watch dog timer.
Internal RC oscillator was used (by fuses) because of unstable work of crystall oscillator.
EEPROM was NOT used because of bugs. Chinese chip problem?

1601 display, rotary encoder, one switch for encoder blocking, one limiter and step&dir output to driver.
Push and rotate - moove up\down to N mm.
Rotate - set speed in mm/h
Push - start down\up\growth programm; start only growth programm; stop programm.
