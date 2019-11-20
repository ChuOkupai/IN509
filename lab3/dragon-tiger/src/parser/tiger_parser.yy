%skeleton "lalr1.cc"
%defines
%define parser_class_name {tiger_parser}

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{
#include <string>
class ParserDriver;
#include "../ast/nodes.hh"
#include "../utils/errors.hh"
#include "../utils/nolocation.hh"

using namespace ast::types;
using utils::nl;
}

// The parsing context.
%param { ParserDriver& driver }

%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};

%define parse.trace
%define parse.error verbose

%code
{
#include "parser_driver.hh"
}

// Define Tiger's symbols and keywords tokens

%define api.token.prefix {TOK_}
%token
  EOF  0  "end of file"
  COMMA ","
  COLON ":"
  SEMICOLON ";"
  LPAREN "("
  RPAREN ")"
  LBRACE "{"
  RBRACE "}"
  PLUS "+"
  MINUS "-"
  TIMES "*"
  DIVIDE "/"
  EQ "="
  NEQ "<>"
  LT "<"
  LE "<="
  GT ">"
  GE ">="
  AND "&"
  OR "|"
  ASSIGN ":="
  IF "if"
  THEN "then"
  ELSE "else"
  WHILE "while"
  FOR "for"
  TO "to"
  DO "do"
  LET "let"
  IN "in"
  END "end"
  BREAK "break"
  FUNCTION "function"
  VAR "var"
  UMINUS "uminus"
;

// Define tokens that have an associated value, such as identifiers or strings

%token <Symbol> ID "id"
%token <Symbol> STRING "string"
%token <int> INT "integer"

// Declare the nonterminals types

// %type <Var *> var;
%type <VarDecl *> param;
%type <std::vector<VarDecl *>> params nonemptyparams;
%type <Decl *> decl funcDecl varDecl;
%type <std::vector<Decl *>> decls;
%type <Expr *> expr stringExpr seqExpr callExpr opExpr negExpr
            assignExpr whileExpr forExpr breakExpr letExpr var intExpr
            ifThenElseExpr ifThenExpr;

%type <std::vector<Expr *>> exprs nonemptyexprs;
%type <std::vector<Expr *>> arguments nonemptyarguments;

%type <Expr *> program;

%type <boost::optional<Symbol>> typeannotation;

%%

// Declare precedence rules

%nonassoc FUNCTION VAR TYPE DO OF ASSIGN;
%left EQ NEQ LT LE GT GE;
%left OR;
%left AND;
%left MINUS PLUS;
%left DIVIDE TIMES;
%left UMINUS;

// Declare grammar rules and production actions

%start program;

program: expr { driver.result_ast = $1; }
;

decl: varDecl { $$ = $1; }
   | funcDecl { $$ = $1; }
;

expr: stringExpr { $$ = $1; }
   | seqExpr { $$ = $1; }
   | var { $$ = $1; }
   | callExpr { $$ = $1; }
   | opExpr { $$ = $1; }
   | negExpr { $$ = $1; }
   | assignExpr { $$ = $1; }
   | whileExpr { $$ = $1; }
   | forExpr { $$ = $1; }
   | breakExpr { $$ = $1; }
   | letExpr { $$ = $1; }
   | intExpr { $$ = $1; }
   | ifThenElseExpr { $$ = $1; }
   | ifThenExpr { $$ = $1; }
;

varDecl: VAR ID typeannotation ASSIGN expr
  { $$ = new VarDecl(@1, $2, $5, $3); }
;

funcDecl: FUNCTION ID LPAREN params RPAREN typeannotation EQ expr
  { $$ = new FunDecl(@1, $2, $4, $8, $6); }
;

/* Exprs */

stringExpr: STRING
  { $$ = new StringLiteral(@1, Symbol($1)); }
;

var : ID
  { $$ = new Identifier(@1, $1); }
;

callExpr: ID LPAREN arguments RPAREN
  { $$ = new FunCall(@1, $3, Symbol($1)); }
;

negExpr: MINUS expr
  { $$ = new BinaryOperator(@1, new IntegerLiteral(@1, 0), $2, o_minus); }
  %prec UMINUS
;

intExpr: INT
  { $$ = new IntegerLiteral(@1, $1); }
;

/*opExp: expr op expr*/

opExpr: expr PLUS expr   { $$ = new BinaryOperator(@2, $1, $3, o_plus); }
      | expr MINUS expr  { $$ = new BinaryOperator(@2, $1, $3, o_minus); }
      | expr TIMES expr  { $$ = new BinaryOperator(@2, $1, $3, o_times); }
      | expr DIVIDE expr { $$ = new BinaryOperator(@2, $1, $3, o_divide); }
      | expr EQ expr     { $$ = new BinaryOperator(@2, $1, $3, o_eq); }
      | expr NEQ expr    { $$ = new BinaryOperator(@2, $1, $3, o_neq); }
      | expr LT expr     { $$ = new BinaryOperator(@2, $1, $3, o_lt); }
      | expr GT expr     { $$ = new BinaryOperator(@2, $1, $3, o_gt); }
      | expr LE expr     { $$ = new BinaryOperator(@2, $1, $3, o_le); }
      | expr GE expr     { $$ = new BinaryOperator(@2, $1, $3, o_ge); }
      | expr AND expr    { $$ = new IfThenElse(@2, $1,
                           new IfThenElse(@3, $3, new IntegerLiteral(nl, 1), new IntegerLiteral(nl, 0)),
                           new IntegerLiteral(nl, 0));
      }
      | expr OR expr     { $$ = new IfThenElse(@2, $1, new IntegerLiteral(nl, 1),
                           new IfThenElse(@3, $3, new IntegerLiteral(nl, 1),
                           new IntegerLiteral(nl, 0)));
      }
;


assignExpr: ID ASSIGN expr
  { $$ = new Assign(@2, new Identifier(@1, $1), $3); }
;

whileExpr: WHILE expr DO expr { $$ = new WhileLoop(@1, $2, $4); }
;

forExpr: FOR ID ASSIGN expr TO expr DO expr
  { $$ = new ForLoop(@1, new VarDecl(@2, $2, $4, boost::none, true), $6, $8); }
;

breakExpr: BREAK { $$ = new Break(@1); }
;

letExpr: LET decls IN exprs END
  { $$ = new Let(@1, $2, new Sequence(nl, $4)); }
;

ifThenElseExpr: IF expr THEN expr ELSE expr
  { $$ = new IfThenElse(@2, $2, $4, $6); }
;

ifThenExpr: IF expr THEN expr
  { $$ = new IfThenElse(@2, $2, $4, new Sequence(nl, std::vector<Expr *>())); }
;

seqExpr : LPAREN exprs RPAREN { $$ = new Sequence(@1, $2); }
;

exprs: { $$ = std::vector<Expr *>(); }
  | nonemptyexprs { $$ = $1; }
;

nonemptyexprs: expr { $$ = std::vector<Expr *>({$1}); }
  | nonemptyexprs SEMICOLON expr
  {
    $$ = std::move($1);
    $$.push_back($3);
  }
;

arguments: { $$ = std::vector<Expr *>(); }
  | nonemptyarguments { $$ = $1; }
;

nonemptyarguments: expr { $$ = std::vector<Expr *>({$1}); }
  | nonemptyarguments COMMA expr
  {
    $$ = std::move($1);
    $$.push_back($3);
  }
;

params: { $$ = std::vector<VarDecl *>(); }
  | nonemptyparams { $$ = $1; }
;

nonemptyparams: param { $$ = std::vector<VarDecl *>({$1}); }
  | nonemptyparams COMMA param
  {
    $$ = std::move($1);
    $$.push_back($3);
  }
;

decls: { $$ = std::vector<Decl *>();}
  | decls decl
  {
    $$ = std::move($1);
    $$.push_back($2);
  }
;

param: ID COLON ID { $$ = new VarDecl(@1, $1, nullptr, $3); }
;

typeannotation: { $$ = boost::none; }
  | COLON ID { $$ = Symbol($2); }
;

%%

void
yy::tiger_parser::error (const location_type& l,
                          const std::string& m)
{
  utils::error (l, m);
}
