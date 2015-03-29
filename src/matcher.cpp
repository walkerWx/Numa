//------------------------------------------------------------------------------
// AST matching sample. Demonstrates:
//
// * Use AST matchers to find all the declarations whose type is 'REAL'
// * Replace all the 'REAL' type declaration with 'double'
//
// Walker Wang (walkernju@gmail.com)
//------------------------------------------------------------------------------
#include <string>
#include <iostream>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory MatcherSampleCategory(
		"Match and Replace type REAL");

class RealHandler: public MatchFinder::MatchCallback {
public:
	RealHandler(Rewriter &Rewrite) :
			Rewrite(Rewrite) {
	}

	virtual void run(const MatchFinder::MatchResult &Result) {
		std::string originalType = "REAL";
		std::string replaceType = "double";
		SourceLocation sourceLocation;
		if (const VarDecl *decl = Result.Nodes.getNodeAs<clang::VarDecl>(
				"realVarDecl")) {
			decl->dump();
			sourceLocation = decl->getLocStart();
			Rewrite.ReplaceText(sourceLocation, originalType.length(),
					replaceType);
		}

		if (const FunctionDecl *decl = Result.Nodes.getNodeAs<
				clang::FunctionDecl>("realFuncDecl")) {
			sourceLocation = decl->getReturnTypeSourceRange().getBegin();
			Rewrite.ReplaceText(sourceLocation, originalType.length(),
					replaceType);
		}

		if (const PointerType *type =
				Result.Nodes.getNodeAs<clang::PointerType>("pointer")) {
			type->dump();
		}
	}
private:
	Rewriter &Rewrite;
};

// In iRRAM programs,the entry point of main function is via function 'void compute()'
// We rewrite the function 'void compute()' to 'int main()'
class MainFuncHandler: public MatchFinder::MatchCallback {
public:
	MainFuncHandler(Rewriter &Rewriter) :
			Rewriter(Rewriter) {
	}

	virtual void run(const MatchFinder::MatchResult &Result) {
		if (const FunctionDecl *mainFuncDecl = Result.Nodes.getNodeAs<
				clang::FunctionDecl>("mainFuncDecl")) {

			std::string originalReturnType = "void";
			std::string replaceReturnType = "int";
			std::string originalFuncName = "compute";
			std::string replaceFuncName = "main";

			SourceLocation sourceLocation =
					mainFuncDecl->getReturnTypeSourceRange().getBegin();
			Rewriter.ReplaceText(sourceLocation, originalReturnType.length(),
					replaceReturnType);

			sourceLocation = mainFuncDecl->getNameInfo().getLoc();
			Rewriter.ReplaceText(sourceLocation, originalFuncName.length(),
					replaceFuncName);

		}
	}
private:
	Rewriter &Rewriter;
};

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser. It registers a couple of matchers and runs them on
// the AST.
class MyASTConsumer: public ASTConsumer {
public:
	MyASTConsumer(Rewriter &R) :
			HandlerForReal(R), HandlerForMainFunc(R) {
		//Add a matcher for finding type 'REAL' variable declaration
//	Matcher.addMatcher(varDecl(hasType(recordDecl(hasName("REAL")))).bind("realVarDecl"), &HandlerForReal);
		Matcher.addMatcher(pointerType().bind("pointer"), &HandlerForReal);

		//Add a matcher for finding function declaration with return type 'REAL'
		Matcher.addMatcher(functionDecl().bind("realFuncDecl"),
				&HandlerForReal);

		//Add a matcher for find the 'void compute()' function
		Matcher.addMatcher(
				functionDecl(hasType(asString("void (void)")),
						hasName("compute")).bind("mainFuncDecl"),
				&HandlerForMainFunc);

	}

	void HandleTranslationUnit(ASTContext &Context) override {
		// Run the matchers when we have the whole TU parsed.
		Matcher.matchAST(Context);
	}

private:
	RealHandler HandlerForReal;
	MainFuncHandler HandlerForMainFunc;
	MatchFinder Matcher;
};

// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction: public ASTFrontendAction {
public:
	MyFrontendAction() {
	}
	void EndSourceFileAction() override {
		TheRewriter.getEditBuffer(TheRewriter.getSourceMgr().getMainFileID()).write(
				llvm::outs());
	}

	std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
			StringRef file) override {
		TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
		return llvm::make_unique<MyASTConsumer>(TheRewriter);
	}

private:
	Rewriter TheRewriter;
};

int main(int argc, const char **argv) {
	CommonOptionsParser op(argc, argv, MatcherSampleCategory);
	ClangTool Tool(op.getCompilations(), op.getSourcePathList());

	return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}
