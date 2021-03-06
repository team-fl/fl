#include <string>
#include <set>
#include <list>
#include <inc/references.h>
#include <parser/ast/enum.h>
#include <parser/ast/list.h>
#include <parser/ast/symbol.h>
#include <interpreter/obj/enum_object.h>
#include <interpreter/obj/class_object.h>
#include <interpreter/context.h>
#include <interpreter/eval_exception.h>

using std::string;
using std::list;
using std::set;

enum_node::enum_node(const string& name, const astref& pDefList)
    :_def_list(pDefList), _name(name)
{
}

objref enum_node::raw_evaluate(context* pContext)
{
    // Create a new class that derives from enum

    // First check the inputs
    auto pList = dynamic_cast<list_node*>(_def_list.get());

    for ( auto e : pList->raw_elements() )
    {
	if ( e->type()!=asttype::symbol )
	    throw eval_exception(cerror::enum_value_not_a_symbol,
				 "Invalid list of symbols provided to defenum" );
    }
    
    auto enum_cls = builtins::flenum::get_class();

    typespec tsn(_name);
    fclass* pNewClass = new fclass(tsn,enum_cls,false,false,false,true);

    // Create class attributes
    int index=0;

    for ( auto e : pList->raw_elements() )
    {
	auto pSym = std::dynamic_pointer_cast<symbol_node>(e);
	enumref i = enumref( new enum_object(pContext,index++,pSym->name(),*pNewClass) );
	pNewClass->add_class_attribute(pSym->name(),i);
    }

    // Register the new class with the type manager and return a class_object
    pContext->types()->add(*pNewClass);

    auto clsObj = classref( new class_object(pContext,pNewClass) );
    pContext->assign(_name,clsObj);
    return clsObj;
}

void enum_node::required_symbols(set<symspec>& s) const
{
    _def_list->required_symbols(s);
}

void enum_node::render_dot(int& uuid,
			   const string& parent, 
			   const string& label,
			   std::ostream& out) const
{
    int myref=uuid++;
    string labelString;

    string myid = "enumdef_" + _name + "_" + std::to_string(myref);
    if (!parent.empty())
    {
	out << parent << " -> ";
	labelString = ",label=\"" + label + "\"";
    }
    out << myid << "[shape=box" << labelString << "];" << std::endl;

    _def_list->render_dot(uuid,myid," cond",out);
}

asttype enum_node::type() const
{
    return asttype::_enum;
}

void enum_node::direct_subordinates( list<astref>& s) const
{
    s.push_back(_def_list);
}
