// cli manipulation library
#include <ncurses.h>

// standard libraries
#include <stdlib.h>
#include <unistd.h>

//--------------------//

#define PADDING 2
#define GAMEAREA_LENGTH 20
#define XSIZE_RATIO 2
#define DELAY_MULTIPLIER 1.2

typedef struct {
  int X;
  int Y;
} Vector2;

//--------------------//

// verification functions
int iswithinborders(Vector2 position,Vector2 borders);
int isplayercollision(Vector2 * parts, Vector2 position, int length, int end);

// routines for altering structs
void changevelocity (int key, Vector2* vel);
void changeposition (Vector2 vel, Vector2 * parts, int length);
void setapplepos (Vector2* apple, Vector2 borders);

int main(){
  WINDOW * mainwin = initscr(); // access to COLS and LINES

  keypad(mainwin, TRUE);        // access to special keys
  nodelay(mainwin, TRUE);       // making them non-blocking routines

  // game area data (should be a constant)
  Vector2 gamearea_start = {PADDING,PADDING};
  Vector2 gamearea_size = {GAMEAREA_LENGTH,GAMEAREA_LENGTH};
  
  // define the game area 
  WINDOW * gamearea = newwin(gamearea_size.Y,gamearea_size.X,gamearea_start.Y,gamearea_start.X);
  refresh;
  
  // player data
  Vector2 player_vel = {0,0};
  
  Vector2 *player_parts = calloc(GAMEAREA_LENGTH*GAMEAREA_LENGTH,sizeof(Vector2));
  player_parts[0] = (Vector2){gamearea_size.X/2,gamearea_size.Y/2};

  int player_length = 0;
  int i=0;
  
  // apple data
  Vector2 apple_pos;  
  setapplepos(&apple_pos, gamearea_size);
  
  while(true){    
    // clear the area
    werase(gamearea);
    
    // draw the borders
    box(gamearea, 0, 0);
        
    // check if player touched the apple
    if(player_parts[0].X==apple_pos.X&&player_parts[0].Y==apple_pos.Y){
      // continue changing the apple's until it fits the criterias
      do setapplepos(&apple_pos, gamearea_size);
      while(iswithinborders(apple_pos,gamearea_size) == 0 ||
	    isplayercollision(player_parts, apple_pos, player_length, 0) == 1);

      // increase the player's length
      player_length++;
      player_parts[player_length]=player_parts[player_length-1];
    }
    else{
      // draw the apple
      mvwaddstr(gamearea,apple_pos.Y,apple_pos.X,"*");
    }

    // get player input
    int pressed = wgetch(mainwin);
    changevelocity(pressed, &player_vel);

    // move the "head"
    changeposition(player_vel, player_parts, player_length);

    // and its body
    for(i=0;i<=player_length;i++)
      mvwaddstr(gamearea,player_parts[i].Y,player_parts[i].X,"#");

    // and mark the head with the cursor for visiblity
    wmove(gamearea,player_parts[0].Y,player_parts[0].X);

    // check if the player isnt stupid
    if(iswithinborders(player_parts[0], gamearea_size) == 0 ||
       isplayercollision(player_parts, player_parts[0], player_length, 1) == 1)
      break;
    
    // restart
    wrefresh(gamearea);
    usleep((int)100000*DELAY_MULTIPLIER);
  }
  
  mvprintw(gamearea_size.Y+PADDING,PADDING, "GAME OVER");
  refresh();

  getchar();
  
  // deallocate memory and exit the program
  endwin();
  free(player_parts);
}

//--------------------//
  
int iswithinborders(Vector2 pos, Vector2 bor){
  if(pos.X<=0||pos.X>=bor.X)
    return 0;
  if(pos.Y<=0||pos.Y>=bor.Y)
    return 0;

  return 1;
}

int isplayercollision(Vector2 * parts, Vector2 position, int length, int end){
  if (length < 1) return 0;

  int i;
  for(i=length; i>=end; i--){
    if (parts[i].X==position.X &&
	parts[i].Y==position.Y)
      return 1;
  }

  return 0;
}

void changevelocity(int key, Vector2* vel){
  switch(key){
  case(KEY_LEFT):
    vel->X=-1;
    vel->Y=0;
    break;
  case(KEY_RIGHT):
    vel->X=1;
    vel->Y=0;
    break;
  case(KEY_UP):
    vel->X=0;
    vel->Y=-1;
    break;
  case(KEY_DOWN):
    vel->X=0;
    vel->Y=1;
    break;
  default:
    break;
  }
  return;
}

void changeposition (Vector2 vel, Vector2 * parts, int length){
  // head of the snake is at index 0
  int i;
  
  // copy all the previous positions 1 index backwards
  if (length>0)
    for(i=length-1;i>=0;i--) parts[i+1]=parts[i];
    
  parts[0].X+=vel.X;
  parts[0].Y+=vel.Y;
}

void setapplepos (Vector2* apple, Vector2 borders){
  // from what i've read, "rand" is not a good random number generator
  // but this isnt a serious project so i dont mind using it :3
  apple->X=(rand()%(borders.X-2))+1;
  apple->Y=(rand()%(borders.Y-2))+1;
}
