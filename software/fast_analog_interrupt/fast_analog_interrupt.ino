// Baseado no codigo disponibilizado em:
// http://yaab-arduino.blogspot.com/2015/02/fast-sampling-from-analog-input.html

#include <SPI.h>

#define NRO_MEDIDAS 1000

// Frequencias teoricas (frequencias reais):
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

//////////////////////////////////////////////////////////////////////////////////
// Define variaveis globais
const int PinoPotenciometro = A0;       // pino analogico ligado ao potenciometro
byte vetor_medidas_H[NRO_MEDIDAS];
byte vetor_medidas_L[NRO_MEDIDAS];
unsigned char inicia_coleta = 0;
int contador = 0;                       // nro de bytes enviados
int contador_medidas = 0;
byte oldl, l, h;

void setup()
{

    ADCSRA = 0;             // clear ADCSRA register
    ADCSRB = 0;             // clear ADCSRB register
    ADMUX |= (0 & 0x07);    // set A0 analog input pin
    ADMUX |= (1 << REFS0);  // set reference voltage (5V)
    // left align ADC value to 8 bits from ADCH register
    // (para realizar coletas de 8bits e ler apenas 1 byte: ADCH)
    //ADMUX |= (1 << ADLAR);

    ADCSRA &= ~PS_128; // 'zera' prescaler
    ADCSRA |= PS_4;    // 4 prescaler for 307 KHz (teorico)
    // enable auto trigger (inicia prox coleta assim que termina a anterior)
    ADCSRA |= (1 << ADATE);
    ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
  
    SPI.setDataMode(SPI_MODE0); // configura SPI em modo 0
    pinMode(MISO, OUTPUT);      // define o pino MISO como saida, ja que o Arduino eh escravo
    SPCR |= bit (SPE);          // habilita o barramento SPI
    SPI.attachInterrupt();      // liga interrupcao para SPI
}

//////////////////////////////////////////////////////////////////////////////////
// Funcao de interrupcao do ADC
ISR(ADC_vect)
{
    if ( (contador_medidas < NRO_MEDIDAS) && (inicia_coleta == 1) ) {
        // Ler ADCL primeiro!
        vetor_medidas_L[contador_medidas] = ADCL;  // read 8 bit value from ADC
        vetor_medidas_H[contador_medidas] = ADCH;  // read 8 bit value from ADC
        contador_medidas++;
    }
    else if (contador_medidas >= NRO_MEDIDAS)
    {
      inicia_coleta = 0;
      ADCSRA &= ~(1 << ADSC);  // stop ADC measurements
      ADCSRA &= ~(1 << ADEN);  // disable ADC
     // contador_medidas = 0;
    }
    else if ( inicia_coleta == 2 )
    {
        l = ADCL;
        h = ADCH;
        if ( l >= 100 && oldl <= 100 ) inicia_coleta = 1;
        oldl = l;
    }
}

//////////////////////////////////////////////////////////////////////////////////
// Funcao de interrupcao do barramento SPI
ISR (SPI_STC_vect)  
{
    unsigned char byteRecebido = SPDR;      // le um byte do barramento SPI
    
    switch ( byteRecebido ){
        case 'e':                           // solicitando dados, enviar...
            if ( contador < NRO_MEDIDAS*2 )
            {
                if ( (contador % 2) == 0 )  // contador par
                {
                    SPDR = vetor_medidas_L[contador/2]; // envia byte menos significativo
                }
                else                        // contador impar
                {
                    SPDR = vetor_medidas_H[(contador-1)/2]; // envia byte mais significativo
                }
                contador++;
            }
            break;

        case 'i':                           // inicia coleta
            SPDR = 'j';                     // responde 'j', indicando que entendeu comando
            contador = 0;
            contador_medidas = 0;
            inicia_coleta = 1;
            ADCSRA |= (1 << ADEN);  // enable ADC
            ADCSRA |= (1 << ADSC);  // start ADC measurements
            break;
            
        case 'k':                           // inicia coleta com trigger UP
            SPDR = 'l';                     // responde 'l', indicando que entendeu comando
            contador = 0;
            contador_medidas = 0;
            oldl = 0xFF;
            inicia_coleta = 2;
            ADCSRA |= (1 << ADEN);  // enable ADC
            ADCSRA |= (1 << ADSC);  // start ADC measurements
            break;
            
        default:                            // outro byte recebido, ignora...
            break;
    }
}


void loop()
{
    delay(1000);
}
