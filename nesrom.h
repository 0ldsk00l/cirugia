#pragma once

class header {
private:
    int prgsize;
    int chrsize;
    int mapper;
    int submapper;
    int mirroring;
    bool battery;
    bool trainer;
    int systype;
    int version;
    int prgramsize;
    int prgnvramsize;
    int chrramsize;
    int chrnvramsize;
    int tvsystem;
    int timing;
    int vsppu;
    int vshardware;
    int extsys;
    int miscroms;
    int expdev;

public:
    enum { MIRRORING_H, MIRRORING_V, MIRRORING_4 };

    int get_version() { return version; }
    void set_version(int ver) { version = (ver > 0 && ver <= 2) ? ver : 1; }

    int get_prgsize() { return prgsize; }
    void set_prgsize(int sz) { prgsize = sz; }

    int get_prgramsize() { return prgramsize; }
    void set_prgramsize(int sz) { prgramsize = sz; }

    int get_prgnvramsize() { return prgnvramsize; }
    void set_prgnvramsize(int sz) { prgnvramsize = sz; }

    int get_chrsize() { return chrsize; }
    void set_chrsize(int sz) { chrsize = sz; }

    int get_chrramsize() { return chrramsize; }
    void set_chrramsize(int sz) { chrramsize = sz; }

    int get_chrnvramsize() { return chrnvramsize; }
    void set_chrnvramsize(int sz) { chrnvramsize = sz; }

    int get_mapper() { return mapper; }
    void set_mapper(int mpr) { if (mpr < 768) mapper = mpr; }

    int get_submapper() { return submapper; }
    void set_submapper(int smpr) { submapper = smpr & 0x0f; }

    int get_mirroring() { return mirroring; }
    void set_mirroring(int mr) { if (mr <= MIRRORING_4) mirroring = mr; }

    bool get_battery() { return battery; }
    void set_battery(bool bt) { battery = bt; }

    bool get_trainer() { return trainer; }
    void set_trainer(bool tr) { trainer = tr; }

    int get_systype() { return systype; }
    void set_systype(int s) { systype = s & 0x03; }

    int get_tvsystem() { return tvsystem; }
    void set_tvsystem(int t) { tvsystem = t; }

    int get_timing() { return timing; }
    void set_timing(int t) { timing = t; }

    int get_vsppu() { return vsppu; }
    void set_vsppu(int v) { vsppu = v; }

    int get_vshardware() { return vshardware; }
    void set_vshardware(int v) { vshardware = v; }

    int get_extsys() { return extsys; }
    void set_extsys(int e) { extsys = e; }

    int get_miscroms() { return miscroms; }
    void set_miscroms(int m) { miscroms = m & 0x03; }

    int get_expdev() { return expdev; }
    void set_expdev(int e) { expdev = e; }

    friend class nesrom;
};

class nesrom {
private:
    header hdr;
    std::vector<uint8_t> prg;
    std::vector<uint8_t> chr;
    std::string filename;

    std::string sha;
    uint32_t crc;
    uint32_t prgcrc;
    uint32_t chrcrc;

public:
    nesrom() { }
    nesrom(std::string fn) { filename = fn; }

    bool load();
    void save();
    void save(std::string fn);

    header& get_header() { return hdr; }
    std::vector<uint8_t>& get_prg() { return prg; }
    std::vector<uint8_t>& get_chr() { return chr; }
    std::string& get_filename() { return filename; }

    std::string get_sha1() { return sha; }
    uint32_t get_crc() { return crc; }
    uint32_t get_prgcrc() { return prgcrc; }
    uint32_t get_chrcrc() { return chrcrc; }
};
