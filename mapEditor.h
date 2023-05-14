/***********************************/
/* A shared header file for edit   */
/* map                             */
/***********************************/

/* include guard				   */
#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

/* Macro const for indicating successful edit operation. */
#define EDIT_SUCCESSFULLY 0
/* Macro const for indicating successful edit operation. */
#define EXIST 20
/* Macro const for indicating that the element does not exist. */
#define NOT_EXIST 21
/* Maximum length for long long variable. */
#define MAX_LONG_LONG 12
/* Maximum length for long long variable. */
#define MAX_DOUBLE 13


/* Function declarations for performing editing operations on the map. */
int ask_modify_map(AdjacencyMatrix* adj_matrix);
int edit_map(AdjacencyMatrix* adj_matrix);
int edit_edge(AdjacencyMatrix* adj_matrix, long long id, long long from,
    long long to, double length, double veg, double arch, double land, double speedLimit, int POIs[], int len_pois);

/* Function declarations for checking if an element exists in the map. */
int is_point_exist(AdjacencyMatrix* adj_matrix, long long point_id, double lat, double lon, int flag);
int is_edge_exist(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to, int flag);
int is_edge_id_exist(AdjacencyMatrix* adj_matrix, long long link_id);
int is_edge_nodes_exist(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to);
int is_node_exist(AdjacencyMatrix* adj_matrix, long long node_id);
int is_POI_exist(AdjacencyMatrix* adj_matrix, int POI);

/* Function declarations for checking if an element exists in the map. */
int get_input_int(const char* prompt);
long long get_input_ll(const char* prompt);
double get_input_double(const char* prompt);
int get_pois_from_input(int* len_poi, int pois[]);

/* Function declarations for checking input format. */
int check_lld(char* s);
int check_double(const char* s);

/* Function declaration for writing the map to a file. */
int write_map(AdjacencyMatrix* adj_matrix, char* new_map_name);

/* end of include guard			   */
#endif /* MAP_EDITOR_H */