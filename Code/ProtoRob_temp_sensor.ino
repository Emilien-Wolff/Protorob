#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <Encoder.h>
#include <RunningMedian.h>
#include <FastLED.h>

// ================== PINS ==================
#define PIN_PWM 9
#define PIN_DIR 7
#define ENC_A   2
#define ENC_B   3

#define LED_PIN        6
#define NUM_LEDS_TOTAL 16    // LEDs physiques
#define NUM_LEDS_TEMP   10    // affichage temperature
#define LED_TYPE       WS2812B
#define COLOR_ORDER    GRB
#define LED_OFFSET     3     // decalage horaire

CRGB leds[NUM_LEDS_TOTAL];

// ================== SYSTEME MECANIQUE ==================
#define ENCODER_PPR     12    // nb de fentes dans l'encodeur par tour
#define QUAD_FACTOR     3     // fronts montants et descendants
#define GEAR_RATIO      19.0
#define NEEDLE_DEG      210.0 // amplitude en ° entre la position max et min

#define TICKS_PER_MOTOR_REV   (ENCODER_PPR * QUAD_FACTOR)
#define TICKS_PER_OUTPUT_REV  (TICKS_PER_MOTOR_REV * GEAR_RATIO)
#define TICKS_MAX (long)(TICKS_PER_OUTPUT_REV * (NEEDLE_DEG / 360.0))
#define OFFSET          -6    //TICKS

// ================== TEMP ==================
#define TEMP_LIMIT_MIN -20.0
#define TEMP_MAX 60.0

// ================== OBJETS ==================
Adafruit_MPL3115A2 tempSensor;
Encoder encoder(ENC_A, ENC_B);
RunningMedian samples(10);

// ================== PID ==================
volatile float Kp = 0.03; //2.0;
volatile float Ki = 0.004; //0.006;
volatile float Kd = 0.01; //10.0;   //impact du Kd négligeable

volatile long targetPosition = 0;
volatile long currentPosition = 0;

volatile float error = 0;
volatile float prevError = 0;
volatile float integral = 0;
volatile float output = 0;

volatile float error_isr = 0;
volatile float output_isr = 0;

#define PWM_MAX 255
#define PWM_MIN 60          // vitesse minimale pour garantir la rotation
#define INTEGRAL_LIMIT 2000

// ================== ROUTINE D'INTERRUPTION (200 Hz) ==================
ISR(TIMER1_COMPA_vect) {

  currentPosition = -encoder.read();
  error = targetPosition - currentPosition;

  integral += error;
  integral = constrain(integral, -INTEGRAL_LIMIT, INTEGRAL_LIMIT);  // partie integrale bornee

  float derivative = error - prevError;                     // calcul de variation de l'erreur
  prevError = error;

  output = Kp * error + Ki * integral + Kd * derivative;    // calcul du PID
  output = constrain(output, -PWM_MAX, PWM_MAX);            // sortie bornee

  error_isr  = error;
  output_isr = output;

  if (abs(error) < 2) {
    analogWrite(PIN_PWM, 0);       // si l'erreur de position est suffisemment faible, on arrete le moteur
  } else {
    int pwm = abs(output);
    pwm = constrain(pwm, PWM_MIN, PWM_MAX);

    digitalWrite(PIN_DIR, (error > 0) ? LOW : HIGH);  // si l'erreur est positive, on tourne dans un sens
    analogWrite(PIN_PWM, pwm);                        // sinon dans l'autre
  }
}

// ================== LED CALIBRATION ==================
void blinkCalibrationLeds(bool state) {             // fait clignoter les leds en orange durant la calibration
  CRGB color = state ? CRGB::Orange : CRGB::Black;
  for (int i = 0; i < NUM_LEDS_TOTAL; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

// ================== CALIBRATION ==================
void calibrateNeedle() {

  Serial.println("=== CALIBRATION EN COURS ===");

  long lastPosition = encoder.read();
  long current;
  int stableCount = 0;
  bool ledState = false;
  unsigned long lastBlink = 0;

  digitalWrite(PIN_DIR, HIGH);      // on fait tourner l'aiguille vers la gauche pour aller sur la butee

  analogWrite(PIN_PWM, 60);             // on lance le moteur à la vitesse min normale
  delay(130);
  analogWrite(PIN_PWM, 30);             // on diminue pour se mettre au plus lent et ne pas casser la butee

  while (stableCount < 3) {

    if (millis() - lastBlink > 250) {   // on change l'etat des leds toutes les 250ms
      lastBlink = millis();
      ledState = !ledState;
      blinkCalibrationLeds(ledState);
    }

    delay(20);
    current = encoder.read();
    long delta = abs(current - lastPosition);
    lastPosition = current;

    if (delta <= 1) stableCount++;      // l'aiguille est bloquee 3 fois de suite pour sortir de la boucle while
    else stableCount = 0;
  }

  analogWrite(PIN_PWM, 0);              // arret du moteur
  encoder.write(0 + OFFSET);            // initialisation du codeur

  FastLED.clear();                      // on eteint les leds
  FastLED.show();                       // application du clear

  integral = 0;
  prevError = 0;
  targetPosition = 0;

  Serial.println("=== CALIBRATION OK ===");
}

// ================== LED TEMPERATURE ==================
void updateTemperatureRing(float temperature) {

  temperature = constrain(temperature, TEMP_LIMIT_MIN, TEMP_MAX);

  int activeLeds = map(
    (int)temperature,
    (int)TEMP_LIMIT_MIN,
    (int)TEMP_MAX,
    1,
    NUM_LEDS_TEMP
  );
  activeLeds = constrain(activeLeds, 1, NUM_LEDS_TEMP);

  FastLED.clear();

  // --- ALLUMAGE DU DÉGRADÉ ---
  for (int i = 0; i < activeLeds; i++) {

    int ledIndex = (LED_OFFSET + i) % NUM_LEDS_TOTAL;

    uint8_t hue = map(i, 0, NUM_LEDS_TEMP - 1, 160, 0); // bleu → rouge
    leds[ledIndex] = CHSV(hue, 255, 255);
  }

  // --- EXTINCTION DES 6 LEDS RESTANTES (ANTI-HORAIRE) ---
  for (int i = NUM_LEDS_TEMP; i < NUM_LEDS_TOTAL; i++) {
    int ledIndex = (LED_OFFSET + i) % NUM_LEDS_TOTAL;
    leds[ledIndex] = CRGB::Black;
  }

  FastLED.show();
}

// ================== TIMER SETUP ==================
void setupTimer1() {                     // setup timer 1 : 16 bits
  cli();                                 // désactive l'interruption globale
  TCCR1A = 0;                            // registres de controle
  TCCR1B = 0;
  OCR1A = 1249;                          // comparaison des sorties sur timer 1
  TCCR1B |= (1 << WGM12);                // activation du mode CTC
  TCCR1B |= (1 << CS11) | (1 << CS10);   // reglage du prescaler
  TIMSK1 |= (1 << OCIE1A);               // flag d'interruption
  sei();                                 // mise en route des interruptions
}

// ================== SETUP ==================
void setup() {

  Serial.begin(115200);
  Wire.begin();

  pinMode(PIN_PWM, OUTPUT);           // pins de controle du pont en H
  pinMode(PIN_DIR, OUTPUT);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS_TOTAL);
  FastLED.setBrightness(10);          // gestion de la luminosite des leds
  FastLED.clear();
  FastLED.show();

  if (!tempSensor.begin()) {          // initialisation de la com avec le capteur de temp
    Serial.println("ERREUR CAPTEUR !");
    while (1);
  }

  calibrateNeedle();                  // recherche de la butee
  setupTimer1();                      // demarrage du timer
}

// ================== LOOP ==================
void loop() {

  float temp = tempSensor.getTemperature(); // mesure de la temperature
  samples.add(temp);                        // stockage dans la variable du PID

  float temperature = samples.getMedian();  // lissage pour eviter les glitchs
  temperature = constrain(temperature, TEMP_LIMIT_MIN, TEMP_MAX);

  targetPosition = map(       // definition des correspondances entre deux intervalles
    (int)temperature,
    (int)TEMP_LIMIT_MIN,      // intervalle de temperatures
    (int)TEMP_MAX,
    0,                        // intervalle des angles
    TICKS_MAX
  );
  targetPosition = constrain(targetPosition, 0, TICKS_MAX); // limitation dans un intervalle realiste
                                                            // par securite materielle
  updateTemperatureRing(temperature);                       // allumage des leds correspondantes

  // ---------- PRINT TEMP TOUTES LES 2 SECONDES ----------
  static unsigned long lastPrint = 0;

  if (millis() - lastPrint >= 2000) {                      // affichage de debug sur console Arduino
    lastPrint = millis();
    Serial.print("Température mesurée : ");
    Serial.print(temperature);
    Serial.println(" °C");
  }
  // -----------------------------------------------------

  delay(50);
}

