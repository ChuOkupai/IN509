#include <iostream>
#include <sstream>

#include "binder.hh"
#include "../utils/errors.hh"
#include "../utils/nolocation.hh"

using utils::error;
using utils::non_fatal_error;

namespace ast {
namespace binder {

/* Returns the current scope */
scope_t &Binder::current_scope() { return scopes.back(); }

/* Pushes a new scope on the stack */
void Binder::push_scope() { scopes.push_back(scope_t()); }

/* Pops the current scope from the stack */
void Binder::pop_scope() { scopes.pop_back(); }

/* Enter a declaration in the current scope. Raises an error if the declared name
 * is already defined */
void Binder::enter(Decl &decl) {
  scope_t &scope = current_scope();
  auto previous = scope.find(decl.name);
  if (previous != scope.end()) {
    non_fatal_error(decl.loc,
                    decl.name.get() + " is already defined in this scope");
    error(previous->second->loc, "previous declaration was here");
  }
  scope[decl.name] = &decl;
}

/* Finds the declaration for a given name. The scope stack is traversed
 * front to back starting from the current scope. The first matching
 * declaration is returned. Raises an error, if no declaration matches. */
Decl &Binder::find(const location loc, const Symbol &name) {
  for (auto scope = scopes.crbegin(); scope != scopes.crend(); scope++) {
    auto decl_entry = scope->find(name);
    if (decl_entry != scope->cend()) {
      return *decl_entry->second;
    }
  }
  error(loc, name.get() + " cannot be found in this scope");
}

Binder::Binder() : scopes() {
  /* Create the top-level scope */
  push_scope();

  /* Populate the top-level scope with all the primitive declarations */
  const Symbol s_int = Symbol("int");
  const Symbol s_string = Symbol("string");
  enter_primitive("print_err", boost::none, {s_string});
  enter_primitive("print", boost::none, {s_string});
  enter_primitive("print_int", boost::none, {s_int});
  enter_primitive("flush", boost::none, {});
  enter_primitive("getchar", s_string, {});
  enter_primitive("ord", s_int, {s_string});
  enter_primitive("chr", s_string, {s_int});
  enter_primitive("size", s_int, {s_string});
  enter_primitive("substring", s_string, {s_string, s_int, s_int});
  enter_primitive("concat", s_string, {s_string, s_string});
  enter_primitive("strcmp", s_int, {s_string, s_string});
  enter_primitive("streq", s_int, {s_string, s_string});
  enter_primitive("not", s_int, {s_int});
  enter_primitive("exit", boost::none, {s_int});
}

/* Declares a new primitive into the current scope*/
void Binder::enter_primitive(
    const std::string &name, const boost::optional<Symbol> &type_name,
    const std::vector<Symbol> &argument_typenames) {
  std::vector<VarDecl *> args;
  int counter = 0;
  for (const Symbol &tn : argument_typenames) {
    std::ostringstream argname;
    argname << "a_" << counter++;
    args.push_back(
        new VarDecl(utils::nl, Symbol(argname.str()), nullptr, tn));
  }

  boost::optional<Symbol> type_name_symbol = boost::none;
  FunDecl *fd = new FunDecl(utils::nl, Symbol(name), std::move(args), nullptr,
                            type_name, true);
  fd->set_external_name(Symbol("__" + name));
  enter(*fd);
}

/* Sets the parent of a function declaration and computes and sets
 * its unique external name */
void Binder::set_parent_and_external_name(FunDecl &decl) {
  auto parent = functions.empty() ? nullptr : functions.back();
  Symbol external_name;
  if (parent) {
    decl.set_parent(parent);
    external_name = parent->get_external_name().get() + '.' + decl.name.get();
  } else
    external_name = decl.name;
  while (external_names.find(external_name) != external_names.end())
    external_name = Symbol(external_name.get() + '_');
  external_names.insert(external_name);
  decl.set_external_name(external_name);
}

/* Binds a whole program. This method wraps the program inside a top-level main
 * function.  Then, it visits the programs with the Binder visitor; binding
 * each identifier to its declaration and computing depths.*/
FunDecl *Binder::analyze_program(Expr &root) {
  std::vector<VarDecl *> main_params;
  Sequence *const main_body = new Sequence(
      utils::nl,
      std::vector<Expr *>({&root, new IntegerLiteral(utils::nl, 0)}));
  FunDecl *const main = new FunDecl(utils::nl, Symbol("main"), main_params,
                                    main_body, Symbol("int"), true);
  main->accept(*this);
  return main;
}

void Binder::visit(IntegerLiteral &literal) {
  std::cout << "IntegerLiteral" << std::endl;
}

void Binder::visit(StringLiteral &literal) {
  std::cout << "StringLiteral" << std::endl;
}

void Binder::visit(BinaryOperator &op) {
  std::cout << "BinaryOperator" << std::endl;
  op.get_left().accept(*this);
  op.get_right().accept(*this);
}

void Binder::visit(Sequence &seq) {
  std::cout << "Sequence" << std::endl;
  std::vector<Expr*> tati_danielle = seq.get_exprs();
  for (Expr* e : tati_danielle)
    e->accept(*this);
}

void Binder::visit(Let &let) {
  std::cout << "Let: " << std::endl;
  push_scope();
  for (auto& decl: let.get_decls())
    decl->accept(*this);
  let.get_sequence().accept(*this);
  pop_scope();
}

void Binder::visit(Identifier &id) {
  std::cout << "Identifier: " << id.name.get() << std::endl;
  id.set_decl(dynamic_cast<VarDecl*>(&find(id.loc, id.name)));
}

void Binder::visit(IfThenElse &ite) {
  std::cout << "IfThenElse" << std::endl;
  ite.get_condition().accept(*this);
  ite.get_then_part().accept(*this);
  ite.get_else_part().accept(*this);
}

void Binder::visit(VarDecl &decl) {
  enter(decl);
  std::cout << "VarDecl: " << decl.name.get() << std::endl;
  decl.set_escapes();
  decl.set_depth(decl.get_depth() - 1);
  decl.get_expr().value().accept(*this);
}

void Binder::visit(FunDecl &decl) {
  std::cout << "FunDecl: " << decl.name.get() << std::endl;
  set_parent_and_external_name(decl);
  enter(decl);
  functions.push_back(&decl);
  push_scope();
  for (VarDecl *v : decl.get_params())
  {
    v->set_escapes();
    enter(*v);
  }
  decl.get_expr()->accept(*this);
  pop_scope();
  functions.pop_back();
}

void Binder::visit(FunCall &call) {
  std::cout << "FunCall" << std::endl;
  call.set_decl(dynamic_cast<FunDecl*>(&find(call.loc, call.func_name)));
}

void Binder::visit(WhileLoop &loop) {
  std::cout << "WhileLoop" << std::endl;
}

void Binder::visit(ForLoop &loop) {
  std::cout << "ForLoop" << std::endl;
}

void Binder::visit(Break &b) {
  std::cout << "Break" << std::endl;
}

void Binder::visit(Assign &assign) {
  std::cout << "Assign" << std::endl;
}

} // namespace binder
} // namespace ast
