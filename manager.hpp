
#pragma once
#include <cstring>
#include <iostream>
#include <string>
#include "address.hpp"
#include "file.hpp"
#include "LRU_TLB.hpp"

#define PAGE_TABLE_SIZE 256
#define PHYS_MEM_SIZE 65536
#define FRAME_SIZE 256
#define SIMULATION_SIZE 1000 
#define PAGE_SIZE 256

class Manager {

private:
    LRU_tlb tlb;
    SBYTE physical_mem[PHYS_MEM_SIZE];

    int place;
    int first_avail;
    int page_table[PAGE_TABLE_SIZE]; 
    int page_fault;
    int page_total;

    File* location, *correct, *binstore;
public:
    Manager() : place(0), first_avail(0), page_fault(0), page_total(0) {

        location  = new File("../data/addresses.txt");
        correct = new File("../data/correct.txt");
        binstore  = new File("../data/BACKING_STORE.bin");

        memset(page_table, -1, sizeof(page_table));
    }

    ~Manager() {
        delete location;
        delete correct;
        delete binstore;
    }

    SBYTE getVal(int address) {
        LogicalAddress direct(address);

        BYTE page_num = direct.getPage(), 
             offset   = direct.getOffset();

        int physical_address;
        bool success = tlb.contains(page_num);

        if(success) { 
            printf("TLB HIT!\n");
            physical_address = tlb[page_num] + offset;

        } else { 
            printf("TLB MISS!\n");
            physical_address = page_table[page_num];
            
            if(physical_address == -1){
                physical_address = handlePageFault(page_num);
            } ++page_total;

            tlb.push(page_num, physical_address);
            physical_address += offset;
        }

        printf("Virtual address: %d Physical address: %d Value: %d ", 
                    address, physical_address, physical_mem[physical_address]);

        return physical_mem[physical_address];
    }

    int handlePageFault(BYTE pagen) {
        printf("PAGE FAULT AT: %d\n", pagen);
        ++page_fault;

        int size_of_page = PAGE_SIZE;
        int new_physical_address = place*FRAME_SIZE;

        for(int x{}; x < size_of_page; ++x){
            physical_mem[new_physical_address + x] = (*binstore)[pagen*size_of_page + x];
        }

        page_table[pagen] = new_physical_address;

        place = (place + 1) % FRAME_SIZE;
        return new_physical_address;
    }

    void simulate() {

        int total{}; 
        int total_correct{};
        int value; 
        int correct_value;

        for(int var{}; var < SIMULATION_SIZE; ++var, ++total) {

            value = this->getVal(location->getAddress()); 
            correct_value = correct->parseValue();
            printf("Correct: %d\n", correct_value);

            if(value == correct_value) 
            ++total_correct;
        }
        float accuracy = (float)total_correct/(float)total;
        printf("----Accuracy: %.0f%%----", accuracy*100.0); 

        auto hits = tlb.getHits();
        auto misses = tlb.getMisses();

        printf("TLB HITS: %d----TLB MISSES: %d----TLB-hit Rate: %.0f%%\n", 
               hits, misses, (float(hits)/float(hits + misses)) * 100);

        printf("Page Fault Rate: %0.f%%", (float)page_fault/(float)page_total * 100);
    }

    void test(int address) {
        LogicalAddress direct(address);
        printf("page: %d, offset: %d\n", direct.getPage(), direct.getOffset());
    }
};
