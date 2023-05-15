// This program includes header files for standard input/output streams,
// commonly used utility libraries, and custom implementation files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h" // Header file for hash table implementation
#include "mapAdjMatrix.h" // Header file for adjacency matrix graph representation
#include "mapReader.h" // Header file for reading map data
#include "mapError.h" // Header file for error handling in map operations.

// the main function in read map module
int read_map(AdjacencyMatrix* adj_matrix)
{
    printf("\nWelcome to the Map Navigator! Here is an overview of the entire process:\n");
    printf("1. Choose your map\n");
    printf("2. Modify map properties (this modification will be added to the map file)\n");
    printf("3. Enter location information\n");
    printf("This will allow you to see a visualization of the map that meets your requirements!\n\n");

    while (1)
    {
        // Prompt the user to enter the map filename and read from standard input
        printf("Please enter the map filename: ");
        char* filename = (char*)malloc(100 * sizeof(char));
        if (fgets(filename, 100, stdin) == NULL)
        {
            // If reading fails, continue to prompt the user
            printf("Failed to read user input, please try again\n");
            free(filename);
            continue;
        }

        // Check if the filename length exceeds the maximum allowed length
        int len = strlen(filename);
        if (len == 100)
        {
            printf("File name too long! Retype!\n");
            free(filename);
            continue;
        }

        // Remove the trailing newline character
        if (filename[len - 1] == '\n')
        {
            filename[len - 1] = '\0';
        }

        // Read the data file and transform it into an adjacency matrix
        int result = read_points(filename, adj_matrix);
        if (result == ERROR_BAD_FILE)
        {
            // If opening the file fails, prompt the user to re-enter the filename
            printf("Failed to open file! Please try again\n");
            free(filename);
            continue;
        }

        // Free the memory allocated for the filename string and print success message
        result = read_edges(filename, adj_matrix);
        result = read_geoms(filename, adj_matrix);
        free(filename);
        printf("\nMap read successfully!\n");
        return FUNCTION_SUCCESS;
    }
}


int read_points(char* filename, AdjacencyMatrix* adj_matrix)
{
    // Define variables for parsing the data file
    char line[512];
    int count = 0;

    // Open the data file
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return ERROR_BAD_FILE;
    }
    // read points bonding
    if (fgets(line, sizeof(line), fp) != NULL)
    {
        // valid bounding: <bounding minLat=53.801600 minLon=-1.564000 maxLat=53.811000 maxLon=-1.543100 /bounding>
        if (line[0] == '<' && line[1] == 'b' && line[2] == 'o' && line[3] == 'u' && line[4] == 'n')
        {
            // Use sscanf() to parse the line string
            if (sscanf(line, "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>",
                &adj_matrix->bounding.minLat, &adj_matrix->bounding.minLon, &adj_matrix->bounding.maxLat, &adj_matrix->bounding.maxLon) != 4)
            {
                free_adjacency_matrix(adj_matrix);
                fclose(fp);
                fprintf(stderr, "Error: Failed to parse node data in %d line\n", count);
                exit(ERROR_DATA_READ_FAILED);
            }
        }
        memset(line, 0, sizeof(line));
    }

    // Read each line from the data file
    while (fgets(line, sizeof(line), fp) != NULL)
    { // Read the contents of the data file line by line
        if (line[0] == '<' && line[1] == 'n' && line[2] == 'o' && line[3] == 'd' && line[4] == 'e')
        {
            // If the line contains information about a point
            long long id = -1;
            double lat = -1;
            double lon = -1;

            // Use sscanf() to parse the line string
            if (sscanf(line, "<node id=%lld lat=%lf lon=%lf /node>", &id, &lat, &lon) != 3)
            {
                free_adjacency_matrix(adj_matrix);
                fclose(fp);
                fprintf(stderr, "Error: Failed to parse node data in %d line\n", count);
                exit(ERROR_DATA_READ_FAILED);
            }

            // Add the point to the adjacency matrix
            add_point(adj_matrix, id, lat, lon);
        }
        count++;
        memset(line, 0, sizeof(line));
    }
    build_matrix(adj_matrix);
    // Close the data file
    fclose(fp);
    return FUNCTION_SUCCESS;
}


int read_edges(char* filename, AdjacencyMatrix* adj_matrix)
{
    // Define variables for parsing the data file
    char line[512];
    int count = 0;

    // Open the data file
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return ERROR_BAD_FILE;
    }

    while (fgets(line, sizeof(line), fp))
    {   // Read the contents of the data file line by line
        if (line[0] == '<' && line[1] == 'l' && line[2] == 'i' && line[3] == 'n' && line[4] == 'k')
        {
            // If the line contains information about an edge
            long long link_id = -1;
            long long node_id_from = -1;
            long long node_id_to = -1;
            int way = -1;
            double length = -1;
            double veg = -1;
            double arch = -1;
            double land = -1;
            double speedLimit = 4.0;
            if (sscanf(line, "<link id=%lld node=%lld node=%lld way=%d length=%lf veg=%lf arch=%lf land=%lf speedLimit=%lf",
                &link_id, &node_id_from, &node_id_to, &way, &length, &veg, &arch, &land, &speedLimit) != 9)
            {
                if (sscanf(line, "<link id=%lld node=%lld node=%lld way=%d length=%lf veg=%lf arch=%lf land=%lf",
                    &link_id, &node_id_from, &node_id_to, &way, &length, &veg, &arch, &land) != 8)
                {
                    free_adjacency_matrix(adj_matrix);
                    fclose(fp);
                    fprintf(stderr, "Error: Failed to parse link data in %d\n", count);
                    exit(ERROR_DATA_READ_FAILED);
                }
            }

            // locate the string "POI="
            char poi_str[50]; // Used to store POI string
            int POIs[20] = { -1 };
            int len_poi = 0;
            
            // Locate the first POI attribute and read its string
            char* poi_ptr = strstr(line, "POI=");
            if (poi_ptr != NULL) 
            {
                // Found a POI attribute, parse its string
                if (sscanf(poi_ptr, "POI=%[^;];", poi_str) == 1) 
                {
                    // Successfully parsed the number list in the POI string, separated by commas
                    char* token = strtok(poi_str, ",");
                    while (token != NULL) 
                    {
                        // Parse each POI number and process it
                        int poi;
                        if (strlen(token) > 0 && sscanf(token, "%d", &poi) == 1)
                        {
                            POIs[len_poi] = poi;
                            len_poi++;
                        }
                        token = strtok(NULL, ",");
                    }
                }
            }
            else 
            {
                // POI attribute not found, need to handle exceptional cases
                free_adjacency_matrix(adj_matrix);
                fclose(fp);
                fprintf(stderr, "Warning: POI attribute not found in `%s`.\n", line);
            }
            add_edge_to_adjacency_matrix(adj_matrix, link_id, node_id_from, node_id_to, length, veg, arch, land, speedLimit, POIs, len_poi); // 将该link添加到邻接表中
        }
        count++;
    }

    // Close the data file
    fclose(fp);
    return FUNCTION_SUCCESS;
}


int read_geoms(char* filename, AdjacencyMatrix* adj_matrix)
{
    // Define variables for parsing the data file
    char line[512];
    int count = 0;

    // Open the data file
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return ERROR_BAD_FILE;
    }

    while (fgets(line, sizeof(line), fp))
    {   // Read the contents of the data file line by line
        if (line[0] == '<' && line[1] == 'g' && line[2] == 'e' && line[3] == 'o' && line[4] == 'm')
        {
            // If the line contains information about an edge
            long long geom_id = -1;
            if (sscanf(line, "<geom id=%lld", &geom_id) != 1)
            {
                free_adjacency_matrix(adj_matrix);
                fclose(fp);
                fprintf(stderr, "Error: Failed to parse geom data in %d\n", count);
                exit(ERROR_DATA_READ_FAILED);
            }

            // Add this geoms to the adjacency matrix
            char tokens[50][20] = { {0} };
            char delim[] = " "; // Separator
            char* token = strtok(line, delim); // Get the first substring after splitting
            int len = 0; // Counter
            while (token != NULL) {
                // Copy the substring to the tokens array
                strncpy(tokens[len], token, sizeof(tokens[len]));

                // Add an end character at the end of the string
                tokens[len][strlen(tokens[len])] = '\0';

                // Get the next substring
                token = strtok(NULL, delim);
                ++len;
            }
            add_geoms_to_adjacency_matrix(adj_matrix, geom_id, len, tokens);
        }
        count++;
    }

    // Close the data file
    fclose(fp);
    return FUNCTION_SUCCESS;
}
