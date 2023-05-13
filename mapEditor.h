
#define EDIT_SUCCESSFULLY 0
#define EXIST 20
#define NOT_EXIST 21

#define MAX_LONG_LONG 12
#define MAX_DOUBLE 13

int ask_modify_map(AdjacencyMatrix* adj_matrix);
int edit_map(AdjacencyMatrix* adj_matrix);
int edit_edge(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to, double length);
int is_point_exist(AdjacencyMatrix* adj_matrix, long long point_id, double lat, double lon, int flag);
int is_edge_exist(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to, int flag);
int is_edge_id_exist(AdjacencyMatrix* adj_matrix, long long link_id);
int is_edge_nodes_exist(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to);
int is_node_exist(AdjacencyMatrix* adj_matrix, long long node_id);
int check_lld(char* s);
int check_double(const char* s);