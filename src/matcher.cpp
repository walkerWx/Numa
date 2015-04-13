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
#include "clang/AST/TypeVisitor.h"
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

struct ReplaceTypePair {
    std::string originType;
    std::string replaceType;
    ReplaceTypePair(std::string originType, std::string replaceType) :
            originType(originType), replaceType(replaceType) {
    }
};

const ReplaceTypePair INTEGER("INTEGER", "int");
const ReplaceTypePair RATIONAL("RATIONAL", "double");
const ReplaceTypePair DYADIC("DYADIC", "unknown");
const ReplaceTypePair LAZY_BOOLEAN("LAZY_BOOLEAN", "bool");
const ReplaceTypePair REAL("REAL", "double");
const ReplaceTypePair COMPLEX("COMPLEX", "unknown");

static llvm::cl::OptionCategory MatcherSampleCategory(
        "Match and Replace type REAL");

class TypeIdentifier;

class NumaTypeVisitor: public TypeVisitor<NumaTypeVisitor> {
public:
    NumaTypeVisitor() :
            typeName(""), flag(false) {
    }

    void VisitRecordType(const RecordType *T) {
        if (typeName.compare(T->getAsCXXRecordDecl()->getNameAsString()) == 0) {
            flag = true;
        };
        return;
    }

    void VisitPointerType(const PointerType *T) {
        return Visit(T->getPointeeType().split().Ty);
    }

    void VisitLValueReferenceType(const LValueReferenceType *T) {
        return Visit(T->getPointeeType().split().Ty);
    }

    void VisitElaboratedType(const ElaboratedType *T) {
        return Visit(T->getNamedType().split().Ty);
    }

    void VisitArrayType(const ArrayType *T) {
        return Visit(T->getElementType().split().Ty);
    }

    bool isDerivedFrom(const Type *T, std::string typeName) {
        this->typeName = typeName;
        this->flag = false;
        Visit(T);
        return flag;
    }
private:
    std::string typeName;
    bool flag; // a flag to record whether the Type is derived from the base type
};

class NamespaceHandler: public MatchFinder::MatchCallback {
public:
    NamespaceHandler(Rewriter &Rewriter) :
            Rewriter(Rewriter) {
    }

    virtual void run(const MatchFinder::MatchResult &Result) {
        SourceLocation sourceLocation;
        if (const UsingDirectiveDecl *decl = Result.Nodes.getNodeAs<
                clang::UsingDirectiveDecl>("iRRAMUsingDirectiveDecl")) {
            if (decl->getNominatedNamespace()->getIdentifier()->getName().str()
                    == "iRRAM") {
                Rewriter.RemoveText(decl->getSourceRange());
            }
        }

        if (const UsingDecl *decl = Result.Nodes.getNodeAs<clang::UsingDecl>(
                "iRRAMUsingDecl")) {
            if (const NamespaceDecl *namespaceDecl =
                    decl->getQualifier()->getAsNamespace()) {
                std::string name = namespaceDecl->getName().str();
                if (name == "iRRAM") {
                    Rewriter.RemoveText(decl->getSourceRange());
                }
            }
        }
    }

private:
    Rewriter &Rewriter;
};

class iRRAMHandler: public MatchFinder::MatchCallback {
public:
    iRRAMHandler(Rewriter &Rewrite) :
            Rewriter(Rewrite) {
    }

    virtual void run(const MatchFinder::MatchResult &Result) {
        SourceRange sourceRange;
        const Type *T;
        int length = 0;

        // rewrite REAL variables to double variables
        // e.g. REAL a; --> double a;
        if (const VarDecl *decl = Result.Nodes.getNodeAs<clang::VarDecl>(
                "iRRAMDecl")) {
            T = decl->getType().split().Ty;
            T->dump();
            if (decl->getTypeSourceInfo() != NULL) {
                sourceRange =
                        decl->getTypeSourceInfo()->getTypeLoc().getSourceRange();
            }
        }

        // rewrite functions with return type REAL to return double
        // e.g. REAL f() { ... } --> double f() { ... }
        if (const FunctionDecl *decl = Result.Nodes.getNodeAs<
                clang::FunctionDecl>("iRRAMDecl")) {
            T = decl->getReturnType().split().Ty;
            sourceRange = decl->getReturnTypeSourceRange();
        }

        // rewrite explicit type casting
        // e.g.  REAL x = REAL(1); --> REAL x = double(1);
        if (const ExplicitCastExpr *expr = Result.Nodes.getNodeAs<
                ExplicitCastExpr>("iRRAMExplicitCastExpr")) {
            T = expr->getTypeAsWritten().split().Ty;
            sourceRange =
                    expr->getTypeInfoAsWritten()->getTypeLoc().getSourceRange();
        }

        if (visitor.isDerivedFrom(T, INTEGER.originType)) {
            Rewriter.ReplaceText(sourceRange, INTEGER.replaceType);
        } else if (visitor.isDerivedFrom(T, RATIONAL.originType)) {
            Rewriter.ReplaceText(sourceRange, RATIONAL.replaceType);
        } else if (visitor.isDerivedFrom(T, DYADIC.originType)) {
            Rewriter.ReplaceText(sourceRange, DYADIC.replaceType);
        } else if (visitor.isDerivedFrom(T, LAZY_BOOLEAN.originType)) {
            Rewriter.ReplaceText(sourceRange, LAZY_BOOLEAN.replaceType);
        } else if (visitor.isDerivedFrom(T, REAL.originType)) {
            Rewriter.ReplaceText(sourceRange, REAL.replaceType);
        } else if (visitor.isDerivedFrom(T, COMPLEX.originType)) {
            Rewriter.ReplaceText(sourceRange, COMPLEX.replaceType);
        }

    }
private:
    Rewriter &Rewriter;
    NumaTypeVisitor visitor;
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
            HandlerForiRRAM(R), HandlerForMainFunc(R), HandlerForNamespace(R) {
        // Add a matcher to find iRRAM variable declaration or function declaration or parameter declaration
        // e.g. -----------------------
        //        REAL a;
        //        REAL& b;
        //        REAL* c;
        //        const REAL d;
        //        const REAL& e;
        //        const REAL* f;
        //        REAL f(REAL, REAL*, REAL &){ ... };
        //      -----------------------
        Matcher.addMatcher(decl().bind("iRRAMDecl"), &HandlerForiRRAM);

        // Add a matcher to find explicit expression of type 'REAL'
        // e.g. -----------------------
        //        REAL x = REAL(1);
        //        INTEGER(10);
        //      -----------------------
        Matcher.addMatcher(explicitCastExpr().bind("iRRAMExplicitCastExpr"),
                &HandlerForiRRAM);

        // Add a mather to find using namespace declaration of iRRAM
        // e.g. -----------------------
        //        using namespace iRRAM;
        //      -----------------------
        Matcher.addMatcher(usingDirectiveDecl().bind("iRRAMUsingDirectiveDecl"),
                &HandlerForNamespace);

        //Add a matcher to find using declaration of iRRAM
        // e.g. -----------------------
        //        using iRRAM::REAL;
        //      -----------------------
        Matcher.addMatcher(usingDecl().bind("iRRAMUsingDecl"),
                &HandlerForNamespace);

        //Add a matcher to find the 'void compute()' function
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
    iRRAMHandler HandlerForiRRAM;
    MainFuncHandler HandlerForMainFunc;
    NamespaceHandler HandlerForNamespace;
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
