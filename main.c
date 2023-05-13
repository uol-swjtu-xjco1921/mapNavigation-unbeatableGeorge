#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
#include "mapAdjMatrix.h"
#include "mapReader.h"
#include "mapEditor.h"
#include "pathFinder.h"
#include "pathVisualizer.h"

/***********************************/
/* XJCO 1921M Programming Project  */
/* 2022-23 Spring Semester         */
/*                                 */
/* Version: Store data in adj matrix */
/* Yifan Xiang					   */
/***********************************/

/***********************************/
/* Main Routine                    */
/***********************************/


int main() {
	AdjacencyMatrix adj_matrix; // Define an adjacency matrix variable

	init_adjacency_matrix(&adj_matrix); // Initialize the adjacency matrix

	int result = read_map(&adj_matrix);

	result = ask_modify_map(&adj_matrix);    // modify_map
	if (result == 1)
	{
		printf("\nNumber of modifications: %d\n", adj_matrix.num_edit);
	}

	// pathFinder
	int* path = (int*)malloc(sizeof(int) * (adj_matrix.num_points)); // 分配内存空间
	memset(path, -1, sizeof(int) * adj_matrix.num_points);
	result = ask_find_path(&adj_matrix, path);
	
	// pathVisualizer
	result = visualize(&adj_matrix, path);

	free(path); // 释放动态分配的内存空间
	return 0;
}