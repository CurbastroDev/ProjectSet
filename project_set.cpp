#include "SimpleSDL.h"
#include <time.h>
#include <stdio.h>
#include <iostream>

using namespace std;

//Screen coordinates
const int SCREEN_X=800;
const int SCREEN_Y=600;

//Spaceship data
const int PLAYER_W=15;
const int PLAYER_H=15;
const float PLAYER_ROCKET_ACCEL = 0.38;		//Calibration needed
const int FIRE_W=15;
const int FIRE_H=10;
int pfuel;                //Improve with difficuly improving
char fuel [50];
char vertspd [50];
char plevel [50];

//Player moving key
const int KEY_LEFT = SDLK_a;
const int KEY_RIGHT = SDLK_d;
const int KEY_UP = SDLK_w;
const int KEY_DOWN = SDLK_s;
bool pavailable=true;

//Parachute act key
const int KEY_SPACE = SDLK_SPACE;
const float X_DECELL = 0.05;
const float Y_DECELL = 1.15;
const float Y_MIN_SPEED = 1.5;

//Landing platform data
const int PLATF_W_MIN = 20;		
const int PLATF_W_MAX = 100;
const int PLATF_H = 10;
int xplatf;
int wplatf;
const float LANDING_VPY_MAX=3.5;				//Need calibration, changes with levels

//Gravitational const
const float G_EARTH = 0.0981;       						//We've divider by 100 all the gravities
const float G_NEPTUNE = 0.1120;
const float G_MARS = 0.0771;
const float G_VENUS = 0.0887;
const float G_SATURN = 0.05044;

//Asteroids constants
const int AST_NUM = 5;
const int AST_V_MAX = 4;
const float AST_V_MIN = 0.5;
const int AST_D_MAX = 20;
const int AST_D_MIN = 5;
const int AST_DIST_X = 20;						//Min X distance from asetroid spawn point and spaceship
const int AST_DIST_Y = 20;						//Max Y distance from asteroid spam point and spaceship

//Spaceship moving data
int xp,yp;
float vpx,vpy;
float apx,apy;
float dt;                              

//Planet gravity
float g;

//Asteroids data
float ast [AST_NUM] [5];
int i;
int j;
int d;                 //Asteroid diameter
bool ac;               //Asteroid collisions var
int xadif;				//Next two are used to verify the distance between asteroids spawn point and spaceship
int yadif;

//Pressed key 
int key;
int rockacc;

//Winning bool
bool w;

//Mouse coords
int xm;
int ym;

//Exit var
int end;

//Level var
int level;



int main(int argc, char* args[]){
	SimpleSDL s("Project_SET", SCREEN_X, SCREEN_Y);
	
	//New screen color 
	s.setColor("spaceCol", 65, 66, 73);  
	s.setColor("warningCol", 255, 24, 13); 
	s.setColor("fire",255,88,40);
	s.setColor("asteroid",94,112,130);                                                                        
	
	srand(time(NULL));
	
	s.useColor("spaceCol");
	s.clear();
	level=0;
	
	
	do{

		if(w==true)
			level++;
		pfuel=150-(level*5);
		end=0;
		g=0;
		
		do{
			s.useColor("fg");
			s.drawRect(100,50,100,50);
			s.text(132,68, "EARTH", 15);
			s.drawRect(600,50,100,50);
			s.text(623,68, "NEPTUNE", 15);
			s.drawRect(100,500,100,50);
			s.text(135,518, "MARS", 15);
			s.drawRect(600,500,100,50);
			s.text(630,518, "VENUS", 15);
			s.drawRect(350,250,100,50);
			s.text(377,268, "SATURN", 15);
		
			s.redraw();
		
			if(xm>100 && xm<200 && ym>50 && ym<100)
				g=G_EARTH;
			if(xm>600 && xm<700 && ym>50 && ym<100)
				g=G_NEPTUNE;
			if(xm>100 && xm<200 && ym>500 && ym<550)
				g=G_MARS;
			if(xm>600 && xm<700 && ym>500 && ym<550)
				g=G_VENUS;
			if(xm>350 && xm<450 && ym>250 && ym<300)
				g=G_SATURN;
			
			if(s.checkEvent()){
				if(s.isMouseButtonEvent()){
					xm=s.getMouseX();
					ym=s.getMouseY();	
				}
		    }
		
		  }while(g==0);
	 
	//Select planet gravity
	                                            												
	//Spaceship starting data
	xp=400;
	yp=5;
	dt=0;																										
	vpx=0;
	vpy=0;
	apx=0;
	apy=0;
	rockacc=0;
	
	//Build platform coords
	wplatf=PLATF_W_MIN+rand()%PLATF_W_MAX;
	xplatf=rand()%800-wplatf;
	
	s.useColor("bg");

    s.redraw(); 
    
    //Set asteroids data
    for(j=0; j!=5; j++){
		for(i=0; i!=AST_NUM; i++){
			  //Set random x   
			if(j==0)
				ast[i][j]=(-100)+rand()%(SCREEN_X+100);	
			  	//Set random y					      
			if(j==1)
				ast[i][j]=(-100)+rand()%(SCREEN_Y+100);					
			  //Set a permanent diamter
			if(j==4)
				ast[i][j]=AST_D_MIN+rand()%(AST_D_MAX-AST_D_MIN);
			 //Set random vax & vay looking at the spam zone of the asteroid
			if(j!=0 && j!=1){
				if(ast[i][0]<=SCREEN_X/2){
					//It means it spawns in the left side of the screen
					if(ast[i][1]<=SCREEN_Y/2){
						//In the high on the left
						ast[i][2]=AST_V_MIN+rand()%AST_V_MAX;
						ast[i][3]=AST_V_MIN+rand()%AST_V_MAX;
					}
					else{
						//In the down left 
						ast[i][2]=AST_V_MIN+rand()%AST_V_MAX;
						ast[i][3]=(AST_V_MIN+rand()%AST_V_MAX)*(-1);
					}
				}
				if(ast[i][0]>SCREEN_X/2){
				//It means it spawns int the right side of the screen
					if(ast[i][1]<=SCREEN_Y/2){
						//In the high on the right
						ast[i][2]=(AST_V_MIN+rand()%AST_V_MAX)*(-1);
						ast[i][3]=AST_V_MIN+rand()%AST_V_MAX;
    				}
					else{
						//In the down right
    					ast[i][2]=(AST_V_MIN+rand()%AST_V_MAX)*(-1);
  						ast[i][3]=(AST_V_MIN+rand()%AST_V_MAX)*(-1);
   					}
       			}
    		}
 		}
	}
	
	ac=false;
	
	if(vpy<0)
		pavailable=true;
						
	//Core code begins//					
	
	do{ 
		//Player y/x acceleration management or parachute activation
		dt=1;
		if(s.checkEvent()){
		 	if(s.isKeyEvent()){
				key=s.getKeyCode();                                                    
		        if(key==SDLK_w && rockacc<pfuel){
					apy=g-PLAYER_ROCKET_ACCEL;
					apx=0;
					rockacc++;
					}
				if(key==SDLK_s && rockacc<pfuel){
					apy=g+PLAYER_ROCKET_ACCEL;
					apx=0;
					rockacc++;
				}
				if(key==SDLK_a){
					apx=(-1)*PLAYER_ROCKET_ACCEL;
				}
				if(key==SDLK_d){
					apx=PLAYER_ROCKET_ACCEL;
				}
				if(key==SDLK_SPACE && pavailable){
					pavailable=false;
				}
			    s.flushEvents(); 
				}
	    	}
		else{
			apy=g;
	    	apx=0;
		}
	    if(pavailable==false){
			//Decrease speed and accelerations if parachute in opened
			if(vpx!=0)
				vpx=vpx-(X_DECELL)*vpx;											
			if(vpy>Y_MIN_SPEED)
				apy=-(Y_DECELL)*apy;
		}				    
		s.useColor("spaceCol");														
		s.clear();
		s.useColor("bg");
		
		
		//Drawing the spaceship and the platform
		s.fillRect(xplatf, SCREEN_Y-PLATF_H, wplatf, PLATF_H ); 
		s.fillRect(xp,yp,PLAYER_W,PLAYER_H);
		
		
		//Writing the different engine fire in the spaceship
		if(key==SDLK_w){
			s.useColor("fire");
			s.fillRect(xp,yp+PLAYER_H,FIRE_W,FIRE_H);
		}
		if(key==SDLK_a){
			s.useColor("fire");
			s.fillRect(xp+PLAYER_W,yp,FIRE_H,FIRE_W);
		}
		if(key==SDLK_d){
			s.useColor("fire");
			s.fillRect(xp-FIRE_H,yp,FIRE_H,PLAYER_W);
		} 
		if(key==SDLK_s){
		s.useColor("fire");
		s.fillRect(xp,yp-FIRE_H,FIRE_W,PLAYER_H);
		}			     
		
		
		//Drawing all the asteroids
		s.useColor("asteroid");
		for(i=0; i!=AST_NUM; i++)
			s.fillOval(ast[i][0], ast[i][1], ast[i][4], ast[i][4]);            
		
		key=0;
		
		//Spaceship position managment
		yp=yp+vpy*dt+apy*0.040*dt*dt;
		xp=xp+vpx*dt+apx*0.025*dt*dt;
		vpy=vpy+apy*dt;
		vpx=vpx+apx*dt;
		s.useColor("fg");
		sprintf(fuel, "Fuel: %d/%d", rockacc,pfuel);
		sprintf(vertspd, "Vertical speed: %g", vpy);
		sprintf(plevel, "Level: %d" , level);
		s.text(650,20, vertspd,10);
		s.text(650,30, fuel  ,10);	
		s.text(650,40,plevel,10);
		s.useColor("warningCol");
		if(rockacc>=pfuel)
			s.text(SCREEN_X/2-80,SCREEN_Y/2,"You've run out of fuel!",10);	
		
		
		//Asteroids managment
			//Update the new asteroids position
		for(i=0; i!=AST_NUM; i++){
			ast[i][0]=ast[i][0]+ast[i][2]*dt;
			ast[i][1]=ast[i][1]+ast[i][3]*dt;
		}
		//I set new coords and new speeds for the asteroids out of the screen
		for(i=0; i!=AST_NUM; i++){
			if(ast[i][0]>900 || ast[i][0]<(-100) || ast[i][1]<(-100) || ast[i][1]>700){
				do{ 
					//I do it while the asteroids are appearing 20 pixel far from the spaceship 
					ast[i][0]=(-100)+rand()%(SCREEN_X+100);
					ast[i][1]=(-100)+rand()%(SCREEN_Y+100);
					//Check taht all the asteroids have born far from spaceship
		            xadif=xp-ast[i][0];
		            yadif=yp-ast[i][1];
				}
				while(abs(xadif)<AST_DIST_X && abs(yadif)<AST_DIST_Y);
				
				//Here i set all the way the asteroids DOESN'T have to appear
				//now it's gonna set all the asteroid's accellerations														
				if(ast[i][0]<=SCREEN_X/2){
					//It means it spawns in the left side of the screen
					if(ast[i][1]<=SCREEN_Y/2){
						//In the high on the left
						ast[i][2]=AST_V_MIN+rand()%AST_V_MAX;
						ast[i][3]=AST_V_MIN+rand()%AST_V_MAX;
					}
					else{
						//In the down left 
			  			ast[i][2]=AST_V_MIN+rand()%AST_V_MAX;
			  			ast[i][3]=(AST_V_MIN+rand()%AST_V_MAX)*(-1);
			 		}
				}
				if(ast[i][0]>SCREEN_X/2){
					//It means it spawns int the right side of the screen
					if(ast[i][1]<=SCREEN_Y/2){
						//In the high on the right
				   		ast[i][2]=(AST_V_MIN+rand()%AST_V_MAX)*(-1);
				   		ast[i][3]=AST_V_MIN+rand()%AST_V_MAX;
					}
					else{
						//In the down right
						ast[i][2]=(AST_V_MIN+rand()%AST_V_MAX)*(-1);
						ast[i][3]=(AST_V_MIN+rand()%AST_V_MAX)*(-1);
					}
			}
		}
	}
		
																																																																						
		
		s.redraw();
		
		//I check if there're colliasoins against the asteroids																																																										
		for(i=0; i!=AST_NUM; i++){
			if(xp<ast[i][0] && yp<ast[i][1]){
				//The spaceship is upper and lefter than the ast
				if(ast[i][0]-xp<=PLAYER_W && ast[i][1]-yp<=PLAYER_H)
				 	ac=true;
			}
	      	if(xp<ast[i][0] && yp>ast[i][1]){
			  	//Downer and lefter
	      	    if(ast[i][0]-xp<=PLAYER_W && yp-ast[i][1]<=PLAYER_H)
				  	ac=true;
	      	}
	      						     
	      	if(xp>ast[i][0] && yp<ast[i][1]){//Upper and righter
	      	    if(xp-ast[i][0]<=PLAYER_W && ast[i][1]-yp<=PLAYER_H)
				  	ac=true;
	      	}
	      	if(xp>ast[i][0] && yp>ast[i][1]){//Downer and righter
	      		if(xp-ast[i][0]<=PLAYER_W && yp-ast[i][1]<=PLAYER_H)
				    ac=true;
	      	}
		}					      
      	//I've checked if there's been contacts along the ateroid's edges
		s.delay(25);
		}
		while(xp>=0 && xp<=SCREEN_X-PLAYER_W && yp>=-10 && yp<SCREEN_Y-PLAYER_H && ac==false);
		
	s.useColor("spaceCol");
	s.clear();
		
	//It goes out when out of the screen, now check if it's on the platform at the right speed
	
	w=true;
	

	//Right landing position checking	
	if((xp+PLAYER_W<xplatf || xp>xplatf+wplatf) && ac==false){
		w=false;
        s.useColor("fg");
		s.text(SCREEN_X/2-150, SCREEN_Y/2, "Your spaceship has been lost in the deep space.", 10);
	}
	                                                                    	     
	//Right speed checking   
	if(vpy>LANDING_VPY_MAX && w==true && ac==false){
		w=false;
		s.useColor("fg");
		s.text(SCREEN_X/2-150, SCREEN_Y/2, "Your spaceship landed on the platform too fastly.", 10);
	}
	
	//I check if there has been a contact with the asteroids and the spaceship
	if(ac==true && w==true){
		w=false;
	    s.useColor("fg");
	    s.text(SCREEN_X/2-150, SCREEN_Y/2, "Your spaceship has been destroyed against an asteroid!", 10);       																								
	}
	
	//If w is still true means that the player has won
	if(w==true){
		s.useColor("fg");
		s.text(SCREEN_X/2-200, SCREEN_Y/2, "Well done captain! Your spaceship has landed on the approach platform!", 10);
	}
	           
	pavailable=true;		//Closes parachute
	           	
	s.redraw();
	
	s.delay(3500);
	xm=0;
	ym=0;
	do{
	s.useColor("spaceCol");
	s.clear();
	s.useColor("fg");
	s.drawRect(50,50,700,100);
	s.text(340,90,"Play again!", 15);
	sprintf(plevel, "You got the level %d ", level);
	s.text(320,290, plevel, 15);
	s.drawRect(50,450,700,100);
	s.text(360,490,"Exit.",15);
	s.useColor("fg");																					/////////////
	s.drawTLine(200, 200, 600, 800, 100);																	/////////////
	s.redraw();
	
	if(s.checkEvent()){
		if(s.isMouseButtonEvent()){
			xm=s.getMouseX();
			ym=s.getMouseY();	
		}
    }
    
    //In the end var 0 means that the player hasn't decided what to do yet, 1 means that he wants to play again and 2 means that he want to go out (asshole)
    
	if(xm>50 && xm<750 && ym>50 && ym<150)
		end=1;
	if(xm>50 && xm<750 && ym>450 && ym<550)
	    end=2;
	
    }while(end==0);
	
	s.useColor("spaceCol");
	s.clear();
		
    }while(end!=2);
	
	system("pause");
	
	return 0;

}

