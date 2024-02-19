#include <stdexcept>
#include <utility>
#include <vector>
template<class T>
class MinHeap {
public:
    MinHeap(int d);
    /* Constructor that builds a d-ary Min Heap
       This should work for any d >= 2,
       but doesn't have to do anything for smaller d.*/
    ~MinHeap();
    void add(T item, int priority);
    /* adds the item to the heap, with the given priority. */

    const T& peek() const;
    /* returns the element with smallest priority.
       Break ties however you wish.
       Throws an exception if the heap is empty. */
    const int peekpriority() const;
    /* returns the priority of the element with smallest priority.
       Break ties however you wish.
       Throws an exception if the heap is empty. */
    void remove();
    /* removes the element with smallest priority.
       Break ties however you wish.
       Throws an exception if the heap is empty. */

    bool isEmpty();
    /* returns true iff there are no elements on the heap. */

private:
    // whatever you need to naturally store things.
    // You may also add helper functions here.
    std::vector<std::pair<T, int>> heap;  // both prio and value in one vector
    void bubbleup();
    void bubbledown();
    int md;
};
template<class T>
MinHeap<T>::MinHeap(int d) {
    md = d;
}
template<class T>
MinHeap<T>::~MinHeap() {}
template<class T>
void MinHeap<T>::add(T item, int priority) {  // adds and resorts with bubble up
    heap.push_back(std::make_pair(item, priority));
    bubbleup();
}
template<class T>
const int MinHeap<T>::peekpriority() const {  // returns the tops priority
    if (heap.size() == 0) {                   // error thrown if heap is empty
        throw std::logic_error("Heap Empty");
    }
    return (heap[0]).second;
}
template<class T>
const T& MinHeap<T>::peek() const {  // returns the tops value
    if (heap.size() == 0) {          // error thrown if heap is empty
        throw std::logic_error("Heap Empty");
    }
    return (heap[0]).first;
}
template<class T>
void MinHeap<T>::remove() {  // takes the top and removes it
    if (heap.size() == 0) {  // error thrown if heap is empty
        throw std::logic_error("Heap Empty");
    }
    heap[0] = heap.back();
    heap.pop_back();
    bubbledown();
}
template<class T>
void MinHeap<T>::bubbleup() {
    int bubble = heap.size() - 1;
    int parent = (bubble - 1) / md;
    std::pair<T, int> temp;
    while (parent >= 0) {
        if ((heap[bubble]).second < (heap[parent]).second) {  // if larger move
            temp = heap[parent];
            heap[parent] = heap[bubble];
            heap[bubble] = temp;
            bubble = parent;
            if (bubble == 0) {  // reached the top
                return;
            }
            parent = (bubble - 1) / md;  // set new parent
        } else if ((heap[bubble]).first < (heap[parent]).first && (heap[bubble]).second == (heap[parent]).second) {
            temp = heap[parent];  // equal but string larger
            heap[parent] = heap[bubble];
            heap[bubble] = temp;
            bubble = parent;
            if (bubble == 0) {
                return;
            }
            parent = (bubble - 1) / md;  // set new parent
        } else {
            return;
        }
    }
}
template<class T>
void MinHeap<T>::bubbledown() {
    int child = 0 * md + 1;
    int bubble = 0;
    std::pair<T, int> temp;
    int smallest = heap[bubble].second;
    int swappoint = 0;
    int size = heap.size();
    bool done = true;
    while (child < size) {
        for (int i = 0; i < md; i++) {  // goes through all children
            if ((child + i) < size) {
                if (heap[child + i].second < smallest
                    || (heap[child + i].first < heap[swappoint].first && heap[child + i].second == smallest)) {
                    smallest = heap[child + i].second;  // if smallest set it
                    swappoint = child + i;
                    done = false;  // more work to do
                }
            } else {
                temp = heap[swappoint];  // if out of range swap any last potential then end
                heap[swappoint] = heap[bubble];
                heap[bubble] = temp;
                bubble = swappoint;
                child = bubble * md + 1;
                return;
            }
        }
        if (done) {  // no more moves
            return;
        }
        temp = heap[swappoint];  // swaps up the smaller value
        heap[swappoint] = heap[bubble];
        heap[bubble] = temp;
        bubble = swappoint;
        child = bubble * md + 1;  // find new child range
        done = true;
        smallest = heap[bubble].second;  // have to reset smallest for next check
    }
}
template<class T>
bool MinHeap<T>::isEmpty() {
    if (heap.size() == 0) {  // nothing inside?
        return true;
    }
    return false;
}
