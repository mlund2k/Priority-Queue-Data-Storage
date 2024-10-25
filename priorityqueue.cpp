#include <iostream>
#include "json.hpp"

#include "priorityqueue.h"
using namespace std; // for cleanliness

PriorityQueue::PriorityQueue(std::size_t max_size) :
	nodes_(max_size + 1, KeyValuePair()),
	max_size_(max_size),
	size_(0) {
}

void PriorityQueue::insert(Key k) {
	insert(std::make_pair(k, std::make_pair(0, 0)));
}

void PriorityQueue::insert(KeyValuePair kv) {
	size_++; // increment size of heap
    nodes_[size_] = kv; // append new keyvaluepair at last entry
    heapifyUp(size_); // call heapifyUp on new last node
}

KeyValuePair PriorityQueue::min() {
	return this->nodes_[ROOT]; // return minimum node
}

KeyValuePair PriorityQueue::removeMin() {
	KeyValuePair kvr = this->nodes_[ROOT]; // define root node
    size_t min = ROOT; // initialize minimum as root key
    for(size_t i=1; i<=size_; i++){ // loop through all keys
        if(this->nodes_[i].first < this->nodes_[min].first){ // if found a smaller key
            min = i; // new smallest key set here
        }
    }
    removeNode(min); // call removeNode function on minimum key
    return kvr; // returns previous root node
}

bool PriorityQueue::isEmpty() const {
	if(this->size_<1){ // check if size of heap is 0
        return true; // returns true if max size of heap is 0
    }
    else{
        return false; // returns false if max size of heap is greater than 0
    }
}

size_t PriorityQueue::size() const {
	return this->size_; // return size of heap
}

nlohmann::json PriorityQueue::JSON() const {
	nlohmann::json result;
  for (size_t i = 1; i <= size_; i++) {
      nlohmann::json node;
      KeyValuePair kv = nodes_[i];
      node["key"] = kv.first;
      node["value"] = kv.second;
      if (i != ROOT) {
          node["parent"] = std::to_string(i / 2);
      }
      if (2 * i <= size_) {
          node["leftChild"] = std::to_string(2 * i);
      }
      if (2 * i + 1 <= size_) {
          node["rightChild"] = std::to_string(2 * i + 1);
      }
      result[std::to_string(i)] = node;
  }
  result["metadata"]["max_size"] = max_size_;
  result["metadata"]["size"] = size_;
	return result;
}

void PriorityQueue::heapifyUp(size_t i) {
	KeyValuePair kv = nodes_[i]; // define target node
    if(i!=ROOT){ // check if provided node is root
        KeyValuePair kvp = nodes_[i/2]; // define parent node
        if(kv.first<kvp.first){ // compare keys of node and its parent
            std::swap(nodes_[i],nodes_[i/2]); // if parent node is larger, swap the two nodes
            heapifyUp(i/2); // and recursively call heapifyUp on new parent
        }
    }
    
}

void PriorityQueue::heapifyDown(size_t i) {
	KeyValuePair kv = nodes_[i]; // define target node
    if(i<size_){ // check if node has children to swap with
        if(((2*i) <= size_) && ((2*i)+1 > size_)){
            KeyValuePair kvl = nodes_[2*i]; // define left child node
            if(kvl.first<kv.first){ // if left child is smaller than target node
                std::swap(nodes_[i],nodes_[2*i]); // swap with left child
                heapifyDown(2*i); // and recursively call heapifyDown on left child
            }
        }
        else if((2*i)+1 <= size_){
            KeyValuePair kvl = nodes_[2*i]; // define left child node
            KeyValuePair kvr = nodes_[2*i+1]; //define right child node
            if(kvl.first<kvr.first){ // if the left node is smaller
                if(kvl.first<kv.first){ // if left child is smaller than target node
                    std::swap(nodes_[i],nodes_[2*i]); // swap with left child
                    heapifyDown(2*i); // and recursively call heapifyDown on left child
                }
                else if(kvr.first<kv.first){ // or if right child is smaller than target node
                    std::swap(nodes_[i],nodes_[2*i+1]); // swap with right child
                    heapifyDown(2*i+1); // and recursively call heapifyDown on right child
                }
            }
            else if(kvr.first<kvl.first){ // or if the right node is smaller
                if(kvr.first<kv.first){ // if right child is smaller than target node
                    std::swap(nodes_[i],nodes_[2*i+1]); // swap with right child
                    heapifyDown(2*i+1); // and recursively call heapifyDown on right child
                }
                else if(kvl.first<kv.first){ // or if left child is smaller than target node
                    std::swap(nodes_[i],nodes_[2*i]); // swap with left child
                    heapifyDown(2*i); // and recursively call heapifyDown on left child
                }
            }
        }
    }
}

void PriorityQueue::removeNode(size_t i) {
	std::swap(nodes_[i],nodes_[this->size_]); // swap last child with node to remove
    nodes_[this->size_].first = 0.0; // setting removed node to 0 to avoid ghost nodes (garbage data)
    this->size_--; // decrement size
    heapifyUp(i); // first move node up to necessary spot
    heapifyDown(i); // then move node down until tree is complete
}

Key PriorityQueue::getKey(size_t i) {
	KeyValuePair kv = nodes_[i]; // define target node
    return kv.first; // return target node's key value
}
