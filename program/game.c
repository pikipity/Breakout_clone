#include <reg52.h>
#include "game.h"
#include "lcd12864.h"

//variables of game
unsigned char game_status=1;//2: win  1: continue. 0: end

//variables of block
unsigned char code block[]={
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xF7,0xEF,0xEF,0xDF,0xDF,0xBF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x7F,0x9F,0xE7,0xFB,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,0xFD,0xFD,
0x0D,0xED,0xED,0xED,0xED,0xED,0x6D,0x6D,0x2C,0x2D,0xED,0xED,0xED,0xED,0xED,0xED,
0xED,0xED,0x2D,0x2C,0x6D,0x6D,0x6D,0xED,0xED,0xED,0xED,0xED,0x0D,0xFD,0xFD,0x01,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,
0x80,0xBF,0xBF,0xBF,0xBC,0xBC,0xBE,0xBE,0xB3,0xAD,0xAF,0xB7,0xBB,0xB9,0xB7,0xAF,
0xAF,0xB5,0xBB,0xBF,0xBE,0xBE,0xBE,0xBF,0xBF,0xBF,0xBF,0xBF,0x80,0xFF,0xFF,0x00,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xFD,0xFD,
0xFD,0xFD,0xCD,0xC1,0xF1,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,
0xFD,0xFD,0xFD,0xFD,0xF1,0xC1,0xCD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFC,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

//functions of block
//draw block at the begining
void draw_block(){
	unsigned char x,y;
	choose_screen(0);
	for(x=0;x<4;x++){
		for(y=0;y<64;y++){
			lcd_write_command(0xb8|x);
			lcd_write_command(0x40|y);
			lcd_write_data(block[x*64+y]);
		}	
	}
}
//check block
void check_block(){
	unsigned char i,x,y,rdata;
	unsigned char now_game_status=game_status;
	game_status=2;
	for(i=1;i<3;i++){
		choose_screen(i);
		for(x=0;x<4;x++){
			for(y=0;y<64;y++){
				lcd_write_command(0xb8|x);
				lcd_write_command(0x40|y);
				rdata=lcd_read_data();
				if(rdata!=0x00){
					game_status=now_game_status;
					break;
				}
			}
			if(game_status==now_game_status){
				break;
			}
		}
		if(game_status==now_game_status){
			break;
		}
	}
}

//initial fiunction
//init the game
void game_init(){
	lcd_init();
	first_page();
	draw_block();
}

//variables of bar
//location of the most left point of bar
unsigned char bar_sit[]={0,63};//(x,y)
//bar speed
unsigned char bar_speed=1;
//bar length
unsigned char bar_len=16;
//bar direction
bit bar_direction=1;//0:left, 1:right

//functions for bar
//clear preview bar
void clear_bar(){
	unsigned char i;
	for(i=0;i<bar_len;i++){
		clear_dot(bar_sit[0]+i,bar_sit[1]);
	}
}
//calculate now bar
void move_bar(){
	if(bar_direction){
		if(bar_sit[0]<=113-bar_speed){
			bar_sit[0]+=bar_speed;
		}
	}else{
		if(bar_sit[0]>=0+bar_speed){
			bar_sit[0]-=bar_speed;
		}
	}
}
//draw bar
void draw_bar(){
	unsigned char i;
	//first, clear previous bar
	clear_bar();
	//calculate now bar
	move_bar();
	//draw now bar
	for(i=0;i<bar_len;i++){
		set_dot(bar_sit[0]+i,bar_sit[1]);
	}
}

//variables of ball
//location of ball
unsigned char ball_sit[]={7,62};//(x,y)
//ball speed
unsigned char ball_speed[]={0,1};//(x,y)
//bar direction
bit ball_direction_x=0;//x: 0->left, 1->right.  
bit ball_direction_y=0;//y: 0->down, 1->up.

//functions for ball
//clear preview ball
void clear_ball(){
	clear_dot(ball_sit[0],ball_sit[1]);
}
//calculate now ball
void move_ball(){
	bit next_dot_y,next_dot_x;
	unsigned char i;
	//check whether the direction should be changed
	if(ball_direction_y){
		if(read_dot(ball_sit[0],ball_sit[1]-1) || ball_sit[1]==0){
			clear_dot(ball_sit[0],ball_sit[1]-1);
			ball_direction_y=~ball_direction_y;	
		}
	}else{
		if(read_dot(ball_sit[0],ball_sit[1]+1)){
			if(ball_sit[1]!=63){
				if(ball_sit[1]+1!=bar_sit[1]){
					clear_dot(ball_sit[0],ball_sit[1]+1);
				}else{
					ball_speed[0]=bar_speed;
					ball_direction_x=bar_direction;
				}
				ball_direction_y=~ball_direction_y;
			}else{
				game_status=0;
			}
		}
	}
	if(game_status==1){
	if(ball_direction_x){
		if(read_dot(ball_sit[0]+1,ball_sit[1]) || ball_sit[0]==127){
			clear_dot(ball_sit[0]+1,ball_sit[1]);
			ball_direction_x=~ball_direction_x;
		}
	}else{
		if(read_dot(ball_sit[0]-1,ball_sit[1]) || ball_sit[0]==0){
			clear_dot(ball_sit[0]-1,ball_sit[1]);
			ball_direction_x=~ball_direction_x;
		}
	}
	//according to y direction to check next y dot status
	next_dot_y=0;
	if(ball_direction_y){
		//go up
		for(i=1;i<1+ball_speed[1];i++){//check if there a dot in speed range
			if(ball_sit[1]-i==0){
				next_dot_y=1;
				ball_sit[1]-=i;
				break;
			}else if(read_dot(ball_sit[0],ball_sit[1]-i)){//if there is a dot
				next_dot_y=1;
				ball_sit[1]-=(i-1);
				break;
			}
		}
		if(~next_dot_y){
			ball_sit[1]-=ball_speed[1];
		}
	}else{
		//go down
		for(i=1;i<1+ball_speed[1];i++){//check if there a dot in speed range
			if(read_dot(ball_sit[0],ball_sit[1]+i)){//if there is a dot
				next_dot_y=1;
				ball_sit[1]+=(i-1);
				break;
			}
		}
		if(~next_dot_y){
			ball_sit[1]+=ball_speed[1];
		}
	}
	//according to x direction to check next x dot status
	next_dot_x=0;
	if(ball_direction_x){
		//go right
		for(i=1;i<1+ball_speed[1];i++){//check if there a dot in speed range
			if(ball_sit[0]+i==127){
				next_dot_x=1;
				ball_sit[0]+=i;
				break;
			}else if(read_dot(ball_sit[0]+i,ball_sit[1])){//if there is a dot
				next_dot_x=1;
				ball_sit[0]+=(i-1);
				break;
			}
		}
		if(~next_dot_x){
			ball_sit[0]+=ball_speed[0];
		}
	}else{
		//go left
		for(i=1;i<1+ball_speed[1];i++){//check if there a dot in speed range
			if(ball_sit[0]-i==0){
				next_dot_x=1;
				ball_sit[0]-=i;
				break;
			}else if(read_dot(ball_sit[0]-i,ball_sit[1])){//if there is a dot
				next_dot_x=1;
				ball_sit[0]-=(i-1);
				break;
			}
		}
		if(~next_dot_x){
			ball_sit[0]-=ball_speed[0];
		}
	}
	}

}
//draw now ball
void draw_ball(){
	//clear previous ball
	clear_ball();
	//calculate now ball
	move_ball();
	//draw now ball
	set_dot(ball_sit[0],ball_sit[1]);;
}

//refresh screen
void refresh_screen(){
	unsigned x,y;//for testing
	//check block
	check_block();
	//draw bar
	if(game_status==1){
		//continuous status
		draw_bar();
		draw_ball();
	}else if(game_status==0){
		//end status
		////////////////////////////////
		//for testing
		choose_screen(0);
		for(x=0;x<8;x++){
		for(y=0;y<64;y++){
			lcd_write_command(0xb8|x);
			lcd_write_command(0x40|y);
			lcd_write_data(0x00);
			}	
		}
		while(1);
		////////////////////////////////
	}else if(game_status==2){
		//win status
		/////////////////////////////
		//for testing
		choose_screen(0);
		for(x=0;x<8;x++){
		for(y=0;y<64;y++){
			lcd_write_command(0xb8|x);
			lcd_write_command(0x40|y);
			lcd_write_data(0xFF);
			}	
		}
		while(1);
		//////////////////////////////
	}
}