from pathlib import Path

# 读取 YAML 内容
import yaml
with open("spec/flit_structs.yaml", "r") as f:
    structs = yaml.safe_load(f)

# 生成 SystemVerilog 和 C++ 的定义
def sv_width_decl(width):
    return f"[{width - 1}:0] " if width > 1 else ""

def cpp_type(width):
    if width <= 8:
        return "uint8_t"
    elif width <= 16:
        return "uint16_t"
    elif width <= 32:
        return "uint32_t"
    elif width <= 64:
        return "uint64_t"
    else:
        return f"std::bitset<{width}>"

sv_lines = []
cpp_lines = ['#pragma once\n#include <cstdint>\n#include <bitset>\n\n']

for struct_name, fields in structs.items():
    # SystemVerilog struct
    sv_lines.append(f"typedef struct packed {{")
    for field in fields:
        name = field["name"]
        width = field["width"]
        sv_lines.append(f"    logic {sv_width_decl(width)}{name};")
    sv_lines.append(f"}} {struct_name};\n")

    # C++ struct
    cpp_lines.append(f"struct {struct_name} {{")
    for field in fields:
        name = field["name"]
        width = field["width"]
        cpp_lines.append(f"    {cpp_type(width)} {name};")
    cpp_lines.append("};\n")

# 写入文件
sv_path = Path("spec/generated_flit_structs.sv")
cpp_path = Path("spec/generated_flit_structs.hpp")

sv_path.write_text("\n".join(sv_lines))
cpp_path.write_text("\n".join(cpp_lines))
