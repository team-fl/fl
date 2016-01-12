#ifndef AST_NODES_H
#define AST_NODES_H

#include <parser/callable.h>

class context;

class symbol_node : public ast
{
public:
    symbol_node( const std::string& );
    virtual objref evaluate(context*);
    virtual objref evaluate(context*) const;
    virtual fclass* type(context*) const;
    virtual const std::string& name() const;
protected:
    const std::string _name;

};

class literal_node : public ast
{
public:
    literal_node( objref );
    virtual objref evaluate(context*);
    virtual objref evaluate(context*) const;
    virtual fclass* type(context*) const;
protected:
    const objref _object;
};

class list_node : public ast
{
public:
    virtual void push_element(ast*);
    virtual objref evaluate(context*);    
    virtual objref evaluate(context*) const;
    virtual fclass* type(context*) const;
    virtual std::list<ast*>& raw_elements();
private:
    std::list<ast*> _elements;
};

class methodcall_node : public ast
{
public:
    methodcall_node(const std::string&);
    virtual objref evaluate(context*);
    virtual objref evaluate(context*) const;
    virtual fclass* type(context*) const;

    virtual void add_target(ast* pObj);
    virtual void add_param(ast*);
    virtual void finalize_params();

protected:
    const std::string _name;
    ast* _target;
    std::list<ast*> _params;
};

class fundef_node : public ast
{
public:
    fundef_node(ast* arglist, ast* definition);
    virtual objref evaluate(context*) const;
    virtual fclass* type(context*);

protected:
    ast* _arglist;
    ast* _definition;
};

class assign_node : public ast
{
public:
    assign_node(ast*,ast*,bool alias=false);
    virtual objref evaluate(context*);
    virtual objref evaluate(context*) const;
    virtual fclass* type(context*) const;

private:
    ast* _lvalue;
    ast* _rvalue;
    const bool _alias;
};

#endif