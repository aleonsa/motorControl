#include <Arduino.h>

const int pinA = 14; // Pin connected to pinA of the encoder
const int pinB = 27; // Pin connected to pinB of the encoder
const int ppr = 1000;
const int pinADC = 36;

const int pwmPin = 32;        // pin del pwm
const int pwmChannel = 0;     // canal del pwm
const int pwmResolution = 10; // resolucion del pwm
const int pwmFrequency = 250; // frecuencia del pwm

const int in4 = 25; // pin del pwm
const int in3 = 26; // pin del pwm

volatile long encoderCount = 0; // Variable to store the encoder count
volatile bool direction = false;
volatile float q = 0;  // angular position
volatile float dq = 0; // diff of angular position

volatile float vel = 0; // velocity

volatile float radians = 0;
volatile float current = 0;

volatile int valorADC = 0;
volatile int ducy = 100;

volatile int increment = 1;
volatile bool timerFlag = false; // bandera para interrupcion del timer

// Parametros del filtro paso altas para derivar
const float dt = 0.005;
const u_int8_t L = 200;
volatile float k1 = 0;
volatile float x = 0;


hw_timer_t *timer = NULL;                             // apuntador del timer
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; // sincroniza main loop y la interrup

void handleEncoder();
void IRAM_ATTR onTimer();

void setup()
{
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, dt*1000000, true); // 
  timerAlarmEnable(timer);

  Serial.begin(115200); // Initialize serial communication
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);

  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(pwmPin, pwmChannel);
  ledcWrite(pwmChannel, 0); // inicia con DC

  attachInterrupt(digitalPinToInterrupt(pinA), handleEncoder, RISING);
  analogReadResolution(12);
}

void loop()
{
  if (timerFlag) // detecta bandera de interrupcion
  {
    timerFlag = false; // baja la bandera
    // ------------------------------------
    /*VELOCIDAD CON DEFINICION DE DERIVADA*/
    // dq = radians - q;
    // q = radians;
    // vel = dq / dt;

    /*VELOCIDAD CON FILTRO PASO ALTAS*/
    k1 = -L*x + L*radians;
    vel = k1;
    x = k1*dt + x;
    // Incrementa o decrementa el ciclo de trabajo del PWM
    ducy += increment;
    if (ducy >= 1023 || ducy <= 100)
    {
      increment = -increment; // Cambia la dirección al alcanzar los límites
    }
    valorADC = analogRead(pinADC);
    valorADC = (analogRead(pinADC)+valorADC)/2;
    current = (valorADC * 3.3 / 4095) / 2;

    ledcWrite(pwmChannel, ducy); // cambiamos el valor del pwm
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%d, %f, %f;", ducy, current, vel);
    Serial.println(buffer);
  }
  radians = encoderCount * (2 * 3.141593) / ppr;
}

void handleEncoder()
{
  // Read the state of pinA and pinB
  int stateA = digitalRead(pinA);
  int stateB = digitalRead(pinB);

  // Update the encoder count based on the quadrature encoder logic
  if (stateA == stateB)
  {
    encoderCount++;
    direction = true; // CW
  }
  else
  {
    encoderCount--;
    direction = false; // CCW
  }
}

void IRAM_ATTR onTimer()
{
  portENTER_CRITICAL_ISR(&timerMux); // prioridad critica
  timerFlag = true;                  // levanta bandera de interrupcion
  portEXIT_CRITICAL_ISR(&timerMux);
}