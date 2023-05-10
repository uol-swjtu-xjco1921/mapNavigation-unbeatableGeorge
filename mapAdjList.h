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
    int num_edit;
    int** adj; // each element stores the index of edges
    int* adj_len; // array of the length of the adjacency lists
    HashTable* point_hash;  // hash table -- mapping from point ID to index
} AdjacencyList;

void init_adjacency_list(AdjacencyList* adj_list);  // initialize the adjacency list
void add_point_to_adjacency_list(AdjacencyList* adj_list, long long id, double lat, double lon); // add a point to the adjacency list
void add_edge_to_adjacency_list(AdjacencyList* adj_list, long long id, long long from, long long to, double length); // add a edge to the adjacency list
void print_adjacency_list(AdjacencyList* adj_list);  // print the adjacency list
void free_adjacency_list(AdjacencyList* adj_list); // free the memory used by the adjacency list




///* Define data structures */
//typedef struct point_t
//{ // structure type for points
//    long long id; // ID of the point
//} Point;
//
//typedef struct edge_t
//{ // structure type for edges
//    long long id; // ID of the edge
//    long long from, to;  // start and end points of the edge
//    double length; // length of the edge
//} Edge;
//
//typedef struct adjacency_list_t
//{ // structure type for adjacency lists
//    int num_points; // number of points in the list
//    Point* points; // array of points
//    int num_edges;  // number of edges in the list
//    Edge* edges; // array of edges, because a point can have at most two edges connected to it
//    int** adj; // each element stores the index of edges
//    int* adj_len; // array of the length of the adjacency lists
//} AdjacencyList;
//
//我给你依次说上面的数据是什么意思
//Point结构体的用法是 point->id = 1533443 是一个较大的数字
//
//Edge 结构体的用法是 edge->id = 7551621 也是一个较大的数字
//                    edge->from = 135643   point结构体对应的id值
//                    edge->to = 124553    point结构体对应的id值
//                    edge->length = 13.5
//
//AdjacencyList用法是 num_points代表了点的个数
//                    points代表了一个一维数组,长度为num_points,存的数据为Point结构体
//                    num_edges代表了边的个数
//                    edges代表了一个一维数组,长度为num_edges,存的数据为Edge结构体
//                    adj是一个很重要的邻接表其全部存的edges的下标，和其他邻接表不同，其索引下标和points下标相同其实也就是与之points相同下标的对应的point的邻接边的数组
//                    adj_len则是adj每个元素的长度 其实也就是与之points相同下标的对应的point的邻接边个数