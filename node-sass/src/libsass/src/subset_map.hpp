#ifndef SASS_SUBSET_MAP_H
#define SASS_SUBSET_MAP_H

#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>


// #include <iostream>
// #include <sstream>
// template<typename T>
// std::string vector_to_string(std::vector<T> v)
// {
//   std::stringstream buffer;
//   buffer << "[";

//   if (!v.empty())
//   {  buffer << v[0]; }
//   else
//   { buffer << "]"; }

//   if (v.size() == 1)
//   { buffer << "]"; }
//   else
//   {
//     for (size_t i = 1, S = v.size(); i < S; ++i) buffer << ", " << v[i];
//     buffer << "]";
//   }

//   return buffer.str();
// }

// template<typename T>
// std::string set_to_string(set<T> v)
// {
//   std::stringstream buffer;
//   buffer << "[";
//   typename std::set<T>::iterator i = v.begin();
//   if (!v.empty())
//   {  buffer << *i; }
//   else
//   { buffer << "]"; }

//   if (v.size() == 1)
//   { buffer << "]"; }
//   else
//   {
//     for (++i; i != v.end(); ++i) buffer << ", " << *i;
//     buffer << "]";
//   }

//   return buffer.str();
// }

namespace Sass {

  template<typename F, typename S, typename T>
  struct triple {
    F first;
    S second;
    T third;

    triple(const F& f, const S& s, const T& t) : first(f), second(s), third(t) { }
  };

  template<typename F, typename S, typename T>
  triple<F, S, T> make_triple(const F& f, const S& s, const T& t)
  { return triple<F, S, T>(f, s, t); }

  template<typename K, typename V>
  class Subset_Map {
  private:
    std::vector<V> values_;
    std::map<K, std::vector<triple<std::vector<K>, std::set<K>, size_t> > > hash_;
  public:
    void put(const std::vector<K>& s, const V& value);
    std::vector<std::pair<V, std::vector<K> > > get_kv(const std::vector<K>& s);
    std::vector<V> get_v(const std::vector<K>& s);
    bool empty() { return values_.empty(); }
    void clear() { values_.clear(); hash_.clear(); }
    const std::vector<V> values(void) { return values_; }
  };

  template<typename K, typename V>
  void Subset_Map<K, V>::put(const std::vector<K>& s, const V& value)
  {
    if (s.empty()) throw "internal error: subset map keys may not be empty";
    size_t index = values_.size();
    values_.push_back(value);
    std::set<K> ss;
    for (size_t i = 0, S = s.size(); i < S; ++i)
    { ss.insert(s[i]); }
    for (size_t i = 0, S = s.size(); i < S; ++i)
    {
      hash_[s[i]].push_back(make_triple(s, ss, index));
    }
  }

  template<typename K, typename V>
  std::vector<std::pair<V, std::vector<K> > > Subset_Map<K, V>::get_kv(const std::vector<K>& s)
  {
    std::vector<K> sorted = s;
    sort(sorted.begin(), sorted.end());
    std::vector<std::pair<size_t, std::vector<K> > > indices;
    for (size_t i = 0, S = s.size(); i < S; ++i) {
      if (!hash_.count(s[i])) {
        continue;
      }
      std::vector<triple<std::vector<K>, std::set<K>, size_t> > subsets = hash_[s[i]];
      // std::cerr << "length of subsets: " << subsets.size() << std::endl;
      for (size_t j = 0, T = subsets.size(); j < T; ++j) {
        if (!includes(sorted.begin(), sorted.end(), subsets[j].second.begin(), subsets[j].second.end())) {
          // std::cout << vector_to_string(s) << " doesn't include " << set_to_string(subsets[j].second) << std::endl;
          continue;
        }
        indices.push_back(std::make_pair(subsets[j].third, subsets[j].first));
        // std::cerr << "pushed " << subsets[j].third << " and " << vector_to_string(subsets[j].first) << " onto indices" << std::endl;
      }
    }
    sort(indices.begin(), indices.end());
    typename std::vector<std::pair<size_t, std::vector<K> > >::iterator indices_end = unique(indices.begin(), indices.end());
    indices.resize(distance(indices.begin(), indices_end));

    std::vector<std::pair<V, std::vector<K> > > results;
    for (size_t i = 0, S = indices.size(); i < S; ++i) {
      results.push_back(std::make_pair(values_[indices[i].first], indices[i].second));
    }
    return results;
  }

  template<typename K, typename V>
  std::vector<V> Subset_Map<K, V>::get_v(const std::vector<K>& s)
  {
    std::vector<std::pair<V, std::vector<K> > > kvs = get_kv(s);
    std::vector<V> results;
    for (size_t i = 0, S = kvs.size(); i < S; ++i) results.push_back(kvs[i].first);
    return results;
  }

}

#endif
