// ESP32 DevKit V1 - AM32 bootloader handshake test (no motor output, no PWM)
// Wiring: GPIO4 -> 330 ohm -> ESC signal pad (J1 pin 8, M4);  ESP32 GND -> ESC GND.
// Serial Monitor: 115200 baud. Commands: s = send handshake, l = check idle line level.

const int PIN = 4;
const uint32_t BAUD = 19200;
uint32_t CPB;          // CPU cycles per bit
uint32_t CYC_PER_MS;   // CPU cycles per millisecond

static inline uint32_t cyc() { return ESP.getCycleCount(); }
static inline void waitUntil(uint32_t t) { while ((int32_t)(cyc() - t) < 0) { } }

// 12 x 0x00, 0x0D, "BLHeli", 0xF4, 0x7D  (21 bytes)
const uint8_t HS[] = {0,0,0,0,0,0,0,0,0,0,0,0, 0x0D,'B','L','H','e','l','i', 0xF4, 0x7D};

// 8N1, LSB first, idle high. Releases the pin half-way through the last stop bit.
void sendBytes(const uint8_t *d, size_t n) {
  uint32_t t = cyc();
  for (size_t k = 0; k < n; k++) {
    digitalWrite(PIN, LOW);  t += CPB; waitUntil(t);             // start bit
    for (int i = 0; i < 8; i++) {                                 // data bits
      digitalWrite(PIN, (d[k] >> i) & 1); t += CPB; waitUntil(t);
    }
    digitalWrite(PIN, HIGH);                                      // stop bit
    if (k == n - 1) {
      t += CPB / 2; waitUntil(t);
      pinMode(PIN, INPUT_PULLUP);                                 // let go of the line
    } else {
      t += CPB; waitUntil(t);
    }
  }
}

bool waitFallingEdge(uint32_t timeoutCycles) {
  uint32_t t0 = cyc();
  while (digitalRead(PIN)) {
    if ((uint32_t)(cyc() - t0) > timeoutCycles) return false;
  }
  return true;
}

int recvByte(uint32_t timeoutCycles) {            // -1 = timeout
  if (!waitFallingEdge(timeoutCycles)) return -1;
  uint32_t t = cyc();                             // start-bit edge
  t += CPB + CPB / 2; waitUntil(t);               // middle of data bit 0
  uint8_t b = 0;
  for (int i = 0; i < 8; i++) {
    if (digitalRead(PIN)) b |= (1 << i);
    t += CPB; waitUntil(t);
  }
  return b;                                       // now in the middle of the stop bit
}

void handshake() {
  uint8_t rx[16];
  int n = 0;
  noInterrupts();
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  sendBytes(HS, sizeof(HS));
  int b = recvByte(50 * CYC_PER_MS);              // wait up to 50 ms for the first byte
  while (b >= 0 && n < 16) { rx[n++] = (uint8_t)b; b = recvByte(5 * CYC_PER_MS); }
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  interrupts();

  Serial.print("Sent 21 bytes. Received "); Serial.print(n); Serial.println(" byte(s):");
  for (int i = 0; i < n; i++) { if (rx[i] < 16) Serial.print('0'); Serial.print(rx[i], HEX); Serial.print(' '); }
  Serial.println();
  if (n >= 3 && rx[0] == 0x34 && rx[1] == 0x37 && rx[2] == 0x31) {
    Serial.println("RESULT: the ESC BOOTLOADER ANSWERED ('471'). The ESC side is healthy.");
  } else if (n == 0) {
    Serial.println("RESULT: no reply (line stuck? ESC not in bootloader? wrong pad/wiring?)");
  } else {
    Serial.println("RESULT: got bytes but not the expected '471' signature (timing/level problem?)");
  }
}

void lineCheck() {
  noInterrupts();
  pinMode(PIN, INPUT_PULLUP);
  delayMicroseconds(200);
  int highs = 0;
  for (int i = 0; i < 100; i++) { highs += digitalRead(PIN); delayMicroseconds(10); }
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  interrupts();
  Serial.print("Idle line was HIGH in "); Serial.print(highs); Serial.println(" of 100 reads.");
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);                        // hold the line HIGH from the start
  CPB = (uint32_t)getCpuFrequencyMhz() * 1000000UL / BAUD;
  CYC_PER_MS = (uint32_t)getCpuFrequencyMhz() * 1000UL;
  Serial.println("Holding the line HIGH. Switch the ESC on now, then send: s = handshake, l = line check");
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == 's') handshake();
    else if (c == 'l') lineCheck();
  }
}
