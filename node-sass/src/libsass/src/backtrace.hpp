#ifndef SASS_BACKTRACE_H
#define SASS_BACKTRACE_H

#include <sstream>

#include "file.hpp"
#include "position.hpp"

namespace Sass {


  struct Backtrace {

    Backtrace*  parent;
    ParserState pstate;
    std::string      caller;

    Backtrace(Backtrace* prn, ParserState pstate, std::string c)
    : parent(prn),
      pstate(pstate),
      caller(c)
    { }

    std::string to_string(bool warning = false)
    {
      size_t i = -1;
      std::stringstream ss;
      std::string cwd(Sass::File::get_cwd());
      Backtrace* this_point = this;

      if (!warning) ss << std::endl << "Backtrace:";
      // the first tracepoint (which is parent-less) is an empty placeholder
      while (this_point->parent) {

        // make path relative to the current directory
        std::string rel_path(Sass::File::abs2rel(this_point->pstate.path, cwd, cwd));

        if (warning) {
          ss << std::endl
             << "\t"
             << (++i == 0 ? "on" : "from")
             << " line "
             << this_point->pstate.line + 1
             << " of "
             << rel_path;
        } else {
          ss << std::endl
             << "\t"
             << rel_path
             << ":"
             << this_point->pstate.line + 1
             << this_point->parent->caller;
        }

        this_point = this_point->parent;
      }

      return ss.str();
    }

    size_t depth()
    {
      size_t d = 0;
      Backtrace* p = parent;
      while (p) {
        ++d;
        p = p->parent;
      }
      return d-1;
    }

  };

}

#endif
