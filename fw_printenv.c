#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define VERSION "1.0"
#define DEFAULT_CONFIG "/etc/fw_env.config"
#define DEFAULT_LOCK "/var/lock"
#define DEFAULT_ENV_FILE ".fw_env" // Just the filename; relative to home
#define MAX_PATH_LENGTH 256
#define MAX_LINE_LENGTH 256
#define MAX_KEY_LENGTH 64
#define MAX_VALUE_LENGTH 192  // Adjusted for likely longer values
#define MAX_VARIABLES 100

// Structure to hold environment variables
typedef struct {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
} EnvironmentVariable;

// Global array to store environment variables
EnvironmentVariable env_data[MAX_VARIABLES];
int num_variables = 0;

// Function to get the full environment file path
char *get_env_file_path() {
    char *user_home = getenv("HOME");  // Get the HOME environment variable
    if (user_home == NULL) {
        fprintf(stderr, "Error: HOME environment variable not set.\n");
        return NULL;
    }

    char *file_path = (char *)malloc(MAX_PATH_LENGTH);
    if (file_path == NULL) {
        perror("malloc");
        return NULL;
    }

    snprintf(file_path, MAX_PATH_LENGTH, "%s/%s", user_home, DEFAULT_ENV_FILE);
    file_path[MAX_PATH_LENGTH - 1] = '\0'; // Ensure null termination

    return file_path;
}


// Function to load environment variables from the file
int load_env_data(const char *filename) {
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    char *key, *value;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open environment file: %s\n", filename);
        return -1;
    }

    num_variables = 0;  // Reset the variable count

    while (fgets(line, sizeof(line), fp) != NULL && num_variables < MAX_VARIABLES) {
         // Remove leading/trailing whitespace
        char *start = line;
        while (*start == ' ' || *start == '\t') start++;
        char *end = start + strlen(start) - 1;
        while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) end--;
        *(end + 1) = '\0'; // Null-terminate the trimmed string

        if (*start == '\0' || *start == '#') continue; // Skip empty and comment lines

        key = strtok(start, "=");
        if (key == NULL) continue;  // Skip lines without '='

        value = strtok(NULL, "=");
        if (value == NULL) continue;  // Skip lines without value

        // Copy the key and value to the environment data
        strncpy(env_data[num_variables].key, key, MAX_KEY_LENGTH - 1);
        env_data[num_variables].key[MAX_KEY_LENGTH - 1] = '\0'; // Ensure null termination

        strncpy(env_data[num_variables].value, value, MAX_VALUE_LENGTH - 1);
        env_data[num_variables].value[MAX_VALUE_LENGTH - 1] = '\0'; // Ensure null termination
        num_variables++;
    }

    fclose(fp);
    return 0;
}

// Function to print all environment variables
void print_all_variables(bool noheader) {
    for (int i = 0; i < num_variables; i++) {
        if (noheader) {
            printf("%s\n", env_data[i].value);
        } else {
            printf("%s=%s\n", env_data[i].key, env_data[i].value);
        }
    }
}

// Function to print specific environment variables
void print_variables(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {  // Start from 1
        bool found = false;
        for (int j = 0; j < num_variables; j++) {
            if (strcmp(argv[i], env_data[j].key) == 0) {
                printf("%s=%s\n", env_data[j].key, env_data[j].value);
                found = true;
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "Error: Variable '%s' not found in environment.\n", argv[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    bool noheader = false;
    const char *config_file = DEFAULT_CONFIG;
    const char *lock_file = DEFAULT_LOCK;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Usage: fw_printenv [OPTIONS]... [VARIABLE]...\n");
            printf("Print variables from U-Boot environment\n\n");
            printf(" -h, --help           print this help.\n");
            printf(" -v, --version        display version\n");
            printf(" -c, --config         configuration file, default:%s\n", DEFAULT_CONFIG);
            printf(" -n, --noheader       do not repeat variable name in output\n");
            printf(" -l, --lock           lock node, default:%s\n", DEFAULT_LOCK);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("fw_printenv version %s\n", VERSION);
            return 0;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) {
            if (i + 1 < argc) {
                config_file = argv[++i];
            } else {
                fprintf(stderr, "Error: Missing argument for --config\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--noheader") == 0) {
            noheader = true;
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--lock") == 0) {
            if (i + 1 < argc) {
                lock_file = argv[++i];
            } else {
                fprintf(stderr, "Error: Missing argument for --lock\n");
                return 1;
            }
        }
    }

    // Get the environment file path
    char *env_file_path = get_env_file_path();
    if (env_file_path == NULL) {
        return 1;
    }

    // Load environment data from file
    if (load_env_data(env_file_path) != 0) {
        free(env_file_path); // Free memory before exiting
        return 1;
    }

    // Print variables
    if (argc == 1) {
        print_all_variables(noheader);
    } else {
        print_variables(argc, argv);
    }

    free(env_file_path);  // Free the allocated memory
    return 0;
}
