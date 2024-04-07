#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <memory>
#include <cmath>

const int TIME_LIMIT = 9000;
int RIGHTS_LIMIT = 10000;
// W.R.Wojtyna 
// The algorithm implemented here is the Complete Karmarkar–Karp algorithm as anytime algorithm.
// My only outside sources for this code were:
// I. Instruction and the example provided by my professor
// https://www.spoj.com/PAA2023/problems/JOHNNY/
// II. Wikipedia page on Karmarkar–Karp algorithm
// https://en.wikipedia.org/wiki/Largest_differencing_method
// Notice that this page does not contain any code - this implementation is 100% original
// I essentially only used four paragraphs that best explain the K-K algorithm:
// Step 1. Order the numbers from large to small.
// Step 2. Replace the largest and second-largest numbers by their difference.
// Step 3. If two or more numbers remain, return to step 1.
// Step 4. Using backtracking, compute the partition.
// III Random Numbers generator (for testing purposes)
// https://numbergenerator.org/randomnumbergenerator

// In the current version, it prints out the original indexes of numbers in set one.
// "Original" meaning their indexes in the input.
// However, one could easily change it so it'll print out numbers from set one or their sum (see comments in the backtrack function).

// I think complexities of the functions go as follows:
// input is approximately           O(n)
// output is approx.                O(n)
// differenciation is approx.       O(1)
// summation is approx.             O(1)
// sort_binary is approx.           O(logn) 
// std::sort guarantees             O(nlogn)
// backtrack is approx.             O(n)
// third_tree_creation can reach    O(n!)
// backtrack is an interesting one, it traverses a tree, the number of calls should be roughly equal to the number of subtractions earlier

using namespace std;

auto startTime = std::chrono::high_resolution_clock::now();


struct Number {
    long long diff = 0; // difference = minuend - subtrahend
    int dx = -1; // index
    shared_ptr <Number> min = nullptr; // minuend 
    shared_ptr <Number> sub = nullptr; // subtrahend 
    bool is_diff = true;

};

struct DataSet {
    vector<Number> vec;
    unsigned long long sum = 0;
    bool visited_right = false;
    bool visited_left = false;
    bool parent_left;
    void clearVector() {
        for (size_t i = 0; i < vec.size(); ++i) {
            Number& num = vec[i];
            if (num.min != nullptr) {
                num.min.reset();
            }
            if (num.sub != nullptr) {
                num.sub.reset();
            }
        }

        vec.resize(0);
        vec.shrink_to_fit();
        sum = 0;
    }

    void clear() {
        visited_right = false;
        visited_left = false;
    }
};

bool is_leaf(const DataSet& dataset) {
    return dataset.vec.size() == 1;
}

bool should_prune(const DataSet& dataset, unsigned long long sum) {
    return (dataset.sum - dataset.vec.back().diff < dataset.vec.back().diff);
}

bool should_create_left_child(const DataSet& dataset) {
    return !dataset.visited_left;
}

bool should_create_right_child(const DataSet& dataset) {
    return !dataset.visited_right;
}

bool compare_numbs(const Number& a, const Number& b) {
    // No comment needed.
    return a.diff < b.diff;
}

void input(int& num, vector<Number>& numdeq, unsigned  long long& sum) {
    // No comment needed.
    cin >> num;
    for (int i = 0; i < num; i++) {
        Number* a = new Number();
        long long diff = 0;
        cin >> diff;
        sum += diff;
        a->diff = diff;
        a->dx = i + 1;
        numdeq.emplace_back(*a);
    }
}

void output(vector<long long> vec) {
    // No comment needed.
    for (long long element : vec) {
        std::cout << element << endl;
    }
}

void differenciation(vector<Number>& numdeq, int size) {
    shared_ptr<Number> a = make_shared<Number>(numdeq[size - 1]);
    shared_ptr<Number> b = make_shared<Number>(numdeq[size - 2]);
    numdeq.pop_back();
    numdeq[size - 2].diff = a->diff - b->diff;
    numdeq[size - 2].min = a;
    numdeq[size - 2].sub = b;
    numdeq[size - 2].dx = -1;
}

void summation(vector<Number>& numdeq, int size) {
    shared_ptr<Number> a = make_shared<Number>(numdeq[size - 1]);
    shared_ptr<Number> b = make_shared<Number>(numdeq[size - 2]);
    numdeq.pop_back();
    numdeq[size - 2].diff = a->diff + b->diff;
    numdeq[size - 2].min = a;
    numdeq[size - 2].sub = b;
    numdeq[size - 2].dx = -1;
    numdeq[size - 2].is_diff = false;
}

void binarySearch(vector<Number>& numdeq, int last, int low, int high) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (numdeq[mid].diff < numdeq[last].diff)
            low = mid + 1;
        else
            high = mid - 1;
    }
    numdeq.insert(numdeq.begin() + low, numdeq[last]);
    numdeq.pop_back();
    return;
}

void backtrack(vector<long long>& vec, shared_ptr <Number> nm, bool in_set1) {
    if (nm->dx != -1) { // dx = -1 means it's not a leaf
        if (in_set1) {
            vec.emplace_back(nm->dx); // Replace this with nm->diff to get a vector of numbers in set1.
        }
        return;
    }
    else {
        if (nm->is_diff) {
            backtrack(vec, nm->min, in_set1);
            backtrack(vec, nm->sub, !in_set1);
        }
        else {
            backtrack(vec, nm->min, in_set1);
            backtrack(vec, nm->sub, in_set1);
        }
    }
}

void tree_creation(vector<Number>& numdeq, int size) {
    // Using a combination of differenciation and binarySearch we essentially transform our vector of "numbers" 
    // into a binary tree (more or less) of numbers, where leaves are our original numbers from the vector
    // and parent nodes are the difference of their child nodes.
    while (size != 1) {
        differenciation(numdeq, size);
        size--;
        binarySearch(numdeq, size - 1, 0, size);
    }
}

void createLeftChildNode(DataSet& parent, DataSet& child) {
    child.vec = parent.vec;
    child.sum = parent.sum - 2 * parent.vec[parent.vec.size() - 1].diff;
    differenciation(child.vec, child.vec.size());
    if (child.vec.size() != 1) {
        binarySearch(child.vec, child.vec.size() - 1, 0, child.vec.size() - 1);
    }
}

void createRightChildNode(DataSet& parent, DataSet& child) {
    child.vec = parent.vec;
    child.sum = parent.sum;
    summation(child.vec, child.vec.size());
    if (child.vec.size() != 1) {
        binarySearch(child.vec, child.vec.size() - 1, 0, child.vec.size() - 1);
    }
}

void clearNode(DataSet& node) {
    node.clear();
}

long long elapsedTimeCheck() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
}


void set_limit(int how_many_weights) {
    RIGHTS_LIMIT = ceil(log(how_many_weights) / logf(2.4));
}

void third_tree_creation(DataSet numdeq, int& size, long long& best_diff, Number& best_numb, unsigned long long sum, int hmw) {
    int indexed = 5;
    int ts = 0;
    int hmi =0;
    int rightsn = 0;
    int leftsn = 0;
    int going_left = 0;
    DataSet ax;
    vector<DataSet> mempool;
    mempool.reserve(hmw + 100);
    mempool.resize(hmw + 100);
    mempool[indexed] = numdeq;
    long long elapsedTime = elapsedTimeCheck();
    double pruneFactor = 1.0;

    while (elapsedTime <= TIME_LIMIT) {
        hmi ++ ;
        if (best_diff == 0) {
            return;
        }
        if (is_leaf(mempool[indexed])) {
            if (mempool[indexed].parent_left) {
                leftsn--;
            }
            else {
                rightsn--;
            }
            if (mempool[indexed].vec[0].diff < best_diff) {
                best_diff = mempool[indexed].vec[0].diff;
                best_numb = mempool[indexed].vec[0];
                std::cout << hmi <<endl;
                std::cout << best_diff << endl;

            }
            clearNode(mempool[indexed]);
            indexed--;
        }
        else if (should_prune(mempool[indexed], sum)) {
            clearNode(mempool[indexed]);
            if (mempool[indexed].parent_left) {
                leftsn--;
            }
            else {
                rightsn--;
            }
            indexed--;
        }
        else if (should_create_left_child(mempool[indexed])) {
            leftsn++;
            createLeftChildNode(mempool[indexed], mempool[indexed + 1]);
            mempool[indexed].visited_left = true;
            indexed++;
            mempool[indexed].parent_left = true;
        }
        else if ((rightsn < RIGHTS_LIMIT) && should_create_right_child(mempool[indexed]) && mempool[indexed].vec.size() > 5     ) { //(rightsn<6)&&
            going_left = 0;
            rightsn++;
            createRightChildNode(mempool[indexed], mempool[indexed + 1]);
            mempool[indexed].visited_right = true;
            indexed++;
            mempool[indexed].parent_left = false;
        }
        else {
            clearNode(mempool[indexed]);
            if (mempool[indexed].parent_left) {
                leftsn--;
            }
            else {
                rightsn--;
            }
            if (indexed == 5) { return; }
            indexed--;
        }
        ts++;
        if (ts == 100) {
            elapsedTime = elapsedTimeCheck();
            ts = 0;
        }


        double elapsedFraction = static_cast<double>(elapsedTime) / TIME_LIMIT;
        pruneFactor = 1.0 - elapsedFraction;
    }
}



int main() {
    ios::sync_with_stdio(false);
    unsigned long long sum = 0;
    long long best_diff = 0;
    int how_many_weights = 0;
    int size = 0;
    Number best_number;
    vector<long long> left_hand;
    vector<Number> weights;
    vector<Number> weights_copy;
    input(how_many_weights, weights, sum);
    std::sort(weights.begin(), weights.end(), compare_numbs);
    weights_copy = weights;
    tree_creation(weights, how_many_weights);
    DataSet inputDataSet;
    inputDataSet.vec = weights_copy;
    inputDataSet.sum = sum;
    best_number = weights[0];
    set_limit(how_many_weights);
    best_diff = weights[0].diff;
    if (how_many_weights > 4 && how_many_weights < 10000) {
        third_tree_creation(inputDataSet, how_many_weights, best_diff, best_number, sum, how_many_weights);
    }
    shared_ptr<Number> best = make_shared<Number>(best_number);
    backtrack(left_hand, best, true);
    std::sort(left_hand.begin(), left_hand.end());

  //  output(left_hand);
    return 0;
}
