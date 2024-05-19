#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>

struct block {
	int x;
	int y;
	struct block *next;
};
struct item {
	int x;
	int y;
	struct item *next;
};
struct block *blockhead = NULL;
struct item *itemhead = NULL;
int curx = 17;
int cury = 15;
int itemNum = 0;
int background[30][30];

void updateXy(char c, char prevc);
void updateSnake(int x, int y);
void printSnake(struct block *blockhead, struct item *itemhead);
struct block *makeBlock(int x, int y);
void makeSnake();
int checkSnake(int x, int y, struct block *blockhead);
void makeItem(int x, int y);
int lengthSnake(struct block *blockhead);
void makeRandItem(int i);

void updateSnake(int x, int y) {
	struct block *p = blockhead;
	struct block *prevp = blockhead;
	struct block *temp = NULL;
	struct item *pitem = itemhead;
	struct item *previtem = itemhead;
	int isitemloc = 0;

	while(p->next != NULL) {
		prevp = p;
		p = p->next;
	}
	
	if(pitem->x == x && pitem->y == y) {
		itemhead = itemhead->next;
		isitemloc = 1;
		itemNum--;
		free(pitem);
	} else {
		while(pitem != NULL) {
			if(pitem->x == x && pitem->y == y) {
				isitemloc = 1;
				previtem->next = pitem->next;
				free(pitem);
				itemNum--;
				break;
			}
			previtem = pitem;
			pitem = pitem->next;
		}
	}

	if(isitemloc != 1) {
		free(p);
		prevp->next = NULL;
	}
	
	temp = makeBlock(x, y);
	temp->next = blockhead;
	blockhead = temp;
}

void printSnake(struct block *blockhead, struct item *itemhead) {
	int y, x;

	for(y = 0; y < 30; y++)
		for(x = 0; x < 30; x++)
			background[y][x] = 0;

	while(itemhead != NULL) {
		background[itemhead->y][itemhead->x] = '+';
		itemhead = itemhead->next;
	}

	while(blockhead != NULL) {
		background[blockhead->y][blockhead->x] = 1;
		blockhead = blockhead->next;
	}

	for(y = 0; y < 30; y++) {
		for(x = 0; x < 30; x++){
			if(background[y][x] == 1)
				mvaddch(y, x, '*');
			else if(background[y][x] == '+')
				mvaddch(y, x, '+');
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

	blockhead = makeBlock(17,15);
	blockhead->next = NULL;
	p = blockhead;
	for(i = 5; i >= 0; i--) {
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

int checkSnake(int x, int y, struct block *blockhead) {
	if(blockhead == NULL)
		return 0;

	blockhead = blockhead->next;
	while(blockhead != NULL) {
		if(blockhead->x == x && blockhead->y == y)
			return 1;
		blockhead = blockhead->next;
	}

	return 0;
}

void makeItem(int x, int y) {
	struct item *p = itemhead;

	if(itemhead == NULL) {
		itemhead = (struct item *)malloc(sizeof(struct item));
		itemhead->x = x;
		itemhead->y = y;
		itemhead->next = NULL;
	} else {
		while(p->next != NULL) {
			p = p->next;
		}
		p->next = (struct item *)malloc(sizeof(struct item));
		p->next->x = x;
		p->next->y = y;
		p->next->next = NULL;
	}
}

int lengthSnake(struct block *blockhead) {
	int i = 0;
	while(blockhead != NULL) {
		i++;
		blockhead = blockhead->next;
	}
	return i;
}

void makeRandItem(int i) {
	int x, y;
	struct item *p = NULL;
	
	srand((unsigned int)time(NULL));

	while(i) {
		x = rand() % 30;
		y = rand() % 30;
		p = itemhead;
		while(p != NULL) {
			if(p->x == x && p->y == y)
				break;
			p = p->next;
		}
		if(p == NULL && (background[y][x] != '*')) {
			makeItem(x, y);
			i--;
		}
	}
}

int main() {
	int randItemNum = 5;
	char c = '\0';
	char prevc = '\0';
	
	initscr();
	timeout(500);
	makeSnake();
	printSnake(blockhead, itemhead);
	makeRandItem(randItemNum);
	itemNum = randItemNum;
	printSnake(blockhead, itemhead);
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
		printSnake(blockhead, itemhead);
		if(checkSnake(curx, cury, blockhead)) {
			mvprintw(12, 15, "GAME OVER!");
			mvprintw(13, 12, "SNAKE LENGTH: %d", lengthSnake(blockhead));
			while((c = getch()) != 'x');
			break;
		}
		if(itemNum == 0) {
			makeRandItem(randItemNum);
			itemNum = randItemNum;
		}
	}
	endwin();
	
	return 0;
}
