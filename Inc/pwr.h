/*
 * pwr.h
 *
 *  Created on: Feb 17, 2022
 *      Author: Medusa
 */
 
#ifndef PWR_H_
#define PWR_H_
void reset_clock();
void reduce_clock_speed();
void prepare_low_power(void);

/**
 * @brief Enter in deep sleep mode.
 */
void pwr_enterSleepMode(void);

void exit_low_power();
#endif /* PWR_H_ */
