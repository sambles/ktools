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

#include "filetool.h"

void print_help() {
    std::cout << "-h\t\tThis message" << std::endl
              << "-p \t\tPipe mode (read from stdin. Mutually exclusive with -r)" << std::endl
			  << "-t " << ktools::filetool::FileTool::get_supported_filetypes() << std::endl
			  << "-o [bin|csv]\tOutput format" << std::endl
		      << "-i [bin|csv]\tInput format" << std::endl
              << "-r <path>\tPath to the file repo (mutually exclusive with -p)" << std::endl
              ;
}

int main(int argc, char **argv)
{
	ktools::filetool::FileTool::Format input = ktools::filetool::FileTool::FMT_UNKNOWN;
	ktools::filetool::FileTool::Format output = ktools::filetool::FileTool::FMT_UNKNOWN;
    ktools::filetool::FileTool::FileType file_type = ktools::filetool::FileTool::FT_UNKOWN; 
    std::string prefix = "";
    bool pipe_mode = false;

  	int opt;	
	while ((opt = getopt(argc, argv, "pho:si:sr:st:s")) != -1) {
		switch (opt) {
            case 'o':
                output = ktools::filetool::FileTool::convert_format(optarg);
				break;
			case 'i':
				input = ktools::filetool::FileTool::convert_format(optarg);
				break;
			case 'r':
                prefix = optarg;
                break;
            case 't':
                file_type = ktools::filetool::FileTool::convert_filetype(optarg);
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

	if (output == ktools::filetool::FileTool::FMT_UNKNOWN) {
		std::cerr << "Invalid/missing output format" << std::endl;
		exit(1);
	}

	if (input == ktools::filetool::FileTool::FMT_UNKNOWN) {
		std::cerr << "Invalid/missing input format" << std::endl;
		exit(1);
	}

	if (file_type == ktools::filetool::FileTool::FT_UNKOWN) {
		std::cerr << "Invalid/missing file type" << std::endl;
		exit(1);
	}

	if (pipe_mode) {
		prefix = "-";
	}

	ktools::filetool::FileTool filetool(prefix, input, output, file_type);
	filetool.run();
}

