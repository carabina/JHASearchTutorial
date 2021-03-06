/*************************************************************************
 *
 * REALM CONFIDENTIAL
 * __________________
 *
 *  [2011] - [2014] Realm Inc
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Realm Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Realm Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Realm Incorporated.
 *
***************************************************************************/

#ifndef REALM_ARRAY_INTEGER_HPP
#define REALM_ARRAY_INTEGER_HPP

#include <realm/array.hpp>
#include <realm/util/safe_int_ops.hpp>

namespace realm {

class ArrayInteger: public Array {
public:
    typedef int64_t value_type;

    explicit ArrayInteger(no_prealloc_tag) REALM_NOEXCEPT;
    explicit ArrayInteger(Allocator&) REALM_NOEXCEPT;
    ~ArrayInteger() REALM_NOEXCEPT override {}

    void add(int64_t value);
    void set(std::size_t ndx, int64_t value);
    void set_uint(std::size_t ndx, uint64_t value) REALM_NOEXCEPT;
    int64_t get(std::size_t ndx) const REALM_NOEXCEPT;
    uint64_t get_uint(std::size_t ndx) const REALM_NOEXCEPT;
    static int64_t get(const char* header, std::size_t ndx) REALM_NOEXCEPT;

    /// Add \a diff to the element at the specified index.
    void adjust(std::size_t ndx, int_fast64_t diff);

    /// Add \a diff to all the elements in the specified index range.
    void adjust(std::size_t begin, std::size_t end, int_fast64_t diff);

    /// Add signed \a diff to all elements that are greater than, or equal to \a
    /// limit.
    void adjust_ge(int_fast64_t limit, int_fast64_t diff);

    int64_t operator[](std::size_t ndx) const REALM_NOEXCEPT { return get(ndx); }
    int64_t front() const REALM_NOEXCEPT;
    int64_t back() const REALM_NOEXCEPT;

    std::size_t lower_bound(int64_t value) const REALM_NOEXCEPT;
    std::size_t upper_bound(int64_t value) const REALM_NOEXCEPT;

    std::vector<int64_t> ToVector() const;

private:
    template<size_t w> bool minmax(size_t from, size_t to, uint64_t maxdiff,
                                   int64_t* min, int64_t* max) const;
};

class ArrayIntNull: public Array {
public:
    typedef int64_t value_type;

    explicit ArrayIntNull(no_prealloc_tag) REALM_NOEXCEPT;
    explicit ArrayIntNull(Allocator&) REALM_NOEXCEPT;
    ~ArrayIntNull() REALM_NOEXCEPT override;

    /// Construct an array of the specified type and size, and return just the
    /// reference to the underlying memory. All elements will be initialized to
    /// the specified value.
    static MemRef create_array(Type, bool context_flag, std::size_t size, int_fast64_t value,
                               Allocator&);
    void create(Type, bool context_flag = false);

    void init_from_ref(ref_type) REALM_NOEXCEPT;
    void init_from_mem(MemRef) REALM_NOEXCEPT;
    void init_from_parent() REALM_NOEXCEPT;

    std::size_t size() const REALM_NOEXCEPT;
    bool is_empty() const REALM_NOEXCEPT;

    void insert(std::size_t ndx, int_fast64_t value);
    void insert(std::size_t ndx, null);
    void add(int64_t value);
    void add(null);
    void set(std::size_t ndx, int64_t value) REALM_NOEXCEPT;
    void set(std::size_t ndx, null) REALM_NOEXCEPT;
    void set_uint(std::size_t ndx, uint64_t value) REALM_NOEXCEPT;
    int64_t get(std::size_t ndx) const REALM_NOEXCEPT;
    uint64_t get_uint(std::size_t ndx) const REALM_NOEXCEPT;
    static int64_t get(const char* header, std::size_t ndx) REALM_NOEXCEPT;

    void set_null(std::size_t ndx) REALM_NOEXCEPT;
    bool is_null(std::size_t ndx) const REALM_NOEXCEPT;
    int64_t null_value() const REALM_NOEXCEPT;

    int64_t operator[](std::size_t ndx) const REALM_NOEXCEPT;
    int64_t front() const REALM_NOEXCEPT;
    int64_t back() const REALM_NOEXCEPT;
    void erase(std::size_t ndx);
    void erase(std::size_t begin, std::size_t end);
    void truncate(std::size_t size);
    void clear();
    void set_all_to_zero();

    void move(std::size_t begin, std::size_t end, std::size_t dest_begin);
    void move_backward(std::size_t begin, std::size_t end, std::size_t dest_end);

    std::size_t lower_bound(int64_t value) const REALM_NOEXCEPT;
    std::size_t upper_bound(int64_t value) const REALM_NOEXCEPT;

    int64_t sum(std::size_t start = 0, std::size_t end = npos) const;
    std::size_t count(int64_t value) const REALM_NOEXCEPT;
    bool maximum(int64_t& result, std::size_t start = 0, std::size_t end = npos,
        std::size_t* return_ndx = nullptr) const;
    bool minimum(int64_t& result, std::size_t start = 0, std::size_t end = npos,
                 std::size_t* return_ndx = nullptr) const;

    bool find(int cond, Action action, int64_t value, std::size_t start, std::size_t end, std::size_t baseindex,
              QueryState<int64_t>* state) const;

    template<class cond, Action action, std::size_t bitwidth, class Callback>
    bool find(int64_t value, std::size_t start, std::size_t end, std::size_t baseindex,
              QueryState<int64_t>* state, Callback callback) const;

    // This is the one installed into the m_finder slots.
    template<class cond, Action action, std::size_t bitwidth>
    bool find(int64_t value, std::size_t start, std::size_t end, std::size_t baseindex,
              QueryState<int64_t>* state) const;

    template<class cond, Action action, class Callback>
    bool find(int64_t value, std::size_t start, std::size_t end, std::size_t baseindex,
              QueryState<int64_t>* state, Callback callback) const;

    // Optimized implementation for release mode
    template<class cond2, Action action, std::size_t bitwidth, class Callback>
    bool find_optimized(int64_t value, std::size_t start, std::size_t end, std::size_t baseindex,
                        QueryState<int64_t>* state, Callback callback) const;

    // Called for each search result
    template<Action action, class Callback>
    bool find_action(std::size_t index, int64_t value,
                     QueryState<int64_t>* state, Callback callback) const;

    template<Action action, class Callback>
    bool find_action_pattern(std::size_t index, uint64_t pattern,
                             QueryState<int64_t>* state, Callback callback) const;

    // Wrappers for backwards compatibility and for simple use without
    // setting up state initialization etc
    template<class cond>
    std::size_t find_first(int64_t value, std::size_t start = 0,
                           std::size_t end = npos) const;

    void find_all(IntegerColumn* result, int64_t value, std::size_t col_offset = 0,
                  std::size_t begin = 0, std::size_t end = npos) const;

    std::size_t find_first(int64_t value, std::size_t begin = 0,
                           std::size_t end = npos) const;

    // Overwrite Array::bptree_leaf_insert to correctly split nodes.
    ref_type bptree_leaf_insert(std::size_t ndx, int64_t value, TreeInsertBase& state);
    ref_type bptree_leaf_insert(std::size_t ndx, null, TreeInsertBase& state);

    MemRef slice(std::size_t offset, std::size_t size, Allocator& target_alloc) const;

    /// Construct a deep copy of the specified slice of this array using the
    /// specified target allocator. Subarrays will be cloned.
    MemRef slice_and_clone_children(std::size_t offset, std::size_t size,
                                    Allocator& target_alloc) const;
protected:
    void avoid_null_collision(int64_t value);
private:
    template<bool find_max>
    bool minmax_helper(int64_t& result, std::size_t start = 0, std::size_t end = npos,
                         std::size_t* return_ndx = nullptr) const;

    int_fast64_t choose_random_null(int64_t incoming);
    void replace_nulls_with(int64_t new_null);
    bool can_use_as_null(int64_t value);
};


// Implementation:

inline ArrayInteger::ArrayInteger(Array::no_prealloc_tag) REALM_NOEXCEPT:
    Array(Array::no_prealloc_tag())
{
}

inline ArrayInteger::ArrayInteger(Allocator& alloc) REALM_NOEXCEPT:
    Array(alloc)
{
}

inline void ArrayInteger::add(int64_t value)
{
    Array::add(value);
}

inline int64_t ArrayInteger::get(size_t ndx) const REALM_NOEXCEPT
{
    return Array::get(ndx);
}

inline uint64_t ArrayInteger::get_uint(std::size_t ndx) const REALM_NOEXCEPT
{
    return get(ndx);
}

inline int64_t ArrayInteger::get(const char* header, size_t ndx) REALM_NOEXCEPT
{
    return Array::get(header, ndx);
}

inline void ArrayInteger::set(size_t ndx, int64_t value)
{
    Array::set(ndx, value);
}

inline void ArrayInteger::set_uint(std::size_t ndx, uint_fast64_t value) REALM_NOEXCEPT
{
    // When a value of a signed type is converted to an unsigned type, the C++
    // standard guarantees that negative values are converted from the native
    // representation to 2's complement, but the effect of conversions in the
    // opposite direction is left unspecified by the
    // standard. `realm::util::from_twos_compl()` is used here to perform the
    // correct opposite unsigned-to-signed conversion, which reduces to a no-op
    // when 2's complement is the native representation of negative values.
    set(ndx, util::from_twos_compl<int_fast64_t>(value));
}


inline int64_t ArrayInteger::front() const REALM_NOEXCEPT
{
    return Array::front();
}

inline int64_t ArrayInteger::back() const REALM_NOEXCEPT
{
    return Array::back();
}

inline void ArrayInteger::adjust(std::size_t ndx, int_fast64_t diff)
{
    Array::adjust(ndx, diff);
}

inline void ArrayInteger::adjust(std::size_t begin, std::size_t end, int_fast64_t diff)
{
    Array::adjust(begin, end, diff);
}

inline void ArrayInteger::adjust_ge(int_fast64_t limit, int_fast64_t diff)
{
    Array::adjust_ge(limit, diff);
}

inline std::size_t ArrayInteger::lower_bound(int64_t value) const REALM_NOEXCEPT
{
    return lower_bound_int(value);
}

inline std::size_t ArrayInteger::upper_bound(int64_t value) const REALM_NOEXCEPT
{
    return upper_bound_int(value);
}


inline
ArrayIntNull::ArrayIntNull(no_prealloc_tag tag) REALM_NOEXCEPT: Array(tag)
{
}

inline
ArrayIntNull::ArrayIntNull(Allocator& alloc) REALM_NOEXCEPT: Array(alloc)
{
}

inline
ArrayIntNull::~ArrayIntNull() REALM_NOEXCEPT
{
}

inline
void ArrayIntNull::create(Type type, bool context_flag)
{
    MemRef r = create_array(type, context_flag, 0, 0, m_alloc);
    init_from_mem(r);
}

inline
std::size_t ArrayIntNull::size() const REALM_NOEXCEPT
{
    return Array::size() - 1;
}

inline
bool ArrayIntNull::is_empty() const REALM_NOEXCEPT
{
    return size() == 0;
}

inline
void ArrayIntNull::insert(std::size_t ndx, int_fast64_t value)
{
    avoid_null_collision(value);
    Array::insert(ndx + 1, value);
}

inline
void ArrayIntNull::insert(std::size_t ndx, null)
{
    Array::insert(ndx + 1, null_value());
}

inline
void ArrayIntNull::add(int64_t value)
{
    avoid_null_collision(value);
    Array::add(value);
}

inline
void ArrayIntNull::add(null)
{
    Array::add(null_value());
}

inline
void ArrayIntNull::set(std::size_t ndx, int64_t value) REALM_NOEXCEPT
{
    avoid_null_collision(value);
    Array::set(ndx + 1, value);
}

inline
void ArrayIntNull::set(std::size_t ndx, null) REALM_NOEXCEPT
{
    Array::set(ndx + 1, null_value());
}

inline
void ArrayIntNull::set_null(std::size_t ndx) REALM_NOEXCEPT
{
    Array::set(ndx + 1, null_value());
}

inline
void ArrayIntNull::set_uint(std::size_t ndx, uint64_t value) REALM_NOEXCEPT
{
    avoid_null_collision(value);
    Array::set(ndx + 1, value);
}

inline
int64_t ArrayIntNull::get(std::size_t ndx) const REALM_NOEXCEPT
{
    return Array::get(ndx + 1);
}

inline
uint64_t ArrayIntNull::get_uint(std::size_t ndx) const REALM_NOEXCEPT
{
    return Array::get(ndx + 1);
}

inline
int64_t ArrayIntNull::get(const char* header, std::size_t ndx) REALM_NOEXCEPT
{
    return Array::get(header, ndx + 1);
}

inline
bool ArrayIntNull::is_null(std::size_t ndx) const REALM_NOEXCEPT
{
    return Array::get(ndx + 1) == null_value();
}

inline
int64_t ArrayIntNull::null_value() const REALM_NOEXCEPT
{
    return Array::get(0);
}

inline
int64_t ArrayIntNull::operator[](std::size_t ndx) const REALM_NOEXCEPT
{
    return get(ndx);
}

inline
int64_t ArrayIntNull::front() const REALM_NOEXCEPT
{
    return get(0);
}

inline
int64_t ArrayIntNull::back() const REALM_NOEXCEPT
{
    return Array::back();
}

inline
void ArrayIntNull::erase(std::size_t ndx)
{
    Array::erase(ndx + 1);
}

inline
void ArrayIntNull::erase(std::size_t begin, std::size_t end)
{
    Array::erase(begin + 1, end + 1);
}

inline
void ArrayIntNull::truncate(std::size_t size)
{
    Array::truncate(size + 1);
}

inline
void ArrayIntNull::clear()
{
    truncate(0);
}

inline
void ArrayIntNull::set_all_to_zero()
{
    // FIXME: Array::set_all_to_zero does something else
    for (size_t i = 0; i < size(); ++i) {
        set(i, 0);
    }
}

inline
void ArrayIntNull::move(std::size_t begin, std::size_t end, std::size_t dest_begin)
{
    Array::move(begin + 1, end + 1, dest_begin + 1);
}

inline
void ArrayIntNull::move_backward(std::size_t begin, std::size_t end, std::size_t dest_end)
{
    Array::move_backward(begin + 1, end + 1, dest_end + 1);
}

inline
std::size_t ArrayIntNull::lower_bound(int64_t value) const REALM_NOEXCEPT
{
    // FIXME: Consider this behaviour with NULLs.
    // Array::lower_bound_int assumes an already sorted array, but
    // this array could be sorted with nulls first or last.
    return Array::lower_bound_int(value);
}

inline
std::size_t ArrayIntNull::upper_bound(int64_t value) const REALM_NOEXCEPT
{
    // FIXME: see lower_bound
    return Array::upper_bound_int(value);
}

inline
int64_t ArrayIntNull::sum(std::size_t start, std::size_t end) const
{
    // FIXME: Optimize!
    int64_t null = null_value();
    int64_t sum = 0;
    if (end == npos)
        end = size();
    for (size_t i = start; i < end; ++i) {
        int64_t x = get(i);
        if (x != null) {
            sum += x;
        }
    }
    return sum;
}

inline
std::size_t ArrayIntNull::count(int64_t value) const REALM_NOEXCEPT
{
    std::size_t count = Array::count(value);
    if (value == null_value()) {
        --count;
    }
    return count;
}

// FIXME: Optimize!
template<bool find_max>
inline
bool ArrayIntNull::minmax_helper(int64_t& result, std::size_t start, std::size_t end, std::size_t* return_ndx) const
{
    size_t best_index = 1;

    if (end == npos) {
        end = m_size;
    }

    ++start;

    REALM_ASSERT(start < m_size && end <= m_size && start < end);

    if (m_size == 1) {
        // empty array
        return false;
    }

    if (m_width == 0) {
        if (return_ndx)
            *return_ndx = best_index - 1;
        result = 0;
        return true;
    }

    int64_t m = Array::get(start);

    const int64_t null_val = null_value();
    for (; start < end; ++start) {
        const int64_t v = Array::get(start);
        if (find_max ? v > m : v < m) {
            if (v == null_val) {
                continue;
            }
            m = v;
            best_index = start;
        }
    }

    result = m;
    if (return_ndx) {
        *return_ndx = best_index - 1;
    }
    return true;
}

inline
bool ArrayIntNull::maximum(int64_t& result, std::size_t start, std::size_t end, std::size_t* return_ndx) const
{
    return minmax_helper<true>(result, start, end, return_ndx);
}

inline
bool ArrayIntNull::minimum(int64_t& result, std::size_t start, std::size_t end, std::size_t* return_ndx) const
{
    return minmax_helper<false>(result, start, end, return_ndx);
}

inline
bool ArrayIntNull::find(int cond, Action action, int64_t value, std::size_t start, std::size_t end, std::size_t baseindex, QueryState<int64_t>* state) const
{
    ++start;
    if (end != npos) {
        ++end;
    }
    return Array::find(cond, action, value, start, end, baseindex, state);
}


template<class cond, Action action, std::size_t bitwidth, class Callback>
bool ArrayIntNull::find(int64_t value, std::size_t start, std::size_t end, std::size_t baseindex, QueryState<int64_t>* state, Callback callback) const
{
    ++start;
    if (end != npos) {
        ++end;
    }
    return Array::find<cond, Action, bitwidth, Callback>(value, start, end, baseindex, state, callback);
}


template<class cond, Action action, std::size_t bitwidth>
bool ArrayIntNull::find(int64_t value, std::size_t start, std::size_t end, std::size_t baseindex, QueryState<int64_t>* state) const
{
    ++start;
    if (end != npos) {
        ++end;
    }
    return Array::find<cond, Action, bitwidth>(value, start, end, baseindex, state);
}


template<class cond, Action action, class Callback>
bool ArrayIntNull::find(int64_t value, std::size_t start, std::size_t end, std::size_t baseindex, QueryState<int64_t>* state, Callback callback) const
{
    ++start;
    if (end != npos) {
        ++end;
    }
    return Array::find<cond, Action, Callback>(value, start, end, baseindex, state, callback);
}


template<Action action, class Callback>
bool ArrayIntNull::find_action(std::size_t index, int64_t value, QueryState<int64_t>* state, Callback callback) const
{
    ++index;
    return Array::find_action<Action, Callback>(index, value, state, callback);
}


template<Action action, class Callback>
bool ArrayIntNull::find_action_pattern(std::size_t index, uint64_t pattern, QueryState<int64_t>* state, Callback callback) const
{
    ++index;
    return Array::find_action_pattern<Action, Callback>(index, pattern, state, callback);
}


template<class cond>
std::size_t ArrayIntNull::find_first(int64_t value, std::size_t start, std::size_t end) const
{
    ++start;
    if (end != npos) {
        ++end;
    }
    std::size_t found = Array::find_first<cond>(value, start, end);
    if (found != npos) {
        --found;
    }
    return found;
}

inline
std::size_t ArrayIntNull::find_first(int64_t value, std::size_t begin, std::size_t end) const
{
    ++begin;
    if (end != npos) {
        ++end;
    }
    std::size_t found = Array::find_first(value, begin, end);
    if (found != npos) {
        --found;
    }
    return found;
}


}

#endif // REALM_ARRAY_INTEGER_HPP
