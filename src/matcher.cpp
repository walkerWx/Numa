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

		//rewrite REAL parameters  to double
		// e.g. f(const REAL& a) --> f(const double& a)
		if (const ParmVarDecl *decl =
				Result.Nodes.getNodeAs<clang::ParmVarDecl>("realParmVarDecl")) {
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

// Hander for real functions in the iRRAM libraries
class LibHandler: public MatchFinder::MatchCallback {
public:
	LibHandler(Rewriter &Rewriter) :
			Rewriter(Rewriter) {
	}

	virtual void run(const MatchFinder::MatchResult &Result) {
		if (const CallExpr *callExpr = Result.Nodes.getNodeAs<
				clang::CallExpr>("realCallExpr")) {
			std::string funcName = callExpr->getDirectCallee()->getNameInfo().getAsString();
			SourceLocation sourceLocation = callExpr->getLocStart();

			std::cout << funcName << std::endl;

			if (funcName.compare("power") == 0) {
				Rewriter.ReplaceText(sourceLocation, 5, "pow");
			}

			if (funcName.compare("modulo") == 0) {
				const Expr *firstArg = callExpr->getArg(0);
				const Expr *secondArg = callExpr->getArg(1);
				const FunctionDecl *callee = callExpr->getDirectCallee();
				for (auto itr = callee->param_begin(); itr != callee->param_end(); itr++) {
					(*itr)->getType().getTypePtr()->dump();
				}
				std::cout << callExpr->getNumArgs() << std::endl;
			}

			if (funcName.compare("maximum") == 0) {
				Rewriter.ReplaceText(sourceLocation, 7, "max");
			}

			if (funcName.compare("minimum") == 0) {
				Rewriter.ReplaceText(sourceLocation, 7, "min");
			}

			if (funcName.compare("sqrt") == 0) {
				// do nothing
			}

			if (funcName.compare("root") == 0) {

			}

			if (funcName.compare("sin") == 0) {
				// we do nothing here because in iRRAM and c++ standard math library they have the same name
			}

			if (funcName.compare("cos") == 0) {
				// do nothing
			}

			if (funcName.compare("tan") == 0) {
				// do nothing
			}

			if (funcName.compare("cotan") == 0) {

			}

			if (funcName.compare("sec") == 0) {

			}

			if (funcName.compare("cosec") == 0) {

			}

			if (funcName.compare("asin") == 0) {

			}

			if (funcName.compare("acos") == 0) {

			}

			if (funcName.compare("atan") == 0) {

			}

			if (funcName.compare("acotan") == 0) {

			}

			if (funcName.compare("asec") == 0) {

			}

			if (funcName.compare("acosec") == 0) {

			}

			if (funcName.compare("sinh") == 0) {

			}

			if (funcName.compare("cosh") == 0) {

			}

			if (funcName.compare("tanh") == 0) {

			}

			if (funcName.compare("coth") == 0) {

			}

			if (funcName.compare("sech") == 0) {

			}

			if (funcName.compare("cosech") == 0) {

			}

			if (funcName.compare("asinh") == 0) {

			}

			if (funcName.compare("acosh") == 0) {

			}

			if (funcName.compare("atanh") == 0) {

			}

			if (funcName.compare("acoth") == 0) {

			}

			if (funcName.compare("asech") == 0) {

			}

			if (funcName.compare("acosech") == 0) {

			}

			if (funcName.compare("exp") == 0) {

			}

			if (funcName.compare("log") == 0) {

			}

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
			HandlerForReal(R), HandlerForMainFunc(R), HandlerForLib(R) {
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
														POINTER_TO_CONST_REAL_CLASS_NAME))).bind(
										"realVarDecl"))), &HandlerForReal);

		// match 'REAL * const a;'
		Matcher.addMatcher(
				declStmt(
						containsDeclaration(0,
								varDecl(
										hasType(
												asString(
														CONST_POINTER_TO_REAL_CLASS_NAME))).bind(
										"realVarDecl"))), &HandlerForReal);

		// match ' returnType  somefunc(..., const REAL& a, ...)'
		Matcher.addMatcher(
				parmVarDecl(hasType(asString(CONST_REAL_REFERENCE_CLASS_NAME))).bind(
						"realParmVarDecl"), &HandlerForReal);

		// Add a matcher to find function declaration with return type 'iRRAM::REAL'
		// e.g. -----------------------
		//		REAL f() {};
		//		-----------------------
		Matcher.addMatcher(
				functionDecl(returns(asString(REAL_CLASS_NAME))).bind(
						"realFuncDecl"), &HandlerForReal);

		// Add a matcher to find explicit expression of type 'REAL'
		// e.g. -----------------------
		//		REAL x = REAL(1);
		//		-----------------------
		Matcher.addMatcher(
				explicitCastExpr(hasType(asString(REAL_CLASS_NAME))).bind(
						"realExplicitCastExpr"), &HandlerForReal);

		//Add a matcher to find the 'void compute()' function
		Matcher.addMatcher(
				functionDecl(hasType(asString("void (void)")),
						hasName("compute")).bind("mainFuncDecl"),
				&HandlerForMainFunc);

		//Add a matcher to find functions in iRRAM libraries
		Matcher.addMatcher(
				callExpr(
						callee(
								functionDecl(
										returns(asString(REAL_CLASS_NAME))))).bind(
						"realCallExpr"), &HandlerForLib);

	}

	void HandleTranslationUnit(ASTContext &Context) override {
		// Run the matchers when we have the whole TU parsed.
		Matcher.matchAST(Context);
	}

private:
	RealHandler HandlerForReal;
	MainFuncHandler HandlerForMainFunc;
	LibHandler HandlerForLib;
	MatchFinder Matcher;

	static const std::string REAL_CLASS_NAME;
	static const std::string REAL_REFERENCE_CLASS_NAME;
	static const std::string CONST_REAL_CLASS_NAME;
	static const std::string CONST_REAL_REFERENCE_CLASS_NAME;
	static const std::string REAL_POINTER_CLASS_NAME;
	static const std::string POINTER_TO_CONST_REAL_CLASS_NAME;
	static const std::string CONST_POINTER_TO_REAL_CLASS_NAME;

};

const std::string MyASTConsumer::REAL_CLASS_NAME = "class iRRAM::REAL";
const std::string MyASTConsumer::REAL_REFERENCE_CLASS_NAME =
		"class iRRAM::REAL &";
const std::string MyASTConsumer::CONST_REAL_CLASS_NAME =
		"const class iRRAM::REAL";
const std::string MyASTConsumer::CONST_REAL_REFERENCE_CLASS_NAME =
		"const class iRRAM::REAL &";
const std::string MyASTConsumer::REAL_POINTER_CLASS_NAME = "class iRRAM::REAL *";
const std::string MyASTConsumer::POINTER_TO_CONST_REAL_CLASS_NAME =
		"const class iRRAM::REAL *";
const std::string MyASTConsumer::CONST_POINTER_TO_REAL_CLASS_NAME =
		"class iRRAM::REAL *const";

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
