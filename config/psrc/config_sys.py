import json

def gen_cpp_config(config):
    lines = [
        "#ifndef CONFIG_H",
        "#define CONFIG_H",
        "#include <cstdint>",
        "",
        "struct Config {"
    ]

    for key, value in config.items():
        if key == '_comment':
            continue
        if isinstance(value, int):
            lines.append(f"    static constexpr uint32_t {key} = {value};")
        elif isinstance(value, list):
            length = len(value)
            arr_str = ', '.join(str(v) for v in value)
            lines.append(f"    static constexpr uint32_t {key}[{length}] = {{{arr_str}}};")
    lines.append("};\n")
    lines.append("extern Config config;\n#endif")
    return '\n'.join(lines)

def gen_sv_config(config):
    def to_hex_list(arr):
        return [f"'h{v:X}" for v in arr]
    
    lines = ["`ifndef CONFIG_H", "`define CONFIG_H", ""]
    for k, v in config.items():
        if k == '_comment':
            continue
        if isinstance(v, list):
            length = len(v)
            hex_vals = to_hex_list(v)
            lines.append(f"parameter int {k} [0:{length - 1}] = '{{" + ", ".join(hex_vals) + "};")
        else:
            lines.append(f"parameter int {k} = {v};")
    lines.append("\n`endif")
    return "\n".join(lines)

if __name__ == "__main__":
    with open("config/config.json", "r") as f:
        config = json.load(f)

    cpp_header = gen_cpp_config(config)
    verilog_header = gen_sv_config(config)

    with open("csrc/include/autoconfig.h", "w") as f:
        f.write(cpp_header)

    with open("vsrc/include/autoconfig.vh", "w") as f:
        f.write(verilog_header)

    print("Generated: config.h and config.vh")
