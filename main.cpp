#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

const std::string input_str = "Hello, World!";
const std::string expected = "SGVsbG8sIFdvcmxkIQ==";
constexpr std::array<char, 65> charmap = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '\\', '='
};

struct chunk {
  std::bitset<24> data;
  std::size_t padding = 0;
};

chunk to_chunk(const char byte_1, const char byte_2, const char byte_3);
std::vector<chunk> to_chunks(const std::string &s);
std::array<char, 4> encode(const chunk &c);

std::size_t get_index(const std::array<char, 65> &a, const char c) {
  return std::distance(a.begin(), std::find(a.begin(), a.end(), c));
}

int main(int argc, const char *argv[]) {
  std::vector<chunk> chunks;
  std::string output;

  if (argc < 2) {
    std::cout << "No input supplied! Running Test!\n";

    chunks = to_chunks(input_str);
    output = "";

    for (chunk c : chunks) {
      for (char e : encode(c)) {
        output += e;
      }
    }

    std::cout << "Input     : " << input_str << "\n";
    std::cout << "Expected  : " << expected << "\n\n";
    std::cout << "Output    : " << output << "\n\n";
    std::cout << "Matching? " << (output == expected ? "Yes" : "No") << "\n";
    return 0;
  }


  chunks = to_chunks(argv[1]);
  output = "";

  for (chunk c : chunks) {
    for (char e : encode(c)) {
      output += e;
    }
  }

  std::cout << output << "\n";

  return 0;
}

chunk to_chunk(const char byte_1, const char byte_2, const char byte_3) {
  chunk c;

  c.data |= byte_1;
  c.data <<= 8;
  c.data |= byte_2;
  c.data <<= 8;
  c.data |= byte_3;

  c.padding = (
    (byte_1 == '\0' ? 1 : 0) +
    (byte_2 == '\0' ? 1 : 0) +
    (byte_3 == '\0' ? 1 : 0)
  );

  return c;
}

std::vector<chunk> to_chunks(const std::string &s) {
  std::size_t char_count = 0;
  std::vector<chunk> chunks;

  std::array<char, 3> chars;

  for (char c : s) {
    chars[char_count] = c;
    ++char_count;

    if (char_count >= 3) {
      chunks.push_back(to_chunk(chars[0], chars[1], chars[2]));
      char_count = 0;
    }
  }

  if (char_count != 0) {
    while (char_count < 3) {
      chars[char_count] = '\0';
      ++char_count;
    }
  }

  if (char_count >= 3) {
    chunks.push_back(to_chunk(chars[0], chars[1], chars[2]));
    char_count = 0;
  }

  return chunks;
}

std::array<char, 4> encode(const chunk &c) {
  uint8_t c1 = (c.data.to_ulong() >> 18);
  uint8_t c2 = (c.data.to_ulong() >> 12) & 0b111111;
  uint8_t c3 = (c.data.to_ulong() >>  6) & 0b111111;
  uint8_t c4 = (c.data.to_ulong()      ) & 0b111111;

  switch (c.padding) {
    case 2: c3 = 64;
    case 1: c4 = 64;
  }

  return {charmap[c1], charmap[c2], charmap[c3], charmap[c4]};
}
