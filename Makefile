CXX = clang++
LLVM_CONFIG = llvm-config
LLVM_CXXFLAGS += $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags) 
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs --system-libs)
CLANG_LIBS = \
  -lclangFrontendTool -lclangFrontend -lclangDriver \
  -lclangSerialization -lclangCodeGen -lclangParse \
  -lclangSema -lclangStaticAnalyzerFrontend \
  -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore \
  -lclangAnalysis -lclangARCMigrate -lclangRewrite \
  -lclangEdit -lclangAST -lclangLex -lclangBasic

.PHONY: all

all: bin/transform bin/traverse bin/matcher

bin/transform: src/transform.cpp
	$(CXX) $^ $(LLVM_CXXFLAGS) $(CLANG_LIBS) $(LLVM_LIBS) $(LLVM_LDFLAGS) -o $@

bin/traverse: src/traverse.cpp
	$(CXX) $^ $(LLVM_CXXFLAGS) $(CLANG_LIBS) $(LLVM_LIBS) $(LLVM_LDFLAGS) -o $@

bin/matcher : matchers_rewriter.cpp 
	$(CXX) $^ $(LLVM_CXXFLAGS) $(CLANG_LIBS) $(LLVM_LIBS) $(LLVM_LDFLAGS) -o $@
	
