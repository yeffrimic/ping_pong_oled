/*
 * This is a game where you and a friend or someone else can control
 * the bars, the first that go to 10 points is the winner.
 * the speed of the ball will be increasing while playing, so you have
 * to pay attention
 *
 */



#include "U8glib.h" //library that controls the Oled dislpay
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

/*--------customize this constant to: --------//
 * Change the pin buttons, size of the ball
 * height and width of the oled display
 * height of the bars and width
 */

/*-------- --------//
 *
 */
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

/*--------don't change this variables --------//
 * this variables are using in the game to calculate distance, speed
 * of the bars, ball, points of each player and time interval to
 * start again
 */
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
int interval = 20000;


/*--------Void draw --------//
 * this void is the one that show all the shapes of the game
 * and show the winner when it has one
 */
void draw(void) {
  if (!winner) {
    u8g.drawLine(widthScreen * 0.5, 0, widthScreen * 0.5, heightScreen);
    u8g.drawEllipse(x, y, sizeXellipse, sizeYellipse);
    u8g.drawBox(player1X, player1Y , widthbars, heightbars);
    u8g.drawBox(player2X, player2Y, widthbars, heightbars);

    u8g.setFont(u8g_font_osb18);
    u8g.setPrintPos(int(widthScreen * 0.25), 18);
    u8g.print(player1Points);
    u8g.setPrintPos(int(widthScreen * 0.75), 18);
    u8g.print(player2Points);
  } else {

    u8g.setPrintPos(int(widthScreen * 0.15), heightScreen / 2 - 15);
    u8g.print("Ganador");
    if (player2Points > player1Points) {

      u8g.setPrintPos(10, heightScreen / 2 + 20);
      u8g.print("Jugador2");
    } else {

      u8g.setPrintPos(10, heightScreen / 2 + 20);
      u8g.print("Jugador1");
    }

  }

}

/*-------- void setup--------//
 * to configure the pin buttons.
 */

void setup() {
  pinMode(j1Up, INPUT);
  pinMode(j2Up, INPUT);
  pinMode(j1down, INPUT);
  pinMode(j2down, INPUT);
  // //Serial.begin(9600); //if you want to debug uncomment the //Serial lines
}

void loop() {
  // read the buttons and increment or decrement the y position of
  // each player
  if (digitalRead(j1Up) == HIGH) {
    player1Y += 2;
  }
  if (digitalRead(j1down) == HIGH) {
    player1Y -= 2;
  }
  if (digitalRead(j2Up) == HIGH) {
    player2Y += 2;
  }
  if (digitalRead(j2down) == HIGH) {
    player2Y -= 2;
  }
//-------- end --------//
  // routine to show the shapes
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );
  
//-------- end --------//

//-------- begin the comparatives and all the logic of the game --------//
  if (x >= widthScreen - widthbars - sizeXellipse / 2 && y >= player2Y - sizeYellipse / 2 && y <= player2Y + heightbars + sizeYellipse / 2) {
    dirX *= -1;
    velEllipseX++;
  }

  if (!winner) {
    if ( x <= 0 + sizeXellipse + widthbars && y > player1Y - sizeYellipse / 2 && y <= player1Y + heightbars + sizeYellipse / 2) {
      dirX *= -1;
      velEllipseX++;
    }
    if (x > widthScreen + sizeXellipse + 10) {
      velEllipseY = 2;
      dirX *= -1;
      x = widthScreen / 2;
      y = random(0, heightScreen);
      player1Points++;
    }
    if ( x < 0 - sizeXellipse - 10 ) {
      velEllipseY = 2;
      dirX *= -1;
      x = widthScreen / 2;
      y = random(0, heightScreen);
      player2Points++;
    }
    //Serial.println(x);
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
    lastpub = millis();
    if (player1Points >= 10 || player2Points >= 10) {
      winner = true;
    }
  }

//-------- end --------//

//--------delay to show the winner  --------//
  if (millis() - lastpub > interval) {
    winner = false;
    player1Points = 0;
    player2Points = 0;
    velEllipseY = 2;
    velEllipseX = 2;
    dirX *= -1;
    x = widthScreen / 2;
    y = random(0, heightScreen);
  }
  
//-------- end --------//
}
