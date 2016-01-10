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
    virtual ast* make_fundef( std::string* name, ast* def) const=0;
    virtual ast* make_symbol( std::string* name) const=0;
    virtual ast* make_assign_node(ast* lvalue, ast* rvalue)=0;
    virtual ast* make_methodcall( ast* target, std::string* method,list_node* args)=0;
    virtual ast* start_list()=0;
    virtual void push_list_element(ast*)=0;
    virtual ast* finish_list()=0;
    virtual void respond( ast* def, std::ostream& os = std::cout ) const=0;
    virtual ast* make_bool(bool b)=0;
};

#endif
