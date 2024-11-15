#pragma once

#include <string>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdint>
#include <cassert>

namespace bytehamster::util {
static std::string prettyBytes(size_t bytes) {
    const char* suffixes[7];
    suffixes[0] = " B";
    suffixes[1] = " KB";
    suffixes[2] = " MB";
    suffixes[3] = " GB";
    suffixes[4] = " TB";
    suffixes[5] = " PB";
    suffixes[6] = " EB";
    uint s = 0; // which suffix to use
    double count = bytes;
    while (count >= 1024 && s < 7) {
        s++;
        count /= 1024;
    }
    return std::to_string(count) + suffixes[s];
}

size_t filesize(int fd) {
    struct stat st = {};
    if (fstat(fd, &st) < 0) {
        assert(false);
    }
    if (S_ISBLK(st.st_mode)) {
        uint64_t bytes;
        if (ioctl(fd, BLKGETSIZE64, &bytes) != 0) {
            assert(false);
        }
        return bytes;
    } else if (S_ISREG(st.st_mode)) {
        return st.st_size;
    }
    return 0;
}

size_t filesize(std::string &filename) {
    int fd = open(filename.c_str(), O_RDONLY);
    size_t size = filesize(fd);
    close(fd);
    return size;
}

} // Namespace util
