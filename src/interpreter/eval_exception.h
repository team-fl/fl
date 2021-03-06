#ifndef EVAL_EXCEPTION_H
#define EVAL_EXCEPTION_H

#include <string>
#include <stdexcept>

class ast;

enum class cerror
{
    internal_error =   0,
	syntax_error = 1,
	undefined_symbol = 2,
	symbol_redefinition=3,
	instantiate_abstract=4,
	undefined_method=5,
	override_sealed=6,
	missing_attribute=7,
	invalid_assignment=8,
	invalid_lvalue=9,
	selector_nomatch=10,
	invalid_index=11,
	index_out_of_bounds=12,
	unsupported_argument=13,
	equate_float=14,
	too_many_arguments=15,
	unsupported_conversion=16,
	not_a_class=17,
        incorrect_arg_count=18,
	no_construction=19,
	sealed_class=20,
	no_multiple_inheritance=21,
	invalid_argument=22,
	enum_value_not_a_symbol=23,
	unsupported_feature=24,
	unhandled_signal=25,
	not_rangeable=26
};

class eval_exception : public std::runtime_error
{

 public:
    eval_exception(cerror errCode, const std::string& arg, bool fatal=false, ast* srcNode=nullptr);
    eval_exception( const eval_exception& e );
    virtual ~eval_exception();
    virtual cerror code() const;
    virtual const char* what() const noexcept;
    bool fatal() const { return _fatal; }

    ast* source_node() const { return _source_node; }

 protected:

 private:
    const cerror _errcode;
    const bool _fatal;
    ast* const _source_node;
};

class terminate_exception : public std::runtime_error
{
public:
    terminate_exception();

private:
    static const std::string _what;
};

#endif
