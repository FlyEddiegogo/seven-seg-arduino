// ======== 可調參數（依實際接線改） ========
const bool ACTIVE_HIGH = true;  // 若接的是主動低（LOW=1），改成 false
#define SERIAL_TEST 1           // 1=開啟序列診斷輸出、0=關閉

// 十位/個位的 BCD 腳位（順序是 A, B, C, D）
const uint8_t TENS_PINS[4] = {2, 3, 4, 5};   // A,B,C,D → 4511 的 A/B/C/D
const uint8_t ONES_PINS[4] = {6, 7, 8, 9};   // A,B,C,D

// ======== 小工具 ========
inline void setPin(uint8_t pin, bool v) {
  digitalWrite(pin, (ACTIVE_HIGH ? v : !v) ? HIGH : LOW);
}

void setPinsOutput(const uint8_t pins[4]) {
  for (uint8_t i = 0; i < 4; ++i) pinMode(pins[i], OUTPUT);
}

// digit 0..9 → 4 個 bit（A=LSB）
static inline void bcdBits(uint8_t digit, bool bits[4]) {
  digit %= 10;
  bits[0] = (digit & 0x01) != 0; // A
  bits[1] = (digit & 0x02) != 0; // B
  bits[2] = (digit & 0x04) != 0; // C
  bits[3] = (digit & 0x08) != 0; // D
}

void writeDigit(uint8_t digit, const uint8_t pins[4]) {
  bool bitv[4];
  bcdBits(digit, bitv);
  for (uint8_t i = 0; i < 4; ++i) setPin(pins[i], bitv[i]);
}

void showNumber(int value) {
  if (value < 0)  value = 0;
  if (value > 99) value = 99;
  const uint8_t tens = value / 10;
  const uint8_t ones = value % 10;
  writeDigit(tens, TENS_PINS);
  writeDigit(ones, ONES_PINS);

#if SERIAL_TEST
  bool tb[4], ob[4];
  bcdBits(tens, tb); bcdBits(ones, ob);
  Serial.print("n="); Serial.print(value);
  Serial.print(" | T("); Serial.print(tens); Serial.print(") A"); Serial.print(tb[0]);
  Serial.print(" B"); Serial.print(tb[1]); Serial.print(" C"); Serial.print(tb[2]); Serial.print(" D"); Serial.print(tb[3]);
  Serial.print(" | O("); Serial.print(ones); Serial.print(") A"); Serial.print(ob[0]);
  Serial.print(" B"); Serial.print(ob[1]); Serial.print(" C"); Serial.print(ob[2]); Serial.print(" D"); Serial.println(ob[3]);
#endif
}

// ======== Arduino 進入點 ========
void setup() {
  setPinsOutput(TENS_PINS);
  setPinsOutput(ONES_PINS);
#if SERIAL_TEST
  Serial.begin(9600);
  while (!Serial) { ; }
#endif
  showNumber(0);
}

void loop() {
  // 自測：00→99
  for (int n = 0; n < 100; ++n) {
    showNumber(n);
    delay(200);
  }
}
