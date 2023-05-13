#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <math.h>

#include "hashTable.h"
#include "mapAdjMatrix.h"
#include "mapError.h"
#include "mapReader.h"
#include "mapEditor.h"
#include "pathFinder.h"
#include "pathVisualizer.h"

// �������뾶Ϊ��������λ km
const double EARTH_RADIUS = 6371.0;
//������ṹ��
SDL_Rect* graphicPoints;
SDL_Window* window;
SDL_Renderer* renderer;

void Mercaor_x_y(double lon, double lat, double* x, double* y) {
	// ����ī����ͶӰ�е� X �� Y ����ֵ
	*x = EARTH_RADIUS * lon * M_PI / 180.0;
	*y = EARTH_RADIUS * log(tan((M_PI / 4) + (lat * M_PI / 180) / 2));
}

void init(AdjacencyMatrix* adj_matrix,int *w_window, int *h_window) 
{
	double lon_len = adj_matrix->bounding.maxLon - adj_matrix->bounding.minLon;
	double lat_len = adj_matrix->bounding.maxLat - adj_matrix->bounding.minLat;
	double factor_window = lat_len / lon_len;
	*w_window = 1000;
	*h_window = *w_window * factor_window*2;
	//��ʼ����Ӧwindow��renderer��texture
	SDL_Init(SDL_INIT_VIDEO);
	// ����һ����Ϊ "Path Draw" �Ĵ��ڣ���͸߾�Ϊ 600 ���أ�������ʾ����Ļ����
	window = SDL_CreateWindow("Path Draw", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, *w_window+200, *h_window+200, SDL_WINDOW_SHOWN);
	// ����һ����Ⱦ�� renderer��������Ⱦͼ������
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	// ���ÿ����
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	graphicPoints = (SDL_Rect*)malloc(sizeof(SDL_Rect) * adj_matrix->num_points);//n�����
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
		graphicPoints[i].w = 2;//���þ��󳤿�
		graphicPoints[i].h = 2;
	}
}

void quit() 
{ //�������ж��е����ݣ������ڴ����
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	free(graphicPoints);
}

void render(AdjacencyMatrix* adj_matrix, int* path)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//red
	SDL_RenderClear(renderer);
	//���ƽ�����
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100); //��ɫ
	for (int i = 0; i < adj_matrix->num_geoms; i++) 
	{
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
	//���ƻ���·��
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // ��ɫ
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);//��ɫ
	for (int i = 0; i < adj_matrix->num_points; i++) {
		for (int j = 0; j < adj_matrix->num_points; j++) {
			if (adj_matrix->adj[i][j] != INF && adj_matrix->adj[i][j] != 0) {//�����ӣ���ô�����
				SDL_RenderDrawLine(renderer, graphicPoints[i].x +
					1, graphicPoints[i].y + 1, graphicPoints[j].x + 1, graphicPoints[j].y + 1);//-1��Ϊ�˱�֤��������
			}
		}
	}
	// SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); //��ɫ
	// SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // ��ɫ
	SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255); //��ɫ
	//�������·����s��t�����·��
	for (int i = 0; i < adj_matrix->num_points; i++)
	{
		if (path[i + 1] < 0 || path[i + 1] > adj_matrix->num_points - 1)
		{
			break;
		}
		int p1 = path[i];
		int p2 = path[i + 1];
		SDL_RenderDrawLine(renderer, graphicPoints[p1].x + 1, graphicPoints[p1].y +
			1, graphicPoints[p2].x + 1, graphicPoints[p2].y + 1);
	}
	//���ƻ�������
	for (int i = 0; i < adj_matrix->num_points; i++) {
		SDL_RenderDrawRect(renderer, &graphicPoints[i]);
	}
	//�����������ɫ
	for (int i = 0; i < adj_matrix->num_points; i++) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 200, 255); // ����ɫ
		SDL_RenderFillRect(renderer, &graphicPoints[i]);
	}
	SDL_RenderPresent(renderer);//��Ⱦ����Ļ
	SDL_Delay(100000);
}

int visualize(AdjacencyMatrix* adj_matrix, int* path)
{
	int w_window = 0.0;
	int h_window = 0.0;
	init(adj_matrix, &w_window, &h_window);
	render(adj_matrix, path);
	quit();
	return FUNCTION_SUCCESS;
}
