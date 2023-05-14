/***********************************/
/* A shared header file for read   */
/* map                             */
/***********************************/

/* include guard				   */
#ifndef MAP_READER_H
#define MAP_READER_H


/* Maximum number of points in the map. */
#define MAX_POINTS 1000

/* Function declarations for reading map data from files. */
int read_map(AdjacencyMatrix* adj_matrix);
int read_points(char* filename, AdjacencyMatrix* adj_matrix);
int read_edges(char* filename, AdjacencyMatrix* adj_matrix);
int read_geoms(char* filename, AdjacencyMatrix* adj_matrix);

/* end of include guard			   */
#endif /* MAP_READER_H */
