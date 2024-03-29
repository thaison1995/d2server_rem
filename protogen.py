#!/bin/python
import os, re

global_enums = dict()
global_structs = list()

def emit_struct(packet_name, packet_code, members):
    global global_structs
    global_structs.append((packet_name, packet_code, members))

def emit_enum(enum_name, enum_type, values):
    global global_enums
    global_enums[enum_name] = (enum_name, enum_type, values)

def gen_code(input, output, nbo=False):
    global global_structs
    global global_enums
    
    global_enums = dict()
    global_structs = list()

    parse_proto(input)

    f = open(output, 'w')
    f.write('#ifndef INCLUDED_D2CS_PROTO_HPP\n')
    f.write('#define INCLUDED_D2CS_PROTO_HPP\n\n')
    f.write('#include <string>\n')
    f.write('#include <sstream>\n')
    f.write('#include <WinSock2.h>\n\n')
    f.write('#include "net/bn_types.h"\n\n')
    f.write('using string = std::string;\n\n')
    for enum_name in global_enums:
        enum_name, enum_type, values = global_enums[enum_name]
        f.write("enum PROTO_%s: %s {\n" % (enum_name, enum_type))
        for name, val in values:
            f.write("  %s = %d,\n" % (name, val))
        f.write("};\n\n");

    for packet_name, packet_code, members in global_structs:
        members_c = ''
        read_c = ''
        write_c = ''
        for member_type, member_name, member_suffix in members:
            if member_type in global_enums:
                member_type = 'PROTO_%s' % member_type
            if member_suffix is not None and member_suffix[0] == 'array':
                if member_type == 'charsave':
                    members_c += '  std::string %s;\n' % (member_name)
                    read_c  += '    %s.resize(%s);\n' % (member_name, member_suffix[1])
                    read_c  += '    if (%s > 0) iss.read((char*)&%s.front(), sizeof(char) * %s);\n' % (member_suffix[1], member_name, member_suffix[1])
                    write_c += '    if (%s > 0) oss.write((char*)&%s.front(), sizeof(char) * %s);\n' % (member_suffix[1], member_name, member_suffix[1])
                else:
                    members_c += '  %s %s[%s];\n' % (member_type, member_name, member_suffix[1])
                    read_c  += '    iss.read((char*)&%s[0], sizeof(%s) * %s);\n' % (member_name, member_type, member_suffix[1])
                    write_c += '    oss.write((char*)&%s[0], sizeof(%s) * %s);\n' % (member_name, member_type, member_suffix[1])
            else:
                members_c += '  %s %s;\n' % (member_type, member_name)
                if member_type == 'string':
                    read_c  += "    std::getline(iss, %s, '\\0');\n" % member_name
                    write_c += '    oss << %s;\n' % member_name
                    write_c += '    oss.put((char)0);\n'
                else:
                    if nbo and (member_type == 'bn_short' or member_type == 'bn_int'):
                        ts = 's' if member_type == 'bn_short' else 'l'
                        read_c  += '    %s nbo_%s;\n' % (member_type, member_name)
                        read_c  += '    iss.read((char*)&nbo_%s, sizeof(%s));\n' % (member_name, member_type)
                        read_c  += '    %s = ntoh%s(nbo_%s);\n' % (member_name, ts, member_name)
                        write_c += '    %s nbo_%s = hton%s(%s);\n' % (member_type, member_name, ts, member_name)
                        write_c += '    oss.write((char*)&nbo_%s, sizeof(%s));\n' % (member_name, member_type)
                    else:
                        read_c  += '    iss.read((char*)&%s, sizeof(%s));\n' % (member_name, member_type)
                        write_c += '    oss.write((char*)&%s, sizeof(%s));\n' % (member_name, member_type)
                    
        methods_c = ''
        methods_c += "  void ReadFromString(std::string& s) {\n";
        methods_c += "    std::istringstream iss(s);\n";
        methods_c += read_c;
        methods_c += "  }\n";
        methods_c += "  std::string WriteAsString() {\n";
        methods_c += "    h.type = %d;\n" % (packet_code)
        methods_c += "    std::ostringstream oss;\n";
        methods_c += write_c;
        methods_c += "    h.size = (bn_short)oss.tellp();\n"
        methods_c += "    oss.seekp(0);\n"
        methods_c += "    oss.write((char*)&h.size, sizeof(bn_int));\n"
        methods_c += "    return oss.str();\n";
        methods_c += "  }\n";
        f.write("""#define %s_typecode %s
struct %s
{
%s
%s};\n\n""" % (packet_name, packet_code, packet_name, members_c, methods_c))
    f.write('#endif\n')

def parse_proto(input):
    STATE_EXPECT_STRUCT = 1
    STATE_EXPECT_MEMBER = 2
    STATE_EXPECT_ENUM_VALUE = 3

    state = STATE_EXPECT_STRUCT

    with open(input, 'r') as f:
        for line in f:
            if not line.strip():
                continue
            cols = re.split(r'\s+', line.strip())
            if state == STATE_EXPECT_STRUCT:
                if cols[0] == 'enum':
                    enum_name, enum_type = cols[1], cols[2]
                    enum_values = list()
                    state = STATE_EXPECT_ENUM_VALUE
                else:
                    packet_name, packet_code = cols[0], int(cols[1], 0)
                    state = STATE_EXPECT_MEMBER
                    members = list()
            elif state == STATE_EXPECT_MEMBER:
                if cols[0] == 'end':
                    emit_struct(packet_name, packet_code, members)
                    state = STATE_EXPECT_STRUCT
                else:
                    member_type, member_name = cols[0], cols[1]
                    member_suffix = None
                    if member_name.endswith(';'):
                        member_name = member_name.strip(';')
                    if member_name.find('[') > -1:
                        member_name, member_suffix = member_name.split('[')
                        member_suffix = ('array', member_suffix.split(']')[0])
                    members.append((member_type, member_name, member_suffix))
            elif state == STATE_EXPECT_ENUM_VALUE:
                if cols[0] == 'end':
                    emit_enum(enum_name, enum_type, enum_values)
                    state = STATE_EXPECT_STRUCT
                else:
                    enum_value = (cols[0], int(cols[1], 0))
                    enum_values.append(enum_value)

gen_code('d2cs_proto.def', 'generated/d2cs_proto.hpp')
gen_code('d2dbs_proto.def', 'generated/d2dbs_proto.hpp')