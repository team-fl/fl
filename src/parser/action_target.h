#ifndef ACTION_TARGET_H
#define ACTION_TARGET_H

#include <string>
#include <iostream>

class ast;
class list_node;

class action_target
{
 
 public:

    virtual ast* make_int(int x) const=0;
    virtual ast* make_null() const=0;
    virtual ast* make_fundef( ast* arglist,  ast* def) const=0;
    virtual ast* make_funcall( ast* fn,  ast* args) const=0;
    virtual ast* make_ifnode( ast* condExpr,  ast* trueExpr, ast* falseExpr) const=0;
    virtual ast* make_symbol( std::string* name) const=0;
    virtual ast* make_alias(ast* alias, ast* existing) const=0;
    virtual ast* make_assign_node(ast* lvalue, ast* rvalue)=0;
    virtual ast* make_methodcall( ast* target, std::string* method,list_node* args)=0;
    virtual ast* start_list()=0;
    virtual void push_list_element(ast*)=0;
    virtual ast* finish_list()=0;
    virtual ast* make_empty_list()=0;
    virtual ast* make_single_list(ast*)=0;
    virtual void respond( ast* def, std::ostream& os = std::cout ) const=0;
    virtual ast* make_bool(bool b)=0;
    virtual ast* build_list( ast* list, ast* element)=0;
    virtual void render(ast* node)=0;
    virtual void enable_debug(bool enable=true)=0;
    virtual void enable_trace(bool enable=true)=0;
};

#endif
