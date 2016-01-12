#ifndef CALLABLE_H
#define CALLABLE_H

#include <tuple>
#include <vector>
#include <memory>
#include <interpreter/object.h>

class context;
class ast;

namespace
{
    template<typename F,typename T, size_t... I>
    decltype(auto) apply_impl(F&& f, T&& t, std::index_sequence<I...>)
    {
	return std::forward<F>(f)(std::get<I>(std::forward<T>(t))...);
    }

    template<typename F, typename T>
    decltype(auto) apply(F&& f, T&& t)
    {
	using Indices = std::make_index_sequence<std::tuple_size<std::decay_t<T>>::value>;
	return apply_impl(std::forward<F>(f), std::forward<T>(t), Indices{} );
    }


    template<std::size_t I=0, typename... P>
    inline typename std::enable_if<I==sizeof...(P),void>::type
    eval_each(std::array<ast*,sizeof...(P)>&, std::tuple<P...>&) {}

    template<std::size_t I=0, typename... P>
    inline typename std::enable_if<I < sizeof...(P), void>::type
    eval_each(std::array<ast*,sizeof...(P)>& p,std::tuple<P...>& t)
    {
	auto result = std::get<I>(p)->evaluate(nullptr);
	std::get<I>(t) = std::dynamic_pointer_cast<typename std::tuple_element<I,std::tuple<P...>>::type::element_type>(result);
	eval_each<I+1,P...>(p,t);
    }

    template<std::size_t I=0, typename... P>
    inline typename std::enable_if<I==sizeof...(P),void>::type
    v_eval_each(context*,std::vector<ast*>&, std::tuple<P...>&) {}

    template<std::size_t I=0, typename... P>
    inline typename std::enable_if<I < sizeof...(P), void>::type
    v_eval_each(context* c,std::vector<ast*>& p,std::tuple<P...>& t)
    {
	auto result = p[I-1]->evaluate(c);
	std::get<I>(t) = std::dynamic_pointer_cast<typename std::tuple_element<I,std::tuple<P...>>::type::element_type>(result);
	v_eval_each<I+1,P...>(c,p,t);
    }

    template<typename T> struct ArgTuple;

    template<typename R, typename ... Args>
    struct ArgTuple<R(Args...)>
    {
	typedef std::tuple<Args...> type;
    };

    template<typename F>
    std::function<marshall_fn_t> make_marshall( F* f)
    {
	return [f](context* pContext, std::vector<ast*>& p)
	{
	    typename ArgTuple<F>::type evaled_params;
	    std::get<0>(evaled_params) = pContext;
	    v_eval_each<1>(pContext,p,evaled_params);
	    return apply(*f,evaled_params);
	};
    }
}

class i_callable
{
public:
    virtual objref operator()(context*)=0;
};

#endif