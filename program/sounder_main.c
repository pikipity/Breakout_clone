#include <reg52.h>

#define reciver P2
#define change 0x00
#define title 0x01
#define level 0x02
#define run 0x03
#define do_1 0x04
#define do_2 0x05
#define die 0x06
#define win 0x07

//timer0 autoreload value
#define TH0value 0xEC
//length of one sound
#define begin_skip 0
#define end_skip 190 
#define max_sounder_time 190

sbit LS1=P3^7;

//count for timer0
unsigned char sounder=0;

//music
//define 7 notes
unsigned char code max_sounder[]={95,85,76,71,63,57,50,48,42,38,36,32,28,25,190,170,151,143,127,113,101,95};
//the length of one note
unsigned char sounder_time=0;
//define music sheet
//do:1,     re:2,     mi:3,     fa:4,     so:5,     la:6,     si:7,
//do_up:8,  re_up:9,  mi_up:10, fa_up:11, so_up:12, la_up:13, si_up:14,
//do_low:15,re_low:16,mi_low:17,fa_low:18,so_low:19,la_low:20,si_low:21,
//skip,22,end:0
unsigned char music_num=0;
unsigned char code title_music[]={15,15,17,19,19,17,15,15,17,19,19,17,15,15,16,17,16,0};
unsigned char code level_music[]={21,21,20,19,18,20,21,19,20,21,21,0};
unsigned char code run_music[]={0};
unsigned char code do_1_music[]={1,0};
unsigned char code do_2_music[]={2,0};
unsigned char code die_music[]={21,20,19,18,17,15,15,15,22,22,0};

void sounder_do(unsigned char music){
	if(sounder>=max_sounder[music-1]){
		sounder=0;
		sounder_time++;
		if(sounder_time>=begin_skip && sounder_time<end_skip){
			if(music!=22){
				LS1=~LS1;
			}
		}else if(sounder_time>=max_sounder_time){
			music_num++;
			sounder_time=0;
		}
	}	
}

//Use timer0 to control sounder
//timer0 initial
//timer -> 10us
void init_timer0(){
	TMOD=TMOD|0x02;
	TL0=TH0value;
	TH0=TH0value;
	TR0=1;
}

void init_int(){
	EA=1;
	ET0=1;
}

void hardware_init(){
	LS1=0;
	init_timer0();
	init_int();
}

void main(){
	//init timer1 and two external interrupts
	hardware_init();
	while(1){
		//adjust sound
		if(reciver==change){
			TR0=0;
			TL0=TH0value;
			music_num=0;
			TR0=1;
		}else if(reciver==title){
			if(title_music[music_num]==0){
				music_num=0;
			}else{
				sounder_do(title_music[music_num]);
			}
		}else if(reciver==level){
			if(level_music[music_num]==0){
				music_num=0;
			}else{
				sounder_do(level_music[music_num]);
			}
		}else if(reciver==run){
			if(run_music[music_num]==0){
				music_num=0;
			}else{
				sounder_do(run_music[music_num]);
			}
		}else if(reciver==do_1){
			if(do_1_music[music_num]==0){
				music_num=0;
			}else{
				sounder_do(do_1_music[music_num]);
			}
		}else if(reciver==do_2){
			if(do_2_music[music_num]==0){
				music_num=0;
			}else{
				sounder_do(do_2_music[music_num]);
			}
		}else if(reciver==die){
			if(die_music[music_num]==0){
				music_num=0;
			}else{
				sounder_do(die_music[music_num]);
			}
		}else if(reciver==win){
			if(title_music[music_num]==0){
				music_num=0;
			}else{
				sounder_do(title_music[music_num]);
			}
		}
	}
}

void int_timer0() interrupt 1{
	sounder++;	
}