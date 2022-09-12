import json
import re

def hash_string( string ):
    try:
        hash = 5381

        for x in string.upper():
            hash = (( hash << 5 ) + hash ) + ord(x)

        return hash & 0xFFFFFFFF
    except:
        pass

def to_snake_case(fname):
    return re.sub(r'(?<!^)(?=[A-Z])', '_', fname).upper()

if __name__ in '__main__':
    in_file = open('data\\nt_data.json')
    out_file = open('src\\nt_functions.h', 'w')

    data = json.load(in_file)
    hash = hash_string("ntdll.dll")
    define = "#define NTDLL 0x{:x}\n\n".format(hash)
    out_file.write(define)

    for fname in data:
        hash = hash_string(fname)
        define = "#define {} 0x{:x}, {}\n".format(to_snake_case(fname), hash, data[fname])
        out_file.write(define)

    in_file.close()
    out_file.close()