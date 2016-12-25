#ifndef SASS_EXTEND_H
#define SASS_EXTEND_H

#include <string>

#include "ast.hpp"
#include "operation.hpp"
#include "subset_map.hpp"

namespace Sass {

  class Context;
  class Node;

  typedef Subset_Map<std::string, std::pair<Complex_Selector*, Compound_Selector*> > ExtensionSubsetMap;

  class Extend : public Operation_CRTP<void, Extend> {

    Context&            ctx;
    ExtensionSubsetMap& subset_map;

    void fallback_impl(AST_Node* n) { }

  public:
    static Node subweave(Node& one, Node& two, Context& ctx);
    static Selector_List* extendSelectorList(Selector_List* pSelectorList, Context& ctx, ExtensionSubsetMap& subset_map, bool isReplace, bool& extendedSomething);
    static Selector_List* extendSelectorList(Selector_List* pSelectorList, Context& ctx, ExtensionSubsetMap& subset_map, bool isReplace = false) {
      bool extendedSomething = false;
      return extendSelectorList(pSelectorList, ctx, subset_map, isReplace, extendedSomething);
    }
    Extend(Context&, ExtensionSubsetMap&);
    ~Extend() { }

    void operator()(Block*);
    void operator()(Ruleset*);
    void operator()(Supports_Block*);
    void operator()(Media_Block*);
    void operator()(Directive*);

    template <typename U>
    void fallback(U x) { return fallback_impl(x); }
  };

}

#endif
