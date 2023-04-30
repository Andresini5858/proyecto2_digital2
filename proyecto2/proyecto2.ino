#include <stdint.h> //Librería para funciones de C
#include <ili9341_8_bits_fast.h> //Librería para la pantalla con chip ILI9341
#include <SPI.h> //Librería para la comunicacion SPI
#include <SD.h> //Librería para comunicación con la memoria microSD

File myFile; //Definimos objeto de la librería

// Pines usados
//#define LCD_RST PD_0
//#define LCD_CS PD_1
//#define LCD_RS PD_2
//#define LCD_WR PD_3
//#define LCD_RD PA_6
// hay que modificar los pines en la librería
//int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};

extern uint8_t projectile []; //Tramos de la memoria flash un sprite

int hp_bar; //Variable para la barra de la vida
int stop_over = 0; //Variable para ver si el jugador terminó los sets pero aún no derrotó al enemigo
int set = 0; //Variable para ver el set de enemigos
int etapa = 0; //Variable de la etapa del juego
int bandera = 0; //Bandera para antirrebotes
signed int hp_muffet = 200; //Variable de la vida del jefe
int x_heart = 152; //Coordenada en x del jugador
int y_heart = 148; //Coordenada en y del jugador
int x_attack = 20; //Coordenada del rectángulo de ataque
int enemigo_1x = 40; //Coordenada en x del enemigo 1
int enemigo_1y = 113; //Coordenada en y del enemigo 1
int enemigo_2x = 264; //Coordenada en x del enemigo 2
int enemigo_2y = 148; //Coordenada en y del enemigo 2
int enemigo_3x = 40; //Coordenada en x del enemigo 3
int enemigo_3y = 183; //Coordenada en y del enemigo 3
int enemigo_4x = 40; //Coordenada en x del enemigo 4
int enemigo_4y = 183; //Coordenada en y del enemigo 4
int enemigo_5x = 40; //Coordenada en x del enemigo 5
int enemigo_5y = 148; //Coordenada en y del enemigo 5
int enemigo_6x = 30; //Coordenada en x del enemigo 6
int enemigo_6y = 20; //Coordenada en y del enemigo 6
int enemigo_7x = 220; //Coordenada en x del enemigo 7
int enemigo_7y = 20; //Coordenada en y del enemigo 7
int enemigo_8x = 210; //Coordenada en x del enemigo 8
int enemigo_8y = 20; //Coordenada en y del enemigo 8
int enemigo_9x = 40; //Coordenada en x del enemigo 9
int enemigo_9y = 148; //Coordenada en y del enemigo 9
int enemigo_10x = 40; //Coordenada en x del enemigo 10
int enemigo_10y = 148; //Coordenada en y del enemigo 10
int background = 0; //Variable para no volver a imprimir el fondo en loop
int retardo = 0; //Retardo para enemigos
int risa = 0; 
int clean = 0; //Variable para limpiar enemigos
int bandera_perdedor = 0; //Variable para el fondo de perdedor
int attack_flag = 0; //Variable para definir en loop el momento de ataque
int overtime = 0; //Variable para definir si el jugador terminó los sets sin ganar el juego
bool collision1 = false; //Definir colisión con enemigo 1
bool collision2 = false; //Definir colisión con enemigo 2
bool collision3 = false; //Definir colisión con enemigo 3
bool collision4 = false; //Definir colisión con enemigo 4
bool collision5 = false; //Definir colisión con enemigo 5
bool collision6 = false; //Definir colisión con enemigo 6
bool collision7 = false; //Definir colisión con enemigo 7
bool collision8 = false; //Definir colisión con enemigo 8
bool collision9 = false; //Definir colisión con enemigo 9
bool collision10 = false; //Definir colisión con enemigo 10
char buffer[32]; //Arreglo para mostrar variable en la pantalla

int asciitohex (int a); //Función para pasar ASCII a valor hexadecimal
void mapeoSD(); //Función para mostrar imagen de la memoria microSD
bool Collision(int x1, int y1, int h1, int w1, int x2, int y2, int h2, int w2); //Función de colisión

#include "bitmaps.h" //Incluir sprites

void setup() {
  // put your setup code here, to run once:
  pinMode(PC_4, OUTPUT);
  pinMode(PC_5, OUTPUT);
  pinMode(PC_6, OUTPUT);
  pinMode(PA_7, INPUT_PULLUP);
  pinMode(PE_3, INPUT_PULLUP);
  pinMode(PF_4, INPUT_PULLUP);
  pinMode(PA_3, OUTPUT); //Se define PA_3 como salida y se convierte en slave select

  digitalWrite(PC_4, LOW);
  digitalWrite(PC_5, LOW);
  digitalWrite(PC_6, LOW);
  
  Serial.begin(115200);
  Serial.println("Iniciando pantalla...");
  SPI.setModule(0); //Utilizar el SPI Module 0
  
  lcdInit();
  lcdClear(0x0000);

   Serial.print("Initializing SD card..."); //Indicar que se está inicializando la memoria

  if (!SD.begin(PA_3)){ //Verificar que el slave select este en 0, y se responda un 1
    Serial.println("initialization failed!"); //Si no falló la inicialización
    return;
  }
  Serial.println("initialization done."); //Se inició la comunicación

}

void loop() {
  lcdClear(0x0000);
  myFile = SD.open("muf.txt");
  mapeoSD();
  LCD_Print("MENU", 140, 30, 2, 0xFFFF, 0x0000);
  LCD_Print("PRESS START", 130, 165, 1, 0xFFFF, 0x0000);
  while (etapa == 0){
  digitalWrite(PC_4, HIGH);
  digitalWrite(PC_5, LOW);
  digitalWrite(PC_6, LOW);
  detachInterrupt(PE_3);
  detachInterrupt(PA_7);
  
    if (digitalRead(PF_4) == LOW){
      
      delay(50);
      etapa = 1;
    }
  }

  while (etapa == 1){
    digitalWrite(PC_4, LOW);
    digitalWrite(PC_5, HIGH);
    digitalWrite(PC_6, LOW);
    detachInterrupt(PF_4);

    if (overtime == 1 && stop_over == 0){
      set = int(random(12));
      if (set == 0){
        enemigo_1x = 40;
        enemigo_2x = 264;
      }
      else if (set == 1){
        enemigo_1x = 40;
        enemigo_3x = 40;
        enemigo_4x = 40;
      }
      else if (set == 2){
        retardo = 0;
        clean = 0;
        enemigo_1x = 40;
        enemigo_2x = 264;
        enemigo_3x = 40;
        enemigo_4x = 40;
      }
      else if (set == 3){
        retardo = 0;
        enemigo_1x = 40;
        enemigo_3x = 40;
        enemigo_5x = 40;
      }

      else if (set == 4){
        set = 3;
        retardo = 0;
        enemigo_1x = 40;
        enemigo_3x = 40;
        enemigo_5x = 40;
      }
      else if (set == 5){
        enemigo_6x = 30;
        enemigo_6y = 20;
      }

      else if (set == 6){
        enemigo_6x = 30;
        enemigo_6y = 20;
        enemigo_7x = 220;
        enemigo_7y = 20;
        retardo = 0;
      }
      else if (set == 7){
        enemigo_6x = 90;
        enemigo_6y = 20;
        enemigo_7x = 140;
        enemigo_7y = 220;
        retardo = 0;
        x_heart = 152;
      }
      else if (set == 8){
        enemigo_6x = 50;
        enemigo_6y = 20;
        enemigo_7x = 140;
        enemigo_7y = 20;
        enemigo_8x = 180;
        enemigo_8y = 20;
        enemigo_9x = 220;
        enemigo_9y = 20;
        enemigo_10x = 260;
        enemigo_10y = 20;
        x_heart = 152;
      }
      else if (set == 9){
        x_heart = 152;
        enemigo_6x = 40;
        enemigo_6y = 210;
        enemigo_7x = 90;
        enemigo_7y = 210;
        enemigo_8x = 140;
        enemigo_8y = 210;
        enemigo_9x = 190;
        enemigo_9y = 210;
        enemigo_10x = 240;
        enemigo_10y = 210;
      }
      else if (set == 10){
        x_heart = 152;
        y_heart = 148;
        enemigo_6x = 240;
        enemigo_6y = 183;
        enemigo_7x = 120;
        enemigo_7y = 20;
        enemigo_8x = 160;
        enemigo_8y = 20;
        enemigo_9x = 200;
        enemigo_9y = 20;
        enemigo_10x = 240;
        enemigo_10y = 20;
      }
      else if (set == 11){
        retardo =  0;
        enemigo_1x = 40;
        enemigo_2x = 40;
        enemigo_3x = 40;
      }
      stop_over = 1;
    }
    
    if (background == 0){
      lcdClear(0x0000);
  
      FillRect(18, 98, 282, 120, 0xFFFF);
      
      delay(500);
      FillRect(20, 100, 278, 116, 0xF1FA);
      delay(500);
      FillRect(20, 100, 278, 116, 0xB914);
      delay(500);
      FillRect(20, 100, 278, 116, 0x80AD);
      delay(500);
      FillRect(20, 100, 278, 116, 0x5869);
      delay(500);
      FillRect(20, 100, 278, 116, 0x0000);
      delay(250);
      FillRect(40, 120, 240, 2,  0xA091);
      delay(250);
      FillRect(40, 155, 240, 2,  0xA091);
      delay(250);
      FillRect(40, 190, 240, 2,  0xA091);
      background = 1;
    }
  
  
  
  if (risa == 0){
    for (int i = 0; i < 80; i++){
      LCD_Sprite_Zoom(134, 25, 26, 35, muffet, 5, (i/20)%5, 0, 2, 0x0000);
      risa = 1;
    }
  }

  if (set < 5 || set > 9){
    LCD_Bitmap(x_heart, y_heart, 16, 16, heart);
    attachInterrupt(digitalPinToInterrupt(PE_3), up, FALLING);
    attachInterrupt(digitalPinToInterrupt(PA_7), down, FALLING);
  }

  else  {
    LCD_Bitmap(x_heart, 111, 16, 16, heart);
    attachInterrupt(digitalPinToInterrupt(PE_3), right, FALLING);
    attachInterrupt(digitalPinToInterrupt(PA_7), left, FALLING);
  }
  

  if (set == 0){
    interrupts();
    collision1 = Collision(x_heart, y_heart, 16, 16, enemigo_1x, enemigo_1y, 16, 16); // detección de colisión
    collision2 = Collision(x_heart, y_heart, 16, 16, enemigo_2x, enemigo_2y, 16, 16); // detección de colisión
    
    if (collision1 || collision2){
      etapa = 2;
    }
    
    LCD_Bitmap(enemigo_1x, enemigo_1y, 16, 16, spider);
    LCD_Bitmap(enemigo_2x, enemigo_2y, 16, 16, spider);
    
    if (enemigo_1x < 264){
      enemigo_1x = enemigo_1x + 1;
      enemigo_2x = enemigo_2x - 1;
      V_line(enemigo_1x - 1 , 113, 16, 0x0000);
      V_line(enemigo_1x - 1, 120, 1, 0xA091);
      V_line(enemigo_2x + 17 , 148, 16, 0x0000);
      V_line(enemigo_2x + 17, 155, 1, 0xA091);

    }
  
    else {
      FillRect(enemigo_1x , 113, 16, 16, 0x0000);
      FillRect(enemigo_1x, 120, 16, 2, 0xA091);
      FillRect(enemigo_2x , 148, 17, 16, 0x0000);
      FillRect(enemigo_2x, 155, 17, 2, 0xA091);
      FillRect(200, 50, 100, 100, 0x0000);
      attack();
      set = 1;
      enemigo_1x = 40;
      enemigo_2x = 264;
      check_over();
    }

  }
  else if (set == 1){
    if (background == 2){
      noInterrupts();
      FillRect(18, 98, 282, 120, 0xFFFF);
      FillRect(20, 100, 278, 116, 0xF1FA);
      FillRect(20, 100, 278, 116, 0xB914);
      FillRect(20, 100, 278, 116, 0x80AD);
      FillRect(20, 100, 278, 116, 0x5869);
      FillRect(20, 100, 278, 116, 0x0000);
      FillRect(40, 120, 240, 2,  0xA091);
      FillRect(40, 155, 240, 2,  0xA091);
      FillRect(40, 190, 240, 2,  0xA091);
      background = 1;
    }
    interrupts();
    collision1 = Collision(x_heart, y_heart, 16, 16, enemigo_1x, enemigo_1y, 16, 16); // detección de colisión
    collision3 = Collision(x_heart, y_heart, 16, 16, enemigo_3x, enemigo_3y, 16, 16); // detección de colisión
    collision4 = Collision(x_heart, y_heart, 16, 16, enemigo_4x, enemigo_4y, 16, 16); // detección de colisión
    
    if (collision1 || collision3 || collision4){
      etapa = 2;
    }
    
    if (enemigo_3x >= 80){
      LCD_Bitmap(enemigo_4x, enemigo_4y, 16, 16, spider);
      retardo = 1;
    }  

    if (retardo == 1){
      enemigo_4x = enemigo_4x + 1;
      V_line(enemigo_4x - 1 , 183, 16, 0x0000);
      V_line(enemigo_4x - 1, 190, 1, 0xA091);
    }

    if (enemigo_1x < 264){
      LCD_Bitmap(enemigo_1x, enemigo_1y, 16, 16, spider);
      LCD_Bitmap(enemigo_3x, enemigo_3y, 16, 16, spider);
      enemigo_1x = enemigo_1x + 1;
      enemigo_3x = enemigo_3x + 1;
      V_line(enemigo_1x - 1 , 113, 16, 0x0000);
      V_line(enemigo_1x - 1, 120, 1, 0xA091);
      V_line(enemigo_3x - 1 , 183, 16, 0x0000);
      V_line(enemigo_3x - 1, 190, 1, 0xA091);
      
    }

    else if (enemigo_1x >= 264 && clean == 0){
      FillRect(enemigo_1x , 113, 16, 16, 0x0000);
      FillRect(enemigo_1x, 120, 16, 2, 0xA091);
      FillRect(enemigo_3x , 183, 17, 16, 0x0000);
      FillRect(enemigo_3x, 190, 17, 2, 0xA091);
      clean = 1;
    }

    else if (enemigo_4x == 264 && clean == 1){
      FillRect(enemigo_4x , 183, 17, 16, 0x0000);
      FillRect(enemigo_4x, 190, 17, 2, 0xA091);
      enemigo_1x = 40;
      enemigo_3x = 40;
      enemigo_4x = 40;
      FillRect(200, 50, 100, 100, 0x0000);
      attack();
      clean = 0;
      retardo = 0;
      set = 2;
      check_over(); 
    }
    }

    else if (set == 2){
      if (background == 2){
        noInterrupts();
        FillRect(18, 98, 282, 120, 0xFFFF);
        FillRect(20, 100, 278, 116, 0xF1FA);
        FillRect(20, 100, 278, 116, 0xB914);
        FillRect(20, 100, 278, 116, 0x80AD);
        FillRect(20, 100, 278, 116, 0x5869);
        FillRect(20, 100, 278, 116, 0x0000);
        FillRect(40, 120, 240, 2,  0xA091);
        FillRect(40, 155, 240, 2,  0xA091);
        FillRect(40, 190, 240, 2,  0xA091);
        background = 1;
      }
      interrupts();
      collision1 = Collision(x_heart, y_heart, 16, 16, enemigo_1x, enemigo_1y, 16, 16); // detección de colisión
      collision2 = Collision(x_heart, y_heart, 16, 16, enemigo_2x, enemigo_2y, 16, 16); // detección de colisión
      collision3 = Collision(x_heart, y_heart, 16, 16, enemigo_3x, enemigo_3y, 16, 16); // detección de colisión
      collision4 = Collision(x_heart, y_heart, 16, 16, enemigo_4x, enemigo_4y, 16, 16); // detección de colisión
    
      if (collision1 || collision2 || collision3 || collision4){
        etapa = 2;
      }

      if (enemigo_3x >= 120){
        LCD_Bitmap(enemigo_2x, enemigo_2y, 16, 16, spider);
        LCD_Bitmap(enemigo_4x, enemigo_4y, 16, 16, spider);
        retardo = 1;
      }

      if (retardo == 1){
        enemigo_4x = enemigo_4x + 1;
        enemigo_2x = enemigo_2x - 1;
        V_line(enemigo_4x - 1 , 183, 16, 0x0000);
        V_line(enemigo_4x - 1, 190, 1, 0xA091);
        V_line(enemigo_2x + 17 , 148, 16, 0x0000);
        V_line(enemigo_2x + 17, 155, 1, 0xA091);
      } 

      if (enemigo_1x < 264){
        LCD_Bitmap(enemigo_1x, enemigo_1y, 16, 16, spider);
        LCD_Bitmap(enemigo_3x, enemigo_3y, 16, 16, spider);
        enemigo_1x = enemigo_1x + 1;
        enemigo_3x = enemigo_3x + 1;
        V_line(enemigo_1x - 1 , 113, 16, 0x0000);
        V_line(enemigo_1x - 1, 120, 1, 0xA091);
        V_line(enemigo_3x - 1 , 183, 16, 0x0000);
        V_line(enemigo_3x - 1, 190, 1, 0xA091);     
      }

      else if (enemigo_1x >= 264 && clean == 0){
        FillRect(enemigo_1x , 113, 16, 16, 0x0000);
        FillRect(enemigo_1x, 120, 16, 2, 0xA091);
        FillRect(enemigo_3x , 183, 17, 16, 0x0000);
        FillRect(enemigo_3x, 190, 17, 2, 0xA091);
        clean = 1;
      }

      else if (enemigo_4x == 264 && clean == 1){
        FillRect(enemigo_4x , 183, 17, 16, 0x0000);
        FillRect(enemigo_4x, 190, 17, 2, 0xA091);
        FillRect(enemigo_2x , 148, 17, 16, 0x0000);
        FillRect(enemigo_2x, 155, 17, 2, 0xA091);
        enemigo_1x = 40;
        enemigo_2x = 264;
        enemigo_3x = 40;
        enemigo_4x = 40;
        FillRect(200, 50, 100, 100, 0x0000);
        attack();
        clean = 0;
        retardo = 0;
        set = 3;
        check_over();
      }
    }

    else if (set == 3){
      if (background == 2){
        noInterrupts();
        FillRect(18, 98, 282, 120, 0xFFFF);
        FillRect(20, 100, 278, 116, 0xF1FA);
        FillRect(20, 100, 278, 116, 0xB914);
        FillRect(20, 100, 278, 116, 0x80AD);
        FillRect(20, 100, 278, 116, 0x5869);
        FillRect(20, 100, 278, 116, 0x0000);
        FillRect(40, 120, 240, 2,  0xA091);
        FillRect(40, 155, 240, 2,  0xA091);
        FillRect(40, 190, 240, 2,  0xA091);
        background = 1;
      }
      interrupts();
      collision5 = Collision(x_heart, y_heart, 16, 16, enemigo_5x, enemigo_5y, 16, 16); // detección de colisión
    
      if (collision5){
        etapa = 2;
      }

      if (enemigo_5x < 264 && retardo == 0){
        delay(1);
        LCD_Bitmap(enemigo_5x, enemigo_5y, 16, 16, spider);
        enemigo_5x = enemigo_5x + 2;
        V_line(enemigo_5x - 1 , 148, 16, 0x0000);
        V_line(enemigo_5x - 1, 155, 1, 0xA091);
        V_line(enemigo_5x - 2 , 148, 16, 0x0000);
        V_line(enemigo_5x - 2, 155, 1, 0xA091);
      }

      else if (enemigo_5x >= 264){
        FillRect(enemigo_5x , 148, 17, 16, 0x0000);
        FillRect(enemigo_5x, 155, 17, 2, 0xA091);
        enemigo_5x = 40;
        retardo = 0;
        set = 4;
      }  
    }

    else if (set == 4){
      collision3 = Collision(x_heart, y_heart, 16, 16, enemigo_3x, enemigo_3y, 16, 16); // detección de colisión
      collision1 = Collision(x_heart, y_heart, 16, 16, enemigo_1x, enemigo_1y, 16, 16); // detección de colisión
      
    if (collision1 || collision3){
        etapa = 2;
      }

    if (enemigo_3x < 264 && retardo == 0){
      LCD_Bitmap(enemigo_3x, enemigo_3y, 16, 16, spider);
      LCD_Bitmap(enemigo_1x, enemigo_1y, 16, 16, spider);
      enemigo_3x = enemigo_3x + 2;
      enemigo_1x = enemigo_1x + 2;
      V_line(enemigo_1x - 1 , 113, 16, 0x0000);
      V_line(enemigo_1x - 1, 120, 1, 0xA091);
      V_line(enemigo_1x - 2 , 113, 16, 0x0000);
      V_line(enemigo_1x - 2, 120, 1, 0xA091);
      V_line(enemigo_3x - 1 , 183, 16, 0x0000);
      V_line(enemigo_3x - 1, 190, 1, 0xA091);
      V_line(enemigo_3x - 2 , 183, 16, 0x0000);
      V_line(enemigo_3x - 2, 190, 1, 0xA091);
     }

     else if (enemigo_3x >= 264){
        FillRect(enemigo_1x , 113, 17, 16, 0x0000);
        FillRect(enemigo_1x, 120, 17, 2, 0xA091);
        FillRect(enemigo_3x , 183, 17, 16, 0x0000);
        FillRect(enemigo_3x, 190, 17, 2, 0xA091);
        enemigo_3x = 40;
        enemigo_2x = 264;
        enemigo_4x = 40;
        enemigo_1x = 40;
        enemigo_5x = 40;
        x_heart = 152;
        retardo = 0;
        FillRect(200, 50, 100, 100, 0x0000);
        attack();
        set = 5;
        check_over();
      }    
    }

    else if (set == 5){
      if (background == 2){
        noInterrupts();
        lcdClear(0x0000);
        FillRect(18, 28, 282, 200, 0xFFFF);
        FillRect(20, 30, 278, 196, 0xF1FA);
        FillRect(20, 30, 278, 196, 0xB914);
        FillRect(20, 30, 278, 196, 0x80AD);
        FillRect(20, 30, 278, 196, 0x5869);
        FillRect(20, 30, 278, 196, 0x0000);
        FillRect(40, 118, 240, 2,  0xA091);
        background = 1;
      }
      interrupts();
      collision6 = Collision(x_heart, 111, 16, 16, enemigo_6x, enemigo_6y, 16, 16); // detección de colisión
      collision7 = Collision(x_heart, 111, 16, 16, enemigo_7x, enemigo_7y, 16, 16); // detección de colisión
      collision8 = Collision(x_heart, 111, 16, 16, enemigo_8x, enemigo_8y, 16, 16); // detección de colisión
      collision9 = Collision(x_heart, 111, 16, 16, enemigo_9x, enemigo_9y, 16, 16); // detección de colisión

      if (collision6){
        etapa = 2;
      }

      delay(3);
      if (enemigo_6y < 210){
        LCD_Bitmap(enemigo_6x, enemigo_6y, 16, 16, projectile);
        enemigo_6x += 1;
        enemigo_6y += 1;
        if (enemigo_6y == 120){
          V_line(enemigo_6x - 1, enemigo_6y - 1, 16, 0x0000);
          H_line(enemigo_6x - 1, enemigo_6y - 1, 16, 0x0000);
          FillRect(enemigo_6x - 18, 118, 40, 2, 0xA091);
        }
        else {
          V_line(enemigo_6x - 1, enemigo_6y - 1, 16, 0x0000);
          H_line(enemigo_6x - 1, enemigo_6y - 1, 16, 0x0000);
        } 
      }

      else {
        FillRect(enemigo_6x, enemigo_6y, 16, 16, 0x0000);
        enemigo_6x = 30;
        enemigo_6y = 20;
        retardo = 0;
        detachInterrupt(PE_3);
        detachInterrupt(PA_7);
        set = 6;      
      }
    }

    else if (set == 6){
      if (background == 2){
        noInterrupts();
        lcdClear(0x0000);
        FillRect(18, 28, 282, 200, 0xFFFF);
        FillRect(20, 30, 278, 196, 0xF1FA);
        FillRect(20, 30, 278, 196, 0xB914);
        FillRect(20, 30, 278, 196, 0x80AD);
        FillRect(20, 30, 278, 196, 0x5869);
        FillRect(20, 30, 278, 196, 0x0000);
        FillRect(40, 118, 240, 2,  0xA091);
        background = 1;
      }

      interrupts();
      collision6 = Collision(x_heart, 111, 16, 16, enemigo_6x, enemigo_6y, 16, 16); // detección de colisión
      collision7 = Collision(x_heart, 111, 16, 16, enemigo_7x, enemigo_7y, 16, 16); // detección de colisión
      collision8 = Collision(x_heart, 111, 16, 16, enemigo_8x, enemigo_8y, 16, 16); // detección de colisión
      collision9 = Collision(x_heart, 111, 16, 16, enemigo_9x, enemigo_9y, 16, 16); // detección de colisión

      if (collision6 || collision7 || collision8|| collision9){
        etapa = 2;
      }
      
      if (enemigo_6y < 210){
        delay(4);
        LCD_Bitmap(enemigo_6x, enemigo_6y, 16, 16, projectile);
        LCD_Bitmap(enemigo_7x, enemigo_7y, 16, 16, projectile);
        enemigo_6x += 1;
        enemigo_6y += 1;
        enemigo_7x -= 1;
        enemigo_7y += 1;
        if (enemigo_6y == 120){
          V_line(enemigo_6x - 1, enemigo_6y - 1, 16, 0x0000);
          H_line(enemigo_6x - 1, enemigo_6y - 1, 16, 0x0000);
          FillRect(enemigo_6x - 18, 118, 40, 2, 0xA091);
          V_line(enemigo_7x + 16, enemigo_7y - 1, 16, 0x0000);
          V_line(enemigo_7x + 17, enemigo_7y - 1, 16, 0x0000);
          FillRect(enemigo_7x, 118, 42, 2, 0xA091);
        }
        else {
          V_line(enemigo_6x - 1, enemigo_6y - 1, 16, 0x0000);
          H_line(enemigo_6x - 1, enemigo_6y - 1, 16, 0x0000);
          V_line(enemigo_7x + 16, enemigo_7y - 1, 16, 0x0000);
          V_line(enemigo_7x + 17, enemigo_7y - 1, 16, 0x0000);
          H_line(enemigo_7x, enemigo_7y - 1, 16, 0x0000);
        } 
      }

      else {
        FillRect(enemigo_6x, enemigo_6y, 16, 16, 0x0000);
        FillRect(enemigo_7x, enemigo_7y, 16, 16, 0x0000);
        enemigo_6x = 90;
        enemigo_6y = 20;
        enemigo_7x = 140;
        enemigo_7y = 220;
        retardo = 0;
        detachInterrupt(PE_3);
        detachInterrupt(PA_7);
        FillRect(0, 0, 300, 120, 0x0000);
        attack();
        x_heart = 152;
        set = 7;      
        check_over();
      }
    }

    else if (set == 7){
      if (background == 2){
        noInterrupts();
        lcdClear(0x0000);
        FillRect(18, 28, 282, 200, 0xFFFF);
        FillRect(20, 30, 278, 196, 0xF1FA);
        FillRect(20, 30, 278, 196, 0xB914);
        FillRect(20, 30, 278, 196, 0x80AD);
        FillRect(20, 30, 278, 196, 0x5869);
        FillRect(20, 30, 278, 196, 0x0000);
        FillRect(40, 118, 240, 2,  0xA091);
        background = 1;
      }

      interrupts();
      collision6 = Collision(x_heart, 111, 16, 16, enemigo_6x, enemigo_6y, 16, 16); // detección de colisión
      collision7 = Collision(x_heart, 111, 16, 16, enemigo_7x, enemigo_7y, 16, 16); // detección de colisión
      collision8 = Collision(x_heart, 111, 16, 16, enemigo_8x, enemigo_8y, 16, 16); // detección de colisión
      collision9 = Collision(x_heart, 111, 16, 16, enemigo_9x, enemigo_9y, 16, 16); // detección de colisión

      if (collision6 || collision7 || collision8|| collision9){
        etapa = 2;
      }

      if (enemigo_6y < 210){
        delay(2);
        LCD_Bitmap(enemigo_6x, enemigo_6y, 16, 16, projectile);
        LCD_Bitmap(enemigo_7x, enemigo_7y, 16, 16, projectile);
        LCD_Bitmap(enemigo_8x, enemigo_8y, 16, 16, projectile);  
        enemigo_6y += 1;
        enemigo_7y -= 1;
        enemigo_8y += 1;
        
        if (enemigo_6y == 120){
          //H_line(enemigo_6x, enemigo_6y - 1, 16, 0x0000);
          FillRect(enemigo_6x, 118, 16, 2, 0xA091);
          //H_line(enemigo_8x, enemigo_8y - 1, 16, 0x0000);
          FillRect(enemigo_8x, 118, 16, 2, 0xA091);
          H_line(enemigo_7x, enemigo_7y + 16, 16, 0x0000);
        }
        
        else {
          H_line(enemigo_6x, enemigo_6y - 1, 16, 0x0000);
          H_line(enemigo_7x, enemigo_7y + 16, 16, 0x0000);
          H_line(enemigo_8x, enemigo_8y - 1, 16, 0x0000);
        } 

        if (enemigo_7y == 102){
          //H_line(enemigo_7x, enemigo_7y + 15, 16, 0x0000);
          FillRect(enemigo_7x, 118, 16, 2, 0xA091);
        }
      }

      else {
        detachInterrupt(PE_3);
        detachInterrupt(PA_7);
        FillRect(enemigo_6x, enemigo_6y, 16, 16, 0x0000);
        FillRect(enemigo_7x, enemigo_7y, 16, 16, 0x0000);
        FillRect(enemigo_8x, enemigo_8y, 16, 16, 0x0000);
        enemigo_6x = 50;
        enemigo_6y = 20;
        enemigo_7x = 140;
        enemigo_7y = 20;
        enemigo_8x = 180;
        enemigo_8y = 20;
        enemigo_9x = 220;
        enemigo_9y = 20;
        enemigo_10x = 260;
        enemigo_10y = 20;
        x_heart = 152;
        FillRect(0, 0, 300, 120, 0x0000);
        attack();
        set = 8; 
        check_over();
      } 
    }

    else if (set == 8){
      if (background == 2){
        noInterrupts();
        lcdClear(0x0000);
        FillRect(18, 28, 282, 200, 0xFFFF);
        FillRect(20, 30, 278, 196, 0xF1FA);
        FillRect(20, 30, 278, 196, 0xB914);
        FillRect(20, 30, 278, 196, 0x80AD);
        FillRect(20, 30, 278, 196, 0x5869);
        FillRect(20, 30, 278, 196, 0x0000);
        FillRect(40, 118, 240, 2,  0xA091);
        background = 1;
      }

      interrupts();
      collision6 = Collision(x_heart, 111, 16, 16, enemigo_6x, enemigo_6y, 16, 16); // detección de colisión
      collision7 = Collision(x_heart, 111, 16, 16, enemigo_7x, enemigo_7y, 16, 16); // detección de colisión
      collision8 = Collision(x_heart, 111, 16, 16, enemigo_8x, enemigo_8y, 16, 16); // detección de colisión
      collision9 = Collision(x_heart, 111, 16, 16, enemigo_9x, enemigo_9y, 16, 16); // detección de colisión
      collision10 = Collision(x_heart, 111, 16, 16, enemigo_10x, enemigo_10y, 16, 16); // detección de colisión

      if (collision6 || collision7 || collision8|| collision9 || collision10){
        etapa = 2;
      }

      if (enemigo_6y < 210){
        LCD_Bitmap(enemigo_6x, enemigo_6y, 16, 16, projectile);
        LCD_Bitmap(enemigo_7x, enemigo_7y, 16, 16, projectile);
        LCD_Bitmap(enemigo_8x, enemigo_8y, 16, 16, projectile);  
        LCD_Bitmap(enemigo_9x, enemigo_9y, 16, 16, projectile);
        LCD_Bitmap(enemigo_10x, enemigo_10y, 16, 16, projectile);
        enemigo_6y += 1;
        enemigo_7y += 1;
        enemigo_8y += 1;
        enemigo_9y += 1;
        enemigo_10y += 1;

      if (enemigo_6y == 120){
          FillRect(enemigo_6x, 118, 16, 2, 0xA091);
          FillRect(enemigo_7x, 118, 16, 2, 0xA091);
          FillRect(enemigo_8x, 118, 16, 2, 0xA091);
          FillRect(enemigo_9x, 118, 16, 2, 0xA091);
          FillRect(enemigo_10x, 118, 16, 2, 0xA091);
        }
        
        else {
          H_line(enemigo_6x, enemigo_6y - 1, 16, 0x0000);
          H_line(enemigo_7x, enemigo_7y -1, 16, 0x0000);
          H_line(enemigo_8x, enemigo_8y - 1, 16, 0x0000);
          H_line(enemigo_9x, enemigo_9y -1, 16, 0x0000);
          H_line(enemigo_10x, enemigo_10y - 1, 16, 0x0000);
        } 
      }

      else {
        detachInterrupt(PE_3);
        detachInterrupt(PA_7);
        FillRect(enemigo_6x, enemigo_6y, 16, 16, 0x0000);
        FillRect(enemigo_7x, enemigo_7y, 16, 16, 0x0000);
        FillRect(enemigo_8x, enemigo_8y, 16, 16, 0x0000);
        FillRect(enemigo_9x, enemigo_9y, 16, 16, 0x0000);
        FillRect(enemigo_10x, enemigo_10y, 16, 16, 0x0000);
        FillRect(0, 0, 300, 120, 0x0000);
        x_heart = 152;
        enemigo_6x = 40;
        enemigo_6y = 210;
        enemigo_7x = 90;
        enemigo_7y = 210;
        enemigo_8x = 140;
        enemigo_8y = 210;
        enemigo_9x = 190;
        enemigo_9y = 210;
        enemigo_10x = 240;
        enemigo_10y = 210;
        attack();
        set = 9; 
        check_over();
      }
    }

    else if (set == 9){
      if (background == 2){
        noInterrupts();
        lcdClear(0x0000);
        FillRect(18, 28, 282, 200, 0xFFFF);
        FillRect(20, 30, 278, 196, 0xF1FA);
        FillRect(20, 30, 278, 196, 0xB914);
        FillRect(20, 30, 278, 196, 0x80AD);
        FillRect(20, 30, 278, 196, 0x5869);
        FillRect(20, 30, 278, 196, 0x0000);
        FillRect(40, 118, 240, 2,  0xA091);
        background = 1;
      }

      interrupts();
      collision6 = Collision(x_heart, 111, 16, 16, enemigo_6x, enemigo_6y, 16, 16); // detección de colisión
      collision7 = Collision(x_heart, 111, 16, 16, enemigo_7x, enemigo_7y, 16, 16); // detección de colisión
      collision8 = Collision(x_heart, 111, 16, 16, enemigo_8x, enemigo_8y, 16, 16); // detección de colisión
      collision9 = Collision(x_heart, 111, 16, 16, enemigo_9x, enemigo_9y, 16, 16); // detección de colisión
      collision10 = Collision(x_heart, 111, 16, 16, enemigo_10x, enemigo_10y, 16, 16); // detección de colisión

      if (collision6 || collision7 || collision8|| collision9 || collision10){
        etapa = 2;
      }

      if (enemigo_6y > 20){
        LCD_Bitmap(enemigo_6x, enemigo_6y, 16, 16, projectile);
        LCD_Bitmap(enemigo_7x, enemigo_7y, 16, 16, projectile);
        LCD_Bitmap(enemigo_8x, enemigo_8y, 16, 16, projectile);  
        LCD_Bitmap(enemigo_9x, enemigo_9y, 16, 16, projectile);
        LCD_Bitmap(enemigo_10x, enemigo_10y, 16, 16, projectile);
        enemigo_6y -= 1;
        enemigo_7y -= 1;
        enemigo_8y -= 1;
        enemigo_9y -= 1;
        enemigo_10y -= 1;

      if (enemigo_6y == 102){
          //H_line(enemigo_6x, enemigo_6y + 15, 16, 0x0000);
          FillRect(enemigo_6x, 118, 16, 2, 0xA091);
          //H_line(enemigo_7x, enemigo_7y + 15, 16, 0x0000);
          FillRect(enemigo_7x, 118, 16, 2, 0xA091);
          //H_line(enemigo_8x, enemigo_8y + 15, 16, 0x0000);
          FillRect(enemigo_8x, 118, 16, 2, 0xA091);
          //H_line(enemigo_9x, enemigo_9y + 15, 16, 0x0000);
          FillRect(enemigo_9x, 118, 16, 2, 0xA091);
          FillRect(enemigo_10x, 118, 16, 2, 0xA091);
          //H_line(enemigo_10x, enemigo_10y + 15, 16, 0x0000);
        }
        
        else {
          H_line(enemigo_6x, enemigo_6y + 16, 16, 0x0000);
          H_line(enemigo_7x, enemigo_7y + 16, 16, 0x0000);
          H_line(enemigo_8x, enemigo_8y + 16, 16, 0x0000);
          H_line(enemigo_9x, enemigo_9y + 16, 16, 0x0000);
          H_line(enemigo_10x, enemigo_10y + 16, 16, 0x0000);
        } 
      }

      else {
        detachInterrupt(PE_3);
        detachInterrupt(PA_7);
        FillRect(enemigo_6x, enemigo_6y, 16, 2, 0x0000);
        FillRect(enemigo_7x, enemigo_7y, 16, 2, 0x0000);
        FillRect(enemigo_8x, enemigo_8y, 16, 2, 0x0000);
        FillRect(enemigo_9x, enemigo_9y, 16, 2, 0x0000);
        FillRect(enemigo_10x, enemigo_10y, 16, 2, 0x0000);
        FillRect(0, 0, 300, 120, 0x0000);
        x_heart = 152;
        y_heart = 148;
        enemigo_6x = 240;
        enemigo_6y = 183;
        enemigo_7x = 120;
        enemigo_7y = 20;
        enemigo_8x = 160;
        enemigo_8y = 20;
        enemigo_9x = 200;
        enemigo_9y = 20;
        enemigo_10x = 240;
        enemigo_10y = 20;
        attack();
        set = 10;
        check_over(); 
      }     
    }

    else if (set == 10){
      if (background == 2){
        noInterrupts();
        FillRect(18, 98, 282, 120, 0xFFFF);
        FillRect(20, 100, 278, 116, 0xF1FA);
        FillRect(20, 100, 278, 116, 0xB914);
        FillRect(20, 100, 278, 116, 0x80AD);
        FillRect(20, 100, 278, 116, 0x5869);
        FillRect(20, 100, 278, 116, 0x0000);
        FillRect(40, 120, 240, 2,  0xA091);
        FillRect(40, 155, 240, 2,  0xA091);
        FillRect(40, 190, 240, 2,  0xA091);
        background = 1;
      }
      interrupts();
      collision1 = Collision(x_heart, y_heart, 16, 16, enemigo_1x, enemigo_1y, 16, 16); // detección de colisión
      collision2 = Collision(x_heart, y_heart, 16, 16, enemigo_2x, enemigo_2y, 16, 16); // detección de colisión
      collision3 = Collision(x_heart, y_heart, 16, 16, enemigo_3x, enemigo_3y, 16, 16); // detección de colisión
      collision5 = Collision(x_heart, y_heart, 16, 16, enemigo_5x, enemigo_5y, 16, 16); // detección de colisión
      collision6 = Collision(x_heart, y_heart, 16, 16, enemigo_6x, enemigo_6y, 16, 16); // detección de colisión

    
      if (collision1 || collision2 || collision3 || collision5 || collision6){
        etapa = 2;
      }

      /*if (enemigo_6y > 103 && retardo == 0){
        LCD_Bitmap(enemigo_6x, enemigo_6y, 16, 16, projectile);
        enemigo_6x -= 1;
        enemigo_6y -= 1;
        V_line(enemigo_6x + 17, enemigo_6y + 1, 16, 0x0000);
        H_line(enemigo_6x + 1, enemigo_6y + 17, 16, 0x0000);
        if (enemigo_6y == 103 || enemigo_6y == 138 || enemigo_6y == 173){
          FillRect(enemigo_6x, 120, 42, 2,  0xA091);
          FillRect(enemigo_6x, 190, 42, 2,  0xA091);
          FillRect(enemigo_6x, 155, 42, 2,  0xA091);
        }
      }

      else if (retardo == 0 && enemigo_6y == 103){
        FillRect(enemigo_6x, enemigo_6y, 17, 17, 0x0000);
        enemigo_6x = 240;
        enemigo_6y = 30;
        retardo = 1;
      }*/

      if (enemigo_1x < 264 && retardo != 2){
        LCD_Bitmap(enemigo_1x, enemigo_1y, 16, 16, spider);
        LCD_Bitmap(enemigo_5x, enemigo_5y, 16, 16, spider);    
        enemigo_1x += 1;
        enemigo_5x += 1;
        V_line(enemigo_1x - 1 , 113, 16, 0x0000);
        V_line(enemigo_1x - 1, 120, 1, 0xA091);
        V_line(enemigo_5x - 1 , 148, 16, 0x0000);
        V_line(enemigo_5x - 1, 155, 1, 0xA091);          
      }

      else if (enemigo_1x >= 263 && retardo != 2){
        FillRect(enemigo_1x , 113, 16, 16, 0x0000);
        FillRect(enemigo_1x, 120, 16, 2, 0xA091);
        FillRect(enemigo_5x , 148, 16, 16, 0x0000);
        FillRect(enemigo_5x, 155, 16, 2, 0xA091);
        enemigo_1x = 40;
        enemigo_3x = 264;
        retardo = 2;
      }

      if (retardo == 2 && enemigo_2x > 39){
        LCD_Bitmap(enemigo_2x, enemigo_2y, 16, 16, spider);
        LCD_Bitmap(enemigo_3x, enemigo_3y, 16, 16, spider);
        enemigo_3x -= 1;
        enemigo_2x -= 1;
        V_line(enemigo_2x + 17 , 148, 16, 0x0000);
        V_line(enemigo_2x + 17, 155, 1, 0xA091);
        V_line(enemigo_3x + 17 , 183, 16, 0x0000);
        V_line(enemigo_3x + 17, 190, 1, 0xA091);
      }

      else if (retardo == 2 && enemigo_2x == 39){
        detachInterrupt(PE_3);
        detachInterrupt(PA_7);
        FillRect(enemigo_2x, 148, 17, 16, 0x0000);
        FillRect(enemigo_2x, 155, 17, 2, 0xA091);
        FillRect(enemigo_3x, 183, 17, 16, 0x0000);
        FillRect(enemigo_3x, 190, 17, 2, 0xA091);
        FillRect(0, 0, 300, 120, 0x0000);
        retardo =  0;
        enemigo_1x = 40;
        enemigo_2x = 40;
        enemigo_3x = 40;
        attack();
        set = 11;
        check_over();
      }
    }

    else if (set == 11){
      if (background == 2){
        noInterrupts();
        FillRect(18, 98, 282, 120, 0xFFFF);
        FillRect(20, 100, 278, 116, 0xF1FA);
        FillRect(20, 100, 278, 116, 0xB914);
        FillRect(20, 100, 278, 116, 0x80AD);
        FillRect(20, 100, 278, 116, 0x5869);
        FillRect(20, 100, 278, 116, 0x0000);
        FillRect(40, 120, 240, 2,  0xA091);
        FillRect(40, 155, 240, 2,  0xA091);
        FillRect(40, 190, 240, 2,  0xA091);
        background = 1;
      }
      interrupts();
      collision1 = Collision(x_heart, y_heart, 16, 16, enemigo_1x, enemigo_1y, 16, 16); // detección de colisión
      collision2 = Collision(x_heart, y_heart, 16, 16, enemigo_2x, enemigo_2y, 16, 16); // detección de colisión
      collision3 = Collision(x_heart, y_heart, 16, 16, enemigo_3x, enemigo_3y, 16, 16); // detección de colisión
    
      if (collision1 || collision2 || collision3){
        etapa = 2;
      }

      if (enemigo_1x < 264 == retardo == 0){
        LCD_Bitmap(enemigo_1x, enemigo_1y, 16, 16, spider);
        enemigo_1x += 1;
        V_line(enemigo_1x - 1 , 113, 16, 0x0000);
        V_line(enemigo_1x - 1, 120, 1, 0xA091);
      }

      else if (enemigo_1x == 264){
        FillRect(enemigo_1x, 113, 16, 16, 0x0000);
        FillRect(enemigo_1x, 120, 16, 2, 0xA091);
      }

      if (enemigo_1x >= 110 && enemigo_2x < 264){
        LCD_Bitmap(enemigo_2x, enemigo_2y, 16, 16, spider);
        enemigo_2x += 1;
        V_line(enemigo_2x - 1 , 148, 16, 0x0000);
        V_line(enemigo_2x - 1, 155, 1, 0xA091);
      }

      else if (enemigo_2x == 264){
        FillRect(enemigo_2x, 148, 16, 16, 0x0000);
        FillRect(enemigo_2x, 155, 16, 2, 0xA091);
      }

      if (enemigo_1x >= 200 && enemigo_3x < 264){
        LCD_Bitmap(enemigo_3x, enemigo_3y, 16, 16, spider);
        enemigo_3x += 1;
        V_line(enemigo_3x - 1 , 183, 16, 0x0000);
        V_line(enemigo_3x - 1, 190, 1, 0xA091);
      }

      else if (enemigo_3x == 264){
        FillRect(enemigo_3x, 183, 16, 16, 0x0000);
        FillRect(enemigo_3x, 190, 16, 2, 0xA091);
        enemigo_1x = 264;
        enemigo_2x = 264;
        enemigo_3x = 264;
        set = 12;
      }
    }

    else if (set == 12){
      collision1 = Collision(x_heart, y_heart, 16, 16, enemigo_1x, enemigo_1y, 16, 16); // detección de colisión
      collision2 = Collision(x_heart, y_heart, 16, 16, enemigo_2x, enemigo_2y, 16, 16); // detección de colisión
      collision3 = Collision(x_heart, y_heart, 16, 16, enemigo_3x, enemigo_3y, 16, 16); // detección de colisión
    
      if (collision1 || collision2 || collision3){
        etapa = 2;
      }

      if (enemigo_1x > 39 && retardo == 0){
        LCD_Bitmap(enemigo_1x, enemigo_1y, 16, 16, spider);
        enemigo_1x -= 1;
        V_line(enemigo_1x + 17, 113, 16, 0x0000);
        V_line(enemigo_1x + 17, 120, 1, 0xA091);
      }

      else if (enemigo_1x == 39){
        FillRect(enemigo_1x, 113, 17, 16, 0x0000);
        FillRect(enemigo_1x, 120, 17, 2, 0xA091);
      }

      if (enemigo_1x <= 200 && enemigo_2x > 39){
        LCD_Bitmap(enemigo_2x, enemigo_2y, 16, 16, spider);
        enemigo_2x -= 1;
        V_line(enemigo_2x + 17 , 148, 16, 0x0000);
        V_line(enemigo_2x + 17, 155, 1, 0xA091);
      }

      else if (enemigo_2x == 39){
        FillRect(enemigo_2x, 148, 17, 16, 0x0000);
        FillRect(enemigo_2x, 155, 17, 2, 0xA091);
      }

      if (enemigo_1x <= 140 && enemigo_3x > 39){
        LCD_Bitmap(enemigo_3x, enemigo_3y, 16, 16, spider);
        enemigo_3x -= 1;
        V_line(enemigo_3x + 17 , 183, 16, 0x0000);
        V_line(enemigo_3x + 17, 190, 1, 0xA091);
      }

      else if (enemigo_3x == 39){
        detachInterrupt(PE_3);
        detachInterrupt(PA_7);
        FillRect(enemigo_3x, 183, 17, 16, 0x0000);
        FillRect(enemigo_3x, 190, 17, 2, 0xA091);
        FillRect(0, 0, 300, 120, 0x0000);
        attack();
        set = 13;
        check_over();
      }
    }

    else if (set == 13){
      if (hp_muffet <= 0){
      etapa = 3;
      }

      else {
        overtime = 1;
      }
    }
  }

  while (etapa == 2){
    digitalWrite(PC_4, LOW);
    digitalWrite(PC_5, LOW);
    digitalWrite(PC_6, HIGH);
    noInterrupts();
    if (bandera_perdedor == 0){
      lcdClear(0x0000);
      myFile = SD.open("mufl.txt");
      mapeoSD();
      LCD_Print("YOU DIED", 100, 100, 2, 0xFFFF, 0x0000);
      bandera_perdedor = 1;
    }

    if (digitalRead(PF_4) == LOW){
      overtime = 0;
      clean = 0;
      background = 0;
      bandera_perdedor = 0;
      risa = 0;
      retardo = 0;
      set = 0;
      y_heart = 148;
      x_heart = 152;
      enemigo_1x = 40;
      enemigo_1y = 113;
      enemigo_2x = 264;
      enemigo_2y = 148;
      enemigo_3x = 40;
      enemigo_3y = 183;
      enemigo_4x = 40;
      enemigo_4y = 183;
      enemigo_5x = 40;
      enemigo_5y = 148;
      enemigo_6x = 30;
      enemigo_6y = 20;
      enemigo_7x = 220;
      enemigo_7y = 20;
      enemigo_8x = 210;
      enemigo_8y = 20;
      enemigo_9x = 40;
      enemigo_9y = 148;
      enemigo_10x = 40;
      enemigo_10y = 148;
      x_attack = 20;
      hp_muffet = 200;
      attack_flag = 0;
      stop_over = 0;
      etapa = 0;
    }
  }

  while (etapa == 3){
    digitalWrite(PC_4, HIGH);
    digitalWrite(PC_5, LOW);
    digitalWrite(PC_6, LOW);
    noInterrupts();
    if (bandera_perdedor == 0){
      lcdClear(0x0000);
      myFile = SD.open("win.txt");
      mapeoSD();
      LCD_Print("YOU ARE FILLED", 40, 150, 2, 0xFFFF, 0x0000);
      LCD_Print("WITH DETERMINATION", 20, 190, 2, 0xFFFF, 0x0000);
      bandera_perdedor = 1;
    }

    if (digitalRead(PF_4) == LOW){
      stop_over = 0;
      overtime = 0;
      background = 0;
      bandera_perdedor = 0;
      risa = 0;
      retardo = 0;
      set = 0;
      y_heart = 148;
      x_heart = 152;
      enemigo_1x = 40;
      enemigo_1y = 113;
      enemigo_2x = 264;
      enemigo_2y = 148;
      enemigo_3x = 40;
      enemigo_3y = 183;
      enemigo_4x = 40;
      enemigo_4y = 183;
      enemigo_5x = 40;
      enemigo_5y = 148;
      enemigo_6x = 30;
      enemigo_6y = 20;
      enemigo_7x = 220;
      enemigo_7y = 20;
      enemigo_8x = 210;
      enemigo_8y = 20;
      enemigo_9x = 40;
      enemigo_9y = 148;
      enemigo_10x = 40;
      enemigo_10y = 148;
      x_attack = 20;
      hp_muffet = 200;
      attack_flag = 0;
      clean = 0;
      etapa = 0;
    }
  }
}


void up(){
  noInterrupts(); //Que no se interrumpa mientras se ejecuta
  delay(10); //delay de 10ms
  bandera = 1; //Bandera de antirrebote
  if (y_heart != 113 && bandera == 1 && digitalRead(PE_3) == LOW){ //Si está presionado y la bandera en 1
    if (y_heart == 148 && bandera == 1 && digitalRead(PE_3) == LOW){ //Si está en la línea de en medio
      FillRect(152, 148, 16, 16, 0x0000); 
      FillRect(152, 155, 16, 2, 0xA091);
      y_heart = 113; //Ir a la línea de arriba
      bandera = 0; //Limpiar bandera
    }

    else if (y_heart == 183 && bandera == 1 && digitalRead(PE_3) == LOW){ //Si está en la línea de en medio de hasta abajo
      y_heart = 148; //Ir a la línea de en medio
      FillRect(152, 183, 16, 16, 0x0000);
      FillRect(152, 190, 16, 2, 0xA091);
      bandera = 0; //Limpiar bandera
    }
  }
}

void down(){
  noInterrupts(); //Que no se interrumpa mientras se ejecuta
  delay(10); //delay de 10ms
  bandera = 2; //Bandera de antirrebote
  if (y_heart != 183 && bandera == 2 && digitalRead(PA_7) == LOW){ //Si está presionado y la bandera en 1
    if (y_heart == 148 && bandera == 2 && digitalRead(PA_7) == LOW){ //Si está en la línea de en medio
      y_heart = 183; //Ir a la línea de abajo
      FillRect(152, 148, 16, 16, 0x0000);
      FillRect(152, 155, 16, 2, 0xA091);
      bandera = 0; //Limpiar bandera
    }

    else if (y_heart == 113 && bandera == 2 && digitalRead(PA_7) == LOW){ //Si está en la línea de en arriba
      y_heart = 148; //Ir a la línea de en medio
      FillRect(152, 113, 16, 16, 0x0000);
      FillRect(152, 120, 16, 2, 0xA091);
      bandera = 0; //Limpiar bandera
    }
  }
}

//Función de ataque
void hit(){
  delay(10); //delay de 10ms
  if (x_attack <= 60 && x_attack >= 20){ //Si se presionó el botón en el color rojo
        hp_muffet -= 5; //Reducir vide en 5 puntos
        if (hp_muffet < 0){ //Si es menor a 0 
          hp_muffet = 0; //Que sea 0
        }
        LCD_Print("-5", 200, 50, 2, 0xEA41, 0x0000);
        attack_flag = 0; //Salir del ataque
        x_attack = 20; //Regresar rectángulo a su posición
        background = 2; //Limpiar bandera del fondo
        detachInterrupt(PF_4); //Quitar las interrupcion del botón de ataque
        sprintf(buffer, "HP %03d/200", hp_muffet); //Volver cadena de carácteres la vida de muffet
        LCD_Print(buffer, 20, 20, 1, 0xFFFF, 0x0000); 
        FillRect(20, 40, 100, 10, 0x6800); 
        if (hp_muffet <= 0){ //Si la vida es 0
          hp_bar = 1;  //Que la barra se muestre en lo mínimo
        }
        else{
          hp_bar = map(hp_muffet, 0, 200, 0, 100); //Si no mapear los valores
        }
        FillRect(20, 40, hp_bar, 10, 0x16A3); //Mostrar barra de vida
        delay(500); //delay de 500ms
      }

      else if (x_attack <= 100 && x_attack >= 60){ //Si se presionó el botón en el color anaranjado
        hp_muffet -= 10; //Reducir vide en 10 puntos
        if (hp_muffet < 0){ //Si es menor a 0 
          hp_muffet = 0; //Que sea 0
        }
        LCD_Print("-10", 200, 50, 2, 0xEA41, 0x0000);
        attack_flag = 0; //Salir del ataque
        x_attack = 20; //Regresar rectángulo a su posición
        background = 2; //Limpiar bandera del fondo
        detachInterrupt(PF_4); //Quitar las interrupcion del botón de ataque
        sprintf(buffer, "HP %03d/200", hp_muffet); //Volver cadena de carácteres la vida de muffet
        LCD_Print(buffer, 20, 20, 1, 0xFFFF, 0x0000);
        FillRect(20, 40, 100, 10, 0x6800); 
        if (hp_muffet <= 0){ //Si la vida es 0
          hp_bar = 1; //Que la barra se muestre en lo mínimo 
        }
        else{
          hp_bar = map(hp_muffet, 0, 200, 0, 100); //Si no mapear los valores
        }
        FillRect(20, 40, hp_bar, 10, 0x16A3);   
        delay(500); //delay de 500ms 
      }

      else if (x_attack <= 140 && x_attack >= 100){ //Si se presionó el botón en el color amarillo
        hp_muffet -= 15; //Reducir vide en 15 puntos
        if (hp_muffet < 0){ //Si es menor a 0 
          hp_muffet = 0; //Que sea 0
        }
        LCD_Print("-15", 200, 50, 2, 0xEA41, 0x0000);
        attack_flag = 0; //Salir del ataque
        x_attack = 20; //Regresar rectángulo a su posición
        background = 2; //Limpiar bandera del fondo
        detachInterrupt(PF_4); //Quitar las interrupcion del botón de ataque
        sprintf(buffer, "HP %03d/200", hp_muffet); //Volver cadena de carácteres la vida de muffet
        LCD_Print(buffer, 20, 20, 1, 0xFFFF, 0x0000);
        FillRect(20, 40, 100, 10, 0x6800); 
        if (hp_muffet <= 0){ //Si la vida es 0
          hp_bar = 1; //Que la barra se muestre en lo mínimo   
        }
        else{
          hp_bar = map(hp_muffet, 0, 200, 0, 100); //Si no mapear los valores
        }
        FillRect(20, 40, hp_bar, 10, 0x16A3); //Si no mapear los valores    
        delay(500); //delay de 500ms
      }

      else if (x_attack <= 180 && x_attack >= 140){ //Si se presionó el botón en el color verde
        hp_muffet -= 20; //Reducir vide en 20 puntos
        if (hp_muffet < 0){ //Si es menor a 0 
          hp_muffet = 0; //Que sea 0
        }
        LCD_Print("-20", 200, 50, 2, 0xEA41, 0x0000);
        attack_flag = 0; //Salir del ataque
        x_attack = 20; //Regresar rectángulo a su posición
        background = 2; //Limpiar bandera del fondo
        detachInterrupt(PF_4); //Quitar las interrupcion del botón de ataque
        sprintf(buffer, "HP %03d/200", hp_muffet); //Volver cadena de carácteres la vida de muffet
        LCD_Print(buffer, 20, 20, 1, 0xFFFF, 0x0000);
        FillRect(20, 40, 100, 10, 0x6800); 
        if (hp_muffet <= 0){ //Si la vida es 0
          hp_bar = 1; //Que la barra se muestre en lo mínimo 
        }
        else{
          hp_bar = map(hp_muffet, 0, 200, 0, 100); //Si no mapear los valores
        }
        FillRect(20, 40, hp_bar, 10, 0x16A3);   
        delay(500); //delay de 500ms
      }

      else if (x_attack <= 220 && x_attack >= 180){ //Si se presionó el botón en el color anaranjado
        hp_muffet -= 15; //Reducir vide en 15 puntos
        if (hp_muffet < 0){ //Si es menor a 0 
          hp_muffet = 0; //Que sea 0
        }
        LCD_Print("-15", 200, 50, 2, 0xEA41, 0x0000); 
        attack_flag = 0; //Salir del ataque
        x_attack = 20; //Regresar rectángulo a su posición
        background = 2; //Limpiar bandera del fondo
        detachInterrupt(PF_4); //Quitar las interrupcion del botón de ataque
        sprintf(buffer, "HP %03d/200", hp_muffet); //Volver cadena de carácteres la vida de muffet
        LCD_Print(buffer, 20, 20, 1, 0xFFFF, 0x0000);
        FillRect(20, 40, 100, 10, 0x6800); 
        if (hp_muffet <= 0){ //Si la vida es 0
          hp_bar = 1; //Que la barra se muestre en lo mínimo
        }
        else{
          hp_bar = map(hp_muffet, 0, 200, 0, 100);  //Si no mapear los valores
        }
        FillRect(20, 40, hp_bar, 10, 0x16A3);   
        delay(500); //delay de 500ms
      }

      else if (x_attack <= 260 && x_attack >= 220){ //Si se presionó el botón en el color anaranjado
        hp_muffet -= 10; //Reducir vide en 10 puntos
        if (hp_muffet < 0){ //Si es menor a 0 
          hp_muffet = 0; //Que sea 0
        }
        LCD_Print("-10", 200, 50, 2, 0xEA41, 0x6800);
        attack_flag = 0; //Salir del ataque
        x_attack = 20; //Regresar rectángulo a su posición
        background = 2; //Limpiar bandera del fondo
        detachInterrupt(PF_4); //Quitar las interrupcion del botón de ataque
        sprintf(buffer, "HP %03d/200", hp_muffet); //Volver cadena de carácteres la vida de muffet
        LCD_Print(buffer, 20, 20, 1, 0xFFFF, 0x0000);
        FillRect(20, 40, 100, 10, 0x6800);
        if (hp_muffet <= 0){ //Si la vida es 0
          hp_bar = 1; //Que la barra se muestre en lo mínimo 
        }
        else{
          hp_bar = map(hp_muffet, 0, 200, 0, 100); //Si no mapear los valores
        }
        FillRect(20, 40, hp_bar, 10, 0x16A3);   
        delay(500); //delay de 500ms
      }

      else if (x_attack <= 298 && x_attack >= 260){ //Si se presionó el botón en el color rojo
        hp_muffet -= 5; //Reducir vide en 15 puntos
        if (hp_muffet < 0){ //Si es menor a 0 
          hp_muffet = 0; //Que sea 0
        }
        LCD_Print("-5", 200, 50, 2, 0xEA41, 0x0000);
        attack_flag = 0;  //Salir del ataque
        x_attack = 20; //Regresar rectángulo a su posición
        background = 2; //Limpiar bandera del fondo
        detachInterrupt(PF_4); //Quitar las interrupcion del botón de ataque
        sprintf(buffer, "HP %03d/200", hp_muffet); //Volver cadena de carácteres la vida de muffet
        LCD_Print(buffer, 20, 20, 1, 0xFFFF, 0x0000);
        FillRect(20, 40, 100, 10, 0x6800); 
        if (hp_muffet <= 0){ //Si la vida es 0
          hp_bar = 1; //Que la barra se muestre en lo mínimo
        }
        else{
          hp_bar = map(hp_muffet, 0, 200, 0, 100); //Si no mapear los valores
        }
        FillRect(20, 40, hp_bar, 10, 0x16A3);   
        delay(500); //delay de 500ms
      }
}

//Función de colisión
bool Collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
  return (x1 < x2 + w2) && (x1+ w1 > x2) && (y1 < y2 + h2) && (y1 + h1 > y2); //Chequear las coordenas, anchos y altos
}

void attack(){
  detachInterrupt(PE_3); //Se desactivan las interrupciones de los botones de movimiento
  detachInterrupt(PA_7); //Se desactivan las interrupciones de los botones de movimiento
  //Si muffet no está en la pantalla mostrarla
  if (set < 5 || set > 9){
    
  }
  else {
    LCD_Sprite_Zoom(134, 25, 26, 35, muffet, 5, 5, 0, 2, 0x0000); //Mostrar a muffet en la pantalla
  }
  //Relleanar el fondon para la preparación del ataque
  FillRect(0, 98, 320, 142, 0x0000);
  FillRect(58, 98, 204, 80, 0xFFFF); 
  FillRect(60, 100, 200, 76, 0x0000);
  FillRect(128, 188, 64, 25, 0xDF61); 
  FillRect(130, 190, 60, 21, 0x0000);
  LCD_Print("ATTACK", 135, 195, 1, 0xDF61, 0x0000);
  if (set == 0){
    LCD_Print("I THINK PURPLE", 80, 110, 1, 0xFFFF, 0x0000);
    LCD_Print("LOOKS GOOD ON YOU", 80, 140, 1, 0xFFFF, 0x0000);
  }
  //Pintar fondo de los rectángulos de colores para el turno de ataque
  while (attack_flag == 0){ //Revisar bandera del fondo
    if (digitalRead(PF_4) == LOW){ //Pollear el botón
      //Pintar fondo
      FillRect(18, 98, 282, 120, 0xFFFF);
      FillRect(20, 100, 278, 116, 0x0000);
      FillRect(20, 100, 40, 116, 0xE841);
      FillRect(60, 100, 40, 116, 0xEA41);
      FillRect(100, 100, 40, 116, 0xDF61);
      FillRect(140, 100, 40, 116, 0x1705);
      FillRect(180, 100, 40, 116, 0xDF61);
      FillRect(220, 100, 40, 116, 0xEA41);
      FillRect(260, 100, 38, 116, 0xE841);
      attack_flag = 1; //Encnder bandera para que no se esté pintando el fondo
    }
  }

  //Si ya se pintó el fondo quedarse en un loop
  while (attack_flag == 1){ 
    if (x_attack < 294){ //Que avance el rectángulo blanco de ataque mientras no haya llegado al límite
      attachInterrupt(digitalPinToInterrupt(PF_4), hit, FALLING); //Activar interrupción del botón de ataque
      FillRect(x_attack, 105, 5, 100, 0xFFFF); //Pintar rectángulo
      x_attack = x_attack + 1; //Ir avanzando un espacio
      //Dependiendo del color de fondo en que se encuentre se limpiará el rastro con el respectivo color
      if (x_attack <= 60 && x_attack >= 25){
        FillRect(x_attack - 5, 105, 5, 100, 0xE841); //Rojo    
      }

      else if (x_attack <= 100 && x_attack >= 65){
        FillRect(x_attack - 5, 105, 5, 100, 0xEA41); //Anaranjado
      }

      else if (x_attack <= 140 && x_attack >= 105){
        FillRect(x_attack - 5, 105, 5, 100, 0xDF61); //Amarillo
      }

      else if (x_attack <= 180 && x_attack >= 145){
        FillRect(x_attack - 5, 105, 5, 100, 0x1705); //Verde
      }

      else if (x_attack <= 220 && x_attack >= 185){
        FillRect(x_attack - 5, 105, 5, 100, 0xDF61); //Amarillo
      }

      else if (x_attack <= 260 && x_attack >= 225){
        FillRect(x_attack - 5, 105, 5, 100, 0xEA41); //Anaranjado
      }

      else if (x_attack <= 298 && x_attack >= 265){
        FillRect(x_attack - 5, 105, 5, 100, 0xE841); //Rojo
      }
      
    }

     //Si el jugador no presiono, lo detecta como si solo haya presionado en el color rojo
     else {
      FillRect(x_attack, 105, 5, 100, 0xE841);
      hp_muffet = hp_muffet - 5; //Restar 5 de HP
      if (hp_muffet < 0){ //Si es menor a 0
          hp_muffet = 0; //Igualar a 0
        }
      LCD_Print("-5", 200, 50, 2, 0xEA41, 0x0000);
      attack_flag = 0; //Terminar ataque
      detachInterrupt(PF_4); //Desactivar interrupción de ataque
      x_attack = 20; //Regresar rectángulo de ataque a su posición
      background = 2; //Reinciar variable del fondo para el juego
      sprintf(buffer, "HP %03d/200", hp_muffet); //Convertir en string la vida para mostrar en la pantalla 
      LCD_Print(buffer, 20, 20, 1, 0xFFFF, 0x0000); //Mostrar en la pantalla
      FillRect(20, 40, 100, 10, 0x6800); 
      if (hp_muffet == 0){ //Si la vida es 0
          hp_bar = 1; //Mostrar un poquita de la barra
        }
        else {
          hp_bar = map(hp_muffet, 0, 200, 0, 100); //Mapear valores para la barra de vida
        }
      FillRect(20, 40, hp_bar, 10, 0x16A3); //Mostrar barra de vida   
      delay(500); //delay de 50ms
     }    
  }
}

void right(){
  noInterrupts(); //Desactivar interrupciones 
  delay(10); //delay de 10ms
  bandera = 1; //Bandera para antirrebote
  if (x_heart < 260 && bandera == 1 && digitalRead(PE_3) == LOW){ //Si está en presionado y la bandera en 1
    x_heart += 12; //Aumentar en 12 la posición
    FillRect(x_heart - 12, 111, 12, 16, 0x0000); //Limpiar rastro
    FillRect(x_heart - 12, 118, 12, 2, 0xA091);
    bandera = 0; //Limpiar bandera
  }
}

void left(){
  noInterrupts(); //Desactivar interrupciones
  delay(10); //delay de 10ms
  bandera = 1; //Bandera para antirrebotes
  if (x_heart > 60 && bandera == 1 && digitalRead(PA_7) == LOW){ //Si está en presionado y la bandera en 1
    x_heart -= 12; //Ir para la izquierda
    FillRect(x_heart + 16, 111, 12, 16, 0x0000); //Limpiar rastro
    FillRect(x_heart + 16, 118, 12, 2, 0xA091);
    bandera = 0; //Limpiar bandera
  }
}

int asciitohex(int a){
  switch (a){
    case 48: //Si se lee un '0' 
      return 0x00; //Devolver un 0
    case 49: //Si se lee un '1'
      return 0x01; //Devolver un 1
    case 50: //Si se lee un '2'
      return 0x02; //Devolver un 2
    case 51: //Si se lee un '3'
      return 0x03; //Devolver un 3
    case 52: //Si se lee un '4'
      return 0x04; //Devolver un 4
    case 53: //Si se lee un '5'
      return 0x05; //Devolver un 5
    case 54: //Si se lee un '6'
      return 0x06; //Devolver un 6
    case 55: //Si se lee un '7'
      return 0x07; //Devolver un 7
    case 56: //Si se lee un '8' 
      return 0x08; //Devolver un 8
    case 57: //Si se lee un '9' 
      return 0x09; //Devolver un 9
    case 97: //Si se lee un 'a'
      return 0x0A; //Devolver un 10
    case 98: //Si se lee un 'b'
      return 0x0B; //Devolver un 11
    case 99: //Si se lee un 'c' 
      return 0x0C; //Devolver un 12
    case 100: //Si se lee un 'd'
      return 0x0D; //Devolver un 13
    case 101: //Si se lee un 'e'
      return 0x0E; //Devolver un 14
    case 102: //Si se lee un 'f'
      return 0x0F; //Devolver un 15
  }
}

//Función para leer de la SD y mostrar en la pantalla
void mapeoSD(){
  int hex1 = 0; //Variable para agarrar lo leído de la SD
  int val1 = 0; //Variable para guardar decena del valor hexadecimal
  int val2 = 0; //Variable para guardar la unidad hexadecimal
  int mapear = 0; //Iniciar con la posición del arreglo en 0
  int vertical = 0; //Iniciar con la coordenada en y en 0
  unsigned char maps[640]; //Definir un arreglo de 640 espacios

  if (myFile){ //Si se encontró un archivo con el nombre definido previamente ejecutar
    while (myFile.available()){ //Siempre que haya contenido por leer en la SD ejecutar
      mapear = 0; //Iniciar la posición de mapear en 0
      while (mapear < 640){ //Mientras se llegue 
        hex1 = myFile.read(); //Leer del archivo de texto
        if (hex1 == 120){ //Si se leyó una "x", significa que vienen dos valores en hexadecimal que si hay que guardar
          val1 = myFile.read(); //Leer y almacenar en una variable
          val2 = myFile.read(); //Leer y almacenar en una variable
          val1 = asciitohex(val1); //Convertir el valor de ASCII a hexadecimal
          val2 = asciitohex(val2); //Convertir el valor de ASCII a hexadecimal
          maps[mapear] = val1*16 + val2; //Guardar el valor de mapeo en el arreglo
          mapear++; //Aumentar la posición del arreglo
        }
      }
      
      LCD_Bitmap(0, vertical, 320, 1, maps); //Mostrar una fila de contenido de la imagen
      vertical++; //Aumentar la línea vertical
    }
    
    myFile.close(); //Cerrar el archivo si ya hay contenido por leer
  }
  else {
    myFile.close(); //Si no se encontró el arhivo cerrar para evitar algún error
    Serial.println("NO SE ABRIO");
  }
}

void check_over(){
  //Si el jugador ya pasó los 10 sets revisar, revisar la vida
  if (overtime == 1){
    set = 13; //set para revisión de la vida
    stop_over = 0; //set por sí aún no termina el juego, pero ya está en overtime
    if (hp_muffet <= 0){ //Si la vida es igual o menor a 0
      stop_over = 4; //No generar más sets
      lcdClear(0xFFFF); //Pintar de blanco
      etapa = 3; //Ir a etapa de ganador
    } 
  }
}
