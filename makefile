CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
OBJDIR = build
SRCDIR = src

OBJS = $(OBJDIR)/main.o \
       $(OBJDIR)/file_input.o \
       $(OBJDIR)/csv_parser.o \
       $(OBJDIR)/xml_writer.o \
       $(OBJDIR)/utils.o

TARGET = csv_to_xml

.PHONY: all clean testdata

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

# Generate test data using Python script
testdata:
	python3 tools/gen_csv.py
