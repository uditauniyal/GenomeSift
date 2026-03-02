CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS =

SRCS = src/main.cpp \
	src/sequence/Sequence.cpp \
	src/sequence/DNASequence.cpp \
	src/sequence/RNASequence.cpp \
	src/sequence/ProteinSequence.cpp \
	src/io/FastaParser.cpp \
	src/io/ReportGenerator.cpp \
	src/alignment/AlignmentResult.cpp \
	src/alignment/NeedlemanWunsch.cpp \
	src/alignment/SmithWaterman.cpp \
	src/alignment/Aligner.cpp \
	src/mutation/Mutation.cpp \
	src/mutation/DiseaseDatabase.cpp \
	src/mutation/MutationDetector.cpp \
	src/observer/ConsoleAlert.cpp \
	src/observer/FileLogger.cpp \
	src/observer/SeverityFilter.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = genomesift

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

setup:
	mkdir -p reports

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f reports/*.txt

run: all setup
	./$(TARGET)

test: all setup
	./$(TARGET) --ref data/reference_sequences/HBB_healthy.fasta --patient data/patient_samples/HBB_patient_001.fasta --algorithm global

.PHONY: all clean run test setup
