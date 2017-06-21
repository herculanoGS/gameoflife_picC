/**
 *
 * @file C_gameoflife_serial.c
 *
 * Conway's Game of Life
 *
 * Code modified from that available at Rosetta Code
 * 
 * http://rosettacode.org/wiki/Conway%27s_Game_of_Life#C
 *
 * Available under GNU Free Documentation License 1.2
 *
 * see README.md for futher details
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f4550.h>
#include <usart.h>
#include <timers.h>
#include <GenericTypeDefs.h>

#ifdef TERMINAL
// flag to use when we're printing directly to terminal on ARM/x86
#include <unistd.h>
#endif

/// loop sugars
#define for_x for (int x = 0; x < w; x++)
#define for_y for (int y = 0; y < h; y++)
#define for_xy for_x for_y

/// global variable containing world's width
const int w = 25;
/// global variable containing world's height
const int h = 25;
/// current world state
unsigned int univ[w][h];
/// next world state
unsigned int new[w][h];

/**
 * shows the current state of the world on ARM/x86 
 * 
 * @param *u the world
 * @param w the world width
 * @param h the world height
 * 
 */
void show(void *u, int w, int h)
{

	// int (*univ)[w] = u;
	printf("\033[H"); /// printf("\033[H"); terminal controls to show animation console
	for_y
	{
		for_x printf(univ[y][x] ? "\033[07m  \033[m" : "  ");
		printf("\033[E");
	}
	fflush(stdout);
}

/**
 * evolves the world to the next state 
 *
 * @param *u the world
 * @param w the world width
 * @param h the world height
 *
 */
void evolve(void *u, int w, int h)
{
	// unsigned (*univ)[w] = u;
	//unsigned new[h][w];

	for_y for_x
	{
		int n = 0;
		for (int y1 = y - 1; y1 <= y + 1; y1++)
			for (int x1 = x - 1; x1 <= x + 1; x1++)
				if (univ[(y1 + h) % h][(x1 + w) % w])
					n++;

		if (univ[y][x])
			n--;
	  // checks if the cell is alive
		new[y][x] = (n == 3 || (n == 2 && univ[y][x])); 
	}
	// updating world using it's new state
	for_y for_x univ[y][x] = new[y][x];
}

/**
 * bootstraping the game
 */
void game()
{
	for_xy univ[y][x] = rand() < RAND_MAX / 10 ? 1 : 0;
	while (1)
	{
		// the game's main loop 
		show(univ, w, h);
		evolve(univ, w, h);
#ifdef TERMINAL
		usleep(200000);
#endif
	}
}

int main(int c, char **v)
{
	game();
	
	/*Pre-emptively terminates the UART communication to establish a new communication*/
    CloseUSART();

	/*Assigns the config variable, the configurations required to initialize the UART
Based on the macros of the usart.h library*/
    config = USART_TX_INT_OFF | USART_RX_INT_OFF | USART_ASYNCH_MODE | USART_EIGHT_BIT | USART_CONT_RX | USART_BRGH_LOW;

	/*baud rate definitions*/
    baudUSART (BAUD_8_BIT_RATE | BAUD_AUTO_OFF);
}
