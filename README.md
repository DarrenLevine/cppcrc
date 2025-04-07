# CPP/CRC

A very small, fast, header-only, C++ library for generating CRCs

![license](https://img.shields.io/badge/license-MIT-informational) ![version](https://img.shields.io/badge/version-1.3-blue)

## Requirements

* C++14 or greater

## Features

* Can be used to generate arbitrary/custom CRC calculators, and CRC lookup tables **at compile time**
* Header-only, single file, very small backend implementation (<100 lines)
* Very fast [check out the benchmarks](#benchmarks)
* Each CRC function has the signature:

```cpp
constexpr out_t crc_type::crc_name::calc(
    const uint8_t *bytes, size_t num_bytes, out_t prior_crc_value = null_crc)
```

* Support for the following algorithms are built-in:
  * CRC8::CRC8::calc
  * CRC8::CDMA2000::calc
  * CRC8::DARC::calc
  * CRC8::DVB_S2::calc
  * CRC8::EBU::calc
  * CRC8::I_CODE::calc
  * CRC8::ITU::calc
  * CRC8::MAXIM::calc
  * CRC8::ROHC::calc
  * CRC8::WCDMA::calc
  * CRC16::ARC::calc
  * CRC16::AUG_CCITT::calc
  * CRC16::BUYPASS::calc
  * CRC16::CCITT_FALSE::calc
  * CRC16::CDMA2000::calc
  * CRC16::DDS_110::calc
  * CRC16::DECT_R::calc
  * CRC16::DECT_X::calc
  * CRC16::DNP::calc
  * CRC16::EN_13757::calc
  * CRC16::GENIBUS::calc
  * CRC16::KERMIT::calc
  * CRC16::MAXIM::calc
  * CRC16::MCRF4XX::calc
  * CRC16::MODBUS::calc
  * CRC16::RIELLO::calc
  * CRC16::T10_DIF::calc
  * CRC16::TELEDISK::calc
  * CRC16::TMS37157::calc
  * CRC16::USB::calc
  * CRC16::X_25::calc
  * CRC16::XMODEM::calc
  * CRC16::A::calc
  * CRC32::CRC32::calc
  * CRC32::BZIP2::calc
  * CRC32::JAMCRC::calc
  * CRC32::MPEG_2::calc
  * CRC32::POSIX::calc
  * CRC32::SATA::calc
  * CRC32::XFER::calc
  * CRC32::C::calc
  * CRC32::D::calc
  * CRC32::Q::calc
  * CRC64::ECMA::calc
  * CRC64::GO_ISO::calc
  * CRC64::WE::calc
  * CRC64::XY::calc

## Usage Examples

### Using the built-in CRC functions

```cpp
// 1) include "cppcrc.h"
#include "cppcrc.h"
#include <stdio.h>

// 2) use the CRC8/CRC16/CRC32 functions, for example:
int main()
{
    uint8_t data[] = {0x12, 0x5A, 0x23, 0x19, 0x92, 0xF3, 0xDE, 0xC2, 0x5A, 0x1F, 0x91, 0xA3};

    // calculate a checksum:
    uint16_t crc_value = CRC16::CCITT_FALSE::calc(data, sizeof(data));
    printf("crc_value = 0x%04X\n", crc_value);

    // view the lookup table:
    auto &crc_table = CRC16::CCITT_FALSE::table();
    printf("crc_table = {0x%04X", crc_table[0]);
    for (int i = 1; i < 256; i++)
        printf(", 0x%04X", crc_table[i]);
    printf("}\n");
}
```

### Creating Your Own CRC Function

```cpp
// 1) include "cppcrc.h"
#include "cppcrc.h"
#include <stdio.h>

int main()
{
    // 2) pick some CRC settings (these are just an example)
    using base_type = uint8_t;
    constexpr base_type poly = 0x12;
    constexpr base_type init_value = 0x34;
    constexpr bool reflect_in = true;
    constexpr bool reflect_out = true;
    constexpr base_type Xor_out = 0xFF;

    // 3) create a new CRC type by filling in the CRC parameters
    using your_crc_name = crc_utils::crc<base_type, poly, init_value, reflect_in, reflect_out, Xor_out>;

    // you can use your new CRC type to calculate a checksum
    uint8_t data[] = {0x12, 0x34, 0x56};
    auto calculated_crc = your_crc_name::calc(data, sizeof(data));
    printf("calculated_crc = 0x%02X\n", calculated_crc);

    // and even continue an existing calculation
    uint8_t more_data[] = {0x78, 0x9A};
    calculated_crc = your_crc_name::calc(more_data, sizeof(more_data),  calculated_crc);
    printf("calculated_crc = 0x%02X\n", calculated_crc);

    // and access the underlying lookup table for your CRC function
    auto &crc_table = your_crc_name::table();
}
```

## Benchmarks

### Using data generated at **run-time** with various array sizes and checksum algorithms

|             ns/64kB |              64kB/s |    err% |        ins/64kB |       bra/64kB |   miss% |     total | Checkum Calculators
|--------------------:|--------------------:|--------:|----------------:|---------------:|--------:|----------:|:--------------------
|          118,718.50 |            8,423.29 |    0.5% |      527,620.00 |      65,943.00 |    0.0% |      1.20 | `DarrenLevine/cppcrc`
|          118,729.00 |            8,422.54 |    0.5% |      577,143.00 |      65,959.00 |    0.0% |      1.20 | `boostorg/crc`
|          122,306.00 |            8,176.21 |    0.5% |      560,697.00 |      65,987.00 |    0.0% |      1.23 | `d-bahr/CRCpp [w/ static table]`
|          151,351.00 |            6,607.16 |    0.5% |      890,311.00 |      65,943.00 |    0.0% |      1.53 | `pstolarz/CRaC`
|          380,683.00 |            2,626.86 |    1.2% |    3,083,889.00 |     115,556.00 |    5.3% |      3.83 | `d-bahr/CRCpp`

### Using data generated at **compile-time** with various array sizes and checksum algorithms

|             ns/64kB |              64kB/s |    err% |        ins/64kB |       bra/64kB |   miss% |     total | Checkum Calculators
|--------------------:|--------------------:|--------:|----------------:|---------------:|--------:|----------:|:--------------------
|                0.45 |    2,229,858,535.96 |    0.5% |            1.00 |           0.00 |    0.0% |      0.22 | `DarrenLevine/cppcrc [w/ constexpr data]`
|                0.45 |    2,219,406,249.99 |    0.5% |            1.00 |           0.00 |    0.0% |      0.22 | `pstolarz/CRaC [w/ constexpr data]`
|              995.09 |        1,004,933.31 |    0.5% |        5,805.27 |         643.95 |    0.2% |      0.22 | `boostorg/crc [w/ constexpr data]`
|            1,014.95 |          985,270.21 |    0.4% |        5,716.24 |         667.95 |    0.2% |      0.22 | `d-bahr/CRCpp [w/ constexpr data]`

Benchmarked using nanobench on a AMD Ryzen 9 5900HS CPU. See benchmark/main.cpp for exact details.

How to run the benchmarks (it's recommended you install boost first):

```sh
git clone https://github.com/DarrenLevine/cppcrc.git
cd cppcrc/benchmark && make
```
