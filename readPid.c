#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <ctype.h>

// Helper function to check if a struct dirent from /proc is a PID folder.
struct Answer
{
    int pid;
    char processName[100];
    long unsigned uTime;
    
};
int is_pid_folder(const struct dirent *entry) {
    const char *p;

    for (p = entry->d_name; *p; p++) {
        if (!isdigit(*p))
            return 0;
    }

    return 1;
}

int main(void) {
    DIR *procdir;
    FILE *fp;
    struct dirent *entry;
    char path[256 + 5 + 5]; // d_name + /proc + /stat
    int pid;
    unsigned long maj_faults;
    int N = 10;
    int counter=0;
    struct Answer ans[N];

    // Open /proc directory.
    procdir = opendir("/proc");
    if (!procdir) {
        perror("opendir failed");
        return 1;
    }

    // Iterate through all files and folders of /proc.
    while ((entry = readdir(procdir))) {
        // Skip anything that is not a PID folder.
        if (!is_pid_folder(entry))
            continue;

        // Try to open /proc/<PID>/stat.
        snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);
        fp = fopen(path, "r");

        if (!fp) {
            perror(path);
            continue;
        }

        // Get PID, process name and number of faults.
        fscanf(fp, "%d %s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %lu",
            &pid, &path, &maj_faults
        );

        ans[counter].pid=pid;
        ans[counter].processName=path;
        ans[counter].uTime=maj_faults;
        counter++;
        // Pretty print.
        // printf("%5d %-20s: %lu\n", pid, path, maj_faults);

        fclose(fp);
    }

    for(int i=0;i<10;i++) {
        printf("%5d %-20s: %lu\n", ans[i].pid, ans[i].processName, ans[i].uTime);
    }

    closedir(procdir);
    return 0;
}