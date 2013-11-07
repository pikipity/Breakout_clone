#ifndef __GAME_H__
#define __GAME_H__

sbit start=P3^4;

extern void game_init(void);
extern void refresh_screen(void);

#endif