
#pragma once
#include <unordered_map>
#include <queue>

#define TLB_SIZE 32

class LRU_tlb {
private:
    std::unordered_map<int, int> tlb;
    using HashItr = std::unordered_map<int, int>::iterator;
    std::queue<int> cache;
    int hits;
    int misses;

public:
    LRU_tlb() : hits(0), misses(0) { }

    bool contains(int page_num){
        auto found = tlb.find(page_num);

        if(found == tlb.end()) { 
            ++misses; 
            return false; 
            }

        ++hits;
        return true;
    }
    
    void push(int page_num, int phys_add) {
        if(tlb.size() > TLB_SIZE){
            
            auto recent = cache.front();
            cache.pop();
            tlb.erase(recent);
        }
        tlb[page_num] = phys_add;
        cache.push(page_num);
    }

    decltype(tlb)::mapped_type operator[](int page_num){
        return tlb[page_num];
    }

    decltype(hits)   getHits()   { return this->hits;   }
    decltype(misses) getMisses() { return this->misses; }
};
