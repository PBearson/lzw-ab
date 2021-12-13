#pragma once

#include <stdint.h>
#ifndef uploadData_h
#define uploadData_h

struct uploadData
{
    float avg_temp;     //t1
    float avg_press; //p1
    float avg_hum;  //h1
    
    float scd30_ppm; //c1
    
    //uint16_t pm1_0[3]; //P11
    uint16_t pm2_5[3]; //P251
    uint16_t pm10[3]; //P101
    
    float pv;   //pv
    float pc;   //pc
    float pp;   //pp
    int bv;    //bv
    int bc;    //bc
    int bt;    //bt
    //uint32_t dVal;  //d
    //uint32_t dCnt;  //cnt
    
    #ifndef DATA_STRUCT_V2

    char gps[20];  //L
    char ts[21];  //ts
    char tz[4];   //TZ
    char serialNum[33]; //uid

    #else

    float lat;
    float lng;
    int ts;  //ts
    int8_t tz;
    char serialNum[16]; //uid

    #endif
};

#endif /* uploadData_h */
