#include <stdio.h>

void main() {
    int in, leakrate, bsize, n, bucket_storage = 0;

    printf("Enter the bucket size: ");
    scanf("%d", &bsize);

    printf("Enter the number of inputs: ");
    scanf("%d", &n);

    printf("Enter the packet outgoing rate: ");
    scanf("%d", &leakrate);

    printf("Initially, Bucket status: contains %d out of %d\n", bucket_storage, bsize);

    while (n != 0) {
        printf("\nEnter the incoming packet size: ");
        scanf("%d", &in);

        printf("Incoming packet size is %d\n", in);

        if (in <= (bsize - bucket_storage)) {
            bucket_storage += in;
            printf("Bucket status: contains %d out of %d\n", bucket_storage, bsize);
        } else {
            printf("Dropped packets: %d\n", in - (bsize - bucket_storage));
            bucket_storage = bsize;
            printf("Bucket status: contains %d out of %d\n", bucket_storage, bsize);
        }

        bucket_storage -= leakrate;
        if (bucket_storage < 0)
            bucket_storage = 0;

        printf("%d packets transmitted, Bucket status: contains %d packets out of %d\n", leakrate, bucket_storage, bsize);
        n--;
    }

    if (bucket_storage != 0) {
        bucket_storage = 0;
        printf("After sending, Bucket contains %d packets out of %d\n", bucket_storage, bsize);
    }
}
