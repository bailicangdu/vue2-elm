#ifndef SASS_DEBUGGER_H
#define SASS_DEBUGGER_H

#include <string>
#include <sstream>
#include "node.hpp"
#include "ast_fwd_decl.hpp"

using namespace Sass;

inline void debug_ast(AST_Node* node, std::string ind = "", Env* env = 0);

inline void debug_sources_set(SourcesSet& set, std::string ind = "")
{
  if (ind == "") std::cerr << "#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
  for(auto const &pair : set) {
    debug_ast(pair, ind + "");
    // debug_ast(set[pair], ind + "first: ");
  }
  if (ind == "") std::cerr << "#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
}

inline std::string str_replace(std::string str, const std::string& oldStr, const std::string& newStr)
{
  size_t pos = 0;
  while((pos = str.find(oldStr, pos)) != std::string::npos)
  {
     str.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
  return str;
}

inline std::string prettyprint(const std::string& str) {
  std::string clean = str_replace(str, "\n", "\\n");
  clean = str_replace(clean, "	", "\\t");
  clean = str_replace(clean, "\r", "\\r");
  return clean;
}

inline std::string longToHex(long long t) {
  std::stringstream is;
  is << std::hex << t;
  return is.str();
}

inline std::string pstate_source_position(AST_Node* node)
{
  std::stringstream str;
  Position start(node->pstate());
  Position end(start + node->pstate().offset);
  str << (start.file == std::string::npos ? -1 : start.file)
    << "@[" << start.line << ":" << start.column << "]"
    << "-[" << end.line << ":" << end.column << "]";
  return str.str();
}

inline void debug_ast(AST_Node* node, std::string ind, Env* env)
{
  if (node == 0) return;
  if (ind == "") std::cerr << "####################################################################\n";
  if (dynamic_cast<Bubble*>(node)) {
    Bubble* bubble = dynamic_cast<Bubble*>(node);
    std::cerr << ind << "Bubble " << bubble;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << bubble->tabs();
    std::cerr << std::endl;
  } else if (dynamic_cast<At_Root_Block*>(node)) {
    At_Root_Block* root_block = dynamic_cast<At_Root_Block*>(node);
    std::cerr << ind << "At_Root_Block " << root_block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << root_block->tabs();
    std::cerr << std::endl;
    debug_ast(root_block->expression(), ind + ":", env);
    debug_ast(root_block->block(), ind + " ", env);
  } else if (dynamic_cast<Selector_List*>(node)) {
    Selector_List* selector = dynamic_cast<Selector_List*>(node);
    std::cerr << ind << "Selector_List " << selector;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " <" << selector->hash() << ">";
    std::cerr << " [@media:" << selector->media_block() << "]";
    std::cerr << (selector->is_optional() ? " [is_optional]": " -");
    std::cerr << (selector->has_parent_ref() ? " [has-parent]": " -");
    std::cerr << (selector->has_line_break() ? " [line-break]": " -");
    std::cerr << (selector->has_line_feed() ? " [line-feed]": " -");
    std::cerr << std::endl;

    for(auto i : selector->elements()) { debug_ast(i, ind + " ", env); }

//  } else if (dynamic_cast<Expression*>(node)) {
//    Expression* expression = dynamic_cast<Expression*>(node);
//    std::cerr << ind << "Expression " << expression << " " << expression->concrete_type() << std::endl;

  } else if (dynamic_cast<Parent_Selector*>(node)) {
    Parent_Selector* selector = dynamic_cast<Parent_Selector*>(node);
    std::cerr << ind << "Parent_Selector " << selector;
//    if (selector->not_selector()) cerr << " [in_declaration]";
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " <" << selector->hash() << ">";
    std::cerr << " <" << prettyprint(selector->pstate().token.ws_before()) << ">" << std::endl;
//    debug_ast(selector->selector(), ind + "->", env);

  } else if (dynamic_cast<Complex_Selector*>(node)) {
    Complex_Selector* selector = dynamic_cast<Complex_Selector*>(node);
    std::cerr << ind << "Complex_Selector " << selector
      << " (" << pstate_source_position(node) << ")"
      << " <" << selector->hash() << ">"
      << " [weight:" << longToHex(selector->specificity()) << "]"
      << " [@media:" << selector->media_block() << "]"
      << (selector->is_optional() ? " [is_optional]": " -")
      << (selector->has_parent_ref() ? " [has parent]": " -")
      << (selector->has_line_feed() ? " [line-feed]": " -")
      << (selector->has_line_break() ? " [line-break]": " -")
      << " -- ";
      std::string del;
      switch (selector->combinator()) {
        case Complex_Selector::PARENT_OF:   del = ">"; break;
        case Complex_Selector::PRECEDES:    del = "~"; break;
        case Complex_Selector::ADJACENT_TO: del = "+"; break;
        case Complex_Selector::ANCESTOR_OF: del = " "; break;
        case Complex_Selector::REFERENCE:   del = "//"; break;
      }
      // if (del = "/") del += selector->reference()->perform(&to_string) + "/";
    std::cerr << " <" << prettyprint(selector->pstate().token.ws_before()) << ">" << std::endl;
    debug_ast(selector->head(), ind + " " /* + "[" + del + "]" */, env);
    if (selector->tail()) {
      debug_ast(selector->tail(), ind + "{" + del + "}", env);
    } else if(del != " ") {
      std::cerr << ind << " |" << del << "| {trailing op}" << std::endl;
    }
    SourcesSet set = selector->sources();
    // debug_sources_set(set, ind + "  @--> ");
  } else if (dynamic_cast<Compound_Selector*>(node)) {
    Compound_Selector* selector = dynamic_cast<Compound_Selector*>(node);
    std::cerr << ind << "Compound_Selector " << selector;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " <" << selector->hash() << ">";
    std::cerr << " [weight:" << longToHex(selector->specificity()) << "]";
    std::cerr << " [@media:" << selector->media_block() << "]";
    std::cerr << (selector->extended() ? " [extended]": " -");
    std::cerr << (selector->is_optional() ? " [is_optional]": " -");
    std::cerr << (selector->has_parent_ref() ? " [has-parent]": " -");
    std::cerr << (selector->has_line_break() ? " [line-break]": " -");
    std::cerr << (selector->has_line_feed() ? " [line-feed]": " -");
    std::cerr << " <" << prettyprint(selector->pstate().token.ws_before()) << ">" << std::endl;
    for(auto i : selector->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Propset*>(node)) {
    Propset* selector = dynamic_cast<Propset*>(node);
    std::cerr << ind << "Propset " << selector;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << selector->tabs() << std::endl;
    if (selector->block()) for(auto i : selector->block()->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Wrapped_Selector*>(node)) {
    Wrapped_Selector* selector = dynamic_cast<Wrapped_Selector*>(node);
    std::cerr << ind << "Wrapped_Selector " << selector;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " <" << selector->hash() << ">";
    std::cerr << " <<" << selector->ns_name() << ">>";
    std::cerr << (selector->is_optional() ? " [is_optional]": " -");
    std::cerr << (selector->has_parent_ref() ? " [has-parent]": " -");
    std::cerr << (selector->has_line_break() ? " [line-break]": " -");
    std::cerr << (selector->has_line_feed() ? " [line-feed]": " -");
    std::cerr << std::endl;
    debug_ast(selector->selector(), ind + " () ", env);
  } else if (dynamic_cast<Pseudo_Selector*>(node)) {
    Pseudo_Selector* selector = dynamic_cast<Pseudo_Selector*>(node);
    std::cerr << ind << "Pseudo_Selector " << selector;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " <" << selector->hash() << ">";
    std::cerr << " <<" << selector->ns_name() << ">>";
    std::cerr << (selector->is_optional() ? " [is_optional]": " -");
    std::cerr << (selector->has_parent_ref() ? " [has-parent]": " -");
    std::cerr << (selector->has_line_break() ? " [line-break]": " -");
    std::cerr << (selector->has_line_feed() ? " [line-feed]": " -");
    std::cerr << std::endl;
    debug_ast(selector->expression(), ind + " <= ", env);
  } else if (dynamic_cast<Attribute_Selector*>(node)) {
    Attribute_Selector* selector = dynamic_cast<Attribute_Selector*>(node);
    std::cerr << ind << "Attribute_Selector " << selector;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " <" << selector->hash() << ">";
    std::cerr << " <<" << selector->ns_name() << ">>";
    std::cerr << (selector->is_optional() ? " [is_optional]": " -");
    std::cerr << (selector->has_parent_ref() ? " [has-parent]": " -");
    std::cerr << (selector->has_line_break() ? " [line-break]": " -");
    std::cerr << (selector->has_line_feed() ? " [line-feed]": " -");
    std::cerr << std::endl;
    debug_ast(selector->value(), ind + "[" + selector->matcher() + "] ", env);
  } else if (dynamic_cast<Selector_Qualifier*>(node)) {
    Selector_Qualifier* selector = dynamic_cast<Selector_Qualifier*>(node);
    std::cerr << ind << "Selector_Qualifier " << selector;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " <" << selector->hash() << ">";
    std::cerr << " <<" << selector->ns_name() << ">>";
    std::cerr << (selector->is_optional() ? " [is_optional]": " -");
    std::cerr << (selector->has_parent_ref() ? " [has-parent]": " -");
    std::cerr << (selector->has_line_break() ? " [line-break]": " -");
    std::cerr << (selector->has_line_feed() ? " [line-feed]": " -");
    std::cerr << std::endl;
  } else if (dynamic_cast<Type_Selector*>(node)) {
    Type_Selector* selector = dynamic_cast<Type_Selector*>(node);
    std::cerr << ind << "Type_Selector " << selector;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " <" << selector->hash() << ">";
    std::cerr << " <<" << selector->ns_name() << ">>";
    std::cerr << (selector->is_optional() ? " [is_optional]": " -");
    std::cerr << (selector->has_parent_ref() ? " [has-parent]": " -");
    std::cerr << (selector->has_line_break() ? " [line-break]": " -");
    std::cerr << (selector->has_line_feed() ? " [line-feed]": " -");
    std::cerr << " <" << prettyprint(selector->pstate().token.ws_before()) << ">";
    std::cerr << std::endl;
  } else if (dynamic_cast<Selector_Placeholder*>(node)) {

    Selector_Placeholder* selector = dynamic_cast<Selector_Placeholder*>(node);
    std::cerr << ind << "Selector_Placeholder [" << selector->ns_name() << "] " << selector
      << " <" << selector->hash() << ">"
      << " [@media:" << selector->media_block() << "]"
      << (selector->is_optional() ? " [is_optional]": " -")
      << (selector->has_line_break() ? " [line-break]": " -")
      << (selector->has_line_feed() ? " [line-feed]": " -")
    << std::endl;

  } else if (dynamic_cast<Simple_Selector*>(node)) {
    Simple_Selector* selector = dynamic_cast<Simple_Selector*>(node);
    std::cerr << ind << "Simple_Selector " << selector;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << (selector->has_line_break() ? " [line-break]": " -") << (selector->has_line_feed() ? " [line-feed]": " -") << std::endl;

  } else if (dynamic_cast<Selector_Schema*>(node)) {
    Selector_Schema* selector = dynamic_cast<Selector_Schema*>(node);
    std::cerr << ind << "Selector_Schema " << selector;
    std::cerr << " (" << pstate_source_position(node) << ")"
      << (selector->at_root() && selector->at_root() ? " [@ROOT]" : "")
      << " [@media:" << selector->media_block() << "]"
      << (selector->has_line_break() ? " [line-break]": " -")
      << (selector->has_line_feed() ? " [line-feed]": " -")
    << std::endl;

    debug_ast(selector->contents(), ind + " ");
    // for(auto i : selector->elements()) { debug_ast(i, ind + " ", env); }

  } else if (dynamic_cast<Selector*>(node)) {
    Selector* selector = dynamic_cast<Selector*>(node);
    std::cerr << ind << "Selector " << selector;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << (selector->has_line_break() ? " [line-break]": " -")
      << (selector->has_line_feed() ? " [line-feed]": " -")
    << std::endl;

  } else if (dynamic_cast<Media_Query_Expression*>(node)) {
    Media_Query_Expression* block = dynamic_cast<Media_Query_Expression*>(node);
    std::cerr << ind << "Media_Query_Expression " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << (block->is_interpolated() ? " [is_interpolated]": " -")
    << std::endl;
    debug_ast(block->feature(), ind + " feature) ");
    debug_ast(block->value(), ind + " value) ");

  } else if (dynamic_cast<Media_Query*>(node)) {
    Media_Query* block = dynamic_cast<Media_Query*>(node);
    std::cerr << ind << "Media_Query " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << (block->is_negated() ? " [is_negated]": " -")
      << (block->is_restricted() ? " [is_restricted]": " -")
    << std::endl;
    debug_ast(block->media_type(), ind + " ");
    for(auto i : block->elements()) { debug_ast(i, ind + " ", env); }

  } else if (dynamic_cast<Media_Block*>(node)) {
    Media_Block* block = dynamic_cast<Media_Block*>(node);
    std::cerr << ind << "Media_Block " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
    debug_ast(block->media_queries(), ind + " =@ ");
    if (block->block()) for(auto i : block->block()->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Supports_Block*>(node)) {
    Supports_Block* block = dynamic_cast<Supports_Block*>(node);
    std::cerr << ind << "Supports_Block " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
    debug_ast(block->condition(), ind + " =@ ");
  } else if (dynamic_cast<Supports_Operator*>(node)) {
    Supports_Operator* block = dynamic_cast<Supports_Operator*>(node);
    std::cerr << ind << "Supports_Operator " << block;
    std::cerr << " (" << pstate_source_position(node) << ")"
    << std::endl;
    debug_ast(block->left(), ind + " left) ");
    debug_ast(block->right(), ind + " right) ");
  } else if (dynamic_cast<Supports_Negation*>(node)) {
    Supports_Negation* block = dynamic_cast<Supports_Negation*>(node);
    std::cerr << ind << "Supports_Negation " << block;
    std::cerr << " (" << pstate_source_position(node) << ")"
    << std::endl;
    debug_ast(block->condition(), ind + " condition) ");
  } else if (dynamic_cast<Supports_Declaration*>(node)) {
    Supports_Declaration* block = dynamic_cast<Supports_Declaration*>(node);
    std::cerr << ind << "Supports_Declaration " << block;
    std::cerr << " (" << pstate_source_position(node) << ")"
    << std::endl;
    debug_ast(block->feature(), ind + " feature) ");
    debug_ast(block->value(), ind + " value) ");
  } else if (dynamic_cast<Block*>(node)) {
    Block* root_block = dynamic_cast<Block*>(node);
    std::cerr << ind << "Block " << root_block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    if (root_block->is_root()) std::cerr << " [root]";
    std::cerr << " " << root_block->tabs() << std::endl;
    if (root_block->block()) for(auto i : root_block->block()->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Warning*>(node)) {
    Warning* block = dynamic_cast<Warning*>(node);
    std::cerr << ind << "Warning " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
  } else if (dynamic_cast<Error*>(node)) {
    Error* block = dynamic_cast<Error*>(node);
    std::cerr << ind << "Error " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
  } else if (dynamic_cast<Debug*>(node)) {
    Debug* block = dynamic_cast<Debug*>(node);
    std::cerr << ind << "Debug " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
    debug_ast(block->value(), ind + " ");
  } else if (dynamic_cast<Comment*>(node)) {
    Comment* block = dynamic_cast<Comment*>(node);
    std::cerr << ind << "Comment " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() <<
      " <" << prettyprint(block->pstate().token.ws_before()) << ">" << std::endl;
    debug_ast(block->text(), ind + "// ", env);
  } else if (dynamic_cast<If*>(node)) {
    If* block = dynamic_cast<If*>(node);
    std::cerr << ind << "If " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
    debug_ast(block->predicate(), ind + " = ");
    debug_ast(block->block(), ind + " <>");
    debug_ast(block->alternative(), ind + " ><");
  } else if (dynamic_cast<Return*>(node)) {
    Return* block = dynamic_cast<Return*>(node);
    std::cerr << ind << "Return " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
  } else if (dynamic_cast<Extension*>(node)) {
    Extension* block = dynamic_cast<Extension*>(node);
    std::cerr << ind << "Extension " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
    debug_ast(block->selector(), ind + "-> ", env);
  } else if (dynamic_cast<Content*>(node)) {
    Content* block = dynamic_cast<Content*>(node);
    std::cerr << ind << "Content " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [@media:" << block->media_block() << "]";
    std::cerr << " " << block->tabs() << std::endl;
  } else if (dynamic_cast<Import_Stub*>(node)) {
    Import_Stub* block = dynamic_cast<Import_Stub*>(node);
    std::cerr << ind << "Import_Stub " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [" << block->imp_path() << "] ";
    std::cerr << " " << block->tabs() << std::endl;
  } else if (dynamic_cast<Import*>(node)) {
    Import* block = dynamic_cast<Import*>(node);
    std::cerr << ind << "Import " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
    // std::vector<std::string>         files_;
    for (auto imp : block->urls()) debug_ast(imp, ind + "@: ", env);
    debug_ast(block->media_queries(), ind + "@@ ");
  } else if (dynamic_cast<Assignment*>(node)) {
    Assignment* block = dynamic_cast<Assignment*>(node);
    std::cerr << ind << "Assignment " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " <<" << block->variable() << ">> " << block->tabs() << std::endl;
    debug_ast(block->value(), ind + "=", env);
  } else if (dynamic_cast<Declaration*>(node)) {
    Declaration* block = dynamic_cast<Declaration*>(node);
    std::cerr << ind << "Declaration " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
    debug_ast(block->property(), ind + " prop: ", env);
    debug_ast(block->value(), ind + " value: ", env);
  } else if (dynamic_cast<Keyframe_Rule*>(node)) {
    Keyframe_Rule* has_block = dynamic_cast<Keyframe_Rule*>(node);
    std::cerr << ind << "Keyframe_Rule " << has_block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << has_block->tabs() << std::endl;
    if (has_block->selector()) debug_ast(has_block->selector(), ind + "@");
    if (has_block->block()) for(auto i : has_block->block()->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Directive*>(node)) {
    Directive* block = dynamic_cast<Directive*>(node);
    std::cerr << ind << "Directive " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [" << block->keyword() << "] " << block->tabs() << std::endl;
    debug_ast(block->selector(), ind + "~", env);
    debug_ast(block->value(), ind + "+", env);
    if (block->block()) for(auto i : block->block()->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Each*>(node)) {
    Each* block = dynamic_cast<Each*>(node);
    std::cerr << ind << "Each " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
    if (block->block()) for(auto i : block->block()->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<For*>(node)) {
    For* block = dynamic_cast<For*>(node);
    std::cerr << ind << "For " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
    if (block->block()) for(auto i : block->block()->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<While*>(node)) {
    While* block = dynamic_cast<While*>(node);
    std::cerr << ind << "While " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << block->tabs() << std::endl;
    if (block->block()) for(auto i : block->block()->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Definition*>(node)) {
    Definition* block = dynamic_cast<Definition*>(node);
    std::cerr << ind << "Definition " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [name: " << block->name() << "] ";
    std::cerr << " [type: " << (block->type() == Sass::Definition::Type::MIXIN ? "Mixin " : "Function ") << "] ";
    // this seems to lead to segfaults some times?
    // std::cerr << " [signature: " << block->signature() << "] ";
    std::cerr << " [native: " << block->native_function() << "] ";
    std::cerr << " " << block->tabs() << std::endl;
    debug_ast(block->parameters(), ind + " params: ", env);
    if (block->block()) for(auto i : block->block()->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Mixin_Call*>(node)) {
    Mixin_Call* block = dynamic_cast<Mixin_Call*>(node);
    std::cerr << ind << "Mixin_Call " << block << " " << block->tabs();
    std::cerr << " [" <<  block->name() << "]";
    std::cerr << " [has_content: " << block->has_content() << "] " << std::endl;
    debug_ast(block->arguments(), ind + " args: ");
    if (block->block()) for(auto i : block->block()->elements()) { debug_ast(i, ind + " ", env); }
  } else if (Ruleset* ruleset = dynamic_cast<Ruleset*>(node)) {
    std::cerr << ind << "Ruleset " << ruleset;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [indent: " << ruleset->tabs() << "]";
    std::cerr << (ruleset->at_root() ? " [@ROOT]" : "");
    std::cerr << (ruleset->is_root() ? " [root]" : "");
    std::cerr << std::endl;
    debug_ast(ruleset->selector(), ind + ">");
    debug_ast(ruleset->block(), ind + " ");
  } else if (dynamic_cast<Block*>(node)) {
    Block* block = dynamic_cast<Block*>(node);
    std::cerr << ind << "Block " << block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [indent: " << block->tabs() << "]" << std::endl;
    for(auto i : block->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Textual*>(node)) {
    Textual* expression = dynamic_cast<Textual*>(node);
    std::cerr << ind << "Textual ";
    if (expression->type() == Textual::NUMBER) std::cerr << " [NUMBER]";
    else if (expression->type() == Textual::PERCENTAGE) std::cerr << " [PERCENTAGE]";
    else if (expression->type() == Textual::DIMENSION) std::cerr << " [DIMENSION]";
    else if (expression->type() == Textual::HEX) std::cerr << " [HEX]";
    std::cerr << expression << " [" << expression->value() << "]";
    std::cerr << " [interpolant: " << expression->is_interpolant() << "] ";
    if (expression->is_delayed()) std::cerr << " [delayed]";
    std::cerr << std::endl;
  } else if (dynamic_cast<Variable*>(node)) {
    Variable* expression = dynamic_cast<Variable*>(node);
    std::cerr << ind << "Variable " << expression;
    std::cerr << " [interpolant: " << expression->is_interpolant() << "] ";
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [" << expression->name() << "]" << std::endl;
    std::string name(expression->name());
    if (env && env->has(name)) debug_ast(static_cast<Expression*>((*env)[name]), ind + " -> ", env);
  } else if (dynamic_cast<Function_Call_Schema*>(node)) {
    Function_Call_Schema* expression = dynamic_cast<Function_Call_Schema*>(node);
    std::cerr << ind << "Function_Call_Schema " << expression;
    std::cerr << " [interpolant: " << expression->is_interpolant() << "] ";
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << "" << std::endl;
    debug_ast(expression->name(), ind + "name: ", env);
    debug_ast(expression->arguments(), ind + " args: ", env);
  } else if (dynamic_cast<Function_Call*>(node)) {
    Function_Call* expression = dynamic_cast<Function_Call*>(node);
    std::cerr << ind << "Function_Call " << expression;
    std::cerr << " [interpolant: " << expression->is_interpolant() << "] ";
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [" << expression->name() << "]";
    if (expression->is_delayed()) std::cerr << " [delayed]";
    if (expression->is_interpolant()) std::cerr << " [interpolant]";
    std::cerr << std::endl;
    debug_ast(expression->arguments(), ind + " args: ", env);
  } else if (dynamic_cast<Arguments*>(node)) {
    Arguments* expression = dynamic_cast<Arguments*>(node);
    std::cerr << ind << "Arguments " << expression;
    if (expression->is_delayed()) std::cerr << " [delayed]";
    std::cerr << " (" << pstate_source_position(node) << ")";
    if (expression->has_named_arguments()) std::cerr << " [has_named_arguments]";
    if (expression->has_rest_argument()) std::cerr << " [has_rest_argument]";
    if (expression->has_keyword_argument()) std::cerr << " [has_keyword_argument]";
    std::cerr << std::endl;
    for(auto i : expression->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Argument*>(node)) {
    Argument* expression = dynamic_cast<Argument*>(node);
    std::cerr << ind << "Argument " << expression;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [" << expression->value() << "]";
    std::cerr << " [name: " << expression->name() << "] ";
    std::cerr << " [rest: " << expression->is_rest_argument() << "] ";
    std::cerr << " [keyword: " << expression->is_keyword_argument() << "] " << std::endl;
    debug_ast(expression->value(), ind + " value: ", env);
  } else if (dynamic_cast<Parameters*>(node)) {
    Parameters* expression = dynamic_cast<Parameters*>(node);
    std::cerr << ind << "Parameters " << expression;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [has_optional: " << expression->has_optional_parameters() << "] ";
    std::cerr << " [has_rest: " << expression->has_rest_parameter() << "] ";
    std::cerr << std::endl;
    for(auto i : expression->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Parameter*>(node)) {
    Parameter* expression = dynamic_cast<Parameter*>(node);
    std::cerr << ind << "Parameter " << expression;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [name: " << expression->name() << "] ";
    std::cerr << " [default: " << expression->default_value() << "] ";
    std::cerr << " [rest: " << expression->is_rest_parameter() << "] " << std::endl;
  } else if (dynamic_cast<Unary_Expression*>(node)) {
    Unary_Expression* expression = dynamic_cast<Unary_Expression*>(node);
    std::cerr << ind << "Unary_Expression " << expression;
    std::cerr << " [interpolant: " << expression->is_interpolant() << "] ";
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [" << expression->type() << "]" << std::endl;
    debug_ast(expression->operand(), ind + " operand: ", env);
  } else if (dynamic_cast<Binary_Expression*>(node)) {
    Binary_Expression* expression = dynamic_cast<Binary_Expression*>(node);
    std::cerr << ind << "Binary_Expression " << expression;
    if (expression->is_interpolant()) std::cerr << " [is interpolant] ";
    if (expression->is_left_interpolant()) std::cerr << " [left interpolant] ";
    if (expression->is_right_interpolant()) std::cerr << " [right interpolant] ";
    std::cerr << " [delayed: " << expression->is_delayed() << "] ";
    std::cerr << " [ws_before: " << expression->op().ws_before << "] ";
    std::cerr << " [ws_after: " << expression->op().ws_after << "] ";
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [" << expression->type_name() << "]" << std::endl;
    debug_ast(expression->left(), ind + " left:  ", env);
    debug_ast(expression->right(), ind + " right: ", env);
  } else if (dynamic_cast<Map*>(node)) {
    Map* expression = dynamic_cast<Map*>(node);
    std::cerr << ind << "Map " << expression;
    std::cerr << " [interpolant: " << expression->is_interpolant() << "] ";
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [Hashed]" << std::endl;
    for (auto i : expression->elements()) {
      debug_ast(i.first, ind + " key: ");
      debug_ast(i.second, ind + " val: ");
    }
  } else if (dynamic_cast<List*>(node)) {
    List* expression = dynamic_cast<List*>(node);
    std::cerr << ind << "List " << expression;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " (" << expression->length() << ") " <<
      (expression->separator() == SASS_COMMA ? "Comma " : expression->separator() == SASS_HASH ? "Map" : "Space ") <<
      " [delayed: " << expression->is_delayed() << "] " <<
      " [interpolant: " << expression->is_interpolant() << "] " <<
      " [listized: " << expression->from_selector() << "] " <<
      " [arglist: " << expression->is_arglist() << "] " <<
      " [hash: " << expression->hash() << "] " <<
      std::endl;
    for(auto i : expression->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Content*>(node)) {
    Content* expression = dynamic_cast<Content*>(node);
    std::cerr << ind << "Content " << expression;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [@media:" << expression->media_block() << "]";
    std::cerr << " [Statement]" << std::endl;
  } else if (dynamic_cast<Boolean*>(node)) {
    Boolean* expression = dynamic_cast<Boolean*>(node);
    std::cerr << ind << "Boolean " << expression;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [interpolant: " << expression->is_interpolant() << "] ";
    std::cerr << " [" << expression->value() << "]" << std::endl;
  } else if (dynamic_cast<Color*>(node)) {
    Color* expression = dynamic_cast<Color*>(node);
    std::cerr << ind << "Color " << expression;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [interpolant: " << expression->is_interpolant() << "] ";
    std::cerr << " [" << expression->r() << ":"  << expression->g() << ":" << expression->b() << "@" << expression->a() << "]" << std::endl;
  } else if (dynamic_cast<Number*>(node)) {
    Number* expression = dynamic_cast<Number*>(node);
    std::cerr << ind << "Number " << expression;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [interpolant: " << expression->is_interpolant() << "] ";
    std::cerr << " [" << expression->value() << expression->unit() << "]" <<
      " [hash: " << expression->hash() << "] " <<
      std::endl;
  } else if (dynamic_cast<String_Quoted*>(node)) {
    String_Quoted* expression = dynamic_cast<String_Quoted*>(node);
    std::cerr << ind << "String_Quoted " << expression;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [" << prettyprint(expression->value()) << "]";
    if (expression->is_delayed()) std::cerr << " [delayed]";
    if (expression->sass_fix_1291()) std::cerr << " [sass_fix_1291]";
    if (expression->is_interpolant()) std::cerr << " [interpolant]";
    if (expression->quote_mark()) std::cerr << " [quote_mark: " << expression->quote_mark() << "]";
    std::cerr << " <" << prettyprint(expression->pstate().token.ws_before()) << ">" << std::endl;
  } else if (dynamic_cast<String_Constant*>(node)) {
    String_Constant* expression = dynamic_cast<String_Constant*>(node);
    std::cerr << ind << "String_Constant " << expression;
    if (expression->concrete_type()) {
      std::cerr << " " << expression->concrete_type();
    }
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " [" << prettyprint(expression->value()) << "]";
    if (expression->is_delayed()) std::cerr << " [delayed]";
    if (expression->sass_fix_1291()) std::cerr << " [sass_fix_1291]";
    if (expression->is_interpolant()) std::cerr << " [interpolant]";
    std::cerr << " <" << prettyprint(expression->pstate().token.ws_before()) << ">" << std::endl;
  } else if (dynamic_cast<String_Schema*>(node)) {
    String_Schema* expression = dynamic_cast<String_Schema*>(node);
    std::cerr << ind << "String_Schema " << expression;
    std::cerr << " " << expression->concrete_type();
    if (expression->is_delayed()) std::cerr << " [delayed]";
    if (expression->is_interpolant()) std::cerr << " [is interpolant]";
    if (expression->has_interpolant()) std::cerr << " [has interpolant]";
    if (expression->is_left_interpolant()) std::cerr << " [left interpolant] ";
    if (expression->is_right_interpolant()) std::cerr << " [right interpolant] ";
    std::cerr << " <" << prettyprint(expression->pstate().token.ws_before()) << ">" << std::endl;
    for(auto i : expression->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<String*>(node)) {
    String* expression = dynamic_cast<String*>(node);
    std::cerr << ind << "String " << expression;
    std::cerr << " " << expression->concrete_type();
    std::cerr << " (" << pstate_source_position(node) << ")";
    if (expression->sass_fix_1291()) std::cerr << " [sass_fix_1291]";
    if (expression->is_interpolant()) std::cerr << " [interpolant]";
    std::cerr << " <" << prettyprint(expression->pstate().token.ws_before()) << ">" << std::endl;
  } else if (dynamic_cast<Expression*>(node)) {
    Expression* expression = dynamic_cast<Expression*>(node);
    std::cerr << ind << "Expression " << expression;
    std::cerr << " (" << pstate_source_position(node) << ")";
    switch (expression->concrete_type()) {
      case Expression::Concrete_Type::NONE: std::cerr << " [NONE]"; break;
      case Expression::Concrete_Type::BOOLEAN: std::cerr << " [BOOLEAN]"; break;
      case Expression::Concrete_Type::NUMBER: std::cerr << " [NUMBER]"; break;
      case Expression::Concrete_Type::COLOR: std::cerr << " [COLOR]"; break;
      case Expression::Concrete_Type::STRING: std::cerr << " [STRING]"; break;
      case Expression::Concrete_Type::LIST: std::cerr << " [LIST]"; break;
      case Expression::Concrete_Type::MAP: std::cerr << " [MAP]"; break;
      case Expression::Concrete_Type::SELECTOR: std::cerr << " [SELECTOR]"; break;
      case Expression::Concrete_Type::NULL_VAL: std::cerr << " [NULL_VAL]"; break;
      case Expression::Concrete_Type::C_WARNING: std::cerr << " [C_WARNING]"; break;
      case Expression::Concrete_Type::C_ERROR: std::cerr << " [C_ERROR]"; break;
      case Expression::Concrete_Type::NUM_TYPES: std::cerr << " [NUM_TYPES]"; break;
    }
    std::cerr << std::endl;
  } else if (dynamic_cast<Has_Block*>(node)) {
    Has_Block* has_block = dynamic_cast<Has_Block*>(node);
    std::cerr << ind << "Has_Block " << has_block;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << has_block->tabs() << std::endl;
    if (has_block->block()) for(auto i : has_block->block()->elements()) { debug_ast(i, ind + " ", env); }
  } else if (dynamic_cast<Statement*>(node)) {
    Statement* statement = dynamic_cast<Statement*>(node);
    std::cerr << ind << "Statement " << statement;
    std::cerr << " (" << pstate_source_position(node) << ")";
    std::cerr << " " << statement->tabs() << std::endl;
  }

  if (ind == "") std::cerr << "####################################################################\n";
}

inline void debug_node(Node* node, std::string ind = "")
{
  if (ind == "") std::cerr << "#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
  if (node->isCombinator()) {
    std::cerr << ind;
    std::cerr << "Combinator ";
    std::cerr << node << " ";
    if (node->got_line_feed) std::cerr << "[LF] ";
    switch (node->combinator()) {
      case Complex_Selector::ADJACENT_TO: std::cerr << "{+} "; break;
      case Complex_Selector::PARENT_OF:   std::cerr << "{>} "; break;
      case Complex_Selector::PRECEDES:    std::cerr << "{~} "; break;
      case Complex_Selector::REFERENCE:   std::cerr << "{@} "; break;
      case Complex_Selector::ANCESTOR_OF: std::cerr << "{ } "; break;
    }
    std::cerr << std::endl;
    // debug_ast(node->combinator(), ind + "  ");
  } else if (node->isSelector()) {
    std::cerr << ind;
    std::cerr << "Selector ";
    std::cerr << node << " ";
    if (node->got_line_feed) std::cerr << "[LF] ";
    std::cerr << std::endl;
    debug_ast(node->selector(), ind + "  ");
  } else if (node->isCollection()) {
    std::cerr << ind;
    std::cerr << "Collection ";
    std::cerr << node << " ";
    if (node->got_line_feed) std::cerr << "[LF] ";
    std::cerr << std::endl;
    for(auto n : (*node->collection())) {
      debug_node(&n, ind + "  ");
    }
  } else if (node->isNil()) {
    std::cerr << ind;
    std::cerr << "Nil ";
    std::cerr << node << " ";
    if (node->got_line_feed) std::cerr << "[LF] ";
    std::cerr << std::endl;
  } else {
    std::cerr << ind;
    std::cerr << "OTHER ";
    std::cerr << node << " ";
    if (node->got_line_feed) std::cerr << "[LF] ";
    std::cerr << std::endl;
  }
  if (ind == "") std::cerr << "#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
}

inline void debug_ast(const AST_Node* node, std::string ind = "", Env* env = 0)
{
  debug_ast(const_cast<AST_Node*>(node), ind, env);
}

inline void debug_node(const Node* node, std::string ind = "")
{
  debug_node(const_cast<Node*>(node), ind);
}

inline void debug_subset_map(Sass::ExtensionSubsetMap& map, std::string ind = "")
{
  if (ind == "") std::cerr << "#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
  for(auto const &it : map.values()) {
    debug_ast(it.first, ind + "first: ");
    debug_ast(it.second, ind + "second: ");
  }
  if (ind == "") std::cerr << "#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
}

typedef std::pair<Complex_Selector*, Compound_Selector*> ExtensionPair;
typedef std::vector<ExtensionPair> SubsetMapEntries;

inline void debug_subset_entries(SubsetMapEntries* entries, std::string ind = "")
{
  if (ind == "") std::cerr << "#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
  for(auto const &pair : *entries) {
    debug_ast(pair.first, ind + "first: ");
    debug_ast(pair.second, ind + "second: ");
  }
  if (ind == "") std::cerr << "#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
}

#endif // SASS_DEBUGGER
