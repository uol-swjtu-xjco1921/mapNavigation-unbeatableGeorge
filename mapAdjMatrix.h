/* Define data structures */

#define INF 9999

typedef struct bounding_t
{ // structure type for points
    double minLat, minLon, maxLat, maxLon; // latitude and longitude of the point
} Bounding;

typedef struct point_t
{ // structure type for points
    long long id; // ID of the point
    double lat, lon; // latitude and longitude of the point
} Point;

typedef struct edge_t
{ // structure type for edges
    long long id; // ID of the edge
    long long from, to;  // start and end points of the edge
    double length; // length of the edge
    double veg, arch, land, speedLimit; // other attri of the edge
    int* pois;
    int num_pois;
} Edge;

typedef struct geom_t
{
    long long id;
    long long* nodes;
    int len_nodes;
}Geom;

typedef struct adjacency_matrix_t
{ // structure type for adjacency matrixs
    Bounding bounding;
    int num_points; // number of points in the matrix
    Point* points; // array of points
    int num_edges;  // number of edges in the matrix
    Edge* edges; // array of edges, because a point can have at most two edges connected to it
    int num_geoms;
    Geom* geoms;
    int num_edit;
    double** adj; // the adj_matrix
    HashTable* point_hash;  // hash table -- mapping from point ID to index
} AdjacencyMatrix;

void init_adjacency_matrix(AdjacencyMatrix* adj_matrix);  // initialize the adjacency matrix
void add_point_to_adjacency_matrix(AdjacencyMatrix* adj_matrix, long long id, double lat, double lon); // add a point to the adjacency matrix
void add_edge_to_adjacency_matrix(AdjacencyMatrix* adj_matrix, long long id, long long from,
    long long to, double length, double veg, double arch, double land, double speedLimit, int POIs[], int len_pois); // add a edge to the adjacency matrix
void add_geoms_to_adjacency_matrix(AdjacencyMatrix* adj_matrix, long long geom_id, int len, char tokens[][20]);
void free_adjacency_matrix(AdjacencyMatrix* adj_matrix); // free the memory used by the adjacency matrix