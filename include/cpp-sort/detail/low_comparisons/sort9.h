/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LOW_COMPARISONS_SORT9_H_
#define CPPSORT_DETAIL_LOW_COMPARISONS_SORT9_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "../rotate_right.h"

namespace cppsort
{
namespace detail
{
    template<>
    struct low_comparisons_sorter_impl<9u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = detail::enable_if_t<is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            // The algorithm used is an unrolled implementation of a more
            // space-efficient variant of the Ford-Johnson merge-insertion
            // sort described by Ayala-Rincón1, de Abreu and de Siqueira

            using utility::iter_move;
            using utility::iter_swap;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            // Order elements pair-wise

            iter_swap_if(first, first + 1u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);

            // Make quadruplets and order them by max value

            if (comp(proj(first[3u]), proj(first[1u]))) {
                iter_swap(first, first + 2u);
                iter_swap(first + 1u, first + 3u);
            }
            if (comp(proj(first[7u]), proj(first[5u]))) {
                iter_swap(first + 4u, first + 6u);
                iter_swap(first + 5u, first + 7u);
            }

            if (comp(proj(first[7u]), proj(first[3u]))) {
                iter_swap(first, first + 4u);
                iter_swap(first + 1u, first + 5u);
                iter_swap(first + 2u, first + 6u);
                iter_swap(first + 3u, first + 7u);
            }

            // Merge insertion, initial state of the collection;
            // note that the initial layout of the elements in
            // memory is A B C D E F G H I

            //       D -- H
            //      /|   /|
            //     B |  F |
            //     | C  | G  I
            //     |    |
            //     A    E

            // First step: insert F into the main chain [A, B, D, H]

            if (comp(proj(first[5u]), proj(first[1u]))) {
                if (comp(proj(first[5u]), proj(first[0u]))) {
                    {
                        auto tmp = iter_move(first + 5u);
                        first[5u] = iter_move(first + 3u);
                        first[3u] = iter_move(first + 1u);
                        first[1u] = std::move(tmp);
                    }
                    {
                        auto tmp = iter_move(first + 4u);
                        first[4u] = iter_move(first + 2u);
                        first[2u] = iter_move(first);
                        first[0u] = std::move(tmp);
                    }

                    // E -- F -- A -- B -- D -- H
                    //                     |    |
                    //                     C    G    I

                    // Second step: insert C into [E, F, A, B]

                    if (comp(proj(first[4u]), proj(first[2u]))) {
                        if (comp(proj(first[4u]), proj(first[1u]))) {
                            if (comp(proj(first[4u]), proj(first[0u]))) {
                                rotate_right<5u>(first);
                            } else {
                                rotate_right<4u>(first + 1u);
                            }
                        } else {
                            rotate_right<3u>(first + 2u);
                        }
                    } else {
                        if (comp(proj(first[4u]), proj(first[3u]))) {
                            rotate_right<2u>(first + 3u);
                        }
                    }

                    // No matter where C landed, we can now insert I
                    // into the chain formed by the first 6 elements
                    // and H

                    {
                        // Reorder G -- H -- I into H -- I -- G in memory
                        // to make subsequent operations easier
                        auto tmp = iter_move(first + 6u);
                        first[6u] = iter_move(first + 7u);
                        first[7u] = iter_move(first + 8u);
                        first[8u] = std::move(tmp);
                    }

                    // 0 -- 1 -- 2 -- 3 -- 4 -- 5 -- H -- I -- G

                    if (comp(proj(first[7u]), proj(first[3u]))) {
                        if (comp(proj(first[7u]), proj(first[1u]))) {
                            if (comp(proj(first[7u]), proj(first[0u]))) {
                                rotate_right<8u>(first);
                            } else {
                                rotate_right<7u>(first + 1u);
                            }
                        } else {
                            if (comp(proj(first[7u]), proj(first[2u]))) {
                                rotate_right<6u>(first + 2u);
                            } else {
                                rotate_right<5u>(first + 3u);
                            }
                        }
                    } else {
                        if (comp(proj(first[7u]), proj(first[5u]))) {
                            if (comp(proj(first[7u]), proj(first[4u]))) {
                                rotate_right<4u>(first + 4u);
                            } else {
                                rotate_right<3u>(first + 5u);
                            }
                        } else {
                            if (comp(proj(first[7u]), proj(first[6u]))) {
                                rotate_right<2u>(first + 6u);
                            } else {
                                // Special case: I is greater than H, so we only
                                // have to insert G into the first 6 elements
                                {
                                    // Reorder H -- I -- G into G -- H -- I
                                    auto tmp = iter_move(first + 8u);
                                    first[8u] = iter_move(first + 7u);
                                    first[7u] = iter_move(first + 6u);
                                    first[6u] = std::move(tmp);
                                }

                                if (comp(proj(first[6u]), proj(first[3u]))) {
                                    if (comp(proj(first[6u]), proj(first[1u]))) {
                                        if (comp(proj(first[6u]), proj(first[0u]))) {
                                            rotate_right<7u>(first);
                                        } else {
                                            rotate_right<6u>(first + 1u);
                                        }
                                    } else {
                                        if (comp(proj(first[6u]), proj(first[2u]))) {
                                            rotate_right<5u>(first + 2u);
                                        } else {
                                            rotate_right<4u>(first + 3u);
                                        }
                                    }
                                } else {
                                    if (comp(proj(first[6u]), proj(first[5u]))) {
                                        if (comp(proj(first[6u]), proj(first[4u]))) {
                                            rotate_right<3u>(first + 4u);
                                        } else {
                                            rotate_right<2u>(first + 5u);
                                        }
                                    }
                                }

                                return;
                            }
                        }
                    }

                    // No special case: insert G into the first 7 elements,
                    // swap it with H first to correctly order them again
                    iter_swap(first + 7u, first + 8u);

                    if (comp(proj(first[7u]), proj(first[3u]))) {
                        if (comp(proj(first[7u]), proj(first[1u]))) {
                            if (comp(proj(first[7u]), proj(first[0u]))) {
                                rotate_right<8u>(first);
                            } else {
                                rotate_right<7u>(first + 1u);
                            }
                        } else {
                            if (comp(proj(first[7u]), proj(first[2u]))) {
                                rotate_right<6u>(first + 2u);
                            } else {
                                rotate_right<5u>(first + 3u);
                            }
                        }
                    } else {
                        if (comp(proj(first[7u]), proj(first[5u]))) {
                            if (comp(proj(first[7u]), proj(first[4u]))) {
                                rotate_right<4u>(first + 4u);
                            } else {
                                rotate_right<3u>(first + 5u);
                            }
                        } else {
                            if (comp(proj(first[7u]), proj(first[6u]))) {
                                rotate_right<2u>(first + 6u);
                            }
                        }
                    }
                } else {
                    auto tmp = iter_move(first + 5u);
                    first[5u] = iter_move(first + 3u);
                    first[3u] = iter_move(first + 1u);
                    first[1u] = iter_move(first + 4u);
                    first[4u] = iter_move(first + 2u);
                    first[2u] = std::move(tmp);

                    // A -- F -- B -- D -- H
                    //      |         |    |
                    //      E         C    G    I

                    // Second step: insert E into [A]

                    iter_swap_if(first, first + 1u, compare, projection);

                    // E -- E -- F -- B -- D -- H
                    // A    A              |    |
                    //                     C    G    I

                    // Third step: insert C into [E, A, F, B]

                    if (comp(proj(first[4u]), proj(first[2u])) ){
                        if (comp(proj(first[4u]), proj(first[1u]))) {
                            if (comp(proj(first[4u]), proj(first[0u]))) {
                                rotate_right<5u>(first);
                            } else {
                                rotate_right<4u>(first + 1u);
                            }
                        } else {
                            rotate_right<3u>(first + 2u);
                        }
                    } else {
                        if (comp(proj(first[4u]), proj(first[3u]))) {
                            rotate_right<2u>(first + 3u);
                        }
                    }

                    // No matter where C landed, we can now insert I
                    // into the chain formed by the first 6 elements
                    // and H

                    {
                        // Reorder G -- H -- I into H -- I -- G in memory
                        // to make subsequent operations easier
                        auto tmp = iter_move(first + 6u);
                        first[6u] = iter_move(first + 7u);
                        first[7u] = iter_move(first + 8u);
                        first[8u] = std::move(tmp);
                    }

                    // 0 -- 1 -- 2 -- 3 -- 4 -- 5 -- H -- I -- G

                    if (comp(proj(first[7u]), proj(first[3u]))) {
                        if (comp(proj(first[7u]), proj(first[1u]))) {
                            if (comp(proj(first[7u]), proj(first[0u]))) {
                                rotate_right<8u>(first);
                            } else {
                                rotate_right<7u>(first + 1u);
                            }
                        } else {
                            if (comp(proj(first[7u]), proj(first[2u]))) {
                                rotate_right<6u>(first + 2u);
                            } else {
                                rotate_right<5u>(first + 3u);
                            }
                        }
                    } else {
                        if (comp(proj(first[7u]), proj(first[5u]))) {
                            if (comp(proj(first[7u]), proj(first[4u]))) {
                                rotate_right<4u>(first + 4u);
                            } else {
                                rotate_right<3u>(first + 5u);
                            }
                        } else {
                            if (comp(proj(first[7u]), proj(first[6u]))) {
                                rotate_right<2u>(first + 6u);
                            } else {
                                // Special case: I is greater than H, so we only
                                // have to insert G into the first 6 elements
                                {
                                    // Reorder H -- I -- G into G -- H -- I
                                    auto tmp = iter_move(first + 8u);
                                    first[8u] = iter_move(first + 7u);
                                    first[7u] = iter_move(first + 6u);
                                    first[6u] = std::move(tmp);
                                }

                                if (comp(proj(first[6u]), proj(first[3u]))) {
                                    if (comp(proj(first[6u]), proj(first[1u]))) {
                                        if (comp(proj(first[6u]), proj(first[0u]))) {
                                            rotate_right<7u>(first);
                                        } else {
                                            rotate_right<6u>(first + 1u);
                                        }
                                    } else {
                                        if (comp(proj(first[6u]), proj(first[2u]))) {
                                            rotate_right<5u>(first + 2u);
                                        } else {
                                            rotate_right<4u>(first + 3u);
                                        }
                                    }
                                } else {
                                    if (comp(proj(first[6u]), proj(first[5u]))) {
                                        if (comp(proj(first[6u]), proj(first[4u]))) {
                                            rotate_right<3u>(first + 4u);
                                        } else {
                                            rotate_right<2u>(first + 5u);
                                        }
                                    }
                                }

                                return;
                            }
                        }
                    }

                    // No special case: insert G into the first 7 elements,
                    // swap it with H first to correctly order them again
                    iter_swap(first + 7u, first + 8u);

                    if (comp(proj(first[7u]), proj(first[3u]))) {
                        if (comp(proj(first[7u]), proj(first[1u]))) {
                            if (comp(proj(first[7u]), proj(first[0u]))) {
                                rotate_right<8u>(first);
                            } else {
                                rotate_right<7u>(first + 1u);
                            }
                        } else {
                            if (comp(proj(first[7u]), proj(first[2u]))) {
                                rotate_right<6u>(first + 2u);
                            } else {
                                rotate_right<5u>(first + 3u);
                            }
                        }
                    } else {
                        if (comp(proj(first[7u]), proj(first[5u]))) {
                            if (comp(proj(first[7u]), proj(first[4u]))) {
                                rotate_right<4u>(first + 4u);
                            } else {
                                rotate_right<3u>(first + 5u);
                            }
                        } else {
                            if (comp(proj(first[7u]), proj(first[6u]))) {
                                rotate_right<2u>(first + 6u);
                            }
                        }
                    }
                }
            } else {
                if (comp(proj(first[5u]), proj(first[3u]))) {
                    iter_swap(first + 2u, first + 4u);
                    iter_swap(first + 3u, first + 5u);

                    // A -- B -- F -- D -- H
                    //           |    |    |
                    //           E    C    G    I

                    // Second step: insert C into [A, B, F]
                }

                // If the previous branch wasn't taken,
                // E and F are already in place wrt [A, B, D, H]

                // A -- B -- D -- F -- H
                //           |    |    |
                //           C    E    G    I

                // As we can see, now the memory layout has the same
                // shape no matter the branch, so we can unroll the
                // same algorithm for both

                bool c_is_small = true;
                if (comp(proj(first[4u]), proj(first[1u]))) {
                    if (comp(proj(first[4u]), proj(first[0u]))) {
                        rotate_right<5u>(first);
                    } else {
                        rotate_right<4u>(first + 1u);
                    }
                } else {
                    if (comp(proj(first[4u]), proj(first[3u]))) {
                        rotate_right<3u>(first + 2u);
                    } else {
                        // C is already in place wrt [A, B, F]
                        c_is_small = false;

                        // Insert E in [A, B]
                        if (comp(proj(first[2u]), proj(first[1u]))) {
                            if (comp(proj(first[2u]), proj(first[0u]))) {
                                rotate_right<3u>(first);
                            } else {
                                rotate_right<2u>(first + 1u);
                            }
                        } // else E is already in place
                    }
                }

                if (c_is_small) {
                    // C was lesser than F

                    // Insert E in [A, B, C]
                    if (comp(proj(first[3u]), proj(first[1u]))) {
                        if (comp(proj(first[3u]), proj(first[0u]))) {
                            rotate_right<4u>(first);
                        } else {
                            rotate_right<3u>(first + 1u);
                        }
                    } else {
                        if (comp(proj(first[3u]), proj(first[2u]))) {
                            rotate_right<2u>(first + 2u);
                        }
                    }
                }

                // 0 -- 1 -- 2 -- 3 -- 4 -- 5 -- H
                //                               |
                //                               G    I

                // No matter where C landed, we can now insert I
                // into the chain formed by the first 6 elements
                // and H

                {
                    // Reorder G -- H -- I into H -- I -- G in memory
                    // to make subsequent operations easier
                    auto tmp = iter_move(first + 6u);
                    first[6u] = iter_move(first + 7u);
                    first[7u] = iter_move(first + 8u);
                    first[8u] = std::move(tmp);
                }

                // 0 -- 1 -- 2 -- 3 -- 4 -- 5 -- H -- I -- G

                if (comp(proj(first[7u]), proj(first[3u]))) {
                    if (comp(proj(first[7u]), proj(first[1u]))) {
                        if (comp(proj(first[7u]), proj(first[0u]))) {
                            rotate_right<8u>(first);
                        } else {
                            rotate_right<7u>(first + 1u);
                        }
                    } else {
                        if (comp(proj(first[7u]), proj(first[2u]))) {
                            rotate_right<6u>(first + 2u);
                        } else {
                            rotate_right<5u>(first + 3u);
                        }
                    }
                } else {
                    if (comp(proj(first[7u]), proj(first[5u]))) {
                        if (comp(proj(first[7u]), proj(first[4u]))) {
                            rotate_right<4u>(first + 4u);
                        } else {
                            rotate_right<3u>(first + 5u);
                        }
                    } else {
                        if (comp(proj(first[7u]), proj(first[6u]))) {
                            rotate_right<2u>(first + 6u);
                        } else {
                            // Special case: I is greater than H, so we only
                            // have to insert G into the first 6 elements
                            {
                                // Reorder H -- I -- G into G -- H -- I
                                auto tmp = iter_move(first + 8u);
                                first[8u] = iter_move(first + 7u);
                                first[7u] = iter_move(first + 6u);
                                first[6u] = std::move(tmp);
                            }

                            if (comp(proj(first[6u]), proj(first[3u]))) {
                                if (comp(proj(first[6u]), proj(first[1u]))) {
                                    if (comp(proj(first[6u]), proj(first[0u]))) {
                                        rotate_right<7u>(first);
                                    } else {
                                        rotate_right<6u>(first + 1u);
                                    }
                                } else {
                                    if (comp(proj(first[6u]), proj(first[2u]))) {
                                        rotate_right<5u>(first + 2u);
                                    } else {
                                        rotate_right<4u>(first + 3u);
                                    }
                                }
                            } else {
                                if (comp(proj(first[6u]), proj(first[5u]))) {
                                    if (comp(proj(first[6u]), proj(first[4u]))) {
                                        rotate_right<3u>(first + 4u);
                                    } else {
                                        rotate_right<2u>(first + 5u);
                                    }
                                }
                            }

                            return;
                        }
                    }
                }

                // No special case: insert G into the first 7 elements,
                // swap it with H first to correctly order them again
                iter_swap(first + 7u, first + 8u);

                if (comp(proj(first[7u]), proj(first[3u]))) {
                    if (comp(proj(first[7u]), proj(first[1u]))) {
                        if (comp(proj(first[7u]), proj(first[0u]))) {
                            rotate_right<8u>(first);
                        } else {
                            rotate_right<7u>(first + 1u);
                        }
                    } else {
                        if (comp(proj(first[7u]), proj(first[2u]))) {
                            rotate_right<6u>(first + 2u);
                        } else {
                            rotate_right<5u>(first + 3u);
                        }
                    }
                } else {
                    if (comp(proj(first[7u]), proj(first[5u]))) {
                        if (comp(proj(first[7u]), proj(first[4u]))) {
                            rotate_right<4u>(first + 4u);
                        } else {
                            rotate_right<3u>(first + 5u);
                        }
                    } else {
                        if (comp(proj(first[7u]), proj(first[6u]))) {
                            rotate_right<2u>(first + 6u);
                        }
                    }
                }
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_LOW_COMPARISONS_SORT9_H_
