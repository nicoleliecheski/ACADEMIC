CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

SRC_DIR = src
TEST_DIR = tests

FILTERS = $(SRC_DIR)/ReadFilter.cpp $(SRC_DIR)/CleanFilter.cpp $(SRC_DIR)/TransformFilter.cpp $(SRC_DIR)/SumFilter.cpp $(SRC_DIR)/ReportFilter.cpp $(SRC_DIR)/Pipeline.cpp

.PHONY: all tests clean

all: pipeline

pipeline: $(SRC_DIR)/main.cpp $(FILTERS)
	$(CXX) $(CXXFLAGS) -o pipeline $(SRC_DIR)/main.cpp $(FILTERS)

tests: $(TEST_DIR)/test_filters.cpp $(SRC_DIR)/CleanFilter.cpp $(SRC_DIR)/TransformFilter.cpp $(SRC_DIR)/SumFilter.cpp $(SRC_DIR)/ReportFilter.cpp
	$(CXX) $(CXXFLAGS) -o run_tests $(TEST_DIR)/test_filters.cpp $(SRC_DIR)/CleanFilter.cpp $(SRC_DIR)/TransformFilter.cpp $(SRC_DIR)/SumFilter.cpp $(SRC_DIR)/ReportFilter.cpp
	./run_tests

clean:
	rm -f pipeline run_tests
