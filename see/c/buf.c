#include <stdio.h>

#include "req.h"
#include "memory.h"

#include "buf.h"

int main()
{
    int_ptr arr = 0;
    req(buf_size(arr) == buf_cap(arr));
    req(buf_size(arr) == 0);

    buf_NEW(arr, 10);
    req(buf_size(arr) == buf_cap(arr));
    req(buf_size(arr) == 10);

    dobuf(arr, i,
          printf("%d ", *AT(arr, i)),
          *AT(arr, i) = 3
          );

    buf_RESIZE(arr, 5);
    req(buf_size(arr) == buf_cap(arr));
    req(buf_size(arr) == 5);
    printbuf_int(arr);

    buf_PUSH(arr, 4);
    printf("After push: ");
    printf("cap: %u\t", buf_cap(arr));
    printf("size: %u\n", buf_size(arr));
    req(buf_size(arr) == 6);
    printbuf_int(arr);

    buf_POP(arr);
    printf("After pop: ");
    printf("cap: %u\t", buf_cap(arr));
    printf("size: %u\n", buf_size(arr));
    req(buf_size(arr) == 5);
    printbuf_int(arr);

    buf_free(arr);
    buf_POP(arr);
    return 0;
}
