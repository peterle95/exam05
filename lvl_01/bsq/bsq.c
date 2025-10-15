#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int rows;
    char empty;
    char obstacle;
    char full;
    char **grid;
} Map;

typedef struct {
    int size;
    int row;
    int col;
} Square;

int ft_atoi(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;
    
    // Skip whitespace
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
        i++;
    }
    
    // Handle sign
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }
    
    // Convert digits
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    
    return sign * result;
}

char* ft_strcpy(char *dest, const char *src) {
    char *original_dest = dest;
    
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    
    return original_dest;
}

int ft_strlen(const char *str) {
    int length = 0;
    
    while (str[length] != '\0') {
        length++;
    }
    
    return length;
}

char* ft_strtok(char *str, const char *delimiters) {
    static char *next_token = NULL;
    char *token_start;
    
    // If str is NULL, continue from where we left off
    if (str == NULL) {
        str = next_token;
    }
    
    // If no string to process, return NULL
    if (str == NULL) {
        return NULL;
    }
    
    // Skip leading delimiters
    while (*str != '\0') {
        int is_delim = 0;
        for (int i = 0; delimiters[i] != '\0'; i++) {
            if (*str == delimiters[i]) {
                is_delim = 1;
                break;
            }
        }
        if (!is_delim) {
            break;
        }
        str++;
    }
    
    // If we reached end of string, no token found
    if (*str == '\0') {
        next_token = NULL;
        return NULL;
    }
    
    // Mark the start of the token
    token_start = str;
    
    // Find the end of the token
    while (*str != '\0') {
        int is_delim = 0;
        for (int i = 0; delimiters[i] != '\0'; i++) {
            if (*str == delimiters[i]) {
                is_delim = 1;
                break;
            }
        }
        if (is_delim) {
            *str = '\0';
            next_token = str + 1;
            return token_start;
        }
        str++;
    }
    
    // Reached end of string
    next_token = NULL;
    return token_start;
}

void free_map(Map *map) {
    if (map && map->grid) {
        for (int i = 0; i < map->rows; i++) {
            free(map->grid[i]);
        }
        free(map->grid);
        map->grid = NULL;
    }
}

int parse_first_line(char *line, Map *map) {
    char *token = ft_strtok(line, " \t\n");
    if (!token) return 0;
    map->rows = ft_atoi(token);
    if (map->rows <= 0) return 0;
    
    token = ft_strtok(NULL, " \t\n");
    if (!token || ft_strlen(token) != 1) return 0;
    map->empty = token[0];
    
    token = ft_strtok(NULL, " \t\n");
    if (!token || ft_strlen(token) != 1) return 0;
    map->obstacle = token[0];
    
    token = ft_strtok(NULL, " \t\n");
    if (!token || ft_strlen(token) != 1) return 0;
    map->full = token[0];
    
    // Check that all characters are different
    if (map->empty == map->obstacle || map->empty == map->full 
        || map->obstacle == map->full) {
        return 0;
    }
    
    return 1;
}

int validate_map_line(char *line, Map *map, int expected_length) {
    int len = ft_strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    
    if (expected_length == -1) {
        if (len <= 0) return 0;
        return len;
    }
    
    if (len != expected_length) return 0;
    
    for (int i = 0; i < len; i++) {
        if (line[i] != map->empty && line[i] != map->obstacle) {
            return 0;
        }
    }
    
    return 1;
}

Map* read_map(FILE *file) {
    Map *map = calloc(1, sizeof(Map));
    if (!map) return NULL;
    
    char *line = NULL;
    size_t line_cap = 0;
    ssize_t line_len;
    
    // Read first line
    line_len = getline(&line, &line_cap, file);
    if (line_len <= 0) {
        free(map);
        free(line);
        return NULL;
    }
    
    if (!parse_first_line(line, map)) {
        free(map);
        free(line);
        return NULL;
    }
    
    // Allocate grid
    map->grid = malloc(map->rows * sizeof(char*));
    if (!map->grid) {
        free(map);
        free(line);
        return NULL;
    }
    
    int expected_length = -1;
    int actual_rows = 0;
    
    // Read map lines
    for (int i = 0; i < map->rows; i++) {
        line_len = getline(&line, &line_cap, file);
        if (line_len <= 0) {
            free_map(map);
            free(map);
            free(line);
            return NULL;
        }
        
        if (expected_length == -1) {
            expected_length = validate_map_line(line, map, -1);
            if (expected_length <= 0) {
                free_map(map);
                free(map);
                free(line);
                return NULL;
            }
        } else {
            if (!validate_map_line(line, map, expected_length)) {
                free_map(map);
                free(map);
                free(line);
                return NULL;
            }
        }
        
        // Remove newline if present
        int len = ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        map->grid[i] = malloc((expected_length + 1) * sizeof(char));
        if (!map->grid[i]) {
            free_map(map);
            free(map);
            free(line);
            return NULL;
        }
        ft_strcpy(map->grid[i], line);
        actual_rows++;
    }
    
    free(line);
    
    if (actual_rows != map->rows) {
        free_map(map);
        free(map);
        return NULL;
    }
    
    return map;
}

int get_cols(Map *map) {
    if (!map || !map->grid || !map->grid[0]) return 0;
    return ft_strlen(map->grid[0]);
}

int can_place_square(Map *map, int row, int col, int size) {
    int cols = get_cols(map);
    
    if (row + size > map->rows || col + size > cols) {
        return 0;
    }
    
    for (int i = row; i < row + size; i++) {
        for (int j = col; j < col + size; j++) {
            if (map->grid[i][j] == map->obstacle) {
                return 0;
            }
        }
    }
    
    return 1;
}

Square find_biggest_square(Map *map) {
    Square best = {0, 0, 0};
    int cols = get_cols(map);
    
    for (int size = 1; size <= map->rows && size <= cols; size++) {
        for (int row = 0; row <= map->rows - size; row++) {
            for (int col = 0; col <= cols - size; col++) {
                if (can_place_square(map, row, col, size)) {
                    if (size > best.size) {
                        best.size = size;
                        best.row = row;
                        best.col = col;
                    }
                }
            }
        }
    }
    
    return best;
}

void place_square(Map *map, Square square) {
    for (int i = square.row; i < square.row + square.size; i++) {
        for (int j = square.col; j < square.col + square.size; j++) {
            map->grid[i][j] = map->full;
        }
    }
}

void print_map(Map *map) {
    for (int i = 0; i < map->rows; i++) {
        printf("%s\n", map->grid[i]);
    }
}

void process_map(FILE *file) {
    Map *map = read_map(file);
    if (!map) {
        fprintf(stderr, "map error\n");
        return;
    }
    
    Square square = find_biggest_square(map);
    if (square.size > 0) {
        place_square(map, square);
    }
    
    print_map(map);
    free_map(map);
    free(map);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        // Read from stdin
        process_map(stdin);
    } else {
        // Process each file
        for (int i = 1; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (!file) {
                fprintf(stderr, "map error\n");
                continue;
            }
            
            process_map(file);
            fclose(file);
            
            if (i < argc - 1) {
                printf("\n");
            }
        }
    }
    
    return 0;
}