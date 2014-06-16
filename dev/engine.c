#include "sprites.c"
#include "tiles.c"

typedef enum {false, true} bool;

const int JUMPFORCE = 3;
const int SPEED = 1;

bool apressed;

struct player {
	int x; // x axis position of the sprite
	int y; // y axis position of the sprite
	int realy; // real y axis position of the sprite
	int frame; // actual frame
	int totalframes; // frames of the animation
	bool inair;
};

static struct player philip;

struct enemy {
	int x;
	int y;
	int frame;
	int totalframes;
	int drop;
	bool left;
	int bombx;
	int bomby;
	bool exploding;
}

static struct enemy airenemy;

int bombframe = 28;

int disx = 0; // x axis displacement of the sprite
int disy = 0; // y axis displacement of the sprite
int disjump = 0; // jump distance

int framecounter = 0; // iterations per frame
int enframecounter = 0;
int explosioncounter = 0;
const int ITERPFRAME = 5;
const int ITERPFRAMEBOMB = 3;

int jumpcounter = 0; // iterations per jump aceleration
const int ITERPJUMP = 3;

int dropcounter = 150;

void movesprites() {

	if (philip.inair) {
		disy += disjump;

		jumpcounter++;

		if (jumpcounter >= ITERPJUMP) {
			disjump += 1;
			jumpcounter = 0;
		}

		if (disjump > JUMPFORCE) {
			disjump = 0;
			philip.inair = false;
			philip.frame = 2;
			jumpcounter = 0;
			framecounter = 0;
		}
	}

	if (disx >= 0) 
		philip.x += disx;
	else
		philip.x -= -disx;

	if (disy >=0)
		philip.y += disy;
	else
		philip.y -= -disy;


	if (philip.x < 1 || philip.x > 151) {
		if (disx >=0)
			philip.x -= disx;
		else
			philip.x += -disx;
	}

	if (philip.realy > 127) {
		philip.y -= SPEED;
		philip.realy = 127;
	} else if(philip.realy < 92) {
		philip.y += SPEED;
		philip.realy = 92;
	}

	move_sprite(0, philip.x + 8, philip.y + 16);
	
	move_sprite(1, philip.x + 8, philip.realy + 17);

	move_sprite(2, airenemy.x + 8, airenemy.y + 17);
	move_sprite(3, airenemy.x + 16, airenemy.y + 17);

	move_sprite(4, airenemy.bombx + 12, airenemy.bomby + 17);
	move_sprite(5, airenemy.bombx + 12, airenemy.bomby + 33);

	if (airenemy.drop == 0) {
		airenemy.bombx = airenemy.x;
		airenemy.bomby = airenemy.y;
	} else if (!airenemy.exploding) {
		airenemy.bomby += 2;

		if (airenemy.bomby > 111) {
			airenemy.exploding = true;
			airenemy.bomby -= 5;
		}
	} else
		airenemy.bombx -= SPEED * 4;

}

void animatesprites() {

	framecounter++;

	if (framecounter >= ITERPFRAME) {
		philip.frame += 2;
	
		if (philip.frame >= philip.totalframes)
			philip.frame = 0;

		framecounter = 0;
	}

	enframecounter++;

	if (enframecounter >= ITERPFRAME) {
		airenemy.frame += 4;

		if (airenemy.frame >= airenemy.totalframes)
			airenemy.frame = 8;

		enframecounter = 0;
	}

	if (airenemy.exploding) {
		explosioncounter++;

		if (explosioncounter >= ITERPFRAMEBOMB) {
			bombframe += 4;
			if (bombframe >= 44) {
				bombframe = 28;
				airenemy.exploding = false;
				airenemy.drop = false;
			}

			explosioncounter = 0;
		}
	}

	if (philip.inair)
		philip.frame = 0;

}

void drawbkg() {
	int i, j;

	for (i = 0; i < 32; i++)
		for (j = 4; j < 12; j++)
			set_bkg_tiles(i, j, 1, 1, track);

	for (i = 0; i < 32; i++)	
		set_bkg_tiles(i, 12, 1, 1, borde);
}

void drawsprites() {

	set_sprite_tile(0, philip.frame);

	if (philip.realy - philip.y > 14)
		set_sprite_tile(1, 6);
	else
		set_sprite_tile(1, 4);

	set_sprite_tile(2, airenemy.frame + airenemy.drop);
	set_sprite_tile(3, airenemy.frame + airenemy.drop + 2);

	if (airenemy.drop > 0 && !airenemy.exploding) {
		set_sprite_tile(4, 26);
		set_sprite_tile(5, 24);
	} else if (!airenemy.exploding) {
		set_sprite_tile(4, 24);
		set_sprite_tile(5, 24);
	} else {
		set_sprite_tile(4, bombframe);
		set_sprite_tile(5, bombframe + 2);
	}

	movesprites();
	animatesprites();
}

void loadbkg() {

	set_bkg_data(1, 2, bkgdata);

	drawbkg();

	SHOW_BKG;
}

void loadsprites() {

	SPRITES_8x16;

	set_sprite_data(0, 4, SuperPhilip);

	philip.x = 40;
	philip.y = 120;
	philip.realy = 120;
	philip.frame = 2;
	philip.totalframes = 4;
	philip.inair = false;

	set_sprite_data(4, 8, Shadow);

	set_sprite_data(8, 24, enRobot);

	airenemy.x = 40;
	airenemy.y = 8;
	airenemy.frame = 8;
	airenemy.totalframes = 16;
	airenemy.drop = 0;
	airenemy.left = false;
	airenemy.bombx = airenemy.x;
	airenemy.bomby = airenemy.y;
	airenemy.exploding = false;

	set_sprite_data(24, 44, Bomb);

	drawsprites();

	SHOW_SPRITES;
}

/* Process the changes in the environment
*/
void process() {
	scroll_bkg(2, 0);

	if (airenemy.left)
		airenemy.x -= SPEED;
	else
		airenemy.x += SPEED;

	if (airenemy.x < 10)
		airenemy.left = false;

	if (airenemy.x > 135)
		airenemy.left = true;

	
	if (airenemy.drop == 0) {

		dropcounter -= 1;

		if (dropcounter <= 0) {
			airenemy.drop = 8;
			do {
				dropcounter = rand();
			} while (dropcounter <= 0);
		}
	}
	
}

/* process the input 
@param keys: array with a list of boolean values that dictates the keys pressed*/
void processinput(bool* keys) {

	if (keys[0]) { // left
		disx = - SPEED;
	}

	if (keys[1]) { // right
		disx = SPEED;
	}

	if (keys[2]) { // up
		disy = - SPEED;
		philip.realy -= -disy;
	}

	if (keys[3]) { // down
		disy = SPEED;
		philip.realy += disy;
	}

	if (keys[4]) { // a
		if (!philip.inair && !apressed) {
			philip.inair = true;
			disjump = - JUMPFORCE;
		}

		apressed = true;
	} else {
		apressed = false;
	}
	
	if (!keys[0] && !keys[1]) {
		disx = 0;
	}

	if (!keys[2] && !keys[3]) {
		disy = 0;
	}
}