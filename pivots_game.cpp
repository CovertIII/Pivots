#include "pivots_game.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int pivots_game::load_file(char *location) 
{
	int i,j;
	FILE *loadFile;
	
	loadFile = fopen(location, "r");
	
	if(loadFile != NULL){
		fscanf(loadFile, "%s", lvl_author);
		fscanf(loadFile, "%d %d", &bound.x, &bound.y);
		fscanf(loadFile, "%d %d", &start.x, &start.y);
		fscanf(loadFile, "%d %d", &end.x, &end.y);
		cursor.x = start.x;
		cursor.y = start.y;
		
		fscanf(loadFile, "%d", &pivot_num);
		for(i=0; i<pivot_num; i++)
		{
			fscanf(loadFile,"%d %d", &pivot[i].x, &pivot[i].y);
		}
		
		fscanf(loadFile, "%d", &line_num);
		for(i=0; i<line_num; i++)
		{
			fscanf(loadFile,"%d %d", &line[i].pt1.x, &line[i].pt1.y);
			fscanf(loadFile,"%d %d", &line[i].pt2.x, &line[i].pt2.y);
		}
	}
	else
		{printf("File %s unable to load.", location);}
	fclose(loadFile);
}

v2i pivots_game::get_bound()
{
	return bound.Get();
}

v2i pivots_game::pos_start()
{
	return start.Get();
}

v2i pivots_game::pos_end()
{
	return end.Get();
}

v2i pivots_game::pos_cursor()
{
	return cursor.Get();
}

int pivots_game::num_pivot()
{
	return pivot_num;
}

v2i pivots_game::pos_pivot(int i)
{
	return pivot[i].Get();
}

int pivots_game::num_line()
{
	return line_num;
}

v2i pivots_game::pos_line_pt1(int i)
{
	return line[i].pt1.Get();
}

v2i pivots_game::pos_line_pt2(int i)
{
	return line[i].pt2.Get();
}