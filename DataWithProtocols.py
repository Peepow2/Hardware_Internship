def Check_Sum(data_bytes):
    checksum = 0x00
    for b in data_bytes:
        checksum ^= b
    return checksum

def Made_Protocols(pack_id, DATA):
    STX = 2 # 0x02
    length = len(DATA)
    len_bh = (length >> 8) & 0xFF
    len_bl = length & 0xFF  # สนใจข้อมูลไบต์เดียว
    checksum = Check_Sum([pack_id, len_bh, len_bl] + DATA)
    EOT = 4 # 0x04
    
    return str(bytes([STX, pack_id, len_bh, len_bl] + DATA + [checksum, EOT]))

Data_Sent = Made_Protocols(0x01, [1, 2, 3])
# str(b'\x02\x01\x00\x03\x01\x02\x03\x02\x04')
print(Valid_protocols(Data_Sent))
