#include "sass.hpp"
#include "ast.hpp"
#include "memory_manager.hpp"

namespace Sass {

  Memory_Manager::Memory_Manager(size_t size)
  : nodes(std::vector<Memory_Object*>())
  {
    size_t init = size;
    if (init < 8) init = 8;
    // reserve some space
    nodes.reserve(init);
  }

  Memory_Manager::~Memory_Manager()
  {
    // release memory for all controlled nodes
    // avoid calling erase for every single node
    for (size_t i = 0, S = nodes.size(); i < S; ++i) {
      deallocate(nodes[i]);
    }
    // just in case
    nodes.clear();
  }

  Memory_Object* Memory_Manager::add(Memory_Object* np)
  {
    // object has been initialized
    // it can be "deleted" from now on
    np->refcount = 1;
    return np;
  }

  bool Memory_Manager::has(Memory_Object* np)
  {
    // check if the pointer is controlled under our pool
    return find(nodes.begin(), nodes.end(), np) != nodes.end();
  }

  Memory_Object* Memory_Manager::allocate(size_t size)
  {
    // allocate requested memory
    void* heap = malloc(size);
    // init internal refcount status to zero
    (static_cast<Memory_Object*>(heap))->refcount = 0;
    // add the memory under our management
    nodes.push_back(static_cast<Memory_Object*>(heap));
    // cast object to its initial type
    return static_cast<Memory_Object*>(heap);
  }

  void Memory_Manager::deallocate(Memory_Object* np)
  {
    // only call destructor if initialized
    if (np->refcount) np->~Memory_Object();
    // always free the memory
    free(np);
  }

  void Memory_Manager::remove(Memory_Object* np)
  {
    // remove node from pool (no longer active)
    nodes.erase(find(nodes.begin(), nodes.end(), np));
    // you are now in control of the memory
  }

  void Memory_Manager::destroy(Memory_Object* np)
  {
    // remove from pool
    remove(np);
    // release memory
    deallocate(np);
  }

}

