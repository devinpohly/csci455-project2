LDFLAGS = -lrt -pthread
CFLAGS = -std=gnu99 -Wall -ggdb -Wno-unused-value
# Turn this on, make clean, and make for strict checking
#CFLAGS += -Werror

# Change this to run tests with Valgrind (encouraged!)
#VALGRIND =
VALGRIND = valgrind --suppressions=$(VGSUPP) --error-exitcode=2 --errors-for-leak-kinds=all

VGSUPP = /dev/null

LIBS = queue.o kfc.o kthread.o
TESTS_LENIENT = create self fcfs yield yield2 exit
TESTS = $(TESTS_LENIENT) join sem
BINS = $(addprefix test-,$(TESTS))
OBJS = $(addsuffix .o,$(BINS))

RUN_TESTS_LENIENT = $(addprefix run-test-,$(TESTS_LENIENT))
RUN_TESTS = $(addprefix run-test-,$(TESTS))

.PHONY: all clean test $(RUN_TESTS)

all: $(LIBS) $(BINS)

clean:
	$(RM) $(BINS) $(OBJS) $(LIBS)

test: $(RUN_TESTS)
	@echo All succeeded

$(RUN_TESTS): run-test-%: test-%
	$(VALGRIND) ./$^

$(RUN_TESTS_LENIENT): VGSUPP=valgrind.supp

# Compile dependencies for libraries and binaries
$(LIBS): %.o: %.h
$(OBJS): test.h queue.h kfc.h kthread.h

# Link each binary with all the libraries
$(BINS): $(LIBS)
