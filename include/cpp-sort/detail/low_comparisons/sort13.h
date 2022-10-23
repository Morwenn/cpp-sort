/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LOW_COMPARISONS_SORT13_H_
#define CPPSORT_DETAIL_LOW_COMPARISONS_SORT13_H_

namespace cppsort::detail
{
    template<>
    struct low_comparisons_sorter_impl<13u>
    {
        template<
            mstd::random_access_iterator Iterator,
            typename Compare = std::less<>,
            typename Projection = std::identity,
            typename = mstd::enable_if_t<is_projection_iterator_v<
                Projection, Iterator, Compare
            >>
        >
        auto operator()(Iterator first, Iterator,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            using utility::iter_swap;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            low_comparisons_sorter<11u>{}(first + 1u, first + 12u, compare, projection);
            iter_swap_if(first, first + 12u, compare, projection);

            if (comp(proj(first[1u]), proj(first[0u]))) {
                iter_swap(first, first + 1u);
                if (comp(proj(first[2u]), proj(first[1u]))) {
                    iter_swap(first + 1u, first + 2u);
                    if (comp(proj(first[3u]), proj(first[2u]))) {
                        iter_swap(first + 2u, first + 3u);
                        if (comp(proj(first[4u]), proj(first[3u]))) {
                            iter_swap(first + 3u, first + 4u);
                            if (comp(proj(first[5u]), proj(first[4u]))) {
                                iter_swap(first + 4u, first + 5u);
                                if (comp(proj(first[6u]), proj(first[5u]))) {
                                    iter_swap(first + 5u, first + 6u);
                                    if (comp(proj(first[7u]), proj(first[6u]))) {
                                        iter_swap(first + 6u, first + 7u);
                                        if (comp(proj(first[8u]), proj(first[7u]))) {
                                            iter_swap(first + 7u, first + 8u);
                                            if (comp(proj(first[9u]), proj(first[8u]))) {
                                                iter_swap(first + 8u, first + 9u);
                                                if (comp(proj(first[10u]), proj(first[9u]))) {
                                                    iter_swap(first + 9u, first + 10u);
                                                    iter_swap_if(first + 10u, first + 11u,
                                                                 compare, projection);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (comp(proj(first[12u]), proj(first[11u]))) {
                iter_swap(first + 11u, first + 12u);
                if (comp(proj(first[11u]), proj(first[10u]))) {
                    iter_swap(first + 10u, first + 11u);
                    if (comp(proj(first[10u]), proj(first[9u]))) {
                        iter_swap(first + 9u, first + 10u);
                        if (comp(proj(first[9u]), proj(first[8u]))) {
                            iter_swap(first + 8u, first + 9u);
                            if (comp(proj(first[8u]), proj(first[7u]))) {
                                iter_swap(first + 7u, first + 8u);
                                if (comp(proj(first[7u]), proj(first[6u]))) {
                                    iter_swap(first + 6u, first + 7u);
                                    if (comp(proj(first[6u]), proj(first[5u]))) {
                                        iter_swap(first + 5u, first + 6u);
                                        if (comp(proj(first[5u]), proj(first[4u]))) {
                                            iter_swap(first + 4u, first + 5u);
                                            if (comp(proj(first[4u]), proj(first[3u]))) {
                                                iter_swap(first + 3u, first + 4u);
                                                if (comp(proj(first[3u]), proj(first[2u]))) {
                                                    iter_swap(first + 2u, first + 3u);
                                                    iter_swap_if(first + 1u, first + 2u,
                                                                 std::move(compare), std::move(projection));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    };
}

#endif // CPPSORT_DETAIL_LOW_COMPARISONS_SORT13_H_
