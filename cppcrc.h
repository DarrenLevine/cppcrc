/// @file cppcrc.h
/// @author Darren V Levine (DarrenVLevine@gmail.com)
/// @brief A very small, fast, header-only, C++ library for generating CRCs
/// @version 1.0
/// @date 2022-11-17
///
/// @copyright (c) 2022 Darren V Levine. This code is licensed under MIT license (see LICENSE file for details).

#ifndef CPPCRC_H_
#define CPPCRC_H_

#include <stdint.h>
#include <stddef.h>
#include <utility>

//
// Backend implimentation:
//

namespace crc_utils
{
    inline constexpr uint8_t reverse(uint8_t x)
    {
        constexpr uint8_t lookup[16] = {0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe, 0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf};
        return (lookup[x & 0x0F] << 4) | lookup[x >> 4];
    }
    inline constexpr uint16_t reverse(uint16_t x)
    {
        return uint16_t(reverse(uint8_t(x))) << 8 | uint16_t(reverse(uint8_t(x >> 8)));
    }
    inline constexpr uint32_t reverse(uint32_t x)
    {
        return uint32_t(reverse(uint16_t(x))) << 16 | uint32_t(reverse(uint16_t(x >> 16)));
    }
    inline constexpr uint64_t reverse(uint64_t x)
    {
        return uint64_t(reverse(uint32_t(x))) << 32 | uint64_t(reverse(uint32_t(x >> 32)));
    }

    template <typename out_t, out_t poly, bool refl_in, bool refl_out, size_t index>
    constexpr out_t get_crc_table_value_at_index()
    {
        constexpr size_t bit_width = sizeof(out_t) * 8;
        out_t remainder = refl_in ? reverse(static_cast<out_t>(index)) >> (bit_width - 8u) : static_cast<out_t>(index);
        constexpr out_t mask = static_cast<out_t>(1) << (bit_width - 1u);
        for (size_t i = 0; i < bit_width; i++)
        {
            if (remainder & mask)
                remainder = (remainder << 1) ^ poly;
            else
                remainder <<= 1;
        }
        return refl_out ? reverse(remainder) : remainder;
    }

    template <typename out_t, out_t poly, bool refl_in, bool refl_out, size_t size = 256, typename = std::make_index_sequence<size>>
    struct crc_lookup_table;

    template <typename out_t, out_t poly, bool refl_in, bool refl_out, size_t size, size_t... indexes>
    struct crc_lookup_table<out_t, poly, refl_in, refl_out, size, std::index_sequence<indexes...>>
    {
        static constexpr out_t value[size] = {get_crc_table_value_at_index<out_t, poly, refl_in, refl_out, indexes>()...};
    };

    template <typename out_t, out_t poly, bool refl_in, bool refl_out, size_t size, size_t... indexes>
    constexpr out_t crc_lookup_table<out_t, poly, refl_in, refl_out, size, std::index_sequence<indexes...>>::value[size];

    template <typename out_t, out_t poly, out_t init, bool refl_in, bool refl_out, out_t x_or_out, typename std::enable_if<refl_in, int *>::type = nullptr>
    constexpr out_t calculate_crc(const uint8_t *bytes, size_t n, out_t crc = init)
    {
        if (n == 0u) // check for the n==0 case up front, so that we can use --n instead of n--, gaining a slight speedup
            return crc;
        constexpr auto &lookup = crc_lookup_table<out_t, poly, refl_in, refl_out>().value;
        crc = reverse(crc);
        do
            crc = lookup[static_cast<uint8_t>(*bytes++ ^ crc)] ^ (crc >> 8);
        while (--n);
        return crc ^ x_or_out;
    }
    template <typename out_t, out_t poly, out_t init, bool refl_in, bool refl_out, out_t x_or_out, typename std::enable_if<!refl_in, int *>::type = nullptr>
    constexpr out_t calculate_crc(const uint8_t *bytes, size_t n, out_t crc = init)
    {
        if (n == 0u) // check for the n==0 case up front, so that we can use --n instead of n--, gaining a slight speedup
            return crc;
        constexpr auto &lookup = crc_lookup_table<out_t, poly, refl_in, refl_out>().value;
        constexpr size_t bit_width_minus_8 = sizeof(out_t) * 8 - 8U;
        do
            crc = lookup[static_cast<uint8_t>(*bytes++ ^ (crc >> bit_width_minus_8))] ^ (crc << 8);
        while (--n);
        return crc ^ x_or_out;
    }
    template <typename out_t, out_t poly, out_t init, bool refl_in, bool refl_out, out_t x_or_out>
    struct crc
    {
        static constexpr out_t calc(const uint8_t *bytes, size_t n, out_t crc = init)
        {
            return calculate_crc<out_t, poly, init, refl_in, refl_out, x_or_out>(bytes, n, crc);
        }
        static constexpr auto &table()
        {
            return crc_lookup_table<out_t, poly, refl_in, refl_out>().value;
        }
    };
} // namespace crc_utils

//
// Default CRC Configurations:
//

namespace CRC8
{
    using CRC8 = crc_utils::crc<uint8_t, 0x07, 0x00, false, false, 0x00>;
    using CDMA2000 = crc_utils::crc<uint8_t, 0x9B, 0xFF, false, false, 0x00>;
    using DARC = crc_utils::crc<uint8_t, 0x39, 0x00, true, true, 0x00>;
    using DVB_S2 = crc_utils::crc<uint8_t, 0xD5, 0x00, false, false, 0x00>;
    using EBU = crc_utils::crc<uint8_t, 0x1D, 0xFF, true, true, 0x00>;
    using I_CODE = crc_utils::crc<uint8_t, 0x1D, 0xFD, false, false, 0x00>;
    using ITU = crc_utils::crc<uint8_t, 0x07, 0x00, false, false, 0x55>;
    using MAXIM = crc_utils::crc<uint8_t, 0x31, 0x00, true, true, 0x00>;
    using ROHC = crc_utils::crc<uint8_t, 0x07, 0xFF, true, true, 0x00>;
    using WCDMA = crc_utils::crc<uint8_t, 0x9B, 0x00, true, true, 0x00>;
}
namespace CRC16
{
    using ARC = crc_utils::crc<uint16_t, 0x8005, 0x0000, true, true, 0x0000>;
    using AUG_CCITT = crc_utils::crc<uint16_t, 0x1021, 0x1D0F, false, false, 0x0000>;
    using BUYPASS = crc_utils::crc<uint16_t, 0x8005, 0x0000, false, false, 0x0000>;
    using CCITT_FALSE = crc_utils::crc<uint16_t, 0x1021, 0xFFFF, false, false, 0x0000>;
    using CDMA2000 = crc_utils::crc<uint16_t, 0xC867, 0xFFFF, false, false, 0x0000>;
    using DDS_110 = crc_utils::crc<uint16_t, 0x8005, 0x800D, false, false, 0x0000>;
    using DECT_R = crc_utils::crc<uint16_t, 0x0589, 0x0000, false, false, 0x0001>;
    using DECT_X = crc_utils::crc<uint16_t, 0x0589, 0x0000, false, false, 0x0000>;
    using DNP = crc_utils::crc<uint16_t, 0x3D65, 0x0000, true, true, 0xFFFF>;
    using EN_13757 = crc_utils::crc<uint16_t, 0x3D65, 0x0000, false, false, 0xFFFF>;
    using GENIBUS = crc_utils::crc<uint16_t, 0x1021, 0xFFFF, false, false, 0xFFFF>;
    using KERMIT = crc_utils::crc<uint16_t, 0x1021, 0x0000, true, true, 0x0000>;
    using MAXIM = crc_utils::crc<uint16_t, 0x8005, 0x0000, true, true, 0xFFFF>;
    using MCRF4XX = crc_utils::crc<uint16_t, 0x1021, 0xFFFF, true, true, 0x0000>;
    using MODBUS = crc_utils::crc<uint16_t, 0x8005, 0xFFFF, true, true, 0x0000>;
    using RIELLO = crc_utils::crc<uint16_t, 0x1021, 0xB2AA, true, true, 0x0000>;
    using T10_DIF = crc_utils::crc<uint16_t, 0x8BB7, 0x0000, false, false, 0x0000>;
    using TELEDISK = crc_utils::crc<uint16_t, 0xA097, 0x0000, false, false, 0x0000>;
    using TMS37157 = crc_utils::crc<uint16_t, 0x1021, 0x89EC, true, true, 0x0000>;
    using USB = crc_utils::crc<uint16_t, 0x8005, 0xFFFF, true, true, 0xFFFF>;
    using X_25 = crc_utils::crc<uint16_t, 0x1021, 0xFFFF, true, true, 0xFFFF>;
    using XMODEM = crc_utils::crc<uint16_t, 0x1021, 0x0000, false, false, 0x0000>;
    using A = crc_utils::crc<uint16_t, 0x1021, 0xC6C6, true, true, 0x0000>;
}
namespace CRC32
{
    using CRC32 = crc_utils::crc<uint32_t, 0x04C11DB7, 0xFFFFFFFF, true, true, 0xFFFFFFFF>;
    using BZIP2 = crc_utils::crc<uint32_t, 0x04C11DB7, 0xFFFFFFFF, false, false, 0xFFFFFFFF>;
    using JAMCRC = crc_utils::crc<uint32_t, 0x04C11DB7, 0xFFFFFFFF, true, true, 0x00000000>;
    using MPEG_2 = crc_utils::crc<uint32_t, 0x04C11DB7, 0xFFFFFFFF, false, false, 0x00000000>;
    using POSIX = crc_utils::crc<uint32_t, 0x04C11DB7, 0x00000000, false, false, 0xFFFFFFFF>;
    using SATA = crc_utils::crc<uint32_t, 0x04C11DB7, 0x52325032, false, false, 0x00000000>;
    using XFER = crc_utils::crc<uint32_t, 0x000000AF, 0x00000000, false, false, 0x00000000>;
    using C = crc_utils::crc<uint32_t, 0x1EDC6F41, 0xFFFFFFFF, true, true, 0xFFFFFFFF>;
    using D = crc_utils::crc<uint32_t, 0xA833982B, 0xFFFFFFFF, true, true, 0xFFFFFFFF>;
    using Q = crc_utils::crc<uint32_t, 0x814141AB, 0x00000000, false, false, 0x00000000>;
}
namespace CRC64
{
    using ECMA = crc_utils::crc<uint64_t, 0x42F0E1EBA9EA3693, 0x0000000000000000, false, false, 0x0000000000000000>;
    using GO_ISO = crc_utils::crc<uint64_t, 0x000000000000001B, 0xFFFFFFFFFFFFFFFF, true, true, 0xFFFFFFFFFFFFFFFF>;
    using WE = crc_utils::crc<uint64_t, 0x42F0E1EBA9EA3693, 0xFFFFFFFFFFFFFFFF, false, false, 0xFFFFFFFFFFFFFFFF>;
    using XY = crc_utils::crc<uint64_t, 0x42F0E1EBA9EA3693, 0xFFFFFFFFFFFFFFFF, true, true, 0xFFFFFFFFFFFFFFFF>;
}

#endif // CPPCRC_H_
