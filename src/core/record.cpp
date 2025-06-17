#include "core/record.h"

void Record::serialize(std::ostream& out) const {
    size_t klen = key.size();
    out.write(reinterpret_cast<const char*>(&klen), sizeof(klen));
    out.write(key.data(), klen);

    size_t vlen = value.size();
    out.write(reinterpret_cast<const char*>(&vlen), sizeof(vlen));
    out.write(value.data(), vlen);

    out.write(reinterpret_cast<const char*>(&timestamp), sizeof(timestamp));
    out.write(reinterpret_cast<const char*>(&deleted), sizeof(deleted));
}

Record Record::deserialize(std::istream& in) {
    Record r;
    size_t klen, vlen;

    in.read(reinterpret_cast<char*>(&klen), sizeof(klen));
    if (in.gcount() == 0) return r;
    r.key.resize(klen);
    in.read(&r.key[0], klen);

    in.read(reinterpret_cast<char*>(&vlen), sizeof(vlen));
    r.value.resize(vlen);
    in.read(&r.value[0], vlen);

    in.read(reinterpret_cast<char*>(&r.timestamp), sizeof(r.timestamp));
    in.read(reinterpret_cast<char*>(&r.deleted), sizeof(r.deleted));
    
    return r;
}