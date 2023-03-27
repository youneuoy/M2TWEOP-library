// original file taken from here:
// https://gist.github.com/mattbierner/6145505#file-tuple_ops-h

/*------------------------------------------------------------------------------
    Function list operations for std::tuple.
    
    Operations are performed on tuple values. Unless noted, these operations
    result in regular flat tuples and not recursive data structures.
------------------------------------------------------------------------------*/

/* Implementation --------------------- */
template<std::size_t... XS>
struct Seq
{
    template<std::size_t X>
    using cons = Seq<X, XS...>;
};

template<std::size_t start, std::size_t end>
struct Range
{
    static_assert(start < end, "Range: start > end");
    typedef
        typename Range<start + 1, end>::type::template cons<start>
        type;
};

template<std::size_t start>
struct Range<start, start>
{
    typedef Seq<> type;
};

template<typename T, std::size_t... N>
std::tuple<typename std::tuple_element<N, T>::type...> Select(const T& t, Seq<N...>)
{
    return std::make_tuple(std::get<N>(t)...);
}

/* API --------------------- */
/**
 * Empty list.
 */
constexpr const std::tuple<> empty;

/**
 * Get the first element of a tuple.
 *
 * Not valid for empty tuples.
 */
template<typename T>
typename std::tuple_element<0, T>::type Car(const T& t)
{
    return std::get<0>(t);
}

/**
 * Get a new tuple consisting of the rest of the tuple besides the first element.
 *
 * Not valid for empty tuples.
 */
template<typename T,
    typename R = typename Range<1, std::tuple_size<T>::value>::type>
auto Cdr(const T& t) -> decltype(Select(t, R()))
{
    return Select(t, R());
}

/**
 * Create a new tuple from a head value and existing tuple.
 */
template<typename X, typename... XS>
std::tuple<X, XS...> Cons(X x, const std::tuple<XS...>& t)
{
    return std::tuple_cat(std::make_tuple(x), t);
}