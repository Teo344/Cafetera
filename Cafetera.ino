#include <LiquidCrystal.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Definición de pines para la LCD
#define LCD_RS PC0
#define LCD_EN PC1
#define LCD_D4 PC2
#define LCD_D5 PC3
#define LCD_D6 PC5
#define LCD_D7 PC6

// Configuración de la matriz LED
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 2
#define CLK_PIN PA5
#define DIN_PIN PA7
//Para Azucar
#define CS_PIN PA4
//Para el Agua
#define CS_PIN_2 PB4

// Inicialización de la matriz LED y la LCD
MD_MAX72XX matrixAzucar = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
MD_MAX72XX matrixAgua = MD_MAX72XX(HARDWARE_TYPE, CS_PIN_2, MAX_DEVICES);
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Definición de pines para los botones
#define BUTTON_DERECHA PC8
#define BUTTON_IZQUIERDA PA9
#define BUTTON_CONFIRMACION PC9
#define SWITCH_RECARGA PA10
#define BUTTON_RECARGA_AZUCAR PA8
#define BUTTON_RECARGA_AGUA PA1
#define BUTTON_REGRESO PB0

// Variables de estado
int nivelAzucar = 8;
int nivelAgua = 8;
int temperatura = 70;
int opcion = 1;
int cantidadAzucarElegida = 1;

void setup() {
  // Inicialización de la LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Maquina Cafe");
  lcd.setCursor(0, 1);
  lcd.print("Elija Una");

  // Inicialización de la matriz LED
  matrixAzucar.begin();
  matrixAgua.begin();
  matrixAzucar.clear();
  matrixAgua.clear();

  // Inicialización del motor DC
  //pinMode(MOTOR_PIN, OUTPUT);
  //digitalWrite(MOTOR_PIN, LOW);

  // Inicialización de botones
  pinMode(BUTTON_DERECHA, INPUT_PULLUP);
  pinMode(BUTTON_IZQUIERDA, INPUT_PULLUP);
  pinMode(SWITCH_RECARGA, INPUT_PULLUP);
  pinMode(BUTTON_RECARGA_AZUCAR, INPUT);
  pinMode(BUTTON_REGRESO, INPUT_PULLUP);


  //Configurar matriz de leds
  actualizarNivel();
}

void loop() {
  while (true) {  
    if (digitalRead(BUTTON_CONFIRMACION) == HIGH) {
      delay(300);  
      prepararCafe();  // Llamamos a la función para elegir el café correctamente
    }

    if (digitalRead(SWITCH_RECARGA) == HIGH) {
      delay(300);
      modoConfiguracion();
    }
  }
}


// Función principal para la preparación del café
void prepararCafe() {
  lcd.clear();  // Borra toda la pantalla al inicio
  lcd.setCursor(0, 0);
  lcd.print("Elija su cafe");

  // Bucle para esperar la confirmación
  while (true) {  // Espera la confirmación

    lcd.setCursor(0, 1);
    lcd.print("                ");  // Borra la línea

    lcd.setCursor(0, 1);
    if (opcion == 1) {
      lcd.print("Cappuccino ");
    } else if (opcion == 2) {
      lcd.print("Espresso ");
    } else if (opcion == 3) {
      lcd.print("Mocaccino ");
    } else if (opcion == 4) {
      lcd.print("Latte ");
    }

    // Verificar si se presiona el botón de la derecha
    if (digitalRead(BUTTON_DERECHA) == HIGH) {
      opcion++;                    
      if (opcion > 4) opcion = 1;  // Si se pasa del límite, vuelve a 1
      delay(200);  // Delay para evitar rebotes
    }

    // Verificar si se presiona el botón de la izquierda
    if (digitalRead(BUTTON_IZQUIERDA) == HIGH) {
      opcion--;                    
      if (opcion < 1) opcion = 4;  // Si baja de 1, vuelve al último
      delay(200);  // Delay para evitar rebotes
    }

    // Si el usuario confirma, pasa a seleccionar el azúcar
    if (digitalRead(BUTTON_CONFIRMACION) == HIGH) {
      delay(300);  // Evitar rebote
      seleccionarAzucar();  // Pasa a seleccionar azúcar
      return;
    }

    // Si el usuario presiona el botón de regresar
    if (digitalRead(BUTTON_REGRESO) == HIGH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cafe_Machine");
      lcd.setCursor(0, 1);
      lcd.print("Elija_Opcion");
      delay(300);
      return;  // Sale de la función y vuelve a loop()
    }

    delay(200);  // Pequeño delay para estabilidad
  }
}

// Función para seleccionar el azúcar
void seleccionarAzucar() {
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("Cantidad Azucar");

  while (true) {
    lcd.setCursor(0, 1);
    lcd.print("                ");  
    lcd.setCursor(0, 1);

    for (int i = 0; i < cantidadAzucarElegida; i++) {
      lcd.print("●");  
    }

    if (digitalRead(BUTTON_DERECHA) == HIGH) {
      if (cantidadAzucarElegida < 4) {
        cantidadAzucarElegida++;
        delay(200);  
      }
    }

    if (digitalRead(BUTTON_IZQUIERDA) == HIGH) {
      if (cantidadAzucarElegida > 0) {  // Permitir bajar a 0
        cantidadAzucarElegida--;
        delay(200);  
      }
    }

    if (digitalRead(BUTTON_CONFIRMACION) == HIGH) {
      delay(300);  
      prepararCafeReal();  
      return;  
    }

    if (digitalRead(BUTTON_REGRESO) == HIGH) {
      delay(300);    
      prepararCafe();  
      return;         
    }

    delay(100);  
  }
}


// Función para preparar el café
void prepararCafeReal() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Verificando...");

  // Verificar si hay suficiente agua y azúcar antes de continuar
  if (nivelAzucar < cantidadAzucarElegida) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Azucar insuf.");
    delay(2000);
    return;
  }

  int aguaNecesaria = 0;
  switch(opcion){
    case 1: aguaNecesaria = 2; break;
    case 2: aguaNecesaria = 1; break;
    case 3: aguaNecesaria = 3; break;
    case 4: aguaNecesaria = 2; break;
  }

  if (nivelAgua < aguaNecesaria) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Agua insuf.");
    delay(2000);
    return;
  }

  // Si hay suficiente agua y azúcar, continuar con la preparación
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Preparando... ");

  delay(2000);  // Simula el proceso de preparación

  nivelAzucar -= cantidadAzucarElegida;
  nivelAgua -= aguaNecesaria;

  actualizarNivel();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cafe listo!");

  delay(2000);  // Muestra el mensaje de café listo

  // Regresa al menú principal
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cafe_Machine");
  lcd.setCursor(0, 1);
  lcd.print("Elija_Opcion");
}




void modoConfiguracion() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Modo Recarga ");
  delay(1000);
  lcd.clear();

  while (digitalRead(SWITCH_RECARGA) == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Azucar: ");
    lcd.setCursor(8, 0);
    lcd.print(nivelAzucar);

    lcd.setCursor(0, 1);
    lcd.print("Agua: ");
    lcd.setCursor(6, 1);
    lcd.print(nivelAgua);


    // Recargar azúcar si el botón se presiona
    if (digitalRead(BUTTON_RECARGA_AZUCAR) == HIGH) {
      if (nivelAzucar < 8) {
        nivelAzucar++;
        actualizarNivel();
        delay(200);  // Pequeño delay para evitar rebotes
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Azucar Lleno ");
        delay(400);
      }
    }

    if (digitalRead(BUTTON_RECARGA_AGUA) == HIGH) {
      if (nivelAgua < 8) {
        nivelAgua++;
        actualizarNivel();
        delay(200);  // Pequeño delay para evitar rebotes
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Agua Llena ");
        delay(400);
      }
    }
    delay(200);  // Pequeño retardo para estabilidad
  }

  // Volver al menú principal al salir del modo configuración
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cafe_Machine");
  lcd.setCursor(0, 1);
  lcd.print("Elija_Opcion");
}





void actualizarNivel() {
  matrixAzucar.clear();
  matrixAgua.clear();


  // Representar el nivel de agua en el primer módulo (columnas 0-7)
  for (int i = 0; i < nivelAzucar; i++) {
    for (int j = 0; j < 8; j++) {
      matrixAzucar.setPoint(i, j, true);  // Rellenar de arriba hacia abajo en el primer módulo
    }
  }
  matrixAzucar.update();  // Refrescar la pantalla

  for (int i = 0; i < nivelAgua; i++) {
    for (int j = 0; j < 8; j++) {
      matrixAgua.setPoint(i, j, true);  // Rellenar de arriba hacia abajo en el primer módulo
    }
  }
  matrixAgua.update();  // Refrescar la pantalla
}
