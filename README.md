Additional libbpf comes from here
https://github.com/libbpf/libbpf

Compiling make performs following commands:

1) compile bpf module
clang -O2 -target bpf -c ip_extraction_bpf.c -o ip_extraction_bpf.o
2) compile main logic
clang -o ip_filter ip_filter.c -lbpf -lelf
3) set network permissions
sudo setcap cap_net_raw+ep ip_filter
4) launch program
./ip_filter

Test folder contains logic to send packets TO server with ip_filter setup with extansion to use multiple proxies to test accept/deny status based on different sender ipv4 addresses