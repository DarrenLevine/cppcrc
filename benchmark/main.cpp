

#include <nanobench.h>
#include <random>

// github.com/DarrenLevine/cppcrc
#include "cppcrc.h"

// Boost/crc
#ifdef HAS_BOOST
#include <boost/crc.hpp>
using boost_CRC8 = boost::crc_optimal<8, 0x07, 0x00, 0x00, false, false>;
using boost_CRC16_CCITT_FALSE = boost::crc_optimal<16, 0x1021, 0xFFFF, 0, false, false>;
using boost_CRC32 = boost::crc_optimal<32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true>;
using boost_CRC64_ECMA = boost::crc_optimal<64, 0x42F0E1EBA9EA3693, 0x0000000000000000, 0x0000000000000000, false, false>;
template<typename T>
constexpr typename T::value_type boost_calc(const uint8_t* data, size_t size)
{
    T res;
    res.process_bytes(data, size);
    return res.checksum();
}
#endif

// github.com/d-bahr/CRCpp
#define CRCPP_INCLUDE_ESOTERIC_CRC_DEFINITIONS
#include "CRC.h"
static auto CRC_8_table             = CRC::CRC_8().MakeTable();
static auto CRC_16_CCITTFALSE_table = CRC::CRC_16_CCITTFALSE().MakeTable();
static auto CRC_32_table            = CRC::CRC_32().MakeTable();
static auto CRC_64_table            = CRC::CRC_64().MakeTable();

// https://github.com/pstolarz/CRaC
#include "crac.h"

// testing macros (runs through four types of CRCs with four different lengths)
// avoiding any for-loops or additional non-constexpr variables that could add
// time/overhead
#define TEST_CHECKSUM_DATA(data_array, name, func1, func2, func3, func4) \
    {                                                                    \
        b.run(                                                           \
            name,                                                        \
            [&]()                                                        \
            {                                                            \
                {                                                        \
                    constexpr size_t size = sizeof(data_array);          \
                    ankerl::nanobench::doNotOptimizeAway(func1);         \
                    ankerl::nanobench::doNotOptimizeAway(func2);         \
                    ankerl::nanobench::doNotOptimizeAway(func3);         \
                    ankerl::nanobench::doNotOptimizeAway(func4);         \
                }                                                        \
                {                                                        \
                    constexpr size_t size = sizeof(data_array) / 2u;     \
                    ankerl::nanobench::doNotOptimizeAway(func1);         \
                    ankerl::nanobench::doNotOptimizeAway(func2);         \
                    ankerl::nanobench::doNotOptimizeAway(func3);         \
                    ankerl::nanobench::doNotOptimizeAway(func4);         \
                }                                                        \
                {                                                        \
                    constexpr size_t size = sizeof(data_array) / 10u;    \
                    ankerl::nanobench::doNotOptimizeAway(func1);         \
                    ankerl::nanobench::doNotOptimizeAway(func2);         \
                    ankerl::nanobench::doNotOptimizeAway(func3);         \
                    ankerl::nanobench::doNotOptimizeAway(func4);         \
                }                                                        \
                {                                                        \
                    constexpr size_t size = sizeof(data_array) / 100u;   \
                    ankerl::nanobench::doNotOptimizeAway(func1);         \
                    ankerl::nanobench::doNotOptimizeAway(func2);         \
                    ankerl::nanobench::doNotOptimizeAway(func3);         \
                    ankerl::nanobench::doNotOptimizeAway(func4);         \
                }                                                        \
            });                                                          \
    }                                                                    \
    static_assert(1)
#define TEST_CHECKSUM(...) TEST_CHECKSUM_DATA(bytes, __VA_ARGS__)
#define TEST_CHECKSUM_CONSTEXPR(...) TEST_CHECKSUM_DATA(bytes_constexpr, __VA_ARGS__)

int main()
{
    std::random_device dev;
    std::mt19937_64 rng(dev());
    constexpr size_t kilobytes_of_test_data = 10;
    uint8_t bytes[kilobytes_of_test_data * 1024];
    for (size_t i = 0; i < sizeof(bytes); i++)
        bytes[i] = rng();
    constexpr uint8_t bytes_constexpr[] = {
        // size doesn't really matter here, since evaluation will be at compile time
        0x09, 0x03, 0x5E, 0x57, 0x3F, 0xA7, 0x11, 0xF1, 0xED, 0xF4, 0xEA, 0xE1, 0x33, 0x96, 0x6A,
        0x22, 0xE1, 0x93, 0x83, 0xC2, 0x56, 0x07, 0x48, 0x47, 0x80, 0xCD, 0x48, 0x19, 0x38, 0xA2,
        0x4E, 0x81, 0x05, 0xF9, 0xE8, 0x02, 0x0E, 0x5F, 0x16, 0x2D, 0x28, 0x11, 0x45, 0x0E, 0x1C,
        0x09, 0x03, 0x5E, 0x57, 0x3F, 0xA7, 0x11, 0xF1, 0xED, 0xF4, 0xEA, 0xE1, 0x33, 0x96, 0x6A,
        0x22, 0xE1, 0x93, 0x83, 0xC2, 0x56, 0x07, 0x48, 0x47, 0x80, 0xCD, 0x48, 0x19, 0x38, 0xA2,
        0x4E, 0x81, 0x05, 0xF9, 0xE8, 0x02, 0x0E, 0x5F, 0x16, 0x2D, 0x28, 0x11, 0x45, 0x0E, 0x1C,
        0x09, 0x03, 0x5E, 0x57, 0x3F, 0xA7, 0x11, 0xF1, 0xED, 0xF4, 0xEA, 0xE1, 0x33, 0x96, 0x6A};

    ankerl::nanobench::Bench b;
    b.title("Checkum Calculators");
    b.unit("64kB"); // approximately 64k = 4*10kB + 4*10kB/2 + 4*10kB/10 + 4*10kB/100
    b.epochs(10000);

    // run-time data tests:
    TEST_CHECKSUM(
        "DarrenLevine/cppcrc",
        CRC8::CRC8::calc(bytes, size),
        CRC16::CCITT_FALSE::calc(bytes, size),
        CRC32::CRC32::calc(bytes, size),
        CRC64::ECMA::calc(bytes, size));
    #ifdef HAS_BOOST
    TEST_CHECKSUM(
        "boostorg/crc",
        boost_calc<boost_CRC8>(bytes, size),
        boost_calc<boost_CRC16_CCITT_FALSE>(bytes, size),
        boost_calc<boost_CRC32>(bytes, size),
        boost_calc<boost_CRC64_ECMA>(bytes, size));
    #endif
    TEST_CHECKSUM(
        "d-bahr/CRCpp [w/ static table]",
        CRC::Calculate(bytes, size, CRC_8_table),
        CRC::Calculate(bytes, size, CRC_16_CCITTFALSE_table),
        CRC::Calculate(bytes, size, CRC_32_table),
        CRC::Calculate(bytes, size, CRC_64_table));
    TEST_CHECKSUM(
        "pstolarz/CRaC",
        crac::CRC8::calc(bytes, size),
        crac::CRC16_CCITT_FALSE::calc(bytes, size),
        crac::CRC32::calc(bytes, size),
        crac::CRC64_ECMA182::calc(bytes, size));
    TEST_CHECKSUM(
        "d-bahr/CRCpp",
        CRC::Calculate(bytes, size, CRC::CRC_8()),
        CRC::Calculate(bytes, size, CRC::CRC_16_CCITTFALSE()),
        CRC::Calculate(bytes, size, CRC::CRC_32()),
        CRC::Calculate(bytes, size, CRC::CRC_64()));

    // a separator for the compile time tests
    printf(
    "|--------------------:|--------------------:"
    "|--------:|----------------:|---------------:|--------:"
    "|----------:|:--------------------\n");

    // constexpr data tests:
    TEST_CHECKSUM_CONSTEXPR(
        "DarrenLevine/cppcrc [w/ constexpr data]",
        CRC8::CRC8::calc(bytes_constexpr, size),
        CRC16::CCITT_FALSE::calc(bytes_constexpr, size),
        CRC32::CRC32::calc(bytes_constexpr, size),
        CRC64::ECMA::calc(bytes_constexpr, size));
    TEST_CHECKSUM_CONSTEXPR(
        "pstolarz/CRaC [w/ constexpr data]",
        crac::CRC8::calc(bytes_constexpr, size),
        crac::CRC16_CCITT_FALSE::calc(bytes_constexpr, size),
        crac::CRC32::calc(bytes_constexpr, size),
        crac::CRC64_ECMA182::calc(bytes_constexpr, size));
    #ifdef HAS_BOOST
    TEST_CHECKSUM_CONSTEXPR(
        "boostorg/crc [w/ constexpr data]",
        boost_calc<boost_CRC8>(bytes_constexpr, size),
        boost_calc<boost_CRC16_CCITT_FALSE>(bytes_constexpr, size),
        boost_calc<boost_CRC32>(bytes_constexpr, size),
        boost_calc<boost_CRC64_ECMA>(bytes_constexpr, size));
    #endif
    TEST_CHECKSUM_CONSTEXPR(
        "d-bahr/CRCpp [w/ constexpr data]",
        CRC::Calculate(bytes_constexpr, size, CRC_8_table),
        CRC::Calculate(bytes_constexpr, size, CRC_16_CCITTFALSE_table),
        CRC::Calculate(bytes_constexpr, size, CRC_32_table),
        CRC::Calculate(bytes_constexpr, size, CRC_64_table));
}