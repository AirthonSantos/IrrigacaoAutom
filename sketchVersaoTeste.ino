#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define LCD_ENDERECO 0x20

const int LEDVERDE = 5;
const int LEDVERMELHO = 6;
const int MOTOR = 10;

int umidadePotenciometro = 0;

// Essa é a variavel que simula o sensor de umidade, ela é incrementada a cada 1s
int umidadeSegundos = 0; 

int leituraPotenciometro;
int leituraPotenciometroformatada;

int valorAtual;
int valorAntigo;

// Crie uma instância do display LCD
LiquidCrystal_I2C lcd(LCD_ENDERECO, 16, 2);

void setup() { 
  // LEDs
  pinMode(LEDVERMELHO, OUTPUT);
  pinMode(LEDVERDE, OUTPUT);
  
  // Motor (Bomba)
  pinMode(MOTOR, OUTPUT);

  // Display
  lcd.init();
  lcd.backlight();
  
}

void loop() { 
  leituraPotenciometro = analogRead(A0);
  leituraPotenciometroformatada = map(leituraPotenciometro, 0, 1023, 0, 100);

  valorAtual = leituraPotenciometroformatada;
  PotenciometroAlterado(leituraPotenciometroformatada);
 
  umidadePotenciometro = leituraPotenciometroformatada; 
  
  if (umidadeSegundos > 0) {
    if (umidadeSegundos <= umidadePotenciometro) {
      digitalWrite(LEDVERMELHO, HIGH);
      digitalWrite(LEDVERDE, LOW);
      
      // Simula a ativação da bomba
      digitalWrite(MOTOR, HIGH);
      
      lcd.clear();
      display("Irrigando");
      lcd.print("...");  
      
      // Simula a terra sendo irrigada
      umidadeSegundos += 10;
        
      // Após a ativação da bomba, o sistema espera cerca de 10 sec antes de verificar a umidade novamente
      delay(10000); 
      
      // Se a umidade estiver acima da especificada, então a terra está úmida
      if (umidadeSegundos > umidadePotenciometro) {
        
        // Desligar o motor
        digitalWrite(MOTOR, LOW);
        
        lcd.clear();
        display("Irrigacao feita");
        
        delay(2000);
        
      }
      
    } else {
      digitalWrite(LEDVERMELHO, LOW);
      digitalWrite(LEDVERDE, HIGH);
    }
  }
  
  // Exibe o nível de umidade no LCD
  display("Umidade da terra");
  lcd.print(umidadeSegundos);
  lcd.print("%");
  
  umidadeSegundos += 1;
  
  delay(1000);
 
}

int display(String string1) {
  // Serve para reduzir a repetição de código

  lcd.setCursor(0, 0);
  lcd.print(string1);
  lcd.setCursor(0, 1);
}

void PotenciometroAlterado(int valor) {
  // Sempre que o potenciometro for alterado, o valor será impresso no display
  
  if (valorAtual != valorAntigo) {
    lcd.clear();
    display("Umidade ajustada");
    lcd.print(valor);
    valorAntigo = valorAtual;
    delay(3000);
  }
}