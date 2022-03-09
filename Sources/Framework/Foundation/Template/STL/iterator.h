//***********************************************************
//! @file
//! @brief		イテレータ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <iterator>

namespace ob {

    using std::iterator_traits;
    using std::input_iterator_tag;
    using std::output_iterator_tag;
    using std::forward_iterator_tag;
    using std::bidirectional_iterator_tag;
    using std::random_access_iterator_tag;

    using std::advance;
    using std::distance;
    using std::next;
    using std::prev;

    using std::reverse_iterator;
    using std::make_reverse_iterator;

    using std::back_insert_iterator;
    using std::back_inserter;
    using std::front_insert_iterator;
    using std::front_inserter;
    using std::insert_iterator;
    using std::inserter;

    using std::move_iterator;
    using std::make_move_iterator;

    //using std::istream_iterator;
    //using std::ostream_iterator;
    //using std::istreambuf_iterator;
    //using std::ostreambuf_iterator;

    using std::begin;
    using std::end;
    using std::cbegin;
    using std::cend;
    using std::rbegin;
    using std::rend;
    using std::crbegin;
    using std::crend;

    using std::size;
    using std::empty;
    using std::empty;
    using std::data;

}// namespcae ob