#ifndef SRC_BLUEZ_HEXDUMP_HPP
#define SRC_BLUEZ_HEXDUMP_HPP

#include <cstdint>
#include <cctype>
#include <iomanip>
#include <ostream>

class IosFlagSaver {
public:
  explicit IosFlagSaver(std::ostream& _ios) : ios(_ios), f(_ios.flags()) {}

  ~IosFlagSaver() { ios.flags(f); }

  IosFlagSaver(const IosFlagSaver& rhs) = delete;

  IosFlagSaver& operator=(const IosFlagSaver& rhs) = delete;

private:
  std::ostream& ios;
  std::ios::fmtflags f;
};

template <unsigned RowSize, bool ShowAscii>
struct CustomHexdump {
  CustomHexdump(const std::uint8_t* data, std::size_t length)
      : mData(data), mLength(length) {}

  const std::uint8_t* mData;
  const std::size_t mLength;
};

template <unsigned RowSize, bool ShowAscii>
std::ostream& operator<<(std::ostream& out,
                         const CustomHexdump<RowSize, ShowAscii>& dump) {
  IosFlagSaver ios_fs(out);

  out.fill('0');
  for (size_t i = 0; i < dump.mLength; i += RowSize) {
    out << "0x" << std::setw(6) << std::hex << i << ": ";
    for (size_t j = 0; j < RowSize; ++j) {
      if (i + j < dump.mLength) {
        out << std::hex << std::setw(2) << static_cast<int>(dump.mData[i + j])
            << " ";
      } else {
        out << "   ";
      }
    }

    out << " ";
    if (ShowAscii) {
      for (size_t j = 0; j < RowSize; ++j) {
        if (i + j < dump.mLength) {
          if (std::isprint(dump.mData[i + j])) {
            out << static_cast<char>(dump.mData[i + j]);
          } else {
            out << ".";
          }
        }
      }
    }
    out << std::endl;
  }

  return out;
}

typedef CustomHexdump<16, true> Hexdump;

#endif  // SRC_BLUEZ_HEXDUMP_HPP