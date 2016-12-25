#ifndef SASS_PARSER_H
#define SASS_PARSER_H

#include <string>
#include <vector>

#include "ast.hpp"
#include "position.hpp"
#include "context.hpp"
#include "position.hpp"
#include "prelexer.hpp"

struct Lookahead {
  const char* found;
  const char* error;
  const char* position;
  bool parsable;
  bool has_interpolants;
};

namespace Sass {

  class Parser : public ParserState {
  public:

    enum Scope { Root, Mixin, Function, Media, Control, Properties, Rules };

    Context& ctx;
    std::vector<Block*> block_stack;
    std::vector<Scope> stack;
    Media_Block* last_media_block;
    const char* source;
    const char* position;
    const char* end;
    Position before_token;
    Position after_token;
    ParserState pstate;
    int indentation;


    Token lexed;
    bool in_at_root;

    Parser(Context& ctx, const ParserState& pstate)
    : ParserState(pstate), ctx(ctx), block_stack(0), stack(0), last_media_block(0),
      source(0), position(0), end(0), before_token(pstate), after_token(pstate), pstate(pstate), indentation(0)
    { in_at_root = false; stack.push_back(Scope::Root); }

    // static Parser from_string(const std::string& src, Context& ctx, ParserState pstate = ParserState("[STRING]"));
    static Parser from_c_str(const char* src, Context& ctx, ParserState pstate = ParserState("[CSTRING]"), const char* source = 0);
    static Parser from_c_str(const char* beg, const char* end, Context& ctx, ParserState pstate = ParserState("[CSTRING]"), const char* source = 0);
    static Parser from_token(Token t, Context& ctx, ParserState pstate = ParserState("[TOKEN]"), const char* source = 0);
    // special static parsers to convert strings into certain selectors
    static Selector_List* parse_selector(const char* src, Context& ctx, ParserState pstate = ParserState("[SELECTOR]"), const char* source = 0);

#ifdef __clang__

    // lex and peak uses the template parameter to branch on the action, which
    // triggers clangs tautological comparison on the single-comparison
    // branches. This is not a bug, just a merging of behaviour into
    // one function

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"

#endif


    bool peek_newline(const char* start = 0);

    // skip over spaces, tabs and line comments
    template <Prelexer::prelexer mx>
    const char* sneak(const char* start = 0)
    {
      using namespace Prelexer;

      // maybe use optional start position from arguments?
      const char* it_position = start ? start : position;

      // skip white-space?
      if (mx == spaces ||
          mx == no_spaces ||
          mx == css_comments ||
          mx == css_whitespace ||
          mx == optional_spaces ||
          mx == optional_css_comments ||
          mx == optional_css_whitespace
      ) {
        return it_position;
      }

      // skip over spaces, tabs and sass line comments
      const char* pos = optional_css_whitespace(it_position);
      // always return a valid position
      return pos ? pos : it_position;

    }

    // peek will only skip over space, tabs and line comment
    // return the position where the lexer match will occur
    template <Prelexer::prelexer mx>
    const char* match(const char* start = 0)
    {
      // match the given prelexer
      return mx(position);
    }

    // peek will only skip over space, tabs and line comment
    // return the position where the lexer match will occur
    template <Prelexer::prelexer mx>
    const char* peek(const char* start = 0)
    {

      // sneak up to the actual token we want to lex
      // this should skip over white-space if desired
      const char* it_before_token = sneak < mx >(start);

      // match the given prelexer
      const char* match = mx(it_before_token);

      // check if match is in valid range
      return match <= end ? match : 0;

    }

    // white-space handling is built into the lexer
    // this way you do not need to parse it yourself
    // some matchers don't accept certain white-space
    // we do not support start arg, since we manipulate
    // sourcemap offset and we modify the position pointer!
    // lex will only skip over space, tabs and line comment
    template <Prelexer::prelexer mx>
    const char* lex(bool lazy = true, bool force = false)
    {

      if (*position == 0) return 0;

      // position considered before lexed token
      // we can skip whitespace or comments for
      // lazy developers (but we need control)
      const char* it_before_token = position;

      // sneak up to the actual token we want to lex
      // this should skip over white-space if desired
      if (lazy) it_before_token = sneak < mx >(position);

      // now call matcher to get position after token
      const char* it_after_token = mx(it_before_token);

      // check if match is in valid range
      if (it_after_token > end) return 0;

      // maybe we want to update the parser state anyway?
      if (force == false) {
        // assertion that we got a valid match
        if (it_after_token == 0) return 0;
        // assertion that we actually lexed something
        if (it_after_token == it_before_token) return 0;
      }

      // create new lexed token object (holds the parse results)
      lexed = Token(position, it_before_token, it_after_token);

      // advance position (add whitespace before current token)
      before_token = after_token.add(position, it_before_token);

      // update after_token position for current token
      after_token.add(it_before_token, it_after_token);

      // ToDo: could probably do this incremetal on original object (API wants offset?)
      pstate = ParserState(path, source, lexed, before_token, after_token - before_token);

      // advance internal char iterator
      return position = it_after_token;

    }

    // lex_css skips over space, tabs, line and block comment
    // all block comments will be consumed and thrown away
    // source-map position will point to token after the comment
    template <Prelexer::prelexer mx>
    const char* lex_css()
    {
      // copy old token
      Token prev = lexed;
      // store previous pointer
      const char* oldpos = position;
      Position bt = before_token;
      Position at = after_token;
      ParserState op = pstate;
      // throw away comments
      // update srcmap position
      lex < Prelexer::css_comments >();
      // now lex a new token
      const char* pos = lex< mx >();
      // maybe restore prev state
      if (pos == 0) {
        pstate = op;
        lexed = prev;
        position = oldpos;
        after_token = at;
        before_token = bt;
      }
      // return match
      return pos;
    }

    // all block comments will be skipped and thrown away
    template <Prelexer::prelexer mx>
    const char* peek_css(const char* start = 0)
    {
      // now peek a token (skip comments first)
      return peek< mx >(peek < Prelexer::css_comments >(start));
    }

#ifdef __clang__

#pragma clang diagnostic pop

#endif

    void error(std::string msg, Position pos);
    // generate message with given and expected sample
    // text before and in the middle are configurable
    void css_error(const std::string& msg,
                   const std::string& prefix = " after ",
                   const std::string& middle = ", was: ");
    void read_bom();

    Block* parse();
    Import* parse_import();
    Definition* parse_definition(Definition::Type which_type);
    Parameters* parse_parameters();
    Parameter* parse_parameter();
    Mixin_Call* parse_include_directive();
    Arguments* parse_arguments();
    Argument* parse_argument();
    Assignment* parse_assignment();
    // Propset* parse_propset();
    Ruleset* parse_ruleset(Lookahead lookahead, bool is_root = false);
    Selector_Schema* parse_selector_schema(const char* end_of_selector);
    Selector_List* parse_selector_list(bool at_root = false);
    Complex_Selector* parse_complex_selector(bool in_root = true);
    Compound_Selector* parse_compound_selector();
    Simple_Selector* parse_simple_selector();
    Wrapped_Selector* parse_negated_selector();
    Simple_Selector* parse_pseudo_selector();
    Attribute_Selector* parse_attribute_selector();
    Block* parse_block(bool is_root = false);
    Block* parse_css_block(bool is_root = false);
    bool parse_block_nodes(bool is_root = false);
    bool parse_block_node(bool is_root = false);

    bool parse_number_prefix();
    Declaration* parse_declaration();
    Expression* parse_map_value();
    Expression* parse_map();
    Expression* parse_list();
    Expression* parse_comma_list();
    Expression* parse_space_list();
    Expression* parse_disjunction();
    Expression* parse_conjunction();
    Expression* parse_relation();
    Expression* parse_expression();
    Expression* parse_operators();
    Expression* parse_factor();
    Expression* parse_value2();
    Expression* parse_value();
    Function_Call* parse_calc_function();
    Function_Call* parse_function_call();
    Function_Call_Schema* parse_function_call_schema();
    String* parse_url_function_string();
    String* parse_url_function_argument();
    String* parse_interpolated_chunk(Token, bool constant = false);
    String* parse_string();
    String_Constant* parse_static_expression();
    // String_Constant* parse_static_property();
    String_Constant* parse_static_value();
    String* parse_ie_property();
    String* parse_ie_keyword_arg();
    String_Schema* parse_value_schema(const char* stop);
    String* parse_identifier_schema();
    // String_Schema* parse_url_schema();
    If* parse_if_directive(bool else_if = false);
    For* parse_for_directive();
    Each* parse_each_directive();
    While* parse_while_directive();
    Return* parse_return_directive();
    Content* parse_content_directive();
    void parse_charset_directive();
    Media_Block* parse_media_block();
    List* parse_media_queries();
    Media_Query* parse_media_query();
    Media_Query_Expression* parse_media_expression();
    Supports_Block* parse_supports_directive();
    Supports_Condition* parse_supports_condition();
    Supports_Condition* parse_supports_negation();
    Supports_Condition* parse_supports_operator();
    Supports_Condition* parse_supports_interpolation();
    Supports_Condition* parse_supports_declaration();
    Supports_Condition* parse_supports_condition_in_parens();
    At_Root_Block* parse_at_root_block();
    At_Root_Query* parse_at_root_query();
    String_Schema* parse_almost_any_value();
    Directive* parse_special_directive();
    Directive* parse_prefixed_directive();
    Directive* parse_directive();
    Warning* parse_warning();
    Error* parse_error();
    Debug* parse_debug();

    // be more like ruby sass
    Expression* lex_almost_any_value_token();
    Expression* lex_almost_any_value_chars();
    Expression* lex_interp_string();
    Expression* lex_interp_uri();
    Expression* lex_interpolation();

    // these will throw errors
    Token lex_variable();
    Token lex_identifier();

    void parse_block_comments();

    Lookahead lookahead_for_value(const char* start = 0);
    Lookahead lookahead_for_selector(const char* start = 0);
    Lookahead lookahead_for_include(const char* start = 0);

    Expression* fold_operands(Expression* base, std::vector<Expression*>& operands, Operand op);
    Expression* fold_operands(Expression* base, std::vector<Expression*>& operands, std::vector<Operand>& ops, size_t i = 0);

    void throw_syntax_error(std::string message, size_t ln = 0);
    void throw_read_error(std::string message, size_t ln = 0);


    template <Prelexer::prelexer open, Prelexer::prelexer close>
    Expression* lex_interp()
    {
      if (lex < open >(false)) {
        String_Schema* schema = SASS_MEMORY_NEW(ctx.mem, String_Schema, pstate);
        // std::cerr << "LEX [[" << std::string(lexed) << "]]\n";
        *schema << SASS_MEMORY_NEW(ctx.mem, String_Constant, pstate, lexed);
        if (position[0] == '#' && position[1] == '{') {
          Expression* itpl = lex_interpolation();
          if (itpl) *schema << itpl;
          while (lex < close >(false)) {
            // std::cerr << "LEX [[" << std::string(lexed) << "]]\n";
            *schema << SASS_MEMORY_NEW(ctx.mem, String_Constant, pstate, lexed);
            if (position[0] == '#' && position[1] == '{') {
              Expression* itpl = lex_interpolation();
              if (itpl) *schema << itpl;
            } else {
              return schema;
            }
          }
        } else {
          return SASS_MEMORY_NEW(ctx.mem, String_Constant, pstate, lexed);
        }
      }
      return 0;
    }
  };

  size_t check_bom_chars(const char* src, const char *end, const unsigned char* bom, size_t len);
}

#endif
