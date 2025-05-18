from pathlib import Path

import json

def load_json(file_path):
    with open(file_path, "r") as f:
        return json.load(f)

def sv_width_decl(width):
    return f"[{width - 1}:0] " if width > 1 else ""

def cpp_type(width):
    # if width <= 8:
    #     return "uint8_t"
    # elif width <= 16:
    #     return "uint16_t"
    # elif width <= 32:
    #     return "uint32_t"
    # elif width <= 64:
    #     return "uint64_t"
    # else:
        return f"std::bitset<{width}>"
    
if __name__ == "__main__":
    config = load_json(Path("config/spec/flit.json"))

    sv_lines = []
    cpp_lines = ['#pragma once\n#include <cstdint>\n#include <bitset>\n\n']

    for struct_name, fields in config.items():
        # sv struct
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

    sv_path = Path("vsrc/chi/auto_chi_flit.vh")
    cpp_path = Path("csrc/chi/auto_flit.h")

    sv_path.write_text("\n".join(sv_lines))
    cpp_path.write_text("\n".join(cpp_lines))
