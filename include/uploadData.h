#ifndef uploadData_h
#define uploadData_h

#include <stdint.h>


struct uploadData
{
    float avg_temp;     //t1
    float avg_hum;  //h1
        
    //uint16_t pm1_0[3]; //P11
    uint16_t pm2_5[3]; //P251
    uint16_t pm10[3]; //P101
    
    float pv;   //pv
    float pc;   //pc
    int bv;    //bv
    int bc;    //bc

    float lat;
    float lng;
    int ts;  //ts
    int8_t tz;
    char serialNum[16]; //uid
};

#endif