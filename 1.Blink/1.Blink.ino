#define LED_BUILTIN 4

void setup() {
  // Incializar o pino como OUTPUT
  pinMode(LED_BUILTIN, OUTPUT);
}

// Função Loop percorre infinitamente 
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // Liga o LED
  delay(1000);                       // Espera 1s
  digitalWrite(LED_BUILTIN, LOW);    // Desliga o LED
  delay(1000);                       // Espera 1s
}