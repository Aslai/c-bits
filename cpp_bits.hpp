// This is free and unencumbered software released into the public domain.

#ifndef CPP_BITS_H
#define CPP_BITS_H

#include <array> 

// Modeled after http://stackoverflow.com/a/13294458/670358

namespace {
    template<size_t... I> 
    struct seq{};
    template<size_t N, size_t... I>
    struct gen_seq : gen_seq<N-1, N*8-8, I...>{};
    template<size_t... I>
    struct gen_seq<0, I...> : seq<I...>{};

    constexpr unsigned char bin_str_coalesce(const char * data, size_t offset){
        return  ((data[offset    ] == '1') << 7) | 
                ((data[offset + 1] == '1') << 6) | 
                ((data[offset + 2] == '1') << 5) | 
                ((data[offset + 3] == '1') << 4) | 
                ((data[offset + 4] == '1') << 3) | 
                ((data[offset + 5] == '1') << 2) | 
                ((data[offset + 6] == '1') << 1) |
                ((data[offset + 7] == '1') << 0);
                
    }

    template<size_t N, size_t... I>
    constexpr std::array<unsigned char const, (N - 1) / 8> bin_str(char const (&a)[N], seq<I...>){
        return {{ bin_str_coalesce(a, I)... }};
    }
}

template<size_t N>
constexpr std::array<unsigned char const, (N - 1) / 8> bin_str(char const (&a)[N]){
    static_assert((N - 1) % 8 == 0, "Must pass in a multiple of 8 bits!");
    return bin_str(a, gen_seq<(N - 1) / 8>{});
}

#endif