LDFLAGS = -lrt -pthread
CFLAGS = -std=gnu99 -Wall -ggdb -Wno-unused-value
# Turn this on, make clean, and make for strict checking
#CFLAGS += -Werror

# "Lenient" tests run with Valgrind suppressions to ignore memory that will be
# freed by functions not yet implemented.
TESTS_LENIENT = create self fcfs yield yield2 exit
TESTS_NOVG = m2m

# List of all tests
TESTS = $(TESTS_LENIENT) join sem sem2 sem3 $(TESTS_NOVG) m2m-pc

# Test-related targets and lists
RUN_TESTS = $(addprefix run-test-,$(TESTS))
RUN_TESTS_LENIENT = $(addprefix run-test-,$(TESTS_LENIENT))
RUN_TESTS_NOVG = $(addprefix run-test-,$(TESTS_NOVG))

# Build products
BINS = $(addprefix test-,$(TESTS))
OBJS = $(addsuffix .o,$(BINS))
LIBS = queue.o kfc.o kthread.o

# For the vtest target
VALGRIND =
VGSUPP = /dev/null

# Phony targets
.PHONY: all clean test vtest $(RUN_TESTS)

all: $(LIBS) $(BINS)

clean:
	$(RM) $(BINS) $(OBJS) $(LIBS)

test: $(RUN_TESTS)
	@echo All succeeded

vtest: test
vtest: VALGRIND = valgrind --suppressions=$(VGSUPP) --error-exitcode=2 --errors-for-leak-kinds=all

$(RUN_TESTS): run-test-%: test-%
	$(VALGRIND) ./$^

$(RUN_TESTS_NOVG): VALGRIND =

# Additional suppressions file when running lenient tests
$(RUN_TESTS_LENIENT): VGSUPP = valgrind.supp

# Compile dependencies for libraries and binaries
$(LIBS): %.o: %.h
$(OBJS): test.h queue.h kfc.h kthread.h

# Link each binary with all the libraries
$(BINS): $(LIBS)
