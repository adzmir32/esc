// ESP32 DevKit V1 - 50 Hz PWM throttle source for a bench spin test of one ESC channel.
// Wiring: GPIO4 (D4) -> ESC signal pad (J1 pin 5 to 8 for M1 to M4); ESP32 GND -> ESC GND.
// Power the ESC first and let it start, then raise the pulse slowly. No propeller; clamp the motor.
// Serial Monitor at 115200 baud: + = target up 10 us, - = target down 10 us, 0 = stop now.
// MAX_US is kept low because the first tests ran through a 50 ohm series resistor bank.
// Do not apply the signal to an unpowered ESC.

const int PIN = 4;
const int MIN_US = 1000;
const int MAX_US = 1250;   // hard limit while the 50 ohm bank is fitted
const int STEP_US = 10;
const int RAMP_US = 2;
const int TICK_MS = 20;

int target = MIN_US;
int current = MIN_US;
unsigned long lastTick = 0;

void writePulse(int p) {
  ledcWrite(PIN, (uint32_t)(p * 65536UL / 20000UL));   // 50 Hz, 16-bit
}

void setup() {
  Serial.begin(115200);
  ledcAttach(PIN, 50, 16);
  writePulse(MIN_US);
  Serial.println("+ = target up, - = target down, 0 = stop now");
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '+') target = min(target + STEP_US, MAX_US);
    else if (c == '-') target = max(target - STEP_US, MIN_US);
    else if (c == '0') { target = MIN_US; current = MIN_US; writePulse(MIN_US); }
    else continue;
    Serial.print("target = "); Serial.print(target); Serial.println(" us");
  }
  if (millis() - lastTick >= TICK_MS) {
    lastTick = millis();
    if (current < target) current = min(current + RAMP_US, target);
    else if (current > target) current = max(current - RAMP_US, target);
    writePulse(current);
  }
}
