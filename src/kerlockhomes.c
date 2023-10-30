#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BPF_OBJECTS 128

struct bpf_object *loaded_objects[MAX_BPF_OBJECTS];
int loaded_count = 0;

void unload_bpf_programs() {
    printf("Unloading BPF programs...\n");
    for (int i = 0; i < loaded_count; ++i) {
        bpf_object__close(loaded_objects[i]);
    }
    exit(0);
}

void signal_handler(int signum) {
    unload_bpf_programs();
}

int load_bpf_program(const char *bpf_file) {
    struct bpf_object *obj;
    int err;

    obj = bpf_object__open_file(bpf_file, NULL);
    if (!obj) {
        fprintf(stderr, "Failed to open BPF object file %s\n", bpf_file);
        return 1;
    }

    err = bpf_object__load(obj);
    if (err) {
        fprintf(stderr, "Failed to load BPF object file %s: %s\n", bpf_file, strerror(-err));
        bpf_object__close(obj);
        return 1;
    }

    if (loaded_count < MAX_BPF_OBJECTS) {
        loaded_objects[loaded_count++] = obj;
    } else {
        fprintf(stderr, "Reached maximum number of loaded BPF objects.\n");
        bpf_object__close(obj);
    }

    printf("Successfully loaded %s\n", bpf_file);
    return 0;
}

void read_trace_pipe() {
	FILE *fp = fopen("/sys/kernel/debug/tracing/trace_pipe", "r");
    if (fp == NULL) {
        perror("Failed to open trace_pipe");
		return;
	}

	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
    }

	fclose(fp);
}


int main() {
    struct dirent *entry;
    DIR *dp = opendir(".");
    struct stat st;

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (dp == NULL) {
        perror("Failed to open directory");
        return 1;
    }

    while ((entry = readdir(dp))) {
        if (stat(entry->d_name, &st) == 0 && S_ISREG(st.st_mode) &&
            strstr(entry->d_name, ".bpf.o") != NULL) {
            file_names[file_count++] = strdup(entry->d_name);
        }
    }

    // Check if the number of files exceeds the maximum limit
    if (file_count > MAX_BPF_OBJECTS) {
        fprintf(stderr, "Error: Too many BPF objects to load. The maximum limit is %d.\n", MAX_BPF_OBJECTS);
        return 1;
    }

    // Sort the file names
    qsort(file_names, file_count, sizeof(char *), compare);

    for (int i = 0; i < file_count; ++i) {
        printf("Loading %s...\n", file_names[i]);
        if (load_bpf_program(file_names[i]) != 0) {
            fprintf(stderr, "Failed to load %s, continuing...\n", file_names[i]);
        }
        free(file_names[i]);
    }

    closedir(dp);

    printf("Press Ctrl+C to unload BPF programs and exit.\n");
    while (1) {
        read_trace_pipe();
        sleep(1);
    }

    return 0;
}
