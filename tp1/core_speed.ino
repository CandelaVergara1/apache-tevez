#include "esp32-hal-cpu.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 1. Probamos a 80 MHz (Velocidad baja)
  setCpuFrequencyMhz(80); 
  delay(500);
  ejecutarPrueba();

  // 2. Probamos a 160 MHz (Velocidad estándar)
  setCpuFrequencyMhz(160); 
    delay(500);
  ejecutarPrueba();

}

void ejecutarPrueba() {
  uint32_t freq = getCpuFrequencyMhz();
  Serial.printf("\n--- Test a %d MHz ---\n", freq);
  Serial.flush();

  long iteraciones = 100000000; // Mantenemos el mismo trabajo
  volatile int suma = 0;
  
  long t0 = micros();
  for (long i = 0; i < iteraciones; i++) {
    suma += 1;
  }
  long t1 = micros();
  
  Serial.printf("Tiempo: %.4f segundos\n", (t1 - t0) / 1000000.0);
  Serial.flush();
}

void loop() {}