
int ask_modify_map(AdjacencyList* adj_list);
int edit_map(AdjacencyList* adj_list);
int edit_edge(AdjacencyList* adj_list, long long link_id, long long node_id_from, long long node_id_to, double length);
int is_point_exist(AdjacencyList* adj_list, long long point_id, double lat, double lon, int flag);
int is_edge_exist(AdjacencyList* adj_list, long long link_id, long long node_id_from, long long node_id_to, int flag);
int is_edge_id_exist(AdjacencyList* adj_list, long long link_id);
int is_edge_nodes_exist(AdjacencyList* adj_list, long long link_id, long long node_id_from, long long node_id_to);