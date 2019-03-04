LDFLAGS = -lrt -pthread
CFLAGS = -std=gnu99 -Wall -ggdb
# Turn this on, make clean, and make for strict checking
#CFLAGS += -Werror

LIBS = queue.o kfc.o kthread.o
TESTS = create self fcfs yield exit join
BINS = $(addprefix test-,$(TESTS))
OBJS = $(addsuffix .o,$(BINS))

RUN_TESTS = $(addprefix run-test-,$(TESTS))

.PHONY: all clean test $(RUN_TESTS)

all: $(LIBS) $(BINS)

clean:
	$(RM) $(BINS) $(OBJS) $(LIBS)

test: $(RUN_TESTS)
	@echo All succeeded

$(RUN_TESTS): run-test-%: test-%
	./$^

# Compile dependencies for libraries and binaries
$(LIBS): %.o: %.h
$(OBJS): test.h queue.h kfc.h kthread.h

# Link each binary with all the libraries
$(BINS): $(LIBS)
