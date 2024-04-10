-include config.mk

CC := clang
CFLAGS := -O2 -target bpf
LDFLAGS :=  -lbpf -lelf

BPF_SRC := ip_extraction_bpf.c
C_SRC := ip_filter.c

BPF_OBJ := $(BPF_SRC:.c=.o)

.PHONY: all clean

all: $(BPF_OBJ) ip_filter
	@echo "Compilation successful"

# Compile BPF program
$(BPF_OBJ): $(BPF_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile and run ip_filter
ip_filter: $(C_SRC)
	$(CC) -o $@ $^ $(LDFLAGS)
	sudo setcap cap_net_raw+ep ip_filter

clean:
	$(RM) $(BPF_OBJ) ip_filter
