#ifndef SASS_MEMORY_MANAGER_H
#define SASS_MEMORY_MANAGER_H

#include <vector>

namespace Sass {

  class Memory_Object {
  friend class Memory_Manager;
    long refcount;
  public:
    Memory_Object() { refcount = 0; };
    virtual ~Memory_Object() {};
  };

  /////////////////////////////////////////////////////////////////////////////
  // A class for tracking allocations of AST_Node objects. The intended usage
  // is something like: Some_Node* n = new (mem_mgr) Some_Node(...);
  // Then, at the end of the program, the memory manager will delete all of the
  // allocated nodes that have been passed to it.
  // In the future, this class may implement a custom allocator.
  /////////////////////////////////////////////////////////////////////////////
  class Memory_Manager {
    std::vector<Memory_Object*> nodes;

  public:
    Memory_Manager(size_t size = 0);
    ~Memory_Manager();

    bool has(Memory_Object* np);
    Memory_Object* allocate(size_t size);
    void deallocate(Memory_Object* np);
    void remove(Memory_Object* np);
    void destroy(Memory_Object* np);
    Memory_Object* add(Memory_Object* np);

  };
}

///////////////////////////////////////////////////////////////////////////////
// Use macros for the allocation task, since overloading operator `new`
// has been proven to be flaky under certain compilers (see comment below).
///////////////////////////////////////////////////////////////////////////////

#define SASS_MEMORY_NEW(mgr, Class, ...)                                                 \
  (static_cast<Class*>(mgr.add(new (mgr.allocate(sizeof(Class))) Class(__VA_ARGS__))))   \

#endif
