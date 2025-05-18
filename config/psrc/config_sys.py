import os

# 配置生成函数
def generate_cpp_config(num_rns, num_hns, num_sns, 
                        credits_hn, credits_sn, 
                        node_rn, node_hn, node_sn,
                        rns_offset):
    cpp_code = f"""
#ifndef CONFIG_H
#define CONFIG_H
#include <vector>
#include <cstdint>

struct Config {{
    static constexpr uint32_t numRNs = 4;
    static constexpr uint32_t numHNs = 1;
    static constexpr uint32_t numSNs = 1;

    static constexpr uint32_t numCreditsForHNReq[1] = {{4}};
    static constexpr uint32_t numCreditsForSNReq[1] = {{4}};

    static constexpr uint32_t RNId[4] = {{0x1, 0x2, 0x3, 0x4}};
    static constexpr uint32_t HNId[1] = {{0x5}};
    static constexpr uint32_t SNId[1] = {{0x6}};

    static constexpr uint32_t RNsOffset[1] = {{4}};
}};

extern Config config;

#endif
"""
    return cpp_code


def generate_verilog_config(num_rns, num_hns, num_sns, 
                            credits_hn, credits_sn, 
                            node_rn, node_hn, node_sn,
                            rns_offset):
    def to_verilog_hex(value):
        return f"'h{value:x}"
    
    verilog_code = f"""
`ifndef CONFIG_H
`define CONFIG_H

parameter int numRNs = {num_rns};
parameter int numHNs = {num_hns};
parameter int numSNs = {num_sns};

parameter int numCreditsForHNReq [0:{len(credits_hn) - 1}] = '{{{', '.join(map(str, credits_hn))}}};
parameter int numCreditsForSNReq [0:{len(credits_sn) - 1}] = '{{{', '.join(map(str, credits_sn))}}};

parameter int RNId [0:{len(node_rn) - 1}] = '{{{', '.join(to_verilog_hex(x) for x in node_rn)}}};
parameter int HNId [0:{len(node_hn) - 1}] = '{{{', '.join(to_verilog_hex(x) for x in node_hn)}}};
parameter int SNId [0:{len(node_sn) - 1}] = '{{{', '.join(to_verilog_hex(x) for x in node_sn)}}};

parameter int RNsOffset [0:{len(rns_offset) - 1}] = '{{{', '.join(map(str, rns_offset))}}};
`endif
"""
    return verilog_code


def write_to_file(filepath, content):
    """将生成的代码写入文件"""
    os.makedirs(os.path.dirname(filepath), exist_ok=True)
    with open(filepath, 'w') as f:
        f.write(content)
    print(f"Generated file: {filepath}")


if __name__ == "__main__":
    num_rns = 4
    num_hns = 1
    num_sns = 1

    credits_hn = [4]
    credits_sn = [4]

    # rnid must encode from 0x1
    node_rn = [0x1, 0x2, 0x3, 0x4]
    rns_offset = [4] # each cache line has 16 bytes
    for id, it in enumerate(node_rn):
        assert(id + 1== it)
    # node_rn = [0x0C, 0x2C, 0x3C, 0x4C]
    node_hn = [0x5]              
    node_sn = [0x6]                    

    cpp_config = generate_cpp_config(num_rns, num_hns, num_sns, 
                                     credits_hn, credits_sn, 
                                     node_rn, node_hn, node_sn,
                                     rns_offset)
    cpp_filepath = "csrc/include/autoconfig.h"
    write_to_file(cpp_filepath, cpp_config)

    verilog_config = generate_verilog_config(num_rns, num_hns, num_sns, 
                                             credits_hn, credits_sn, 
                                             node_rn, node_hn, node_sn,
                                             rns_offset)
    verilog_filepath = "vsrc/include/autoconfig.v"
    write_to_file(verilog_filepath, verilog_config)