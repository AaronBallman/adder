#include <stdio.h>

struct one_bit {
  static const unsigned value = 1;
};

struct zero_bit {
  static const unsigned value = 0;
};

template <unsigned N, unsigned BN>
struct to_bin_helper {
  static const unsigned value = to_bin_helper<(N >> 1), BN - 1>::value;
};

template <unsigned N>
struct to_bin_helper<N, 0> : to_bin_helper<N & 1, 0>{};

template <>
struct to_bin_helper<1, 0> : one_bit{};

template <>
struct to_bin_helper<0, 0> : zero_bit{};

template <unsigned Num, unsigned Bit>
struct to_bin {
  static const unsigned value = to_bin_helper<Num, Bit>::value;
};

template <unsigned N>
struct pow2 {
  static const unsigned value = 2 * pow2<N - 1>::value;
};

template <>
struct pow2<0> {
  static const unsigned value = 1;
};

template <unsigned N, unsigned S, unsigned... Rest>
struct to_dec_helper {
  static const unsigned value = (S * pow2<N>::value) + to_dec_helper<N + 1, Rest...>::value;
};

template <unsigned N, unsigned S>
struct to_dec_helper<N, S> {
  static const unsigned value = S * pow2<N>::value;
};

template <unsigned Bit1, unsigned... BitN>
struct to_dec {
  static const unsigned value = to_dec_helper<0, Bit1, BitN...>::value;
};

template <unsigned A1, unsigned B1>
struct and_op : public zero_bit {};

template <>
struct and_op<1, 1> : public one_bit{};

template <unsigned A1, unsigned B1>
struct xor_op : public zero_bit {};

template <>
struct xor_op<1, 0> : public one_bit{};

template <>
struct xor_op<0, 1> : public one_bit{};

template <unsigned A1, unsigned B1, unsigned cin>
struct full_adder {
  static const unsigned sum = xor_op<xor_op<A1, B1>::value, cin>::value;
  static const unsigned carry = xor_op<and_op<xor_op<A1, B1>::value, cin>::value, and_op<A1, B1>::value>::value;
};

template <unsigned A0, unsigned A1, unsigned B0, unsigned B1, unsigned C0>
struct two_bit_adder {
private:
  static const unsigned c1 = full_adder<A0, B0, C0>::carry;

public:
  static const unsigned s0 = full_adder<A0, B0, C0>::sum;
  static const unsigned s1 = full_adder<A1, B1, c1>::sum;
  static const unsigned carry = full_adder<A1, B1, c1>::carry;
};

template <unsigned A3, unsigned A2, unsigned A1, unsigned A0,
  unsigned B3, unsigned B2, unsigned B1, unsigned B0,
  unsigned C0>
struct four_bit_adder_inner {
private:
  static const unsigned c1 = two_bit_adder<A0, A1, B0, B1, C0>::carry;

public:
  static const unsigned s0 = two_bit_adder<A0, A1, B0, B1, C0>::s0;
  static const unsigned s1 = two_bit_adder<A0, A1, B0, B1, C0>::s1;
  static const unsigned s2 = two_bit_adder<A2, A3, B2, B3, c1>::s0;
  static const unsigned s3 = two_bit_adder<A2, A3, B2, B3, c1>::s1;
  static const unsigned carry = two_bit_adder<A2, A3, B2, B3, c1>::carry;
};

template <unsigned A, unsigned B>
struct four_bit_adder {
  static const unsigned value = to_dec<four_bit_adder_inner<to_bin<A, 3>::value, to_bin<A, 2>::value, to_bin<A, 1>::value, to_bin<A, 0>::value,
                                                            to_bin<B, 3>::value, to_bin<B, 2>::value, to_bin<B, 1>::value, to_bin<B, 0>::value,
                                                            0>::s0,
                                       four_bit_adder_inner<to_bin<A, 3>::value, to_bin<A, 2>::value, to_bin<A, 1>::value, to_bin<A, 0>::value,
                                                            to_bin<B, 3>::value, to_bin<B, 2>::value, to_bin<B, 1>::value, to_bin<B, 0>::value,
                                                            0>::s1,
                                       four_bit_adder_inner<to_bin<A, 3>::value, to_bin<A, 2>::value, to_bin<A, 1>::value, to_bin<A, 0>::value,
                                                            to_bin<B, 3>::value, to_bin<B, 2>::value, to_bin<B, 1>::value, to_bin<B, 0>::value,
                                                            0>::s2,
                                       four_bit_adder_inner<to_bin<A, 3>::value, to_bin<A, 2>::value, to_bin<A, 1>::value, to_bin<A, 0>::value,
                                                            to_bin<B, 3>::value, to_bin<B, 2>::value, to_bin<B, 1>::value, to_bin<B, 0>::value,
                                                            0>::s3
                                       >::value;
};

int main(void) {
  static const unsigned ret = four_bit_adder<NUMBER_1, NUMBER_2>::value;
  printf("%d\n", ret);
  return 0;
}
