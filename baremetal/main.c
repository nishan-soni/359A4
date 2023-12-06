#include <unistd.h>
#include <stdio.h>
#include "uart.h"
#include "timer.h"
#include "i2c.h"
#include "bmi270.h"
#include <stdlib.h>
#include "accel.h"

#define ACCEL_THRESHOLD 400 // The threshold that our acceleration needs to pass to be valid


/**
 * Given the accelerometer values, output the direction (as a char) that our I2C device is accelerating in
*/
char generateDirection(struct accelerations *accel) {

    // Check if movement is mostly in the X direction
    if (abs(accel -> Ax) > abs(accel -> Ay) & (abs(accel -> Ax) > ACCEL_THRESHOLD) ) {
        if (accel -> Ax < 0) {
            return 'u'; // Up
        }
        return 'd'; // Down
    }

    // Check if movement is mostly in the Y direction
    if ((abs(accel -> Ay) > abs(accel -> Ax)) & (abs(accel -> Ay) > ACCEL_THRESHOLD) ) {
        if (accel -> Ay < 0) {
            return 'r'; // Right
        }
        return 'l'; // Left
    }

    return 'n'; // Return nothing if there is no movement

}


int main() {
    delay(1000);
    
    // Initializing UART and I2C
    uart_init();
    initI2CPins();
    bmi270LoadConfigFile();
    bmi270SetMode( PERFORMANCE_MODE );
    bmi270SetAccRange( ACC_RANGE_2G );
    bmi270SetGyrRange( GYR_RANGE_1000 );
    bmi270SetAccOdr( ACC_ODR_200 );
    bmi270SetGyrOdr( GYR_ODR_200 );
    bmi270SetAccBwp( ACC_BWP_OSR4 );  
    bmi270SetGyrBwp( GYR_BWP_OSR4 );
    bmi270DisableFifoHeader();
    bmi270EnableDataStreaming();
    bmi270EnableAccFilterPerf();
    bmi270EnableGyrNoisePerf( );
    bmi270EnableGyrFilterPerf( );
    
    
    int aVals[6];
    struct accelerations accel;

    while( 1 ) {
        // Continually get data for bmi270
        bmi270GetAllData( aVals );
        accel.Ax = aVals[0];
        accel.Ay = aVals[1];
        accel.Az = aVals[2];

        uart_send(generateDirection(&accel)); // Generate the direction that the game should move in and send it via UART

        delay( 10 );
    }
}



