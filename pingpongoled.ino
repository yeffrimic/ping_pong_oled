
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC


#define sizeXellipse 5
#define sizeYellipse 5
#define widthbars 3
#define heightbars 25
#define widthScreen 128
#define heightScreen 64
#define j1Up 23
#define j2Up 27
#define j1down 25
#define j2down 29

int player1X = 0;
int player1Y = 0;
int player2X = widthScreen - widthbars;
int player2Y = 30;
int velEllipseX = 5;
int velEllipseY = 1;
float velFinalX;
int    x = widthScreen / 2;
int    y = heightScreen / 2;
int dirX = 1;
int dirY = 1;
int player1Points = 0;
int player2Points = 9;
boolean winner = false;
long lastpub;
int interval=20000;
void draw(void) {
  if(!winner){
  u8g.drawLine(widthScreen * 0.5, 0, widthScreen * 0.5, heightScreen);
  u8g.drawEllipse(x, y, sizeXellipse, sizeYellipse);
  u8g.drawBox(player1X, player1Y , widthbars, heightbars);
  u8g.drawBox(player2X, player2Y, widthbars, heightbars);

  u8g.setFont(u8g_font_osb18);
  u8g.setPrintPos(int(widthScreen * 0.25), 18);
  u8g.print(player1Points);
  u8g.setPrintPos(int(widthScreen * 0.75), 18);
  u8g.print(player2Points);
}else{
  
  u8g.setPrintPos(int(widthScreen * 0.15), heightScreen/2-15);
  u8g.print("Ganador");
  if(player2Points > player1Points){
    
  u8g.setPrintPos(10, heightScreen/2+20);
  u8g.print("Jugador2");
  }else{
    
  u8g.setPrintPos(10, heightScreen/2+20);
  u8g.print("Jugador1");
  }
  
}

}

void setup() {
  // put your setup code here, to run once:
  pinMode(j1Up,INPUT);
  pinMode(j2Up,INPUT);
  pinMode(j1down,INPUT);
  pinMode(j2down,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
if(digitalRead(j1Up)==HIGH){
  player1Y++;
}
if(digitalRead(j1down)==HIGH){
  player1Y--;
}
if(digitalRead(j2Up)==HIGH){
  player2Y++;
}
if(digitalRead(j2down)==HIGH){
  player2Y--;
}
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );
  if (x >= widthScreen - widthbars - sizeXellipse / 2 && y >= player2Y - sizeYellipse / 2 && y <= player2Y + heightbars + sizeYellipse / 2) {
   dirX *= -1;
    velEllipseX++;
     }

if(!winner){
  if ( x <= 0 + sizeXellipse + widthbars && y > player1Y - sizeYellipse / 2 && y<= player1Y+heightbars + sizeYellipse / 2) {
    dirX *= -1;
    velEllipseX++;
  }  
  if (x > widthScreen + sizeXellipse + 10){
    velEllipseY = 2;
    dirX *= -1;
    x = widthScreen / 2;
    y = random(0,heightScreen);
    player1Points++;
  }
    if( x < 0 - sizeXellipse - 10 ) {
    velEllipseY = 2;
    dirX *= -1;
    x = widthScreen / 2;
    y = random(0,heightScreen);
    player2Points++;
  }
  Serial.println(x);
  if (y + sizeYellipse > heightScreen  || y < sizeYellipse) {
    dirY *= -1;
    velEllipseY++;
  }
  if (velEllipseX > 20) {
    velEllipseX = 20;
  }  if (velEllipseY > 12) {
    velEllipseY = 12;

  }
  x +=  velEllipseX * dirX;
   y +=  velEllipseY * dirY;
   lastpub=millis();
   if(player1Points>=10 || player2Points>=10){
    winner = true;
   }
}

if(millis()-lastpub>interval){
  winner=false;
  player1Points=0;
  player2Points=0;
    velEllipseY = 2;
    velEllipseX = 2;
    dirX *= -1;
    x = widthScreen / 2;
    y = random(0,heightScreen);
}
}
