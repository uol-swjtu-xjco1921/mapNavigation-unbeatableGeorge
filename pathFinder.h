#ifndef PATH_FINDER_H_INCLUDED
#define PATH_FINDER_H_INCLUDED

#define INF 9999

void dijkstra(AdjacencyMatrix* adj_matrix, int start, int end);
int ask_find_path(AdjacencyMatrix* adj_matrix);

#endif // PATH_FINDER_H_INCLUDED
