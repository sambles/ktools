/*
 * Copyright (c)2015 - 2019 Oasis LMF Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 *   * Neither the original author of this software nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
#pragma once

#include "BaseFileReader.h"

namespace ktools {
namespace filetool {

template <typename T>
class BinFileReader : public BaseFileReader<T> {
public:
    BinFileReader(const std::string& prefix)
        : BaseFileReader<T>(prefixed_path(
              prefix, FileReaderSpecialization<T>::bin_filename())) {}

    virtual ~BinFileReader() {}

public:
    uint32_t num_records() {
        BaseFileReader<T>::_stream.seekg(0, BaseFileReader<T>::_stream.end);
        std::streamoff sz = BaseFileReader<T>::_stream.tellg();
        BaseFileReader<T>::_stream.seekg(0, BaseFileReader<T>::_stream.beg);

        return static_cast<uint32_t>(sz / record_size());
    }

    virtual bool read(T& rec) {
        BaseFileReader<T>::_stream.read(reinterpret_cast<char*>(&rec),
                                        sizeof(T));

        bool fail = !BaseFileReader<T>::_stream;

        return !fail;
    }

protected:
    virtual int record_size() const { return sizeof(T); }

private:
    std::string prefixed_path(const std::string& prefix,
                              const std::string& filename) const {
        if (!prefix.empty()) {
            return prefix + "/" + filename;
        }

        return filename;
    }
};

}  // namespace filetool
}  // namespace ktools