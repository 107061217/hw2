#include "mbed.h"

DigitalIn Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[20] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
                  0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};      // with decimal point
int num[5] = {0};        
Serial pc( USBTX, USBRX );
AnalogIn Ain(A0);
AnalogOut Aout(DAC0_OUT);


int freq;
int sample = 1024;
float ADCdata[1024] = {0};
void SSD(int freq);



int main() 
{
    int i;
    int a, b;

    a = 0;
    b = 0;
    
    for (i=0; i<sample; i++){
        ADCdata[i] = Ain;
        wait(1./sample);
    }
    for (i=0; i<sample; i++){
        pc.printf("%1.3f\r\n", ADCdata[i]);
      
    }

    freq = 0;

    for (i=0; (i<sample) && (b == 0 || a == 0); i++) {
        if (ADCdata[i] < 0.01) {
            a = i;
        }
        else if (ADCdata[i] > 0.625) {
            b = i;
        }
    }

    if (a > b) freq = 512 / (a - b) ;
    else freq = 512 / (b - a) ; 

    while(1){
        if(Switch == 1) {
            redLED = 1;
            greenLED = 0;
            display = 0;      
        }
        else {
            redLED = 0;
            greenLED = 1;
            SSD(freq);    
        }
        for(i=0;i<sample;i++){
            Aout = ADCdata[i];
            wait(1.0/1024);
        }
    }

        

}

void SSD(int freq)
{
    int t;
    int n = 0;
    int i;          

    t = freq;

    while ((t / 10) != 0) {
        num[n] = (t % 10);
        n++;
        t = (t / 10);
    }
    num[n] = t;

    for (i=n; i>=0; i--) {
        if (i == 0) {
            display = table[num[i] + 10];
            wait(1);
        }
        else {
            display = table[num[i]];
            wait(1);
        }
    }
}

