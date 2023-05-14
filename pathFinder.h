/***********************************/
/* A shared header file for find   */
/* path                            */
/***********************************/

/* include guard				   */
#ifndef PATH_FINDER_H
#define PATH_FINDER_H

/* Macro const for infinity value. */
#define INF 9999

/* Function declarations for path finding using Dijkstra's algorithm. */
int ask_find_path(AdjacencyMatrix* adj_matrix, int* path);
double dijkstra(AdjacencyMatrix* adj_matrix, int start, int end, int* path);
void dijkstra_via(AdjacencyMatrix* adj_matrix, int start, int end, int via, int* path);

/* Function declaration for finding the nearest point of interest (POI) given a location. */
int ask_location_POI(AdjacencyMatrix* adj_matrix, long long* location, int* POI);

#endif // PATH_FINDER_H
