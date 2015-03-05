CXX = clang++
LLVM_CONFIG = llvm-config
LLVM_CXXFLAGS += $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags)
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs)
CLANG_LIBS = \
  -lclangFrontendTool -lclangFrontend -lclangDriver \
  -lclangSerialization -lclangCodeGen -lclangParse \
  -lclangSema -lclangStaticAnalyzerFrontend \
  -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore \
  -lclangAnalysis -lclangARCMigrate -lclangRewrite \
  -lclangEdit -lclangAST -lclangLex -lclangBasic

all: transform

transform:
	$(CXX) rewritersample.cpp $(LLVM_CXXFLAGS) $(CLANG_LIBS) $(LLVM_LIBS) $(LLVM_LDFLAGS) -lz -lcurses -lpthread -ldl 
