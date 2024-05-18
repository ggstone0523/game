#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

struct block {
	int x;
	int y;
	struct block *next;
};
struct block *blockhead = NULL;
int curx = 15;
int cury = 15;

void updateXy(char c, char prevc);
void updateSnake(int x, int y);
void printSnake(struct block *blockhead);
struct block *makeBlock(int x, int y);
void makeSnake();

void updateSnake(int x, int y) {
	struct block *p = blockhead;
	struct block *prevp = blockhead;
	struct block *temp = NULL;

	while(p->next != NULL) {
		prevp = p;
		p = p->next;
	}
	
	free(p);
	prevp->next = NULL;
	
	temp = makeBlock(x, y);
	temp->next = blockhead;
	blockhead = temp;
}

void printSnake(struct block *blockhead) {
	int background[30][30];
	int y, x;

	for(y = 0; y < 30; y++)
		for(x = 0; x < 30; x++)
			background[y][x] = 0;

	while(blockhead != NULL) {
		background[blockhead->y][blockhead->x] = 1;
		blockhead = blockhead->next;
	}

	for(y = 0; y < 30; y++) {
		for(x = 0; x < 30; x++){
			if(background[y][x] == 1)
				mvaddch(y, x, '*');
			else
				mvaddch(y, x, ' ');
		}
	}
}

struct block *makeBlock(int x, int y) {
	struct block *p;
	
	p = (struct block *)malloc(sizeof(struct block));
	p->x = x;
	p->y = y;
	
	return p;
}

void makeSnake() {
	struct block *p;
	struct block *prevp;
	int i;

	blockhead = makeBlock(15,15);
	blockhead->next = NULL;
	p = blockhead;
	for(i = 3; i >= 0; i--) {
		prevp = p;
		p = makeBlock(11+i, 15);
		prevp->next = p;
	}
	p->next = NULL;
}

void updateXy(char c, char prevc) {
	switch(c) {
	case 'w':
		if(prevc == 's')
			curx = (curx + 1) % 30;
		else
			cury = ((cury - 1 < 0) ? 29 : cury - 1);
		break;
	case 'a':
		if(prevc == 'd' || prevc == '\0')
			cury = ((cury - 1 < 0) ? 29 : cury - 1);
		else
			curx = ((curx - 1 < 0) ? 29 : curx - 1);
		break;
	case 's':
		if(prevc == 'w')
			curx = ((curx - 1 < 0) ? 29 : curx - 1);
		else
			cury = (cury + 1) % 30;
		break;
	case 'd':
	default:
		if(prevc == 'a')
			cury = (cury + 1) % 30;
		else
			curx = (curx + 1) % 30;
		break;
	}
}

int main() {
	char c = '\0';
	char prevc = '\0';
	
	initscr();
	timeout(500);
	makeSnake();
	printSnake(blockhead);
	while(1) {
		c = getch();
		if(c == 'x')
			break;
		if(c == 'w' || c == 's' || c == 'a' || c == 'd') {
			updateXy(c, prevc);
			prevc = c;
		} else {
			updateXy(prevc, prevc);
		}
		updateSnake(curx, cury);
		printSnake(blockhead);
	}
	endwin();
	
	return 0;
}
