// Baseado no codigo disponibilizado em:
// http://yaab-arduino.blogspot.com/2015/02/fast-sampling-from-analog-input.html

// Frequencias teoricas (frequencias reais verificadas em um Arduino Nano):
// PS_2 <- nao funciona
// PS_4: 307.7 kHz (196.6   kHz; Ts =   5.086 us)
// PS_8: 153.8 kHz (153.17  kHz; Ts =   6.53  us)
// PS_16: 76.9 kHz ( 76.77  kHz; Ts =  13.03  us)
// PS_32: 38.5 kHz ( 38.38  kHz; Ts =  26.05  us)
// PS_64: 19.2 kHz ( 19.19  kHz; Ts =  51.10  us)
// PS_128: 9.6 kHz (  9.596 kHz; Ts = 104.21  us)
// sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
// for Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles
// prescaler < 16 diminui resolucao, nao adianta usar 10bits
const unsigned char PS_2 = (1 << ADPS0);                   // 2 prescaler for 615 KHz -> nao funciona
const unsigned char PS_4 = (1 << ADPS1);                   // 4 prescaler for 307 KHz -> usar 8-bit
const unsigned char PS_8 = (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz -> usar 8-bit
const unsigned char PS_16 = (1 << ADPS2);                  // 16 prescaler for 76.9 KHz -> maximo com 10-bit
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);   // 32 prescaler for 38.5 KHz
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) |(1 << ADPS0);


bool flag_8bit_global_ADC = false;
unsigned int NRO_MEDIDAS_ADC = 0;
unsigned int contador_medidas_ADC = 0;
byte *vetor_medidas_H_ADC;
byte *vetor_medidas_L_ADC;
bool flag_coleta_terminada_ADC = false;

//////////////////////////////////////////////////////////////////////////////////
void setup_ADC(uint8_t pin, unsigned char PS, bool flag_8bit, unsigned int nro_medidas, byte* vetor_medidas_H, byte* vetor_medidas_L){
  ADCSRA = 0;             // clear ADCSRA register
  ADCSRB = 0;             // clear ADCSRB register
  ADMUX |= (pin & 0x07);  // set analog input pin
  ADMUX |= (1 << REFS0);  // set reference voltage (5V)

  if (flag_8bit){
    // left align ADC value to 8 bits from ADCH register
    // (para realizar coletas de 8bits e ler apenas 1 byte: ADCH)
    ADMUX |= (1 << ADLAR);
    flag_8bit_global_ADC = true;
  }
  
  ADCSRA &= ~PS_128; // 'zera' prescaler
  ADCSRA |= PS;
  // enable auto trigger (inicia prox coleta assim que termina a anterior)
  ADCSRA |= (1 << ADATE);
  ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete

  NRO_MEDIDAS_ADC = nro_medidas;
  vetor_medidas_H_ADC = vetor_medidas_H;
  vetor_medidas_L_ADC = vetor_medidas_L;
}

//////////////////////////////////////////////////////////////////////////////////
void inicia_coleta_ADC(){
  flag_coleta_terminada_ADC = false;
  contador_medidas_ADC = 0;
  ADCSRA |= (1 << ADEN);  // enable ADC
  ADCSRA |= (1 << ADSC);  // start ADC measurements
}

//////////////////////////////////////////////////////////////////////////////////
bool verifica_termino_ADC(){
  return flag_coleta_terminada_ADC;
}

//////////////////////////////////////////////////////////////////////////////////
// Funcao de interrupcao do ADC
ISR(ADC_vect)
{
    if ( (contador_medidas_ADC < NRO_MEDIDAS_ADC) ) {
        // Ler ADCL primeiro!
        if (!flag_8bit_global_ADC){
          vetor_medidas_L_ADC[contador_medidas_ADC] = ADCL;  // read 8 bit value from ADC
        }
        vetor_medidas_H_ADC[contador_medidas_ADC] = ADCH;  // read 8 bit value from ADC
        contador_medidas_ADC++;
    }
    else
    {
      ADCSRA &= ~(1 << ADSC);  // stop ADC measurements
      ADCSRA &= ~(1 << ADEN);  // disable ADC
      flag_coleta_terminada_ADC = true;
    }
}
