#include <client/client.h>

#include <stdio.h>
#include <inttypes.h>

static uint64_t
rdtsc()
{
        uint32_t lo, hi;

#ifdef __GNUC__
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
#else
        asm("rdtsc" : "=a" (lo), "=d" (hi));
#endif

        return (((uint64_t)hi << 32) | lo);
}

int
main()
{
    RAMCloud::Client *client = new RAMCloud::DefaultClient();
    uint64_t b;
    uint64_t table;

    b = rdtsc();
    client->CreateTable("test");
    table = client->OpenTable("test");
    printf("create+open table took %lu ticks\n", rdtsc() - b);

    b = rdtsc();
    client->Ping();
    printf("ping took %lu ticks\n", rdtsc() - b);

    b = rdtsc();
    client->Write(table, 42, "Hello, World!", 14);
    printf("write took %lu ticks\n", rdtsc() - b);

    char buf[100];
    b = rdtsc();
    uint64_t buf_len;
    client->Read(table, 42, buf, &buf_len);
    printf("read took %lu ticks\n", rdtsc() - b);
    printf("Got back [%s] len %lu\n", buf, buf_len);

    client->DropTable("test");

    delete client;
    return (0);
}
