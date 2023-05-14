// This program includes header files for standard input/output streams, 
// commonly used utility libraries, and custom implementation files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h" // Header file for hash table implementation
#include "mapAdjMatrix.h" // Header file for adjacency matrix graph representation
#include "mapError.h" // Header file for error handling in map operations
#include "mapReader.h" // Header file for reading map data
#include "mapEditor.h" // Header file for editing map data
#include "pathFinder.h" // Header file for path finding algorithms
#include "pathVisualizer.h" // Header file for visualizing paths on the map.


/***********************************/
/* XJCO 1921M Programming Project  */
/* 2022-23 Spring Semester         */
/*                                 */
/* Version: Final Version          */
/* Yifan Xiang					   */
/***********************************/

/***********************************/
/* Main Routine                    */
/***********************************/


int main() {

	// Define an adjacency matrix variable
	AdjacencyMatrix adj_matrix;

	// Initialize the adjacency matrix
	init_adjacency_matrix(&adj_matrix);

	// Read in map data and populate adjacency matrix
	read_map(&adj_matrix);

	// Prompt user to modify map if map is modified, new map writed
	ask_modify_map(&adj_matrix);

	// Find a path between two points using Dijkstra's algorithm
	int* path = (int*)malloc(sizeof(int) * (adj_matrix.num_points));
	memset(path, -1, sizeof(int) * adj_matrix.num_points);
	ask_find_path(&adj_matrix, path);
	
	// Visualize the path on the map
	visualize(&adj_matrix, path);

	// Free dynamically allocated memory before exiting program
	free(path);
	free_adjacency_matrix(&adj_matrix);

	return 0;
}
