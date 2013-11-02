#include <reg52.h>
#include "game.h"
#include "lcd12864.h"

//location of the most left point of bar
unsigned char bar_sit_x=23;
unsigned char bar_sit_y=63;
//bar speed
unsigned char bar_speed=3;
//bar length
unsigned char bar_len=16;
//bar direction
bit bar_direction=0;//0:left, 1:right

void game_init(){
	lcd_init();
	first_page();
}

void clear_bar(){
	unsigned char i;
	for(i=0;i<bar_len;i++){
		clear_dot(bar_sit_x+i,bar_sit_y);
	}
}

void draw_bar(){
	unsigned char i;
	for(i=0;i<bar_len;i++){
		set_dot(bar_sit_x+i,bar_sit_y);
	}
}

void move_bar(){
	if(bar_direction){
		if(bar_sit_x<=113-bar_speed){
			bar_sit_x+=bar_speed;
		}
	}else{
		if(bar_sit_x>=0+bar_speed){
			bar_sit_x-=bar_speed;
		}
	}
}

void refresh_screen(){
	clear_bar();
	move_bar();
	draw_bar();
}