#ifndef XML_WRITER_H
#define XML_WRITER_H

#include "csv_parser.h"

void write_xml(const char *output_filename, CSVTable **tables, int num_tables);

#endif
