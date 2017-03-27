/* Constants */
#define NUM_ENTRIES 4

/* Struct to store the entries of a process */
typedef struct {
    int time_created;
    int process_id;
    int memory_size;
    int job_time;
} process_t;

/* Struct represents a disk to store a list of processes */
typedef struct {
    process_t *list;
    int num_processes;
} disk_t;

/* Struct represents a main memory */
typedef struct {
    int memsize;
} memory_t;

/* Singleton struct for disk */
typedef struct {
    disk_t *disk;
} disk_singleton_t;

/* Singleton struct for memory */
typedef struct {
    memory_t *memory;
} memory_singleton_t;

void simulate(char*, char*, int, int);

disk_singleton_t* get_disk_instance();

memory_singleton_t* get_memory_instance();

disk_t *load_processes();
