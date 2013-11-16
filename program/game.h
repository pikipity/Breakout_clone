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

extern void game_init(void);
extern void refresh_screen(void);

#endif