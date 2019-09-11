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
#if defined(_MSC_VER)
#include "../wingetopt/wingetopt.h"
#else
#include <unistd.h>
#endif
#include <iostream>
#include <string>

#include "ItemsFile.h"
#include "VulnerabilitiesFile.h"
#include "DamageBinDictionaryFile.h"
#include "CoveragesFile.h"
#include "FootprintFile.h"
#include "FootprintIndexFile.h"
#include "FmPolicyTCFile.h"

enum Format
{
	FMT_UNKNOWN,
	FMT_CSV,
	FMT_BIN,
	FMT_JSON
};

enum FileType
{
    FT_UNKOWN,
    FT_EVENTS,
    FT_VULNERABILITIES,
    FT_DAMAGEBINDICT,
    FT_COVERAGES,
    FT_FOOTPRINT,
    FT_FOOTPRINT_INDEX,
	FT_FM_POLICY_TC
};

bool equals(const std::string& a, const std::string& b)
{
	unsigned int sz = a.size();
	if (b.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}

Format parse_format(const std::string& format, const std::string& direction) {
    if (equals("csv", format) == 0) {
        return FMT_CSV;
	} else if (equals("bin", format) == 0) {
		return FMT_BIN;
	} else if (equals("json", format) == 0) {
        return FMT_JSON;                
    } else {
        std::cerr << "Unknown " << direction << " format: " << optarg << std::endl;
        exit(1);
    }
}

FileType parse_filetype(const std::string& filetype)
{
    if (equals(filetype, "events") == 0) {
        return FT_EVENTS;
    } else if (equals(filetype, "vulnerabilities") == 0) {
        return FT_VULNERABILITIES;
    } else if (equals(filetype, "damagebindict") == 0) {
        return FT_DAMAGEBINDICT;
    } else if (equals(filetype, "coverages") == 0) {
        return FT_COVERAGES;
    } else if (equals(filetype, "footprint") == 0) {
        return FT_FOOTPRINT;
	} else if (equals(filetype, "footprintidx") == 0) {
		return FT_FOOTPRINT_INDEX;
	} else if (equals(filetype, "fmpolicytc") == 0) {
		return FT_FM_POLICY_TC;
	} else {
        std::cerr << "Unknown file type: " << filetype << std::endl;
        exit(1);
    }
}

template<typename T>
void read_and_print_csv(ktools::filetool::BaseFileReader<T>& reader) {
    ktools::filetool::CsvFormatter<T> formatter;

    std::cout << formatter.header() << std::endl;
    T rec;
    while (reader.read(rec)) {
        std::cout << formatter.row(rec) << std::endl;
    }
	std::cout.flush();
}

template<typename T>
void read_and_print_bin(ktools::filetool::BaseFileReader<T>& reader) {
	T rec;
	while (reader.read(rec)) {
		std::cout.write(reinterpret_cast<const char *>(&rec), sizeof(rec));
	}
	std::cout.flush();
}

template<typename T>
void read_and_print(ktools::filetool::BaseFileReader<T>& reader, Format output) {
    switch (output) {
        case FMT_CSV:
            read_and_print_csv(reader);
            break;
		case FMT_BIN:
			read_and_print_bin(reader);
			break;
		default:
            std::cerr << "Unhandled output type specified" << std::endl;
    }
}

void print_help() {
    std::cout << "-h\t\tThis message" << std::endl
              << "-p \t\tPipe mode (read from stdin. Mutually exclusive with -r)" << std::endl
  	          << "-o [bin|csv]\tOutput format" << std::endl
		      << "-i [bin|csv]\tInput format" << std::endl
              << "-r <path>\tPath to the file repo (mutually exclusive with -p)" << std::endl
              << "-t [events|vulnerabilities|damagebindict|coverages|footprint|footprintidx]\tType of data to process" << std::endl
              ;
}

int main(int argc, char **argv)
{
    Format output = FMT_UNKNOWN;
	Format input = FMT_UNKNOWN;
    FileType file_type = FT_UNKOWN;
    std::string prefix = "";
    bool pipe_mode = false;

  	int opt;	
	while ((opt = getopt(argc, argv, "pho:si:sr:st:s")) != -1) {
		switch (opt) {
            case 'o':
                output = parse_format(optarg, "output");
                break;
			case 'i':
				input = parse_format(optarg, "input");
				break;
			case 'r':
                prefix = optarg;
                break;
            case 't':
                file_type = parse_filetype(optarg);
                break;
            case 'p':
                std::cout << "pipe mode" << std::endl;
                pipe_mode = true;
                break;
            case 'h':
            default:
                print_help();
                exit(0);
        }
    }

    if (output == FMT_UNKNOWN) {
        std::cerr << "No output type given" << std::endl;
        exit (1);
    }

	if (input == FMT_UNKNOWN) {
		std::cerr << "No input type given" << std::endl;
		exit(1);
	}

    if (file_type == FT_UNKOWN) {
        std::cerr << "No file type given" << std::endl;
        exit (1);
    }

    switch(file_type) {
        case FT_EVENTS: {
            ktools::filetool::ItemsFile file(prefix);
            read_and_print(file, output);
        }
        break;
        case FT_VULNERABILITIES: {
            ktools::filetool::VulnerabilitiesFile file(prefix);
            read_and_print(file, output);
        }
        break;
        case FT_DAMAGEBINDICT: {
            ktools::filetool::DamageBinDictionaryFile file(prefix);
            read_and_print(file, output);
        }
        break;
        case FT_COVERAGES: {
            ktools::filetool::CoveragesFile file(prefix);
            read_and_print(file, output);
        }
        break;
        case FT_FOOTPRINT_INDEX: {
            ktools::filetool::FootprintIndexFile file(prefix, false);
            read_and_print(file, output);
        }
        break;
        case FT_FOOTPRINT: {
            ktools::filetool::FootprintIndexFile index_file(prefix, false);
            ktools::filetool::FootprintFile file(prefix, false);

            EventIndex index;
            while (index_file.read(index)) {
                if (index.size == 0) {
                    continue;
                }
                file.init(index);
                read_and_print(file, output);
            }
        }
        break;
		case FT_FM_POLICY_TC: {
			ktools::filetool::FMPolicyTCFile file(prefix);
			read_and_print(file, output);
		}
		break;
    }
}

