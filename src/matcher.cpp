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

		// rewrite REAL variables to double variables
		// e.g. REAL a; --> double a;
		if (const VarDecl *decl = Result.Nodes.getNodeAs<clang::VarDecl>(
				"realVarDecl")) {

			QualType declType = decl->getTypeSourceInfo()->getType();

			// we have to get to the location of the type name without any qualifiers
			// e.g.
			// when we match the declaration 'static REAL a;'
			// we have to move get the location of 'REAL',but not the location of 'static'
			sourceLocation =
					decl->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
			Rewrite.ReplaceText(sourceLocation, originalType.length(),
					replaceType);
		}

		// rewrite functions with return type REAL to return double
		// e.g. REAL f() { ... } --> double f() { ... }
		if (const FunctionDecl *decl = Result.Nodes.getNodeAs<
				clang::FunctionDecl>("realFuncDecl")) {
			sourceLocation = decl->getReturnTypeSourceRange().getBegin();
			Rewrite.ReplaceText(sourceLocation, originalType.length(),
					replaceType);
		}

		// rewrite explicit type casting
		// e.g.  REAL x = REAL(1); --> REAL x = double(1);
		if (const ExplicitCastExpr *expr = Result.Nodes.getNodeAs<
				ExplicitCastExpr>("realExplicitCastExpr")) {
			sourceLocation =
					expr->getTypeInfoAsWritten()->getTypeLoc().getBeginLoc();
			Rewrite.ReplaceText(sourceLocation, originalType.length(),
					replaceType);
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
		// Add a matcher for finding type 'iRRAM::REAL' variable declaration
		// e.g. -----------------------
		//		REAL a;
		//		REAL& b;
		//		REAL* c;
		//		const REAL d;
		//		const REAL& e;
		//		const REAL* f;
		//		-----------------------

		// match 'REAL a;'
		Matcher.addMatcher(
				declStmt(
						containsDeclaration(0,
								varDecl(hasType(asString(REAL_CLASS_NAME))).bind(
										"realVarDecl"))), &HandlerForReal);

		// match 'const REAL a;'
		Matcher.addMatcher(
				declStmt(
						containsDeclaration(0,
								varDecl(
										hasType(
												asString(
														CONST_REAL_CLASS_NAME)),
										hasType(isConstQualified())).bind(
										"realVarDecl"))), &HandlerForReal);

		// match 'REAL& a;'
		Matcher.addMatcher(
				declStmt(
						containsDeclaration(0,
								varDecl(
										hasType(
												asString(
														REAL_REFERENCE_CLASS_NAME))).bind(
										"realVarDecl"))), &HandlerForReal);

		// match 'const REAL& a;'
		Matcher.addMatcher(
				declStmt(
						containsDeclaration(0,
								varDecl(
										hasType(
												asString(
														CONST_REAL_REFERENCE_CLASS_NAME))).bind(
										"realVarDecl"))), &HandlerForReal);

		// match 'REAL* a;'
		Matcher.addMatcher(
						declStmt(
								containsDeclaration(0,
										varDecl(
												hasType(
														asString(
																REAL_POINTER_CLASS_NAME))).bind(
												"realVarDecl"))), &HandlerForReal);

		// match 'const REAL* a;'
		Matcher.addMatcher(
								declStmt(
										containsDeclaration(0,
												varDecl(
														hasType(
																asString(
																		CONST_REAL_POINTER_CLASS_NAME))).bind(
														"realVarDecl"))), &HandlerForReal);

		// Add a matcher for finding function declaration with return type 'iRRAM::REAL'
		// e.g. -----------------------
		//		REAL f() {};
		//		-----------------------
		Matcher.addMatcher(
				functionDecl(returns(asString(REAL_CLASS_NAME))).bind(
						"realFuncDecl"), &HandlerForReal);

		// Add a matcher for finding explicit expression of type 'REAL'
		// e.g. -----------------------
		//		REAL x = REAL(1);
		//		-----------------------
		Matcher.addMatcher(
				explicitCastExpr(hasType(asString(REAL_CLASS_NAME))).bind(
						"realExplicitCastExpr"), &HandlerForReal);

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

	static const std::string REAL_CLASS_NAME;
	static const std::string REAL_REFERENCE_CLASS_NAME;
	static const std::string CONST_REAL_CLASS_NAME;
	static const std::string CONST_REAL_REFERENCE_CLASS_NAME;
	static const std::string REAL_POINTER_CLASS_NAME;
	static const std::string CONST_REAL_POINTER_CLASS_NAME;

};

const std::string MyASTConsumer::REAL_CLASS_NAME = "class iRRAM::REAL";
const std::string MyASTConsumer::REAL_REFERENCE_CLASS_NAME =
		"class iRRAM::REAL &";
const std::string MyASTConsumer::CONST_REAL_CLASS_NAME =
		"const class iRRAM::REAL";
const std::string MyASTConsumer::CONST_REAL_REFERENCE_CLASS_NAME =
		"const class iRRAM::REAL &";
const std::string MyASTConsumer::REAL_POINTER_CLASS_NAME = "class iRRAM::REAL *";
const std::string MyASTConsumer::CONST_REAL_POINTER_CLASS_NAME =
		"const class iRRAM::REAL *";

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
