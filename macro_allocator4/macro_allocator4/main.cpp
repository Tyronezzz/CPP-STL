//
//  main.cpp
//  macro_allocator4
//
//  Created by Tyrone Hou on 11/9/20.
//  Copyright © 2020 Tyrone Hou. All rights reserved.
//

#include <iostream>
#include <complex>

using namespace std;


//DECLAREE_POOL_ALLOC-- used in class definition
#define DECLARE_POOL_ALLOC() \
    public: \
        void* operator new(size_t size) {return myAlloc.allocate(size);}\
        void operator delete(void* p) {return myAlloc.deallocate(p, 0);} \
    public:\
        static my_allocator myAlloc;

//IMPLEMENT_POOL_ALLOC -- used in class implementation file
#define IMPLEMENT_POOL_ALLOC(class_name) \
    my_allocator class_name::myAlloc



class my_allocator{
    
public:
    struct obj{
        struct obj* next;
    };
    
public:
    my_allocator() {}
    ~my_allocator() {}
public:
    void* allocate(size_t);
    void deallocate(void* p, size_t);
    obj* freeStore = nullptr;
    int const CHUNK = 5;
};

void* my_allocator::allocate(size_t size) {
    //cout << "allocating..." << endl;
    
    obj* p;
    if(!freeStore) {
        freeStore = p = static_cast<obj*>(malloc(size * CHUNK));
        
        for(int i = 1; i < CHUNK - 1; i ++) {
            p->next = static_cast<obj*>(p + size);
            p = p->next;
        }
        p->next = nullptr;
    }
    p = freeStore;
    freeStore = freeStore->next;
    return p;
}


void my_allocator::deallocate(void* p, size_t) {
    static_cast<obj*>(p)->next= freeStore;
    freeStore = static_cast<obj*>(p);
}

class Foo{
    DECLARE_POOL_ALLOC();
    public:
        long L;
        string str;
    public:
        Foo(long _L) : L(_L) {}
};
IMPLEMENT_POOL_ALLOC(Foo);

class Goo{
    DECLARE_POOL_ALLOC();
    public:
        complex<double> c;
        std::string str;
    public:
        Goo(const complex<double>& l) : c(l) {}
};
IMPLEMENT_POOL_ALLOC(Goo);




int main(int argc, const char * argv[]) {
    
    Foo* p[10];
    cout << "sizeof(Foo)" << sizeof(Foo) << endl;
    for(int i =0 ; i < 5; i ++) {
        p[i] = new Foo(static_cast<long>(i));
        cout << p[i] << endl;
    }
    
    for(int i = 0; i < 5; i ++) {
        delete p[i];
    }

    cout<<"----------------------"<<endl;
    
    
    
    Goo *Gp[100];
    cout << "sizeof(Goo): " << sizeof(Goo) << endl;
    for(int i = 0; i < 10; i ++) {
        Gp[i] = new Goo(complex<double>(i, i + 1));
        cout << Gp[i] << ' '<< Gp[i]->c << endl;
    }
    
    for(int i = 0; i < 10; i ++) {
        delete p[i];
    }
    
    
    return 0;
}
