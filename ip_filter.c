#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <bpf/libbpf.h>

int main() {
    struct bpf_object *obj;
    struct bpf_program *prog;
    int prog_fd;

    obj = bpf_object__open_file("ip_extraction_bpf.o", NULL);
    if (!obj) {
        fprintf(stderr, "Error opening BPF object file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    prog = bpf_object__find_program_by_name(obj, "filter_packet");
    if (!prog) {
        fprintf(stderr, "Error finding BPF program: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // prog_fd = bpf_program__fd(prog);
    // if (prog_fd < 0) {
    //     fprintf(stderr, "Error getting BPF program FD: %s\n", strerror(errno));
    //     return EXIT_FAILURE;
    // }

    if (bpf_prog_load(bpf_program__section_name(prog), BPF_PROG_TYPE_XDP, NULL, 0)) {
        fprintf(stderr, "Error loading BPF program: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("BPF program loaded successfully.\n");

    return EXIT_SUCCESS;
}