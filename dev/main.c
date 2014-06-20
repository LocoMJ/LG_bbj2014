#include <stdio.h>
#include <stdlib.h>
#include <rand.h>
#include <gb/gb.h>
#include <gb/console.h>
#include <gb/sample.h>
#include "utils.c"
#include "map.c"
#include "engine.c"

#define PERFRAME 34 // milliseconds per iteration (34)

int seed = 0;

/* keys pressed (true) or not (false)
0: LEFT
1: RIGHT
2: UP
3: DOWN
4: A
5: B
6: START
7: SELECT
 */
unsigned bool keys[] = {
	false, false, false, false, false, false, false, NULL
};

/* gets the keys input */
void recordinput(int key) {

	if (key & J_LEFT) {
		keys[0] = true;
	} else {
		keys[0] = false;
	}

	if (key & J_RIGHT) {
		keys[1] = true;
	} else {
		keys[1] = false;
	}

	if (key & J_UP) {
		keys[2] = true;
	} else {
		keys[2] = false;
	}

	if (key & J_DOWN) {
		keys[3] = true;
	} else {
		keys[3] = false;
	}

	if (key & J_A) {
		keys[4] = true;
	} else {
		keys[4] = false;
	}

	if (key & J_B) {
		keys[5] = true;
	} else {
		keys[5] = false;
	}

	if (key & J_START) {
		keys[6] = true;
	} else {
		keys[6] = false;
	}

	if (key & J_SELECT) {
		keys[7] = true;
	} else {
		keys[7] = false;
	}

	processinput(keys);
}

void game() {

	myCls();

	gotoxy(4, 4);
	printf("Super Philip\n");

	gotoxy(4, 8);
	printf("PRESS START\n");

	gotoxy(0, 12);
	printf("Light Games:\n");
	printf("Imanolea, LocoMJ");

	gotoxy(10, 15);
	printf("BitBitJam\n");

	while(!(joypad() & J_START)) {
		seed++;
	}

	initrand(seed); 

	initialize();

	loadbkg();
	SHOW_BKG;

	loadsprites();

	/* main loop */
	while (TRUE) {

		recordinput(joypad());

		if (!pause) {

			movesprites();

			animatesprites();

			process();

			checkcollisions();
		}

		drawsprites();

		delay(PERFRAME);

	}
}

/* main program */
void main() {
	game();
}