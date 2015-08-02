import sys
import matplotlib.pyplot as plt


def fetch_results(fresults):
    results = fresults.readline().split(' ')
    results.pop()
    return [int(elem) for elem in results]

    
if __name__ == '__main__':
    # Results of timing functions
    cppsort = []
    stdsort = []
    
    # Fetch the results
    with open(sys.argv[1]) as f:
        cppsort = fetch_results(f)
        stdsort = fetch_results(f)
            
    # Plot the results
    xaxis = list(range(len(cppsort)))
    line_cpp, = plt.plot(xaxis, cppsort)
    line_std, = plt.plot(xaxis, stdsort)
    plt.legend([line_cpp, line_std], ['cppsort::sort', 'std::sort'])
    plt.xlabel('Number of elements to sort')
    plt.ylabel('Execution time (ms)')
    plt.show()
