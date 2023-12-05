#include <unistd.h>
#include <stdio.h>
#include "uart.h"
#include "timer.h"
#include "i2c.h"
#include "bmi270.h"
#include <stdlib.h>

#define ACCEL_THRESHOLD 400 // The threshold that our acceleration needs to pass to be valid

struct accelerations
{
    int Ax;
    int Ay;
    int Az;
};

/**
 * Check if the absolute value of one number (num) is greater than two others
*/
unsigned int compareThree(int num, int compare1, int compare2) {
    if (abs(num) > abs(compare1) & abs(num) > abs(compare2)) {
        return 1;
    }
    
    return 0;
}

/**
 * Given the accelerometer values, output the direction (as an unsigned int) that our I2C device is moving
 * Directions: 0: None, 1: Left, 2: Right, 3: Down, 4: Up
*/
unsigned int generateDirection(struct accelerations *accel) {

    // Check if movement is mostly in the horizontal direction
    if (abs(accel -> Ax) > abs(accel -> Ay)) {
        if (accel -> Ax < 0) {
            return 1; // Left direction
        }
        return 2; // Right direction
    }

    // Check if movement is mostly in vertical direction (Y)
    if (abs(accel -> Ay) > abs(accel -> Ax)) {
        if (accel -> Ay < 0) {
            return 3; // Left direction
        }
        return 4; // Right direction
    }

    return 0;


}


int main() {
    delay(1000);
    
    // uart_puts( "\n----------------------------------------\n\nWelcome\n\n" );
    
    uart_init();
    initI2CPins();
    
    // uart_puts( "init done\n" );
    
    int error = bmi270Init( 0x69 );
    if ( error ) {
        uart_puts( "BMI270 not found\n" );
    } else {
        uart_puts( "BMI270 found\n" );
    }

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
    
    // uart_puts( "\n\n" );
    
    int aVals[6];
    struct acclerations accel;

    while( 1 ) {
        bmi270GetAllData( aVals );

        accel.Ax = aVals[0]
        accel.Ay = aVals[1]
        accel.Az = aVals[2]

        uart_puts(generateDirection(&accel))

        // uart_puts( "Ax: " );
        // uart_int( aVals[0] );
        // uart_puts( "  Ay: " );
        // uart_int( aVals[1] );
        // uart_puts( "  Az: " );
        // uart_int( aVals[2] );
        // uart_puts( "  Gx: " );
        // uart_int( aVals[3] );
        // uart_puts( "  Gy: " );
        // uart_int( aVals[4] );
        // uart_puts( "  Gz: " );
        // uart_int( aVals[5] );
        // uart_puts( "\n" );
        delay( 10 );
    }
}



