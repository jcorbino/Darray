#include "dist_array.hpp"

using namespace std;

int main()
{
    upcxx::init();

    Darray dist_array({2, 2}, {4, 3});

    dist_array.allocate();

    dist_array.fill();

    dist_array.print();

    Darray fragment({2}, {2});

    fragment.allocate();

    real* buf;
    //dist_array.get_contig(0, 0, 0, 3, (real**)&stdvec);
    dist_array.get_contig(0, 0, 0, 3, &buf, false);
    //fragment.put_contig(0, 0, 1, 1, stdvec.data());
    fragment.put_contig(0, 0, 1, 1, buf);

    fragment.print();

    auto ptr = fragment.local_gptr.local();
    for(uint64_t i = 0; i < fragment.local_nelems; ++i)
        *(ptr + i) += 1;

    fragment.print();

    vector<real> values(dist_array.nelems);

    iota(values.begin(), values.end(), 1.0);

    // Chaining whole matrix operations:
    dist_array.op<'+'>(values.data()).op<'*'>(values.data()); // <'='> by default

    dist_array.print();

    dist_array.deallocate();

    fragment.deallocate();

    upcxx::finalize();

    return 0;
}
