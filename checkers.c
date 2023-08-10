/* project.c
 *Tom O'Connor
 *Fundamentals of Computing
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gfx.h"
#include <math.h>
#define squareLen 100
#define boardWidth 800 
#define boardHeight 800
#define boardSize 8
typedef struct{
	int color; //-1=not playable 0=not 1=r or 2=b
	char type; //n for normal k for king
	int xpos;
	int ypos;
	}piece;
void drawBoard(piece [boardSize][boardSize]);
void initBoard(piece [boardSize][boardSize]);
void drawSquare(int, int);
void inMove(piece [boardSize][boardSize], int *,int *);
void makeMove(piece [boardSize][boardSize],int, int ,int ,int);
int isValid(piece [boardSize][boardSize], int *,int,int,int,int);
int kingValid(piece [boardSize][boardSize], int *,int,int,int,int);

int main()
{
	piece board[boardSize][boardSize];
	
	gfx_open(boardWidth,boardHeight,"Checkers");
	initBoard(board);//initialize the board
	int firstMove=1;//set red to move first
	int *move;
	move=&firstMove;
	int q=0;
	int *quit=&q;
	while(*quit==0)//check to make sure that user did not input 'q'
	{
		drawBoard(board);//draw the board
		inMove(board,move, quit); //take input for the move
	}

	return 0;
}

void initBoard(piece board[boardSize][boardSize])
{
	for(int i=0;i<boardSize;i++){
		for(int j=0;j<boardSize;j++){
			if(i<3 && (i+j)%2==0){//set up the red pieces
				board[i][j].color=1;
				board[i][j].type='n';//set the piece types to normal
			}else if(i>4 &&(i+j)%2==0){//set up black pieces
				board[i][j].color=2;
				board[i][j].type='n';
			}else if(i>=3&&i<=4&&(i+j)%2==0){//set the color for the playable squares
				board[i][j].color=0;
			}
			else {
				 board[i][j].color=-1;//mark the unplayable squares
			 }
		}
	}
}

void drawBoard(piece board[boardSize][boardSize])
{
	for (int i=0; i<8; i++){
		for (int j=0; j<8;j++){ //alternate the color of the squares
			if((i+j)%2==0){
				gfx_color(22,184,135);
				}else{
					gfx_color(255,255,255);
				}
			drawSquare(j*squareLen,i*squareLen);
		}
	}
	for(int i=0; i<8; i++){
		for(int j=0;j<8;j++){
			if(board[i][j].color==1){
				gfx_color(255,0,0);
				for(int r=35; r>0 ;r--){
				gfx_circle(j*squareLen+50,i*squareLen+50,r);
				}
				if(board[i][j].type=='k'){//signal that is king
					for(int r=10;r>0; r--){
						gfx_color(255,255,255);
						gfx_circle(j*squareLen+50,i*squareLen+50,r);
					}
				}
			}else if(board[i][j].color==2){
				for(int r=35; r>0 ;r--){
					gfx_color(0,0,0);
					gfx_circle(j*squareLen+50,i*squareLen+50,r);
				}
				if(board[i][j].type=='k'){//signal that a piece is a king
					for(int r=10;r>0; r--){
						gfx_color(255,255,255);
						gfx_circle(j*squareLen+50,i*squareLen+50,r);
					}
				}
			}else if(board[i][j].color==0){
				for(int r=35;r>0;r--){
					gfx_color(22,184,135);
					gfx_circle(j*squareLen+50,i*squareLen+50,r); }
			}

		}
	}
}	
void drawSquare(int x, int y)
{
	int xmax=x+squareLen;
	int ymax=y+squareLen;
	while(x<xmax)//keep making squares until both x values meet to fill them
	{
		gfx_line(x,y,xmax,y);
		gfx_line(x, y, x, ymax);
		gfx_line(x, ymax, xmax, ymax);
		gfx_line(xmax, y, xmax, ymax);
		x++;//increase lower bounds
		y++;
		xmax--;//decrease upper bounds
		ymax--;
	}

}
void inMove(piece board[boardSize][boardSize], int *move, int *quit)
{
	int xclick1, yclick1, xclick2, yclick2;
	char c=gfx_wait();
	if(c==1){//get first click
		xclick1=gfx_xpos();
		yclick1=gfx_ypos();
	}
	if (c=='q')//check if quitting
	{
		*quit=1;
		return;
	}
	char d=gfx_wait();
	if(d==1){//get second click
		xclick2=gfx_xpos();
		yclick2=gfx_ypos();
	}
	if (d=='q')//check if quitting
	{
		*quit=1;
		return;
	}
	int x1=xclick1/100;
	int y1=yclick1/100;
	int x2=xclick2/100;
	int y2=yclick2/100;
	if (board[y1][x1].type=='k'){
		if(kingValid(board, move, x1, y1,x2,y2)==1){//check if valid king move
			makeMove(board,x1,y1,x2,y2);
			if(*move==1){//change what color's move it is
				*move=2;
			}
			else{
				*move=1;
			}
		}
	}
	else if(board[y1][x1].type=='n'){
		if(isValid(board, move, x1, y1, x2, y2)==1){//check if valid normal move
			makeMove(board,x1,y1,x2,y2);
			if(*move==1){//chenge whose move it is
				*move=2;
			}
			else if(*move==2){
				*move=1;
			}
		}
	}
	else{
		return;
	}
}

void makeMove(piece board[boardSize][boardSize], int x1, int y1, int x2, int y2)
{
	piece tempPiece=board[y2][x2];
	board[y2][x2]=board[y1][x1];
	board[y1][x1]= tempPiece;
	if(y2==0 || y2== 7){//check if reached the end of board
		board[y2][x2].type='k';//make it a king if reaches the end
	}
}

int isValid(piece board[boardSize][boardSize], int *move,int x1,int y1,int x2,int y2)
{
	if(x1==x2||y1==y2){//make sure piece is trying to move
		return 0;
	}
	if(board[y1][x1].color==*move){
		if(*move==1&& board[y2][x2].color==0){//check if space is open and red's move
			if(y2==y1+1 && abs(x2-x1)==1){//make sure diagonal move and correct direction
				return 1;
			}else if(board[(y1+y2)/2][(x1+x2)/2].color==2&&y2>y1){
				board[(y1+y2)/2][(x1+x2)/2].color=0;//jump the piece and remove the piece being
													//jumped
				return 1;
			}
			
		}
		else if(board[y2][x2].color==0&&*move==2){//check if space is open and black's move
			if(board[y2][x2].color==0&&y2==y1-1 && abs(x2-x1)==1){
				return 1;
			}else if(board[(y1+y2)/2][(x1+x2)/2].color==1&&y2<y1){
				board[(y1+y2)/2][(x1+x2)/2].color=0;//jump the piece
				return 1;
			}
		}
	}
	return 0;//return invalid
}


int kingValid(piece board[boardSize][boardSize], int *move,int x1,int y1,int x2,int y2)
{
	if(x1==x2||y1==y2){//make sure trying to move to different square
		return 0;
	}
	if(board[y1][x1].color==*move){//check that correct color's turn
		if(board[y2][x2].color==0){
			if(abs(y2-y1)==1 && abs(x2-x1)==1)//check that moving one space
			{
				return 1;
			}
			else if(board[(y1+y2)/2][(x1+x2)/2].color!=board[y1][x1].color){
				board[(y1+y2)/2][(x1+x2)/2].color=0;//jump the piece
				return 1;
			}
		}
	}
	return 0; 	
}






