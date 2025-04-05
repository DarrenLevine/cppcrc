#include "../cppcrc.h"
#include <stdio.h>

int main()
{
    // you can chunk up a calculation into multiple parts like so for most algorithms:
    uint8_t data_chunk1[] = {0x12, 0x34, 0x56};
    uint8_t data_chunk2[] = {0x78, 0x9A, 0xBC, 0xDE};
    {
        auto crc16 = CRC16::CCITT_FALSE::calc(data_chunk1, sizeof(data_chunk1));
        crc16      = CRC16::CCITT_FALSE::calc(data_chunk2, sizeof(data_chunk2), crc16);
    }

    // However! Some algorithms will throw a compile-time error at you if you try to do this
    // when it is not safe. Specifically when the crc value can't be passed back into the calc
    // function due to the output being X-ORd and/or bit reflected.
    // For example, this code would fail to compile do to its algorithm having an X-OR:
    //
    //      auto crc32_failing = CRC32::CRC32::calc(data_chunk1, sizeof(data_chunk1));
    //      crc32_failing      = CRC32::CRC32::calc(data_chunk2, sizeof(data_chunk2), crc32_failing);
    //
    // To remove the error, there are two options:
    {
        // 1) If you want to let CppCRC automatically correct for that detail, just use "calc_chunk":
        auto crc32 = CRC32::CRC32::calc_chunk(data_chunk1, sizeof(data_chunk1));
        crc32      = CRC32::CRC32::calc_chunk(data_chunk2, sizeof(data_chunk2), crc32);

        // Or, 2) if you want to stick to using the exact CRC algorithm as found in literature, then
        // you can use the "calc_exact" function.
        auto crc32_manual = CRC32::CRC32::calc_exact(data_chunk1, sizeof(data_chunk1));

        // Just remember to undo any X-OR or reflections before, like this:
        auto recovered_intermediate_crc = crc32_manual;
        if (CRC32::CRC32::x_or_out)
            recovered_intermediate_crc ^= CRC32::CRC32::x_or_out;
        if (CRC32::CRC32::refl_out)
            recovered_intermediate_crc = crc_utils::reverse(recovered_intermediate_crc);
        crc32_manual = CRC32::CRC32::calc_exact(data_chunk2, sizeof(data_chunk2), recovered_intermediate_crc);
        printf("%s\n", crc32 == crc32_manual ? "CRCs matched!" : "Failed to match...");
    }

    // NOTE: you can use "calc_chunk" or "calc_exact" on any algorithm, so feel free to just
    // use either as the default instead of "calc" if you know what you're doing
    // If you're not sure, just use "calc" and the library will throw a compile-time error
    // if you mess up.
    {
        auto crc16 = CRC16::CCITT_FALSE::calc_chunk(data_chunk1, sizeof(data_chunk1));
        crc16      = CRC16::CCITT_FALSE::calc_chunk(data_chunk2, sizeof(data_chunk2), crc16);
    }

    // One word of warning, the calc_chunk function assumes you're passing it a valid CRC value,
    // to continue from. So if you want to initialize your CRC to an empty starting init value, you
    // will have to be consistent and not mix "calc_chunk" and "calc_exact" when setting the initial value:
    {
        auto crc32 = CRC32::CRC32::calc_chunk(); // or CRC32::CRC32::calc_chunk_init
        crc32      = CRC32::CRC32::calc_chunk(data_chunk1, sizeof(data_chunk1), crc32);
        crc32      = CRC32::CRC32::calc_chunk(data_chunk2, sizeof(data_chunk2), crc32);
    }
}