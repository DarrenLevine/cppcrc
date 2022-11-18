#include "../cppcrc.h"
#include <stdio.h>

constexpr uint8_t test_sequence[] = {
    0x09, 0x03, 0x5E, 0x57, 0x3F, 0xA7, 0x11, 0xF1, 0xED, 0xF4, 0xEA, 0xE1, 0x33, 0x96, 0x6A, 0x00,
    0x22, 0xE1, 0x93, 0x83, 0xC2, 0x56, 0x07, 0x48, 0x47, 0x80, 0xCD, 0x48, 0x19, 0x38, 0xA2, 0x4C,
    0x4E, 0x81, 0x05, 0xF9, 0xE8, 0x02, 0x0E, 0x5F, 0x16, 0x2D, 0x28, 0x11, 0x45, 0x0E, 0x1C, 0x4F,
    0x84, 0x63, 0x84, 0x1E, 0x2F, 0x28, 0x6E, 0x26, 0x12, 0xBC, 0xC3, 0x82, 0x11, 0x70, 0x18, 0x41};

static_assert(CRC8::CRC8::calc(test_sequence, sizeof(test_sequence)) == 0x45, "validate");
static_assert(CRC8::CDMA2000::calc(test_sequence, sizeof(test_sequence)) == 0x92, "validate");
static_assert(CRC8::DARC::calc(test_sequence, sizeof(test_sequence)) == 0x19, "validate");
static_assert(CRC8::DVB_S2::calc(test_sequence, sizeof(test_sequence)) == 0xA7, "validate");
static_assert(CRC8::EBU::calc(test_sequence, sizeof(test_sequence)) == 0x7C, "validate");
static_assert(CRC8::I_CODE::calc(test_sequence, sizeof(test_sequence)) == 0x07, "validate");
static_assert(CRC8::ITU::calc(test_sequence, sizeof(test_sequence)) == 0x10, "validate");
static_assert(CRC8::MAXIM::calc(test_sequence, sizeof(test_sequence)) == 0xE6, "validate");
static_assert(CRC8::ROHC::calc(test_sequence, sizeof(test_sequence)) == 0xB6, "validate");
static_assert(CRC8::WCDMA::calc(test_sequence, sizeof(test_sequence)) == 0x6E, "validate");
static_assert(CRC16::ARC::calc(test_sequence, sizeof(test_sequence)) == 0xCC8C, "validate");
static_assert(CRC16::AUG_CCITT::calc(test_sequence, sizeof(test_sequence)) == 0x3AA1, "validate");
static_assert(CRC16::BUYPASS::calc(test_sequence, sizeof(test_sequence)) == 0x98B1, "validate");
static_assert(CRC16::CCITT_FALSE::calc(test_sequence, sizeof(test_sequence)) == 0x39CD, "validate");
static_assert(CRC16::CDMA2000::calc(test_sequence, sizeof(test_sequence)) == 0x935B, "validate");
static_assert(CRC16::DDS_110::calc(test_sequence, sizeof(test_sequence)) == 0x9689, "validate");
static_assert(CRC16::DECT_R::calc(test_sequence, sizeof(test_sequence)) == 0x509C, "validate");
static_assert(CRC16::DECT_X::calc(test_sequence, sizeof(test_sequence)) == 0x509D, "validate");
static_assert(CRC16::DNP::calc(test_sequence, sizeof(test_sequence)) == 0xB832, "validate");
static_assert(CRC16::EN_13757::calc(test_sequence, sizeof(test_sequence)) == 0x7B5D, "validate");
static_assert(CRC16::GENIBUS::calc(test_sequence, sizeof(test_sequence)) == 0xC632, "validate");
static_assert(CRC16::KERMIT::calc(test_sequence, sizeof(test_sequence)) == 0xDFD2, "validate");
static_assert(CRC16::MAXIM::calc(test_sequence, sizeof(test_sequence)) == 0x3373, "validate");
static_assert(CRC16::MCRF4XX::calc(test_sequence, sizeof(test_sequence)) == 0x84B9, "validate");
static_assert(CRC16::MODBUS::calc(test_sequence, sizeof(test_sequence)) == 0xE3CC, "validate");
static_assert(CRC16::RIELLO::calc(test_sequence, sizeof(test_sequence)) == 0xCA61, "validate");
static_assert(CRC16::T10_DIF::calc(test_sequence, sizeof(test_sequence)) == 0x9A7B, "validate");
static_assert(CRC16::TELEDISK::calc(test_sequence, sizeof(test_sequence)) == 0x3C4F, "validate");
static_assert(CRC16::TMS37157::calc(test_sequence, sizeof(test_sequence)) == 0x9337, "validate");
static_assert(CRC16::USB::calc(test_sequence, sizeof(test_sequence)) == 0x1C33, "validate");
static_assert(CRC16::X_25::calc(test_sequence, sizeof(test_sequence)) == 0x7B46, "validate");
static_assert(CRC16::XMODEM::calc(test_sequence, sizeof(test_sequence)) == 0xEF17, "validate");
static_assert(CRC16::A::calc(test_sequence, sizeof(test_sequence)) == 0xB10E, "validate");
static_assert(CRC32::CRC32::calc(test_sequence, sizeof(test_sequence)) == 0x6B7D6115, "validate");
static_assert(CRC32::BZIP2::calc(test_sequence, sizeof(test_sequence)) == 0xAB94EA2E, "validate");
static_assert(CRC32::JAMCRC::calc(test_sequence, sizeof(test_sequence)) == 0x94829EEA, "validate");
static_assert(CRC32::MPEG_2::calc(test_sequence, sizeof(test_sequence)) == 0x546B15D1, "validate");
static_assert(CRC32::POSIX::calc(test_sequence, sizeof(test_sequence)) == 0x38ADA47F, "validate");
static_assert(CRC32::SATA::calc(test_sequence, sizeof(test_sequence)) == 0xB4B8412F, "validate");
static_assert(CRC32::XFER::calc(test_sequence, sizeof(test_sequence)) == 0xA6CAEBDE, "validate");
static_assert(CRC32::C::calc(test_sequence, sizeof(test_sequence)) == 0xB64A90F7, "validate");
static_assert(CRC32::D::calc(test_sequence, sizeof(test_sequence)) == 0xCFACF452, "validate");
static_assert(CRC32::Q::calc(test_sequence, sizeof(test_sequence)) == 0x7FDF7CA6, "validate");

int main()
{
    constexpr uint8_t data[] = {0x12, 0x5A, 0x23, 0x19, 0x92, 0xF3, 0xDE, 0xC2, 0x5A, 0x1F, 0x91, 0xA3};
    constexpr uint8_t crc_val = CRC8::CRC8::calc(data, 12);
    static_assert(crc_val == 0x71);

    auto &crc_table = CRC8::CRC8::table();
    static_assert(crc_table[0] == 0x00);
    static_assert(crc_table[1] == 0x07);

    printf("PASSED! (all the tests run at compile time)\n");
}