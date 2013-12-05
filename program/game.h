#ifndef __GAME_H__
#define __GAME_H__

#define LS1 P1
#define change 0x00
#define title_music 0x01
#define level_music 0x02
#define run_music 0x03
#define do_1 0x04
#define do_2 0x05
#define die_music 0x06
#define win_music 0x07;

sbit start=P3^4;
sbit leftB=P3^2;
sbit rightB=P3^3;
sbit begin_bit=P3^6;
sbit finish_bit=P3^5;

extern void game_init(void);
extern void refresh_screen(void);
extern void handle_button(void);

#endif