#pragma once
#include <vector>
#include <stdexcept>
#include <new>
#include <utility>
#include <cstring>
#include <cassert>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 26495) // uninitialized member t at TU::TU()
#endif
#if defined(__llvm__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdangling-else"
#endif

template<typename T, typename Alloc = std::allocator<T>>
class ring_buffer
{
    union TU
    {
        T t;
        TU() {}
        ~TU() {}
    };
    using vec_t = std::vector<TU, typename std::allocator_traits<Alloc>::template rebind_alloc<TU>>;
    using it_t = typename vec_t::iterator;
public:
    struct iterator;
    struct const_iterator
    {
        const_iterator();
        const_iterator(const_iterator const& it);
        const_iterator& operator +=(std::ptrdiff_t offset);
        const_iterator& operator -=(std::ptrdiff_t offset);
        const_iterator& operator  =(const_iterator const& it);
        bool            operator ==(const_iterator const& it) const;
        bool            operator !=(const_iterator const& it) const;
        bool            operator  <(const_iterator const& it) const;
        bool            operator <=(const_iterator const& it) const;
        bool            operator  >(const_iterator const& it) const;
        bool            operator >=(const_iterator const& it) const;
        const_iterator  operator  +(std::ptrdiff_t offset) const;
        const_iterator  operator  -(std::ptrdiff_t offset) const;
        T const& operator  *() const;
        const_iterator& operator ++();
        const_iterator& operator --();
        const_iterator  operator ++(int);
        const_iterator  operator --(int);
        T const& operator [](std::ptrdiff_t offset) const;
        T const& at(std::ptrdiff_t offset) const;
    private:
        friend class ring_buffer;
        const_iterator(ring_buffer& ring, it_t it);
        std::ptrdiff_t  idx() const;
        template<bool THROW = false>
        it_t         offset_it(std::ptrdiff_t offset) const;
        ring_buffer* m_ring;
        it_t         m_it;
    };
    struct iterator : public const_iterator
    {
        iterator();
        iterator(iterator const& it);
        iterator& operator +=(std::ptrdiff_t offset);
        iterator& operator -=(std::ptrdiff_t offset);
        iterator& operator  =(iterator const& it);
        iterator  operator  +(std::ptrdiff_t offset) const;
        iterator  operator  -(std::ptrdiff_t offset) const;
        T& operator  *() const;
        iterator& operator ++();
        iterator& operator --();
        iterator  operator ++(int);
        iterator  operator --(int);
        T& operator [](std::ptrdiff_t offset) const;
        T& at(std::ptrdiff_t offset) const;
    private:
        iterator(ring_buffer& ring, it_t it);
        explicit  iterator(const_iterator const& it);
        friend class ring_buffer;
    };
    ring_buffer(std::size_t cap);
    ring_buffer(ring_buffer const& other);
    ~ring_buffer();
    T& operator [](std::size_t offset);
    T& at(std::size_t offset);
    T& front() const;
    iterator        begin() const;
    iterator        end() const;
    const_iterator  cbegin() const;
    const_iterator  cend() const;
    void            pop_front();
    template<typename ... Args>
    T& emplace_back(Args &&...args);
    T& push_back(T const& other);
    T& push_back(T&& other);
    bool            full() const;
    std::size_t     size() const;
    void            capacity(std::size_t newCap);
private:
    vec_t  m_ring;
    it_t   m_read, m_write;
    size_t m_size, m_capacity;
};

template<typename T, typename Alloc>
inline
std::ptrdiff_t ring_buffer<T, Alloc>::const_iterator::idx() const
{
    if (m_it >= m_ring->m_read)
    {
        assert(m_ring->m_read <= m_ring->m_write && m_it <= m_ring->m_write
            || m_ring->m_write < m_ring->m_read & m_it >= m_ring->m_ring.begin() && m_it <= m_ring->m_write);
        return m_it - m_ring->m_read;
    }
    else
    {
        assert(m_it >= m_ring->m_ring.begin() && m_it <= m_ring->m_write);
        return (m_it - m_ring->m_ring.begin()) + (m_ring->m_ring.end() - m_ring->m_read);
    }
}

template<typename T, typename Alloc>
template<bool THROW>
inline
typename ring_buffer<T, Alloc>::it_t ring_buffer<T, Alloc>::const_iterator::offset_it(std::ptrdiff_t offset) const
{
    using namespace std;
    ptrdiff_t rel;
    static
        char const outOfRangeStr[] = "ring-buffer access out of range";
    if (m_ring->m_read <= m_ring->m_write)
    {
        rel = m_it - m_ring->m_read + offset;
        if constexpr (!THROW)
            assert(rel >= 0 && rel <= m_ring->m_write - m_ring->m_read);
        else
            if (rel < 0 || rel > m_ring->m_write - m_ring->m_read)
                throw out_of_range(outOfRangeStr);
        return m_it + offset;
    }
    if (offset >= 0)
    {
        if (m_it >= m_ring->m_read)
        {
            rel = m_it - m_ring->m_read + offset;
            if (rel < m_ring->m_ring.end() - m_ring->m_read)
                return m_it + rel;
            rel -= m_ring->m_ring.end() - m_ring->m_read;
            if constexpr (!THROW)
                assert(rel <= m_ring->m_write - m_ring->m_ring.begin());
            else
                if (rel > m_ring->m_write - m_ring->m_ring.begin())
                    throw out_of_range(outOfRangeStr);
            return m_ring->m_ring.begin() + rel;
        }
        if constexpr (!THROW)
            assert(m_it - m_ring->m_ring.begin() + offset <= m_ring->m_write - m_ring->m_ring.begin());
        else
            if (m_it - m_ring->m_ring.begin() + offset > m_ring->m_write - m_ring->m_ring.begin())
                throw out_of_range(outOfRangeStr);
        return m_it + offset;
    }
    if (m_it >= m_ring->m_read)
    {
        rel = m_it - m_ring->m_read + offset;
        if constexpr (!THROW)
            assert(rel >= 0);
        else
            if (rel < 0)
                throw out_of_range(outOfRangeStr);
        return m_it + offset;
    }
    rel = m_it - m_ring->m_ring.begin() + offset;
    if (rel >= 0)
        return m_it + rel;
    rel += m_it - m_ring->m_ring.begin();
    if constexpr (!THROW)
        assert(m_ring->m_ring.end() - m_ring->m_read >= -rel);
    else
        if (m_ring->m_ring.end() - m_ring->m_read < -rel)
            throw out_of_range(outOfRangeStr);
    return m_ring->m_ring.end() + rel;
}

template<typename T, typename Alloc>
inline
ring_buffer<T, Alloc>::const_iterator::const_iterator()
#if !defined(NDEBUG)
    : m_ring(nullptr)
#endif
{
}

template<typename T, typename Alloc>
inline
ring_buffer<T, Alloc>::const_iterator::const_iterator(const_iterator const& it) :
    m_ring(it.m_ring),
    m_it(it.m_it)
{
}

template<typename T, typename Alloc>
inline
ring_buffer<T, Alloc>::const_iterator::const_iterator(ring_buffer& ring, it_t it) :
    m_ring(&ring),
    m_it(it)
{
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::const_iterator& ring_buffer<T, Alloc>::const_iterator::operator +=(std::ptrdiff_t offset)
{
    m_it = offset_it(offset);
    return *this;
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::const_iterator& ring_buffer<T, Alloc>::const_iterator::operator -=(std::ptrdiff_t offset)
{
    m_it = offset_it(-offset);
    return *this;
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::const_iterator& ring_buffer<T, Alloc>::const_iterator::operator =(const_iterator const& it)
{
    m_ring = it.m_ring;
    m_it = it.m_it;
    return *this;
}

template<typename T, typename Alloc>
inline
bool ring_buffer<T, Alloc>::const_iterator::operator ==(const_iterator const& it) const
{
    assert(m_ring == it.m_ring);
    return m_it == it.m_it;
}

template<typename T, typename Alloc>
inline
bool ring_buffer<T, Alloc>::const_iterator::operator !=(const_iterator const& it) const
{
    assert(m_ring == it.m_ring);
    return m_it != it.m_it;
}

template<typename T, typename Alloc>
inline
bool ring_buffer<T, Alloc>::const_iterator::operator  <(const_iterator const& it) const
{
    assert(m_ring == it.m_ring);
    return idx() < it.idx();
}

template<typename T, typename Alloc>
inline
bool ring_buffer<T, Alloc>::const_iterator::operator <=(const_iterator const& it) const
{
    assert(m_ring == it.m_ring);
    return idx() <= it.idx();
}

template<typename T, typename Alloc>
inline
bool ring_buffer<T, Alloc>::const_iterator::operator >(const_iterator const& it) const
{
    assert(m_ring == it.m_ring);
    return idx() > it.idx();
}

template<typename T, typename Alloc>
inline
bool ring_buffer<T, Alloc>::const_iterator::operator >=(const_iterator const& it) const
{
    assert(m_ring == it.m_ring);
    return idx() >= it.idx();
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::const_iterator ring_buffer<T, Alloc>::const_iterator::operator +(std::ptrdiff_t offset) const
{
    return const_iterator(*m_ring, offset_it(offset));
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::const_iterator ring_buffer<T, Alloc>::const_iterator::operator -(std::ptrdiff_t offset) const
{
    return const_iterator(*m_ring, offset_it(-offset));
}

template<typename T, typename Alloc>
inline
T const& ring_buffer<T, Alloc>::const_iterator::operator *() const
{
    assert(idx() < (std::ptrdiff_t)m_ring->m_size);
    return m_it->t;
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::const_iterator& ring_buffer<T, Alloc>::const_iterator::operator ++()
{
    m_it = offset_it(1);
    return *this;
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::const_iterator& ring_buffer<T, Alloc>::const_iterator::operator --()
{
    m_it = offset_it(-1);
    return *this;
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::const_iterator ring_buffer<T, Alloc>::const_iterator::operator ++(int)
{
    const_iterator ret(*this);
    m_it = offset_it(1);
    return ret;
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::const_iterator ring_buffer<T, Alloc>::const_iterator::operator --(int)
{
    const_iterator ret(*this);
    m_it = offset_it(-1);
    return ret;
}

template<typename T, typename Alloc>
inline
T const& ring_buffer<T, Alloc>::const_iterator::operator [](std::ptrdiff_t offset) const
{
    assert(offset_it(offset) != m_ring->m_write);
    return offset_it(offset)->t;
}

template<typename T, typename Alloc>
inline
T const& ring_buffer<T, Alloc>::const_iterator::at(std::ptrdiff_t offset) const
{
    assert(offset_it(offset) != m_ring->m_write);
    return offset_it<true>(offset)->t;
}

template<typename T, typename Alloc>
inline
ring_buffer<T, Alloc>::iterator::iterator() :
    const_iterator()
{
}

template<typename T, typename Alloc>
inline
ring_buffer<T, Alloc>::iterator::iterator(iterator const& it) :
    const_iterator(it)
{
}

template<typename T, typename Alloc>
inline
ring_buffer<T, Alloc>::iterator::iterator(ring_buffer& ring, it_t it) :
    const_iterator(ring, it)
{
}

template<typename T, typename Alloc>
inline
ring_buffer<T, Alloc>::iterator::iterator(const_iterator const& it) :
    const_iterator(it)
{
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::iterator& ring_buffer<T, Alloc>::iterator::operator +=(std::ptrdiff_t offset)
{
    return (iterator&)const_iterator::operator +=(offset);
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::iterator& ring_buffer<T, Alloc>::iterator::operator -=(std::ptrdiff_t offset)
{
    return (iterator&)const_iterator::operator -=(-offset);
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::iterator& ring_buffer<T, Alloc>::iterator::operator =(iterator const& it)
{
    return (iterator&)const_iterator::operator =(it);
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::iterator ring_buffer<T, Alloc>::iterator::operator +(std::ptrdiff_t offset) const
{
    return (iterator)const_iterator::operator +(offset);
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::iterator ring_buffer<T, Alloc>::iterator::operator -(std::ptrdiff_t offset) const
{
    return (iterator)const_iterator::operator -(offset);
}

template<typename T, typename Alloc>
inline
T& ring_buffer<T, Alloc>::iterator::operator *() const
{
    return (T&)const_iterator::operator *();
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::iterator& ring_buffer<T, Alloc>::iterator::operator ++()
{
    return (iterator&)const_iterator::operator ++();
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::iterator& ring_buffer<T, Alloc>::iterator::operator --()
{
    return (iterator&)const_iterator::operator --();
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::iterator ring_buffer<T, Alloc>::iterator::operator ++(int)
{
    return (iterator)const_iterator::operator ++(1);
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::iterator ring_buffer<T, Alloc>::iterator::operator --(int)
{
    return (iterator)const_iterator::operator --(1);
}

template<typename T, typename Alloc>
inline
T& ring_buffer<T, Alloc>::iterator::operator [](std::ptrdiff_t offset) const
{
    return (T&)const_iterator::operator [](offset);
}

template<typename T, typename Alloc>
inline
T& ring_buffer<T, Alloc>::iterator::at(std::ptrdiff_t offset) const
{
    return (T&)const_iterator::at(offset);
}

template<typename T, typename Alloc>
ring_buffer<T, Alloc>::ring_buffer(std::size_t cap) :
    m_ring(cap + 1),
    m_read(m_ring.begin()),
    m_write(m_ring.begin()),
    m_size(0),
    m_capacity(cap)
{
}

template<typename T, typename Alloc>
ring_buffer<T, Alloc>::ring_buffer(ring_buffer const& other) :
    m_ring(other.m_capacity + 1),
    m_size(other.m_size),
    m_capacity(other.m_capacity),
    m_read(m_ring.begin()),
    m_write(m_ring.begin() + m_size)
{
    it_t write = m_read;
    try
    {
        for (TU const& tu : other.m_ring)
            new((void*)&*write++)T(tu.t);
    }
    // if an exception occurs, destroy the objects which we've copied so far
    catch (...)
    {
        for (it_t begin = m_read; write > begin; (--write)->t.~T());
        throw;
    }
}

template<typename T, typename Alloc>
ring_buffer<T, Alloc>::~ring_buffer()
{
    auto destrRange = [](it_t destr, it_t end)
    {
        for (; destr != end; destr++->t.~T());
    };
    if (m_read <= m_write)
    {
        destrRange(m_read, m_write);
        return;
    }
    destrRange(m_read, m_ring.end());
    destrRange(m_ring.begin(), m_write);
}

template<typename T, typename Alloc>
inline
T& ring_buffer<T, Alloc>::operator [](std::size_t offset)
{
    using namespace std;
    assert(offset < m_size);
    if (m_write >= m_read)
    {
        assert((size_t)(m_ring.end() - m_ring.begin()) > offset);
        return m_read[offset].t;
    }
    if (offset < (size_t)(m_ring.end() - m_read))
        return m_read[offset].t;
    offset -= m_ring.end() - m_read;
    assert((size_t)(m_write - m_ring.begin()) > offset);
    return m_ring[offset].t;
}

template<typename T, typename Alloc>
inline
T& ring_buffer<T, Alloc>::at(std::size_t offset)
{
    using namespace std;
    if (m_size == 0)
        throw logic_error("ring-buffer empty");
    if (m_write >= m_read)
        if (offset < (size_t)(m_write - m_read))
            return m_read[offset].t;
        else
            throw out_of_range("ring-buffer index too large");
    if (offset < (size_t)(m_ring.end() - m_read))
        return m_read[offset].t;
    if ((offset -= m_ring.end() - m_read) >= (size_t)(m_write - m_ring.begin()))
        throw out_of_range("ring-buffer index too large");
    return m_ring[offset].t;
}

template<typename T, typename Alloc>
inline
T& ring_buffer<T, Alloc>::front() const
{
    assert(m_size);
    return m_read->t;
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::iterator ring_buffer<T, Alloc>::begin() const
{
    return iterator(*(ring_buffer*)this, m_read);
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::iterator ring_buffer<T, Alloc>::end() const
{
    return iterator(*(ring_buffer*)this, m_write);
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::const_iterator ring_buffer<T, Alloc>::cbegin() const
{
    return const_iterator(*(ring_buffer*)this, m_read);
}

template<typename T, typename Alloc>
inline
typename ring_buffer<T, Alloc>::const_iterator ring_buffer<T, Alloc>::cend() const
{
    return const_iterator(*(ring_buffer*)this, m_write);
}

template<typename T, typename Alloc>
inline
void ring_buffer<T, Alloc>::pop_front()
{
    assert(m_size);
    m_read->t.~T();
    if (++m_read == m_ring.end())
        m_read = m_ring.begin();
    --m_size;
}

template<typename T, typename Alloc>
template<typename ... Args>
T& ring_buffer<T, Alloc>::emplace_back(Args &&...args)
{
    using namespace std;
    assert(m_capacity);
    if (m_size == m_capacity)
        pop_front();
    T& t = m_write->t;
    new((void*)&t)T(forward<Args>(args) ...);
    if (++m_write == m_ring.end())
        m_write = m_ring.begin();
    ++m_size;
    return t;
}

template<typename T, typename Alloc>
T& ring_buffer<T, Alloc>::push_back(T const& other)
{
    using namespace std;
    assert(m_capacity);
    if (m_size == m_capacity)
        pop_front();
    T& t = m_write->t;
    new((void*)&t)T(other);
    if (++m_write == m_ring.end())
        m_write = m_ring.begin();
    ++m_size;
    return t;
}

template<typename T, typename Alloc>
T& ring_buffer<T, Alloc>::push_back(T&& other)
{
    using namespace std;
    assert(m_capacity);
    if (m_size == m_capacity)
        pop_front();
    T& t = m_write->t;
    new((void*)&t)T(move(other));
    if (++m_write == m_ring.end())
        m_write = m_ring.begin();
    ++m_size;
    return t;
}

template<typename T, typename Alloc>
inline
std::size_t ring_buffer<T, Alloc>::size() const
{
    return m_size;
}

template<typename T, typename Alloc>
inline
bool ring_buffer<T, Alloc>::full() const
{
    return m_size == m_capacity;
}

template<typename T, typename Alloc>
void ring_buffer<T, Alloc>::capacity(std::size_t newCap)
{
    using namespace std;
    if (newCap < m_size)
        throw invalid_argument("new ring-buffer size too small");
    vec_t newRing(newCap + 1);
    it_t  to = newRing.begin();
    auto moveRange = [&to](it_t from, it_t fromEnd)
    {
        for (; from != fromEnd; ++from, ++to)
            new((void*)&*to)T(move(from->t));
    };
    auto destrRange = [](it_t destr, it_t end)
    {
        for (; destr != end; (--destr)->t.~T());
    };
    try
    {
        // destroy after copying / moving completely because whe might copy only if there isn't
        // move-constructor and have an exception; so the whole thing becomes transactional
        if (m_read <= m_write)
            moveRange(m_read, m_write),
            destrRange(m_write, m_read);
        else
            moveRange(m_read, m_ring.end()),
            moveRange(m_ring.begin(), m_write),
            destrRange(m_write, m_ring.begin()),
            destrRange(m_ring.end(), m_read);
    }
    // if there's an exception, the objects haven't been moved but copied;
    // so we can destroy the objects in the destination-vector created so far
    catch (...)
    {
        destrRange(to, newRing.begin());
        throw;
    }
    swap(m_ring, newRing);
    m_read = m_ring.begin();
    m_write = to;
    m_capacity = newCap;
}

template<typename T, typename Alloc>
void swap(ring_buffer<T, Alloc>& a, ring_buffer<T, Alloc>& b)
{
    swap(a.m_ring, b.m_ring);
    swap(a.m_read, b.m_read);
    swap(a.m_write, b.m_write);
    swap(a.m_capacity, b.m_capacity);
    swap(a.m_size, b.m_size);
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#if defined(__llvm__)
#pragma clang diagnostic pop
#endif
