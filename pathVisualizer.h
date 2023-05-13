
#define M_PI 3.1415926

void Mercaor_x_y(double lon, double lat, double* x, double* y);
void init(AdjacencyMatrix* adj_matrix, int* w_window, int* h_window);
void quit();
void render(AdjacencyMatrix* adj_matrix, int* path);
int visualize(AdjacencyMatrix* adj_matrix, int* path);
