// This program includes header files for standard input/output streams, SDL graphics library,
// commonly used utility libraries, and custom implementation files.
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h> // Header file for SDL graphics library
#include <math.h>

#include "hashTable.h" // Header file for hash table implementation
#include "mapAdjMatrix.h" // Header file for adjacency matrix graph representation
#include "mapError.h" // Header file for error handling in map operations
#include "mapReader.h" // Header file for reading map data
#include "mapEditor.h" // Header file for editing map data
#include "pathFinder.h" // Header file for path finding algorithms
#include "pathVisualizer.h" // Header file for visualizing paths on the map.

// Define the constant of Earth radius in kilometers.
const double EARTH_RADIUS = 6371.0;
#define M_PI 3.1415926
// Define the struct for nodes.
SDL_Rect* graphicPoints;
SDL_Window* window;
SDL_Renderer* renderer;

void Mercaor_x_y(double lon, double lat, double* x, double* y) {
	// Calculate the X and Y coordinates in Mercator projection.
	*x = EARTH_RADIUS * lon * M_PI / 180.0;
	*y = EARTH_RADIUS * log(tan((M_PI / 4) + (lat * M_PI / 180) / 2));
}

void init(AdjacencyMatrix* adj_matrix,int *w_window, int *h_window) 
{
	double lon_len = adj_matrix->bounding.maxLon - adj_matrix->bounding.minLon;
	double lat_len = adj_matrix->bounding.maxLat - adj_matrix->bounding.minLat;
	double factor_window = lat_len / lon_len;
	*w_window = 1200;
	*h_window = *w_window * factor_window*2;
	//Initialize the corresponding window, renderer, and texture.
	SDL_Init(SDL_INIT_VIDEO);
	// Create a window named "Path Draw", with width and height of 600 pixels, and show it in the center of the 
	window = SDL_CreateWindow("Path Draw", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, *w_window+200, *h_window+200, SDL_WINDOW_SHOWN);
	// Create a renderer to render graphics content.
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	// Enable anti-aliasing.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	//n nodes
	graphicPoints = (SDL_Rect*)malloc(sizeof(SDL_Rect) * adj_matrix->num_points);
	double min_x, min_y, max_x, max_y, factor_x, factor_y;
	Mercaor_x_y(adj_matrix->bounding.minLon, adj_matrix->bounding.minLat, &min_x, &min_y);
	Mercaor_x_y(adj_matrix->bounding.maxLon, adj_matrix->bounding.maxLat, &max_x, &max_y);
	factor_x = *w_window / (max_x - min_x);
	factor_y = *h_window / (max_y - min_y);
	for (int i = 0; i < adj_matrix->num_points; i++) {
		double x, y;
		Mercaor_x_y(adj_matrix->points[i].lon, adj_matrix->points[i].lat, &x, &y);
		x = (x - min_x) * factor_x + 100;
		y = *h_window - (y - min_y) * factor_y + 100;
		graphicPoints[i].x = x - 1;
		graphicPoints[i].y = y - 1;
		//Set the matrix length and width.
		graphicPoints[i].w = 2;
		graphicPoints[i].h = 2;
	}
}

void quit() 
{ // Destroy all data in heap to avoid memory overflow
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	free(graphicPoints);
}

// Render the adjacency matrix and shortest path between two points
void render(AdjacencyMatrix* adj_matrix, int* path)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // clear the renderer
	SDL_RenderClear(renderer);
	
	// Draw buildings
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100); // set building color to green
	
	for (int i = 0; i < adj_matrix->num_geoms; i++) 
	{ // draw each building's outline
		for (int j = 0; j < adj_matrix->geoms[i].len_nodes - 1; j++) 
		{
			int p1 = adj_matrix->geoms[i].nodes[j];
			int p2 = adj_matrix->geoms[i].nodes[j + 1];
			SDL_RenderDrawLine(renderer, graphicPoints[p1].x + 1, graphicPoints[p1].y +
				1, graphicPoints[p2].x + 1, graphicPoints[p2].y + 1);
		}
		int last_point = adj_matrix->geoms[i].nodes[adj_matrix->geoms[i].len_nodes - 1];
		int first_point = adj_matrix->geoms[i].nodes[0];
		SDL_RenderDrawLine(renderer, graphicPoints[last_point].x + 1, graphicPoints[last_point].y +
			1, graphicPoints[first_point].x + 1, graphicPoints[first_point].y + 1);
	}
	
	// Draw base path
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // set base path color to gray
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); // black
	for (int i = 0; i < adj_matrix->num_points; i++) 
	{
		for (int j = 0; j < adj_matrix->num_points; j++) 
		{
			if (adj_matrix->adj[i][j] != INF && adj_matrix->adj[i][j] != 0) 
			{
				// if two points are connected, draw the line
				double link_cos = 0;
				double link_sin = 1;
				if ((graphicPoints[j].x - graphicPoints[i].x) != 0)
				{
					double slope = (graphicPoints[j].y - graphicPoints[i].y) / (graphicPoints[j].x - graphicPoints[i].x);
					link_cos = 1 / sqrt(1 + slope * slope);
					link_sin = slope / sqrt(1 + slope * slope);
				}
				SDL_RenderDrawLine(renderer, graphicPoints[i].x +1 + link_sin, graphicPoints[i].y + 1 + link_cos,
					graphicPoints[j].x + 1 + link_sin, graphicPoints[j].y + 1 + link_cos);
				SDL_RenderDrawLine(renderer, graphicPoints[i].x + 1 - link_sin, graphicPoints[i].y + 1 - link_cos,
					graphicPoints[j].x + 1 - link_sin, graphicPoints[j].y + 1 - link_cos);
			}
		}
	}
	
	// SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
	// SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // orange
	SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255); // set shortest path color to red
	// Draw shortest path in red from s to t
	for (int i = 0; i < adj_matrix->num_points; i++)
	{
		if (path[i + 1] < 0 || path[i + 1] > adj_matrix->num_points - 1)
		{
			break;
		}
		int p1 = path[i];
		int p2 = path[i + 1];
		double link_cos = 0;
		double link_sin = 1;
		if ((graphicPoints[p1].x - graphicPoints[p2].x) != 0)
		{
			double slope = (graphicPoints[p1].y - graphicPoints[p2].y) / (graphicPoints[p1].x - graphicPoints[p2].x);
			link_cos = 1 / sqrt(1 + slope * slope);
			link_sin = slope / sqrt(1 + slope * slope);
		}
		SDL_RenderDrawLine(renderer, graphicPoints[p1].x + 1 + link_sin, graphicPoints[p1].y + 1 + link_cos,
			graphicPoints[p2].x + 1 + link_sin, graphicPoints[p2].y + 1 + link_cos);
		SDL_RenderDrawLine(renderer, graphicPoints[p1].x + 1 - link_sin, graphicPoints[p1].y + 1 - link_cos,
			graphicPoints[p2].x + 1 - link_sin, graphicPoints[p2].y + 1 - link_cos);
		SDL_RenderDrawLine(renderer, graphicPoints[p1].x + 1, graphicPoints[p1].y +
			1, graphicPoints[p2].x + 1, graphicPoints[p2].y + 1);
	}

	// Draw base vertices
	for (int i = 0; i < adj_matrix->num_points; i++) {
		SDL_RenderDrawRect(renderer, &graphicPoints[i]);
	}

	// Fill vertices with light blue color
	for (int i = 0; i < adj_matrix->num_points; i++) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 200, 255); // blue sky
		SDL_RenderFillRect(renderer, &graphicPoints[i]);
	}

	// render to the screen
	// delay for 1 millisecond to run test.sh
	SDL_RenderPresent(renderer);
	SDL_Delay(20000);
}


// Visualize the adjacency matrix and shortest path using SDL2
int visualize(AdjacencyMatrix* adj_matrix, int* path)
{
	int w_window = 0.0;
	int h_window = 0.0;
	
	// initialize the SDL2 window and renderer with adjacency matrix dimensions
	init(adj_matrix, &w_window, &h_window);
	
	// render the adjacency matrix and shortest path using SDL2
	render(adj_matrix, path);
	
	// destroy all data in heap and free memory to avoid memory leaks
	quit();
	return FUNCTION_SUCCESS;
}
