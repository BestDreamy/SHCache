from pathlib import Path
import json

def load_json(file_path):
    with open(file_path, "r") as f:
        return json.load(f)

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

def cpp_init_expr(width):
    if width > 64:
        return f"std::bitset<{width}>()"
    else:
        return f"{cpp_type(width)}()"

if __name__ == "__main__":
    config = load_json(Path("config/spec/flit.json"))

    sv_lines = []
    cpp_lines = ['#pragma once\n#include <cstdint>\n#include <bitset>\n\n']

    for struct_name, fields in config.items():
        flit_width = 0

        # sv struct
        sv_lines.append(f"typedef struct packed {{")
        for field in fields:
            name = field["name"]
            width = field["width"]
            sv_lines.append(f"    logic {sv_width_decl(width)}{name};")
            flit_width += width
        sv_lines.append(f"}} {struct_name};\n")
        sv_lines.append(f"parameter int {struct_name[:-2]}_width = {flit_width};\n")
        for field in fields:
            name = field["name"]
            width = field["width"]
            sv_lines.append(f"parameter int {struct_name[:-2]}_{name}_width = {width};")
        sv_lines.append("\n")

        # C++ struct
        cpp_lines.append(f"struct {struct_name} {{")
        for field in fields:
            name = field["name"]
            width = field["width"]
            cpp_lines.append(f"    {cpp_type(width)} {name};")

        # default constructor
        if fields:
            cpp_lines.append(f"    {struct_name}() noexcept")
            cpp_lines.append("        : " + ",\n          ".join(
                [f"{field['name']}({cpp_init_expr(field['width'])})" for field in fields]
            ) + " {}")
        else:
            cpp_lines.append(f"    {struct_name}() {{}}")

        # copy constructor
        cpp_lines.append(f"    {struct_name}(const {struct_name}& other) noexcept {{")
        for field in fields:
            name = field["name"]
            cpp_lines.append(f"        {name} = other.{name};")
        cpp_lines.append("    }")

        # copy assignment operator
        cpp_lines.append(f"    {struct_name}& operator=(const {struct_name}& other) noexcept {{")
        cpp_lines.append("        if (this != &other) {")
        for field in fields:
            name = field["name"]
            cpp_lines.append(f"            {name} = other.{name};")
        cpp_lines.append("        }")
        cpp_lines.append("        return *this;")
        cpp_lines.append("    }")

        cpp_lines.append("};\n")
        # C++ struct total width
        cpp_lines.append(f"constexpr size_t {struct_name[:-2]}_width = {flit_width};\n")
        # C++ field width
        for field in fields:
            name = field["name"]
            width = field["width"]
            cpp_lines.append(f"constexpr size_t {struct_name[:-2]}_{name}_width = {width};")
        cpp_lines.append("\n")

    sv_path = Path("vsrc/chi/auto_chi_flit.vh")
    cpp_path = Path("csrc/chi/flit/auto_flit.h")

    sv_path.write_text("\n".join(sv_lines))
    cpp_path.write_text("\n".join(cpp_lines))