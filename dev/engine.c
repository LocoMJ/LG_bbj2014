#include "sprites.c"

typedef enum {false, true} bool;

struct player {
	int x; // x axis position of the sprite
	int y; // y axis position of the sprite
	int frame; // actual frame
	int totalframes; // frames of the animation
};

static struct player philip;

int disx = 0; // x axis displacement of the sprite
int disy = 0; // y axis displacement of the sprite

int framecounter = 0; // iterations per frame
const int ITERPFRAME = 5;

void movesprites() {

	philip.x += disx;
	philip.y += disy;

	move_sprite(0, philip.x, philip.y);

}

void animatesprites() {

	framecounter++;

	if (framecounter >= ITERPFRAME) {
		philip.frame += 2;
	
		if (philip.frame >= philip.totalframes)
			philip.frame = 0;

		framecounter = 0;
	}

}

void drawsprites() {

	set_sprite_tile(0, philip.frame);

	movesprites();
	animatesprites();
}

void loadsprites() {

	SPRITES_8x16;

	set_sprite_data(0, 4, SuperPhilip);

	philip.x = 40;
	philip.y = 120;
	philip.frame = 0;
	philip.totalframes = 4;

	drawsprites();

	SHOW_SPRITES;
}

/* process the input 
@param keys: array with a list of boolean values that dictates the keys pressed*/
void processinput(bool* keys) {

	if (keys[0]) { // left
		disx = - 1;
	}

	if (keys[1]) { // right
		disx = 1;
	}

	if (keys[2]) { // up
		disy = - 1;
	}

	if (keys[3]) { // down
		disy = 1;
	}

	if (!keys[0] && !keys[1]) {
		disx = 0;
	}

	if (!keys[2] && !keys[3]) {
		disy = 0;
	}
}