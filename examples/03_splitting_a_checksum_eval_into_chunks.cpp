#include "../cppcrc.h"
#include <stdio.h>

int main()
{
    // you can chunk up a calculation into multiple parts by passing in the previous crc value:
    uint8_t data_chunk1[] = {0x12, 0x34, 0x56};
    uint8_t data_chunk2[] = {0x78, 0x9A, 0xBC, 0xDE};
    {
        auto crc16 = CRC16::CCITT_FALSE::calc(data_chunk1, sizeof(data_chunk1));
        crc16      = CRC16::CCITT_FALSE::calc(data_chunk2, sizeof(data_chunk2), crc16);
        printf("CRC16 = 0x%08X - when split into multiple calculations\n", crc16);
    }

    {
        uint8_t all_data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE};
        auto crc16 = CRC16::CCITT_FALSE::calc(all_data, sizeof(all_data));
        printf("CRC16 = 0x%08X - when calculated in one pass (same as the split version)\n", crc16);
    }

    // WARNING: The "calc" function's third argument is "prior_crc_value", which assumes
    // you're passing in a valid CRC value to continue from, this first CRC is NOT
    // always equivalent to the crc algorithm's "init" parameter. So never pass in "init"!
    // Instead, prefer "null_crc", or "calc()" if you're manually initializing a chunked up
    // calculation, like so:
    {
        uint32_t crc32 = CRC32::CRC32::null_crc; // or use "= CRC32::CRC32::calc();"
        crc32 = CRC32::CRC32::calc(data_chunk1, sizeof(data_chunk1), crc32);
        crc32 = CRC32::CRC32::calc(data_chunk2, sizeof(data_chunk2), crc32);
        printf("CRC32 = 0x%08X\n", crc32);
    }
}