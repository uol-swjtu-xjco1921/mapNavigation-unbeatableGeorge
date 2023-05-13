#ifndef PATH_FINDER_H_INCLUDED
#define PATH_FINDER_H_INCLUDED

#define INF 9999

int ask_find_path(AdjacencyMatrix* adj_matrix, int* path);
double dijkstra(AdjacencyMatrix* adj_matrix, int start, int end, int* path);
int ask_location_POI(AdjacencyMatrix* adj_matrix, long long* location, int* POI);
void dijkstra_via(AdjacencyMatrix* adj_matrix, int start, int end, int via, int* path);

#endif // PATH_FINDER_H_INCLUDED
