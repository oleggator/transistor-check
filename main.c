#include <msp430xG46x.h>
#include <stdio.h>

#include "LCD.h"
#include "checks.h"

#define DAC_V_REF 2500
#define ADC_V_REF 2500
#define DAC0_V 2000
#define DAC1_V 1300
#define VCC 3300

#define R1 330
#define R2 6800
#define R3 100

#define R4 100
#define R5 6800
#define R6 330

#define ADC_CYCLES 64

inline void setupADC();
inline void setupDAC();
inline void setupPorts();

inline void calcVT1();
inline void calcVT2();

volatile float vt1_e_v_sum, vt1_b_v_sum, vt1_k_v_sum,
      vt2_e_v_sum, vt2_b_v_sum, vt2_k_v_sum;

volatile float vt1_e_v, vt1_b_v, vt1_k_v, vt1_h_fe, vt1_input_r,
      vt2_e_v, vt2_b_v, vt2_k_v, vt2_h_fe, vt2_input_r;

volatile int i = 0;

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    setupADC();
    setupDAC();
    setupPorts();

    __enable_interrupt();

    initLCD_A();
    clrLCD();

    while (1) {
        vt1_e_v_sum = 0;
        vt1_b_v_sum = 0;
        vt1_k_v_sum = 0;
        vt2_e_v_sum = 0;
        vt2_b_v_sum = 0;
        vt2_k_v_sum = 0;
        i = 0;

        __bis_SR_register(LPM0_bits);

        calcVT1();
        calcVT2();

        printf("vt1_e_v: %5.0f | vt1_b_v: %5.0f | vt1_k_v: %5.0f | vt1_h_fe: %4.0f | vt1_r: %8.0f || ",
               vt1_e_v, vt1_b_v, vt1_k_v, vt1_h_fe, vt1_input_r);
        printf("vt2_e_v: %5.0f | vt2_b_v: %5.0f | vt2_k_v: %5.0f | vt2_h_fe: %4.0f | vt2_r: %.0f\n",
               vt2_e_v, vt2_b_v, vt2_k_v, vt2_h_fe, vt2_input_r);

        if (check_hfe(vt1_h_fe, vt2_h_fe) && check_input_r(vt1_input_r, vt2_input_r)) {
            displayPass();
        } else {
            displayFail();
        }
    }
}

void setupDAC(void) {
    // dac0 DAC12.0
    DAC12_0CTL = DAC12IR
               | DAC12AMP_5
               | DAC12ENC;
    DAC12_0DAT = DAC0_V * (4096. / DAC_V_REF);

//     dac1 DAC12.1
    DAC12_1CTL = DAC12IR
               | DAC12AMP_5
               | DAC12ENC;
    DAC12_1DAT = DAC1_V * (4096. / DAC_V_REF);
}


void setupADC(void) {
    ADC12CTL0 = REF2_5V     // set reference voltage (for DAC)
              | REFON       // enable reference voltage generator (for DAC)
              | ADC12ON     // enable adc
              | MSC         // enable multi sample conversion
              | SHT0_12;    // 1024 cycles

    ADC12CTL1 |= CONSEQ_1
              | SHP;   // multichannel

    // A0 | 6.0
    P6SEL |= BIT0;
    ADC12MCTL0 |= INCH_0;

    // A1 | 6.1
    P6SEL |= BIT1;
    OA0CTL0 |= OAPM_0;
    ADC12MCTL1 |= INCH_1;

    // A2 | 6.2
    P6SEL |= BIT2;
    ADC12MCTL2 |= INCH_2;

    // A3 | 6.3
    P5SEL |= BIT3;
    ADC12MCTL3 |= INCH_3;

    // A4 | 6.4
    P5SEL |= BIT4;
    ADC12MCTL4 |= INCH_4;

    // A5 | 6.5
    P10SEL |= BIT5;
    ADC12MCTL5 |= INCH_5
               | EOS;


    OA0CTL0 = 0;
    OA0CTL1 = 0;
    ADC12CTL0 |= ENC;
    ADC12IE = BIT5;
}

void setupPorts() {
    // init button
    P1SEL &= ~BIT0;     // enable GPIO in P1.0
    P1IE |= BIT0;       // enable interrupt from P1.0
    P1IES |= BIT0;      // select interrupt-edge
    P1DIR &= ~BIT0;     // set P1.0 to input

    // init all ports
    P2SEL &= ~BIT0;
    P3SEL &= ~BIT0;
    P4SEL &= ~BIT0;
    P7SEL &= ~BIT0;
    P8SEL &= ~BIT0;
    P9SEL &= ~BIT0;
    P10SEL &= ~BIT0;
    PASEL &= ~BIT0;
    PBSEL &= ~BIT0;
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR(void) {
    vt1_e_v_sum += VCC - ADC12MEM3 * (VCC / 4095.);
    vt1_b_v_sum += ADC12MEM4 * (VCC / 4095.) - DAC0_V;
    vt1_k_v_sum += ADC12MEM2 * (VCC / 4095.);

    vt2_k_v_sum += VCC - ADC12MEM3 * (VCC / 4095.);
    vt2_b_v_sum += DAC1_V - ADC12MEM4 * (VCC / 4095.);
    vt2_e_v_sum += ADC12MEM5 * (VCC / 4095.);

    i++;

    if (i < ADC_CYCLES) {
        ADC12CTL0 |= ADC12SC;
    } else {
        __bic_SR_register_on_exit(LPM0_bits);
    }
}

void calcVT1() {
    vt1_e_v = vt1_e_v_sum / ADC_CYCLES;
    vt1_b_v = vt1_b_v_sum / ADC_CYCLES;
    vt1_k_v = vt1_k_v_sum / ADC_CYCLES;

    const float vt1_b_i = vt1_b_v / R2;
    const float vt1_k_i = vt1_k_v / R3;

    vt1_h_fe = vt1_k_i / vt1_b_i;
    vt1_input_r = (vt1_e_v - vt1_b_v) / vt1_b_i;
}

void calcVT2() {
    vt2_e_v = vt2_e_v_sum / ADC_CYCLES;
    vt2_b_v = vt2_b_v_sum / ADC_CYCLES;
    vt2_k_v = vt2_k_v_sum / ADC_CYCLES;

    const float vt2_k_i = vt2_k_v / R4;
    const float vt2_b_i = vt2_b_v / R5;

    vt2_h_fe = vt2_k_i / vt2_b_i;
    vt2_input_r = (vt2_e_v - vt2_b_v) / vt2_b_i;
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void) {
    if (!(P1IN & BIT0)) {
        printf("Reading...\n");
        displayWait();
        ADC12CTL0 |= ADC12SC;
    }

    P1IFG &= ~BIT0;
}
