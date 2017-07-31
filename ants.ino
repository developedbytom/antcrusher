// Tom Bedford (oldmantom)
// July 26th 2017
// Ants

#include <Arduboy2.h>
Arduboy2 arduboy;

// gamestates
int gamestate = 0;
int gametype = 0;

// resolution
int screenWidth = 128;
int screenHeight = 64;

// button press buffers
int bufferA = 0;
int bufferB = 0;
int bufferUp = 0;
int bufferDown = 0;
int bufferLeft = 0;
int bufferRight = 0;

// spawner
int spawnX = screenWidth;
int spawnY = screenHeight;

// ants
bool antsSpawned = false;
int antSize = 2;
class Ant {
  public:
    int antX;
    int antY;
    bool alive = true;
};
int currentAnt;
int numberOfAnts = 10;
Ant Ants[10]; // prepare array of ants

// scoring
int playerScore;
int computerScore;

// player
int footX = 50;
int footY = -20;
int footWidth = 10;
int footHeight = screenHeight;
bool footRaised = true;
bool footLowering = false;
bool footRaising = false;

void setup() {

  // start arduboy
  arduboy.begin();

  // set the frame rate to 60 FPS
  arduboy.setFrameRate(60);
  
  // clear the screen
  arduboy.clear();

}

void loop() {

  // prevent the game from running too fast (i.e. if not ready to display next frame, wait)
  if (!arduboy.nextFrame()) {
    return;
  }

  // clear the display
  arduboy.clear();

  // game code here
  switch(gamestate) {
    case 0:
      // title screen
      title();
      break;
    case 1:
      // game screen
      gameplay();
      break;
    case 2:
      // win screen
      win();
      break;
    case 3:
      // lose screen
      lose();
      break;
  }

  // reset variables for button presses
  resetbuttonbuffers();

  // update the display
  arduboy.display();

}

void title () {
  arduboy.setCursor(0, 0);
  arduboy.print("ANT CRUSHER");
  arduboy.setCursor(0, 16);
  arduboy.print("Crush 10 ants...");
  arduboy.setCursor(0, 26);
  arduboy.print("...before 20 escape!");
  arduboy.setCursor(0, 42);
  arduboy.print("> Press A to play");
  if(arduboy.pressed(A_BUTTON) and bufferA == 0) {
    bufferA = 1;
    gamestate = 1;
  }
}

void gameplay() {
  
  printScore();

  checkScore();

  footUpDown();

  // draw player foot
  arduboy.fillRect(footX, footY, footWidth, footHeight, WHITE);

  // set Ant spawn coordinates
  if (antsSpawned == false) {
    for ( currentAnt = 0; currentAnt < numberOfAnts; currentAnt++ ) {
      Ants[currentAnt].antX = spawnX + (currentAnt*10); // spawn ants 10px apart
      Ants[currentAnt].antY = spawnY;
    }
    antsSpawned = true; // stop them spawning again
  }
  
  // for each Ant in Ants
  for (currentAnt = 0; currentAnt < numberOfAnts; currentAnt++) {
    // if the ants are alive
    if (Ants[currentAnt].alive == true) {
      moveAnts(); // move to the right
      escapedAnts(); // score if ants escape
      crushedAnts(); // score if ants are crushed
      // draw ants
      arduboy.fillRect(Ants[currentAnt].antX - antSize, Ants[currentAnt].antY - antSize, antSize, antSize, WHITE);
    }
  }
  
}

void checkScore() {
  if (playerScore >= numberOfAnts) {
    gamestate = 2; // win
  }
  if (computerScore >= 20) {
    gamestate = 3; // lose
  }
}

void printScore() {
  // computer score
  arduboy.setCursor(0,0);
  arduboy.print(computerScore);
  arduboy.print(" Lost");
  // player score
  arduboy.setCursor(68,0);
  arduboy.print(playerScore);
  arduboy.print(" Crushed");
}

void footUpDown() {
//  // foot down when A pressed
//  if(arduboy.pressed(A_BUTTON)) {
//    // if the foot is not at the bottom of the screen...
//    if (footY < 0) {
//      // if not 1px away
//      if (footY != -1) {
//        // move foot down
//        footY = footY + 2;
//      }
//      // if 1px away
//      if (footY == -1) {
//        // move foot down
//        footY = footY + 1;
//      }
//    }
//  }  
//  // foot up when A not pressed
//  if(arduboy.notPressed(A_BUTTON)) {
//    // if foot is not at top of screen...
//    if (footY > -20) {
//      // move foot up
//      footY--;
//    }
//  }

  // if the foot is raised
  if (footRaised == true) {
    // if A is pressed..
    if(arduboy.pressed(A_BUTTON) and bufferA == 0) {
      bufferA = 1;
      footLowering = true;
      footRaised = false;
    }
  }

  // if foot is lowering, and not at the bottom of the screen...
  if (footLowering == true and footY < 0) {
    // move the foot down
    footY = footY + 2;  
  }

  // if foot is at the bottom of the screen...
  if (footY == 0) {
    // stop foot lowering
    footLowering = false;
    // start foot raising
    footRaising = true;
  }

  // if foot is raising, and not at the top of the screen...
  if (footRaising == true and footY > -20) {
    footY = footY - 1;
  }

  // if the foot is at the top of the screen...
  if (footY == -20) {
    footRaising = false;
    footRaised = true;
  }
  
}

void moveAnts() {
  Ants[currentAnt].antX = Ants[currentAnt].antX - 1; 
}

void escapedAnts() {
  // if ant is offscreen
  if (Ants[currentAnt].antX <= 0) {
   // increment computer score
   computerScore++;
   // respawn ant
   Ants[currentAnt].antX = spawnX;
  }
}

void crushedAnts() {
  // if player foot is down
  if (footY == -1) {
    // if ant is under foot
    if (Ants[currentAnt].antX >= footX and Ants[currentAnt].antX <= footX + footWidth) {
      // increment player score
      playerScore++;
      // kill the ant
      Ants[currentAnt].alive = false;
    }
  }
}

void win() {
  arduboy.setCursor(0, 0);
  arduboy.print("You crushed them!");
  arduboy.setCursor(0, 16);
  arduboy.print("Press B to play again.");
  // press B to reset
  if(arduboy.pressed(B_BUTTON) and bufferB == 0) {
    bufferB = 1;
    resetgame();
    gamestate = 0;
  }
}

void lose() {
  arduboy.setCursor(0, 0);
  arduboy.print("Too many escaped!");
  arduboy.setCursor(0, 16);
  arduboy.print("Press B to try again.");
  // press B to reset
  if(arduboy.pressed(B_BUTTON) and bufferB == 0) {
    bufferB = 1;
    resetgame();
    gamestate = 0;
  }
}

void resetgame() {
  // ants
  antsSpawned = false;
  for ( int i = 0; i < numberOfAnts; i++ ) {
    Ants[i].alive = true;
  }
  numberOfAnts = 10;
  // scoring
  playerScore = 0;
  computerScore = 0;
  // player
  footX = 50;
  footY = -20;
  footRaised = true;
  footLowering = false;
  footRaising = false;
}

void resetbuttonbuffers() {
  // reset buffer state
  if (arduboy.notPressed(A_BUTTON)) {
    bufferA = 0;
  }
  if (arduboy.notPressed(B_BUTTON)) {
    bufferB = 0;
  }
  if (arduboy.notPressed(UP_BUTTON)) {
    bufferUp = 0;
  }
  if (arduboy.notPressed(DOWN_BUTTON)) {
    bufferDown = 0;
  }
  if (arduboy.notPressed(LEFT_BUTTON)) {
    bufferLeft = 0;
  }
  if (arduboy.notPressed(RIGHT_BUTTON)) {
    bufferRight = 0;
  }
}
