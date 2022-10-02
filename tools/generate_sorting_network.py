# -*- coding: utf-8 -*-

import ast
import sys
import textwrap

import z3


def parse_network(path) -> list[list[tuple]]:
    """
    Reads a file and returns for each line of list of tuples representing the indices
    of elements on which to perform compare-and-swap operations.
    """
    res: list[list[tuple]] = []
    with open(path) as fd:
        for line in fd:
            res.append(ast.literal_eval(line))
    return res


def swap_if(a, b):
    # When a, b are in {0, 1} this function returns min(a, b), max(a, b), AKA a
    # compare-and-swap unit in a sorting network.

    # Note that min(a, b) is a & b and max(a, b) is a | b.
    return z3.And(a, b), z3.Or(a, b)


def verify_network(pairs: list[tuple]):
    # Zero-one principle: if the sorting network is valid for all boolean inputs
    # then it is valid for all inputs.
    n = max(max(i, j) for i, j in pairs) + 1
    inputs = [z3.Bool("x" + str(k)) for k in range(n)]

    network = inputs[:]
    for i, j in pairs:
        network[i], network[j] = swap_if(network[i], network[j])

    # Attempt to find counterexample.
    solver = z3.Solver()
    solver.add(z3.Or(*[z3.And(network[i-1], z3.Not(network[i])) for i in range(1, n)]))
    if solver.check() == z3.sat:
        model = solver.model()
        return False, [int(bool(model[inp])) for inp in inputs]
    return True, None


def generate_cxx(network: list[list[tuple]]):
    template = textwrap.dedent("""
        namespace cppsort
        {{
        namespace detail
        {{
            template<>
            struct sorting_network_sorter_impl<{nb_inputs}>
            {{
                template<
                    typename RandomAccessIterator,
                    typename Compare = std::less<>,
                    typename Projection = utility::identity,
                    typename = detail::enable_if_t<is_projection_iterator_v<
                        Projection, RandomAccessIterator, Compare
                    >>
                >
                auto operator()(RandomAccessIterator first, RandomAccessIterator,
                                Compare compare={{}}, Projection projection={{}}) const
                    -> void
                {{
                    {swaps}
                }}

                template<typename DifferenceType=std::ptrdiff_t>
                CPPSORT_ATTRIBUTE_NODISCARD
                static constexpr auto index_pairs()
                    -> std::array<utility::index_pair<DifferenceType>, {nb_indices}>
                {{
                    return {{{{
                        {indices}
                    }}}};
                }}
            }};
        }}}}
    """)
    
    pairs = sum(network, [])

    # Find highest index in pairs
    highest_index = max(
        max(pair[0] for pair in pairs),
        max(pair[1] for pair in pairs)
    )

    # Generate pairs of swaps
    swaps = []
    for pair in pairs:
        lhs = "first" if pair[0] == 0 else f"first + {pair[0]}"
        rhs = "first" if pair[1] == 0 else f"first + {pair[1]}"
        swaps.append(f"iter_swap_if({lhs}, {rhs}, compare, projection);")
    
    # Generate list of indices
    indices = []
    for line in network:
        indices.append(", ".join(f"{{{pair[0]}, {pair[1]}}}" for pair in line) + ",")
    
    return template.format(
        nb_inputs=highest_index + 1,
        swaps="\n            ".join(swaps),
        indices="\n                ".join(indices),
        nb_indices=len(pairs),
    )


def main():
    network = parse_network(sys.argv[1])
    pairs = sum(network, [])
    print(f"Number of pairs: {len(pairs)}")

    valid, failing_input = verify_network(pairs)
    if not valid:
        print("Network failed to sort: {failing_input}")
        exit(1)

    print(generate_cxx(network))


if __name__ == '__main__':
    main()
