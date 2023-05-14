/***********************************/
/* A shared header file for visulize */
/* map by SDL2                       */
/***********************************/

/* include guard				   */
#ifndef PATH_VISUALIZER_H
#define PATH_VISUALIZER_H

/* Function declarations for converting longitude and latitude to x, y coordinates. */
void Mercaor_x_y(double lon, double lat, double* x, double* y);

/* Function declarations for initializing the visualization window. */
void init(AdjacencyMatrix* adj_matrix, int* w_window, int* h_window);

/* Function declarations for quitting the visualization window and free. */
void quit();

/* Function declarations for quitting the visualization window. */
void render(AdjacencyMatrix* adj_matrix, int* path);

/* Function declarations for quitting the visualization window. */
int visualize(AdjacencyMatrix* adj_matrix, int* path);

#endif // PATH_VISUALIZER_H