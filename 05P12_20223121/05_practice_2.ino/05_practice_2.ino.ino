#define LED 7

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
}

void loop() {
  digitalWrite(LED, 0);
  delay(1000);

  for(int i=0; i<5; i++){
    ledFlash();
  }

  digitalWrite(LED, 1);
  delay(1000);

  while(1){}
}

void ledFlash(){
  digitalWrite(LED, 1);
  delay(100);
  digitalWrite(LED, 0);
  delay(100);
}