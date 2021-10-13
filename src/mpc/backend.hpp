#pragma once

#include <bitset>
#include <vector> 
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <boost/dynamic_bitset.hpp>
#include <cstdint>
#include <fmt/core.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "common/utility/functions/io.hpp"
#include "mpc/frontend.hpp"
namespace mcc {

    class InstructionIndexMapping
    {
    public:
        bool has_key(std::string const& key)
        {
            return index_map.count(key);
        }
        int operator[](std::string const& key) const
        {
            return index_map.at(key);
        }

        const std::vector<std::tuple<std::string, int>> mapping =
        {
            {"R_W",0},
            {"PAM",1},
            {"ALU_OP0",2},
            {"ALU_OP1",3},
            {"ALU_OP2",4},
            {"M_DBUS",5},
            {"P1",6},
            {"P2",7},

            {"IR_O",8},
            {"PC_O",9},
            {"RS_O",10},
            {"ALU_O",11},
            {"DR_O",12},
            {"RD_O",13},
            {"ER_O",14},

            {"DR_I",15},

            {"IR_I",16},
            {"PC_I",17},
            {"RS_I",18 },
            {"ALU_I0",19 },
            {"ALU_I1", 20},
            {"AR_I",21},
            {"RD_I",22},
            {"ER_I",23},

            {"INT_IR", 24},
            {"RAM_ROM", 25},
            {"PC1_DBUS", 26},
            {"J0", 27},
            {"J1", 28},
            {"J2", 29},
            {"J3", 30},
            {"ABS_REL", 31},
        };

        InstructionIndexMapping()
        {
            for (auto&& [key, val] : mapping) {
                index_map[key] = val;
            }
        }
    private:
        std::map<std::string, int> index_map;

    };

    class PartitionedBitset
    {
        int parts;
        int bits_per_part;
        std::vector<boost::dynamic_bitset<uint8_t>> bitsets;

    public:
        PartitionedBitset(int parts, int bits_per_part)
            :parts(parts), bits_per_part(bits_per_part)
        {
            for (int i = 0;i < parts; i++) {
                bitsets.push_back(boost::dynamic_bitset<uint8_t>(bits_per_part));
            }
        }

        int total_bits()
        {
            return parts * bits_per_part;
        }

        void set_bit(int index, int val = 1)
        {
            bitsets[index / bits_per_part][index % bits_per_part] = val != 0;
        }

        int operator[](int index)
        {
            return bitsets[index / bits_per_part][index % bits_per_part];
        }

        void reset_all()
        {
            for (auto&& bitset : bitsets) {
                bitset.reset();
            }
        }

        auto to_bytes() -> std::vector<std::vector<uint8_t>>
        {
            std::vector<std::vector<uint8_t>> bytesets;
            for (int i = 0; i < parts; i++) {
                std::vector<uint8_t> byteset = to_bytes(bitsets[i]);
                bytesets.push_back(byteset);
            }
            return bytesets;
        }

    private:
        std::vector<uint8_t> to_bytes(boost::dynamic_bitset<uint8_t> bitset)
        {
            std::vector<uint8_t> bytes;
            boost::to_block_range(bitset, std::back_inserter(bytes));
            return bytes;
        }
    };

    struct SyntaxError
    {
        String why;

        SyntaxError(String why = "")
        {
            this->why = why;
        }

        String what() const
        {
            return why;
        }
    };

    class MyComputerBackend
    {
    public:
        static constexpr int parts = 4;
        static constexpr int bits_per_part = 8;

        MyComputerBackend(std::string output_dir)
            : bitset(parts, bits_per_part), bitset_empty(parts, bits_per_part)
        {
            // init files
            namespace fs = std::filesystem;
            fs::path base = output_dir;
            for (int i = 0; i < parts; i++) {
                fs::path p = base / (std::to_string(i) + ".bin"); 
                files.emplace_back(p.string());
            }

            // init manifest_file
            manifest_file.open((base / "manifest.txt").string());
        }

        void write_output(std::vector<mcc::Command> commands)
        {
            write_manifest_header();
            int row = 0;
            for (auto&& cmd : commands) {
                // default flags: rjmp=1
                bitset.set_bit(mapping["ABS_REL"], 1);
                // foreach property
                for (auto&& prop : cmd.props) {
                    if (prop.key == "ajmp") {
                        int to = string_to_int(prop.value);   
                        int j[4];
                        for (int i = 0; i < 4; i++) {
                            j[i] = to % 2;
                            to /= 2;
                        }
                        bitset.set_bit(mapping["J0"], j[0]);
                        bitset.set_bit(mapping["J1"], j[1]);
                        bitset.set_bit(mapping["J2"], j[2]);
                        bitset.set_bit(mapping["J3"], j[3]);
                        bitset.set_bit(mapping["ABS_REL"], 0);
                    } else if (prop.key == "rjmp") {
                        int to = string_to_int(prop.value);   
                        int j[4];
                        for (int i = 0; i < 4; i++) {
                            j[i] = to % 2;
                            to /= 2;
                        }
                        bitset.set_bit(mapping["J0"], j[0]);
                        bitset.set_bit(mapping["J1"], j[1]);
                        bitset.set_bit(mapping["J2"], j[2]);
                        bitset.set_bit(mapping["J3"], j[3]);
                        bitset.set_bit(mapping["ABS_REL"], 1);
                    }
                }
                // foreach flag
                for (auto&& flag : cmd.flags) {
                    if (mapping.has_key(flag)) {
                        bitset.set_bit(mapping[flag]);
                    } else if (flag == "pass") {
                        if (cmd.flags.size() != 1) {
                            throw SyntaxError("Cannot specify other flags when `pass` is specified");
                        }
                        // do nothing when `pass` specified
                    } else {
                        throw SyntaxError("Unknown flag `" + flag + "`.");
                    }
                }
                if (cmd.row < row) {
                    throw SyntaxError("Row number should be monotonically increasing.");
                }
                while (cmd.row > row) {
                    write_empty_command_to_file();
                    row++;
                }
                //  cmd.row == row  
                write_to_file();
                write_manifest(row);
                reset_bits();
                row++;
            }
        }
    private:
        std::vector<OutputFile> files;
        OutputFile manifest_file;
        InstructionIndexMapping mapping;
        PartitionedBitset bitset;
        PartitionedBitset bitset_empty;

        void reset_bits()
        {
            bitset.reset_all();
        }

        void write_empty_command_to_file()
        {
            std::vector<std::vector<unsigned char>> buffers = bitset_empty.to_bytes();
            for (int i = 0; i < parts; i++) {
                files[i].write(buffers[i]);
            }
        }

        void write_to_file()
        {
            std::vector<std::vector<unsigned char>> buffers = bitset.to_bytes();
            for (int i = 0; i < parts; i++) {
                files[i].write(buffers[i]);
            }
        }

        void write_manifest_header()
        {
            for (auto&& [name, index] : mapping.mapping) {
                manifest_file.print("{}: {}\n", name, index);// << name << ": " << index << "\n";
            }
            manifest_file.print("\n");
        }

        void write_manifest(int row)
        {
            int begin = row * bits_per_part / 8;
            int end = (row + 1) * bits_per_part / 8;
            manifest_file.print("row: {} (0x{:X}), begin: 0x{:X}, end: 0x{:X}\n",
                row, row, begin, end);// << fmt::format("row: {} (0x{:X}), begin: 0x{:X}, end: 0x{:X}\n", row, row, begin, end);

            for (int i = 0; i < bitset.total_bits(); i++) {
                char ch = bitset[i] == 0 ? '.' : 'X';
                manifest_file.print("[{:2d}] {} ", i, ch); 
                if (i % 8 == 7) {
                    manifest_file.print("\n"); 
                }
            }
            manifest_file.print("\n");
        }
    };
}
