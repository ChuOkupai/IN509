#ifndef AST_EVALUATOR_HH
#define AST_EVALUATOR_HH

#include "nodes.hh"

namespace ast {

class ASTEvaluator : public ConstASTIntVisitor {
public:
  ASTEvaluator() {};
  ~ASTEvaluator() {};
  int visit(const IntegerLiteral &);
  int visit(const StringLiteral &);
  int visit(const BinaryOperator &);
  int visit(const Sequence &);
  int visit(const Let &);
  int visit(const Identifier &);
  int visit(const IfThenElse &);
  int visit(const VarDecl &);
  int visit(const FunDecl &);
  int visit(const FunCall &);
  int visit(const WhileLoop &);
  int visit(const ForLoop &);
  int visit(const Break &);
  int visit(const Assign &);
};

} // namespace ast

#endif // _AST_EVALUATOR_HH
