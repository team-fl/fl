#include <string>
#include <vector>
#include <cassert>
#include <random>
#include <deque>
#include "builtins.h"
#include <interpreter/class.h>
#include <interpreter/context.h>
#include <interpreter/object.h>
#include <parser/callable.h>
#include <parser/ast_nodes.h>
#include <logger/logger.h>
#include <interpreter/eval_exception.h>

using std::string;
using std::vector;
using std::deque;

#define N_INT(x) (x->internal_value())

namespace builtins
{
    void build_globals(context* pContext)
    {
	wlog_entry();
	typespec fnspec("function",{});
	fclass& fncls = pContext->types()->lookup(fnspec);

	deque<std::string> args;
	args.push_back("a"); 
	args.push_back("b");

	pContext->assign("rnd", 
			 fnref( new fn_object(pContext,fncls,
					      make_marshall(&builtins::rnd),args,{}) ) );
    }

    std::shared_ptr<fclass> object::build_class()
    {
	typespec spec("object",{});
	std::shared_ptr<fclass> pCls(new fclass(spec,nullptr));
	pCls->add_method( {"dump", make_marshall_mthd(&builtins::obj_dump),false});
	pCls->add_method( {"class", make_marshall_mthd(&builtins::obj_class)} );
	pCls->add_method( {".ctor", make_marshall_mthd(&builtins::obj_ctor),true});
	pCls->add_method( {".assign", make_marshall_mthd(&builtins::obj_assign),false});
	pCls->add_method( {"eq", make_marshall_mthd(&builtins::obj_equate),false} );
	pCls->add_method( {"is", make_marshall_mthd(&builtins::obj_is),true } );
	pCls->add_method( {"invoke", make_marshall_mthd(&builtins::obj_invoke),true} );
	return pCls;
    }

    std::shared_ptr<fclass> flclass::build_class(typemgr* pTm)
    {
	typespec base_spec("object",{});
	fclass& base_cls = pTm->lookup(base_spec);

	typespec spec("class",{});
	std::shared_ptr<fclass> pCls(new fclass(spec,&base_cls));

	pCls->add_method({"addmethod",make_marshall_mthd(&builtins::class_addmethod)});
	pCls->add_method({"methods",make_marshall_mthd(&builtins::class_methods)});
	pCls->add_method({"base",make_marshall_mthd(&builtins::class_base)});
	pCls->add_method({"derive",make_marshall_mthd(&builtins::class_derive),true});
	pCls->add_method({"new",make_marshall_mthd(&builtins::class_new),true});
	pCls->add_method({"addattr",make_marshall_mthd(&builtins::class_addattr),true});
	return pCls;
    }

    std::shared_ptr<fclass> string::build_class(typemgr* pTm)
    {
	typespec base_spec("object",{});
	fclass& base_cls = pTm->lookup(base_spec);

	typespec spec("string",{});
	std::shared_ptr<fclass> pCls(new fclass(spec,&base_cls));
	pCls->add_method({"size",make_marshall_mthd(&builtins::string_length)});
	return pCls;
    }

    std::shared_ptr<fclass> integer::build_class(typemgr* pTm)
    {
	typespec base_spec("object",{});
	fclass& base_cls = pTm->lookup(base_spec);

	typespec spec("integer",{});
	std::shared_ptr<fclass> pCls(new fclass(spec,&base_cls));
	pCls->add_method({"add", make_marshall_mthd(&builtins::add_integers)});
	pCls->add_method({"in_range", make_marshall_mthd(&builtins::in_range_integers)});
	pCls->add_method({"gt", make_marshall_mthd(&builtins::int_gt)});
	pCls->add_method({"lt", make_marshall_mthd(&builtins::int_lt)});
	pCls->add_method({"dec", make_marshall_mthd(&builtins::int_dec)});
	pCls->add_method({"div", make_marshall_mthd(&builtins::int_div)});
	pCls->add_method({"mod", make_marshall_mthd(&builtins::int_mod)});
	return pCls;
    }

    std::shared_ptr<fclass> flvoid::build_class(typemgr* pTm)
    {
	typespec base_spec("object",{});
	fclass& base_cls = pTm->lookup(base_spec);

	typespec spec("void",{});
	std::shared_ptr<fclass> pCls(new fclass(spec,&base_cls));
	return pCls;
    }

    std::shared_ptr<fclass> list::build_class(typespec spec, typemgr* pTm)
    {
	assert( spec.params().size()>0);

	typespec base_spec("object",{});
	typespec listobj_spec("list",{base_spec});

	if ( spec==listobj_spec)
	{
	    fclass& base_cls = pTm->lookup(base_spec);
	    std::shared_ptr<fclass> pCls(new fclass(spec,&base_cls));
	    pCls->add_method({"size", make_marshall_mthd(&builtins::list_size)});
	    pCls->add_method({"head", make_marshall_mthd(&builtins::list_head)});
	    pCls->add_method({"append", make_marshall_mthd(&builtins::list_append)});
	    pCls->add_method({"prepend", make_marshall_mthd(&builtins::list_prepend)});
	    pCls->add_method({"tail", make_marshall_mthd(&builtins::list_tail)});
	    pCls->add_method({"duplicate_and_append", make_marshall_mthd(&builtins::list_dup_and_append)});
	    return pCls;
	}
	else
	{
	    fclass& base_cls = pTm->lookup(listobj_spec);
	    std::shared_ptr<fclass> pCls(new fclass(spec,&base_cls));
	    return pCls;
	}
    }

    std::shared_ptr<fclass> function::build_class(typespec spec, typemgr* pTm)
    {
	typespec base_spec("object",{});
	fclass& base_cls = pTm->lookup(base_spec);
	std::shared_ptr<fclass> pCls(new fclass(spec,&base_cls));
	return pCls;
    }

    std::shared_ptr<fclass> boolean::build_class(typemgr* pTm)
    {
	typespec base_spec("object",{});
	fclass& base_cls = pTm->lookup(base_spec);

	typespec spec("boolean",{});
	std::shared_ptr<fclass> pCls(new fclass(spec,&base_cls));
	pCls->add_method({"not", make_marshall_mthd(&builtins::logical_not)});
	return pCls;
    }

    objref add_integers(context* pContext, intref a,intref b)
    {
	const int result = a->internal_value() + b->internal_value();
	objref pObject(new int_object(pContext,result));
    
	return pObject;
    }

    objref in_range_integers(context* pContext, intref pThis, intref min, intref max)
    {
	bool in_range = (N_INT(pThis) >= N_INT(min)) && ( N_INT(pThis) <= N_INT(max));
	typespec bool_spec = typespec("boolean",{});
	fclass& bool_cls = pContext->types()->lookup(bool_spec);
	objref pObject( new bool_object(pContext, (in_range)?true:false, bool_cls));
	return pObject;
    }

    objref logical_not(context* pContext,boolref a)
    {
	typespec bool_spec = typespec("boolean",{});
	fclass& bool_cls = pContext->types()->lookup(bool_spec);
	objref pObject( new bool_object(pContext, !a->internal_value(), bool_cls ));
	return pObject;

    }
    
    objref list_size(context* pContext, listref pThis)
    {
	objref pObject(new int_object(pContext,pThis->internal_value().size()));
	return pObject;
    }

    objref list_head(context* pContext, listref pThis)
    {
	return pThis->first();
    }

    objref list_append(context* pContext, listref pThis, objref e)
    {
	pThis->append(e);
	return pThis;
    }

    objref list_prepend(context* pContext, listref pThis, objref e)
    {
	pThis->prepend(e);
	return pThis;
    }

    objref list_tail(context* pContext, listref pThis)
    {
	return pThis->tail(pContext);
    }

    objref int_gt(context* pContext, intref pThis, intref pOther )
    {
	bool result = pThis->internal_value() > pOther->internal_value();
	typespec ts("boolean",{});
	return boolref(new bool_object(pContext,result,pContext->types()->lookup(ts)));

    }

    objref int_lt(context* pContext, intref pThis, intref pOther )
    {
	bool result = pThis->internal_value() < pOther->internal_value();
	typespec ts("boolean",{});
	return boolref(new bool_object(pContext,result,pContext->types()->lookup(ts)));

    }

    objref int_dec(context* pContext,intref pThis)
    {
	return intref( new int_object(pContext, pThis->internal_value()-1));

	
    }

    objref int_div(context* pContext, intref pThis, intref divisor)
    {
	return intref( new int_object(pContext, N_INT(pThis)/N_INT(divisor)));

    }
    
    objref int_mod(context* pContext, intref pThis, intref modulus)
    {
	return intref( new int_object(pContext, N_INT(pThis)%N_INT(modulus)));
    }

    objref obj_dump(context*, objref pThis)
    {
	pThis->dump(std::cout);
	return pThis;
    }

    objref obj_class(context* pContext, objref pThis)
    {
	typespec ts("class",{});
	class_object* pClass = 
	    new class_object(pContext,&pThis->get_class(),pContext->types()->lookup(ts));
	return objref(pClass);
    }

    objref list_dup_and_append(context* pContext, listref pThis, objref pElement)
    {
	list_object* pNewList = new list_object(pContext, pThis->get_class(),
						pThis->internal_value() );

	pNewList->append(pElement);
	return objref(pNewList);
    }

    objref string_length(context* pContext, stringref pThis )
    {
	// Create a new integer
	int_object* pResult = new int_object( pContext, pThis->internal_value().length() );

	return objref(pResult);
    }

    objref class_methods(context* pContext, classref pThis)
    {
	// Typespecs
	typespec string_ts("string",{});
	typespec list_ts("list",{string_ts});
	fclass& string_cls = pContext->types()->lookup(string_ts);

	// Create a native list of string_objects
	std::list<objref> nativeList;

	// Add all the methods of the given class
	for ( auto m : pThis->internal_value()->methods() )
	{
	    string_object* pString = new string_object(pContext, m,string_cls);
	    nativeList.push_back( objref(pString) );
	}

	// Create a new list
	list_object* pList = new list_object( pContext, pContext->types()->lookup(list_ts),
					      nativeList);

	// return a managed reference
	return objref(pList);

    }

    objref class_base(context* pContext, classref pThis)
    {
	fclass* pInternalClass = pThis->internal_value();
	fclass* baseClass = pInternalClass->base();

	if ( pInternalClass->is_root() )
	{
	    // There is no base, return void
	    typespec ts("void",{});
	    return objref( new void_object(pContext, pContext->types()->lookup(ts)));
	}
	else
	{
	    // There is a base!
	    typespec spec("class",{});
	    return objref(new class_object(pContext, baseClass,pContext->types()->lookup(spec)));
	}
	
    }

    objref obj_ctor(context* pContext, objref pThis)
    {
	return pThis;
    }

    objref class_addmethod(context* pContext, classref pThis, fnref  fn, stringref name)
    {
	// Construct a lambda which executes the method on the object
	auto le = [fn](context* pContext, objref pThis, std::vector<ast*>& params)
	    {
		// Evaluate each parameter,ignoring the first two
		vector<objref> evaled_params;
		evaled_params.push_back(pThis);
		int guard=0;
		for ( auto arg : params )
		{
		    if (guard++>1)
			evaled_params.push_back( arg->evaluate(pContext) );
		}
		return (*fn)(pContext,evaled_params);
	    };

	// Add the method to the class
	fclass* pInternalClass = pThis->internal_value();
	pInternalClass->add_method({name->internal_value(), le});

	// Return a reference to the class object
	return pThis;
    }

    objref class_derive(context* pContext, classref pThis, stringref name)
    {
	typespec ts(name->internal_value(),{});
	typespec tsc("class",{});
	fclass* pNewNativeClass = new fclass(ts,pThis->internal_value());
	(pContext->types())->add(*pNewNativeClass);
	class_object*  pNewClass = 
	    new class_object(pContext, pNewNativeClass,(pContext->types())->lookup(tsc));

	return objref(pNewClass,[](class_object*){});
    }

    objref class_new(context* pContext, classref pThis, listref params)
    {
	vector<objref> evaled_params;
	for ( auto o : params->internal_value() )
	{
	    evaled_params.push_back(o);
	}
	::object* pObj = new ::object(pContext, *(pThis->internal_value()),evaled_params);
	return objref(pObj);
    }

    objref class_addattr(context* pContext, classref pThis, stringref name, objref d)
    {
	fclass* pNativeClass = pThis->internal_value();
	pNativeClass->add_attribute(name->internal_value(),d);
	return pThis;
    }

    objref obj_assign(context* pContext, objref pThis, objref pOther)
    {
	(*pThis)=pOther;
    }
    
    objref obj_equate(context* pContext, objref pThis,objref pOther)
    {
	bool result = (*pThis)==pOther;
	typespec ts("boolean",{});
	return boolref(new bool_object(pContext, result,pContext->types()->lookup(ts)));
    }

    objref rnd(context* pContext, intref a, intref b)
    {
	int lower = a->internal_value();
	int upper = b->internal_value();

	static std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(lower,upper);

	return objref( new int_object(pContext, distribution(generator)));
    }

    objref obj_is(context* pContext,objref pThis, objref pOther)
    {
	bool result = pThis.get()==pOther.get();
	typespec ts("boolean",{});
	return boolref(new bool_object(pContext,result,pContext->types()->lookup(ts)));
    }

    objref obj_invoke(context* pContext,objref pThis,stringref name, listref params)
    {
	vector<objref> raw_params(params->internal_value().size());

	int index=0;
	for ( auto p : params->internal_value() )
	{
	    raw_params[index++] = p;
	}
	return pThis->invoke( name->internal_value(), pContext, raw_params );
    }
}
