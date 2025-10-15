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

int ft_atoi(const char *str) 
{
    int result = 0;
    int sign = 1;
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') 
    { i++; }
    if (str[i] == '-') 
    { 
        sign = -1; i++; 
    } 
    else if (str[i] == '+') 
    { 
        i++; 
    }
    while (str[i] >= '0' && str[i] <= '9') 
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return sign * result;
}

char* ft_strcpy(char *dest, const char *src) 
{
    char *original_dest = dest;
    while ((*dest++ = *src++));
    return original_dest;
}

int ft_strlen(const char *str) 
{
    int length = 0;
    while (str[length] != '\0') 
    { length++; }
    return length;
}

char* ft_strtok(char *str, const char *delimiters) 
{
    static char *next_token = NULL;
    char *token_start;
    if (str == NULL) 
    { str = next_token; }
    if (str == NULL) 
    { return NULL; }
    while (*str != '\0') 
    {
        int is_delim = 0;
        for (int i = 0; delimiters[i] != '\0'; i++) 
        {
            if (*str == delimiters[i]) 
            { is_delim = 1; break; }
        }
        if (!is_delim) 
        { break; }
        str++;
    }
    if (*str == '\0') 
    { next_token = NULL; return NULL; }
    token_start = str;
    while (*str != '\0') 
    {
        int is_delim = 0;
        for (int i = 0; delimiters[i] != '\0'; i++) 
        {
            if (*str == delimiters[i]) 
            { is_delim = 1; break; }
        }
        if (is_delim) 
        { *str = '\0'; next_token = str + 1; return token_start; }
        str++;
    }
    next_token = NULL;
    return token_start;
}

void free_map(Map *map) 
{
    if (map && map->grid) 
    {
        for (int i = 0; i < map->rows; i++) 
        {
            free(map->grid[i]);
        }
        free(map->grid);
        map->grid = NULL;
    }
}

int parse_first_line(char *line, Map *map) 
{
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
    
    if (map->empty == map->obstacle || map->empty == map->full || map->obstacle == map->full) 
    {
        return 0;
    }
    return 1;
}

int validate_map_line(char *line, Map *map, int expected_length) 
{
    int len = ft_strlen(line);
    if (len > 0 && line[len - 1] == '\n') 
    {
        line[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return -1;
    
    if (expected_length != -1 && len != expected_length) return -1;
    
    for (int i = 0; i < len; i++) 
    {
        if (line[i] != map->empty && line[i] != map->obstacle) 
        {
            return -1;
        }
    }
    return len;
}

Map* read_map(FILE *file) 
{
    Map *map = calloc(1, sizeof(Map));
    if (!map) return NULL;
    
    char *line = NULL;
    size_t line_cap = 0;
    
    if (getline(&line, &line_cap, file) <= 0) 
    {
        free(map); 
        free(line);
        return NULL;
    }
    
    if (!parse_first_line(line, map)) 
    {
        free(map); 
        free(line);
        return NULL;
    }
    
    map->grid = malloc(map->rows * sizeof(char*));
    if (!map->grid) 
    {
        free(map); 
        free(line);
        return NULL;
    }
    
    for (int k = 0; k < map->rows; k++) 
    {
        map->grid[k] = NULL;
    }
    
    int expected_length = -1;
    for (int i = 0; i < map->rows; i++) 
    {
        if (getline(&line, &line_cap, file) <= 0) 
        {
            free_map(map); 
            free(map); 
            free(line);
            return NULL;
        }
        
        int current_length = validate_map_line(line, map, expected_length);
        if (current_length == -1) 
        {
            free_map(map); 
            free(map); 
            free(line);
            return NULL;
        }
        
        if (expected_length == -1) 
        {
            expected_length = current_length;
        }
        
        map->grid[i] = malloc((expected_length + 1) * sizeof(char));
        if (!map->grid[i]) 
        {
            free_map(map); 
            free(map); 
            free(line);
            return NULL;
        }
        
        ft_strcpy(map->grid[i], line);
    }
    
    free(line);
    return map;
}

int get_cols(Map *map) 
{
    if (!map || !map->grid || !map->grid[0]) return 0;
    return ft_strlen(map->grid[0]);
}

int min(int a, int b, int c) 
{
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

Square find_biggest_square(Map *map) 
{
    Square best = {0, 0, 0};
    int cols = get_cols(map);
    if (map->rows == 0 || cols == 0) return best;

    int **dp = malloc(map->rows * sizeof(int*));
    if (!dp) return best;
    for (int i = 0; i < map->rows; i++) 
    {
        dp[i] = malloc(cols * sizeof(int));
        if (!dp[i]) 
        {
            for (int k = 0; k < i; k++) free(dp[k]);
            free(dp);
            return best;
        }
    }

    for (int i = 0; i < map->rows; i++) 
    {
        for (int j = 0; j < cols; j++) 
        {
            if (map->grid[i][j] == map->obstacle) 
            {
                dp[i][j] = 0;
            } else 
            {
                if (i == 0 || j == 0) 
                {
                    dp[i][j] = 1;
                } else 
                {
                    dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]);
                }
            }
            if (dp[i][j] > best.size) 
            {
                best.size = dp[i][j];
                best.row = i - best.size + 1;
                best.col = j - best.size + 1;
            }
        }
    }

    for (int i = 0; i < map->rows; i++) free(dp[i]);
    free(dp);

    return best;
}

void place_square(Map *map, Square square) 
{
    for (int i = square.row; i < square.row + square.size; i++) 
    {
        for (int j = square.col; j < square.col + square.size; j++) 
        {
            map->grid[i][j] = map->full;
        }
    }
}

void print_map(Map *map) 
{
    for (int i = 0; i < map->rows; i++) 
    {
        printf("%s\n", map->grid[i]);
    }
}

void process_map(FILE *file) 
{
    Map *map = read_map(file);
    if (!map) 
    {
        fprintf(stderr, "map error\n");
        return;
    }
    
    Square square = find_biggest_square(map);
    if (square.size > 0) 
    {
        place_square(map, square);
    }
    
    print_map(map);
    free_map(map);
    free(map);
}

int main(int argc, char **argv) 
{
    if (argc == 1) 
    {
        process_map(stdin);
    } else 
    {
        for (int i = 1; i < argc; i++) 
        {
            FILE *file = fopen(argv[i], "r");
            if (!file) 
            {
                fprintf(stderr, "map error\n");
                continue;
            }
            
            process_map(file);
            fclose(file);
            
            if (i < argc - 1) 
            {
                printf("\n");
            }
        }
    }
    return 0;
}