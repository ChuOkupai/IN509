#include "ast_evaluator.hh"
#include "../utils/errors.hh"

namespace ast {

int ASTEvaluator::visit(const IntegerLiteral &literal) {
  return (literal.value);
}

int ASTEvaluator::visit(const StringLiteral &literal) {
  utils::error("internal error: attempting to evaluate a StringLiteral");
  return (-1);
}

int ASTEvaluator::visit(const BinaryOperator &binop) {
	int l, r;

	l = binop.get_left().accept(*this);
	r = binop.get_right().accept(*this);
  if (binop.op == o_plus)
	  return (l + r);
  if (binop.op == o_minus)
	  return (l - r);
  if (binop.op == o_plus)
	  return (l + r);
  if (binop.op == o_times)
	  return (l * r);
  if (binop.op == o_divide)
	  return (l / r);
  if (binop.op == o_eq)
	  return (l == r);
  if (binop.op == o_neq)
	  return (l != r);
  if (binop.op == o_lt)
	  return (l < r);
  if (binop.op == o_le)
	  return (l <= r);
  if (binop.op == o_gt)
	  return (l > r);
  if (binop.op == o_ge)
	  return (l >= r);
  utils::error("internal error: attempting to evaluate an unknown BinaryOperator");
  return (-1);
}

int ASTEvaluator::visit(const Sequence &seqExpr) {
  std::vector<Expr *> e;
  unsigned int i;

  e = seqExpr.get_exprs();
  if (e.empty())
  {
    utils::error("internal error: attempting to evaluate an empty Sequence");
    return (-1);
  }
  for (i = 0; i < e.size() - 1; i++)
    e[i]->accept(*this);
  return (e[i]->accept(*this));
}

int ASTEvaluator::visit(const Let &let) {
  utils::error("internal error: attempting to evaluate a Let");
  return (-1);
}

int ASTEvaluator::visit(const Identifier &id) {
  utils::error("internal error: attempting to evaluate a Identifier");
  return (-1);
}

int ASTEvaluator::visit(const IfThenElse &ite) {
  if (ite.get_condition().accept(*this))
	  return (ite.get_then_part().accept(*this));
  return (ite.get_else_part().accept(*this));
}

int ASTEvaluator::visit(const VarDecl &decl) {
  utils::error("internal error: attempting to evaluate a VarDecl");
  return (-1);
}

int ASTEvaluator::visit(const FunDecl &decl) {
   utils::error("internal error: attempting to evaluate a FunDecl");
  return (-1);
}

int ASTEvaluator::visit(const FunCall &call) {
  utils::error("internal error: attempting to evaluate a FunCall");
  return (-1);
}

int ASTEvaluator::visit(const WhileLoop &loop) {
  utils::error("internal error: attempting to evaluate a WhileLoop");
  return (-1);
}

int ASTEvaluator::visit(const ForLoop &loop) {
  utils::error("internal error: attempting to evaluate a ForLoop");
  return (-1);
}

int ASTEvaluator::visit(const Break &brk) {
  utils::error("internal error: attempting to evaluate a Break");
  return (-1);
}

int ASTEvaluator::visit(const Assign &assign) {
  utils::error("internal error: attempting to evaluate a Assign");
  return (-1);
}

} // namespace ast
