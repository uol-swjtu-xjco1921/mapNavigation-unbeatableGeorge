/* Define data structures */
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
} Edge;

typedef struct adjacency_list_t
{ // structure type for adjacency lists
    int num_points; // number of points in the list
    Point* points; // array of points
    int num_edges;  // number of edges in the list
    Edge* edges; // array of edges, because a point can have at most two edges connected to it
    int** adj; // each element stores the index of edges
    int* adj_len; // array of the length of the adjacency lists
    HashTable* point_hash;  // hash table -- mapping from point ID to index
} AdjacencyList;

void init_adjacency_list(AdjacencyList* adj_list);  // initialize the adjacency list
void add_point_to_adjacency_list(AdjacencyList* adj_list, long long id, double lat, double lon); // add a point to the adjacency list
void add_edge_to_adjacency_list(AdjacencyList* adj_list, long long id, long long from, long long to, double length); // add a edge to the adjacency list
void print_adjacency_list(AdjacencyList* adj_list);  // print the adjacency list
void free_adjacency_list(AdjacencyList* adj_list); // free the memory used by the adjacency list
