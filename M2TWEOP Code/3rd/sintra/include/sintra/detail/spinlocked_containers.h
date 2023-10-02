/*
Copyright 2017 Ioannis Makris

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SINTRA_SPINLOCKED_CONTAINERS_H
#define SINTRA_SPINLOCKED_CONTAINERS_H


// This file defines crippled spinlock-wrapped versions of common containers.
// The only guarantee that these containers provide is that individual
// operations on them are thread safe.
// By no means should anyone ever assume that this offers any kind of generic
// thread safety associated with their usage. Their sole purpose was to save
// some typing, in a few common scenarios within sintra, but also to simplify
// debugging, by eliminating a class of bugs associated with container
// corruption.


#include "spinlock.h"

#include <deque>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace sintra {


using std::deque;
using std::list;
using std::map;
using std::memory_order_acquire;
using std::memory_order_release;
using std::set;
using std::unordered_map;
using std::unordered_set;
using std::vector;


namespace detail {


template <template <typename...> typename CT, typename... Args>
struct spinlocked
{
    using iterator          = typename CT<Args...>::iterator;
    using const_iterator    = typename CT<Args...>::const_iterator;
    using reference         = typename CT<Args...>::reference;
    using const_reference   = typename CT<Args...>::const_reference;
    using size_type         = typename CT<Args...>::size_type;
    using locker            = spinlock::locker;

    reference back() noexcept                      {locker l(m_sl); return m_c.back();            }
    const_reference back() const noexcept          {locker l(m_sl); return m_c.back();            }
    iterator begin() noexcept                      {locker l(m_sl); return m_c.begin();           }
    const_iterator begin() const noexcept          {locker l(m_sl); return m_c.begin();           }
    void clear() noexcept                          {locker l(m_sl); return m_c.clear();           }


    template <class... FArgs>
    auto emplace(FArgs&&... args)
    {
        locker l(this->m_sl);
        return this->m_c.emplace(args...);
    }

    template <class... FArgs>
    auto emplace_hint(const_iterator hint, FArgs&&... args)
    {
        locker l(this->m_sl);
        return this->m_c.emplace_hint(hint, args...);
    }

    bool empty() const noexcept                    {locker l(m_sl); return m_c.empty();           }
    iterator end() noexcept                        {locker l(m_sl); return m_c.end();             }
    const_iterator end() const noexcept            {locker l(m_sl); return m_c.end();             }

    template <typename... FArgs>
    auto erase(FArgs&... v)                        {locker l(m_sl); return m_c.erase(v...);       }
    template <typename... FArgs>
    auto erase(FArgs&&... v)                       {locker l(m_sl); return m_c.erase(v...);       }

    template <typename... FArgs>
    iterator find(const FArgs&... v)               {locker l(m_sl); return m_c.find(v...);        }
    template <typename... FArgs>
    const_iterator find(const FArgs&&... v) const  {locker l(m_sl); return m_c.find(v...);        }

    reference front() noexcept                     {locker l(m_sl); return m_c.front();           }
    const_reference front() const noexcept         {locker l(m_sl); return m_c.front();           }

    template <typename... FArgs>
    auto insert(const FArgs&... v)                 {locker l(m_sl); return m_c.insert(v...);      }
    template <typename... FArgs>
    auto insert(FArgs&&... v)                      {locker l(m_sl); return m_c.insert(v...);      }

    auto pop_front()                               {locker l(m_sl); return m_c.pop_front();       }

    template <typename... FArgs>
    auto push_back(const FArgs&... v)              {locker l(m_sl); return m_c.push_back(v...);   }

    auto size()  const noexcept                    {locker l(m_sl); return m_c.size();            }

    operator CT<Args...>() const                   {locker l(m_sl); return m_c;                   }
    auto operator=(const CT<Args...>& x)           {locker l(m_sl); return m_c.operator=(x);      }
    auto operator=(CT<Args...>&& x)                {locker l(m_sl); return m_c.operator=(x);      }


    //auto operator=(const CT<Args...>& x)           {locker l(m_sl); return m_c.operator=(x.m_c);  }
    //auto operator=(CT<Args...>&& x)                {locker l(m_sl); return m_c.operator=(x.m_c);  }

    reference operator[] (size_type p)             {locker l(m_sl); return m_c[p];                }
    const_reference operator[] (size_type p) const {locker l(m_sl); return m_c[p];                }

protected:
    CT<Args...>         m_c;
    mutable spinlock    m_sl;
};



} // namespace detail



template <typename T>
using spinlocked_deque = detail::spinlocked<deque, T>;

template <typename T>
using spinlocked_list = detail::spinlocked<list, T>;

template <typename T>
using spinlocked_set = detail::spinlocked<set, T>;

template <typename T>
using spinlocked_uset = detail::spinlocked<unordered_set, T>;


template <typename Key, typename T>
struct spinlocked_map: detail::spinlocked<map, Key, T>
{
    using locker = spinlock::locker;

    template <typename... FArgs>
    auto lower_bound(const FArgs&... v) {
        locker l(this->m_sl);
        return this->m_c.lower_bound(v...);
    }

    T& operator[] (const Key& k)                   {locker l(this->m_sl); return this->m_c[k];    }
    T& operator[] (Key&& k)                        {locker l(this->m_sl); return this->m_c[k];    }
};


template <typename Key, typename T>
struct spinlocked_umap: detail::spinlocked<unordered_map, Key, T>
{
    using locker = spinlock::locker;
    T& operator[] (const Key& k)                   {locker l(this->m_sl); return this->m_c[k];    }
    T& operator[] (Key&& k)                        {locker l(this->m_sl); return this->m_c[k];    }
};


template <typename T>
using spinlocked_vector = detail::spinlocked<vector, T>;

} // namespace sintra


#endif
