/*
 * author: bingus
 * */
#include "bpgame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef bpgame_c
#define bpgame_c

const int isGradescope = 0;

struct bpgame {

	int score;
	int nRow;
	int nCol;
	int move;

	char** mtx;

	BPGame* before;
};

void bp_display_STD(BPGame *b) {
	for (int r = 0; r < b->nRow; r++) {
		for (int c = 0; c < b->nCol; c++) {
			printf("%c", b->mtx[r][c]);
		}
		printf("\n");
	}
}

extern BPGame* bp_create(int nrows, int ncols) {
	BPGame* game = malloc(sizeof(BPGame));

	if(nrows < 0 || nrows > MAX_ROWS) {
		printf("ERROR in bp_create(int nrows, int ncols) nrows: %i out of range", nrows);
		return NULL;
	}

	if(ncols < 0 || ncols > MAX_ROWS) {
		printf("ERROR in bp_create(int nrows, int ncols) ncols: %i out of range", ncols);
		return NULL;
	}

	char colors[4] = {Red, Blue, Green, Yellow};

	char **matrix = (char **) malloc(sizeof(char *) * nrows);

	for (int i = 0; i < nrows; i++) {

		matrix[i] = (char *) malloc(sizeof(char) * ncols);
		for (int f = 0; f < ncols; f++) {

			int randomColor = rand() % 4;
			matrix[i][f] = colors[randomColor];
		}
	}

	game->score = 0;
	game->move = 0;
	game->mtx = matrix;
	game->nRow = nrows;
	game->nCol = ncols;
	game->before = NULL;

	return game;
}

extern BPGame* bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols) {
	if(nrows < 0) {
		printf("ERROR");
		return NULL;
	}

	if(ncols < 0) {
		printf("ERROR");
		return NULL;
	}

	BPGame *newGame = bp_create(nrows,ncols);
	newGame->score = 0;
	newGame->move = 0;
	newGame->nRow = nrows;
	newGame->nCol = ncols;
	newGame->before = NULL;

	newGame->mtx = malloc(nrows * sizeof(char*));

	for (int r = 0; r < nrows; r++) {
		newGame->mtx[r] = malloc(ncols * sizeof (char));

		for (int c = 0; c < ncols; c++) {
			newGame->mtx[r][c] = mtx[r][c];
		}
	}

	return newGame;
}

extern BPGame* bp_copy (BPGame* b){

	BPGame *newGame = bp_create(b->nRow,b->nCol);
	newGame->score = b->score;
	newGame->move = b->move;
	newGame->nRow = b->nRow;
	newGame->nCol = b->nCol;
	newGame->before = NULL;

	newGame->mtx = malloc(b->nRow * sizeof(char*));

	for (int r = 0; r < b->nRow; r++) {
		newGame->mtx[r] = malloc(b->nCol * sizeof (char));

		for (int c = 0; c < b->nCol; c++) {
			newGame->mtx[r][c] = b->mtx[r][c];
		}
	}

	return newGame;

}

extern void bp_destroy(BPGame * b) {

	int nrows = b->nRow;

	for(int i = 0; i < nrows; i++)
		free(b->mtx[i]);

	free(b->mtx);
}

void printBalloon(char b) {

	if (b == None) {
		printf(". ");
		return;
	}

	switch (b) {
		case Red:
			printf("\e[0;31m");
			break;
		case Blue:
			printf("\e[0;34m");
			break;
		case Green:
			printf("\e[0;32m");
			break;
		case Yellow:
			printf("\e[0;33m");
			break;
		default: // pink
			printf("\e[0;35m");
			break;

	}

	printf("O ");
	printf("\e[0m");
}

extern void bp_display(BPGame* b) {

	int nRow = b->nRow;
	//printf("  +-");
	int nCol = b->nCol;
	char** matrix = b->mtx;

	for (int i = 0; i < nCol; i ++) {
		printf("--");
	}

	printf("+\n");

	for (int j = 0; j < nRow; j ++) {
		printf("%i %s", j, "| ");
		for (int f = 0; f < nCol; f++) {

			(isGradescope) ? printf("%c ", matrix[j][f]) : printBalloon(matrix[j][f]);

		}
		printf("|\n");
	}

	printf("  +-");

	for (int i = 0; i < nCol; i ++) {
		printf("--");
	}

	printf("+\n    ");

	for (int k = 0; k < nCol; k ++) {

		printf("%i ", (k/10));

	}
	printf("\n    ");

	for (int k = 0; k < nCol; k ++) {

		printf("%i ", (k%10));

	}

	printf("\n");

}

extern int bp_can_pop(BPGame * b) {

	for (int r = 0; r < b->nRow; r++) {
		for (int c = 0; c < b->nCol - 1; c++) {
			if ((b->mtx[r][c] == b->mtx[r][c + 1]) && (b->mtx[r][c] != None)) {
				//printf("mtx[%i][%i]  mtx[%i][%i]\n", r, c, r, c + 1);
				return 1;

			}
		}
	}

	for (int r = 0; r < b->nRow - 1; r++) {
		for (int c = 0; c < b->nCol; c++) {
			if ((b->mtx[r][c] == b->mtx[r + 1][c]) && b->mtx[r][c] != None) {
				//printf("mtx[%i][%i]  mtx[%i][%i]\n", r, c, r + 1, c);
				return 1;
			}
		}
	}

	return 0;

}

int actuallyPop(BPGame* b, int r, int c) {

	if(r < 0 || r > MAX_ROWS) {
		printf("ERROR in bp_create(int nrows, int ncols) nrows: %i out of range", r);
		return 0;
	}

	if(c < 0 || c > MAX_COLS) {
		printf("ERROR in bp_create(int nrows, int ncols) ncols: %i out of range", c);
		return 0;
	}

	//printf("actuallypop(%i,%i)", r, c);

	char color = b->mtx[r][c];
	int totalPoped = 0;

	b->mtx[r][c] = None;
	totalPoped += 1;

	if (((r - 1 >= 0) && (r - 1 < b->nRow)) && (b->mtx[r - 1][c] == color)) {
		totalPoped  += actuallyPop(b,r - 1, c);
	}

	if (((r + 1 >= 0) && (r + 1 < b->nRow)) && (b->mtx[r + 1][c] == color)) {
		totalPoped += actuallyPop(b,r + 1, c);
	}

	if (((c - 1 >= 0) && (c - 1 < b->nCol)) && (b->mtx[r][c - 1] == color)) {
		totalPoped += actuallyPop(b,r, c - 1);
	}

	if (((c + 1 >= 0) && (c + 1 < b->nCol)) && b->mtx[r][c + 1] == color) {
		totalPoped += actuallyPop(b,r, c + 1);
	}

	return totalPoped;

}

extern int bp_pop(BPGame* b, int r, int c) {

	if ((r < 0) || (r > b->nRow)) {
		printf("ERROR in bp_create(int nrows, int ncols) nrows: %i out of range", r);
		return 0;
	}

	if ((c < 0) || (c > b->nCol)) {
		printf("ERROR in bp_create(int nrows, int ncols) ncols: %i out of range", c);
		return 0;
	}

	char color = b->mtx[r][c];

	if(color == None) {
		return 0;
	}

	int foundFriend = 0;

	foundFriend += ((r - 1 >= 0) && (b->mtx[r - 1][c] == color));
	//printf("foundFriend: %i\n",foundFriend);

	foundFriend += ((r + 1 < b->nRow) && (b->mtx[r + 1][c] == color));
	//printf("foundFriend: %i\n",foundFriend);

	foundFriend += ((c - 1 >= 0) && (b->mtx[r][c - 1] == color));
	//printf("foundFriend: %i\n",foundFriend);

	foundFriend += ((c + 1 < b->nCol) && (b->mtx[r][c + 1] == color));
	//printf("foundFriend: %i\n",foundFriend);

	if (foundFriend < 1) {
		return 0;
	}

	BPGame* old = bp_copy(b);

	b->move++;
	old->before = b->before;

	b->before = old;

	int totalPoped = actuallyPop(b, r, c);

	b->score += totalPoped * (totalPoped - 1);

	/*
	 * new
	 * V
	 * b
	 * V
	 * asdfsdf
	 * V
	 * null
	 *
	 * */

	return totalPoped;

}

extern int bp_is_compact(BPGame * b){

	int restIsAir;

	for (int c = 0; c < b->nCol; c++) {

		restIsAir = 0;

		for (int r = 0; r < b->nRow; r++) {

			if (b->mtx[r][c] == None) {
				restIsAir = 1;
			}

			if (restIsAir && b->mtx[r][c] != None) {
				return 0;
			}
		}
	}

	return 1;
}

extern void bp_float_one_step(BPGame * b){

	for (int c = 0; c < b->nCol; c++) {

		for (int r = 1; r < b->nRow; r++) {

			if (b->mtx[r][c] != None && b->mtx[r - 1][c] == None) {

				b->mtx[r - 1][c] = b->mtx[r][c];
				b->mtx[r][c] = None;

			}
		}
	}

}

extern int bp_score(BPGame * b){

	return b->score;
}

extern int bp_get_balloon(BPGame * b, int r, int c) {

	if ((r < 0) || (r >= b->nRow) || (c < 0) || (c >= b->nCol)) {
		return -1;
	}

	return b->mtx[r][c];
}

extern int bp_undo(BPGame* b) {

	if (b->before != NULL) {


		/*
		 * b
		 * V
		 * somn
		 * V
		 * null
		 * */
		//bp_create_from_mtx(b->next->mtx, b->next->nRow, b->next->nCol);

		//BPGame* bptr = b;


		//newGame->mtx = malloc(b->nRow * sizeof(char*));



		//b->mtx = b->next->mtx;

		BPGame* resetBtoThis = b->before;

		b->score = resetBtoThis->score;
		b->before = resetBtoThis->before;

		for (int r = 0; r < b->nRow; r++) {
			for (int c = 0; c < b->nCol; c++) {
				b->mtx[r][c] = resetBtoThis->mtx[r][c];
			}
		}

		//free(bptr);
		return 1;
	} else {
		return 0;
	}

}

/*
 * g
 * 0 0
 * g
 * 0 1
 * g
 * 0 2
 * g
 * 0 0
 * g
 * 3 2
 * g
 * 4 2
 * u
 * u
 * u
 * u
 *
 *
 *
 *
 *
 *
 *
 * */

#endif