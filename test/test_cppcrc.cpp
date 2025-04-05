#include "../cppcrc.h"
#include <stdio.h>

// some helpful literals
inline constexpr uint8_t operator""_u8(unsigned long long val) noexcept { return val; }
inline constexpr uint16_t operator""_u16(unsigned long long val) noexcept { return val; }
inline constexpr uint32_t operator""_u32(unsigned long long val) noexcept { return val; }
inline constexpr uint64_t operator""_u64(unsigned long long val) noexcept { return val; }

constexpr uint8_t test_sequence[] = {
    0x09, 0x03, 0x5E, 0x57, 0x3F, 0xA7, 0x11, 0xF1, 0xED, 0xF4, 0xEA, 0xE1, 0x33, 0x96, 0x6A, 0x00,
    0x22, 0xE1, 0x93, 0x83, 0xC2, 0x56, 0x07, 0x48, 0x47, 0x80, 0xCD, 0x48, 0x19, 0x38, 0xA2, 0x4C,
    0x4E, 0x81, 0x05, 0xF9, 0xE8, 0x02, 0x0E, 0x5F, 0x16, 0x2D, 0x28, 0x11, 0x45, 0x0E, 0x1C, 0x4F,
    0x84, 0x63, 0x84, 0x1E, 0x2F, 0x28, 0x6E, 0x26, 0x12, 0xBC, 0xC3, 0x82, 0x11, 0x70, 0x18, 0x41};

// tests both a single crc calc and a chunked up calculation
#define TEST_CRC(name, expected)                                        \
    static_assert(name::calc_exact(                                     \
                      test_sequence,                                    \
                      sizeof(test_sequence)) == expected,               \
                  "validate one pass data sequence");                   \
    static_assert(name::calc_chunk(                                     \
                      test_sequence + 10,                               \
                      sizeof(test_sequence) - 10,                       \
                      name::calc_chunk(test_sequence, 10)) == expected, \
                  "validate chunked up data")

TEST_CRC(CRC8::CRC8, 0x45_u8);
TEST_CRC(CRC8::CDMA2000, 0x92_u8);
TEST_CRC(CRC8::DARC, 0x19_u8);
TEST_CRC(CRC8::DVB_S2, 0xA7_u8);
TEST_CRC(CRC8::EBU, 0x7C_u8);
TEST_CRC(CRC8::I_CODE, 0x07_u8);
TEST_CRC(CRC8::ITU, 0x10_u8);
TEST_CRC(CRC8::MAXIM, 0xE6_u8);
TEST_CRC(CRC8::ROHC, 0xB6_u8);
TEST_CRC(CRC8::WCDMA, 0x6E_u8);
TEST_CRC(CRC16::ARC, 0xCC8C_u16);
TEST_CRC(CRC16::AUG_CCITT, 0x3AA1_u16);
TEST_CRC(CRC16::BUYPASS, 0x98B1_u16);
TEST_CRC(CRC16::CCITT_FALSE, 0x39CD_u16);
TEST_CRC(CRC16::CDMA2000, 0x935B_u16);
TEST_CRC(CRC16::DDS_110, 0x9689_u16);
TEST_CRC(CRC16::DECT_R, 0x509C_u16);
TEST_CRC(CRC16::DECT_X, 0x509D_u16);
TEST_CRC(CRC16::DNP, 0xB832_u16);
TEST_CRC(CRC16::EN_13757, 0x7B5D_u16);
TEST_CRC(CRC16::GENIBUS, 0xC632_u16);
TEST_CRC(CRC16::KERMIT, 0xDFD2_u16);
TEST_CRC(CRC16::MAXIM, 0x3373_u16);
TEST_CRC(CRC16::MCRF4XX, 0x84B9_u16);
TEST_CRC(CRC16::MODBUS, 0xE3CC_u16);
TEST_CRC(CRC16::RIELLO, 0xCA61_u16);
TEST_CRC(CRC16::T10_DIF, 0x9A7B_u16);
TEST_CRC(CRC16::TELEDISK, 0x3C4F_u16);
TEST_CRC(CRC16::TMS37157, 0x9337_u16);
TEST_CRC(CRC16::USB, 0x1C33_u16);
TEST_CRC(CRC16::X_25, 0x7B46_u16);
TEST_CRC(CRC16::XMODEM, 0xEF17_u16);
TEST_CRC(CRC16::A, 0xB10E_u16);
TEST_CRC(CRC32::CRC32, 0x6B7D6115_u32);
TEST_CRC(CRC32::BZIP2, 0xAB94EA2E_u32);
TEST_CRC(CRC32::JAMCRC, 0x94829EEA_u32);
TEST_CRC(CRC32::MPEG_2, 0x546B15D1_u32);
TEST_CRC(CRC32::POSIX, 0x38ADA47F_u32);
TEST_CRC(CRC32::SATA, 0xB4B8412F_u32);
TEST_CRC(CRC32::XFER, 0xA6CAEBDE_u32);
TEST_CRC(CRC32::C, 0xB64A90F7_u32);
TEST_CRC(CRC32::D, 0xCFACF452_u32);
TEST_CRC(CRC32::Q, 0x7FDF7CA6_u32);
TEST_CRC(CRC64::ECMA, 0x0D408B7A4211A473_u64);
TEST_CRC(CRC64::GO_ISO, 0xAA139955A8E60504_u64);
TEST_CRC(CRC64::WE, 0x4D648712E74F8E08_u64);
TEST_CRC(CRC64::XY, 0x13CEC7423537E08D_u64);

// testing that a non-matching reflection in/out values still work
static_assert(crc_utils::crc<uint32_t, 0x4C11DB7, 0xAB111FF, true, false, 0xFFFFFFFF>::calc(nullptr, 0) == 0xF54EEE00_u32, "validate");
static_assert(crc_utils::crc<uint32_t, 0x4C11DB7, 0xAB111FF, true, false, 0xFFFFFFFF>::calc(test_sequence, sizeof(test_sequence)) == 0xEB8C5295_u32, "validate");
static_assert(crc_utils::crc<uint32_t, 0x4C11DB7, 0xAB111FF, false, true, 0xFFFFFFFF>::calc(nullptr, 0) == 0x007772AF_u32, "validate");
static_assert(crc_utils::crc<uint32_t, 0x4C11DB7, 0xAB111FF, false, true, 0xFFFFFFFF>::calc(test_sequence, sizeof(test_sequence)) == 0xB6607917_u32, "validate");

// testing that null buffers returns bit reversed init value
static_assert(crc_utils::crc<uint8_t, 0x31, 0xbf, true, true, 0x00>::calc(nullptr, 0) == 0xfd_u8, "validate");

static_assert(CRC8::CRC8::calc(nullptr, 0) == 0x00_u8, "validate");
static_assert(CRC8::CDMA2000::calc(nullptr, 0) == 0xFF_u8, "validate");
static_assert(CRC8::DARC::calc(nullptr, 0) == 0x00_u8, "validate");
static_assert(CRC8::DVB_S2::calc(nullptr, 0) == 0x00_u8, "validate");
static_assert(CRC8::EBU::calc(nullptr, 0) == 0xFF_u8, "validate");
static_assert(CRC8::I_CODE::calc(nullptr, 0) == 0xFD_u8, "validate");
static_assert(CRC8::ITU::calc(nullptr, 0) == 0x55_u8, "validate");
static_assert(CRC8::MAXIM::calc(nullptr, 0) == 0x00_u8, "validate");
static_assert(CRC8::ROHC::calc(nullptr, 0) == 0xFF_u8, "validate");
static_assert(CRC8::WCDMA::calc(nullptr, 0) == 0x00_u8, "validate");
static_assert(CRC16::ARC::calc(nullptr, 0) == 0x0000_u16, "validate");
static_assert(CRC16::AUG_CCITT::calc(nullptr, 0) == 0x1D0F_u16, "validate");
static_assert(CRC16::BUYPASS::calc(nullptr, 0) == 0x0000_u16, "validate");
static_assert(CRC16::CCITT_FALSE::calc(nullptr, 0) == 0xFFFF_u16, "validate");
static_assert(CRC16::CDMA2000::calc(nullptr, 0) == 0xFFFF_u16, "validate");
static_assert(CRC16::DDS_110::calc(nullptr, 0) == 0x800D_u16, "validate");
static_assert(CRC16::DECT_R::calc(nullptr, 0) == 0x0001_u16, "validate");
static_assert(CRC16::DECT_X::calc(nullptr, 0) == 0x0000_u16, "validate");
static_assert(CRC16::DNP::calc(nullptr, 0) == 0xFFFF_u16, "validate");
static_assert(CRC16::EN_13757::calc(nullptr, 0) == 0xFFFF_u16, "validate");
static_assert(CRC16::GENIBUS::calc(nullptr, 0) == 0x0000_u16, "validate");
static_assert(CRC16::KERMIT::calc(nullptr, 0) == 0x0000_u16, "validate");
static_assert(CRC16::MAXIM::calc(nullptr, 0) == 0xFFFF_u16, "validate");
static_assert(CRC16::MCRF4XX::calc(nullptr, 0) == 0xFFFF_u16, "validate");
static_assert(CRC16::MODBUS::calc(nullptr, 0) == 0xFFFF_u16, "validate");
static_assert(CRC16::RIELLO::calc(nullptr, 0) == 0x554D_u16, "validate");
static_assert(CRC16::T10_DIF::calc(nullptr, 0) == 0x0000_u16, "validate");
static_assert(CRC16::TELEDISK::calc(nullptr, 0) == 0x0000_u16, "validate");
static_assert(CRC16::TMS37157::calc(nullptr, 0) == 0x3791_u16, "validate");
static_assert(CRC16::USB::calc(nullptr, 0) == 0x0000_u16, "validate");
static_assert(CRC16::X_25::calc(nullptr, 0) == 0x0000_u16, "validate");
static_assert(CRC16::XMODEM::calc(nullptr, 0) == 0x0000_u16, "validate");
static_assert(CRC16::A::calc(nullptr, 0) == 0x6363_u16, "validate");
static_assert(CRC32::CRC32::calc(nullptr, 0) == 0x00000000_u32, "validate");
static_assert(CRC32::BZIP2::calc(nullptr, 0) == 0x00000000_u32, "validate");
static_assert(CRC32::JAMCRC::calc(nullptr, 0) == 0xFFFFFFFF_u32, "validate");
static_assert(CRC32::MPEG_2::calc(nullptr, 0) == 0xFFFFFFFF_u32, "validate");
static_assert(CRC32::POSIX::calc(nullptr, 0) == 0xFFFFFFFF_u32, "validate");
static_assert(CRC32::SATA::calc(nullptr, 0) == 0x52325032_u32, "validate");
static_assert(CRC32::XFER::calc(nullptr, 0) == 0x00000000_u32, "validate");
static_assert(CRC32::C::calc(nullptr, 0) == 0x00000000_u32, "validate");
static_assert(CRC32::D::calc(nullptr, 0) == 0x00000000_u32, "validate");
static_assert(CRC32::Q::calc(nullptr, 0) == 0x00000000_u32, "validate");
static_assert(CRC64::ECMA::calc(nullptr, 0) == 0x0000000000000000_u64, "validate");
static_assert(CRC64::GO_ISO::calc(nullptr, 0) == 0x0000000000000000_u64, "validate");
static_assert(CRC64::WE::calc(nullptr, 0) == 0x0000000000000000_u64, "validate");
static_assert(CRC64::XY::calc(nullptr, 0) == 0x0000000000000000_u64, "validate");

constexpr uint8_t test_sequence2[] = {0xff, 0x01};
static_assert(crc_utils::crc<uint8_t, 0x9B, 0x00, false, false, 0x00>::calc(test_sequence2, 2) == 0x2A_u8, "validate");

constexpr uint8_t test_sequence3[] = {0x01};
static_assert(crc_utils::crc<uint8_t, 0x9B, 0xFF, false, false, 0x00>::calc(test_sequence3, 1) == 0xE0_u8, "validate");

static_assert(CRC16::CCITT_FALSE::calc() == CRC16::CCITT_FALSE::init);
static_assert(CRC16::CCITT_FALSE::calc_exact() == CRC16::CCITT_FALSE::init);
static_assert(CRC16::CCITT_FALSE::calc_chunk() == CRC16::CCITT_FALSE::calc_chunk_init);

int main()
{
    constexpr uint8_t data[]  = {0x12, 0x5A, 0x23, 0x19, 0x92, 0xF3, 0xDE, 0xC2, 0x5A, 0x1F, 0x91, 0xA3};
    constexpr uint8_t crc_val = CRC8::CRC8::calc(data, 12);
    static_assert(crc_val == 0x71);

    auto &crc_table = CRC8::CRC8::table();
    static_assert(crc_table[0] == 0x00);
    static_assert(crc_table[1] == 0x07);

    printf("PASSED! (all the tests run at compile time)\n");
}