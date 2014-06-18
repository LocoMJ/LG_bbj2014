#include "sprites.c"
#include "tiles.c"

typedef enum {false, true} bool;

int DIFFICULTY = 2;
int SPEED = 2;

const int PLAYERSPEED = 1;

const int JUMPFORCE = 3;

bool apressed;
bool startpressed;
bool pause;

struct player {
	int x; // x axis position of the sprite
	int y; // y axis position of the sprite
	int realy; // real y axis position of the sprite
	int frame; // actual frame
	int totalframes; // frames of the animation
	bool inair;
	bool dead;
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

int bombframe;

struct danger {
	int x;
	int y;
	int set;
}

static struct danger spike0;
static struct danger spike1;
static struct danger spike2;

int disx; // x axis displacement of the sprite
int disy; // y axis displacement of the sprite
int disjump; // jump distance

int framecounter; // iterations per frame
int enframecounter;
int explosioncounter;
const int ITERPFRAME = 5;
const int ITERPFRAMEBOMB = 3;

int jumpcounter; // iterations per jump aceleration
const int ITERPJUMP = 3;

int dropcounter;

int setcounter;

void initialize() {

	philip.x = 40;
	philip.y = 120;
	philip.realy = 120;
	philip.frame = 2;
	philip.totalframes = 4;
	philip.inair = false;
	philip.dead = false;

	airenemy.x = 40;
	airenemy.y = 8;
	airenemy.frame = 12;
	airenemy.totalframes = 20;
	airenemy.drop = 0;
	airenemy.left = false;
	airenemy.bombx = airenemy.x;
	airenemy.bomby = airenemy.y;
	airenemy.exploding = false;

	spike0.x = 168;
	spike0.y = 50;
	spike0.set = 0;

	spike1.x = 168;
	spike1.y = 50;
	spike1.set = 0;

	spike2.x = 168;
	spike2.y = 50;
	spike2.set = 0;


	DIFFICULTY = 2;
	SPEED = 2;

	apressed = false;
	startpressed = false;
	pause = false;

	bombframe = 32;

	disx = 0;
	disy = 0;
	disjump = 0;

	framecounter = 0;
	enframecounter = 0;
	explosioncounter = 0;
	jumpcounter = 0;

	dropcounter = 150;
	setcounter = 30;

}

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

	if (philip.realy > 123) {
		philip.y -= PLAYERSPEED;
		philip.realy = 123;
	} else if(philip.realy < 96) {
		philip.y += PLAYERSPEED;
		philip.realy = 96;
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
		airenemy.bombx -= SPEED * 2;

	if (spike0.set > 0)
		spike0.x -= SPEED * 2;

	if (spike1.set > 0)
		spike1.x -= SPEED * 2;

	if (spike2.set > 0)
		spike2.x -= SPEED * 2;

	move_sprite(6, spike0.x, spike0.y);	
	move_sprite(7, spike1.x, spike1.y);	
	move_sprite(8, spike2.x, spike2.y);	

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
			airenemy.frame = 12;

		enframecounter = 0;
	}

	if (airenemy.exploding) {
		explosioncounter++;

		if (explosioncounter >= ITERPFRAMEBOMB) {
			bombframe += 4;
			if (bombframe >= 48) {
				bombframe = 32;
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
		set_sprite_tile(1, 10);
	else
		set_sprite_tile(1, 8);

	set_sprite_tile(2, airenemy.frame + airenemy.drop);
	set_sprite_tile(3, airenemy.frame + airenemy.drop + 2);

	if (airenemy.drop > 0 && !airenemy.exploding) {
		set_sprite_tile(4, 30);
		set_sprite_tile(5, 28);
	} else if (!airenemy.exploding) {
		set_sprite_tile(4, 28);
		set_sprite_tile(5, 28);
	} else {
		set_sprite_tile(4, bombframe);
		set_sprite_tile(5, bombframe + 2);
	}

	set_sprite_tile(6, 48 + spike0.set);
	set_sprite_tile(7, 48 + spike1.set);
	set_sprite_tile(8, 48 + spike2.set);

	if (philip.dead) {

		disy += disjump;

		jumpcounter++;

		if (jumpcounter >= ITERPJUMP) {
			disjump += 1;
			jumpcounter = 0;
		}

		if (disy >=0)
			philip.y += disy;
		else
			philip.y -= -disy;

		move_sprite(0, philip.x + 8, philip.y + 16);

		if (disjump > JUMPFORCE * 3)
			initialize();

	}
}

void loadbkg() {

	set_bkg_data(1, 2, bkgdata);

	drawbkg();

	SHOW_BKG;
}

void loadsprites() {

	SPRITES_8x16;

	set_sprite_data(0, 8, SuperPhilip);

	set_sprite_data(8, 12, Shadow);

	set_sprite_data(12, 28, enRobot);

	set_sprite_data(28, 48, Bomb);

	set_sprite_data(48, 52, Spikes);

	SHOW_SPRITES;
}

/* Process the changes in the environment
*/
void process() {
	scroll_bkg(SPEED, 0);

	if (airenemy.left)
		airenemy.x -= SPEED/2;
	else
		airenemy.x += SPEED/2;

	if (airenemy.x < 10)
		airenemy.left = false;

	if (airenemy.x > 135)
		airenemy.left = true;


	if (airenemy.drop == 0) {

		dropcounter -= 1;

		if (dropcounter <= 0) {
			airenemy.drop = 8;
			do {
				dropcounter = rand() / DIFFICULTY;
			} while (dropcounter <= 0);
		}
	}

	if (spike0.x < 0) {
		spike0.x = 168;
		spike0.y = 50;
		spike0.set = 0;
	}

	if (spike1.x < 0){
		spike1.x = 168;
		spike1.y = 50;
		spike1.set = 0;
	}

	if (spike2.x < 0){
		spike2.x = 168;
		spike2.y = 50;
		spike2.set = 0;
	}

	setcounter -= 1;

	if (setcounter <= 0) {
		if (spike0.set == 0) {
			spike0.y = 120 + abs(rand()%25);
			spike0.x = 168;
			spike0.set = 2;
		} else if (spike1.set == 0) {
			spike1.y = 120 + abs(rand()%25);
			spike1.x = 168;
			spike1.set = 2;
		} else if (spike2.set == 0) {
			spike2.y = 120 + abs(rand()%25);
			spike2.x = 168;
			spike2.set = 2;
		}

		do {
			setcounter = rand() / 2 / DIFFICULTY;
		} while (setcounter <= 0);
	}

}

/* process the input 
@param keys: array with a list of boolean values that dictates the keys pressed*/
void processinput(bool* keys) {

	if (!pause) {

		if (keys[0]) { // left
			disx = - PLAYERSPEED;
		}

		if (keys[1]) { // right
			disx = PLAYERSPEED;
		}

		if (keys[2]) { // up
			disy = - PLAYERSPEED;
			philip.realy -= -disy;
		}

		if (keys[3]) { // down
			disy = PLAYERSPEED;
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

	} else {
		disx = 0;
		disy = 0;
	}

	if (keys[6]) {
		
		if (!startpressed)
			if (!pause)
				pause = true;
			else
				pause = false;

		startpressed = true;
	} else {
		startpressed = false;
	}

}

void death() {

	philip.dead = true;

	pause = true;

	philip.frame = 4;

	disjump = -JUMPFORCE * 2;

	jumpcounter = 0;

	move_sprite(1, 160, 160);

}

void checkcollisions() {

	if (((philip.x + 16 > spike0.x && philip.x + 8 < spike0.x + 8 &&
		philip.y + 32 > spike0.y && philip.y + 12 < spike0.y) ||
		(philip.x + 16 > spike1.x && philip.x + 8 < spike1.x + 8 &&
		philip.y + 32 > spike1.y && philip.y + 12 < spike1.y) ||
		(philip.x + 16 > spike2.x && philip.x + 8 < spike2.x + 8 &&
		philip.y + 32 > spike2.y && philip.y + 12 < spike2.y) ||
		(airenemy.exploding && philip.x > airenemy.bombx && philip.x < airenemy.bombx + 12)
		) && philip.realy - philip.y < 3)
		death();
}