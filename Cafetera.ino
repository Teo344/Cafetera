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
#define MAX_DEVICES 3                      
#define CLK_PIN PA5                        
#define CS_PIN PA4                         
#define DIN_PIN PA7                        

// Definición de pines para los botones
#define BUTTON_DERECHA PC8   
#define BUTTON_IZQUIERDA PA9   
#define SWITCH_RECARGA PA10  
#define BUTTON_RECARGA_AZUCAR PA11

// Pin para el motor DC
#define MOTOR_PIN PA3  

// Variables de estado
int nivelAzucar = 8;  // Representa el nivel en la matriz LED (máximo 8)
int nivelAgua = 8;
int temperatura = 70;

// Inicialización de la matriz LED y la LCD
MD_MAX72XX matrix = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
    // Inicialización de la LCD
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Azucar: ");
    lcd.setCursor(0, 1);
    lcd.print("Agua: ");

    // Inicialización de la matriz LED
    matrix.begin();
    matrix.clear();

    // Inicialización del motor DC
    pinMode(MOTOR_PIN, OUTPUT);
    digitalWrite(MOTOR_PIN, LOW);  

    // Inicialización de botones
    pinMode(BUTTON_DERECHA, INPUT_PULLUP);
    pinMode(BUTTON_IZQUIERDA, INPUT_PULLUP);
    pinMode(SWITCH_RECARGA, INPUT_PULLUP);
    pinMode(BUTTON_RECARGA_AZUCAR, INPUT_PULLUP);

    actualizarNivel();
}

void loop() {
    

    while(SWITCH_RECARGA == LOW)





    if (digitalRead(BUTTON_RECARGA_AZUCAR) == LOW) {
        delay(200);  
        if (nivelAzucar > 0) {
            nivelAzucar--;
            actualizarNivel();
        }
    }

    // Recargar azúcar con el switch
    if (digitalRead(SWITCH_RECARGA) == LOW) {
        delay(200);
        nivelAzucar = 8;  // Llenamos el nivel de azúcar
        actualizarNivel();
    }
}

// Función para actualizar el nivel de azúcar y agua en la matriz LED y la LCD
void actualizarNivel() {
    matrix.clear();
    lcd.setCursor(8, 0);
    lcd.print("   "); // Borrar valor anterior
    lcd.setCursor(8, 0);
    lcd.print(nivelAzucar);

    lcd.setCursor(6, 1);
    lcd.print("   "); // Borrar valor anterior
    lcd.setCursor(6, 1);
    lcd.print(nivelAgua);

    // Dibujar barras en la matriz LED
    for (int i = 0; i < nivelAzucar; i++) {
        for (int col = 0; col < 8; col++) {
            matrix.setPoint(i, col, true);  // Activa los puntos en la columna "i"
        }
    }
}
