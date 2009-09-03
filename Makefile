#Copyright (C) 2009 by Jhair Tocancipa Triana

SHELL = /bin/sh

BOOST_INCLUDE = -I$(BOOST_HOME)/include/boost-1_38
BOOST_LIB = -L$(BOOST_HOME)/lib
CFLAGS = -g -O0 $(INCLUDE)
CC = g++ $(CFLAGS)

regex_obj = regex.o
regex_src = regex.cpp

graph_obj = graph.o
graph_src = graph.cpp

graph_driver_obj = graph_driver.o
graph_driver_src = graph_driver.cpp

graph_reader_obj = graph_reader.o
graph_reader_src = graph_reader.cpp

SRC = $(regex_src) $(graph_src)

.SUFFIXES:
.SUFFIXEX: .cpp .o

regex : $(regex_obj)
	@echo $(HOME)
	$(CC) -o regex $< $(BOOST_LIB) -lboost_regex-gcc42-mt-1_38

regex.o : $(regex_src)
	$(CC) -c $(BOOST_INCLUDE) $<

graph_driver : $(graph_driver_obj) $(graph_obj) $(graph_reader_obj)
	$(CC) -o graph_driver \
		$(graph_driver_obj) \
		$(graph_obj) \
		$(graph_reader_obj) \
		$(BOOST_LIB) \
		-lboost_graph-gcc42-mt-1_38 \
		-lboost_math_c99-gcc42-mt-1_38 \
		-lboost_program_options-gcc42-mt-1_38


graph_reader.o : $(graph_reader_src)
	$(CC) -c $(BOOST_INCLUDE) $(graph_reader_src)

graph_driver.o : $(graph_driver_src) $(graph_obj)
	$(CC) -c $(BOOST_INCLUDE) $(graph_driver_src)

graph.o : $(graph_src)
	$(CC) -c $(BOOST_INCLUDE) $(graph_src)

clean :
	rm -f *.o a.out regex graph graph_driver

tags: $(SRC)
	etags $(SRC)

ebrowse: $(SRC)
	ebrowse $(SRC)