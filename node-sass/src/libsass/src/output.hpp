#ifndef SASS_OUTPUT_H
#define SASS_OUTPUT_H

#include <string>
#include <vector>

#include "util.hpp"
#include "inspect.hpp"
#include "operation.hpp"

namespace Sass {
  class Context;

  // Refactor to make it generic to find linefeed (look behind)
  inline bool ends_with(std::string const & value, std::string const & ending)
  {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
  }

  class Output : public Inspect {
  protected:
    using Inspect::operator();

  public:
    Output(Sass_Output_Options& opt);
    virtual ~Output();

  protected:
    std::string charset;
    std::vector<AST_Node*> top_nodes;

  public:
    OutputBuffer get_buffer(void);

    virtual void operator()(Map*);
    virtual void operator()(Ruleset*);
    // virtual void operator()(Propset*);
    virtual void operator()(Supports_Block*);
    virtual void operator()(Media_Block*);
    virtual void operator()(Directive*);
    virtual void operator()(Keyframe_Rule*);
    virtual void operator()(Import*);
    virtual void operator()(Comment*);
    virtual void operator()(Number*);
    virtual void operator()(String_Quoted*);
    virtual void operator()(String_Constant*);

    void fallback_impl(AST_Node* n);

  };

}

#endif
